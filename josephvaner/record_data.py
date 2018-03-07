#!/usr/bin/env python

import obd_io
import serial
import platform
import obd_sensors
from datetime import datetime
import time

from obd_utils import scanSerial

class OBD_Recorder():
    def __init__(self, path, log_items):
        self.port = None
        self.sensorlist = []
        localtime = time.localtime(time.time())
        filename = path+"CivicSI-"+str(localtime[0])+"-"+str(localtime[1])+"-"+str(localtime[2])+"-"+str(localtime[3])+"-"+str(localtime[4])+"-"+str(localtime[5])+".log"
	#filename = path+"1st-"+str(localtime[0])+"-"+str(localtime[1])+"-"+str(localtime[2])+"-"+str(localtime[3])+"-"+str(localtime[4])+"-"+str(localtime[5])+".log"
	#filename = path+"2nd-"+str(localtime[0])+"-"+str(localtime[1])+"-"+str(localtime[2])+"-"+str(localtime[3])+"-"+str(localtime[4])+"-"+str(localtime[5])+".log"
	#filename = path+"3rd-"+str(localtime[0])+"-"+str(localtime[1])+"-"+str(localtime[2])+"-"+str(localtime[3])+"-"+str(localtime[4])+"-"+str(localtime[5])+".log"
	#filename = path+"4th-"+str(localtime[0])+"-"+str(localtime[1])+"-"+str(localtime[2])+"-"+str(localtime[3])+"-"+str(localtime[4])+"-"+str(localtime[5])+".log"

        self.log_file = open(filename, "w", 128)
        self.log_file.write("Time, RPM, MPH, short term fuel trim, long term fuel trim, Throttle, Gear\n");

        for item in log_items:
            self.add_log_item(item)

        self.gear_ratios = [3.267, 2.13, 1.517, 1.147, 0.921, 0.659]
        #log_formatter = logging.Formatter('%(asctime)s.%(msecs).03d,%(message)s', "%H:%M:%S")

    def connect(self):
        portnames = scanSerial()
        #portnames = ['COM10']
        print portnames
        for port in portnames:
            self.port = obd_io.OBDPort(port, None, 2, 2)
            if(self.port.State == 0):
                self.port.close()
                self.port = None
            else:
                break

        if(self.port):
            print "Connected to "+self.port.port.name
            
    def is_connected(self):
        return self.port
        
    def add_log_item(self, item):
        for index, e in enumerate(obd_sensors.SENSORS):
            if(item == e.shortname):
                self.sensorlist.append(index)
                print "Logging item: "+e.name
                break
            
            
    def record_data(self):
        if(self.port is None):
            return None
        
        print "Logging started"
        
        while 1:
            localtime = datetime.now()
            current_time = str(localtime.hour)+":"+str(localtime.minute)+":"+str(localtime.second)+"."+str(localtime.microsecond)
            log_string = current_time
            results = {}
            for index in self.sensorlist:
                (name, value, unit) = self.port.sensor(index)
                log_string = log_string + ","+str(value)
                results[obd_sensors.SENSORS[index].shortname] = value;

            gear = self.calculate_gear(results["rpm"], results["speed"])
            log_string = log_string + "," + str(gear)
            self.log_file.write(log_string+"\n")
            
    def calculate_gear(self, rpm, speed):
        if speed == "" or speed == 0:
            return 0
        if rpm == "" or rpm == 0:
            return 0

        rps = rpm/60
        mps = (speed*0.44704) #meters per second
        
        final_drive  = 4.765
        
        tire_circumference = 1.964 #meters

        current_gear_ratio = (rps / (mps / tire_circumference)) / final drive
        
        print current_gear_ratio
	
	#gear = min((abs(current_gear_ratio - i), i) for i in self.gear_ratios)[1] 
        #return gear
            
            
logitems = ["rpm","speed","short_term_fuel_trim_1","long_term_fuel_trim_1","throttle_pos","gear_ratio"]
#o = OBD_Recorder('/root/pi/logs/', logitems)
o = OBD_Recorder('/media/usb0/logs/', logitems)
o.connect()
if not o.is_connected():
    print "Not connected"
o.record_data()
