# Particle Electron implementation

# PIDs read from car

Connections (Pinout)
===========
| Semtech SX1276/77/78/79 | Arduino UNO |
| :---------------------: | :------:|
| VCC | 3.3V |
| GND | GND |
| SCK | SCK |
| MISO | MISO |
| MOSI | MOSI (13) |
| NSS | 6 |
| NRESET | 5 |
| DIO0 | 2 |
| DIO1 | 3 |
| DIO2 | 4 |

# Serial console (Logger)

```
ls /dev/tty.*
screen /dev/tty.usbmodem1481 112500 
```

# References

[Telemtry Viewer](http://farrellf.com/TelemetryViewer/)
