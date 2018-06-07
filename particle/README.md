# Particle Electron implementation

PIDs read from car
===========
| Semtech SX1276/77/78/79 | Arduino UNO |
| :---------------------: | :------:|
| Run time since engine start | 3.3V |
| Engine RPM | GND |
| Vehicle Speed | SCK |
| Intake manifold absolute pressure | SCK |
| Intake air temperature | MISO |
| Throttle position | MOSI (13) |
| Relative throttle position | 6 |
| Absolute throttle position B | 5 |
| Accelerator pedal position D | 2 |
| Accelerator pedal position E | 3 |
| Commanded throttle actuator | 4 |
| Fuel–Air commanded equivalence ratio | 4 |
| Fuel Tank Level Input | 4 |
| Short term fuel trim—Bank 1 | 4 |
| Long term fuel trim—Bank 1| 4 |
| Timing advance | 12 |
| Calculated engine load | GND |
| Engine coolant temperature | GND |
| Oxygen Sensor 1 A: Voltage B: Short term fuel trim | GND |
| Oxygen Sensor 2 A: Voltage B: Short term fuel trim | GND |

# Serial console (Logger)

```
ls /dev/tty.*
screen /dev/tty.usbmodem1481 112500 
```

# References

[Telemtry Viewer](http://farrellf.com/TelemetryViewer/)
