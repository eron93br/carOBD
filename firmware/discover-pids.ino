/*
 * Copyright 2016 Emerson Garland
 * Free to modify, share and do whatever, just give me credit if you like it!
 *
 * This code will loop over requesting PID availablility and print all parameters available.
 * Monitor you com port (ensure you've installed necessary drivers!) using your favorite tool, program below is set to 115200 baud.
 * Reference https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_1_PID_00 for pid info.
 *
 *
 * Be sure to include carloop library via the web ide. Libraries>(search Carloop)>add to existing app.
 *
 *
 * Make sure to include base85.h in the ide.
 */


#include "carloop/carloop.h"

//#include "carloop.h"
#include "base85.h"


SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void sendObdRequest();
void waitForObdResponse();
void delayUntilNextRequest();
void printValuesAtInterval();
void publishValuesAtInterval();
void printValues();


String dumpMessage(const CANMessage &message);
bool byteArray8Equal(uint8_t a1[8], uint8_t a2[8]);

Carloop<CarloopRevision2> carloop;

int canMessageCount = 0;

///////////////////////////////////////////////////////////////////////////
//GLOBAL INTEGERS FOR USE IN PERFORMING MATH AND EXTRACTION OF OBDII DATA//
///////////////////////////////////////////////////////////////////////////
int data0;
int data1;
int data2;
int data3;
int data4;
int data5;
int data6;
int data7;

int PID_Array[] = {data3, data4, data5, data6};


int AvailablePID_1_20[32];
int AvailablePID_21_40[32];
int AvailablePID_41_60[32];
int AvailablePID_61_80[32];

// OBD CAN MESSAGE IDs
const auto OBD_CAN_BROADCAST_ID    = 0X7DF;
const auto OBD_CAN_REQUEST_ID      = 0x7E0;
const auto OBD_CAN_REPLY_ID_MIN    = 0x7E8;
const auto OBD_CAN_REPLY_ID_MAX    = 0x7EF;

// OBD MODES
const auto OBD_MODE_CURRENT_DATA = 0x01;


///////////////////////////////////////////////////////////////////////////////
//LIST OF ALL MODE 1 AVAILABLE PIDS POLLING. NOT ALL MAY BE AVAILABLE TO YOU.//
///////////////////////////////////////////////////////////////////////////////

const auto OBD_PID_SUPPORTED_PIDS_01_20                  = 0x00;
const auto OBD_PID_SUPPORTED_PIDS_21_40                  = 0x20;
const auto OBD_PID_SUPPORTED_PIDS_41_60                  = 0x40;
const auto OBD_PID_SUPPORTED_PIDS_61_80                  = 0x60;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SUM THE TOTAL AMOUNT OF PIDS YOU WOULD LIKE TO REQUEST AND PLACE THAT IN const size_t NUM_PIDS_TO_REQUEST// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

const size_t NUM_PIDS_TO_REQUEST = 4;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//COMMENT OUT OR REMOVE THE PIDS THAT YOU DO NOT HAVE TO INCREASE EFFECIENCY BUT BE SURE TO UPDATE THE ABOVE CONSTANT//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t pidsToRequest[NUM_PIDS_TO_REQUEST] = {
    
OBD_PID_SUPPORTED_PIDS_01_20,
OBD_PID_SUPPORTED_PIDS_21_40,
OBD_PID_SUPPORTED_PIDS_41_60,
OBD_PID_SUPPORTED_PIDS_61_80

};
    
    
uint8_t pidIndex = NUM_PIDS_TO_REQUEST - 1;

String dumpForPublish;

auto *obdLoopFunction = sendObdRequest;
unsigned long transitionTime = 0;
uint8_t lastMessageData[8];

void setup() {

	Serial.begin(115200);
	carloop.begin();
	Particle.connect();
	transitionTime = millis();
}



void loop() {
	
	carloop.update();
	printValuesAtInterval();
	obdLoopFunction();
	waitForObdResponse();
    math();
    
}


