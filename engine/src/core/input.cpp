#include "input.hpp"
#include "core/event.hpp"
#include "core/logger.hpp"

InputHandler::InputHandler(EventManager& eventManager) : mEventManager{&eventManager} {
    MSG_TRACE("Inputhandler: {:p} created", static_cast<void*>(this));
}

void InputHandler::update(f64 /*unused*/) {
    mInputState.previousKeyBoardState = mInputState.currentKeyBoardState;
    mInputState.currentMouseState = mInputState.currentMouseState;
}

void InputHandler::process_key(Key key, bool pressed) {
    if (mInputState.currentKeyBoardState.keys[key] != pressed) {
        mInputState.currentKeyBoardState.keys[key] = pressed;

        EventManager::Context eventContext{};
        eventContext.i16[0] = static_cast<i16>(key);
        mEventManager->fire_event(pressed ? EventManager::EventCode::EVENT_CODE_KEY_PRESSED : EventManager::EventCode::EVENT_CODE_KEY_RELEASED,
                                  this, eventContext);
    }
}

void InputHandler::process_button(Button button, bool pressed) {
    if (mInputState.currentMouseState.buttons[button] != pressed) {
        mInputState.currentMouseState.buttons[button] = pressed;

        EventManager::Context eventContext{};
        eventContext.i16[0] = static_cast<i16>(button);
        mEventManager->fire_event(pressed ? EventManager::EventCode::EVENT_CODE_MOUSE_BUTTON_PRESSED : EventManager::EventCode::EVENT_CODE_MOUSE_BUTTON_RELEASED,
                                  this, eventContext);
    }
}
void InputHandler::process_mouse_move(i16 x, i16 y) {
    if (mInputState.currentMouseState.x != x || mInputState.currentMouseState.y != y) {
        mInputState.currentMouseState.x = x;
        mInputState.currentMouseState.y = y;

        EventManager::Context eventContext{};
        eventContext.i16[0] = x;
        eventContext.i16[1] = y;

        mEventManager->fire_event(EventManager::EventCode::EVENT_CODE_MOUSE_MOVED,
                                  this, eventContext);
    }
}
void InputHandler::process_mouse_wheel(i8 z_delta) {
    // No internal state for now
    EventManager::Context eventContext{};
    eventContext.i8[0] = z_delta;
    mEventManager->fire_event(EventManager::EventCode::EVENT_CODE_MOUSE_WHEEL,
                              this, eventContext);
}

bool InputHandler::is_key_down(Key key) {
    return mInputState.currentKeyBoardState.keys[key];
}

bool InputHandler::is_key_up(Key key) {
    return !is_key_down(key);
}

bool InputHandler::was_key_down(Key key) {
    return mInputState.previousKeyBoardState.keys[key];
}

bool InputHandler::was_key_up(Key key) {
    return !was_key_down(key);
}

bool InputHandler::is_button_down(Button button) {
    return mInputState.currentMouseState.buttons[button];
}

bool InputHandler::is_button_up(Button button) {
    return !is_button_down(button);
}

bool InputHandler::was_button_down(Button button) {
    return mInputState.previousMouseState.buttons[button];
}

bool InputHandler::was_button_up(Button button) {
    return !was_button_down(button);
}

void InputHandler::get_mouse_position(i32& x, i32& y) const {
    x = mInputState.currentMouseState.x;
    y = mInputState.currentMouseState.y;
}

void InputHandler::get_previous_mouse_position(i32& x, i32& y) const {
    x = mInputState.previousMouseState.x;
    y = mInputState.previousMouseState.y;
}