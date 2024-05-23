#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Keke";
const char* password = "kekette11";

const char* mqtt_server = "264102e9ac51433ebbd21aaeb57014e9.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "arduino";
const char* mqtt_password = "Arduino1";

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;
int previousDistance = -1;

WiFiClient espClient;
PubSubClient client(espClient);

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
  Serial.println("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentative de connexion au serveur MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("connecté");
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  if (distance != previousDistance) {
    if (distance < 10) { 
      client.publish("home/porte", "Fermé");
      Serial.println("Fermé");
    } else {
      client.publish("home/porte", "Ouvert");
      Serial.println("Ouvert");
    }
    previousDistance = distance;
  }

  delay(5000);
}
