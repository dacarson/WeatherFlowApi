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

#include "WeatherFlowUdp.h"

//#define DEBUG (0)
#define WEATHERFLOW_UDP_PORT (50222)

WeatherFlowUdp::WeatherFlowUdp() {
}

WeatherFlowUdp::~WeatherFlowUdp() {
}

uint8_t WeatherFlowUdp::begin() {
	return weatherUDP.begin(WEATHERFLOW_UDP_PORT);
}

void WeatherFlowUdp::update() {
	// Process all waiting packets
	int packetSize = 0;
	while(packetSize = weatherUDP.parsePacket()) {
		char* buffer = (char*)malloc(packetSize+1);
		int bufferLen = weatherUDP.read(buffer, packetSize);
		
		// Null terminate the buffer, just in case.
		buffer[bufferLen] = 0;
		bufferLen++;
#ifdef DEBUG
		Serial.println(F("Received WeatherFlow packet"));
		Serial.println(buffer);
#endif
		// Free the buffer if it wasn't handled.
		if (processPacket(buffer) != 0) {
		Serial.println(F("Packet not handled"));
				Serial.println(buffer);
			free(buffer);
			}
	}
}

