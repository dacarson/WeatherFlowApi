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

#include "WeatherFlowData.h"

//#define DEBUG (0)

// Commonly used strings when parsing objects
const char *WeatherFlowData::SERIAL_NUMBER = "serial_number";
const char *WeatherFlowData::TYPE = "type";
const char *WeatherFlowData::HUB_SERIAL_NUMBER = "hub_sn";
const char *WeatherFlowData::EVT = "evt";
const char *WeatherFlowData::OB = "ob";
const char *WeatherFlowData::OBS = "obs";
const char *WeatherFlowData::FIRMWARE_REVISION = "firmware_revision";
const char *WeatherFlowData::TIMESTAMP = "timestamp";
const char *WeatherFlowData::UPTIME = "uptime";
const char *WeatherFlowData::RSSI = "rssi";
const char *WeatherFlowData::RADIO_STATS = "radio_stats";

WeatherFlowData::WeatherFlowData() :
	rainEventJsonDocument(0),
	strikeEventJsonDocument(0),
	windEventJsonDocument(0),
	skyEventJsonDocument(0),
	airEventJsonDocument(0),
	tempestEventJsonDocument(0),
	statusEventJsonDocument(0),
	hubEventJsonDocument(0),
	eventCallback(0),
	currentCallback(LAST_OBJECT)
	{
}

WeatherFlowData::~WeatherFlowData() {
}

// Takes ownership of the buffer on success (return 0)
int WeatherFlowData::processPacket(char* buffer) {
	// What sort of object is it
	DynamicJsonDocument tempDoc(400);
	DeserializationError err = deserializeJson(tempDoc, buffer);
	
	if (err) {
//#ifdef DEBUG
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(err.c_str());
//#endif
		return -1;
	}
	
#ifdef DEBUG
	Serial.print(F("Processing object type: "));
	Serial.println((const char*)tempDoc[TYPE]);
#endif
	
	if (strcmp("evt_precip", tempDoc[TYPE]) == 0) {
		// copy the document as it will be freed by processPacket
		rainEventJsonDocument = tempDoc;
		rainEventJsonDocument.shrinkToFit();
		currentCallback = RAIN;
	}
	else if (strcmp("evt_strike", tempDoc[TYPE]) == 0) {
		strikeEventJsonDocument = tempDoc;
		strikeEventJsonDocument.shrinkToFit();
		currentCallback = LIGHTNING;
	}
	else if (strcmp("rapid_wind", tempDoc[TYPE]) == 0) {
		windEventJsonDocument = tempDoc;
		windEventJsonDocument.shrinkToFit();
		currentCallback = WIND;
	}
	else if (strcmp("obs_air", tempDoc[TYPE]) == 0) {
		airEventJsonDocument = tempDoc;
		airEventJsonDocument.shrinkToFit();
		currentCallback = AIR;
	}
	else if (strcmp("obs_sky", tempDoc[TYPE]) == 0) {
		skyEventJsonDocument = tempDoc;
		skyEventJsonDocument.shrinkToFit();
		currentCallback = SKY;
	}
	else if (strcmp("obs_st", tempDoc[TYPE]) == 0) {
		tempestEventJsonDocument = tempDoc;
		tempestEventJsonDocument.shrinkToFit();
		currentCallback = TEMPEST;
	}
	else if (strcmp("device_status", tempDoc[TYPE]) == 0) {
		statusEventJsonDocument = tempDoc;
		statusEventJsonDocument.shrinkToFit();
		currentCallback = STATUS;
	}
	else if (strcmp("hub_status", tempDoc[TYPE]) == 0) {
		hubEventJsonDocument = tempDoc;
		hubEventJsonDocument.shrinkToFit();
		currentCallback = HUB;
	}

#ifdef DEBUG
	for (int objInt = WeatherFlowData::RAIN; objInt != WeatherFlowData::LAST_OBJECT; objInt++) {
		WeatherFlowData::Object obj = static_cast<WeatherFlowData::Object>(objInt);
		serializeJson(lastObject(obj), Serial);
		Serial.println(F(""));
		}
#endif

	// Invoke callback if packet is identified
	if (currentCallback != LAST_OBJECT) {
		// call callback function
		if (eventCallback) {
			eventCallback(currentCallback, callbackContext);
		}
		currentCallback = LAST_OBJECT;
		return 0;
	}

#ifdef DEBUG
	Serial.print(F("unknown message type"));
	Serial.println((const char*)tempDoc[TYPE]);
#endif
	return -1;
}

JsonVariantConst WeatherFlowData::getValue(WeatherFlowData::Key key) {
	return getValue(currentCallback, key);
}

