#include <ESP8266WiFi.h>  // Utilise <WiFi.h> pour ESP32
#include <PubSubClient.h>
#include <dht_nonblocking.h>
#include <pitches.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

const char* ssid = "Keke";
const char* password = "kekette11";

const char* mqtt_server = "264102e9ac51433ebbd21aaeb57014e9.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "arduino";
const char* mqtt_password = "Arduino1";

int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
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
  Serial.println("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void sonnerAlarme() {
  int i = 0;
  while (i <= 30) {
    tone(8, melody[1], duration);
    tone(8, melody[2], duration);
    i = i + 1;
  }
  char* payload = "ALARM_OFF"
  client.publish("home/alarm", payload.c_str())
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu [");
  Serial.print(topic);
  Serial.print("] : ");
  String message;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
  }

  if (message == "ALARM_ON") {
        sonnerAlarme
        Serial.println("Alarme activée !");
      }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentative de connexion au serveur MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connecté");
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop() {

  float temperature;
  float humidity;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if( measure_environment( &temperature, &humidity ) == true )
  {
    String payload = "La temperature dans la maison est de : " + String(temperature) + "°C et l'humidité est de : " + String(humidity) + "%";
    Serial.print("Envoi du message : ");
    Serial.println(payload);
    client.publish("home/temperature", (char*) payload.c_str());
  }

  
  delay(2000);
}
