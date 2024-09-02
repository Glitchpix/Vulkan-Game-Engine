#include "platform/platform.hpp"

#define ENGINE_PLATFORM_WINDOWS 1

#if ENGINE_PLATFORM_WINDOWS

#define NOMINMAX
#include <core/logger.hpp>

#include <windows.h>
#include <windowsx.h>  // param input extraction
#include <stdlib.h>

LRESULT CALLBACK win32_process_message(HWND hwnd, unsigned int msg, WPARAM w_param, LPARAM l_param);

struct WindowsState : Platform::State {
    HINSTANCE h_instance;
    HWND hwnd;
    LARGE_INTEGER mStart_time;
};

Platform::Platform() {
    mState = std::make_unique<WindowsState>();
}
bool Platform::startup(const std::string& application_name, int x, int y, unsigned int width, unsigned int height) {

    //TODO Check if there's a better way instead of static_cast
    static_cast<WindowsState*>(mState.get())->h_instance = GetModuleHandleA(0);

    // Setup and register window class.
    HICON icon = LoadIcon(static_cast<WindowsState*>(mState.get())->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // Get double-clicks
    wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = static_cast<WindowsState*>(mState.get())->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL; // Manage the cursor manually
    wc.hbrBackground = NULL;                   // Transparent
    wc.lpszClassName = "engine_window_class";

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // Create window
    unsigned int client_x = x;
    unsigned int client_y = y;
    unsigned int client_width = width;
    unsigned int client_height = height;

    unsigned int window_x = client_x;
    unsigned int window_y = client_y;
    unsigned int window_width = client_width;
    unsigned int window_height = client_height;

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

    HWND handle = CreateWindowExA(
        window_ex_style, "engine_window_class", application_name.c_str(),
        window_style, window_x, window_y, window_width, window_height,
        0, 0, static_cast<WindowsState*>(mState.get())->h_instance, 0);

    if (handle == nullptr) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        MSG_FATAL("Window creation failed!");
        return false;
    } else {
        static_cast<WindowsState*>(mState.get())->hwnd = handle;
    }

    // Show the window
    bool should_activate = true;  // TODO: if the window should not accept input, this should be false.
    int show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
    // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
    ShowWindow(static_cast<WindowsState*>(mState.get())->hwnd, show_window_command_flags);

    // Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    mClock_frequency = 1.0 / (double)frequency.QuadPart;
    QueryPerformanceCounter(&static_cast<WindowsState*>(mState.get())->mStart_time);

    return true;
}
void Platform::shutdown() {
    if (static_cast<WindowsState*>(mState.get())->hwnd) {
        DestroyWindow(static_cast<WindowsState*>(mState.get())->hwnd);
        static_cast<WindowsState*>(mState.get())->hwnd = nullptr;
    }
}

bool Platform::pumpMessages() {
    MSG message;
    while(PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return true;
}

void Platform::consoleWrite(const std::string& message, unsigned char colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // Warning levels Fatal to Trace
    static unsigned char levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message.c_str());
    size_t length = strlen(message.c_str());
    LPDWORD number_written = 0;
    WriteConsoleA(console_handle, message.c_str(), static_cast<DWORD>(length), number_written, 0);
}
//TODO refactor this into inner function
void Platform::consoleWriteError(const std::string& message, unsigned char colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // Warning levels Fatal to Trace
    static unsigned char levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message.c_str());
    size_t length = strlen(message.c_str());
    LPDWORD number_written = 0;
    WriteConsoleA(console_handle, message.c_str(), static_cast<DWORD>(length), number_written, 0);
}

double Platform::getAbsoluteTime() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (double)now_time.QuadPart * mClock_frequency;
}

void Platform::sleep(std::size_t ms) {
    Sleep(static_cast<DWORD>(ms));
}

LRESULT CALLBACK win32_process_message(HWND hwnd, unsigned int msg, WPARAM w_param, LPARAM l_param){
    switch (msg) {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            // TODO: Fire quit event here
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // RECT r;
            // GetClientRect(hwnd, &r);
            // unsigned int width = r.right - r.left;
            // unsigned int height = r.bottom - r.top;
            // TODO: Fire resize event here
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // char pressed = (msg == WM_KEYDOWN) || (msg == WM_SYSKEYDOWN);
            // TODO: input process
        } break;
        case WM_MOUSEMOVE: {
            // int x_position GET_X_LPARAM(l_param);
            // int y_position GET_Y_LPARAM(l_param);
            // TODO: mouse process
        } break;
        case WM_MOUSEWHEEL: {
            // int z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            // if (z_delta != 0) {
            //     z_delta = (z_delta < 0) ? -1 : 1;
            // }
            // TODO: mousewheel process
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            // char pressed = (msg == WM_LBUTTONDOWN) || (msg == WM_MBUTTONDOWN) || (msg == WM_RBUTTONDOWN);
            // TODO: input process
        } break;
    }

    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif