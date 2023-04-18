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
#else if defined (ESP32_DEV)
  #include <WiFi.h>
#endif

#include <WiFiClient.h>
#include <WeatherFlowUdp.h>
#include <WeatherFlowStrings.h>

//WiFi
const char *ssid = "your_wifi_name";
const char *password = "your_wifi_password";

// Construct a weather object
WeatherFlowUdp currentWeather;

/*
Handler function to call when a new WeatherFlow object is recieved.
*/
void handleNewObject(WeatherFlowData::Object obj, void* context) {
  Serial.println(WeatherFlowStrings::description_P(obj));

  // Walk through all the possible keys and see if there is a value 
  // for the current object
  for (int keyInt = WeatherFlowData::Serial_Number; keyInt != WeatherFlowData::Last_Value; keyInt++) {
    WeatherFlowData::Key key = static_cast<WeatherFlowData::Key>(keyInt);
    JsonVariantConst value = (currentWeather.getValue(key));
    if (!value.isNull()) {
      Serial.print(WeatherFlowStrings::description_P(key));
      Serial.print(F(": "));

      // Print the value of the key in it's native type
      if (value.is<const char*>())
        Serial.print((const char*)value);
      else if (value.is<int>())
        Serial.print((int)value);
      else if (value.is<float>())
        Serial.print((float)value);

      Serial.print(F(" "));
      Serial.println(WeatherFlowStrings::unit_P(key));
    }
  }
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

  // Register the handler function
  currentWeather.registerCallback(handleNewObject);

  // Start listening for UDP broadcasts
  currentWeather.begin();
}

void loop() {
  currentWeather.update();

}
