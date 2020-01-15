#include "TinyEvent.h"

namespace TinyEvent
{

std::vector<BaseEventManager*> BaseEventManager::s_eventManagers;
unsigned int BaseEventManager::s_id = 0;

}
