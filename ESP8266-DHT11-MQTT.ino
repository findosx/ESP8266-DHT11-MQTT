#include <dht11.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define DHT11PIN 4 // DHT11PIN olarak Dijital 2'yi belirliyoruz.z
dht11 DHT11;

const char* ssid = "enes";
const char* password = "123456789";
const char* mqtt_server = "192.168.1.42";


WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
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
   
  } 
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
      client.subscribe("espLED");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  Serial.println("\n");
  int chk = DHT11.read(DHT11PIN);
  int veri1=((int)DHT11.humidity);
  Serial.println("Nem (%): ");
  Serial.println(veri1);

  Serial.println("\n");
  int chk2 = DHT11.read(DHT11PIN);
  int veri2=((int)DHT11.temperature);
  Serial.println("Sıcaklık (Celcius):");
  Serial.println(veri2);

  Serial.println("\n");
  int chk3 = DHT11.read(DHT11PIN);
  int veri3=((int)DHT11.fahrenheit());
  Serial.println("Sıcaklık (Fahrenheit):");
  Serial.println(veri3);

  Serial.println("\n");
  int chk4 = DHT11.read(DHT11PIN);
  int veri4=((int)DHT11.kelvin());
  Serial.println("Sıcaklık (Kelvin):");
  Serial.println(veri4);

  Serial.println("\n");
  int chk5 = DHT11.read(DHT11PIN);
  int veri5=((int)DHT11.dewPoint());
  Serial.println("Çiğ Oluşma Noktası :");
  Serial.println(veri5);
  reconnect();
  client.loop();
  
  client.publish("nem", String(veri1).c_str(), true);
  client.publish("celcius", String(veri2).c_str(), true);
  client.publish("fahrenheit", String(veri3).c_str(), true);
  client.publish("kelvin", String(veri4).c_str(), true);
  client.publish("cig", String(veri5).c_str(), true);
  
  
  delay(2000);
     
  
     }
   
  
