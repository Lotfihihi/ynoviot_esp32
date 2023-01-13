#include "EspMQTTClient.h"
#include <ESP32Servo.h>

const int SERVO_2_PIN = 32;
const int SERVO_1_PIN = 33;
const int SERVO_50_PIN = 2;
const int SERVO_20_PIN = 4;
const int SERVO_10_PIN = 5;

const int IR_SENSOR_2_PIN = 26;
const int IR_SENSOR_1_PIN = 27;
const int IR_SENSOR_50_PIN = 12;
const int IR_SENSOR_20_PIN = 16;
const int IR_SENSOR_10_PIN = 14;

const int DEBOUNCE_TIMER = 50;

Servo servo2;
Servo servo1;
Servo servo50;
Servo servo20;
Servo servo10;

int posServo2 = 0;
int posServo1 = 0;
int posServo50 = 0;
int posServo20 = 0;
int posServo10 = 0;

bool is2Detected = false;
bool is1Detected = false;
bool is50Detected = false;
bool is20Detected = false;
bool is10Detected = false;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

float totalSum = 0;

void IRAM_ATTR triggeredSensor2() {
  is2Detected = true;
  lastTrigger = millis();
  startTimer = true;
}

void IRAM_ATTR triggeredSensor1() {
  is1Detected = true;
  lastTrigger = millis();
  startTimer = true;
}

void IRAM_ATTR triggeredSensor50() {
  is50Detected = true;
  lastTrigger = millis();
  startTimer = true;
}

void IRAM_ATTR triggeredSensor20() {
  is20Detected = true;
  lastTrigger = millis();
  startTimer = true;
}

void IRAM_ATTR triggeredSensor10() {
  is10Detected = true;
  lastTrigger = millis();
  startTimer = true;
}


// Initialisation de la connexion Wi-Fi/MQTT
EspMQTTClient client(
  "Galaxy Z Flip3", // SSID
  "lotfihihi", // Password
  "broker.emqx.io",  // MQTT Broker server ip
  "",   // Can be omitted if not needed
  "",   // Can be omitted if not needed
  "MQTTESP32Lotfihihi"      // Client name that uniquely identify your device
);


void setup() {
  Serial.begin(115200);
  client.enableMQTTPersistence();

  // Initialisation des pins des servos moteurs
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  
	servo2.setPeriodHertz(50);
	servo1.setPeriodHertz(50);
	servo50.setPeriodHertz(50);
	servo20.setPeriodHertz(50);
	servo10.setPeriodHertz(50);

	servo2.attach(SERVO_2_PIN, 500, 2400);
	servo1.attach(SERVO_1_PIN, 500, 2400);
	servo50.attach(SERVO_50_PIN, 500, 2400);
	servo20.attach(SERVO_20_PIN, 500, 2400);
	servo10.attach(SERVO_10_PIN, 500, 2400);

  // Initialisation des pins et interruptions des capteurs infrarouge
  pinMode(IR_SENSOR_2_PIN, INPUT);
  pinMode(IR_SENSOR_1_PIN, INPUT);
  pinMode(IR_SENSOR_50_PIN, INPUT);
  pinMode(IR_SENSOR_20_PIN, INPUT);
  pinMode(IR_SENSOR_10_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_2_PIN), triggeredSensor2, FALLING);
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_1_PIN), triggeredSensor1, FALLING);
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_50_PIN), triggeredSensor50, FALLING);
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_20_PIN), triggeredSensor20, FALLING);
  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_10_PIN), triggeredSensor10, FALLING);
}

void onConnectionEstablished() {

  client.subscribe("esp32lotfihihi", [] (const String &payload)  {
    Serial.println(payload);
    if(payload == "1") {
      turnServo2();
      totalSum -= 1;
    }
    if(payload == "2") {
      turnServo1();
      totalSum -= 2;
    }
    
  });

  client.publish("esp32lotfihihi", "ESP32 Connected to MQTT");
}

void turnServo2() {
	servo2.write(180);
  delay(15);
	for (posServo2 = 180; posServo2 >= 0; posServo2 -= 1) {
		servo2.write(posServo2);
		delay(15);
	}
}

void turnServo1() {
	servo1.write(180);
  delay(15);
	for (posServo1 = 180; posServo1 >= 0; posServo1 -= 1) {
		servo1.write(posServo1);
		delay(15);
	}
}

void turnServo50() {
	servo50.write(180);
  delay(15);
	for (posServo50 = 180; posServo50 >= 0; posServo50 -= 1) {
		servo50.write(posServo50);
		delay(15);
	}
}

void turnServo20() {
	servo20.write(180);
  delay(15);
	for (posServo20 = 180; posServo20 >= 0; posServo20 -= 1) {
		servo20.write(posServo20);
		delay(15);
	}
}

void turnServo10() {
	servo10.write(180);
  delay(15);
	for (posServo10 = 180; posServo10 >= 0; posServo10 -= 1) {
		servo10.write(posServo10);
		delay(15);
	}
}

void loop() {
  now = millis();
  client.loop();

  if(is1Detected == true) {
    if(startTimer == true && (now - lastTrigger) > DEBOUNCE_TIMER) {
      addToSum(1);
      is1Detected = false;
      startTimer = false;
    }
  }
  if(is2Detected == true) {
    if(startTimer == true && (now - lastTrigger) > DEBOUNCE_TIMER) {
      addToSum(2);
      is2Detected = false;
      startTimer = false;
    }
  }
  if(is50Detected == true) {
    if(startTimer == true && (now - lastTrigger) > DEBOUNCE_TIMER) {
      addToSum(0.50);
      is1Detected = false;
      startTimer = false;
    }
  }
  if(is20Detected == true) {
    if(startTimer == true && (now - lastTrigger) > DEBOUNCE_TIMER) {
      addToSum(0.20);
      is1Detected = false;
      startTimer = false;
    }
  }
  if(is10Detected == true) {
    if(startTimer == true && (now - lastTrigger) > DEBOUNCE_TIMER) {
      addToSum(0.10);
      is1Detected = false;
      startTimer = false;
    }
  }
}

void addToSum(float value) {
  totalSum += value;
  client.publish("esp32lotfihihi", String(totalSum));
}