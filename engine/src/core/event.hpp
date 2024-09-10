#pragma once

#include "defines.hpp"
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <bitset>

class EventManager{
public:
    struct Context{
        union {
            i64 i64[2];
            u64 u64[2];
            f64 f64[2];

            i32 i32[2];
            u32 u32[2];
            f32 f32[2];

            i16 i16[2];
            u16 u16[2];

            i8 i8[2];
            u8 u8[2];

            char c[16];
        };
    };

    enum class EventCode {
        EVENT_CODE_APPLICATION_QUIT = 0x01,

        // Single u16 required, to identify key -> Context.Data.u16[0]
        EVENT_CODE_KEY_PRESSED = 0x02,
        EVENT_CODE_KEY_RELEASED = 0x03,

        // Single u16 required, to identify mouse button -> Context.Data.u16[0]
        EVENT_CODE_MOUSE_BUTTON_PRESSED = 0x04,
        EVENT_CODE_MOUSE_BUTTON_RELEASED = 0x05,

        // Two u16 required, to identify coordinates -> x, y
        EVENT_CODE_MOUSE_MOVED = 0x06,
        EVENT_CODE_MOUSE_WHEEL = 0x07,

        // Two u16 required, new width and height
        EVENT_CODE_WINDOW_RESIZED = 0x08,
    };

    DLL_EXPORT EventManager();

    // https://stackoverflow.com/questions/2298242/callback-functions-in-c
    // https://stackoverflow.com/questions/14306497/performance-of-stdfunction-compared-to-raw-function-pointer-and-void-this
    // https://stackoverflow.com/questions/51477423/stdfunction-and-comparison-of-stdfunction
    // Function pointer cannot trivially be used here due to comparison issues

    // TODO: Return to this issue if more sophisticated callbacks are required
    using event_callback = bool (*)(EventCode code, void* sender, void* listener, Context data);

    DLL_EXPORT bool register_event(EventCode code, void* listener, event_callback callback);

    DLL_EXPORT bool unregister_event(EventCode code, void* listener, event_callback callback);

    DLL_EXPORT bool fire_event(EventCode code, void* sender, Context data);

private:
    struct Event {
        void* listener;
        event_callback callback;

        Event(void* listener_i, event_callback callback_i) : listener{listener_i}, callback{callback_i} {};

        bool operator==(const Event& other) const { return listener == other.listener && callback == other.callback;};
        struct hash
        {
            size_t operator()(const Event& event) const
            {
                size_t rowHash = std::hash<void*>()(event.listener);
                size_t colHash = std::hash<void*>()(event.callback) << 1;
                return rowHash ^ colHash;
            }
        };
    };

    std::unordered_map<EventCode,std::unordered_set<Event,Event::hash>> mRegisteredEvents;
};