#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <iostream>

using EventId = unsigned int;

// SDBM Hash 

template <int length>
struct sdbm_hash {
    consteval static unsigned int _calculate(const char* const text, unsigned int& value) {
        const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
        value = character + (value << 6) + (value << 16) - value;
        return text[length - 1];
    }

    consteval static unsigned int calculate(const char* const text) {
        unsigned int value = 0;
        const auto character = _calculate(text, value);
        return character + (value << 6) + (value << 16) - value;
    }
};

template <> struct sdbm_hash<1> {
    consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
};

template <size_t N>
consteval unsigned int make_sdbm_hash(const char(&text)[N]) {
    return sdbm_hash<N - 1>::calculate(text);
}

// Event System Types

struct EventArg {};

struct Event {
    static constexpr uint8_t MAX_ARGS = 8;
    EventId id;
    uint8_t nbArgs = 0;
    EventArg* args[MAX_ARGS]; 
    explicit Event(EventId _id) : id{ _id }, args{} {}
};

// Event System Core

class EventSystem {
public:
    using EventCallback = std::function<void(const Event&)>;

    static EventSystem& GetInstance() {
        static EventSystem instance;
        return instance;
    }

    void Subscribe(EventId eventID, EventCallback callback) {
        listeners[eventID].push_back(callback);
    }

    void Notify(const Event& event) {
        auto it = listeners.find(event.id);
        if (it != listeners.end()) {
            for (const auto& callback : it->second) {
                callback(event);
            }
        }
    }

private:
    std::unordered_map<EventId, std::vector<EventCallback>> listeners;
};

// Build in Collision events
constexpr EventId OnTriggerEnter = make_sdbm_hash("OnTriggerEnter");
constexpr EventId OnTriggerStay = make_sdbm_hash("OnTriggerStay");
constexpr EventId OnTriggerExit = make_sdbm_hash("OnTriggerExit");
