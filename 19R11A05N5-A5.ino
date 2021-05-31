//Assignment 5
//Develop a Webserver to monitor the weather data.

//complete the following tasks:
//Get the weather data from open weather map API using the HTTP protocol
//Create a webserver to display the weather parameters (Humidity and Temperature).




 #include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "Praveen reddy";
const char* password = "8013121408";
 
WiFiServer server(80);
 
String header;
 
String output26State = "off";
String output27State = "off";

 
const int output26 = 26;
const int output27 = 27;

 
unsigned long currentTime = millis();
 
unsigned long previousTime = 0; 
 
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  delay(3000);
 
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
 
  digitalWrite(output26, LOW);
  
  digitalWrite(output27, LOW);
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();  

  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");           
    String currentLine = "";                 
    while (client.connected() && currentTime - previousTime <= timeoutTime) {   
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                     
           
         
          if (currentLine.length() == 0) {
           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
             
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
 
    
            
          
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
           
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<body><h1>Smart Home Automation</h1>");
             
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
           
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
                 
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
             if ((WiFi.status() == WL_CONNECTED)) { 
 
    HTTPClient http;
 
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Hyderabad,IN&appid=faf753f080cf149bc462c69b5e7b2640"); 
    int httpCode = http.GET();
 
    if (httpCode > 0) {  
 
        String payload = http.getString();
        String Temp=payload.substring(147,160);
        
       String Humidity=payload.substring(233,246);
 
   
        
            client.println();
            client.println("<body><h1>API Weather Data</h1>");
            client.println("<h1>Temperature:</h1>");
            client.println(Temp);
            client.println();
            client.println("<h1>Humidity:</h1>");
            client.println(Humidity);
             http.end();  
  }}
      
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {   
          currentLine += c;      
        }
      }
    }
     
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
