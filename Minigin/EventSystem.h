#pragma once
#include <unordered_map>
#include <vector>
#include <functional>

enum class EventID {
    PlayerDied,
	HealthChanged,
	PointsChanged,
};

class EventSystem {
public:
    using EventCallback = std::function<void()>;

    static EventSystem& GetInstance() {
        static EventSystem instance;
        return instance;
    }

    void Subscribe(EventID eventID, EventCallback callback) {
        listeners[eventID].push_back(callback);
    }

    void Notify(EventID eventID) {
        auto it = listeners.find(eventID);
        if (it != listeners.end()) {
            for (const auto& callback : it->second) {
                callback();
            }
        }
    }

private:
    std::unordered_map<EventID, std::vector<EventCallback>> listeners;
};
