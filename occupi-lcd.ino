#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

const char* SSID = "<SSID>";
const char* PSK = "<PASSWORD>";
const char* MQTT_BROKER = "192.168.178.191";

WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2);

char msg[50];

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
    client.setCallback(onMessage);

     // lcd setup
     lcd.begin(16,2);
     lcd.init();
     lcd.backlight();
     lcd.setCursor(0, 0);
     lcd.print("BATHROOM IS");
}

void loop() {
   if (!client.connected()) {
        reconnect();
    }
    client.loop();
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

void onMessage(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();
 
    msg[length] = '\0';
    lcd.setCursor(7, 1);
    lcd.print("    ");
    lcd.setCursor(3, 1);      
    lcd.print(msg);
    Serial.println(msg);
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Reconnecting MQTT...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    client.subscribe("BATHROOM");
    Serial.println("MQTT Connected...");
}
