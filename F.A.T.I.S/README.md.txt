Tips:
* Directly jump 3.3v and GND to SD Card or it won't work
* Set baud rate of Arducam OV2640 to 1,000,000 or the Serial will be grabbled
* The Catalex MicroSD module will occupy the MISO pin (pin 12 on UNO) even when it is not in use. Consequentially, the ArduCam OV2460 will not function if the Catalex MicroSD card module is powered. To circumvent this, power the Catalex MircoSD card module with a digital pin, set it to "HIGH" when you need to read/write and "LOW" at all other times.
* I couldn't figure out how to make subdirectories sorry