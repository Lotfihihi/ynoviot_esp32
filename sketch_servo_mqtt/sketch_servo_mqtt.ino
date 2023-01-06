#include "EspMQTTClient.h"
#include <ESP32Servo.h>
 
Servo myservo;
 
int pos = 0;
int servoPin = 33;

EspMQTTClient client(
  "Lotfi",
  "lotfihihi",
  "broker.emqx.io",  // MQTT Broker server ip
  "",   // Can be omitted if not needed
  "",   // Can be omitted if not needed
  "MQTTESP32Lotfihihi"      // Client name that uniquely identify your device
);

void setup() {
  Serial.begin(115200);
  client.enableMQTTPersistence();

  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);
	myservo.attach(servoPin, 500, 2400);
}

void onConnectionEstablished() {

  client.subscribe("esp32lotfihihi", [] (const String &payload)  {
    Serial.println(payload);
    if(payload == "Tourne !") {
      turnServo();
    }
  });

  client.publish("esp32lotfihihi", "ESP32 Connected to MQTT");
}

void turnServo() {
	myservo.write(180);
  delay(15);
	for (pos = 180; pos >= 0; pos -= 1) {
		myservo.write(pos);
		delay(15);
	}
}

void loop() {
  client.loop();
}