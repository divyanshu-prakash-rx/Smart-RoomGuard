//Below is the code of my Smart-RoomGuard Project
//For any help feel free to reach out to me

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <LiquidCrystal.h>
#include "MQ135.h"
#include "DHT.h"

//dht pins
#define DHTPIN 4
#define relay 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int flag = 0;

//thingspeak and telegram integration
String apiKey = "#api_key"; //enter apikey of thingspeak
const char *ssid = "#Wifi_name";  //enter name of Wifi
const char *pass = "#Wifi_password"; //enter password of Wifi network
#define BOTtoken "#bot_token"     //enter telegram bot token of the bot from botfather 
#define CHAT_ID "#chat_id"    //enter your own chat_id so that bot can use that to send message
#define REPORTING_PERIOD_MS 1000
const char *server = "api.thingspeak.com";  //thingspeak server integration
WiFiClient client2;
WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);
//lcd pins
LiquidCrystal My_LCD(22, 23, 5, 18, 19, 21);

void setup() {
  Serial.begin(115200);
  My_LCD.begin(16, 2);
  My_LCD.clear();
  //gas sensor pins
  pinMode(16, OUTPUT);     //Buzzer will be output to ESP32
  pinMode(relay, OUTPUT);  //LOCK
  pinMode(17, OUTPUT);     //Red Led will be output to ESP32
  pinMode(34, INPUT);      //Gas sensor will be an input to the ESP32
  digitalWrite(relay, HIGH);
  digitalWrite(17, LOW);
  delay(10);

  //DHT
  dht.begin();
  //Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // bot.sendMessage(CHAT_ID, "Wifi connected!");
}

void loop() {
  //gas sensor
  MQ135 gasSensor = MQ135(34);
  float air_quality;
  air_quality = getVoltage(34);
  
  //dht
  float Humidity = dht.readHumidity();
  float Temperature = dht.readTemperature();

  if (isnan(Humidity) || isnan(Temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(1000);
    return;
  }


  if (air_quality > 9.5 || Temperature > 200) {
    flag = 1;
    delay(10);
    tone(16, 1000, 1000);  //Buzzer is ON
    digitalWrite(17, HIGH);
    My_LCD.clear();
    My_LCD.setCursor(5, 0);
    My_LCD.print("WARNING!!");
    My_LCD.setCursor(2, 1);
    My_LCD.print("FIRE DETECTED");
    bot.sendMessage(CHAT_ID, "Warning!! Fire Detected");
    delay(2000);
    digitalWrite(relay, LOW);  //emergency lock is OPEN
    bot.sendMessage(CHAT_ID, "Emergency Gate Opened");
    bot.sendMessage(CHAT_ID, "Check Room Condition here : https://thingspeak.com/channels/2363375");
    My_LCD.clear();
    delay(10);
    My_LCD.setCursor(1, 0);
    My_LCD.print("EMERGENCY GATE");
    My_LCD.setCursor(5, 1);
    My_LCD.print("OPENED");
    delay(1000);
    // digitalWrite(17, LOW);


    Serial.println("Warning!! Fire Detected");
    Serial.print(" Gas Level:");
    Serial.println(air_quality);
    Serial.print("Humidity: ");
    Serial.print(Humidity);
    Serial.print("%  Temperature: ");
    Serial.print(Temperature);
    Serial.println("°C ");
    Serial.println("");
    delay(1000);
  }
  if (air_quality < 9.5 && Temperature < 200) {
    if (flag == 1) {
      flag = 0;
      //Buzzer is OFF
      digitalWrite(relay, HIGH);  //emergency lock is OFF
      digitalWrite(17, LOW);      //RED LED id OFF
      bot.sendMessage(CHAT_ID, "Condition Stabilized");
      Serial.print("Humidity: ");
      Serial.print(Humidity);
      Serial.print("%  Temperature: ");
      Serial.print(Temperature);
      Serial.print("°C ");
      Serial.print("  Gas Level:");
      Serial.println(air_quality);
      //LCD

      tone(16, 1000, 1000);
      My_LCD.clear();
      My_LCD.setCursor(4, 0);
      My_LCD.print("Condition");
      My_LCD.setCursor(3, 1);
      My_LCD.print("Stabilized");

      tone(16, 0, 1000);
      delay(1000);
      My_LCD.clear();
      delay(100);
      My_LCD.setCursor(0, 0);
      My_LCD.print("H     T     GL");
      My_LCD.setCursor(0, 1);
      My_LCD.print(Humidity);
      My_LCD.setCursor(6, 1);
      My_LCD.print(Temperature);
      My_LCD.setCursor(12, 1);
      My_LCD.print(air_quality);
    } else {
      tone(16, 0, 1000);
      My_LCD.clear();             //Buzzer is OFF
      digitalWrite(relay, HIGH);  //emergency lock is OFF
      digitalWrite(17, LOW);      //RED LED id OFF
      Serial.print("Humidity: ");
      Serial.print(Humidity);
      Serial.print("%  Temperature: ");
      Serial.print(Temperature);
      Serial.print("°C ");
      Serial.print("  Gas Level:");
      Serial.println(air_quality);
      //LCD
      My_LCD.setCursor(0, 0);
      My_LCD.print("H     T     GL");
      My_LCD.setCursor(0, 1);
      My_LCD.print(Humidity);
      My_LCD.setCursor(6, 1);
      My_LCD.print(Temperature);
      My_LCD.setCursor(12, 1);
      My_LCD.print(air_quality);
    }

    // delay(1000);
  }

  // thing speak
  if (client2.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    Serial.println("Sending....");
    String sendData = apiKey + " &field1=" + (air_quality) + " &field2=" + (Temperature) + " &field3=" + (Humidity) + "r\n";
    // Serial.println(sendData);

    client2.print("POST /update HTTP/1.1\n");
    client2.print("Host: api.thingspeak.com\n");
    client2.print("Connection: close\n");
    client2.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client2.print("Content-Type: application/x-www-form-urlencoded\n");
    client2.print("Content-Length: ");
    client2.print(sendData.length());
    client2.print("\n\n");
    client2.print(sendData);

    delay(1000);
  } else {
    Serial.println("Server Not found");
  }

  client2.stop();

  delay(1000);
}
float getVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}
