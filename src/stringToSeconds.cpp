#include <sunsetLED.h>

string convertToString(const String& arduinoString) {
    // Convert Arduino String to C-style string
    const char* cString = arduinoString.c_str();

    // Constructing std::string using the constructor that takes a const char*
    string result(cString);

    return result;
}

uint32_t strToSec(string timeString){
    uint8_t hours, minutes, seconds; // the time format is "h:mm:ss aa"
    string period;

    // Use string methods to extract hours, minutes, seconds, and period
    size_t pos1 = timeString.find(':');
    size_t pos2 = timeString.rfind(':');
    size_t pos3 = timeString.rfind(' ');

    // Extract hours
    hours = stoi(timeString.substr(0, pos1));

    // Extract minutes
    minutes = stoi(timeString.substr(pos1 + 1, pos2 - pos1 - 1));

    // Extract seconds
    seconds = stoi(timeString.substr(pos2 + 1, pos3 - pos2 - 1));

    // Extract period (AM/PM)
    period = timeString.substr(pos3 + 1, 2);

    // Adjust hours based on period
    if (period == "PM" && hours != 12) {
        hours += 12;
    }
    else if (period == "AM" && hours == 12) {
        hours = 0;
    }

    // Calculate total seconds
    uint32_t totalSeconds = hours * 3600 + minutes * 60 + seconds;

    return totalSeconds;
}

uint32_t timeToSunset(string timeString, NTPClient timeClient){
    // Get current time
    uint32_t currentTime = timeClient.getEpochTime();

    // Calculate time since midnight
    uint32_t timeSinceMidnight = currentTime % 86400;

    // Send time to serial
    Serial.print("Time since midnight: ");
    Serial.println(timeSinceMidnight);

    uint32_t sunsetSeconds = strToSec(timeString);

    int32_t difference = sunsetSeconds - timeSinceMidnight;
    Serial.print("Received time in seconds: ");
    Serial.println(sunsetSeconds);

    Serial.print("Time to sunset: ");
    Serial.println(difference);
    if (difference < 0) difference = 0;
    Serial.print("Time to sunset2: ");
    Serial.println(difference);

    return uint32_t(difference);
}

