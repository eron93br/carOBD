// This #include statement was automatically added by the Particle IDE.
//#include <sd-card-library-photon-compat.h>

// This #include statement was automatically added by the Particle IDE.


/*
 * Copyright 2016 Emerson Garland
 * Free to modify, share and do whatever, just give me credit if you like it!
 * This code will publish chosen obdii compliant messages to blynk for data visualization.
 */

//#include "carloop.h"
// include carloop.h using the ide add library function.
#include "carloop/carloop.h"
#include "base85.h"
// include base85.h by copy and pasting base85.h located in this repository into the web ide


SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void sendObdRequest();
void waitForObdResponse();
void delayUntilNextRequest();
void printValuesAtInterval();
void publishValuesAtInterval();
void printValues();
void getEngineLoad();
void receiveObdRequestVIN();
void sendObdRequestVIN();

String dumpMessage(const CANMessage &message);
bool byteArray8Equal(uint8_t a1[8], uint8_t a2[8]);

Carloop<CarloopRevision2> carloop;

int canMessageCount = 0;

/////////////////////////////////////////////////////////////////////////
//GLOBAL FLOATS FOR USE IN PRINTING AND PUBLISHING TO EXTERNAL SERVICES//
/////////////////////////////////////////////////////////////////////////

//ACRONYM DEFINITIONS:
//O2_V_1 = OXYGEN SENSOR 1 VOLTAGE
//O2_T_1 = OXYGEN SENSOR 1 FUEL TRIM
//O2_F_A_E_R_V_1_FUEL = OXYGEN SENSOR 1 FUEL TO AIR EQUIVILANCE RATIO AND VOLTAGE FUEL BYTE
//O2_F_A_E_R_V_1_VOLTAGE = OXYGEN SENSOR 1 FUEL TO AIR EQUIVILANCE RATIO AND VOLTAGE VOLTAGE BYTE
//O2_F_A_E_R_C_1_CURRENT = OXYGEN SENSOR 1 FUEL TO AIR EQUIVILANCE RATIO AND CURRENT CURRENTE BYTE

float MIL_STATUS;
float FUEL_SYSTEM_STATUS;
float ENGINE_LOAD;
float COOLANT_TEMPERATURE;
float SHORT_TERM_FUEL_TRIM_BANK_1;
float LONG_TERM_FUEL_TRIM_BANK_1;
float SHORT_TERM_FUEL_TRIM_BANK_2;
float LONG_TERM_FUEL_TRIM_BANK_2;
float FUEL_PRESSURE;
float INTAKE_MANIFOLD_PRESSURE;
float ENGINE_RPM;
float VEHICLE_SPEED;
float TIMING_ADVANCE;
float INTAKE_AIR_TEMPERATURE;
float MAF_AIR_FLOW_RATE;
float THROTTLE;
float COMMANDED_SECONDARY_AIR_STATUS;
float O2_SENSORS_PRESENT;
float O2_V_1;
float O2_T_1;
float O2_V_2;
float O2_T_2;
float O2_V_3;
float O2_T_3;
float O2_V_4;
float O2_T_4;
float O2_V_5;
float O2_T_5;
float O2_V_6;
float O2_T_6;
float O2_V_7;
float O2_T_7;
float O2_V_8;
float O2_T_8;
float OBD_STANDARDS;
float ENGINE_RUN_TIME;
float SUPPORTED_PIDS_21_40;
float DISTANCE_TRAVELED_WITH_MIL_ON;
float FUEL_RAIL_PRESSURE;
float FUEL_RAIL_GAUGE_PRESSURE;
float O2_F_A_E_R_V_1_FUEL;
float O2_F_A_E_R_V_1_VOLTAGE;
float O2_F_A_E_R_V_2_FUEL;
float O2_F_A_E_R_V_2_VOLTAGE;
float O2_F_A_E_R_V_3_FUEL;
float O2_F_A_E_R_V_3_VOLTAGE;
float O2_F_A_E_R_V_4_FUEL;
float O2_F_A_E_R_V_4_VOLTAGE;
float O2_F_A_E_R_V_5_FUEL;
float O2_F_A_E_R_V_5_VOLTAGE;
float O2_F_A_E_R_V_6_FUEL;
float O2_F_A_E_R_V_6_VOLTAGE;
float O2_F_A_E_R_V_7_FUEL;
float O2_F_A_E_R_V_7_VOLTAGE;
float O2_F_A_E_R_V_8_FUEL;
float O2_F_A_E_R_V_8_VOLTAGE;
float COMMANDED_EGR;
float EGR_ERROR;
float COMMANDED_EVAPORATIVE_PURGE;
float FUEL_TANK_LEVEL_INPUT;
float WARM_UPS_SINCE_CODES_CLEARED;
float DISTANCE_TRAVELED_SINCE_CODES_CLEARED;
float EVAPORATOR_SYSTEM_PRESSURE;
float ABSOLUTE_BAROMETRIC_PRESSURE;
float O2_F_A_E_R_C_1_FUEL;
float O2_F_A_E_R_C_1_CURRENT;
float O2_F_A_E_R_C_2_FUEL;
float O2_F_A_E_R_C_2_CURRENT;
float O2_F_A_E_R_C_3_FUEL;
float O2_F_A_E_R_C_3_CURRENT;
float O2_F_A_E_R_C_4_FUEL;
float O2_F_A_E_R_C_4_CURRENT;
float O2_F_A_E_R_C_5_FUEL;
float O2_F_A_E_R_C_5_CURRENT;
float O2_F_A_E_R_C_6_FUEL;
float O2_F_A_E_R_C_6_CURRENT;
float O2_F_A_E_R_C_7_FUEL;
float O2_F_A_E_R_C_7_CURRENT;
float O2_F_A_E_R_C_8_FUEL;
float O2_F_A_E_R_C_8_CURRENT;
float CATALYST_TEMPERATURE_BANK1_SENSOR1;
float CATALYST_TEMPERATURE_BANK2_SENSOR1;
float CATALYST_TEMPERATURE_BANK1_SENSOR2;
float CATALYST_TEMPERATURE_BANK2_SENSOR2;
float SUPPORTED_PIDS_41_60;
float MONITOR_STATUS;
float CONTROL_MODULE_VOLTAGE;
float ABSOLUTE_LOAD_VALUE;
float FUEL_AIR_COMMANDED_EQUIV_RATIO;
float RELATIVE_THROTTLE_POSITION;
float AMBIENT_AIR_TEMPERATURE;
float ABSOLUTE_THROTTLE_B;
float ABSOLUTE_THROTTLE_C;
float ACCELERATOR_PEDAL_POSITION_D;
float ACCELERATOR_PEDAL_POSITION_E;
float ACCELERATOR_PEDAL_POSITION_F;
float COMMANDED_THROTTLE_ACTUATOR;
float TIME_RUN_WITH_MIL_ON;
float TIME_SINCE_TROUBLE_CODES_CLEARED;
float MAX_VALUES;
float MAX_VALUES_2;
float FUEL_TYPE;
float ETHANOL_FUEL_PERCENT;
float ABSOLUTE_EVAP_SYS_VAPOR_PRESSURE;
float EVAP_SYSTEM_VAPOR_PRESSURE;
float SHORT_TERM_SECONDARY_O2_TRIM_BANK_1_3;
float LONG_TERM_SECONDARY_O2_TRIM_BANK_1_3;
float SHORT_TERM_SECONDARY_O2_TRIM_BANK_2_4;
float LONG_TERM_SECONDARY_O2_TRIM_BANK_2_4;
float FUEL_RAIL_ABSOLUTE_PRESSURE;
float RELATIVE_ACCELERATOR_PEDAL_POSITION;
float HYBRID_BATTERY_PACK_REMAINING_LIFE;
float ENGINE_OIL_TEMPERATURE;
float FUEL_INJECTION_TIMING;
float ENGINE_FUEL_RATE;
float EMISSION_REQUIREMENTS;
float SUPPORTED_PIDS_61_80;
float DEMAND_PERCENT_TORQUE;
float ACTUAL_PERCENT_TORQUE;
float ENGINE_REFERENCE_TORQUE;
float ENGINE_PERCENT_TORQUE_DATA;
float AUXILLARY_IO_SUPPORTED;
float MASS_AIR_FLOW_SENSOR;
float ENGINE_COOLANT_TEMPERATURE;
float INTAKE_AIR_TEMPERATURE_SENSOR;

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


