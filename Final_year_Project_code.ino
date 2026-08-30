#include <Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
 
// Motor control pins 
#define LEFT_MOTOR_IN1 2 
#define LEFT_MOTOR_IN2 3 
#define RIGHT_MOTOR_IN3 4 
#define RIGHT_MOTOR_IN4 5 
 
#define LEFT_ENABLE 9   // PWM pin for left motor speed control 
#define RIGHT_ENABLE 10 // PWM pin for right motor speed control 
 
// Ultrasonic sensor pins 
#define TRIG_PIN 8 
#define ECHO_PIN 7 
 
// Servo motor pin 
#define SERVO_PIN 6 
 
// Grass cutter motor relay pin 
#define RELAY_PIN 12 
 
Servo servo; // Create a servo object 
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD with I2C 
 
void setup() { 
  pinMode(LEFT_MOTOR_IN1, OUTPUT); 
  pinMode(LEFT_MOTOR_IN2, OUTPUT);  
  pinMode(RIGHT_MOTOR_IN3, OUTPUT);   
  pinMode(RIGHT_MOTOR_IN4, OUTPUT);  
  pinMode(LEFT_ENABLE, OUTPUT); 
  pinMode(RIGHT_ENABLE, OUTPUT); 
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);  
  pinMode(RELAY_PIN, OUTPUT); 
 
  servo.attach(SERVO_PIN);   
  servo.write(90); // Move servo to center position 
 
  lcd.begin(16, 2);   
  lcd.clear();
  lcd.print("Robot Ready!"); 
  Serial.begin(9600); 
} 
 
void loop() {   
  int distance = measureDistance(); 
 
  if (distance > 50) {    
    moveForward();    
    lcd.setCursor(0, 1);  
    lcd.print("Moving Forward");   
    digitalWrite(RELAY_PIN, LOW); // Grass cutter ON 
  } else {    
    stopMotors();  
  lcd.setCursor(0, 1);   
  lcd.print("Obstacle Detected"); 
  digitalWrite(RELAY_PIN, HIGH); // Grass cutter OFF     scanAndTurn(); 
  }   delay(100); 
} 
int measureDistance() {   
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2);   
  digitalWrite(TRIG_PIN, HIGH);   
  delayMicroseconds(10);   
  digitalWrite(TRIG_PIN, LOW);   
  long duration = pulseIn(ECHO_PIN, HIGH);  
  return duration * 0.034 / 2; // Convert to cm 
} 
 
void moveForward() {  
  digitalWrite(LEFT_MOTOR_IN1, HIGH);  
  digitalWrite(LEFT_MOTOR_IN2, LOW);   
  digitalWrite(RIGHT_MOTOR_IN3, HIGH); 
  digitalWrite(RIGHT_MOTOR_IN4, LOW);  
  analogWrite(LEFT_ENABLE, 255);   
  analogWrite(RIGHT_ENABLE, 255); 
} 
 
void moveBackward() { 
  lcd.setCursor(0, 1); 
  lcd.print("Moving Backward"); 
  digitalWrite(LEFT_MOTOR_IN1, LOW); 
  digitalWrite(LEFT_MOTOR_IN2, HIGH);  
  digitalWrite(RIGHT_MOTOR_IN3, LOW);  
  digitalWrite(RIGHT_MOTOR_IN4, HIGH); 
  analogWrite(LEFT_ENABLE, 200);  
  analogWrite(RIGHT_ENABLE, 200); 
  delay(1000);  
  stopMotors(); 
} 
 
void stopMotors() {   
  digitalWrite(LEFT_MOTOR_IN1, LOW);  
  digitalWrite(LEFT_MOTOR_IN2, LOW);  
  
  digitalWrite(RIGHT_MOTOR_IN3, LOW); 
  digitalWrite(RIGHT_MOTOR_IN4, LOW); 
} 
  
}
void turnLeft() {  
  lcd.setCursor(0, 1);   
  lcd.print("Turning Left..."); 
  digitalWrite(LEFT_MOTOR_IN1, LOW);  
  digitalWrite(LEFT_MOTOR_IN2, HIGH); 
  digitalWrite(RIGHT_MOTOR_IN3, HIGH); 
  digitalWrite(RIGHT_MOTOR_IN4, LOW);  
  analogWrite(LEFT_ENABLE, 255);
  analogWrite(RIGHT_ENABLE, 255); 
  delay(700); // Adjust duration for a 70-90 degree turn 
} 
 
void turnRight() { 
  lcd.setCursor(0, 1);  
  lcd.print("Turning Right..."); 
  digitalWrite(LEFT_MOTOR_IN1, HIGH);  
  digitalWrite(LEFT_MOTOR_IN2, LOW);
  digitalWrite(RIGHT_MOTOR_IN3, LOW);  
  digitalWrite(RIGHT_MOTOR_IN4, HIGH); 
  analogWrite(LEFT_ENABLE, 255);  
  analogWrite(RIGHT_ENABLE, 255); 
  delay(700); // Adjust duration for a 70-90 degree turn 
} 
 
void scanAndTurn() {  
  int leftDistance = 0; 
  int rightDistance = 0; 
 
  // Scan left 
  servo.write(0);   delay(500); 
  leftDistance = measureDistance(); 
  Serial.print("Left Distance: "); 
  Serial.println(leftDistance); 
    // Scan right 
    servo.write(180); 
    delay(500); 
  rightDistance = measureDistance(); 
  Serial.print("Right Distance: "); 
  Serial.println(rightDistance); 
 
  servo.write(90);  
  delay(500); 
 
  // Logic for turning based on obstacle detection 
  if (leftDistance < 50 && rightDistance >= 50) {    
    turnLeft(); // Turn left if obstacle is on the left side and right side is clear 
  } else if (rightDistance < 50 && leftDistance >= 50) {     
    turnRight(); // Turn right if obstacle is on the right side and left side is clear 
  } else {    
    stopMotors(); // Stop if both sides are blocked 
  } 
 
  delay(1000); // Allow time for movement   stopMotors(); 
} 
