#pragma once

class InputHandler {
public:
    static bool oldKeyState[256];

    __declspec(noinline) static bool IsKeyJustPressed(unsigned int key);
    static bool IsKeyPressed(unsigned int key);
    static void UpdateOldKeyState();
    static void ResetKeyState();
};