// OBD CAN MESSAGE IDs
const auto OBD_CAN_BROADCAST_ID    = 0X7DF;
const auto OBD_CAN_REQUEST_ID      = 0x7E0;
const auto OBD_CAN_REPLY_ID_MIN    = 0x7E8;
const auto OBD_CAN_REPLY_ID_MAX    = 0x7EF;

const String key = "YN8B4S3KMEFDM0FT";

// OBD MODES
const auto OBD_MODE_CURRENT_DATA = 0x01;

//const auto OBD_MODE_REQUEST_VEHICLE_DATA = 0x09;

///////////////////////////////////////////////////////////////////////////////////////////////////////
//LIST OF ALL (Almost) MODE 1 PIDS THAT YOU ARE CAPABLE OF POLLING. NOT ALL WILL BE AVAILABLE TO YOU.//
///////////////////////////////////////////////////////////////////////////////////////////////////////

const auto OBD_PID_SUPPORTED_PIDS_01_20                  = 0x00;
const auto OBD_PID_MIL_STATUS                            = 0x01;
const auto OBD_PID_FUEL_SYSTEM_STATUS                    = 0x03;
const auto OBD_PID_ENGINE_LOAD                           = 0x04;
const auto OBD_PID_COOLANT_TEMPERATURE                   = 0x05;
const auto OBD_PID_SHORT_TERM_FUEL_TRIM_BANK_1           = 0x06;
const auto OBD_PID_LONG_TERM_FUEL_TRIM_BANK_1            = 0x07;
const auto OBD_PID_SHORT_TERM_FUEL_TRIM_BANK_2           = 0x08;
const auto OBD_PID_LONG_TERM_FUEL_TRIM_BANK_2            = 0x09;
const auto OBD_PID_FUEL_PRESSURE                         = 0x0a;
const auto OBD_PID_INTAKE_MANIFOLD_PRESSURE              = 0x0b;
const auto OBD_PID_ENGINE_RPM                            = 0x0c;
const auto OBD_PID_VEHICLE_SPEED                         = 0x0d;
const auto OBD_PID_TIMING_ADVANCE                        = 0x0e;
const auto OBD_PID_INTAKE_AIR_TEMPERATURE                = 0x0f;
const auto OBD_PID_MAF_AIR_FLOW_RATE                     = 0x10;
const auto OBD_PID_THROTTLE    	                         = 0x11;
const auto OBD_PID_COMMANDED_SECONDARY_AIR_STATUS        = 0x12;
const auto OBD_PID_O2_SENSORS_PRESENT                    = 0x13;
const auto OBD_PID_O2_V_F_T_1                            = 0x14;
const auto OBD_PID_O2_V_F_T_2                            = 0x15;
const auto OBD_PID_O2_V_F_T_3                            = 0x16;
const auto OBD_PID_O2_V_F_T_4                            = 0x17;
const auto OBD_PID_O2_V_F_T_5                            = 0x18;
const auto OBD_PID_O2_V_F_T_6                            = 0x19;
const auto OBD_PID_O2_V_F_T_7                            = 0x1a;
const auto OBD_PID_O2_V_F_T_8                            = 0x1b;
const auto OBD_PID_OBD_STANDARDS                         = 0x1c;
const auto OBD_PID_ENGINE_RUN_TIME                       = 0x1f;
const auto OBD_PID_SUPPORTED_PIDS_21_40                  = 0x20;
const auto OBD_PID_DISTANCE_TRAVELED_WITH_MIL_ON         = 0x21;
const auto OBD_PID_FUEL_RAIL_PRESSURE                    = 0x22;
const auto OBD_PID_FUEL_RAIL_GAUGE_PRESSURE              = 0x23;
const auto OBD_PID_O2_F_A_E_R_V_1                        = 0x24;
const auto OBD_PID_O2_F_A_E_R_V_2                        = 0x25;
const auto OBD_PID_O2_F_A_E_R_V_3                        = 0x26;
const auto OBD_PID_O2_F_A_E_R_V_4                        = 0x27;
const auto OBD_PID_O2_F_A_E_R_V_5                        = 0x28;
const auto OBD_PID_O2_F_A_E_R_V_6                        = 0x29;
const auto OBD_PID_O2_F_A_E_R_V_7                        = 0x2a;
const auto OBD_PID_O2_F_A_E_R_V_8                        = 0x2b;
const auto OBD_PID_COMMANDED_EGR                         = 0x2c;
const auto OBD_PID_EGR_ERROR                             = 0x2d;
const auto OBD_PID_COMMANDED_EVAPORATIVE_PURGE           = 0x2e;
const auto OBD_PID_FUEL_TANK_LEVEL_INPUT                 = 0x2f;
const auto OBD_PID_WARM_UPS_SINCE_CODES_CLEARED          = 0x30;
const auto OBD_PID_DISTANCE_TRAVELED_SINCE_CODES_CLEARED = 0x31;
const auto OBD_PID_EVAPORATOR_SYSTEM_PRESSURE            = 0x32;
const auto OBD_PID_ABSOLUTE_BAROMETRIC_PRESSURE          = 0x33;
const auto OBD_PID_O2_F_A_E_R_1_C                        = 0x34;
const auto OBD_PID_O2_F_A_E_R_2_C                        = 0x35;
const auto OBD_PID_O2_F_A_E_R_3_C                        = 0x36;
const auto OBD_PID_O2_F_A_E_R_4_C                        = 0x37;
const auto OBD_PID_O2_F_A_E_R_5_C                        = 0x38;
const auto OBD_PID_O2_F_A_E_R_6_C                        = 0x39;
const auto OBD_PID_O2_F_A_E_R_7_C                        = 0x3a;
const auto OBD_PID_O2_F_A_E_R_8_C                        = 0x3b;
const auto OBD_PID_CATALYST_TEMPERATURE_BANK1_SENSOR1    = 0x3c;
const auto OBD_PID_CATALYST_TEMPERATURE_BANK2_SENSOR1    = 0x3d;
const auto OBD_PID_CATALYST_TEMPERATURE_BANK1_SENSOR2    = 0x3e;
const auto OBD_PID_CATALYST_TEMPERATURE_BANK2_SENSOR2    = 0x3f;
const auto OBD_PID_SUPPORTED_PIDS_41_60                  = 0x40;
const auto OBD_PID_MONITOR_STATUS                        = 0x41;
const auto OBD_PID_CONTROL_MODULE_VOLTAGE                = 0x42;
const auto OBD_PID_ABSOLUTE_LOAD_VALUE                   = 0x43;
const auto OBD_PID_FUEL_AIR_COMMANDED_EQUIV_RATIO        = 0x44;
const auto OBD_PID_RELATIVE_THROTTLE_POSITION            = 0x45;
const auto OBD_PID_AMBIENT_AIR_TEMPERATURE               = 0x46;
const auto OBD_PID_ABSOLUTE_THROTTLE_B                   = 0x47;
const auto OBD_PID_ABSOLUTE_THROTTLE_C                   = 0x48;
const auto OBD_PID_ACCELERATOR_PEDAL_POSITION_D          = 0x49;
const auto OBD_PID_ACCELERATOR_PEDAL_POSITION_E          = 0x4a;
const auto OBD_PID_ACCELERATOR_PEDAL_POSITION_F          = 0x4b;
const auto OBD_PID_COMMANDED_THROTTLE_ACTUATOR           = 0x4c;
const auto OBD_PID_TIME_RUN_WITH_MIL_ON                  = 0x4d;
const auto OBD_PID_TIME_SINCE_TROUBLE_CODES_CLEARED      = 0x4e;
const auto OBD_PID_MAX_VALUES                            = 0x4f;
const auto OBD_PID_MAX_VALUES_2                          = 0x50;
const auto OBD_PID_FUEL_TYPE                             = 0x51;
const auto OBD_PID_ETHANOL_FUEL_PERCENT                  = 0x52;
const auto OBD_PID_ABSOLUTE_EVAP_SYS_VAPOR_PRESSURE      = 0x53;
const auto OBD_PID_EVAP_SYSTEM_VAPOR_PRESSURE            = 0x54;
const auto OBD_PID_SHORT_TERM_SECONDARY_O2_TRIM_BANK_1_3 = 0x55;
const auto OBD_PID_LONG_TERM_SECONDARY_O2_TRIM_BANK_1_3  = 0x56;
const auto OBD_PID_SHORT_TERM_SECONDARY_O2_TRIM_BANK_2_4 = 0x57;
const auto OBD_PID_LONG_TERM_SECONDARY_O2_TRIM_BANK_2_4  = 0x58;
const auto OBD_PID_FUEL_RAIL_ABSOLUTE_PRESSURE           = 0x59;
const auto OBD_PID_RELATIVE_ACCELERATOR_PEDAL_POSITION   = 0x5a;
const auto OBD_PID_HYBRID_BATTERY_PACK_REMAINING_LIFE    = 0x5b;
const auto OBD_PID_ENGINE_OIL_TEMPERATURE                = 0x5c;
const auto OBD_PID_FUEL_INJECTION_TIMING                 = 0x5d;
const auto OBD_PID_ENGINE_FUEL_RATE                      = 0x5e;
const auto OBD_PID_EMISSION_REQUIREMENTS                 = 0x5f;
const auto OBD_PID_SUPPORTED_PIDS_61_80                  = 0x60;
const auto OBD_PID_DEMAND_PERCENT_TORQUE                 = 0x61;
const auto OBD_PID_ACTUAL_PERCENT_TORQUE                 = 0x62;
const auto OBD_PID_ENGINE_REFERENCE_TORQUE               = 0x63;
const auto OBD_PID_ENGINE_PERCENT_TORQUE_DATA            = 0x64;
const auto OBD_PID_AUXILLARY_IO_SUPPORTED                = 0x65;
const auto OBD_PID_MASS_AIR_FLOW_SENSOR                  = 0x66;
const auto OBD_PID_ENGINE_COOLANT_TEMPERATURE            = 0x67;
const auto OBD_PID_INTAKE_AIR_TEMPERATURE_SENSOR         = 0x68;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SUM THE TOTAL AMOUNT OF PIDS YOU WOULD LIKE TO REQUEST AND PLACE THAT IN const size_t NUM_PIDS_TO_REQUEST// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

