void setup()
{
  //initialize Serial
  Serial.begin(9600);

  //pins for builtin LED as output and motion sensor as input
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);
}

//Boolean as flag
bool moveDetected;

//input state is defaulted to low
int inputState = LOW;

void loop()
{	
  //defaults to no move detected	
  moveDetected = false;

  //read the state of the input pin
  inputState = digitalRead(3);

  //print the input state to Serial Monitor
  Serial.print("Input State: ");
  Serial.println(inputState);

  //if input state is high, boolean becomes true
  if (inputState == HIGH)
  {
    moveDetected = true;
  }

  //if flag is set to true, LED turns on
  if (moveDetected)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    //Print the status of the LED to Serial Monitor
    Serial.println("LED ON");
  }
  //if no longer moveDetected, turn LED off
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    //print the status of LED to Serial Monitor
    Serial.println("LED OFF");
  }

  //slight delay before re-entering loop
  delay(5);
}
