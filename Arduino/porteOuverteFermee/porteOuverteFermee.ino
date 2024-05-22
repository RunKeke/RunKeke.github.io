#include <ESP8266WiFi.h>  // Utilisez <WiFi.h> pour ESP32
#include <PubSubClient.h>

// Paramètres WiFi
const char* ssid = "Keke";
const char* password = "kekette11";

// Paramètres MQTT
const char* mqtt_server = "";

// Pins du capteur à ultrasons
const int trigPin = 9;
const int echoPin = 10;

// Variables pour mesurer la distance
long duration;
int distance;
int previousDistance = -1;

WiFiClient espClient;
PubSubClient client(espClient);

// Fonction pour se connecter au WiFi
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

// Fonction de reconnexion au serveur MQTT
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

  // Envoi d'une impulsion de 10 microsecondes sur trigPin pour démarrer la mesure
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lecture du temps de retour de l'impulsion
  duration = pulseIn(echoPin, HIGH);

  // Calcul de la distance
  distance = duration * 0.034 / 2;

  // Comparer la distance actuelle avec la distance précédente
  if (distance != previousDistance) {
    if (distance < 10) {  // Ajuster ce seuil selon les besoins
      client.publish("home/porte", "Fermé");
      Serial.println("Fermé");
    } else {
      client.publish("home/porte", "Ouvert");
      Serial.println("Ouvert");
    }
    previousDistance = distance;
  }

  delay(5000);  // Attendre 5 secondes avant la prochaine mesure
}
