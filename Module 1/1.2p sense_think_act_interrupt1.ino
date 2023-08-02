//global variable to store motion detection status
bool moveDetected = false;

void setup()
{
  //initialize Serial
  Serial.begin(9600);

  //pins for built-in LED as output and motion sensor as input
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);

  //interrupt on digital pin 3 (RISING because we only want to interrupt on movement detected)
  attachInterrupt(digitalPinToInterrupt(3), motionDetectedSensor, RISING);
}

//function called by interrupt toggles flag
void motionDetectedSensor()
{
  //set the moveDetected flag to true when the interrupt is triggered (motion detected)
  moveDetected = true;
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
    // Reset the flag to false after processing
    moveDetected = false;
    
  }
  //if no motion detected, turn off the LED and print LED off to Serial Monitor
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED OFF");
  }

  delay(1000);
}