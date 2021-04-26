#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


/*设置WIFI参数*/
const char* ssid = "602iot";
const char* password = "18wulian";
const char* mqtt_server = "82.156.189.164";

/*需要订阅的TOPIC*/
const char * LightTopic = "2018A14128/Light";


#define Red_PIN  21
#define Green_PIN  19
#define Blue_PIN  18

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (1024)
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

      if ((char)payload[0] == '1') {
      digitalWrite(Red_PIN, HIGH);
      }
      else if ((char)payload[0] == '2'){
        digitalWrite(Red_PIN, LOW);
          }
      else if ((char)payload[0] == '3'){
          digitalWrite(Green_PIN, HIGH);
          }
      else if((char)payload[0] == '4') {
              digitalWrite(Green_PIN, LOW);
          }
      else if ((char)payload[0] == '5'){
              digitalWrite(Blue_PIN, HIGH);
          }
      else if ((char)payload[0] == '6'){
              digitalWrite(Blue_PIN, LOW);
          }
    
 }

  // Switch on the LED if an 1 was received as first character
  /*if ((char)payload[0] == '1') {
    openLight();   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    closeLight();  // Turn the LED off by making the voltage HIGH
  }*/


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(LightTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(Red_PIN,OUTPUT);
  pinMode(Green_PIN,OUTPUT);
  pinMode(Blue_PIN,OUTPUT);
  /*MQTT服务器初始化*/
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