JsonVariantConst WeatherFlowData::getValue(WeatherFlowData::Object obj, WeatherFlowData::Key key) {
	switch (obj) {
		case RAIN:
		{
			switch (key) {
				case Serial_Number:
					return rainEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return rainEventJsonDocument[HUB_SERIAL_NUMBER];
				case Time_Epoch:
					return rainEventJsonDocument[EVT][0];
			}
			break;
		}
		case LIGHTNING:
		{
			switch (key) {
				case Serial_Number:
					return strikeEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return strikeEventJsonDocument[HUB_SERIAL_NUMBER];
				case Time_Epoch:
					return strikeEventJsonDocument[EVT][0];
				case Strike_Distance:
					return strikeEventJsonDocument[EVT][1];
				case Energy:
					return strikeEventJsonDocument[EVT][2];
			}
			break;
		}
		case WIND:
		{
			switch (key) {
				case Serial_Number:
					return windEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return windEventJsonDocument[HUB_SERIAL_NUMBER];
				case Time_Epoch:
					return windEventJsonDocument[OB][0];
				case Wind_Speed:
					return windEventJsonDocument[OB][1];
				case Wind_Direction:
					return windEventJsonDocument[OB][2];
			}
			break;
		}
		case AIR:
		{
			switch (key) {
				case Serial_Number:
					return airEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return airEventJsonDocument[HUB_SERIAL_NUMBER];
				case Firmware:
					return airEventJsonDocument[FIRMWARE_REVISION];
				case Time_Epoch:
					return airEventJsonDocument[OBS][0][0];
				case Station_Pressure:
					return airEventJsonDocument[OBS][0][1];
				case Air_Temperature:
					return airEventJsonDocument[OBS][0][2];
				case Relative_Humidity:
					return airEventJsonDocument[OBS][0][3];
				case Strike_Count:
					return airEventJsonDocument[OBS][0][4];
				case Strike_Avg_Distance:
					return airEventJsonDocument[OBS][0][5];
				case Battery:
					return airEventJsonDocument[OBS][0][6];
				case Report_Interval:
					return airEventJsonDocument[OBS][0][7];
			}
			break;
		}
		case SKY:
		{
			switch (key) {
				case Serial_Number:
					return skyEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return skyEventJsonDocument[HUB_SERIAL_NUMBER];
				case Firmware:
					return skyEventJsonDocument[FIRMWARE_REVISION];
				case Time_Epoch:
					return skyEventJsonDocument[OBS][0][0];
				case Illuminance:
					return skyEventJsonDocument[OBS][0][1];
				case UV:
					return skyEventJsonDocument[OBS][0][2];
				case Rain_Last_Minute:
					return skyEventJsonDocument[OBS][0][3];
				case Wind_Lull:
					return skyEventJsonDocument[OBS][0][4];
				case Wind_Avg:
					return skyEventJsonDocument[OBS][0][5];
				case Wind_Gust:
					return skyEventJsonDocument[OBS][0][6];
				case Wind_Direction:
					return skyEventJsonDocument[OBS][0][7];
				case Battery:
					return skyEventJsonDocument[OBS][0][8];
				case Report_Interval:
					return skyEventJsonDocument[OBS][0][9];
				case Solar_Radiation:
					return skyEventJsonDocument[OBS][0][10];
				case PrecipitationType:
					return skyEventJsonDocument[OBS][0][12];
				case Wind_Sample_Interval:
					return skyEventJsonDocument[OBS][0][13];
			}
			break;
		}
		case TEMPEST:
		{
			switch (key) {
				case Serial_Number:
					return tempestEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return tempestEventJsonDocument[HUB_SERIAL_NUMBER];
				case Firmware:
					return tempestEventJsonDocument[FIRMWARE_REVISION];
				case Time_Epoch:
					return tempestEventJsonDocument[OBS][0][0];
				case Wind_Lull:
					return tempestEventJsonDocument[OBS][0][1];
				case Wind_Avg:
					return tempestEventJsonDocument[OBS][0][2];
				case Wind_Gust:
					return tempestEventJsonDocument[OBS][0][3];
				case Wind_Direction:
					return tempestEventJsonDocument[OBS][0][4];
				case Wind_Sample_Interval:
					return tempestEventJsonDocument[OBS][0][5];
				case Station_Pressure:
					return tempestEventJsonDocument[OBS][0][6];
				case Air_Temperature:
					return tempestEventJsonDocument[OBS][0][7];
				case Relative_Humidity:
					return tempestEventJsonDocument[OBS][0][8];
				case Illuminance:
					return tempestEventJsonDocument[OBS][0][9];
				case UV:
					return tempestEventJsonDocument[OBS][0][10];
				case Solar_Radiation:
					return tempestEventJsonDocument[OBS][0][11];
				case Rain_Last_Minute:
					return tempestEventJsonDocument[OBS][0][12];
				case PrecipitationType:
					return tempestEventJsonDocument[OBS][0][13];
				case Strike_Avg_Distance:
					return tempestEventJsonDocument[OBS][0][14];
				case Strike_Count:
					return tempestEventJsonDocument[OBS][0][15];
				case Battery:
					return tempestEventJsonDocument[OBS][0][16];
				case Report_Interval:
					return tempestEventJsonDocument[OBS][0][17];
			}
			break;
		}
		case STATUS:
		{
			switch (key) {
				case Serial_Number:
					return statusEventJsonDocument[SERIAL_NUMBER];
				case Hub_Serial_Number:
					return statusEventJsonDocument[HUB_SERIAL_NUMBER];
				case Firmware:
					return statusEventJsonDocument[FIRMWARE_REVISION];
				case Time_Epoch:
					return statusEventJsonDocument[TIMESTAMP];
				case Uptime:
					return statusEventJsonDocument[UPTIME];
				case Battery:
					return statusEventJsonDocument["voltage"];
				case Rssi:
					return statusEventJsonDocument[RSSI];
				case Hub_RSSI:
					return statusEventJsonDocument["hub_rssi"];
				case Sensor_Status:
					return statusEventJsonDocument["sensor_status"];
				case Debug:
					return statusEventJsonDocument["debug"];
			}
			break;
		}
		case HUB:
		{
			switch (key) {
				case Serial_Number:
				case Hub_Serial_Number:
					return hubEventJsonDocument[SERIAL_NUMBER];
				case Firmware:
					return hubEventJsonDocument[FIRMWARE_REVISION];
				case Time_Epoch:
					return hubEventJsonDocument[TIMESTAMP];
				case Uptime:
					return hubEventJsonDocument[UPTIME];
				case Rssi:
					return hubEventJsonDocument[RSSI];
				case Reset_Flags:
					return hubEventJsonDocument["reset_flags"];
				case Sequence_Count:
					return hubEventJsonDocument["seq"];
				case Radio_Stats_Version:
					return hubEventJsonDocument[RADIO_STATS][0];
				case Radio_Stats_Reboot_Count:
					return hubEventJsonDocument[RADIO_STATS][1];
				case Radio_Stats_Bus_Error_Count:
					return hubEventJsonDocument[RADIO_STATS][2];
				case Radio_Stats_Status:
					return hubEventJsonDocument[RADIO_STATS][3];
				case Radio_Stats_Network_Id:
					return hubEventJsonDocument[RADIO_STATS][4];
			}
			break;
		}
		case LAST_OBJECT:
		{
#ifdef DEBUG
			Serial.println(F("getValue called when not in callback"));
#endif
			break;
		}
	}
  JsonObject empty;
  return empty;
}

