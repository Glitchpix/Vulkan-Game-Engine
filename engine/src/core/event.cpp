#include "core/event.hpp"
#include "core/logger.hpp"
#include <stdexcept>


EventManager::EventManager() {
    MSG_TRACE("EventManager: {:p} created", static_cast<void*>(this));
}

bool EventManager::register_event(EventCode code, void* listener, event_callback callback) {
    auto emplaceResult = mRegisteredEvents[code].emplace(listener, callback);
    if (!emplaceResult.second) {
        MSG_WARN("Listener: {:p} tried to register already existing callback: {:p} with code: {:x}", static_cast<void*>(listener), reinterpret_cast<void*>(callback), static_cast<int>(code))
        return false;
    }
    MSG_TRACE("Listener: {:p} registered callback: {:p} with code: {:x}", static_cast<void*>(listener), reinterpret_cast<void*>(callback), static_cast<int>(code))
    return true;
}

bool EventManager::unregister_event(EventCode code, void* listener, event_callback callback) {
    try {
        auto codeEvents = mRegisteredEvents.at(code);
    }
    catch (const std::out_of_range& e) {
        MSG_WARN("Listener: {:p} tried to unregister non-registered event callback: {:p} with code: {:x}, Error: {}", static_cast<void*>(listener), reinterpret_cast<void*>(callback), static_cast<int>(code), e.what());
        return false;
    }

    for (auto&& eventIt = mRegisteredEvents[code].begin(); eventIt != mRegisteredEvents[code].end(); ++eventIt) {
        if (eventIt->listener == listener && eventIt->callback == callback) {
            mRegisteredEvents[code].erase(eventIt);
            MSG_TRACE("Listener: {:p} unregistered event callback: {:p} with code: {:x}", static_cast<void*>(listener), reinterpret_cast<void*>(callback), static_cast<int>(code))
            return true;
        }
    };

    MSG_WARN("Listener: {:p} tried to unregister non-registered event callback: {:p} with code: {:x}", static_cast<void*>(listener), reinterpret_cast<void*>(callback), static_cast<int>(code));
    return false;
}

bool EventManager::fire_event(EventCode code, void* sender, Context data) {
    try {
        auto codeEvents = mRegisteredEvents.at(code);
    }
    catch (const std::out_of_range& e) {
        MSG_WARN("Sender: {:p} tried to fire non-registered event with code: {:x}, Error: {}", sender, static_cast<int>(code), e.what());
        return false;
    }

    for (auto&& event : mRegisteredEvents[code]) {
        bool isHandled = event.callback(code, sender, event.listener, data);
        if (isHandled) {
            return true;
        }
    }

    return false;
}