/*************** Begin: OBD Loop Functions ****************/

void sendObdRequest() {
	pidIndex = (pidIndex + 1) % NUM_PIDS_TO_REQUEST;

	CANMessage message;
	message.id = OBD_CAN_BROADCAST_ID;
	message.len = 8; // just always use 8
	message.data[0] = 0x02; // 0 = single-frame format, 2  = num data bytes
	message.data[1] = OBD_MODE_CURRENT_DATA; // OBD MODE
	message.data[2] = pidsToRequest[pidIndex]; // OBD PID

	carloop.can().transmit(message);

	obdLoopFunction = waitForObdResponse;
	transitionTime = millis();
}

void waitForObdResponse() {
	if (millis() - transitionTime >= 10) {
		obdLoopFunction = delayUntilNextRequest;
		transitionTime = millis();
		return;
	}
    bool responseReceived = false;
	String dump;
	CANMessage message;
	while (carloop.can().receive(message)) {
		canMessageCount++;
		if (message.id == 0x130) {
			if (!byteArray8Equal(message.data, lastMessageData)) {
				memcpy(lastMessageData, message.data, 8);
			}
		} else {
			if (message.id >= OBD_CAN_REPLY_ID_MIN &&
					message.id <= OBD_CAN_REPLY_ID_MAX &&
					message.data[2] == pidsToRequest[pidIndex]) {
				    responseReceived = true;
				    //Serial.println("response recieved");
                    data0 = message.data[0];
                    data1 = message.data[1];
			        data2 = message.data[2];
                    data3 = message.data[3];
                    data4 = message.data[4];
                    data5 = message.data[5];
                    data6 = message.data[6];
                    data7 = message.data[7];
                    
                    PID_Array[0] = message.data[3];
                    PID_Array[1] = message.data[4];
                    PID_Array[2] = message.data[5];
                    PID_Array[3] = message.data[6];
                    
                    return;
					}
		return;
		}
	return;
	}
}


void delayUntilNextRequest() {
	if (millis() - transitionTime >= 8) {
		obdLoopFunction = sendObdRequest;
		transitionTime = millis();
	}
}

/*************** End: OBD Loop Functions ****************/


void printValuesAtInterval() {
	static const unsigned long interval = 10000;
	static unsigned long lastDisplay = 0;
	if (millis() - lastDisplay < interval) {
		return;
	}
	lastDisplay = millis();
	printValues();
}

