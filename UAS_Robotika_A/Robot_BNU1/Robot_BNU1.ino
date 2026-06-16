/*************************************
 * Program : ROBOT BNU 4.0
 * Sesuai Buku - Robot IoT
 * ***********************************/

#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

String Topic;
String Payload;

const char* ssid = "HOTSPOT@UPNJATIM.AC.ID";
const char* password = "belanegara";

#define IN_1 D4 // Maju
#define IN_2 D3 // Mundur
#define IN_3 D7 // Belok Kanan
#define IN_4 D8 // Belok Kiri

//#define mqttServer "broker.emqx.io"
//#define mqttPort 1883

#define mqttServer "broker.emqx.io"
#define mqttPort 1883

WiFiServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);
  Serial.print("payload: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  /* we got '1' -> Maju */
  if ((char)payload[0] == '1') {
    digitalWrite(IN_1, HIGH);
    delay(100);  
    digitalWrite(IN_1, LOW);
  } 
  
/* we got '2' -> Kanan */
  if ((char)payload[0] == '2') {
    digitalWrite(IN_4, HIGH);   
    delay(300);
    digitalWrite(IN_1, HIGH);
    delay(100);  
    digitalWrite(IN_4, LOW);
    digitalWrite(IN_1, LOW);
  } 

  /* we got '3' -> Kiri */
  if ((char)payload[0] == '3') {
    digitalWrite(IN_3, HIGH);   
    delay(300);
    digitalWrite(IN_1, HIGH);
    delay(100);  
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_1, LOW);
  } 

  /* we got '4' -> Mundur */
  if ((char)payload[0] == '4') {
    digitalWrite(IN_2, HIGH);   
    delay(100);  
    digitalWrite(IN_2, LOW);
  } 
}
  
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
   
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
 
  server.begin();
  Serial.println("Server started");
 
  // Connect to Server IoT (CloudMQTT)

  client.setServer(mqttServer, mqttPort);
  client.setCallback(receivedCallback);
 
  while (!client.connected()) {
    Serial.println("Connecting to CLoudMQTT...");
 
    if (client.connect("RobotBNU_AhmadBaihaqi_23081010218")) {
 
      Serial.println("connected");
 
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("Maju");
  client.subscribe("Kiri");
  client.subscribe("Kanan");
  client.subscribe("Mundur");
}
 
void loop() { 

  client.loop();
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  } 
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  } 
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  if (request.indexOf("/IN_1on") > 0)  {
    digitalWrite(IN_1, HIGH);
    delay(100);  
    digitalWrite(IN_1, LOW);
  }
  if (request.indexOf("/IN_1off") >0)  {
    digitalWrite(IN_1, LOW);   
  }
   if (request.indexOf("/IN_2on") > 0)  {
    digitalWrite(IN_2, HIGH);   
    delay(100);  
    digitalWrite(IN_2, LOW);
  }
  if (request.indexOf("/IN_2off") >0)  {
    digitalWrite(IN_2, LOW);   
  }
   if (request.indexOf("/IN_3on") > 0)  {
    digitalWrite(IN_3, HIGH);   
    delay(300);
    digitalWrite(IN_1, HIGH);
    delay(100);  
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_1, LOW);
  }
  if (request.indexOf("/IN_3off") >0)  {
    digitalWrite(IN_3, LOW);   
  }
   if (request.indexOf("/IN_4on") > 0)  {
    digitalWrite(IN_4, HIGH);   
    delay(300);
    digitalWrite(IN_1, HIGH);
    delay(100);  
    digitalWrite(IN_4, LOW);
    digitalWrite(IN_1, LOW);
  }
  if (request.indexOf("/IN_4off") >0)  {
    digitalWrite(IN_4, LOW);   
  }
 
}
