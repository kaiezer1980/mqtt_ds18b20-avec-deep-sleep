#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4 // pin 4
 

OneWire oneWire(ONE_WIRE_BUS);
 
// Passez la référence oneWire à Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 

 
const char* ssid = "*****";
const char* password = "*****";
const char* mqtt_server = "*****"; /// MQTT Broker
int mqtt_port = 1883;
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
 
void setup() { 
 Serial.begin(115200);
 // Start up the library
 sensors.begin();
 
 setup_wifi();
 client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);
 
 Serial.println("Connecté ");
 Serial.print("MQTT Server ");
 Serial.print(mqtt_server);
 Serial.print(":");
 Serial.println(String(mqtt_port)); 
 Serial.print("ESP8266 IP ");
 Serial.println(WiFi.localIP()); 
 Serial.println("Modbus RTU Master Online");
 
 
 
}
 
void setup_wifi() {
 
 delay(10);
 
 Serial.println();
 Serial.print("Connexion à ");
 Serial.println(ssid);
 
 WiFi.begin(ssid, password);
 
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 
 Serial.println("");
 Serial.println("WiFi connecté");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i = 0; i < length; i++) {
 Serial.print((char)payload[i]);
 }
 Serial.println();
 
}
void reconnect() {
 
 while (!client.connected()) {
 Serial.print("Tentative de connexion MQTT ...");
 
 if (client.connect("ESP8266Client")) {
 
 Serial.println("connecté");
 // client.subscribe("event");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" réessayez dans 5 secondes");
 
 delay(5000);
 }
 }
}
void loop() {
 
 sensors.requestTemperatures();
 float celsius = sensors.getTempCByIndex(0);
 Serial.println(sensors.getTempCByIndex(0));
 
 char temperaturenow [15];
 dtostrf(celsius,7, 3, temperaturenow); 
 client.publish("temperature/belka/sensor", temperaturenow); 
 
 if (!client.connected()) {
 reconnect();
 delay(2000);
 }
 client.loop();
 
 delay(2000); 
 ESP.deepSleep(600e6);
 delay(2000); 
}
