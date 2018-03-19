/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  
char server[] = "requestb.in/1cjoi0z1";

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

bool connected = false; 

OneWire  ds(9);  // on pin 10 (a 4.7K resistor is necessary)

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

}

void loop() {

  // if you get a connection, report back via serial:
  if (!connected) {

    delay(250);
    String inputTemperature = readTemperature();
    
    if (inputTemperature == "") {
      return;
    }
     
    if (client.connect(server, 80)) {  
      String temperaturePart = "{\"temperature\": \"" + inputTemperature + "\"}";
      Serial.print("connected to server");
      // Make a HTTP request:
      sendMessage(client, temperaturePart);

      //Serial.println("Content-Length: " + temperaturePart.length());
      //Serial.println(temperaturePart);
      connected = true;
      Serial.println("done");
    } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
      connected = false;
      delay(1000);
    }
  } else {
    // if there are incoming bytes available
    // from the server, read them and print them:
    if (client.available()) {
      char c = client.read();
      //Serial.print(c);
    }
  
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      
      connected = false;
      delay(60000);
    }
  }
 
}