bool WeatherFlowData::hasObject(WeatherFlowData::Object obj) {
	switch (obj) {
		case RAIN:
			return rainEventJsonDocument.isNull() ? false : true;
		case LIGHTNING:
			return strikeEventJsonDocument.isNull() ? false : true;
		case WIND:
			return windEventJsonDocument.isNull() ? false : true;
		case AIR:
			return airEventJsonDocument.isNull() ? false : true;
		case SKY:
			return skyEventJsonDocument.isNull() ? false : true;
		case TEMPEST:
			return tempestEventJsonDocument.isNull() ? false : true;
		case STATUS:
			return statusEventJsonDocument.isNull() ? false : true;
		case HUB:
			return hubEventJsonDocument.isNull() ? false : true;
	}
  return false;
}

DynamicJsonDocument WeatherFlowData::lastObject(WeatherFlowData::Object obj) {
	switch (obj) {
		case RAIN:
			return rainEventJsonDocument;
		case LIGHTNING:
			return strikeEventJsonDocument;
		case WIND:
			return windEventJsonDocument;
		case AIR:
			return airEventJsonDocument;
		case SKY:
			return skyEventJsonDocument;
		case TEMPEST:
			return tempestEventJsonDocument;
		case STATUS:
			return statusEventJsonDocument;
		case HUB:
			return hubEventJsonDocument;
	}
  return DynamicJsonDocument(0);
}

void WeatherFlowData::registerCallback(WeatherFlowData::ENotifierFunction callback, void* context) {
	eventCallback = callback;
	callbackContext = context;
}
