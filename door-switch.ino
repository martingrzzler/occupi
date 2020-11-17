#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int pin = 13;
int prevState = 1;

const char* SSID = "<SSID>";
const char* PSK = "<PASSWORD>";
const char* MQTT_BROKER = "192.168.178.191";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  pinMode(pin, INPUT_PULLUP);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  const int state = digitalRead(pin);

  if (state != prevState) {
    publish(state);
    prevState = state;
  }

  delay(200);
}

void publish(int state) {
  const char* message = state == LOW ? "OCCUPIED" : "FREE";
  client.publish("BATHROOM", message, true);  
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting MQTT...");
    if (!client.connect("ESP8266ClientDoorSwitch")) {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
  Serial.println("MQTT Connected...");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".,");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
