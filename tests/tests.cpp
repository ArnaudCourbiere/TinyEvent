#include <gtest/gtest.h>
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
    bool OnEvent(AnEvent* /*unused*/)
    {
        // Do something with event

        called = true;

        // Return true to stop this event propagation to subsequent listeners.
        return false;
    }

    bool called{ false };
};

// Demonstrate some basic assertions.
TEST(TinyEvent, BasicTest) {

    // Queue an event.
    AnEvent event{ true, 5 };

    TinyEvent::EventManager<AnEvent>::Get()->AddEvent(event);

    // Add an event listener.
    AnEventListener eventListener;
    TinyEvent::EventManager<AnEvent>::Get()->AddEventListener(&eventListener);

    // ...

    ASSERT_FALSE(eventListener.called);

    // Process queued events.
    TinyEvent::BaseEventManager::ProcessEvents();

    ASSERT_TRUE(eventListener.called);
}
