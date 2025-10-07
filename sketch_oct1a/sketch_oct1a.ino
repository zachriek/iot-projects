#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN D5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi terkoneksi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba koneksi MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("terhubung");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi 5 detik...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca sensor DHT!");
    return;
  }

  char suhuStr[8];
  dtostrf(suhu, 1, 2, suhuStr);
  client.publish("sensor/suhu", suhuStr);

  char humStr[8];
  dtostrf(kelembapan, 1, 2, humStr);
  client.publish("sensor/kelembapan", humStr);

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" °C  |  Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println(" %");

  delay(5000);
}
