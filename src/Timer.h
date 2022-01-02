#ifndef TIMER_H
#define TIMER_H

#if defined(ESP32) || defined(ESP8266)
#include <functional>
#endif

#include "Arduino.h"
#include "ArrayList.h"

class DTimer {
   public:
    DTimer();
    ~DTimer();
#if defined(ESP32) || defined(ESP8266)
    using Callback = std::function<void()>;
#else
    using Callback = void (*)();
#endif
    uint32_t registerEvent(Callback callback);
    uint32_t setInterval(Callback callback, uint32_t interval);
    uint32_t setTimeout(Callback callback, uint32_t interval);
    void unregisterEvent(uint32_t eventId);
    void clearInterval(uint32_t eventId);
    void clearTimeout(uint32_t eventId);
    void run();

   private:
    struct RegularEvent {
        uint32_t id;
        boolean passed;
        Callback callback;

        RegularEvent() : id(0), passed(false), callback(NULL) {}
        RegularEvent(uint32_t id, Callback callback)
            : id(id), passed(false), callback(callback) {}
    };

    struct TimeEvent {
        uint32_t id;
        uint32_t interval;
        uint32_t counter;
        boolean passed;
        Callback callback;

        TimeEvent() : id(0), interval(0), counter(0), passed(false), callback(NULL) {}
        TimeEvent(uint32_t id, uint32_t interval, uint32_t counter, Callback callback)
            : id(id), interval(interval), counter(counter), passed(false), callback(callback) {}
    };

    ArrayList<RegularEvent> regularEvents;
    ArrayList<TimeEvent> intervalEvents;
    ArrayList<TimeEvent> timeoutEvents;

    static uint32_t id;
    bool regularFlag;
    bool intervalFlag;
    bool timeoutFlag;
};

extern DTimer Timer;

#endif
