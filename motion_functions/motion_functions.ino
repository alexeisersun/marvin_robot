#define EN_A D2 //enable right motor
#define IN_1A D5 //right motor plus
#define IN_2A D4 //right motor minus

#define EN_B A4 //enable left motor
#define IN_1B A0 //left motor plus
#define IN_2B A1 //left motor minus


// M1 -- right motor
// M2 -- left motor

char input;
int power = 150;

void m1_init() {
  pinMode(EN_A, OUTPUT);
  pinMode(IN_1A, OUTPUT);
  pinMode(IN_2A, OUTPUT);
}

void m2_init() {
  pinMode(EN_B, OUTPUT);
  pinMode(IN_1B, OUTPUT);
  pinMode(IN_2B, OUTPUT);
}

void m1_driveForward(int power) {
  digitalWrite(EN_A, HIGH);
  analogWrite(IN_1A, power);
  digitalWrite(IN_2A, LOW);
}

void m1_driveBackwards(int power) {
  digitalWrite(EN_A, HIGH);
  digitalWrite(IN_1A, LOW);
  analogWrite(IN_2A, power);
}

void m2_driveForward(int power) {
  digitalWrite(EN_B, HIGH);
  analogWrite(IN_1B, power);
  digitalWrite(IN_2B, LOW);
}

void m2_driveBackwards(int power) {
  digitalWrite(EN_B, HIGH);
  digitalWrite(IN_1B, LOW);
  analogWrite(IN_2B, power);
}

void moveForward(int power) {
  m1_driveForward(power);
  m2_driveForward(power);
}

void moveBackwards(int power) {
  m1_driveBackwards(power);
  m2_driveBackwards(power);
}

void rotateLeft(int power) {
  m1_driveForward(power);
  m2_driveBackwards(power);
}

void rotateRight(int power) {
  m1_driveBackwards(power);
  m2_driveForward(power);
}

void turnLeft(int power) {
  m1_driveForward(power);
  m2_driveForward(power / 2);
}

void turnRight(int power) {
  m1_driveForward(power / 2);
  m2_driveForward(power);
}

void fullStop() {
  digitalWrite(EN_A, LOW);
  digitalWrite(EN_B, LOW);
}

void setup() {
  Serial.begin(9600);
  m1_init();
  m2_init();
  
}

void loop() {
  moveForward(power);
  delay(5000);
  fullStop();
  delay(1000);
}
