#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>

#define dhtPin D0
#define dhtType 11
#define relayPinH D1
#define relayPinC D2

const char* ssid = "LAB_2.32";
const char* password = "LAB_2.32";

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);
DHT dht11(dhtPin, dhtType);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

const char* mqtt_server = "e3a0ad2ab69842fbb684a383f2306b47.s2.eu.hivemq.cloud";
const char* mqtt_username = "eregulation";
const char* mqtt_password = "eRegulation123!";
const int mqtt_port = 8883;

uint8_t heating_state = 0, cooling_state = 0, flag_temp = 0, flag_hum = 0;
float current_temp = 12, current_hum = 0;
int min_temp = 16, max_temp = 24, min_hum = 60, max_hum = 80;
unsigned long elapsedTime = 0;

/****** root certificate *********/
static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      mqttClient.subscribe("eregulation");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void publishMessage(const char* topic, String payload, boolean retained) {
  if (mqttClient.publish(topic, payload.c_str(), true))
    Serial.println("Message publised [" + String(topic) + "]: " + payload);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage += (char)payload[i]; 

  if (incommingMessage.compareTo("t-on") == 0) {
    flag_temp = 1;
  } else if (incommingMessage.compareTo("t-off") == 0) {
    flag_temp = 0;
  } else if (incommingMessage.compareTo("h-on") == 0) {
    flag_hum = 1;
  } else if (incommingMessage.compareTo("h-off") == 0) {
    flag_hum = 0;
  } else if (incommingMessage.startsWith("t")) {
    const char* msg = incommingMessage.c_str();
    min_temp = ((msg[2] - '0') * 10) + (msg[3] - '0');
    max_temp = ((msg[5] - '0') * 10) + (msg[6] - '0');
  } else if (incommingMessage.startsWith("h")) {
    const char* msg = incommingMessage.c_str();
    min_hum = ((msg[2] - '0') * 10) + (msg[3] - '0');
    max_hum = ((msg[5] - '0') * 10) + (msg[6] - '0');
  } else if (incommingMessage.compareTo("welcome") == 0) {
    char* welcomeMessage;
    sprintf(message, "t-%d-%d-%d-%d-h-%d-%d-%d-%d", current_temp, heating_state, min_temp, max_temp, current_hum, cooling_state, min_hum, max_hum);
    publishMessage("eregulation", welcomeMessage, true);
  } else if (incommingMessage.compareTo("ping") == 0) {
    char* pingMessage;
    sprintf(message, "t-%d-h-%d", current_temp, current_hum);
    publishMessage("eregulation", pingMessage, true);
  }
}

void ReadDHTValues() {
  current_temp = dht11.readTemperature();
  current_hum = dht11.readHumidity();
}

void RelayControl(uint8_t hState, uint8_t cState) {
  digitalWrite(relayPinH, hState);
  digitalWrite(relayPinC, cState);
}

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  while (!Serial)
    ;

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(WiFi.status());
    delay(200);
  }

  Serial.println("");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
  }

#ifdef ESP8266
  wifiClient.setInsecure();
#else
  wifiClient.setCACert(root_ca);
#endif

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);

  dht11.begin();

  pinMode(relayPinH, OUTPUT);
  pinMode(relayPinC, OUTPUT);
}

void loop() {
  if (millis() - elapsedTime > 60000) { 
    elapsedTime = millis();

    ReadDHTValues();

    char* message;
    sprintf(message, "t-%d-h-%d", current_temp, current_hum);
    publishMessage("eregulation", message, true);
  }

  if (flag_temp) {
    if (current_temp < min_temp) {
      heating_state = 1;
    } else if (current_temp > max_temp) {
      heating_state = 0;
    }
  } else {
    heating_state = 0;
  }

  if (flag_hum) {
    if (current_hum < min_hum) {
      cooling_state = 1;
    } else if (current_hum > max_hum) {
      cooling_state = 0;
    }

  } else {
    cooling_state = 0;
  }

  RelayControl(heating_state, cooling_state);
}
