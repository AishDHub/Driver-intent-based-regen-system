#if !defined(ARDUINO)
#define A0 14
#define A1 15
#define HIGH 1
#define LOW 0
#define OUTPUT 1

class HardwareSerial {
public:
  void begin(unsigned long) {}
  template <typename T>
  void print(T value) {}
  template <typename T>
  void println(T value) {}
  void print(const char* text) {}
  void println(const char* text) {}
};

static HardwareSerial Serial;

void pinMode(int, int) {}
void digitalWrite(int, int) {}
int analogRead(int) { return 0; }
void analogWrite(int, int) {}
void delay(unsigned long) {}
#else
#include <Arduino.h>
#endif

// ===== PIN DEFINITIONS =====
int voltagePin = A1;   // Voltage sensor output
int potPin = A0;       // Potentiometer (brake input)

int ENA = 9;           // Motor speed control
int IN1 = 8;           
int IN2 = 7;

// ===== VARIABLES =====
int brakeValue = 0;
int motorSpeed = 0;
float voltage = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Set motor direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void loop()
{
  // ===== READ BRAKE INPUT =====
  brakeValue = analogRead(potPin);

  // ===== DRIVER INTENT LOGIC =====
  if(brakeValue < 300)
  {
    motorSpeed = 200;   // Normal driving
  }
  else if(brakeValue >= 300 && brakeValue < 600)
  {
    motorSpeed = 130;   // Gentle braking
  }
  else if(brakeValue >= 600 && brakeValue < 900)
  {
    motorSpeed = 70;    // Medium braking
  }
  else
  {
    motorSpeed = 0;     // Emergency braking
  }

  // Apply motor speed
  analogWrite(ENA, motorSpeed);

  // ===== VOLTAGE SENSOR READING =====
  int sensorValue = analogRead(voltagePin);

  // Convert ADC to actual voltage (0–25V sensor)
  voltage = sensorValue * (25.0 / 1023.0);

  // ===== SERIAL OUTPUT =====
  Serial.print("Brake Value: ");
  Serial.print(brakeValue);

  Serial.print(" | Motor Speed: ");
  Serial.print(motorSpeed);

  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(300);
}
