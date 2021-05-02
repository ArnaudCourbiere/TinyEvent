# TinyEvent

[![Build Status](https://travis-ci.com/ArnaudCourbiere/TinyEvent.svg?branch=master)](https://travis-ci.com/ArnaudCourbiere/TinyEvent)

Tiny eventing system in C++<br/>
Broadcast and listen to anything.

## Example usage
```C++
#include <TinyEvent.h>

// An event to broadcast.
// Any class/struct can be broadcasted as an event.
struct AnEvent
{
    bool one;
    int two;
};

// An event listener.
class AnEventListener : public TinyEvent::EventListener<AnEvent>
{
public:
    bool OnEvent(AnEvent* pEvent)
    {
        // Do something with event

        // Return true to stop this event propagation to subsequent listeners.
        return false;
    }
};

int main(int argv, char** argv)
{
    // Queue an event.
    AnEvent event;
    event.one =true;
    event.two = 5;

    TinyEvent::EventManager<AnEvent>::Get()->AddEvent(event);

    // Add an event listener.
    AnEventListener eventListener;
    TinyEvent::EventManager<AnEvent>::Get()->AddEventListener(&eventListener);

    // ...

    // Process queued events.
    TinyEvent::BaseEventManager::ProcessEvents();

    return 0;
}
```