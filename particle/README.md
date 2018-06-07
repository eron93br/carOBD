# Particle Electron implementation

PIDs read from car
===========
| Car parameter | Variable name |
| :---------------------: | :------:|
| Run time since engine start | ENGINE_RUN_TIME |
| Engine RPM | ENGINE_RPM |
| Vehicle Speed | VEHICLE_SPEED |
| Throttle position | THROTTLE |
| Calculated engine load | ENGINE_LOAD |
| Engine coolant temperature | COOLANT_TEMPERATURE |
| Long term fuel trim—Bank 1| LONG_TERM_FUEL_TRIM_BANK_1 |
| Short term fuel trim—Bank 1 | SHORT_TERM_FUEL_TRIM_BANK_1 |
| Intake manifold absolute pressure | INTAKE_MANIFOLD_PRESSURE |
| Fuel Tank Level Input | FUEL_TANK_LEVEL_INPUT |
| Absolute throttle position B | ABSOLUTE_THROTTLE_B |
| Accelerator pedal position D | ACCELERATOR_PEDAL_POSITION_D |
| Accelerator pedal position E | ACCELERATOR_PEDAL_POSITION_E |
| Commanded throttle actuator |  COMMANDED_THROTTLE_ACTUATOR |
| Fuel–Air commanded equivalence ratio |  FUEL_AIR_COMMANDED_EQUIV_RATIO |
| Absolute Barometric Pressure | ABSOLUTE_BAROMETRIC_PRESSURE |
| Relative throttle position | RELATIVE_THROTTLE_POSITION |
| Commanded throttle actuator | COMMANDED_THROTTLE_ACTUATOR | 
| Intake air temperature | INTAKE_AIR_TEMPERATURE |
| Timing advance | TIMING_ADVANCE |
| Catalyst Temperature: Bank 1, Sensor 1 | CATALYST_TEMPERATURE_BANK1_SENSOR1 |
| Catalyst Temperature: Bank 1, Sensor 2 | CATALYST_TEMPERATURE_BANK1_SENSOR2 |
| Control module voltage | CONTROL_MODULE_VOLTAGE |
| Commanded evaporative purge | COMMANDED_EVAPORATIVE_PURGE | 
| Time run with MIL on | TIME_RUN_WITH_MIL_ON |
| Time since trouble codes cleared | TIME_SINCE_TROUBLE_CODES_CLEARED |
| Distance traveled with malfunction indicator lamp (MIL) on | DISTANCE_TRAVELED_WITH_MIL_ON | 
| Warm-ups since codes cleared | WARM_UPS_SINCE_CODES_CLEARED | 






# Serial console (Logger)

```
ls /dev/tty.*
screen /dev/tty.usbmodem1481 112500 
```

# References

[Telemtry Viewer](http://farrellf.com/TelemetryViewer/)
