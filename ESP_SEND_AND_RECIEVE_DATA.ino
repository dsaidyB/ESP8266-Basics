#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NewPing.h>

// Replace with your network
const char* ssid     = "wifi_network_name";
const char* password = "wifi_network_password";
const char* url = "web server";

int ledPin = 4;

NewPing sense(13, 12, 100); 

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(ledPin, LOW);

  pinMode(13, OUTPUT); //send signal
  pinMode(12, INPUT); // recieves distance

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  WiFiClientSecure client;
  HTTPClient http;
  
  client.setInsecure();
  client.connect(url+"text file name or file in which you are storing LED status", 443);
  
  http.begin(client, url+"text file or file in which you are storing LED status");

  String data;
  if (http.GET() > 0){    
    data = http.getString();
    Serial.println(data);
    http.end();
  }             
  // led control 
  if (data.indexOf("on")>-1){       // idk why LED high command is backwards on INBUILT LED
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(ledPin, HIGH);
   }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(ledPin, LOW);
  }
  
  if (client.connected()) {
    client.stop();
  }

  
  // send ultrasonic sensor value to web server via a HTTP POST request
  String distValue;
  String postVariable = "dist=";
  int distance = sense.ping_cm();
  distValue = postVariable + String(distance);
  
  client.setInsecure();
  client.connect(url + "distance sensor data processing php file", 443);
  
  http.begin(client, url + "distance sensor data processing php file");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");           
  int httpResponseCode = http.POST(distValue);
  http.end();

  Serial.println("Distance is: " + distValue);
  
  delay(50);

}
