#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <utility>
#include <any>
#include <utility>
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
    virtual ~EventListener() { std::cout << "~EventListener()\n"; };
    virtual bool OnEvent(T e) = 0;
};

class EventBus
{
public:
    template <typename T>
    void AddEventListener(std::shared_ptr<EventListener<T>> pEventListener)
    {
        m_map[GetTypeId<T>()].second.push_back([pEventListener](std::any e) { pEventListener->OnEvent(std::any_cast<T>(e)); });
    }

    template <typename T>
    void AddEventListener(std::function<void(T)> f)
    {
        m_map[GetTypeId<T>()].second.push_back([f](std::any e) { f(std::any_cast<T>(e)); });
    }

    template <typename T>
    void RemoveEventListener(/* tbd */)
    {
        // TODO
    }

    template <typename T>
    void AddEvent(T event)
    {
        m_map[GetTypeId<T>()].first.push_back(event);
    }

    void ProcessQueuedEvents()
    {
        for (auto& p : m_map)
        {
            for (auto& event : p.second.first)
            {
                for (auto& eventListener : p.second.second)
                {
                    eventListener(event);
                }
            }

            // Clear events.
            p.second.first.clear();
        }
    }

private:
    template <typename T>
    inline static int GetTypeId()
    {
        static const int id = s_lastTypeId++;
        return id;
    }

    static std::atomic_int s_lastTypeId;

    using GenEventListener = std::function<void(std::any)>;
    using GenVector        = std::vector<std::any>;

    std::unordered_map<int, std::pair<GenVector, std::vector<GenEventListener>>> m_map;
};
} // namespace TinyEvent
