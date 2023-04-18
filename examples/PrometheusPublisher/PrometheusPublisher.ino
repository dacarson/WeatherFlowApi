/*
Copyright 2023 David Carson (dacarson at gmail)

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the “Software”), to deal in the
Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#if defined (ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#else if defined (ESP32_DEV)
  #include <WiFi.h>
  #include <WebServer.h>
#endif

#include <WiFiClient.h>
#include <WeatherFlowUdp.h>
#include <WeatherFlowStrings.h>

//WiFi
const char *ssid = "your_wifi_name";
const char *password = "your_wifi_password";

// Construct a WeatherFlow UDP listener
WeatherFlowUdp currentWeather;

// Construct a WebServer
#if defined (ESP8266)
  ESP8266WebServer server ( 80 );
#else if defined (ESP32_DEV)
  WebServer server ( 80 );
#endif


/*
This function is called everytime a new WeatherFlow object is processed. The
type of the object is passed in as obj.
*/
void handleNewObject(WeatherFlowData::Object obj, void* context) {
  // Print out that an object has been recieved
  Serial.print(F("Received new object: "));
  Serial.println(WeatherFlowStrings::description_P(obj));
}

// Send a simple web page that has one link on it, to the Prometheus metrics
// which are usually under /metrics url
void handleRoot() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send ( 200, "text/html", "" );
  server.sendContent(F("<html><head>Prometheus Publisher</head><body>"));
  server.sendContent(F("<p><a href=\"metrics\">Prometheus metrics</a></p>"));
  server.sendContent(F("Compiled: "));
  server.sendContent(F(__DATE__));
  server.sendContent( F(", "));
  server.sendContent(F(__TIME__));
  server.sendContent( F(", GCC: "));
  server.sendContent(F(__VERSION__));
  //server.sendContent( F(", ARDUINO IDE: "));
  //server.sendContent(F("ARDUINO"));
  server.sendContent(F("</body></html>"));
  server.client().stop();
}

void handleMetrics() {
  Serial.println(F("Handling a metrics request"));
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send ( 200, "text/plain", "" );

  // Walk through all object types and examine ones that have data
  for (int objInt = WeatherFlowData::RAIN; objInt != WeatherFlowData::LAST_OBJECT; objInt++) {
    WeatherFlowData::Object obj = static_cast<WeatherFlowData::Object>(objInt);
    if (currentWeather.hasObject(obj)) {

      // Walk through all the possible keys, and see if there is a value 
      // for the current object. Skip serial number as that is used as a key
      for (int keyInt = WeatherFlowData::Serial_Number+1; keyInt != WeatherFlowData::Last_Value; keyInt++) {
        WeatherFlowData::Key key = static_cast<WeatherFlowData::Key>(keyInt);
        JsonVariantConst value = currentWeather.getValue(obj, key);
        if (!value.isNull()) {

          // We need the metric name to have no spaces.
          int metricLen = strlen_P(reinterpret_cast<const char *>(WeatherFlowStrings::description_P(key)));
          char* noSpaceMetric = (char*)malloc(metricLen + 1);
          strncpy_P(noSpaceMetric, reinterpret_cast<const char *>(WeatherFlowStrings::description_P(key)), metricLen + 1);
          for (int i = 0; i < metricLen; i++) {
            if (noSpaceMetric[i] == ' ')
              noSpaceMetric[i] = '_';
          }

          // Send #HELP line
          server.sendContent(F("#HELP weather_"));
          server.sendContent(noSpaceMetric);
          server.sendContent(F(" "));
          server.sendContent(WeatherFlowStrings::description_P(key));
          if (strlen_P(reinterpret_cast<const char *>(WeatherFlowStrings::unit_P(key))) > 0) {
            server.sendContent(F(" ("));
            server.sendContent(WeatherFlowStrings::unit_P(key));
            server.sendContent(F(")"));
          }
          server.sendContent(F(" generated by "));
          server.sendContent(WeatherFlowStrings::description_P(obj));
          server.sendContent(F(" ("));
          server.sendContent((const char*) currentWeather.getValue(obj, WeatherFlowData::Serial_Number));
          server.sendContent(F(")\n"));

          // Send #TYPE line
          server.sendContent(F("#TYPE weather_"));
          server.sendContent(noSpaceMetric); // needs no spaces
          if (key == WeatherFlowData::Sequence_Count)
            server.sendContent(F(" counter\n"));
          else
            server.sendContent(F(" gauge\n"));

          // Send metric and value
          server.sendContent(F("weather_"));
          server.sendContent(noSpaceMetric); // needs no spaces
          server.sendContent(F("{serial=\""));
          server.sendContent((const char*) currentWeather.getValue(obj, WeatherFlowData::Serial_Number));
          server.sendContent(F("\"} "));
          // Print the value of the key in it's native type
          if (value.is<const char*>()) {
            server.sendContent((const char*)value);
          } else if (value.is<int>()) {
            char buffer[10];
            sprintf(buffer, "%d", (int)value);
            server.sendContent(buffer);
          } else if (value.is<float>()) {
            char buffer[10];
            sprintf(buffer, "%f", (float)value);
            server.sendContent(buffer);
          }
          server.sendContent(F("\n"));
          free(noSpaceMetric);
        }
      }
    }

  }

  server.client().stop();
}

void setup() {
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );
  Serial.print ( F("Connecting to WiFi "));

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( F(".") );
	}

	Serial.println ( F("") );
	Serial.print (F("Connected to ") );
	Serial.println ( ssid );
	Serial.print ( F("IP address: ") );
	Serial.println ( WiFi.localIP() );

  // Setup to listen for WeatherFlow UDP packets 
  currentWeather.registerCallback(handleNewObject);
  currentWeather.begin();

  // Setup web server pages
	server.on ( "/", handleRoot );
  server.on ( "/metrics", handleMetrics );
  server.begin();
}

void loop() {
  currentWeather.update();
  server.handleClient();  
}
