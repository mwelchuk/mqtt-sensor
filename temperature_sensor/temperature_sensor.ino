/*
 * Firmware for temperature sensor node.
 *
 * This is the firmware for the ESP8266 in the temperature sensor node. The
 * ESP8266 is responsible for taking temperature and humidity readings and
 * publishing them via MQTT.
 *
 * This firmware has a number of tweakable options that are described below.
 */

/*
 * Sensor selection
 *
 * Pick the sensor you are using in your node. This firmware can currently
 * support DHT22 (AM2302) or HTU21D devices. Either use:
 * #define SENSOR_DHT22
 * #define SENSOR_HTU21D
 */
/* NB: I've got this defined within the room details block */

#ifdef SENSOR_DHT22
  #ifdef SENSOR_HTU21D
    #error Can't enable both sensor types at the same time
  #endif
#endif

/*
 * External power monitor
 *
 * The firmware can either utilise an external power monitor to control
 * power or utilise the ESP8266 "deep sleep" functionality. Unless the
 * following option is set, deep sleep will be used:
 * #define POWER_EXTERNAL
 */
#define POWER_EXTERNAL

/*
 * Serial output
 *
 * The device can be configured to put out debug on it's serial port with the
 * following define:
 *
 * #define DEBUG
 *
 * Note: This can't be used when using the external power control as we use
 * the receive pin for that functionality
 */


#ifdef DEBUG
  #ifdef POWER_EXTERNAL
    #error debug and external power control can't be used at the same time.
  #endif
#endif

/*
 * Locaton configuration
 *
 * This is where you encode the location information for the build. There is
 * an example block further down in the code. Alternatively add blocks to
 * `private_details.h`.
 */
//#define ROOM_TEST
//#define ROOM_STUDY
#define ROOM_LIVINGROOM
//#define ROOM_BEDROOM1
//#define ROOM_BEDROOM2
//#define ROOM_BEDROOM3

#include "private_details.h"

#ifdef SENSOR_HTU21D
#include <Wire.h>
#endif

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#ifdef SENSOR_DHT22
#include "DHT.h"
#endif

#ifdef SENSOR_HTU21D
#include "SparkFunHTU21D.h"
#endif

/* Example Room configuration
#ifdef ROOM_TEST
// MQTT Location variables
const char* location = "home";
const char* room = "test2";

#define SENSOR_DHT22
#define DHTPIN 2
#endif
*/

#ifdef SENSOR_DHT22
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT sensor(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
#endif

#ifdef SENSOR_HTU21D
HTU21D sensor;
#endif

//DHT Variables
float hum;  //Stores humidity value
float temp; //Stores temperature value

// Update these with values suitable for your network.
/* Actual values stored in private_details.h
const char* ssid = "<ssid>";
const char* password = "<passwd>";
const char* mqtt_server = "<server>";
*/

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char topic[75];
char reading[20];
char str_value[20];

void setup_wifi() {
  delay(10);

#ifdef DEBUG
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG
    Serial.print(".");
#endif
  }
#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

void readings() {
  temp= sensor.readTemperature();
  hum = sensor.readHumidity();

  snprintf (topic, 75, "%s/%s/temperature", location, room);
  dtostrf (temp, 1, 2, str_value);
  snprintf (reading, 10, "%s C", str_value);
  client.publish(topic, reading);
#ifdef DEBUG
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(reading);
#endif
  client.loop();

  snprintf (topic, 75, "%s/%s/humidity", location, room);
  dtostrf (hum, 1, 2, str_value);
  snprintf (reading, 20, "%s %%", str_value);
  client.publish(topic, reading);
#ifdef DEBUG
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(reading);
#endif
  client.loop();
}

void setup() {
#ifdef POWER_EXTERNAL
  digitalWrite(3, LOW);
  pinMode(3, OUTPUT);
#endif

#ifdef DEBUG
  Serial.begin(115200);
#endif

  // Start Library
  sensor.begin();

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  while (!client.connected()) {
    if (!client.connect("ESP8266Client")) {
#ifdef DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
#endif
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  // Send data
  readings();

  delay(2000);

#ifdef POWER_EXTERNAL
  // Get power monitor to switch power off
  digitalWrite(3, HIGH);
#else
  // Go to sleep
#ifdef DEBUG
  Serial.println("About to sleep");
#endif
  ESP.deepSleep(5*60e6);
#endif
}

void loop() {
}
