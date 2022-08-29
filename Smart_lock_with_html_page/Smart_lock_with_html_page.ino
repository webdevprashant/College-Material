#include <ESP8266WiFi.h>
 
const char* ssid = "Redmi";
const char* password = "12345678";
 
int Solenoid = 13; // GPIO13---D7 of NodeMCU
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(Solenoid, OUTPUT);    //D7 as output
  digitalWrite(Solenoid, LOW);  //Deactivate Solenoid
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
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
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/Solenoid=ON") != -1)  {
    digitalWrite(Solenoid, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/Solenoid=OFF") != -1)  {
    digitalWrite(Solenoid, LOW);
    value = LOW;
  }
 
// Set Solenoid according to the request
//digitalWrite(Solenoid, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Solenoid is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/Solenoid=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/Solenoid=OFF\"\"><button>Off </button></a><br />");  
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println(""); 
}
