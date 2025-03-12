#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

class EventSystem
{
public:
    using EventCallback = std::function<void()>;

    static EventSystem& GetInstance()
    {
        static EventSystem instance;
        return instance;
    }

    void Subscribe(const std::string& eventName, EventCallback callback)
    {
        m_EventListeners[eventName].push_back(callback);
    }

    void Dispatch(const std::string& eventName)
    {
        if (m_EventListeners.find(eventName) != m_EventListeners.end())
        {
            for (const auto& callback : m_EventListeners[eventName])
            {
                callback();
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<EventCallback>> m_EventListeners;
};
