#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN  ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <base64.h>
#include <DHT.h>

#define LED_LAMPU  D5
#define LED_AC     D6
#define LED_KULKAS D7
#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

String accountSID = "";
String authToken  = "";
String fromNumber = "";
String toNumber   = "";

bool statusLampu = false;
bool statusAC = false;
bool statusKulkas = false;

void updateStatus() {
  String statusMsg = "";

  statusMsg += statusLampu ? "Lampu Hidup, " : "Lampu Mati, ";
  statusMsg += statusAC ? "AC Hidup, " : "AC Mati, ";
  statusMsg += statusKulkas ? "Kulkas Hidup" : "Kulkas Mati";

  Serial.println(statusMsg);
  Blynk.virtualWrite(V5, statusMsg);

  sendWhatsAppTwilio(statusMsg);
}        

void sendWhatsAppTwilio(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + accountSID + "/Messages.json";
    http.begin(client, url);

    String auth = accountSID + ":" + authToken;
    String encodedAuth = base64::encode(auth);
    http.addHeader("Authorization", "Basic " + encodedAuth);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "To=" + urlencode(toNumber) +
                      "&From=" + urlencode(fromNumber) +
                      "&Body=" + urlencode(message);

    int httpCode = http.POST(postData);

    if (httpCode == 201) {
      Serial.println("Pesan WhatsApp terkirim via Twilio!");
    } else {
      Serial.println("Gagal kirim WhatsApp, kode: " + String(httpCode));
      Serial.println(http.getString());
    }

    http.end();
  } else {
    Serial.println("WiFi tidak terhubung!");
  }
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i =0; i < str.length(); i++) {
    c=str.charAt(i);
    if (isalnum(c)) {
      encodedString += c;
    } else {
      encodedString += '%';
      code1=(c & 0xf)+'0';
      if ((c & 0xf) >9) {
        code1=(c & 0xf) -10 + 'A';
      }
      code0=(c >> 4)+'0';
      if ((c >> 4) >9) {
        code0=(c >> 4) -10 + 'A';
      }
      encodedString += code0;
      encodedString += code1;
    }
  }
  return encodedString;
}

void sendDHT() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal baca sensor DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C, Kelembapan: ");
  Serial.print(humidity);
  Serial.println(" %");

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  statusLampu = value;
  digitalWrite(LED_LAMPU, value ? HIGH : LOW);
  updateStatus();
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  statusAC = value;
  digitalWrite(LED_AC, value ? HIGH : LOW);
  updateStatus();
}

BLYNK_WRITE(V4) {
  int value = param.asInt();
  statusKulkas = value;
  digitalWrite(LED_KULKAS, value ? HIGH : LOW);
  updateStatus();
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_LAMPU, OUTPUT);
  pinMode(LED_AC, OUTPUT);
  pinMode(LED_KULKAS, OUTPUT);

  digitalWrite(LED_LAMPU, LOW);
  digitalWrite(LED_AC, LOW);
  digitalWrite(LED_KULKAS, LOW);

  dht.begin();
  timer.setInterval(2000L, sendDHT);

  Serial.println("Menghubungkan ke Blynk...");
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  timer.run();
}
