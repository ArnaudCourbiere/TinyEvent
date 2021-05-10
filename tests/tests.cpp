#include <gtest/gtest.h>
#include <TinyEvent.h>

using namespace TinyEvent;

// An event to broadcast.
// Any class/struct can be broadcasted as an event.
struct AnEvent
{
    bool one;
    int  two;
};

// An event listener.
class AnEventListener : public EventListener<AnEvent>
{
public:
    bool OnEvent(AnEvent /*unused*/)
    {
        // Do something with event

        called = true;

        // Return true to stop this event propagation to subsequent listeners.
        return false;
    }

    bool called{false};
};

// Demonstrate some basic assertions.
TEST(TinyEvent, BasicTest)
{

    // Queue an event.
    AnEvent event{true, 5};

    // Add an event listener.
    EventBus bus;
    auto     pSharedListener = std::make_shared<AnEventListener>();
    bool     lambdaCalled    = false;

    bus.AddEvent(event);
    bus.AddEventListener<AnEvent>([&lambdaCalled](AnEvent /*unused*/) { lambdaCalled = true; });
    bus.AddEventListener<AnEvent>(pSharedListener);

    ASSERT_FALSE(pSharedListener->called);
    ASSERT_FALSE(lambdaCalled);

    // Process queued events.
    bus.ProcessQueuedEvents();

    ASSERT_TRUE(pSharedListener->called);
    ASSERT_TRUE(lambdaCalled);
}
