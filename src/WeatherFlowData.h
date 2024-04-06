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


#ifndef _WeatherFlowData_H
#define _WeatherFlowData_H

#include "Arduino.h"
#include "ArduinoJson.h"

/***
	Class to process WeatherFlow objects. Allows clients to register callbacks 
  for when a new object is processed.
  
	Objects are handled as defined in:
	https://weatherflow.github.io/Tempest/api/udp/v171/
*/

class WeatherFlowData {
  public:
    WeatherFlowData();
    ~WeatherFlowData();
    
    /* Handle a new JSON formated Weatherflow object */
    int processPacket(const char *packet);
    
    /* Known types of WeatherFlow objects */
	enum Object {
		RAIN,
		LIGHTNING,
		WIND,
		AIR,
		SKY,
		TEMPEST,
		STATUS,
		HUB,
		LAST_OBJECT // Last object, for easy iterating
	};
	
	/* Known keys that exist in one or more objects */
	enum Key {
		Serial_Number,
		Hub_Serial_Number,
		Time_Epoch,
		Strike_Distance,
		Energy,
		
		Wind_Speed,
		Wind_Direction,
		
		Wind_Lull,
		Wind_Avg,
		Wind_Gust,
		Wind_Sample_Interval,
		Station_Pressure,
		Air_Temperature,
		Relative_Humidity,
		Illuminance,
		UV,
		Solar_Radiation,
		Rain_Last_Minute,
		PrecipitationType,
		Strike_Avg_Distance,
		Strike_Count,
		Battery,
		Report_Interval,
		Firmware,
		
		Uptime,
		Rssi,
		Hub_RSSI,
		Sensor_Status,
		Debug,
		
		Reset_Flags,
		Sequence_Count,
		Radio_Stats_Version,
		Radio_Stats_Reboot_Count,
		Radio_Stats_Bus_Error_Count,
		Radio_Stats_Status,
		Radio_Stats_Network_Id,
		
		Last_Value
	};
	
	/* possible precipitation types */
	enum PrecipitationTypes {
		Precipitation_None = 0,
		Precipitation_Rain = 1,
		Precipitation_Hail = 2,
		Precipitation_Rain_Hail = 3
	};
	
	// Convenience method to use within a callback to get a
	// specified value for key
	JsonVariantConst getValue(Key val);
	
	// get a specific value for key from a specific object
	JsonVariantConst getValue(Object obj, Key val);
	
	// Return true if this type of object has ever been processed
	bool hasObject(Object obj);
	
	// Return a COPY of the last processed object of given type
	JsonDocument lastObject(Object obj);
	
	// Callback function prototype is of the form:
	//    void callback(void* context)
	// Where the context passed to callback is the same as the
	// context value provided during registration.
	typedef std::function<void(Object obj, void* context)> ENotifierFunction;
	void registerCallback(ENotifierFunction callback, void* context = 0);
	
  protected:
	int processJsonDocument(JsonDocument& doc);
	
  private:
	// Local copies of the last objects
	JsonDocument rainEventJsonDocument;
	JsonDocument strikeEventJsonDocument;
	JsonDocument windEventJsonDocument;
	JsonDocument skyEventJsonDocument;
	JsonDocument airEventJsonDocument;
	JsonDocument tempestEventJsonDocument;
	JsonDocument statusEventJsonDocument;
	JsonDocument hubEventJsonDocument;

	ENotifierFunction eventCallback;
	void* callbackContext;
	Object currentCallback;

	// Commonly used strings when parsing objects
	static const char *SERIAL_NUMBER;
	static const char *TYPE;
	static const char *HUB_SERIAL_NUMBER;
	static const char *EVT;
	static const char *OB;
	static const char *OBS;
	static const char *FIRMWARE_REVISION;
	static const char *TIMESTAMP;
	static const char *UPTIME;
	static const char *RSSI;
	static const char *RADIO_STATS;

};
#endif
