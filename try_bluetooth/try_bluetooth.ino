char state;
HardwareSerial Serial1(PA10, PA9); //rx tx

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if(Serial1.available() > 0) {
    state = Serial1.read();
    Serial.println(state);
  }

  if (state == 'a') {
    digitalWrite(LED_BUILTIN, LOW); // Turn LED OFF
    Serial1.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
    
  }
  else if (state == 'b') {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.println("LED: ON");
    
  } 
}
