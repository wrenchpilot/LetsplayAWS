#include <M5StickC.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "secrets.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll = 0.0F;
float yaw = 0.0F;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

String readIMU()
{
  //Test Reading the IMU
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);

  String json;

  //Create JSON document
  DynamicJsonDocument doc(300);

  //doc["sensor"] = "imu";
  doc["gyroX"] = gyroX;
  doc["gyroY"] = gyroY;
  doc["gyroZ"] = gyroZ;
  //JsonArray gyro = doc.createNestedArray("gyro");
  //gyro.add(gyroX);
  //gyro.add(gyroY);
  //gyro.add(gyroZ);

  doc["accX"] = accX;
  doc["accY"] = accY;
  doc["accZ"] = accZ;
  //JsonArray acc = doc.createNestedArray("acc");
  //acc.add(accX);
  //acc.add(accY);
  //acc.add(accZ);

  doc["pitch"] = pitch;
  doc["roll"] = roll;
  doc["yaw"] = yaw;
  //JsonArray ahrs = doc.createNestedArray("ahrs");
  //ahrs.add(pitch);
  //ahrs.add(roll);
  //ahrs.add(yaw);

  serializeJson(doc, json);

  return json;
}

void publishMessage()
{
  client.publish(AWS_IOT_PUBLISH_TOPIC, readIMU());
}

void messageHandler(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  //  StaticJsonDocument<200> doc;
  //  deserializeJson(doc, payload);
  //  const char* message = doc["message"];
}

void setup()
{
  M5.begin();
  M5.IMU.Init();
  Serial.begin(9600);
  connectAWS();
}

void loop()
{
  publishMessage();
  client.loop();
  delay(100);
}
