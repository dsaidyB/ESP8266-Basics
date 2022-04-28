#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Resources to start with
// https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
// https://www.youtube.com/watch?v=p06NNRq5NTU&app=desktop
// https://randomnerdtutorials.com/esp32-http-get-post-arduino/#http-post

// Replace with your network credentials
const char* ssid     = "wifi_network_name";
const char* password = "wifi_network_password";
const char* url = "web server";

int ledPin = 4;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(ledPin, LOW);

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
  
  client.setInsecure(); //quote online: "the magic line, use with caution"
  client.connect(url, 443);
  
  http.begin(client, url);

  String data;
  
  // send an http get request from the webserver in order to retrive LED status
  
  if (http.GET() > 0){    
    data = http.getString();
    Serial.println(data);
    http.end();
  }             
        
  if (data.indexOf("off")>-1){       // idk why LED high command is backwards on INBUILT LED
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(ledPin, LOW);
    }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(ledPin, HIGH);
  }

  delay(50);
}
