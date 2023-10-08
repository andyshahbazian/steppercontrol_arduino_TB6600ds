// Arduino based controller for rotating plate - expecting serial commands from jestson nanao - use softserial 
// tested with NEMA 14, micro stepper controller (TB6600) and Arduino Uno 
// to test open serial and type "m100 1000" this will move the shaft for 90 degree (adjust per your stepper) 100 will deinf e the movement and 500 is the steps defining the sped 
// to stop send n 
//so nano will send something like this on serial = m100 1000
//stepper then will move 90 degrees
//100 is the angle transalation and 1000 is the speed - you need to play with the values
//my micro stepper has 6 dip switches - s1 , s2, s3 = microsteps: I chose 400  (OFF ON ON) and s4, s5, s6 for current I set it to max 3 amp  OFF ON OFF



#include <AccelStepper.h>
const int buttonPin = 2; // Pin for the "read" button, change to your actual pin number
bool buttonState = HIGH; // Initial state of the button
bool lastButtonState = HIGH; // Previous state of the button

long receivedPosition = 0; //position sent from the serial command 
long receivedSteps = 0; //delay between two steps, received from the jestson nano - use serial 
long receivedAcceleration = 0; //acceleration value from serial command 
char receivedCommand; //character for commands
/* m = Move concat steps and speed values
 * o = open  // define steps and speed values
 * c = close  //define steps and speed values
 * a = set acceleration // define acceleration value
 * n = stop  now! //immedite stop of the servo
 */
 
bool newData, unlock = false; // boolean for new data from serial 
 
 
 
// direction Digital 9 (CCW), pulses Digital 8 (CLK)
AccelStepper stepper(1, 11, 10);
 
 
void setup()
{
  pinMode(buttonPin, INPUT_PULLUP); // Configure the button pin as INPUT_PULLUP
  Serial.begin(9600); //define baud rate
  Serial.println("Testing Accelstepper"); //print a message
 
  //setting up the default values for maximum speed and maximum acceleration
  stepper.setMaxSpeed(2000); //SPEED = Steps / second
  stepper.setAcceleration(1000); //ACCELERATION = Steps /(second)^2
  stepper.disableOutputs(); //disable outputs, so the motor is not getting warm (no current)

}
 
void loop()
{
   buttonState = digitalRead(buttonPin);
   if (buttonState == LOW && lastButtonState == HIGH) {
    // Call the readbuttonprocess function when D2 button is pressed - button is connected to d2 and will be grounded when dpressed 
    readButtonPressed();
  }
  checkSerial(); //check serial port for new commands
  continuousRun2(); //method to handle the motor
}
 
 
void continuousRun2() //method for the motor
{
  if (unlock == true)
  {
    if (abs(stepper.currentPosition()) < receivedPosition) //abs() is needed because of the '<'
    {
      stepper.enableOutputs(); //enable pins
      stepper.run(); //step the motor (this will step the motor by 1 step at each loop)
    }
    else //program enters this part if the required distance is completed
    {
      unlock = false; //disable running -> the program will not try to enter this if-else anymore
      stepper.disableOutputs(); // disable power
      Serial.print("POS: ");
      Serial.println(stepper.currentPosition()); // print pos -> this will show you the latest relative number of steps
      stepper.setCurrentPosition(0); //reset the position to zero
      Serial.print("POS: ");
      Serial.println(stepper.currentPosition()); // print pos -> this will show you the latest relative number of steps; we check here if it is zero for real
    }
 
 
  }
  else //program enters this part if the unlock is FALSE, we do not do anything
  {
    return;
  }
}
 
void checkSerial() //method for receiving the commands
{  
  
  if (Serial.available() > 0) //if something comes
  {
    receivedCommand = Serial.read(); // this will read the command character
    newData = true; //this creates a flag
  }
 
  if (newData == true) //if we received something (see above)
  {
    //START - MEASURE
    if (receivedCommand == 'm') //this is the movement command
    {
      //example m 100 500 - 100 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s would define our speed 
      unlock = true; //allow running
      receivedPosition = Serial.parseFloat(); //value for the steps
      receivedSteps = Serial.parseFloat(); //value for the speed
 
      Serial.print(receivedPosition); //print the values for checking
      Serial.print(receivedSteps);
      Serial.println("Measure "); //print the action
      stepper.setMaxSpeed(receivedSteps); //set speed
      stepper.move(receivedPosition); //set distance
 
    }
    //START - OPEN
    if (receivedCommand == 'o') //OPENING
    {
      //example o 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed
      unlock = true; //allow rotation
      receivedPosition = Serial.parseFloat(); //value for the steps
      receivedSteps = Serial.parseFloat(); //value for the speed
 
      Serial.print(receivedPosition); //print the values for checking
      Serial.print(receivedSteps);
      Serial.println("OPEN "); //print action
      stepper.setMaxSpeed(receivedSteps); //set speed
      stepper.move(receivedPosition); //set distance
 
    }
 
    //START - CLOSE
    if (receivedCommand == 'c') // - Rotates the motor in the opposite direction as starting with m
    {
      //example m 2000 500 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
      unlock = true; //allow running
  
      receivedPosition = Serial.parseFloat(); //value for the steps
      receivedSteps = Serial.parseFloat(); //value for the speed
 
      Serial.print(receivedPosition);  //print the values for checking
      Serial.print(receivedSteps);
      Serial.println("CLOSE "); //print action
      stepper.setMaxSpeed(receivedSteps); //set speed
      stepper.move(-1 * receivedPosition); ////set distance - negative value flips the direction
 
    }
 
    //STOP - STOP
    if (receivedCommand == 'n') //immediately stops the motor
    {
      unlock = false; //disable running
       
      stepper.setCurrentPosition(0); // reset position
      Serial.println("STOP "); //print action
      stepper.stop(); //stop motor
      stepper.disableOutputs(); //disable power
 
    }
 
    //SET ACCELERATION
    if (receivedCommand == 'a') //Setting up a new acceleration value
    {
      unlock = false; //we still keep running disabled, since we just update a variable
      receivedAcceleration = Serial.parseFloat(); //receive the acceleration from serial
      stepper.setAcceleration(receivedAcceleration); //update the value of the variable
      Serial.println("ACC Updated "); //confirm update by message
    }
 
  }
  //after we went through the above tasks, newData becomes false again, so we are ready to receive new commands again.
  newData = false;
}


void readButtonPressed() {
  // This is the function to be called when the "read" button is pressed
  // Add your code here to perform the desired action
  Serial.println("Read button pressed!");
  // Call your function or add the desired functionality here
  // For example, you can read a sensor value or perform any other action.
   unlock = true; //allow running
      receivedPosition = 100; //value for the steps
      receivedSteps = 500; //value for the speed
 
      Serial.print(receivedPosition); //print the values for checking
      Serial.print(receivedSteps);
      stepper.setMaxSpeed(receivedSteps); //set speed
      stepper.move(receivedPosition); //set distance
 
}
