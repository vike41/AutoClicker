#define NOMINMAX

#include <windows.h>
#include <iostream>
#include <limits>
#include <chrono>

class AutoClicker {
    /*
        Main logic for autoclicker
    */
private:
    unsigned int click_period;
    POINT cursor_position;

public:
    AutoClicker() : click_period(0) {
        std::cout << "Initializing Autoclicker...\n";
        setup();
    }

    ~AutoClicker() {
        std::cout << "Autoclicker shutting down. Resources released.\n";
    }

    void setup() {
        std::cout << "Setup Program\n";
        set_click_period();
        std::cout << "Interval is: " << click_period << " ms\n";

        cursor_position = track_mouse_clicks();
        std::cout << "Click position: " << cursor_position.x << ", " << cursor_position.y << "\n";

        execute_auto_click();
        wait_for_exit();
        std::cout << "END" << std::endl;
    }

    void set_click_period() {
        std::cout << "Enter the period (seconds) between clicks: ";
        click_period = getValidInput();
        std::cout << "Click period set to: " << click_period << " ms\n";
    }

    POINT track_mouse_clicks() {
        std::cout << "Click LMB to set position, RMB to cancel.\n";
        while (true) {

            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                GetCursorPos(&cursor_position);
                std::cout << "Mouse clicked at: " << cursor_position.x << ", " << cursor_position.y << "\n";
                Sleep(200);
                return cursor_position;
            }

            if (GetAsyncKeyState(VK_RBUTTON)) {
                std::cout << "Operation cancelled.\n";
                return { 0, 0 };
            }
            Sleep(10);
        }
    }

    void execute_auto_click() {
        std::cout << "Executing auto-click at (" << cursor_position.x << ", " << cursor_position.y << ") every "
            << click_period << " ms.\n";

        auto Start = std::chrono::high_resolution_clock::now();
        while (true) {

            SetCursorPos(cursor_position.x, cursor_position.y);
            left_mouse_click();
            Sleep(click_period * 1000); // Convert sec into millisec  // TODO Extract Value 

            if (GetAsyncKeyState(VK_ESCAPE)) {
                std::cout << "Operation cancelled.\n";
                return;
            }
        }
    }

    void wait_for_exit() {
        std::cout << "Press ESC to exit.\n";
        while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
            Sleep(10);
        }
    }

private:
    unsigned int getValidInput() {
        unsigned int input;
        while (!(std::cin >> input) || input <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a positive integer: ";
        }
        return input;
    }

    void left_mouse_click() {
        INPUT input = { 0 };
        input.type = INPUT_MOUSE;

        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));

        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input, sizeof(INPUT));
    }
};
