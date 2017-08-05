/*
 * This sketch reads data from the temperature sensor and sends it to the defined server.
 */
#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTTYPE DHT21

const char * SSID = "";
const char * Password = "";
const char * Server = "";
const uint16_t port = 8080;
const uint16_t id = 100;
const int DHTPIN = D2;
const int sleepTimeS = 10;
DHT dht(DHTPIN,DHTTYPE);


void setup() {
  
  Serial.begin(115200);
  delay(10);

  // Connect D0 to RST to wake up
  pinMode(D0, WAKEUP_PULLUP);

  
  String message = "Hello, World!";

  WiFi.mode(WIFI_STA);  
  WiFi.begin(SSID, Password);  
  Serial.println("Wait for Wifi...");

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("Connected. IP:");
  Serial.println(WiFi.localIP());

  delay(500);
  String temperature = getTemperature();
  String humidity = getHumidity();
  Serial.println(temperature + " " + humidity);
  message = "{\"stationId\": " + String(id) + ", " +
            "\"Temperature\": " + temperature + ", " +
            "\"Humidity\": " + humidity + "}";
  Serial.println(message);
  WiFiClient client;

  if(!client.connect(Server, port))
  {
    Serial.println("connection failed");
  }
  else
  {
     client.println("POST /api/stations HTTP/1.1");
     client.println("Host: " + String(Server) + ":" + String(port));
     client.println("Accept: */*");
     client.println("User-Agent: ESP8266 Custom");
     client.println("Content-Type: application/json");
     client.print("Content-Length: ");
     client.println(message.length());
     client.println();                
     client.print(message);
     delay(500);
     String line = client.readStringUntil('\r');
     Serial.println(line);
     if(client.connected())
     {
        client.stop();
     }
  }
  
   WiFi.disconnect(true);

   Serial.println("Sleeping for 20 minutes");
   ESP.deepSleep(sleepTimeS * 1000000);     

}

void loop() {

  
}

String getTemperature() {
  float temp = dht.readTemperature();
  char temperatureString[6];
  if(isnan(temp))
  {
  return "NaN";
  }
  else
  {
    dtostrf(temp, 2, 2, temperatureString);
  } 
  return temperatureString;
}
String getHumidity() {
  float hum = dht.readHumidity();
  char humidityString[6];
  if(isnan(hum))
  { 
  return "NaN";
  }
  else
  {
    dtostrf(hum, 2, 2, humidityString);
  }
  return humidityString;
}
