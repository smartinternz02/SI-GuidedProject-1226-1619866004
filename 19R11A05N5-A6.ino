//Assignment 6
//Develop a code to upload the ultrasonic sensor values and light intensity values to the IBM IoT platform and visualize them in the web application.


#include <WiFi.h>
#include <PubSubClient.h>
int echotime,distance;
int trig=5;
int echo=4;
String command;
String data="";

void callback(char* topic, byte* payload, unsigned int payloadLength);

const char* ssid = "Praveen reddy";
const char* password = "8013121408";

// CHANGE TO YOUR DEVICE CREDENTIALS AS PER IN IBM BLUMIX
#define ORG "whkvy8"
#define DEVICE_TYPE "ESP32"
#define DEVICE_ID "88854"
#define TOKEN "987654321" 



#define led1 2
#define led2 0


//-------- Customise the above values --------
const char publishTopic[] = "iot-2/evt/Data/fmt/json";
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";// cmd  REPRESENT command type AND COMMAND IS TEST OF FORMAT STRING
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;


WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

int publishInterval = 5000; // 30 seconds
long lastPublishMillis;
void publishData();

void setup() {
   pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(9600);
  Serial.println();
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);

  wifiConnect();
  mqttConnect();
}

void loop() {
 
 if (millis() - lastPublishMillis > publishInterval)
  {
    publishData();
    lastPublishMillis = millis();
  }
  
  if (!client.loop()) {
    mqttConnect();
  }
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected, IP address: "); 
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) 
  {
    Serial.print("Reconnecting MQTT client to ");
    Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    
    initManagedDevice();
    Serial.println();
  }
}

void initManagedDevice() {
  if (client.subscribe(topic)) {
   
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  
  Serial.print("callback invoked for topic: ");
  Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    
    command+= (char)payload[i];
  }
  
  Serial.print("data: "+ command);
  control_func();
  command= "";
}

void control_func()
{
  
   
  if(command== "lightoff")
 {

 
      digitalWrite(led2,LOW);
     Serial.println(".......lights are off..........");
    
  }
  else if(command== "lighton")
  {
   
      digitalWrite(led2,HIGH);
     Serial.println(".......lights are on..........");

  }

   
  else
  {
    Serial.println("......no commands have been subscribed..........");
   
    }
}

void publishData() 
{
 digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  echotime=pulseIn(echo,HIGH);
  distance=echotime*0.034/2;
  
  Serial.println(distance);
  delay(500);
  
  int intensity=analogRead(13);

  Serial.println(intensity);
  
  String payload = "{\"d\":{\"distance\":";
  payload += distance;
  
   payload += ",""\"intensity\":";
  payload +=  intensity;
  
  payload += "}}";


  Serial.print("\n");
  Serial.print("Sending payload: "); Serial.println(payload);

  if (client.publish(publishTopic, (char*) payload.c_str())) {
    Serial.println("Publish OK");
  } else {
    Serial.println("Publish FAILED");
  }
}
