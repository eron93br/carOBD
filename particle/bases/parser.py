# This is code is to be used as Python parser to save data from Serial to .csv file
# store OBD-II data from VEHICLE

import serial
import time
import csv

ser = serial.Serial('/dev/tty.usbmodem1411')
ser.flushInput()

f = open('log_data.csv', 'w+')    
while True:
# Read a line and convert it from b'xxx\r\n' to xxx
    line = ser.readline().decode('utf-8')
    if line:  # If it isn't a blank line
        f.write(line)
f.close()