const size_t NUM_PIDS_TO_REQUEST = 102;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//COMMENT OUT OR REMOVE THE PIDS THAT YOU DO NOT HAVE TO INCREASE EFFECIENCY BUT BE SURE TO UPDATE THE ABOVE CONSTANT//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t pidsToRequest[NUM_PIDS_TO_REQUEST] = {
    
OBD_PID_SUPPORTED_PIDS_01_20,
OBD_PID_MIL_STATUS,
OBD_PID_FUEL_SYSTEM_STATUS,
OBD_PID_ENGINE_LOAD,
OBD_PID_COOLANT_TEMPERATURE,
OBD_PID_SHORT_TERM_FUEL_TRIM_BANK_1,
OBD_PID_LONG_TERM_FUEL_TRIM_BANK_1,
OBD_PID_SHORT_TERM_FUEL_TRIM_BANK_2,
OBD_PID_LONG_TERM_FUEL_TRIM_BANK_2,
OBD_PID_FUEL_PRESSURE,
OBD_PID_INTAKE_MANIFOLD_PRESSURE,
OBD_PID_ENGINE_RPM,
OBD_PID_VEHICLE_SPEED,
OBD_PID_TIMING_ADVANCE,
OBD_PID_INTAKE_AIR_TEMPERATURE,
OBD_PID_MAF_AIR_FLOW_RATE,
OBD_PID_THROTTLE,
OBD_PID_COMMANDED_SECONDARY_AIR_STATUS,
OBD_PID_O2_SENSORS_PRESENT,
OBD_PID_O2_V_F_T_1,
OBD_PID_O2_V_F_T_2,
OBD_PID_O2_V_F_T_3,
OBD_PID_O2_V_F_T_4,
OBD_PID_O2_V_F_T_5,
OBD_PID_O2_V_F_T_6,
OBD_PID_O2_V_F_T_7,
OBD_PID_O2_V_F_T_8,
OBD_PID_OBD_STANDARDS,
OBD_PID_ENGINE_RUN_TIME,
OBD_PID_SUPPORTED_PIDS_21_40,
OBD_PID_DISTANCE_TRAVELED_WITH_MIL_ON,
OBD_PID_FUEL_RAIL_PRESSURE,
OBD_PID_FUEL_RAIL_GAUGE_PRESSURE,
OBD_PID_O2_F_A_E_R_V_1,
OBD_PID_O2_F_A_E_R_V_2,
OBD_PID_O2_F_A_E_R_V_3,
OBD_PID_O2_F_A_E_R_V_4,
OBD_PID_O2_F_A_E_R_V_5,
OBD_PID_O2_F_A_E_R_V_6,
OBD_PID_O2_F_A_E_R_V_7,
OBD_PID_O2_F_A_E_R_V_8,
OBD_PID_COMMANDED_EGR,
OBD_PID_EGR_ERROR,
OBD_PID_COMMANDED_EVAPORATIVE_PURGE,
OBD_PID_FUEL_TANK_LEVEL_INPUT,
OBD_PID_WARM_UPS_SINCE_CODES_CLEARED,
OBD_PID_DISTANCE_TRAVELED_SINCE_CODES_CLEARED,
OBD_PID_EVAPORATOR_SYSTEM_PRESSURE,
OBD_PID_ABSOLUTE_BAROMETRIC_PRESSURE,
OBD_PID_O2_F_A_E_R_1_C,
OBD_PID_O2_F_A_E_R_2_C,
OBD_PID_O2_F_A_E_R_3_C,
OBD_PID_O2_F_A_E_R_4_C,
OBD_PID_O2_F_A_E_R_5_C,
OBD_PID_O2_F_A_E_R_6_C,
OBD_PID_O2_F_A_E_R_7_C,
OBD_PID_O2_F_A_E_R_8_C,
OBD_PID_CATALYST_TEMPERATURE_BANK1_SENSOR1,
OBD_PID_CATALYST_TEMPERATURE_BANK2_SENSOR1,
OBD_PID_CATALYST_TEMPERATURE_BANK1_SENSOR2,
OBD_PID_CATALYST_TEMPERATURE_BANK2_SENSOR2,
OBD_PID_SUPPORTED_PIDS_41_60,
OBD_PID_MONITOR_STATUS,
OBD_PID_CONTROL_MODULE_VOLTAGE,
OBD_PID_ABSOLUTE_LOAD_VALUE,
OBD_PID_FUEL_AIR_COMMANDED_EQUIV_RATIO,
OBD_PID_RELATIVE_THROTTLE_POSITION,
OBD_PID_AMBIENT_AIR_TEMPERATURE,
OBD_PID_ABSOLUTE_THROTTLE_B,
OBD_PID_ABSOLUTE_THROTTLE_C,
OBD_PID_ACCELERATOR_PEDAL_POSITION_D,
OBD_PID_ACCELERATOR_PEDAL_POSITION_E,
OBD_PID_ACCELERATOR_PEDAL_POSITION_F,
OBD_PID_COMMANDED_THROTTLE_ACTUATOR,
OBD_PID_TIME_RUN_WITH_MIL_ON,
OBD_PID_TIME_SINCE_TROUBLE_CODES_CLEARED,
OBD_PID_MAX_VALUES,
OBD_PID_MAX_VALUES_2,
OBD_PID_FUEL_TYPE,
OBD_PID_ETHANOL_FUEL_PERCENT,
OBD_PID_ABSOLUTE_EVAP_SYS_VAPOR_PRESSURE,
OBD_PID_EVAP_SYSTEM_VAPOR_PRESSURE,
OBD_PID_SHORT_TERM_SECONDARY_O2_TRIM_BANK_1_3,
OBD_PID_LONG_TERM_SECONDARY_O2_TRIM_BANK_1_3,
OBD_PID_SHORT_TERM_SECONDARY_O2_TRIM_BANK_2_4,
OBD_PID_LONG_TERM_SECONDARY_O2_TRIM_BANK_2_4,
OBD_PID_FUEL_RAIL_ABSOLUTE_PRESSURE,
OBD_PID_RELATIVE_ACCELERATOR_PEDAL_POSITION,
OBD_PID_HYBRID_BATTERY_PACK_REMAINING_LIFE,
OBD_PID_ENGINE_OIL_TEMPERATURE,
OBD_PID_FUEL_INJECTION_TIMING,
OBD_PID_ENGINE_FUEL_RATE,
OBD_PID_EMISSION_REQUIREMENTS,
OBD_PID_SUPPORTED_PIDS_61_80,
OBD_PID_DEMAND_PERCENT_TORQUE,
OBD_PID_ACTUAL_PERCENT_TORQUE,
OBD_PID_ENGINE_REFERENCE_TORQUE,
OBD_PID_ENGINE_PERCENT_TORQUE_DATA,
OBD_PID_AUXILLARY_IO_SUPPORTED,
OBD_PID_MASS_AIR_FLOW_SENSOR,
OBD_PID_ENGINE_COOLANT_TEMPERATURE,
OBD_PID_INTAKE_AIR_TEMPERATURE_SENSOR
};
    
    
uint8_t pidIndex = NUM_PIDS_TO_REQUEST - 1;

