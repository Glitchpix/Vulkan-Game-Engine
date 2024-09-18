#pragma once
#include "platform/platform.hpp"
#include <windows.h>

LRESULT CALLBACK win32_process_message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct WindowsState : Platform::State {
    HINSTANCE h_instance{};
    HWND hwnd{};
    LARGE_INTEGER mStart_time{};
};