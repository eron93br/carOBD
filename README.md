## carOBD dataset

This respository describes the OBDII dataset adquired during my master thesis. The dataset is composed by 27 different vehicle parameters (PIDs). The data acquisition system is composed by an embedded system connected to the On-Board Diagnostic II (OBD-II)interface of a Toyota Etios (year 2014, Engine Displacement1496 CC). The device used was the Carloop, an open-source development kit based on a microcontroller with cellular connectivity and OBD-II interface connector. The systemcaptures all vehicular data at a rate of one sample per second (1 Hz).

- Vehicle parameters

The following parameters are available in this dataset:

`ENGINE_RUN_TINE (),ENGINE_RPM (),VEHICLE_SPEED (),THROTTLE (),ENGINE_LOAD (),COOLANT_TEMPERATURE (),LONG_TERM_FUEL_TRIM_BANK_1 (),SHORT_TERM_FUEL_TRIM_BANK_1 (),INTAKE_MANIFOLD_PRESSURE (),FUEL_TANK (),ABSOLUTE_THROTTLE_B (),PEDAL_D (),PEDAL_E (),COMMANDED_THROTTLE_ACTUATOR (),FUEL_AIR_COMMANDED_EQUIV_RATIO (),ABSOLUTE_BAROMETRIC_PRESSURE (),RELATIVE_THROTTLE_POSITION (),INTAKE_AIR_TEMP (),TIMING_ADVANCE (),CATALYST_TEMPERATURE_BANK1_SENSOR1 (),CATALYST_TEMPERATURE_BANK1_SENSOR2 (),CONTROL_MODULE_VOLTAGE (),COMMANDED_EVAPORATIVE_PURGE (),TIME_RUN_WITH_MIL_ON (),TIME_SINCE_TROUBLE_CODES_CLEARED (),DISTANCE_TRAVELED_WITH_MIL_ON (),WARM_UPS_SINCE_CODES_CLEARED ()`

- Vehicle modes

The drive files refer to the car in the motion mode (moving on roads), differently, IDLE mode refers to the vehicle stopped in parking mode, but with the engine on. The files in the dataset correspond to specific conditions of the vehicle:

- `idleX.csv` refers to the vehicle in the IDLE mode
- `driveX.csv` refers to the vehicle in motion mode in high speed roads
- `liveX.csv` refers to the vehicle in a specific traject (work --> home)
- `ufpeX.csv` refers to the vehicle in motion mode inside an university campus (low speed)
- `longX.csv` refers to long trips with the vehicle in motion mode. 



# LICENSE

This is a open-source repository, free to everyone. *If you use PLEASE CITE MY ARTCILE AS REFERENCE OF THE DATABASE*. The work of my master thesis (`Detecting Anomalies in the Engine Coolant Sensor Using One-Class Classifiers`) are available ate [IEEE](https://ieeexplore.ieee.org/abstract/document/8891367)

# Firmware used on the Carloop implementation 

[Discover PIDs firmware](https://github.com/eron93br/carOBD/blob/master/firmware/discover-pids.ino)

[OBD-II logger firmware](https://github.com/eron93br/carOBD/blob/master/firmware/obd-logger.ino)

# Related projects

- [Pi Bluetooth OBD](https://github.com/eron93br/pi-bluetooth-obd)

- [openOBD](https://github.com/isand3r/openOBD)

- [OBD Information logger](https://github.com/eron93br/carOBD/tree/master/josephvaner)

- **Particle**: describes the files/projects related to the Particle Electron OBD vehicle data acquisition system
- **josephvaner**: describes an implementation based on OBD bluetooth dongle and Raspberry Pi Zero

# Master thesis References 

All the references are incuded in a JabRef file (`bib`) that can be imported and cited. 

[Jabref file with references](https://github.com/eron93br/carOBD/blob/master/jabref/mapeamento.bib)