String dumpForPublish;

auto *obdLoopFunction = sendObdRequest;
unsigned long transitionTime = 0;
uint8_t lastMessageData[8];

void setup() 
{
	Serial.begin(115200);
	carloop.begin();
	Particle.connect();
	transitionTime = millis();
}



void loop() 
{
	carloop.update();
	printValuesAtInterval();
	publishValuesAtInterval();
	obdLoopFunction();
	waitForObdResponse();
    math();

}


/*************** Begin: OBD Loop Functions ****************/

/* For help understanding the OBD Query format over CAN bus,
 * see: https://en.wikipedia.org/wiki/OBD-II_PIDs#Query
 *
 * For help understanding why the first data byte is 0x02,
 * see: http://hackaday.com/2013/10/29/can-hacking-protocols/
 *
 * For help understanding modes and PIDs,
 * see: https://en.wikipedia.org/wiki/OBD-II_PIDs#Modes
 * and: https://en.wikipedia.org/wiki/OBD-II_PIDs#Standard_PIDs
 */
void sendObdRequest() 
{
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

void waitForObdResponse() 
{
	if (millis() - transitionTime >= 10) 
	{
		obdLoopFunction = delayUntilNextRequest;
		transitionTime = millis();
		return;
	}
    bool responseReceived = false;
	String dump;
	CANMessage message;
	while (carloop.can().receive(message))
	{
		canMessageCount++;
		if (message.id == 0x130) {
			if (!byteArray8Equal(message.data, lastMessageData))
			{
				memcpy(lastMessageData, message.data, 8);
			}
		} else {
			if (message.id >= OBD_CAN_REPLY_ID_MIN &&
					message.id <= OBD_CAN_REPLY_ID_MAX &&
					message.data[2] == pidsToRequest[pidIndex]) {
				    responseReceived = true;
                    data0 = message.data[0];
                    data1 = message.data[1];
			        data2 = message.data[2];
                    data3 = message.data[3];
                    data4 = message.data[4];
                    data5 = message.data[5];
                    data6 = message.data[6];
                    data7 = message.data[7];
                    return;
					}
		return;
		}
	return;
	}
}


void delayUntilNextRequest()
{
	if (millis() - transitionTime >= 8) {
		obdLoopFunction = sendObdRequest;
		transitionTime = millis();
	}
}

/*************** End: OBD Loop Functions ****************/


void printValuesAtInterval() 
{
	static const unsigned long interval = 500;
	static unsigned long lastDisplay = 0;
	if (millis() - lastDisplay < interval) 
	{
		return;
	}
	lastDisplay = millis();
	printValues();
}

void printValues() 
{
    /////////////////////////////
    //PRINT VALUES FOR DEBUGING//
     ////////////////////////////
     Serial.print(ENGINE_LOAD);                  Serial.print(" , "); 
     Serial.print(COOLANT_TEMPERATURE);          Serial.print(" , ");
     Serial.print(LONG_TERM_FUEL_TRIM_BANK_1);   Serial.print(" , ");
     Serial.print(SHORT_TERM_FUEL_TRIM_BANK_1);  Serial.print(" , "); 
     Serial.print(INTAKE_MANIFOLD_PRESSURE);     Serial.print(" , ");
     Serial.print(ENGINE_RPM);                   Serial.print(" , "); 
     Serial.print(VEHICLE_SPEED);                Serial.print(" , ");
     Serial.print(TIMING_ADVANCE);               Serial.print(" , "); 
     Serial.print(INTAKE_AIR_TEMPERATURE);       Serial.print(" , ");
     Serial.print(THROTTLE);                     Serial.print(" , "); 
     Serial.print(OBD_STANDARDS);                Serial.print(" , ");
     Serial.print(ENGINE_RUN_TIME);              Serial.print(" , ");
     Serial.print(FUEL_TANK_LEVEL_INPUT);        Serial.print(" , ");
     Serial.print(WARM_UPS_SINCE_CODES_CLEARED); Serial.print(" , "); 
     Serial.print(DISTANCE_TRAVELED_SINCE_CODES_CLEARED); Serial.print(" , "); 
     Serial.print(ABSOLUTE_BAROMETRIC_PRESSURE); Serial.print(" , ");  
     Serial.print(CATALYST_TEMPERATURE_BANK1_SENSOR1); Serial.print(" , "); 
     Serial.print(CATALYST_TEMPERATURE_BANK1_SENSOR2); Serial.print(" , ");
     Serial.print(CONTROL_MODULE_VOLTAGE); Serial.print(" , ");  
     Serial.print(FUEL_AIR_COMMANDED_EQUIV_RATIO); Serial.print(" , ");
     Serial.print(ACCELERATOR_PEDAL_POSITION_F); Serial.print(" , "); 
     Serial.print(ACCELERATOR_PEDAL_POSITION_D); Serial.print(" , "); 
     Serial.print(ACCELERATOR_PEDAL_POSITION_E); Serial.print(" , ");
     Serial.print(COMMANDED_THROTTLE_ACTUATOR);  Serial.print(" , ");
     Serial.print(TIME_RUN_WITH_MIL_ON);         Serial.print(" , ");
     Serial.println(TIME_SINCE_TROUBLE_CODES_CLEARED); 
     
                    
 // Engine coolant temperature
// Short term fuel trim—Bank 1
// Long term fuel trim—Bank 1
// Intake manifold absolute pressure
// Engine RPM
// Vehicle speed
// Timing advance
// Intake air temperature
// Throttle position
// Oxygen sensors present (in 2 banks)
// Oxygen Sensor 1 A: Voltage B: Short term fuel trim
// Oxygen Sensor 2 A: Voltage B: Short term fuel trim
// OBD standards this vehicle conforms to
// Run time since engine start
// Distance traveled with malfunction indicator lamp (MIL) on
// Fuel Tank Level Input
// Warm-ups since codes cleared
// Distance traveled since codes cleared
// Absolute Barometric Pressure
// Catalyst Temperature: Bank 1, Sensor 1
// Catalyst Temperature: Bank 1, Sensor 2
// Control module voltage
// Fuel–Air commanded equivalence ratio
// Relative throttle position
// Absolute throttle position B
// Accelerator pedal position D
// Accelerator pedal position E
// Commanded throttle actuator
// Time run with MIL on
// Time since trouble codes cleared
}

void publishValuesAtInterval() 
{
    static const unsigned long interval2 = 10000;
	static unsigned long lastDisplay2 = 0;
    if(millis() - lastDisplay2 < interval2) 
    {
        return;
    }
    lastDisplay2 = millis();
    //publishValues();
    blynkValues();
}

///////////////////////////////////////////////////
//FUNCTION TO SEND DATA TO BYNK FOR VISUALIZATION//
//THESE PARAMETERS ARE HERE AS AN EXAMPLE AND ARE//
//AVAILABLE ON MY 2016 MIATA///////////////////////

void blynkValues() 
{
        //TODO: ONLY SEND DATA WHEN THE ENGINE IS ON
        // ... variables ...
        String StringMIL_STATUS                             = String(MIL_STATUS);
        String StringFUEL_SYSTEM_STATUS                     = String(FUEL_SYSTEM_STATUS);
	    String StringENGINE_LOAD                            = String(ENGINE_LOAD);
	    String StringENGINE_COOLANT_TEMPERATURE             = String(ENGINE_COOLANT_TEMPERATURE);
	    String StringSHORT_TERM_FUEL_TRIM_BANK_1            = String(SHORT_TERM_FUEL_TRIM_BANK_1);
	    String StringLONG_TERM_FUEL_TRIM_BANK_1             = String(LONG_TERM_FUEL_TRIM_BANK_1);
	    String StringINTAKE_MANIFOLD_PRESSURE               = String(INTAKE_MANIFOLD_PRESSURE);
	    String StringENGINE_RPM                             = String(ENGINE_RPM);
	    String StringVEHICLE_SPEED                          = String(VEHICLE_SPEED);
	    String StringTIMING_ADVANCE                         = String(TIMING_ADVANCE);
	    String StringINTAKE_AIR_TEMPERATURE                 = String(INTAKE_AIR_TEMPERATURE);
	    String StringMAF_AIR_FLOW_RATE                      = String(MAF_AIR_FLOW_RATE);
	    String StringTHROTTLE                               = String(THROTTLE);
	    String StringO2_V_6                                 = String(O2_V_6);
	    String StringO2_T_6                                 = String(O2_T_6);
	    String StringDISTANCE_TRAVELED_WITH_MIL_ON          = String(DISTANCE_TRAVELED_WITH_MIL_ON);
	    String StringCOMMANDED_EVAPORATIVE_PURGE            = String(COMMANDED_EVAPORATIVE_PURGE);
	    String StringFUEL_TANK_LEVEL_INPUT                  = String(FUEL_TANK_LEVEL_INPUT);
	    String StringWARM_UPS_SINCE_CODES_CLEARED           = String(WARM_UPS_SINCE_CODES_CLEARED);
	    String StringDISTANCE_TRAVELED_SINCE_CODES_CLEARED  = String(DISTANCE_TRAVELED_SINCE_CODES_CLEARED);
	    String StringEVAPORATOR_SYSTEM_PRESSURE             = String(EVAPORATOR_SYSTEM_PRESSURE);
	    String StringABSOLUTE_BAROMETRIC_PRESSURE           = String(ABSOLUTE_BAROMETRIC_PRESSURE);
	    String StringO2_F_A_E_R_C_1_FUEL                    = String(O2_F_A_E_R_C_1_FUEL);
	    String StringCATALYST_TEMPERATURE_BANK1_SENSOR1     = String(CATALYST_TEMPERATURE_BANK1_SENSOR1);
	    String StringCONTROL_MODULE_VOLTAGE                 = String(CONTROL_MODULE_VOLTAGE);
	    String StringABSOLUTE_LOAD_VALUE                    = String(ABSOLUTE_LOAD_VALUE);
	    String StringFUEL_AIR_COMMANDED_EQUIV_RATIO         = String(FUEL_AIR_COMMANDED_EQUIV_RATIO);
	    String StringRELATIVE_THROTTLE_POSITION             = String(RELATIVE_THROTTLE_POSITION);
	    String StringAMBIENT_AIR_TEMPERATURE                = String(AMBIENT_AIR_TEMPERATURE);
	    String StringABSOLUTE_THROTTLE_B                    = String(ABSOLUTE_THROTTLE_B);
	    String StringACCELERATOR_PEDAL_POSITION_D           = String(ACCELERATOR_PEDAL_POSITION_D);
	    String StringACCELERATOR_PEDAL_POSITION_E           = String(ACCELERATOR_PEDAL_POSITION_E);
	    String StringFUEL_TYPE                              = String(FUEL_TYPE);
	    String StringFUEL_RAIL_ABSOLUTE_PRESSURE            = String(FUEL_RAIL_ABSOLUTE_PRESSURE);
	    
	    ////////////////////////////
	    //PUBLISH STRINGS TO BLYNK//
	    ////////////////////////////
	    // Blynk.virtualWrite(V0, StringMIL_STATUS);
	    // Blynk.virtualWrite(V1, StringFUEL_SYSTEM_STATUS);
	    // Blynk.virtualWrite(V2, StringENGINE_LOAD);
	    // Blynk.virtualWrite(V3, StringENGINE_COOLANT_TEMPERATURE);
	    // Blynk.virtualWrite(V4, StringSHORT_TERM_FUEL_TRIM_BANK_1);
	    // Blynk.virtualWrite(V5, StringLONG_TERM_FUEL_TRIM_BANK_1);
	    // Blynk.virtualWrite(V6, StringINTAKE_MANIFOLD_PRESSURE);
	    // Blynk.virtualWrite(V7, StringENGINE_RPM);
	    // Blynk.virtualWrite(V8, StringVEHICLE_SPEED);
	    // Blynk.virtualWrite(V9, StringTIMING_ADVANCE);
	    // Blynk.virtualWrite(V10, StringINTAKE_AIR_TEMPERATURE);
	    // Blynk.virtualWrite(V11, StringMAF_AIR_FLOW_RATE);
	    // Blynk.virtualWrite(V12, StringTHROTTLE);
	    // Blynk.virtualWrite(V13, StringO2_V_6);
	    // Blynk.virtualWrite(V14, StringO2_T_6);
	    // Blynk.virtualWrite(V15, StringDISTANCE_TRAVELED_WITH_MIL_ON);
	    // Blynk.virtualWrite(V16, StringCOMMANDED_EVAPORATIVE_PURGE);
	    // Blynk.virtualWrite(V17, StringFUEL_TANK_LEVEL_INPUT);
	    // Blynk.virtualWrite(V18, StringWARM_UPS_SINCE_CODES_CLEARED);
	    // Blynk.virtualWrite(V19, StringDISTANCE_TRAVELED_SINCE_CODES_CLEARED);
	    // Blynk.virtualWrite(V20, StringEVAPORATOR_SYSTEM_PRESSURE);
	    // Blynk.virtualWrite(V21, StringABSOLUTE_BAROMETRIC_PRESSURE);
	    // Blynk.virtualWrite(V22, StringO2_F_A_E_R_C_1_FUEL);
	    // Blynk.virtualWrite(V23, StringCATALYST_TEMPERATURE_BANK1_SENSOR1);
	    // Blynk.virtualWrite(V24, StringCONTROL_MODULE_VOLTAGE);
	    // Blynk.virtualWrite(V25, StringABSOLUTE_LOAD_VALUE);
	    // Blynk.virtualWrite(V26, StringFUEL_AIR_COMMANDED_EQUIV_RATIO);
	    // Blynk.virtualWrite(V27, StringRELATIVE_THROTTLE_POSITION);
	    // Blynk.virtualWrite(V28, StringAMBIENT_AIR_TEMPERATURE);
	    // Blynk.virtualWrite(V29, StringABSOLUTE_THROTTLE_B);
	    // Blynk.virtualWrite(V30, StringACCELERATOR_PEDAL_POSITION_D);
	    // Blynk.virtualWrite(V31, StringACCELERATOR_PEDAL_POSITION_E);
	    // Blynk.virtualWrite(V32, StringFUEL_TYPE);
	    // Blynk.virtualWrite(V33, StringFUEL_RAIL_ABSOLUTE_PRESSURE);
	     Particle.publish("RPM:", StringENGINE_RPM, 60, PRIVATE);

}

//////////////////////////////////////
//ALMOST EVERY MODE 1 OBDII PID MATH//
//////////////////////////////////////

void math() 
{

//void mathMIL_STATUS() {
    
    if (data2 == 2) {
        float MILStatus;
        MILStatus = (data3>>7) & 1;
        MIL_STATUS = MILStatus;
    }
//}

//void mathFUEL_SYSTEM_STATUS() {
    
    if (data2 == 3) 
    {
        float fuelSystemStatus;
        fuelSystemStatus = data3;
        FUEL_SYSTEM_STATUS = fuelSystemStatus;
    }
//}

//void mathENGINE_LOAD() {
    
    if (data2 == 4) {
        float load;
        load = (data3)/2.55;
        ENGINE_LOAD = load;
    }
//}

//void mathENGINE_COOLANT_TEMPERATURE() {
    
    if (data2 == 5) {
        float coolantTemp;
        coolantTemp = (data3)-40;
        ENGINE_COOLANT_TEMPERATURE = coolantTemp;
    }
//}

//void mathSHORT_TERM_FUEL_TRIM_BANK_1() {
    
    if (data2 == 6) {
        float stFuelTrimBank1;
        stFuelTrimBank1 = (data3/1.28 - 100);
        SHORT_TERM_FUEL_TRIM_BANK_1 = stFuelTrimBank1;
    }
//}

//void mathLONG_TERM_FUEL_TRIM_BANK_1() {
    
    if (data2 == 7) {
        float ltFuelTrimBank1;
        ltFuelTrimBank1 = (data3/1.28 - 100);
        LONG_TERM_FUEL_TRIM_BANK_1 = ltFuelTrimBank1;
    }
//}

//void mathSHORT_TERM_FUEL_TRIM_BANK_2() {
    
    if (data2 == 8) {
        float stFuelTrimBank2;
        stFuelTrimBank2 = (data3/1.28 - 100);
        SHORT_TERM_FUEL_TRIM_BANK_2 = stFuelTrimBank2;
    }
//}

//void mathLONG_TERM_FUEL_TRIM_BANK_2() {
    
    if (data2 == 9) {
        float ltFuelTrimBank2;
        ltFuelTrimBank2 = (data3/1.28 - 100);
        LONG_TERM_FUEL_TRIM_BANK_2 = ltFuelTrimBank2;
    }
//}

//void mathFUEL_PRESSURE() {
    
    if (data2 == 10) {
        float fuelPressure;
        fuelPressure = (data3*3);
        FUEL_PRESSURE = fuelPressure;
    }
//}

//void mathINTAKE_MANIFOLD_PRESSURE() {
    
    if (data2 == 11) {
        float intakeManifoldPressure;
        intakeManifoldPressure = (data3);
        INTAKE_MANIFOLD_PRESSURE = intakeManifoldPressure;
    }
//}

//void mathENGINE_RPM() {


    if (data2 == 12) {
        float RPM1;
        float RPM2;
        RPM1 = data3;
        RPM2 = data4;
        ENGINE_RPM = ((RPM1*256)+RPM2)/4;
    }
//}

//void mathVEHICLE_SPEED() {
    
    if (data2 == 13) {
        float vehicleSpeed;
        vehicleSpeed = (data3);
        VEHICLE_SPEED = vehicleSpeed;
    }
//}

//void mathTIMING_ADVANCE() {
	
	if (data2 == 14) {
	    float timingAdvance;
		timingAdvance = (data3/2 - 64);
		TIMING_ADVANCE = timingAdvance;
	}
//}

//void mathINTAKE_AIR_TEMPERATURE() {
	
	if (data2 == 15) {
	    float intakeAirTemperature;
		intakeAirTemperature = (data3 - 40);
		INTAKE_AIR_TEMPERATURE = intakeAirTemperature;
	}
//}

//void mathMAF_AIR_FLOW_RATE() {
	
	if (data2 == 16) {
	    float MAFAirFlowRate1;
	    float MAFAirFlowRate2;
		MAFAirFlowRate1 = data3;
		MAFAirFlowRate2 = data4;
		MAF_AIR_FLOW_RATE = ((MAFAirFlowRate1*256)+MAFAirFlowRate2)/4;
	}
//}

//void mathTHROTTLE() {
    
    if (data2 == 17) {
        float throttle;
        throttle = (data3)/2.55;
        THROTTLE = throttle;
    }
//}

//void mathCOMMANDED_SECONDARY_AIR_STATUS() {
    
    if (data2 == 18) {
        float commandedSecondaryAirStatus;
        commandedSecondaryAirStatus = (data3);
        COMMANDED_SECONDARY_AIR_STATUS = commandedSecondaryAirStatus;
    }
//}

//void mathO2_V_T_1(){
    
    if (data2 == 20) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_1 = o2Voltage1;
        O2_T_1 = o2Trim1;
    }
//}

//void mathO2_V_T_2(){
    
    if (data2 == 21) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_2 = o2Voltage1;
        O2_T_2 = o2Trim1;
    }
