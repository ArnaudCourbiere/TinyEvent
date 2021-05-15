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

struct AnotherEvent
{
    float f;
};

// An event listener.
class AnEventListener : public EventListener<AnEvent>
{
public:
    void OnEvent(AnEvent /*unused*/)
    {
        // Do something with event

        called = true;
    }

    bool called{false};
};

// Demonstrate some basic assertions.
TEST(TinyEvent, Basic)
{
    EventBus bus;
    AnEvent  event{true, 5};

    auto pSharedListener     = std::make_shared<AnEventListener>();
    bool lambdaCalled        = false;
    bool anotherCalled       = false;
    bool floatListenerCalled = false;

    bus.AddEvent(event);
    bus.AddEvent<AnotherEvent>({0.5});
    bus.AddEvent<float>(1.0);
    bus.AddEventListener<AnEvent>([&lambdaCalled](AnEvent /*unused*/) { lambdaCalled = true; });
    bus.AddEventListener<AnEvent>(pSharedListener);
    bus.AddEventListener<AnotherEvent>([&anotherCalled](AnotherEvent /*unused*/) { anotherCalled = true; });
    bus.AddEventListener<float>([&floatListenerCalled](float /*unused*/) { floatListenerCalled = true; });

    ASSERT_FALSE(pSharedListener->called);
    ASSERT_FALSE(lambdaCalled);
    ASSERT_FALSE(anotherCalled);
    ASSERT_FALSE(floatListenerCalled);

    // Process queued events.
    bus.ProcessQueuedEvents();

    ASSERT_TRUE(pSharedListener->called);
    ASSERT_TRUE(lambdaCalled);
    ASSERT_TRUE(anotherCalled);
    ASSERT_TRUE(floatListenerCalled);
}

TEST(TinyEvent, MultipleEvents)
{
    EventBus bus;

    std::vector<int> eventsIn = {5, 12, 7};
    std::vector<int> eventsOut;

    for (int i : eventsIn)
    {
        bus.AddEvent<int>(i);
    }

    bus.AddEventListener<int>([&eventsOut](int i) { eventsOut.push_back(i); });
    bus.ProcessQueuedEvents();

    ASSERT_TRUE(eventsIn == eventsOut);

    // Previous events should have been remoevd from the bus.
    bus.ProcessQueuedEvents();

    ASSERT_TRUE(eventsIn == eventsOut);
}

TEST(TinyEvent, RemoveListener)
{
    EventBus bus;
    bool     called = false;

    bus.AddEvent<bool>(true);

    int i = bus.AddEventListener<bool>([&called](bool /*unused*/) { called = true; });
    ASSERT_TRUE(bus.RemoveEventListener(i));

    bus.ProcessQueuedEvents();
    ASSERT_FALSE(called);
}

TEST(TinyEvent, RemoveInvalidListener)
{
    EventBus bus;
    ASSERT_FALSE(bus.RemoveEventListener(0));
}

TEST(TinyEvent, ThreadSafety)
{
    // TODO
}
