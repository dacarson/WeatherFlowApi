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

#ifndef _WeatherFlowStrings_H
#define _WeatherFlowStrings_H

/***
    Helper class to provide String mappings for weather flow data.
 */
#include "Arduino.h"
#include "WeatherFlowData.h"

class WeatherFlowStrings {
    public:
	/* User friendly description of the object */
    static const __FlashStringHelper *description_P(WeatherFlowData::Object obj);
    
    /* User friendly description of value */
    static const __FlashStringHelper *description_P(WeatherFlowData::Key key);

    /* Units for a value */
    static const __FlashStringHelper *unit_P(WeatherFlowData::Key key);
    
	/* Strings for precipitation types */
    static const __FlashStringHelper *precipitation_P(WeatherFlowData::PrecipitationTypes type);
    
};
#endif