//}

//void mathO2_V_T_3(){
    
    if (data2 == 22) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_3 = o2Voltage1;
        O2_T_3 = o2Trim1;
    }
//}

//void mathO2_V_T_4(){
    
    if (data2 == 23) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_4 = o2Voltage1;
        O2_T_4 = o2Trim1;
    }
//}

//void mathO2_V_T_5(){

    if (data2 == 24) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_5 = o2Voltage1;
        O2_T_5 = o2Trim1;
    }
//}

//void mathO2_V_T_6(){
    
    if (data2 == 25) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_6 = o2Voltage1;
        O2_T_6 = o2Trim1;
    }
//}

//void mathO2_V_T_7(){
    
    if (data2 == 26) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_7 = o2Voltage1;
        O2_T_7 = o2Trim1;
    }
//}

//void mathO2_V_T_8(){
    
    if (data2 == 27) {
        float o2Voltage1;
        float o2Trim1;
        o2Voltage1 = data3/200;
        o2Trim1 = data4/1.28-100;
        O2_V_8 = o2Voltage1;
        O2_T_8 = o2Trim1;
    }
//}

//void mathENGINE_RUN_TIME() {
    
    if (data2 == 31) {
        float engineRunTime1;
        float engineRunTime2;
        engineRunTime1 = data3;
        engineRunTime2 = data4;
        ENGINE_RUN_TIME = (engineRunTime1*256+engineRunTime2);
    }
