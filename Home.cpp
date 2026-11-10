#include "includes/Hooks.h"
#include "includes/UIConfig.h"
#include "includes/InputHandler.h"
#include <plugin.h>

#ifdef GTASA
#include "CHud.h"
#include "CTimer.h"
#include "CMessages.h"
#include <cstdlib>
#endif

using namespace plugin;

#ifdef GTASA
static const unsigned short validStats[] = { 21, 22, 23, 68, 160, 163, 165, 225 };
#endif

static void OnGameProcess() {
    if (config.ReloadKey != 0) {
        if (config.ReloadKey == -1 || InputHandler::IsKeyJustPressed(config.ReloadKey)) {
            config.Read();
        }
        InputHandler::UpdateOldKeyState();
    }
}

static void OnDrawHud() {
#if DEBUG
#ifdef GTASA
    static unsigned int lastPressTime = 0;
    static char msgBuf[128];
    unsigned int now;
    unsigned short randomStat;
    int index;
#endif

    if (InputHandler::IsKeyJustPressed('Z')) {
        Hooks::TriggerHudInfoDisplay();
    }

#ifdef GTASA
    if (InputHandler::IsKeyJustPressed('L')) {
        now = CTimer::m_snTimeInMilliseconds;
        if (now - lastPressTime > 250) {
            lastPressTime = now;
            index = rand() % (sizeof(validStats) / sizeof(validStats[0]));
            randomStat = validStats[index];
            CHud::SetHelpMessageStatUpdate(1, randomStat, 10.0f, 1000.0f);
        }
    }

    if (InputHandler::IsKeyJustPressed('U')) {
        now = CTimer::m_snTimeInMilliseconds;
        if (now - lastPressTime > 250) {
            lastPressTime = now;
            strcpy_s(msgBuf, "MISSION PASSED!~n~~w~RESPECT +");
            CMessages::AddBigMessage(msgBuf, 5000, 1);
        }
    }
#endif
#endif
}

class Home {
  public:
    Home() {
#ifdef GTA3
        Hooks::TemplateHooksIII();
#elif defined(GTAVC)
        Hooks::TemplateHooksVC();
#else
        Hooks::TemplateHooksSA();
#endif
        Events::initRwEvent += [] {
            config.GenerateDefault();
            if (config.ReloadKey) {
                Events::gameProcessEvent += OnGameProcess;
            }
            Events::drawHudEvent += OnDrawHud;
        };
    }
} home;
