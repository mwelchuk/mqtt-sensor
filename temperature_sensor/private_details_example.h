/* Private Settings */
const char* ssid = "<ssid>";
const char* password = "<passwd>";
const char* mqtt_server = "<server>";

#ifdef ROOM_TEST
// MQTT Location variables
const char* location = "home";
const char* room = "test";

#define SENSOR_DHT22
#define DHTPIN 2
#endif

#ifdef ROOM_TEST2
// MQTT Location variables
const char* location = "home";
const char* room = "test2";

#define SENSOR_DHT22
#define DHTPIN 2
#endif
