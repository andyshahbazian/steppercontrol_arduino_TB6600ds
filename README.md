# steppercontrol_arduino_TB6600ds
Arduino based controller for rotating plate - expecting serial commands from jestson nanao - use softserial  // tested with NEMA 14, micro stepper controller (TB6600) and Arduino Uno  // to test open serial and type "m100 1000" this will move the shaft for 90 degree (adjust per your stepper) 100 will define  the movement and 500 is the steps
![image](https://github.com/andyshahbazian/steppercontrol_arduino_TB6600ds/assets/16087670/2ff29e59-f850-452e-9c7e-04399ac37945)


ENA -   Open 
ENA +   ------------- +5  V   Orange |
DIR -  -> D10  (Black)               |
DIR +   ------------  +5  V   Ornage |----> +5V arduino Pin 5 from barrel connector side 
PUL -  -> D11  (Green)               |
PUL +   -------------  +5 V   Orange |

B-  M1
B + M11
A-  M2
A+  M22

GND - GND PS
VCC + +   PS


1 ON
2 Off 
3 ON
4 ON 
5 OFF
6 OFF 

