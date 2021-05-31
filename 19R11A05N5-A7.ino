//Assignment 7
//Develop a mobile application that takes the user input and sends it to IoT device (ESP32). Display the received data on the OLED display.


#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  delay(3000);
  SerialBT.begin("ESP32test");
delay(200);
Serial.println("oled test");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    for(;;);
}
 
}

void loop() { 
    display.clearDisplay(); 
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
  while(SerialBT.available()) {
   // Serial.write(SerialBT.read());
   char   a =SerialBT.read();
  
  Serial.write(a);
  
  delay(20);

 
  // Display static text
  display.write(a);
 display.display();

  
}

}

  

  
