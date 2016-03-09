# BattTemp
CR2032 battery powered temperature data logger

This project uses an ST32L0 part and a thermister. It wakes up once per hour and logs the temperature to an external EEPROM device.
The system can store up to 1024 samples. This equates to slightly more than 42 days. The unit should run for over 2 years on a single 
CR2032 battery. 

The device also has a USB port for data offload. The interface is a simple serial console like system. It allows a person to read and set the 
