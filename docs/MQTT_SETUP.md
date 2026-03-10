# Setting Up HiveMQ Cloud MQTT Broker

HiveMQ Cloud is a managed MQTT broker service that allows you to connect IoT devices easily. Here’s how to set it up for free and configure MQTT connections for ESP32 and Android clients.

## Step 1: Create a HiveMQ Cloud Account
1. Go to [HiveMQ Cloud](https://www.hivemq.com/cloud/) and sign up for a free account.
2. Once logged in, create a new cluster. Follow the on-screen instructions.

## Step 2: Retrieve Broker Credentials
1. After creating the cluster, go to the cluster details page.
2. Note the following credentials:
   - **Broker URL**: This is the endpoint your clients will connect to.
   - **Port**: Usually, it’s 8883 for secure MQTT connections.
   - **Username & Password**: The credentials required to authenticate your MQTT clients.

## Step 3: Topic Structure
- Choose a well-defined topic structure to organize your messages. For example:
  - **Home/Room1/Sensor1/Temperature**
  - **Home/Room1/Sensor1/Humidity**

## Step 4: Configure MQTT Connection for ESP32
### Required Libraries
- Install the following libraries in your Arduino IDE:
  - PubSubClient
  - WiFi

### Sample Code
```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your_ssid";
const char* password = "your_password";
const char* mqtt_server = "your_broker_url";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 8883);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client", "your_username", "your_password")) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      delay(5000);
    }
  }
}
```  

## Step 5: Configure MQTT Connection for Android
### Required Libraries
- Use the Paho MQTT client in your Android project dependencies.

### Sample Code
```java
import org.eclipse.paho.client.mqttv3.*;

public class MqttClientExample {
  public static void main(String[] args) throws MqttException {
    String broker = "your_broker_url";
    String username = "your_username";
    String password = "your_password";
    MqttClient client = new MqttClient(broker, MqttClient.generateClientId());
    MqttConnectOptions options = new MqttConnectOptions();
    options.setUserName(username);
    options.setPassword(password.toCharArray());
    client.connect(options);
    System.out.println("Connected to MQTT Broker!");
  }
}
```  

## Conclusion
You have successfully set up the HiveMQ Cloud MQTT broker and configured connections for both ESP32 and Android clients. Now you can publish and subscribe to topics using the defined structure.