//}

//void mathDISTANCE_TRAVELED_WITH_MIL_ON() {
    
    if (data2 == 33) {
        float distanceTraveledWithMILOn1;
        float distanceTraveledWithMILOn2;
        distanceTraveledWithMILOn1 = data3;
        distanceTraveledWithMILOn2 = data4;
        DISTANCE_TRAVELED_WITH_MIL_ON = (distanceTraveledWithMILOn1*256+distanceTraveledWithMILOn2);
    }
//}

//void mathFUEL_RAIL_PRESSURE() {
    
    if (data2 == 34) {
        float fuelRailPressureA;
        float fuelRailPressureB;
        fuelRailPressureA = data3;
        fuelRailPressureB = data4;
        FUEL_RAIL_PRESSURE = 0.079*(256*fuelRailPressureA+fuelRailPressureB);
    }
//}

//void mathFUEL_RAIL_GAUGE_PRESSURE() {
    
    if (data2 == 35) {
        float fuelRailGaugePressureA;
        float fuelRailGaugePressureB;
        fuelRailGaugePressureA = data3;
        fuelRailGaugePressureB = data4;
        FUEL_RAIL_GAUGE_PRESSURE = 10*(256*fuelRailGaugePressureA+fuelRailGaugePressureB);
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_1() {
    
    if (data2 == 36) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_1_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_1_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_2() {
    
    if (data2 == 37) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_2_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_2_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_3() {
    
    if (data2 == 38) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_3_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_3_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_4() {
    
    if (data2 == 39) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_4_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_4_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_5() {
    
    if (data2 == 40) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_5_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_5_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_6() {
    
    if (data2 == 41) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_6_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_6_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_7() {
    
    if (data2 == 42) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_7_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_7_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_V_8() {
    
    if (data2 == 43) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float voltageC;
        float voltageD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        voltageC = data5;
        voltageD = data6;
        O2_F_A_E_R_V_8_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_V_8_VOLTAGE = (8/65536)*(256*voltageC+voltageD);
        
    }
//}

//void mathOBD_PID_COMMANDED_EGR() {
    
    if(data2 == 44) {
        float commandedEGR;
        commandedEGR = data3/2.55;
        COMMANDED_EGR = commandedEGR;
    }
//}

//void mathOBD_PID_EGR_ERROR() {
    
    if(data2 == 45) {
        float EGRError;
        EGRError = data3/1.28 - 100;
        EGR_ERROR= EGRError;
    }
//}

//void math_OBD_PID_COMMANDED_EVAPORATIVE_PURGE() {
    
    if(data2 == 46) {
        float commandedEvaporativePurge;
        commandedEvaporativePurge = data3/2.55;
        COMMANDED_EVAPORATIVE_PURGE = commandedEvaporativePurge;
    }
//}

//void mathOBD_PID_FUEL_TANK_LEVEL_INPUT() {
    
    if (data2 == 47) {
        float fuelTankLevelInput;
        fuelTankLevelInput = data3/2.55;
        FUEL_TANK_LEVEL_INPUT = fuelTankLevelInput;
    }
//}

//void mathOBD_PID_WARM_UPS_SINCE_CODES_CLEARED() {
    
    if (data2 == 48) {
        float warmUpsSinceCodesCleared;
        warmUpsSinceCodesCleared = data3;
        WARM_UPS_SINCE_CODES_CLEARED = warmUpsSinceCodesCleared;
    }
//}

//void mathOBD_PID_DISTANCE_TRAVELED_SINCE_CODES_CLEARED() {
    
    if (data2 == 49) {
        float distanceTraveledSinceCodesClearedA;
        float distanceTraveledSinceCodesClearedB;
        distanceTraveledSinceCodesClearedA = data3; 
        distanceTraveledSinceCodesClearedB = data4;
        DISTANCE_TRAVELED_SINCE_CODES_CLEARED = 256*distanceTraveledSinceCodesClearedA + distanceTraveledSinceCodesClearedB;
    }
//}

//void mathOBD_PID_EVAPORATOR_SYSTEM_PRESSURE() {
    
    if (data2 == 50) {
        float evaporatorSystemPressureA;
        float evaporatorSystemPressureB;
        evaporatorSystemPressureA = data3;
        evaporatorSystemPressureB = data4;
        EVAPORATOR_SYSTEM_PRESSURE = (256 * evaporatorSystemPressureA + evaporatorSystemPressureB)/4;
    }
//}

//void mathOBD_PID_ABSOLUTE_BAROMETRIC_PRESSURE() {
    
    if (data2 == 51) {
        float AbsoluteBarometricPressure;
        AbsoluteBarometricPressure = data3;
        ABSOLUTE_BAROMETRIC_PRESSURE = AbsoluteBarometricPressure;
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_1() {
    
    if (data2 == 52) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_1_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_1_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_2() {
    
    if (data2 == 53) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_2_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_2_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_3() {
    
    if (data2 == 54) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_3_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_3_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_4() {
    
    if (data2 == 55) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_4_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_4_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_5() {
    
    if (data2 == 56) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_5_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_5_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_6() {
    
    if (data2 == 57) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_6_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_6_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_7() {
    
    if (data2 == 58) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_7_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_7_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_O2_F_A_E_R_C_8() {
    
    if (data2 == 59) {
        float fuelAirRatioA;
        float fuelAirRatioB;
        float currentC;
        float currentD;
        fuelAirRatioA = data3;
        fuelAirRatioB = data4;
        currentC = data5;
        currentD = data6;
        O2_F_A_E_R_C_8_FUEL = (2/65536)*(256*fuelAirRatioA+fuelAirRatioB);
        O2_F_A_E_R_C_8_CURRENT = currentC+(currentD/256)-128;
        
    }
//}

//void mathOBD_PID_CATALYST_TEMPERATURE_BANK1_SENSOR1() {
    
    if (data2 == 60) 
    {
        float catalystTemperatureBank1Sensor1A;
        float catalystTemperatureBank1Sensor1B;
        catalystTemperatureBank1Sensor1A = data3;
        catalystTemperatureBank1Sensor1B = data4;
        CATALYST_TEMPERATURE_BANK1_SENSOR1 = (256 * catalystTemperatureBank1Sensor1A + catalystTemperatureBank1Sensor1B)/10 - 40;
    }
//}

//void mathOBD_PID_CATALYST_TEMPERATURE_BANK2_SENSOR1() {
    
    if (data2 == 61) 
    {
        float catalystTemperatureBank2Sensor1A;
        float catalystTemperatureBank2Sensor1B;
        catalystTemperatureBank2Sensor1A = data3;
        catalystTemperatureBank2Sensor1B = data4;
        CATALYST_TEMPERATURE_BANK2_SENSOR1 = (256 * catalystTemperatureBank2Sensor1A + catalystTemperatureBank2Sensor1B)/10 - 40;
    }
//}

//void mathOBD_PID_CATALYST_TEMPERATURE_BANK1_SENSOR2() {
    
    if (data2 == 62) 
    {
        float catalystTemperatureBank1Sensor2A;
        float catalystTemperatureBank1Sensor2B;
        catalystTemperatureBank1Sensor2A = data3;
        catalystTemperatureBank1Sensor2B = data4;
        CATALYST_TEMPERATURE_BANK1_SENSOR2 = (256 * catalystTemperatureBank1Sensor2A + catalystTemperatureBank1Sensor2B)/10 - 40;
    }
//}

//void mathOBD_PID_CATALYST_TEMPERATURE_BANK2_SENSOR2() {
    
    if (data2 == 63) {
        float catalystTemperatureBank2Sensor2A;
        float catalystTemperatureBank2Sensor2B;
        catalystTemperatureBank2Sensor2A = data3;
        catalystTemperatureBank2Sensor2B = data4;
        CATALYST_TEMPERATURE_BANK2_SENSOR2 = (256 * catalystTemperatureBank2Sensor2A + catalystTemperatureBank2Sensor2B)/10 - 40;
    }
//}

//void mathOBD_PID_CONTROL_MODULE_VOLTAGE() {
    
    if (data2 == 66) {
        float controlModuleVoltageA;
        float controlModuleVoltageB;
        controlModuleVoltageA = data3;
        controlModuleVoltageB = data4;
        CONTROL_MODULE_VOLTAGE = (256 * controlModuleVoltageA + controlModuleVoltageB)/1000;
    }
//}

//void mathOBD_PID_ABSOLUTE_LOAD_VALUE() {
    
    if (data2 == 67) {
        float AbsoluteLoadValueA;
        float AbsoluteLoadValueB;
        AbsoluteLoadValueA = data3;
        AbsoluteLoadValueB = data4;
        ABSOLUTE_LOAD_VALUE = (256 * AbsoluteLoadValueA + AbsoluteLoadValueB)/2.55;
    }
//}

//void mathOBD_PID_FUEL_AIR_COMMANDED_EQUIV_RATIO() {
    
    if (data2 == 68) {
        float fuelAirCommandedEquivRatioA;
        float fuelAirCommandedEquivRatioB;
        fuelAirCommandedEquivRatioA = data3;
        fuelAirCommandedEquivRatioB = data4;
        FUEL_AIR_COMMANDED_EQUIV_RATIO = (2 / 65536)*(256 * fuelAirCommandedEquivRatioA + fuelAirCommandedEquivRatioB);
    }
//}

//void mathOBD_PID_RELATIVE_THROTTLE_POSITION() {
    
    if (data2 == 69) {
        float relativeThrottlePosition;
        relativeThrottlePosition = data3/2.55;
        RELATIVE_THROTTLE_POSITION = relativeThrottlePosition;
    }
//}

//void mathOBD_PID_AMBIENT_AIR_TEMPERATURE() {
    
    if (data2 == 70) {
        float ambientAirTemperature;
        ambientAirTemperature = data3 - 40;
        AMBIENT_AIR_TEMPERATURE = ambientAirTemperature;
    }
//}

//void mathOBD_PID_ABSOLUTE_THROTTLE_B() {
    
    if (data2 == 71) {
        float AbsoluteThrottleB;
        AbsoluteThrottleB = data3/2.55;
        ABSOLUTE_THROTTLE_B = AbsoluteThrottleB;
    }
//}

//void mathOBD_PID_ABSOLUTE_THROTTLE_C() {
    
    if (data2 == 72) {
        float AbsoluteThrottleC;
        AbsoluteThrottleC = data3/2.55;
        ABSOLUTE_THROTTLE_C = AbsoluteThrottleC;
    }
//}

//void mathOBD_PID_ACCELERATOR_PEDAL_POSITION_D() {
    
    if (data2 == 73) {
        float acceleratorPedalPositionD;
        acceleratorPedalPositionD = data3/2.55;
        ACCELERATOR_PEDAL_POSITION_D = acceleratorPedalPositionD;
    }
//}

//void mathOBD_PID_ACCELERATOR_PEDAL_POSITION_E() {
    
    if (data2 == 74) {
        float acceleratorPedalPositionE;
        acceleratorPedalPositionE = data3/2.55;
        ACCELERATOR_PEDAL_POSITION_E = acceleratorPedalPositionE;
    }
//}

//void mathOBD_PID_ACCELERATOR_PEDAL_POSITION_F() {
    
    if (data2 == 75) {
        float acceleratorPedalPositionF;
        acceleratorPedalPositionF = data3/2.55;
        ACCELERATOR_PEDAL_POSITION_F = acceleratorPedalPositionF;
    }
//}

//void mathCOMMANDED_THROTTLE_ACTUATOR() {
    
    if (data2 == 76) {
        float commandedThrottleActuator;
        commandedThrottleActuator = data3/2.55;
        COMMANDED_THROTTLE_ACTUATOR = commandedThrottleActuator;
    }
//}

//void mathOBD_PID_TIME_RUN_WITH_MIL_ON() {
    
    if (data2 == 77) {
        float TimeRunWithMILOnA;
        float TimeRunWithMILOnB;
        TimeRunWithMILOnA = data3;
        TimeRunWithMILOnB = data4;
        TIME_RUN_WITH_MIL_ON = 256 * TimeRunWithMILOnA + TimeRunWithMILOnB;
    }
//}

//void mathOBD_PID_TIME_SINCE_TROUBLE_CODES_CLEARED() {
    
    if (data2 == 78) {
        float TimeSinceTroubleCodesClearedA;
        float TimeSinceTroubleCodesClearedB;
        TimeSinceTroubleCodesClearedA = data3;
        TimeSinceTroubleCodesClearedB = data4;
        TIME_SINCE_TROUBLE_CODES_CLEARED = 256 * TimeSinceTroubleCodesClearedA + TimeSinceTroubleCodesClearedB;
    }
//}
//TODO: FINISH MATH FOR THESE PARAMETERS
//void mathOBD_PID_MAX_VALUES() {
    
//}
//void mathOBD_PID_MAX_VALUES_2() {
    
//}
//void mathOBD_PID_FUEL_TYPE() {
    
    if (data2 == 81) {
        float fuelType;
        fuelType = data3;
        FUEL_TYPE = fuelType;
    }
//}
//void mathOBD_PID_ETHANOL_FUEL_PERCENT() {
    
    if (data2 == 82) {
        float ethanolFuelPercent;
        ethanolFuelPercent = data3/2.55;
        ETHANOL_FUEL_PERCENT = ethanolFuelPercent;
    }
//}
//void mathOBD_PID_ABSOLUTE_EVAP_SYS_VAPOR_PRESSURE() {
    
    if (data2 == 83) {
        float AbsoluteEvapSysVaporPressureA;
        float AbsoluteEvapSysVaporPressureB;
        AbsoluteEvapSysVaporPressureA = data3;
        AbsoluteEvapSysVaporPressureB = data4;
        ABSOLUTE_EVAP_SYS_VAPOR_PRESSURE = (256 * AbsoluteEvapSysVaporPressureA + AbsoluteEvapSysVaporPressureB)/200;
    }
    
//}
//void mathOBD_PID_EVAP_SYSTEM_VAPOR_PRESSURE() {
    
    if (data2 == 84) {
        float evapSystemVaporPressureA;
        float evapSystemVaporPressureB;
        evapSystemVaporPressureA = data3;
        evapSystemVaporPressureB = data4;
        EVAP_SYSTEM_VAPOR_PRESSURE = ((evapSystemVaporPressureA * 256) + evapSystemVaporPressureB) - 32767;
    }
//}
//TODO: FINISH MATH FOR THESE PARAMETERS

/*void mathOBD_PID_SHORT_TERM_SECONDARY_O2_TRIM_BANK_1_3() {

}
void mathOBD_PID_LONG_TERM_SECONDARY_O2_TRIM_BANK_1_3() {
    
}
void mathOBD_PID_SHORT_TERM_SECONDARY_O2_TRIM_BANK_2_4() {
    
}
void mathOBD_PID_LONG_TERM_SECONDARY_O2_TRIM_BANK_2_4() {
    
}
*/
//void mathOBD_PID_FUEL_RAIL_ABSOLUTE_PRESSURE() {
    
    if (data2 == 89) {
        float fuelRailAbsolutePressureA;
        float fuelRailAbsolutePressureB;
        fuelRailAbsolutePressureA = data3;
        fuelRailAbsolutePressureB = data4;
        FUEL_RAIL_ABSOLUTE_PRESSURE = (256 * fuelRailAbsolutePressureA + fuelRailAbsolutePressureB)*10;
    }
//}
//void mathOBD_PID_RELATIVE_ACCELERATOR_PEDAL_POSITION() {
    
    if (data2 == 91) {
        float relativeAcceleratorPedalPosition;
        relativeAcceleratorPedalPosition = data3/2.55;
        RELATIVE_ACCELERATOR_PEDAL_POSITION = relativeAcceleratorPedalPosition;
    }
//}
//void mathOBD_PID_HYBRID_BATTERY_PACK_REMAINING_LIFE() {
    
    if (data2 == 92) {
        float hybridBatteryPackRemainingLife;
        hybridBatteryPackRemainingLife = data3/2.55;
        HYBRID_BATTERY_PACK_REMAINING_LIFE = hybridBatteryPackRemainingLife;
    }
//}
//void mathOBD_PID_ENGINE_OIL_TEMPERATURE() {
    
    if (data2 == 93) {
        float engineOilTemperature;
        engineOilTemperature = data3 - 40;
        ENGINE_OIL_TEMPERATURE = engineOilTemperature;
    } 
//}
//void mathOBD_PID_FUEL_INJECTION_TIMING() {
    
    if (data2 == 94) {
        float fuelInjectionTimingA;
        float fuelInjectionTimingB;
        fuelInjectionTimingA = data3;
        fuelInjectionTimingB = data4;
        FUEL_INJECTION_TIMING = (256 * fuelInjectionTimingA + fuelInjectionTimingB)/128 - 210;
    }
//}
//void mathOBD_PID_ENGINE_FUEL_RATE() {
    
    if (data2 == 95) {
        float engineFuelRateA;
        float engineFuelRateB;
        engineFuelRateA = data3;
        engineFuelRateB = data4;
        ENGINE_FUEL_RATE = (256 * engineFuelRateA + engineFuelRateB)/20;
    }
//}

//TODO: FINISH MATH FOR THESE PARAMETERS
//void mathOBD_PID_EMISSION_REQUIREMENTS() {
    
//}
//void mathOBD_PID_SUPPORTED_PIDS_61_80() {
    
//}
//void mathOBD_PID_DEMAND_PERCENT_TORQUE() {
    
    if (data2 == 98) {
        float demandPercentTorque;
        demandPercentTorque = data3 - 125;
        DEMAND_PERCENT_TORQUE = demandPercentTorque;
    }
//}
//void mathOBD_PID_ACTUAL_PERCENT_TORQUE() {
    
    if (data2 == 99) {
        float actualPercentTorque;
        actualPercentTorque = data3 - 125;
        ACTUAL_PERCENT_TORQUE = actualPercentTorque;
    }
//}
//void mathOBD_PID_ENGINE_REFERENCE_TORQUE() {
    
    if (data2 == 100) {
        float engineReferenceTorqueA;
        float engineReferenceTorqueB;
        engineReferenceTorqueA = data3;
        engineReferenceTorqueB = data4;
        ENGINE_REFERENCE_TORQUE = 256 * engineReferenceTorqueA + engineReferenceTorqueB;
    }
}
//TODO: FINISH MATH FOR THESE PARAMETERS
//void mathOBD_PID_ENGINE_PERCENT_TORQUE_DATA() {
    
//}
//void mathOBD_PID_AUXILLARY_IO_SUPPORTED() {
    
//}



bool byteArray8Equal(uint8_t a1[8], uint8_t a2[8]) {
	for (int i = 0; i < 8; i++) {
		if (a1[i] != a2[i]) return false;
	}
	return true;
}
