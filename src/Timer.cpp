#include "Timer.h"

uint32_t DTimer::id = 0;

DTimer::DTimer()
    : regularEvents(ArrayList<RegularEvent>()), intervalEvents(ArrayList<TimeEvent>()), timeoutEvents(ArrayList<TimeEvent>()), regularFlag(false), intervalFlag(false), timeoutFlag(false) {}

DTimer::~DTimer() {}

uint32_t DTimer::registerEvent(Callback callback) {
    id = millis();
    while (regularEvents.contains([](RegularEvent data) -> bool { return data.id == id; })) id++;
    regularEvents.add(RegularEvent(id, callback));
    return id;
}

uint32_t DTimer::setInterval(Callback callback, uint32_t interval) {
    id = millis();
    while (intervalEvents.contains([](TimeEvent data) -> bool { return data.id == id; })) id++;
    intervalEvents.add(TimeEvent(id, interval, millis(), callback));
    return id;
}

uint32_t DTimer::setTimeout(Callback callback, uint32_t interval) {
    id = millis();
    while (timeoutEvents.contains([](TimeEvent data) -> bool { return data.id == id; })) id++;
    timeoutEvents.add(TimeEvent(id, interval, millis(), callback));
    return id;
}

void DTimer::unregisterEvent(uint32_t eventId) {
    id = eventId;
    int index = regularEvents.indexOf([](RegularEvent data) -> bool { return data.id == id; });
    if (index >= 0) {
        regularEvents[index].passed = true;
        regularFlag = true;
    }
}

void DTimer::clearInterval(uint32_t eventId) {
    id = eventId;
    int index = intervalEvents.indexOf([](TimeEvent data) -> bool { return data.id == id; });
    if (index >= 0) {
        intervalEvents[index].passed = true;
        intervalFlag = true;
    }
}

void DTimer::clearTimeout(uint32_t eventId) {
    id = eventId;
    int index = timeoutEvents.indexOf([](TimeEvent data) -> bool { return data.id == id; });
    if (index >= 0) {
        timeoutEvents[index].passed = true;
        timeoutFlag = true;
    }
}

void DTimer::run() {
    regularEvents.forEach([](RegularEvent& data, size_t index) -> bool {
        if (!data.passed) {
            data.callback();
        }
        return true;
    });

    intervalEvents.forEach([](TimeEvent& data, size_t index) -> bool {
        if (!data.passed && (millis() - data.counter >= data.interval)) {
            data.counter = millis();
            data.callback();
        }
        return true;
    });

    timeoutEvents.forEach([](TimeEvent& data, size_t index) -> bool {
        if (!data.passed && (millis() - data.counter >= data.interval)) {
            data.passed = true;
            data.callback();
        }
        return true;
    });

    if (regularFlag) {
        regularFlag = false;
        regularEvents.removeIf([](RegularEvent data) -> bool {
            return data.passed;
        });
    }

    if (intervalFlag) {
        intervalFlag = false;
        intervalEvents.removeIf([](TimeEvent data) -> bool {
            return data.passed;
        });
    }

    if (timeoutFlag) {
        timeoutFlag = false;
        timeoutEvents.removeIf([](TimeEvent data) -> bool {
            return data.passed;
        });
    }
}

DTimer Timer;
