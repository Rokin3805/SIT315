//global variable to store motion detection status
bool moveDetected = false;
//global variable to store button change detection
bool btnChangeDetected = false;

void setup()
{
  //initialize Serial
  Serial.begin(9600);

  //pins for built-in LED as output and motion sensor as input
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);
  //added input pinMode delcaration for button sensor
  pinMode(2, INPUT);
  
  //interrupt on digital pin 3 (RISING because we only want to interrupt on movement detected)
  attachInterrupt(digitalPinToInterrupt(3), motionDetectedSensor, RISING);
  //interrupt on digital pin 2 
  attachInterrupt(digitalPinToInterrupt(2), btnChangeDetectedSensor, RISING);

}

//function called by interrupt toggles flag
void motionDetectedSensor()
{
  //set the moveDetected flag to true when the interrupt is triggered (motion detected)
  moveDetected = true;
}

//second interrupt function for second sensor toggles relevant flag (button sensor and flag)
void btnChangeDetectedSensor()
{
  //delay for button fluctuation/bouncing
  delay(50);
  btnChangeDetected = true;
}

void loop()
{
  //if moveDetected is true, turn on the LED and print status to Serial Monitor
  if (moveDetected)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    //print state of flag boolean and LED on notification
    Serial.println("Move detected: ");
    Serial.println(moveDetected);
    Serial.println("LED ON");
    //reset the flag to false after processing
    moveDetected = false;
    
  }
  else if (btnChangeDetected)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    //print state of flag boolean and LED on notification
    Serial.println("Button change detected : ");
    Serial.println(btnChangeDetected);
    Serial.println("LED ON");
    //reset the flag to false after processing
    btnChangeDetected = false;
  
  }
  //if no motion detected, and no button change detected turn off the LED and print LED off to Serial Monitor
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED OFF");
  }

  delay(1000);
}