#include "input.hpp"
#include "core/event.hpp"
#include "core/logger.hpp"
#include "logger.hpp"

InputHandler::InputHandler(EventManager& eventManager) : mEventManager{&eventManager} {
    MSG_TRACE("Inputhandler: {:p} created", static_cast<void*>(this));
}

void InputHandler::update(f64 /*unused*/) {
    mInputState.previousKeyBoardState = mInputState.currentKeyBoardState;
    mInputState.currentMouseState = mInputState.currentMouseState;
}

void InputHandler::process_key(const Key key, const bool pressed) {
    if (mInputState.currentKeyBoardState.keys.at(key) != pressed) {
        mInputState.currentKeyBoardState.keys.at(key) = pressed;

        EventManager::Context eventContext{};
        eventContext.i16[0] = static_cast<i16>(key);
        mEventManager->fire_event(pressed ? EventManager::EventCode::EVENT_CODE_KEY_PRESSED : EventManager::EventCode::EVENT_CODE_KEY_RELEASED,
                                  this, eventContext);
    }
}

void InputHandler::process_button(const Button button, const bool pressed) {
    if (mInputState.currentMouseState.buttons.at(button) != pressed) {
        mInputState.currentMouseState.buttons.at(button) = pressed;

        EventManager::Context eventContext{};
        eventContext.i16[0] = static_cast<i16>(button);
        mEventManager->fire_event(pressed ? EventManager::EventCode::EVENT_CODE_MOUSE_BUTTON_PRESSED : EventManager::EventCode::EVENT_CODE_MOUSE_BUTTON_RELEASED,
                                  this, eventContext);
    }
}
void InputHandler::process_mouse_move(const i16 x, const i16 y) {
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
void InputHandler::process_mouse_wheel(const i8 z_delta) {
    // No internal state for now
    EventManager::Context eventContext{};
    eventContext.i8[0] = z_delta;
    mEventManager->fire_event(EventManager::EventCode::EVENT_CODE_MOUSE_WHEEL,
                              this, eventContext);
}

bool InputHandler::is_key_down(const Key key) {
    return mInputState.currentKeyBoardState.keys.at(key);
}

bool InputHandler::is_key_up(const Key key) {
    return !is_key_down(key);
}

bool InputHandler::was_key_down(const Key key) {
    return mInputState.previousKeyBoardState.keys.at(key);
}

bool InputHandler::was_key_up(const Key key) {
    return !was_key_down(key);
}

bool InputHandler::is_button_down(const Button button) {
    return mInputState.currentMouseState.buttons.at(button);
}

bool InputHandler::is_button_up(const Button button) {
    return !is_button_down(button);
}

bool InputHandler::was_button_down(const Button button) {
    return mInputState.previousMouseState.buttons.at(button);
}

bool InputHandler::was_button_up(const Button button) {
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