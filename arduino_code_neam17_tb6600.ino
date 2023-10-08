Arduino based controller for rotating plate - expecting serial commands from jestson nanao - use softserial 
// tested with NEMA 14, micro stepper controller (TB6600) and Arduino Uno 
// to test open serial and type "m100 1000" this will move the shaft for 90 degree (adjust per your stepper) 100 will deinf e the movement and 500 is the steps defining the sped 
// to stop send n 
//so nano will send something like this on serial = m100 1000
//stepper then will move 90 degrees
//100 is the angle transalation and 1000 is the speed - you need to play with the values
//my micro stepper has 6 dip switches - s1 , s2, s3 = microsteps: I chose 400  (OFF ON ON) and s4, s5, s6 for current I set it to max 3 amp  OFF ON OFF
