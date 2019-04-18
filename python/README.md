# OBD II Information Logger and Diagnostics Reader

This repository is a backup copy of Joseph Varner project posted on [Hackster IO](https://www.hackster.io/jvarner4/obd-ii-information-logger-and-diagnostics-reader-952dd9)

- Capturing Live OBD-II Data

In order to get some live diagnostic data from the ECU

```
>> cd pyobd/
>> python live_capture.py
```

- Recording Live OBD-II Data

The logging function in the "Record data" script saves the sensor outputs that you predefine in the script as a .log file. This can be called in the terminal by typing:

```
>> cd pyobd/
>> python obd_recorder.py
```

I updated many of the calculations in this script to accurately print the gear ratio information in the terminal. I decided to log the time, engine rpm, speed (in MPH), and the fuel trim information. If you want to modify any of the logged parameters, you could do so in the "Record data" script.


# Reference

[Hackster IO](https://www.hackster.io/jvarner4/obd-ii-information-logger-and-diagnostics-reader-952dd9)

