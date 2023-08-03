//global variable to store motion detection status
bool moveDetected = false;

//global variable to store button change detection
bool btnChangeDetected = false;

//counter and compare values, as per provided material (500ms cycle on interrupt)
const uint16_t t1_load = 0;
const uint16_t t1_comp = 31250;

//global variable to toggle external LED state
int ledStateExternal = LOW;

void setup()
{
  //initialize Serial
  Serial.begin(9600);

  //pins for built-in LED as output and motion sensor as input
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);
  //added input pinMode delcaration for button sensor
  pinMode(2, INPUT);
  
  //added output pinmode for external LED (controlled by timer interrupt rather than input interrupt)
  pinMode(10, OUTPUT);
  
  //added 2 new inputs for PCInterrupt
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  
  //default value is LOW/off
  digitalWrite(10, ledStateExternal);
  
  //interrupt on digital pin 3 (RISING because we only want to interrupt on movement detected)
  attachInterrupt(digitalPinToInterrupt(3), motionDetectedSensor, RISING);
  //interrupt on digital pin 2 
  attachInterrupt(digitalPinToInterrupt(2), btnChangeDetectedSensor, RISING);
  
  //reset timer1 control register A
  TCCR1A = 0;
  
  //set CTC mode (reset timer upon compare match)
  TCCR1B &= ~(1 << WGM13); 
  TCCR1B |= (1 << WGM12); 

  //prescaler set to 256 (CS12 bit is 1, 11 and 10 are 0)
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  
  //load the previously assigned constant (0) into the timer
  TCNT1 = t1_load;
  //load the compare value constant for compare interrupt
  OCR1A = t1_comp;
  //enable compare interrupt for timer 1 
  TIMSK1 = (1 << OCIE1A);
  
  //PIN CHANGE INTTERRUPT BIT ASSIGNMENT
  //enable PCI2 on PCICR register
  PCICR = 0b00000100;
  //enable pins 5 and 6 in PCMSK2 register
  PCMSK2 = 0b01100000;
  
  //enable global interrupts
  sei();
}

//pin change interrupt function for relevant pin group
ISR(PCINT2_vect)
{
  //since cant set RISING flag, using if statement to check state 
  if (digitalRead(5) == HIGH || digitalRead(6) == HIGH)
  {
    //serial print line to show interrupt on pin set (pins 5 and 6)
    Serial.println("ALERT: PCInterrupt Sensor triggered");
  }
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
    //delay with light on
    delay(1000);
    
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
    //delay with light on
    delay(1000);
  
  }
  //if no motion detected, and no button change detected turn off the LED and print LED off to Serial Monitor
  else
  {
    //check to only happen when light is on (stops flood of LED off in serial)
    if (digitalRead(LED_BUILTIN) != LOW)
    {          
      digitalWrite(LED_BUILTIN, LOW);    
      Serial.println("LED OFF");
    }
  }
}

//interrupt function for timer1
ISR(TIMER1_COMPA_vect)
{  
  //toggle led state
  if (ledStateExternal == LOW) 
  {
  ledStateExternal = HIGH;
  } 
  else 
  {
  ledStateExternal = LOW;
  }
  //assign the toggled state using digital write
  digitalWrite(10, ledStateExternal);  
}