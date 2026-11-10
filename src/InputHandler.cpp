#include "../includes/InputHandler.h"
#include "../includes/UIConfig.h"
#include "../includes/ProcessHelper.h"
#include <windows.h>

typedef SHORT(WINAPI *fnGetKeyState)(int);
static fnGetKeyState pGetKeyState = nullptr;

static bool MyKeyPressed(unsigned int keyCode) {
    if (!pGetKeyState) {
        pGetKeyState = (fnGetKeyState)ProcessHelper::GetFunc("user32.dll", "GetKeyState");
    }
    if (pGetKeyState) {
        return (pGetKeyState(keyCode) & 0x8000) != 0;
    }
    return false;
}

bool InputHandler::oldKeyState[256] = {false};

__declspec(noinline) bool InputHandler::IsKeyJustPressed(unsigned int key) {
    bool current, pressed;

    if (key >= 256) {
        return false;
    }
    current = MyKeyPressed(key);
    pressed = current && !oldKeyState[key];
    if (current) {
        oldKeyState[key] = true;
    } else {
        oldKeyState[key] = false;
    }
    return pressed;
}

bool InputHandler::IsKeyPressed(unsigned int key) {
    if (key >= 256) {
        return false;
    }
    return MyKeyPressed(key);
}

void InputHandler::UpdateOldKeyState() {
    int key;

    key = config.ReloadKey;
    if (key > 0 && key < 256) {
        if (!MyKeyPressed(key)) {
            oldKeyState[key] = false;
        }
    }
}

void InputHandler::ResetKeyState() {
    int i;

    for (i = 0; i < 256; i++) {
        oldKeyState[i] = false;
    }
}
