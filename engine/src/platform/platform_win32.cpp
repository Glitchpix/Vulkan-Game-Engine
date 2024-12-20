#include "core/event.hpp"
#include "platform.hpp"
#include <memory>
#if ENGINE_PLATFORM_WINDOWS
#define NOMINMAX
#include "core/input.hpp"
#include "core/logger.hpp"

#include "platform_win32.hpp"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <windowsx.h>  // param input extraction


Platform::Platform(InputHandler& inputHandler, EventManager& eventHandler) {
    mState = std::make_unique<WindowsState>();
    mContext = std::make_unique<EventContext>(&inputHandler, &eventHandler);
    MSG_TRACE("Platform: {:p} created", static_cast<void*>(this));
}
bool Platform::startup(const std::string& application_name, int x, int y, int width, int height) {
    //TODO Check if there's a better way instead of static_cast
    dynamic_cast<WindowsState*>(mState.get())->h_instance = GetModuleHandleA(0);

    // Setup and register window class.
    HICON icon = LoadIcon(dynamic_cast<WindowsState*>(mState.get())->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // Get double-clicks
    wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = dynamic_cast<WindowsState*>(mState.get())->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);  // NULL; // Manage the cursor manually
    wc.hbrBackground = nullptr;  // Transparent
    wc.lpszClassName = "engine_window_class";

    if (RegisterClassA(&wc) == 0U) {
        MessageBoxA(nullptr, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // Create window
    int client_x = x;
    int client_y = y;
    int client_width = width;
    int client_height = height;

    int window_x = client_x;
    int window_y = client_y;
    int window_width = client_width;
    int window_height = client_height;

    unsigned int window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    unsigned int window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(window_ex_style, "engine_window_class", application_name.c_str(), window_style,
                                  window_x, window_y, window_width, window_height, nullptr, nullptr,
                                  dynamic_cast<WindowsState*>(mState.get())->h_instance, mContext.get());

    if (handle == nullptr) {
        MessageBoxA(nullptr, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        MSG_FATAL("Window creation failed!");
        return false;
    }

    dynamic_cast<WindowsState*>(mState.get())->hwnd = handle;

    // Show the window
    bool should_activate = true;  // TODO: if the window should not accept input, this should be false.
    int show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
    // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
    ShowWindow(dynamic_cast<WindowsState*>(mState.get())->hwnd, show_window_command_flags);

    // Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    mClock_frequency = 1.0 / (double)frequency.QuadPart;
    QueryPerformanceCounter(&dynamic_cast<WindowsState*>(mState.get())->mStart_time);

    MSG_TRACE("Platform: {:p} initialized", static_cast<void*>(this));
    return true;
}
void Platform::shutdown() {
    if (dynamic_cast<WindowsState*>(mState.get())->hwnd != nullptr) {
        DestroyWindow(dynamic_cast<WindowsState*>(mState.get())->hwnd);
        dynamic_cast<WindowsState*>(mState.get())->hwnd = nullptr;
    }
}

bool Platform::pumpMessages() {
    MSG message;
    while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE) != 0) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return true;
}

void Platform::consoleWrite(const std::string& message, unsigned char colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, Logger::logSeverityColours.at(colour));

    OutputDebugStringA(message.c_str());
    size_t length = strlen(message.c_str());
    LPDWORD number_written = 0;
    WriteConsoleA(console_handle, message.c_str(), static_cast<DWORD>(length), number_written, 0);
}
//TODO refactor this into inner function
void Platform::consoleWriteError(const std::string& message, unsigned char colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(console_handle, Logger::logSeverityColours.at(colour));

    OutputDebugStringA(message.c_str());
    size_t length = strlen(message.c_str());
    LPDWORD number_written = nullptr;
    WriteConsoleA(console_handle, message.c_str(), static_cast<DWORD>(length), number_written, 0);
}

double Platform::getAbsoluteTime() const {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (double)now_time.QuadPart * mClock_frequency;
}

Platform::State* Platform::getState() {
    return mState.get();
}

void Platform::sleep(std::size_t ms) {
    Sleep(static_cast<DWORD>(ms));
}

LRESULT CALLBACK win32_process_message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Get pointer to platform instance of input handler
    Platform::EventContext* eventContext = nullptr;
    if (msg == WM_CREATE) {
        auto* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        eventContext = reinterpret_cast<Platform::EventContext*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventContext));
    } else {
        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        eventContext = reinterpret_cast<Platform::EventContext*>(ptr);
    }
    switch (msg) {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            eventContext->eventManager->fire_event(EventManager::EventCode::EVENT_CODE_APPLICATION_QUIT,
                                                   static_cast<void*>(hwnd), EventManager::Context{});
            return 1;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            RECT r;
            GetClientRect(hwnd, &r);
            u16 width = r.right - r.left;
            u16 height = r.bottom - r.top;
            EventManager::Context eventData{};
            eventData.i16[0] = width;
            eventData.i16[1] = height;
            eventContext->eventManager->fire_event(EventManager::EventCode::EVENT_CODE_WINDOW_RESIZED,
                                                   static_cast<void*>(hwnd), eventData);
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            bool pressed = (msg == WM_KEYDOWN) || (msg == WM_SYSKEYDOWN);
            auto key = static_cast<InputHandler::Key>(wParam);
            eventContext->inputHandler->process_key(key, pressed);
        } break;
        case WM_MOUSEMOVE: {
            i16 x_position = static_cast<i16>(GET_X_LPARAM(lParam));
            i16 y_position = static_cast<i16>(GET_Y_LPARAM(lParam));

            eventContext->inputHandler->process_mouse_move(x_position, y_position);
        } break;
        case WM_MOUSEWHEEL: {
            i8 z_delta = static_cast<i8>(GET_WHEEL_DELTA_WPARAM(wParam));
            if (z_delta != 0) {
                z_delta = (z_delta < 0) ? -1 : 1;
                eventContext->inputHandler->process_mouse_wheel(z_delta);
            }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            bool pressed = (msg == WM_LBUTTONDOWN) || (msg == WM_MBUTTONDOWN) || (msg == WM_RBUTTONDOWN);
            InputHandler::Button button{};
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = InputHandler::Button::BUTTON_LEFT;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = InputHandler::Button::BUTTON_RIGHT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = InputHandler::Button::BUTTON_MIDDLE;
                    break;
            }
            eventContext->inputHandler->process_button(button, pressed);
        } break;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

#endif