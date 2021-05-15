#pragma once

#include <vector>
#include <any>
#include <unordered_map>
#include <atomic>
#include <functional>
#include <memory>

namespace TinyEvent
{

template <typename T>
class EventListener
{
public:
    virtual ~EventListener(){};
    virtual void OnEvent(T e) = 0;
};

class EventBus
{
public:
    template <typename T>
    int AddEventListener(std::shared_ptr<EventListener<T>> pEventListener)
    {
        ListenerRecord r = {m_lastListenerId++, [pEventListener](std::any e) { pEventListener->OnEvent(std::any_cast<T>(e)); }};
        m_map[GetTypeId<T>()].listeners.push_back(r);
        return r.id;
    }

    template <typename T>
    int AddEventListener(std::function<void(T)> f)
    {
        ListenerRecord r = {m_lastListenerId++, [f](std::any e) { f(std::any_cast<T>(e)); }};
        m_map[GetTypeId<T>()].listeners.push_back(r);
        return r.id;
    }

    bool RemoveEventListener(int id)
    {
        for (auto& p : m_map)
        {
            auto i = std::begin(p.second.listeners);

            while (i != std::end(p.second.listeners))
            {
                if (i->id == id)
                {
                    p.second.listeners.erase(i);
                    return true;
                }

                i++;
            }
        }

        return false;
    }

    template <typename T>
    void AddEvent(T event)
    {
        m_map[GetTypeId<T>()].events.push_back(event);
    }

    void ProcessQueuedEvents()
    {
        for (auto& p : m_map)
        {
            for (auto& event : p.second.events)
            {
                for (auto& eventListener : p.second.listeners)
                {
                    eventListener.f(event);
                }
            }

            // Clear events.
            p.second.events.clear();
        }
    }

private:
    struct ListenerRecord
    {
        int                           id;
        std::function<void(std::any)> f;
    };

    struct TypeData
    {
        std::vector<std::any>       events;
        std::vector<ListenerRecord> listeners;
    };

    template <typename T>
    inline static int GetTypeId()
    {
        static const int id = s_lastTypeId++;
        return id;
    }

    static std::atomic_int s_lastTypeId;
    std::atomic_int        m_lastListenerId = 0;

    std::unordered_map<int, TypeData> m_map;
};
} // namespace TinyEvent
