#include "Timer.h"

int count = 0;
uint32_t intervalId = 0;
uint32_t timeoutId1 = 0;
uint32_t timeoutId2 = 0;

void printTimeout() {
    Serial.println("\n[Timeout-1]: 5 Second has been passed...");
    Serial.println("\n[Timeout-1]: Timeout Event 1 with id [" + String(timeoutId1) + "] has been finished.");
}

void printEveryTenSecond() {
    Serial.println("\n[Interval]: Print Every 10 Second...");
    if (++count >= 5) {
        //Clear interval using intervalId
        Timer.clearInterval(intervalId);
        Serial.println("\n[Interval]: Interval Event with id [" + String(intervalId) + "] has been cancelled.");
    }
}

void setup() {
    Serial.begin(115200);

    //Execute callback function every 10 seconds.
    //Stop after 50 second. (When count reached 5)
    intervalId = Timer.setInterval(printEveryTenSecond, 10000);
    Serial.println("\n[Interval]: Interval Event has been succesfully registered with id [" + String(intervalId) + "]");

    //Execute callback after 5 second.
    timeoutId1 = Timer.setTimeout(printTimeout, 5000);
    Serial.println("\n[Timeout-1]: Timeout Event 1 has been succesfully registered with id [" + String(timeoutId1) + "]");

    //You can cancel the timeout by calling clearTimeout() function.
    //Let's make another timeout event:
    timeoutId2 = Timer.setTimeout(printTimeout, 5000);
    Serial.println("\n[Timeout-2]: Timeout Event 2 has been succesfully registered with id [" + String(timeoutId2) + "]");

    //Clear timeout event after 3 seconds:
    Timer.setTimeout([]() -> void {
        Timer.clearTimeout(timeoutId2);
        Serial.println("\n[Timeout-2]: Timeout Event 2 with id [" + String(timeoutId2) + "] has been cancelled.");
    }, 3000);
}

void loop() {
    Timer.run(); //You do not need to call this if you are using ESP32
}
