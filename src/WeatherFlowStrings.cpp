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

#include "WeatherFlowStrings.h"


const __FlashStringHelper *WeatherFlowStrings::description_P(WeatherFlowData::Object obj) {
	switch (obj) {
		case WeatherFlowData::RAIN:
			return F("Rain Start Event");
		case WeatherFlowData::LIGHTNING:
			return F("Lightning Strike Event");
		case WeatherFlowData::WIND:
			return F("Rapid Wind Event");
		case WeatherFlowData::AIR:
			return F("Observation (AIR) Event");
		case WeatherFlowData::SKY:
			return F("Observation (Sky) Event");
		case WeatherFlowData::TEMPEST:
			return F("Observation (Tempest) Event");
		case WeatherFlowData::STATUS:
			return F("Status (device) Event");
		case WeatherFlowData::HUB:
			return F("Status (hub) Event");
	}
	return F("");
}

const __FlashStringHelper *WeatherFlowStrings::description_P(WeatherFlowData::Key key) {
	
	switch (key) {
		case WeatherFlowData::Serial_Number:
			return F("Serial Number");
		case WeatherFlowData::Hub_Serial_Number:
			return F("Hub Serial Number");
		case WeatherFlowData::Time_Epoch:
			return F("Time Epoch");
		case WeatherFlowData::Strike_Distance:
			return F("Strike Distance");
		case WeatherFlowData::Energy:
			return F("Strike Energy");
			
		case WeatherFlowData::Wind_Speed:
			return F("Wind Speed");
		case WeatherFlowData::Wind_Direction:
			return F("Wind Direction");
			
		case WeatherFlowData::Wind_Lull:
			return F("Wind Lull");
		case WeatherFlowData::Wind_Avg:
			return F("Wind Average");
		case WeatherFlowData::Wind_Gust:
			return F("Wind Gust");
		case WeatherFlowData::Wind_Sample_Interval:
			return F("Wind Sample Interval");
		case WeatherFlowData::Station_Pressure:
			return F("Station Pressure");
		case WeatherFlowData::Air_Temperature:
			return F("Air Temperature");
		case WeatherFlowData::Relative_Humidity:
			return F("Relative Humidity");
		case WeatherFlowData::Illuminance:
			return F("Illuminance");
		case WeatherFlowData::UV:
			return F("UV");
		case WeatherFlowData::Solar_Radiation:
			return F("Solar Radiation");
		case WeatherFlowData::Rain_Last_Minute:
			return F("Rain amount over previous minute");
		case WeatherFlowData::PrecipitationType:
			return F("Precipation Type");
		case WeatherFlowData::Strike_Avg_Distance:
			return F("Lightning Strike Avg Distance");
		case WeatherFlowData::Strike_Count:
			return F("Lightning Strike Count");
		case WeatherFlowData::Battery:
			return F("Battery");
		case WeatherFlowData::Report_Interval:
			return F("Report Interval");
		case WeatherFlowData::Firmware:
			return F("Firmware revision");
			
		case WeatherFlowData::Uptime:
			return F("Uptime");
		case WeatherFlowData::Rssi:
			return F("RSSI");
		case WeatherFlowData::Hub_RSSI:
			return F("Hub RSSI");
		case WeatherFlowData::Sensor_Status:
			return F("Sensor Status flags");
		case WeatherFlowData::Debug:
			return F("Debug enabled");
			
		case WeatherFlowData::Reset_Flags:
			return F("Reset Flags");
		case WeatherFlowData::Sequence_Count:
			return F("Sequence Count");
		case WeatherFlowData::Radio_Stats_Version:
			return F("Radio Stats Version");
		case WeatherFlowData::Radio_Stats_Reboot_Count:
			return F("Radio Reboot Count");
		case WeatherFlowData::Radio_Stats_Bus_Error_Count:
			return F("Radio I2C Bus Error Count");
		case WeatherFlowData::Radio_Stats_Status:
			return F("Radio Status");
		case WeatherFlowData::Radio_Stats_Network_Id:
			return F("Radio Network ID");
	}
	return F("");
}

const __FlashStringHelper *WeatherFlowStrings::unit_P(WeatherFlowData::Key key) {
	
	switch (key) {
		case WeatherFlowData::Serial_Number:
		case WeatherFlowData::Hub_Serial_Number:
		case WeatherFlowData::Energy:
		case WeatherFlowData::PrecipitationType:
		case WeatherFlowData::Strike_Count:
		case WeatherFlowData::Firmware:
		case WeatherFlowData::Sensor_Status:
		case WeatherFlowData::Debug:
		case WeatherFlowData::Reset_Flags:
		case WeatherFlowData::Radio_Stats_Version:
		case WeatherFlowData::Radio_Stats_Reboot_Count:
		case WeatherFlowData::Radio_Stats_Bus_Error_Count:
		case WeatherFlowData::Radio_Stats_Status:
		case WeatherFlowData::Radio_Stats_Network_Id:
			return F("");
		case WeatherFlowData::Time_Epoch:
		case WeatherFlowData::Wind_Sample_Interval:
		case WeatherFlowData::Uptime:
			return F("seconds");
		case WeatherFlowData::Strike_Distance:
		case WeatherFlowData::Strike_Avg_Distance:
			return F("km");
			
		case WeatherFlowData::Wind_Speed:
		case WeatherFlowData::Wind_Lull:
		case WeatherFlowData::Wind_Avg:
		case WeatherFlowData::Wind_Gust:
			return F("m/s");
		case WeatherFlowData::Wind_Direction:
			return F("degrees");
			
		case WeatherFlowData::Station_Pressure:
			return F("MB");
		case WeatherFlowData::Air_Temperature:
			return F("C");
		case WeatherFlowData::Relative_Humidity:
			return F("%");
		case WeatherFlowData::Illuminance:
			return F("lux");
		case WeatherFlowData::UV:
			return F("index");
		case WeatherFlowData::Solar_Radiation:
			return F("W/m^2");
		case WeatherFlowData::Rain_Last_Minute:
			return F("mm");
		case WeatherFlowData::Battery:
			return F("voltage");
		case WeatherFlowData::Report_Interval:
			return F("minutes");
			
			
		case WeatherFlowData::Rssi:
		case WeatherFlowData::Hub_RSSI:
			return F("dBm");
	}
	return F("");
}

static const __FlashStringHelper *precipitation_P(WeatherFlowData::PrecipitationTypes type) {
	
	switch(type) {
		case WeatherFlowData::Precipitation_Rain:
			return F("Rain");
		case WeatherFlowData::Precipitation_Hail:
			return F("Hail");
		case WeatherFlowData::Precipitation_Rain_Hail:
			return F("Rain and Hail");
		default:
			return F("None");
	}
	
}