void printValues() {

    //PIDs 1-20 in HEX
    Serial.println("**************Printing PIDs 0-20 (HEXIDECIMAL)**************");
    if (AvailablePID_1_20[7] == true) {
        Serial.println("Monitor status since DTCs cleared");
    }
    if (AvailablePID_1_20[6] == true) {
        Serial.println("Freeze DTC");
    }
    if (AvailablePID_1_20[5] == true) {
        Serial.println("Fuel system status");
    }
    if (AvailablePID_1_20[4] == true) {
        Serial.println("Calculated engine load");
    }
    if (AvailablePID_1_20[3] == true) {
        Serial.println("Engine coolant temperature");
    }
    if (AvailablePID_1_20[2] == true) {
        Serial.println("Short term fuel trim Bank 1");
    }
    if (AvailablePID_1_20[1] == true) {
        Serial.println("Long term fuel trim Bank 1");
    }
    if (AvailablePID_1_20[0] == true) {
        Serial.println("Short term fuel trim Bank 2");
    }
    if (AvailablePID_1_20[15] == true) {
        Serial.println("Long term fuel trim Bank 2");
    }
    if (AvailablePID_1_20[14] == true) {
        Serial.println("Fuel pressure");
    }
    if (AvailablePID_1_20[13] == true) {
        Serial.println("Intake manifold absolute pressure");
    }
    if (AvailablePID_1_20[12] == true) {
        Serial.println("Engine RPM");
    }
    if (AvailablePID_1_20[11] == true) {
        Serial.println("Vehicle speed");
    }
    if (AvailablePID_1_20[10] == true) {
        Serial.println("Timing advance");
    }
    if (AvailablePID_1_20[9] == true) {
        Serial.println("Intake air temperature");
    }
    if (AvailablePID_1_20[8] == true) {
        Serial.println("MAF air flow rate");
    }
    if (AvailablePID_1_20[23] == true) {
        Serial.println("Throttle position");
    }
    if (AvailablePID_1_20[22] == true) {
        Serial.println("Commanded secondary air status");
    }
    if (AvailablePID_1_20[21] == true) {
        Serial.println("Oxygen sensors present (in 2 banks)");
    }
    if (AvailablePID_1_20[20] == true) {
        Serial.println("Oxygen Sensor 1 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[19] == true) {
        Serial.println("Oxygen Sensor 2 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[18] == true) {
        Serial.println("Oxygen Sensor 3 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[17] == true) {
        Serial.println("Oxygen Sensor 4 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[16] == true) {
        Serial.println("Oxygen Sensor 5 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[31] == true) {
        Serial.println("Oxygen Sensor 6 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[30] == true) {
        Serial.println("Oxygen Sensor 7 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[29] == true) {
        Serial.println("Oxygen Sensor 8 A:Voltage B:Short term fuel trim");
    }
    if (AvailablePID_1_20[28] == true) {
        Serial.println("OBD standards this vehicle conforms to");
    }
    if (AvailablePID_1_20[27] == true) {
        Serial.println("Oxygen sensors present (in 4 banks)");
    }
    if (AvailablePID_1_20[26] == true) {
        Serial.println("Auxiliary input status");
    }
    if (AvailablePID_1_20[25] == true) {
        Serial.println("Run time since engine start");
    }
    if (AvailablePID_1_20[24] == true) {
        Serial.println("PIDs supported [21 - 40]");
    }
    
    //PIDs 21-40 in HEX
    Serial.println("**************Printing PIDs 21-40 (HEXIDECIMAL)**************");
    if (AvailablePID_21_40[7] == true) {
        Serial.println("Distance traveled with malfunction indicator lamp (MIL) on");
    }
    if (AvailablePID_21_40[6] == true) {
        Serial.println("Fuel Rail Pressure (relative to manifold vacuum)");
    }
    if (AvailablePID_21_40[5] == true) {
        Serial.println("Fuel Rail Gauge Pressure (diesel, or gasoline direct injection)");
    }
    if (AvailablePID_21_40[4] == true) {
        Serial.println("Oxygen Sensor 1 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[3] == true) {
        Serial.println("Oxygen Sensor 2 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[2] == true) {
        Serial.println("Oxygen Sensor 3 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[1] == true) {
        Serial.println("Oxygen Sensor 4 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[0] == true) {
        Serial.println("Oxygen Sensor 5 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[15] == true) {
        Serial.println("Oxygen Sensor 6 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[14] == true) {
        Serial.println("Oxygen Sensor 7 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[13] == true) {
        Serial.println("Oxygen Sensor 8 AB: Fuel Air Equivalence Ratio CD: Voltage");
    }
    if (AvailablePID_21_40[12] == true) {
        Serial.println("Commanded EGR");
    }
    if (AvailablePID_21_40[11] == true) {
        Serial.println("EGR Error");
    }
    if (AvailablePID_21_40[10] == true) {
        Serial.println("Commanded evaporative purge");
    }
    if (AvailablePID_21_40[9] == true) {
        Serial.println("Fuel Tank Level Input");
    }
    if (AvailablePID_21_40[8] == true) {
        Serial.println("Warm-ups since codes cleared");
    }
    if (AvailablePID_21_40[23] == true) {
        Serial.println("Distance traveled since codes cleared");
    }
    if (AvailablePID_21_40[22] == true) {
        Serial.println("Evap. System Vapor Pressure");
    }
    if (AvailablePID_21_40[21] == true) {
        Serial.println("Absolute Barometric Pressure");
    }
    if (AvailablePID_21_40[20] == true) {
        Serial.println("Oxygen Sensor 1 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[19] == true) {
        Serial.println("Oxygen Sensor 2 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[18] == true) {
        Serial.println("Oxygen Sensor 3 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[17] == true) {
        Serial.println("Oxygen Sensor 4 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[16] == true) {
        Serial.println("Oxygen Sensor 5 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[31] == true) {
        Serial.println("Oxygen Sensor 6 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[30] == true) {
        Serial.println("Oxygen Sensor 7 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[29] == true) {
        Serial.println("Oxygen Sensor 8 AB: Fuel Air Equivalence Ratio CD: Current");
    }
    if (AvailablePID_21_40[28] == true) {
        Serial.println("Catalyst Temperature: Bank 1, Sensor 1");
    }
    if (AvailablePID_21_40[27] == true) {
        Serial.println("Catalyst Temperature: Bank 2, Sensor 1");
    }
    if (AvailablePID_21_40[26] == true) {
        Serial.println("Catalyst Temperature: Bank 1, Sensor 2");
    }
    if (AvailablePID_21_40[25] == true) {
        Serial.println("Catalyst Temperature: Bank 2, Sensor 2");
    }
    if (AvailablePID_21_40[24] == true) {
        Serial.println("PIDs supported [41 - 60]");
    }
    
    //PIDs41-60 in HEX
    Serial.println("**************Printing PIDs 41-60 (HEXIDECIMAL)**************");
    if (AvailablePID_41_60[7] == true) {
        Serial.println("Monitor status this drive cycle");
    }
    if (AvailablePID_41_60[6] == true) {
        Serial.println("Control module voltage");
    }
    if (AvailablePID_41_60[5] == true) {
        Serial.println("Absolute load value");
    }
    if (AvailablePID_41_60[4] == true) {
        Serial.println("Fuel Air commanded equivalence ratio");
    }
    if (AvailablePID_41_60[3] == true) {
        Serial.println("Relative throttle position");
    }
    if (AvailablePID_41_60[2] == true) {
        Serial.println("Ambient air temperature");
    }
    if (AvailablePID_41_60[1] == true) {
        Serial.println("Absolute throttle position B");
    }
    if (AvailablePID_41_60[0] == true) {
        Serial.println("Absolute throttle position C");
    }
    if (AvailablePID_41_60[15] == true) {
        Serial.println("Accelerator pedal position D");
    }
    if (AvailablePID_41_60[14] == true) {
        Serial.println("Accelerator pedal position E");
    }
    if (AvailablePID_41_60[13] == true) {
        Serial.println("Accelerator pedal position F");
    }
    if (AvailablePID_41_60[12] == true) {
        Serial.println("Commanded throttle actuator");
    }
    if (AvailablePID_41_60[11] == true) {
        Serial.println("Time run with MIL on");
    }
    if (AvailablePID_41_60[10] == true) {
        Serial.println("Time since trouble codes cleared");
    }
    if (AvailablePID_41_60[9] == true) {
        Serial.println("Maximum value for Fuel–Air equivalence ratio, oxygen sensor voltage, oxygen sensor current, and intake manifold absolute pressure");
    }
    if (AvailablePID_41_60[8] == true) {
        Serial.println("Maximum value for air flow rate from mass air flow sensor");
    }
    if (AvailablePID_41_60[23] == true) {
        Serial.println("Fuel Type");
    }
    if (AvailablePID_41_60[22] == true) {
        Serial.println("Ethanol fuel %");
    }
    if (AvailablePID_41_60[21] == true) {
        Serial.println("Absolute Evap system Vapor Pressure");
    }
    if (AvailablePID_41_60[20] == true) {
        Serial.println("Evap system vapor pressure");
    }
    if (AvailablePID_41_60[19] == true) {
        Serial.println("Short term secondary oxygen sensor trim, A: bank 1, B: bank 3");
    }
    if (AvailablePID_41_60[18] == true) {
        Serial.println("Long term secondary oxygen sensor trim, A: bank 1, B: bank 3");
    }
    if (AvailablePID_41_60[17] == true) {
        Serial.println("Short term secondary oxygen sensor trim, A: bank 2, B: bank 4");
    }
    if (AvailablePID_41_60[16] == true) {
        Serial.println("Long term secondary oxygen sensor trim, A: bank 2, B: bank 4");
    }
    if (AvailablePID_41_60[31] == true) {
        Serial.println("Fuel rail absolute pressure");
    }
    if (AvailablePID_41_60[30] == true) {
        Serial.println("Relative accelerator pedal position");
    }
    if (AvailablePID_41_60[29] == true) {
        Serial.println("Hybrid battery pack remaining life");
    }
    if (AvailablePID_41_60[28] == true) {
        Serial.println("Engine oil temperature");
    }
    if (AvailablePID_41_60[27] == true) {
        Serial.println("Fuel injection timing");
    }
    if (AvailablePID_41_60[26] == true) {
        Serial.println("Engine fuel rate");
    }
    if (AvailablePID_41_60[25] == true) {
        Serial.println("Emission requirements to which vehicle is designed");
    }
    if (AvailablePID_41_60[24] == true) {
        Serial.println("PIDs supported [61 - 80]");
    }
    
    Serial.println("**************Printing PIDs 61-80 (HEXIDECIMAL)**************");
    if (AvailablePID_61_80[7] == true) {
        Serial.println("Driver's demand engine - percent torque");
    }
    if (AvailablePID_61_80[6] == true) {
        Serial.println("Actual engine - percent torque");
    }
    if (AvailablePID_61_80[5] == true) {
        Serial.println("Engine reference torque");
    }
    if (AvailablePID_61_80[4] == true) {
        Serial.println("Engine percent torque data");
    }
    if (AvailablePID_61_80[3] == true) {
        Serial.println("Auxiliary input / output supported");
    }
    if (AvailablePID_61_80[2] == true) {
        Serial.println("Mass air flow sensor");
    }
    if (AvailablePID_61_80[1] == true) {
        Serial.println("Engine coolant temperature");
    }
    if (AvailablePID_61_80[0] == true) {
        Serial.println("Intake air temperature sensor");
    }
    if (AvailablePID_61_80[15] == true) {
        Serial.println("Commanded EGR and EGR Error");
    }
    if (AvailablePID_61_80[14] == true) {
        Serial.println("Commanded Diesel intake air flow control and relative intake air flow position");
    }
    if (AvailablePID_61_80[13] == true) {
        Serial.println("Exhaust gas recirculation temperature");
    }
    if (AvailablePID_61_80[12] == true) {
        Serial.println("Commanded throttle actuator control and relative throttle position");
    }
    if (AvailablePID_61_80[11] == true) {
        Serial.println("Fuel pressure control system");
    }
    if (AvailablePID_61_80[10] == true) {
        Serial.println("Injection pressure control system");
    }
    if (AvailablePID_61_80[9] == true) {
        Serial.println("Turbocharger compressor inlet pressure");
    }
    if (AvailablePID_61_80[8] == true) {
        Serial.println("Boost pressure control");
    }
    if (AvailablePID_61_80[23] == true) {
        Serial.println("Variable Geometry turbo (VGT) control");
    }
    if (AvailablePID_61_80[22] == true) {
        Serial.println("Wastegate control");
    }
    if (AvailablePID_61_80[21] == true) {
        Serial.println("Exhaust pressure");
    }
    if (AvailablePID_61_80[20] == true) {
        Serial.println("Turbocharger RPM");
    }
    if (AvailablePID_61_80[19] == true) {
        Serial.println("Turbocharger temperature");
    }
    if (AvailablePID_61_80[18] == true) {
        Serial.println("Turbocharger temperature");
    }
    if (AvailablePID_61_80[17] == true) {
        Serial.println("Charge air cooler temperature (CACT)");
    }
    if (AvailablePID_61_80[16] == true) {
        Serial.println("Exhaust Gas temperature (EGT) Bank 1");
    }
    if (AvailablePID_61_80[31] == true) {
        Serial.println("Exhaust Gas temperature (EGT) Bank 2");
    }
    if (AvailablePID_61_80[30] == true) {
        Serial.println("Diesel particulate filter (DPF)");
    }
    if (AvailablePID_61_80[29] == true) {
        Serial.println("Diesel particulate filter (DPF)");
    }
    if (AvailablePID_61_80[28] == true) {
        Serial.println("Diesel Particulate filter (DPF) temperature");
    }
    if (AvailablePID_61_80[27] == true) {
        Serial.println("NOx NTE control area status");
    }
    if (AvailablePID_61_80[26] == true) {
        Serial.println("PM NTE control area status");
    }
    if (AvailablePID_61_80[25] == true) {
        Serial.println("Engine run time");
    }
    if (AvailablePID_61_80[24] == true) {
        Serial.println("PIDs supported [81 - A0]");
    }
    
}

///////////////////////////////////////
//MATH FOR DETERMINING AVAILABLE PIDS//
///////////////////////////////////////

void math() {
    if (data2 == 0) {
      int n = 0;
      int arrayPosition = 0;
      int arrayPlaceHolder = 0;

      int a = 0;
      for (int i = 0; i <= 31; i++) {
        int x;

        AvailablePID_1_20[i] = (PID_Array[arrayPosition] >> n) & 1;
        n = n + 1;
        arrayPlaceHolder = i;
        x = arrayPlaceHolder;
        if (x == 7 || x == 15 || x == 23) {
          arrayPosition = arrayPosition + 1;
          n = 0;
        }
      }
    }

    if (data2 == 32) {
      int n = 0;
      int arrayPosition = 0;
      int arrayPlaceHolder = 0;
      
      for (int i = 0; i <= 31; i++) {
        int x;
  
        AvailablePID_21_40[i] = (PID_Array[arrayPosition] >> n) & 1;
        n = n+1;

        arrayPlaceHolder = i;
        x = arrayPlaceHolder;
        if (x == 7 || x == 15 || x == 23) {
          arrayPosition = arrayPosition + 1;
          n = 0;
        }
      }
    }
    
    
    if (data2 == 64) {
      int n = 0;
      int arrayPosition = 0;
      int arrayPlaceHolder = 0;
      
      for (int i = 0; i <= 31; i++) {
        int x;
  
        AvailablePID_41_60[i] = (PID_Array[arrayPosition] >> n) & 1;
        n = n+1;

        arrayPlaceHolder = i;
        x = arrayPlaceHolder;
        if (x == 7 || x == 15 || x == 23) {
          arrayPosition = arrayPosition + 1;
          n = 0;
        }
      }
    }
    
    
     if (data2 == 96) {
      int n = 0;
      int arrayPosition = 0;
      int arrayPlaceHolder = 0;
      
      for (int i = 0; i <= 31; i++) {
        int x;
  
        AvailablePID_61_80[i] = (PID_Array[arrayPosition] >> n) & 1;
        n = n+1;

        arrayPlaceHolder = i;
        x = arrayPlaceHolder;
        if (x == 7 || x == 15 || x == 23) {
          arrayPosition = arrayPosition + 1;
          n = 0;
        }
      }
    }

}

bool byteArray8Equal(uint8_t a1[8], uint8_t a2[8]) {
	for (int i = 0; i < 8; i++) {
		if (a1[i] != a2[i]) return false;
	}
	return true;
}
