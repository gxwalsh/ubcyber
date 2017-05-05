#include <Servo.h>
#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char* ssid = "whimsybot-test";
const char WiFiAPPSK[] = "sparkfun";
 .
 



int val; //sets the command

Servo servoLeft;          // Define left servo
Servo servoRight;         // Define right servo

//setting the addresses
IPAddress ip(192,168,8,1);
IPAddress gateway(192, 168, 8, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80); //make a webserver
void setup() { 
  Serial.begin(115200);
  servoLeft.attach(D6);  // Set left servo to digital pin D6
  servoRight.attach(D7);  // Set right servo to digital pin D7

  setupWiFi(); //calls the function at the end
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address on the serial monitor
  Serial.println(WiFi.localIP());

  
} 

void loop() {            // Put whatever you want to execute repeatedly here.
  

  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  
  if (req.indexOf("/gpio/f") != -1)
    val = 1;
  else if (req.indexOf("/gpio/r") != -1)
    val = 2;
  
  else {
    Serial.println("invalid request");
    
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  //digitalWrite(2, val);

  if(val == 1) {
    forward(1);
  } else if (val==2) {
    turnRight(.25);
  } 

  
  client.flush(); //this clears the memory so it is ready for another set of instructions

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  //s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
  
}

// Motion routines for forward & right turn (about 45 degrees)
void forward(float time) {
  servoLeft.write(180);
  servoRight.write(0);
  delay(time*1000);
  val = 0;
  
}



void turnRight(float time) {
  servoLeft.write(180);
  servoRight.write(180);
  delay(time*1000);
  val = 0;
}




/////////////////////////////////
// DO NOT TOUCH BELOW HERE     //
/////////////////////////////////

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
 // String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
 //                String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  //macID.toUpperCase();
  String AP_NameString = ssid;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}