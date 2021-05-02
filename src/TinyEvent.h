#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <utility>

namespace TinyEvent
{

template <typename T>
class EventListener
{
public:
    virtual ~EventListener() = default;
    virtual bool OnEvent(T* pEvent) = 0;
};

class BaseEventManager
{
public:
    BaseEventManager() = default;

    static void ProcessEvents()
    {
        for (auto pEventManager : s_eventManagers)
        {
            pEventManager->ProcessQueuedEvents();
        }
    }

protected:
    virtual ~BaseEventManager() = default;
    virtual void ProcessQueuedEvents() = 0;

    void AddEventManager(BaseEventManager* pMgr)
    {
        s_eventManagers.push_back(pMgr);
    }

private:
    BaseEventManager(const BaseEventManager&) = delete;
    BaseEventManager(BaseEventManager&&) = delete;
    void operator=(const BaseEventManager&) = delete;

    static std::vector<BaseEventManager*> s_eventManagers;
};

template <typename T>
class EventManager final : public BaseEventManager
{
public:
    static EventManager<T>*Get()
    {
        static EventManager<T> eventManager;
        return &eventManager;
    }

    void AddEvent(T & event)
    {
        m_pendingEvents.push_back(std::move(event));
    }

    void AddEventListener(EventListener<T>*pEventListener)
    {
        if (pEventListener != nullptr)
        {
            m_eventListeners.push_back(pEventListener);
        }
    }

    bool RemoveEventListener(EventListener<T>*pEventListener)
    {
        if (m_processingEvents)
        {
            return false;
        }

        bool found = false;

        auto it = std::find(m_eventListeners.begin(), m_eventListeners.end(), pEventListener);

        if (it != m_eventListeners.end())
        {
            found = true;
            m_eventListeners.erase(it);
        }

        return found;
    }

protected:
    void ProcessQueuedEvents() override
    {
        m_processingEvents = true;

        if (m_eventListeners.size() > 0)
        {
            for (T& event : m_pendingEvents)
            {
                for (EventListener<T>* pEventListener : m_eventListeners)
                {
                    bool stopProcessing = pEventListener->OnEvent(&event);

                    if (stopProcessing)
                    {
                        break;
                    }
                }
            }
        }

        m_pendingEvents.clear();
        m_processingEvents = false;
    }

    EventManager()
    {
        AddEventManager(this);
    }

private:
    EventManager(const EventManager&) = delete;
    EventManager(EventManager&&) = delete;
    void operator=(const EventManager&) = delete;

    std::vector<T> m_pendingEvents;
    std::vector<EventListener<T>*> m_eventListeners;
    bool m_processingEvents = false;
};

}
