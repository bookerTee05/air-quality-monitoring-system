#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float t = 0.0;
float h = 0.0;

const int aqsensor = A0; 


const char *ssid = "Super user"; 
const char *pass = "qwertyui"; 

WiFiServer server(80);

void setup() {
 
  pinMode (aqsensor,INPUT);  

  dht.begin();

  Serial.begin (115200);

  

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  delay(5000);
  Serial.println(WiFi.localIP());
  delay(5000);
  server.begin();
}

void loop() {
  int ppm = analogRead(aqsensor); 

  Serial.print("Air Quality: ");  
  Serial.println(ppm); 
  float newT = dht.readTemperature();
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.println(t);
    }
    float newH = dht.readHumidity();
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.println(h);
    }           

  
  delay(1000);

  WiFiClient client = server.available();
  if ( client.connected()) { 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println("Refresh: 3"); 
  client.println();

 
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");

 
  client.println("<body><h1 style=\"color:black;\"> Air Quality Monitoring </h1>");
  client.println("<body><p style=\"color:blue;\"> Pollution Content(in PPM) = " + String(ppm) +" ppm"+ " </p>");
  
  if(ppm <= 130)
  {
    client.println("<body><p style=\"color:green;\"> Normal </p>");
    Serial.println("AQ Level Normal");
  }
  else if (ppm > 130 && ppm < 250)
  {
    client.println("<body><p style=\"color:purple;\"> Medium </p>");
    
    Serial.println("AQ Level Medium");
  }
  else
  {
    client.println("<body><p style=\"font-size:200%; color:red\"> Danger!!! </p>");
    
    Serial.println("AQ Level Danger!");     
    
  }

  client.println("<body><p style=\"color:blue;\"> Temperature(in &degC) = " + String(t) +"&degC"+ " </p>");
  client.println("<body><p style=\"color:blue;\"> Temperature(in %) = " + String(h) +"%"+ " </p>");

  client.println("</body></html>");
  delay(500);
  

}
}