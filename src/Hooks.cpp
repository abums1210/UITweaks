#include "../includes/Hooks.h"
#include "../includes/SomeMacros.h"
#include "../includes/UIConfig.h"
#include "CFont.h"
#include "CHud.h"
#include "CCamera.h"
#include "CCutsceneMgr.h"
#include "CMessages.h"

#ifdef GTASA
#include "CTheScripts.h"
#include "CMenuManager.h"
#include "CGameLogic.h"
#include "CAudioEngine.h"
#include "CGarages.h"
#include "CTimer.h"
#include "CText.h"
#include "CHudColours.h"
#include "CSprite2d.h"
#include "CStats.h"
#include "CPlayerPed.h"
#include "CPedGroupMembership.h"
#endif

#ifdef GTA3
#define ZVHPrintString PrintString
#else
#define ZVHPrintString PrintStringFromBottom
#endif

#ifndef GTASA
#define ZVMChar wchar_t
#else
#define ZVMChar char
#endif
using namespace plugin;

static float barX = 0.0f, barY = 0.0f, barWidth = 0.0f, barHeight = 0.0f, barProgress = 0.0f;

static void __cdecl PrintZone(float x, float y, ZVMChar *text) {
    float rightPos, bottomPos;
    if (!text || text[0] == '\0')
        return;
    rightPos = SCREEN_SCALE_FROM_RIGHT(config.ZoneVehicleMissionRight);
    bottomPos = SCREEN_SCALE_FROM_BOTTOM(config.ZoneBottom);
    CFont::SetScale(SCREEN_SCALE_X(config.ZoneVehicleMissionScale.x), SCREEN_SCALE_Y(config.ZoneVehicleMissionScale.y));
    if (!config.VCSlantToggle)
        CFont::SetSlant(0.0f);
    CFont::ZVHPrintString(rightPos, bottomPos, text);
}

static void __cdecl PrintVehicle(float x, float y, ZVMChar *text) {
    float rightPos, bottomPos;
    if (!text || text[0] == '\0')
        return;
    rightPos = SCREEN_SCALE_FROM_RIGHT(config.ZoneVehicleMissionRight);
    bottomPos = SCREEN_SCALE_FROM_BOTTOM(config.VehicleBottom);
    CFont::SetScale(SCREEN_SCALE_X(config.ZoneVehicleMissionScale.x), SCREEN_SCALE_Y(config.ZoneVehicleMissionScale.y));
    if (!config.VCSlantToggle)
        CFont::SetSlant(0.0f);
    CFont::ZVHPrintString(rightPos, bottomPos, text);
}

static void __cdecl PrintMission(float x, float y, ZVMChar *text) {
    float rightPos, bottomPos;
    if (!text || text[0] == '\0')
        return;
    rightPos = SCREEN_SCALE_FROM_RIGHT(config.ZoneVehicleMissionRight);
    bottomPos = SCREEN_SCALE_FROM_BOTTOM(config.MissionBottom);
    CFont::SetScale(SCREEN_SCALE_X(config.ZoneVehicleMissionScale.x), SCREEN_SCALE_Y(config.ZoneVehicleMissionScale.y));
    CFont::ZVHPrintString(rightPos, bottomPos, text);
}

static void __cdecl PrintRadioStation(float x, float y, ZVMChar *text) {
    if (!text || text[0] == '\0')
        return;
    CFont::SetFontStyle(config.RadioStationFontStyle);
    CFont::SetScale(SCREEN_SCALE_X(config.RadioStationScale.x), SCREEN_SCALE_Y(config.RadioStationScale.y));
    if (config.RadioStationPosition == 0) {

        CFont::PrintString(SCREEN_SCALE_X(SCREEN_WIDTH / 2.0f), SCREEN_SCALE_Y(config.RadioStationTop), text);
        CFont::SetOrientation(ALIGN_CENTER);
    }
    else {

        CFont::SetOrientation(ALIGN_LEFT);
#ifdef GTAVC
        CFont::PrintStringFromBottom(SCREEN_SCALE_X(config.RadioStationLeft),
                                     SCREEN_SCALE_FROM_BOTTOM(config.RadioStationBottom), text);
#else
        CFont::PrintString(SCREEN_SCALE_X(config.RadioStationLeft), SCREEN_SCALE_FROM_BOTTOM(config.RadioStationBottom),
                           text);
#endif
    }
}


static void __cdecl PrintHelpMessage(float x, float y, ZVMChar *text) {
    float boxWidth, messageWidth, padding, wrapWidth;
    if (!text || text[0] == '\0')
        return;
#ifdef GTASA
    if (CHud::m_nHelpMessageStatId != 0) {
        return;
    }
#endif

    CFont::SetScale(SCREEN_SCALE_X(config.HelpMessageScale.x), SCREEN_SCALE_Y(config.HelpMessageScale.y));

    boxWidth = SCREEN_SCALE_X(config.HelpMessageBoxWidth);
    padding = SCREEN_SCALE_X(8.0f);
    messageWidth = CFont::GetStringWidth(text, true) + padding;

    if (boxWidth > messageWidth) {
        wrapWidth = messageWidth;
    } else {
        wrapWidth = boxWidth;
    }

    CFont::SetWrapx(x + wrapWidth);
    CFont::SetBackgroundColor(config.HelpMessageBoxColour);
#ifndef GTASA
    CFont::SetDropShadowPosition(1);
#else
    CFont::SetEdge(1);
#endif
    CFont::PrintString(x, y, text);
}

#if defined(GTA3) || defined(GTAVC)

static void __cdecl PrintSubtitlesIIIVC(float x, float y, wchar_t *text) {

    float wrapWidth, yPos, radarEdge, rightMargin, availableWidth, centerPos;
    int numLines;

    CFont::SetScale(SCREEN_SCALE_X(config.SubTextScale.x), SCREEN_SCALE_Y(config.SubTextScale.y));

    if (TheCamera.m_bWideScreenOn || CCutsceneMgr::ms_running) {
        centerPos = SCREEN_WIDTH / 2.0f;
        availableWidth = SCREEN_WIDTH;
    } else {
        radarEdge = SCREEN_SCALE_X(config.SubtitleOffsetL) + SCREEN_SCALE_X(8.0f);
        rightMargin = SCREEN_SCALE_X(config.SubtitleOffsetR) + SCREEN_SCALE_X(8.0f);
        availableWidth = SCREEN_WIDTH - radarEdge - rightMargin;
        centerPos = radarEdge + (availableWidth / 2.0f);
    }

    wrapWidth = SCREEN_SCALE_X(config.SubtitleBoxWidth);
    if (wrapWidth > availableWidth)
        wrapWidth = availableWidth;
    if (wrapWidth < 10.0f)
        wrapWidth = 10.0f;

    CFont::SetCentreSize(wrapWidth);

    yPos = SCREEN_SCALE_FROM_BOTTOM(config.SubtitleBottom + 2.0f);
    numLines = CFont::GetNumberLines(centerPos, yPos, text);
    if (numLines > 1) {
#ifdef GTA3
        yPos -= (numLines - 1) * (18.0f * CFont::Details.m_vScale.y);
#elif defined(GTAVC)
        yPos -= (numLines - 1) * (18.0f * CFont::Details.m_vecScale.y);
#endif
    }

    CFont::PrintString(centerPos, yPos, text);
}
#endif

#ifdef GTASA
static void __cdecl DrawHelpTextSA() {
    float xPos, yPos, boxWidth, messageWidth, padding, wrapWidth, alpha, statValue, baseValue, maxStatValue, progress, progressAdd, barX, barY, textWidth;
    unsigned short statId, barWidth;
    unsigned char barHeight;
    const char* statText;
    char statKey[16];
    CRGBA color, addColor;
    static bool& bHelpMessagePermanent = *reinterpret_cast<bool*>(0xBAA464);
    static bool& bHelpMessageQuick = *reinterpret_cast<bool*>(0xBAA472);
    static float& m_fTextBoxNumLines = *reinterpret_cast<float*>(0xBAA460);
    static float& m_fHelpMessageStatUpdateValue = *reinterpret_cast<float*>(0xBAA468);

    if (!CHud::m_pHelpMessage[0]) {
        return;
    }

    if (!CMessages::StringCompare(CHud::m_pHelpMessage, CHud::m_pLastHelpMessage, 400)) {
        switch (CHud::m_nHelpMessageState) {
        case 0:
            CHud::m_nHelpMessageState = 2;
            CHud::m_nHelpMessageTimer = 0;
            CHud::m_nHelpMessageFadeTimer = 0;
            CMessages::StringCopy(CHud::m_pHelpMessageToPrint, CHud::m_pHelpMessage, 400);

            CFont::SetOrientation(ALIGN_LEFT);
            CFont::SetJustify(false);

            boxWidth = SCREEN_SCALE_X(config.HelpMessageBoxWidth);
            messageWidth = CFont::GetStringWidth(CHud::m_pHelpMessageToPrint, true, false);
            padding = SCREEN_SCALE_X(8.0f);

            if (boxWidth > messageWidth + padding) {
                wrapWidth = messageWidth + padding;
            } else {
                wrapWidth = boxWidth;
            }

            CFont::SetWrapx(SCREEN_SCALE_X(30.0f) + wrapWidth);
            CFont::SetFontStyle(FONT_SUBTITLES);
            CFont::SetBackground(true, true);
            CFont::SetDropShadowPosition(0);

            m_fTextBoxNumLines = (float)(CFont::GetNumberLines(SCREEN_SCALE_X(30.0f), SCREEN_SCALE_Y(config.HelpMessageTop), CHud::m_pHelpMessageToPrint) + 3);

            CFont::SetWrapx(SCREEN_WIDTH);
            AudioEngine.ReportFrontendAudioEvent(32, 0.0f, 1.0f);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            CHud::m_nHelpMessageState = 4;
            CHud::m_nHelpMessageTimer = 5;
            break;
        }
        CMessages::StringCopy(CHud::m_pLastHelpMessage, CHud::m_pHelpMessage, 400);
    }

    alpha = 200.0f;
    if (CHud::m_nHelpMessageState != 0) {
        switch (CHud::m_nHelpMessageState) {
        case 1:
            if (!TheCamera.m_bWideScreenOn) {
                CHud::m_nHelpMessageFadeTimer += static_cast<int>(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
                if (CHud::m_nHelpMessageFadeTimer > 1000) {
                    CHud::m_nHelpMessageFadeTimer = 1000;
                    CHud::m_nHelpMessageState = 1;
                }
            }
            break;
        case 2:
            CHud::m_nHelpMessageFadeTimer -= static_cast<int>(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
            if (CHud::m_nHelpMessageFadeTimer <= 0 || TheCamera.m_bWideScreenOn) {
                CHud::m_nHelpMessageFadeTimer = 0;
                CHud::m_nHelpMessageState = 0;
            }
            break;
        case 3:
            CHud::m_nHelpMessageFadeTimer -= static_cast<int>(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
            if (CHud::m_nHelpMessageFadeTimer <= 0) {
                CHud::m_nHelpMessageFadeTimer = 0;
                CHud::m_nHelpMessageState = 2;
                CMessages::StringCopy(CHud::m_pHelpMessageToPrint, CHud::m_pLastHelpMessage, 400);
            }
            break;
        case 0:
            if (bHelpMessagePermanent) {
                CHud::m_nHelpMessageFadeTimer = 600;
            } else {
                if (CHud::m_nHelpMessageTimer > m_fTextBoxNumLines * 1000.0f ||
                    (bHelpMessageQuick && CHud::m_nHelpMessageTimer > 1500.0f)) {
                    CHud::m_nHelpMessageState = 3;
                    CHud::m_nHelpMessageFadeTimer = 600;
                }
            }
            break;
        }
        alpha = static_cast<float>(CHud::m_nHelpMessageFadeTimer) * 0.001f * 200.0f;
    }

    if (CCutsceneMgr::ms_running) {
        return;
    }

    CHud::m_nHelpMessageTimer += static_cast<int>(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
    CFont::SetAlphaFade(alpha);
    CFont::SetProportional(true);
    CFont::SetScaleForCurrentlanguage(SCREEN_SCALE_X(config.HelpMessageScale.x), SCREEN_SCALE_Y(config.HelpMessageScale.y));

    // 1st PrintString & DrawBarChart: Handle stat update text and bar chart when m_nHelpMessageStatId != 0
    if (CHud::m_nHelpMessageStatId != 0) {
        if (!TheCamera.m_bWideScreenOn) {
            statId = CHud::m_nHelpMessageStatId;
            if (statId < 10) {
                sprintf(statKey, "STAT00%d", statId);
            } else if (statId < 100) {
                sprintf(statKey, "STAT0%d", statId);
            } else {
                sprintf(statKey, "STAT%d", statId);
            }

            statText = TheText.Get(statKey);
            xPos = SCREEN_SCALE_X(30.0f);
            yPos = SCREEN_SCALE_Y(config.HelpMessageTop);

            if (statText) {
                textWidth = CFont::GetStringWidth((char*)statText, true, false);
            } else {
                textWidth = 0.0f;
            }

            barWidth = static_cast<unsigned short>(SCREEN_SCALE_X(100.0f));
            barHeight = static_cast<unsigned char>(SCREEN_SCALE_Y(10.0f));
            barX = xPos + textWidth + SCREEN_SCALE_X(12.0f);
            barY = yPos + SCREEN_SCALE_Y(3.0f);

            wrapWidth = textWidth + SCREEN_SCALE_X(12.0f) + barWidth + SCREEN_SCALE_X(8.0f);

            CFont::SetOrientation(ALIGN_LEFT);
            CFont::SetJustify(false);
            CFont::SetWrapx(xPos + wrapWidth);
            CFont::SetFontStyle(FONT_SUBTITLES);
            CFont::SetBackground(true, true);
            CFont::SetDropShadowPosition(0);
            CFont::SetBackgroundColor(config.HelpMessageBoxColour);
            CFont::SetColor(HudColour.GetRGBA(HUD_COLOUR_WHITE));

            if (statText) {
                CFont::PrintString(xPos, yPos, (char*)statText);
            }

            // Render stat progress bar chart
            if (statId == 0x150) {
                CPedGroupMembership* pedGroup = reinterpret_cast<CPedGroupMembership*>(0xC09928);
                statValue = static_cast<float>(pedGroup->CountMembersExcludingLeader());
            } else {
                statValue = CStats::GetStatValue(statId);
            }

            maxStatValue = static_cast<float>(CHud::m_nHelpMessageMaxStatValue);
            if (maxStatValue <= 0.0f) {
                maxStatValue = 1.0f;
            }

            baseValue = statValue - m_fHelpMessageStatUpdateValue;
            if (baseValue < 0.0f) {
                baseValue = 0.0f;
            }

            progress = (baseValue / maxStatValue) * 100.0f;
            progressAdd = (m_fHelpMessageStatUpdateValue / maxStatValue) * 100.0f;

            if (CHud::m_pHelpMessageToPrint[0] == '+') {
                color = HudColour.GetRGB(HUD_COLOUR_GREENDARK, static_cast<unsigned char>(alpha));
                addColor = HudColour.GetRGB(HUD_COLOUR_GREEN, static_cast<unsigned char>(alpha));
            } else {
                color = HudColour.GetRGB(HUD_COLOUR_REDDARK, static_cast<unsigned char>(alpha));
                addColor = HudColour.GetRGB(HUD_COLOUR_RED, static_cast<unsigned char>(alpha));
            }

            CSprite2d::DrawBarChart(barX, barY, barWidth, barHeight, progress, static_cast<signed char>(progressAdd), false, true, color, addColor);
        }
        return;
    }

    if (CHud::m_BigMessage[0][0] || CHud::m_BigMessage[2][0] || CGarages::MessageIDString[0]) {
        return;
    }

    // 2nd PrintString: Handle standard help text message
    CFont::SetOrientation(ALIGN_LEFT);
    CFont::SetJustify(false);

    boxWidth = SCREEN_SCALE_X(config.HelpMessageBoxWidth);
    messageWidth = CFont::GetStringWidth(CHud::m_pHelpMessageToPrint, true, false);
    padding = SCREEN_SCALE_X(8.0f);

    if (boxWidth > messageWidth + padding) {
        wrapWidth = messageWidth + padding;
    } else {
        wrapWidth = boxWidth;
    }

    xPos = SCREEN_SCALE_X(30.0f);
    yPos = SCREEN_SCALE_Y(config.HelpMessageTop);

    CFont::SetWrapx(xPos + wrapWidth);
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetBackground(true, true);
    CFont::SetDropShadowPosition(0);
    CFont::SetBackgroundColor(config.HelpMessageBoxColour);
    CFont::SetColor(CRGBA(255, 255, 255, static_cast<unsigned char>(alpha)));

    CFont::PrintString(xPos, yPos, CHud::m_pHelpMessageToPrint);
    CFont::SetDropShadowPosition(0);
}

static void __cdecl DrawSubtitlesSA() {
    float xPos, yPos, radarEdge, rightMargin, availableWidth, centerPos, wrapWidth;
    int numLines;
    bool isCutscene;
    static bool& byte_BAB214 = *reinterpret_cast<bool*>(0xBAB214);

    if (!CHud::m_Message[0]) {
        return;
    }
    if (CHud::m_BigMessage[0][0] && !CGameLogic::IsCoopGameGoingOn()) {
        return;
    }

    if (CHud::m_VehicleState != 0) {
        CHud::m_VehicleState = 3;
    }
    if (CHud::m_ZoneState != 0) {
        CHud::m_ZoneState = 3;
    }

    CFont::SetBackground(false, false);
    CFont::SetBackgroundColor(CRGBA(0, 0, 0, 128));
    CFont::SetOrientation(ALIGN_CENTER);
    CFont::SetProportional(true);
    CFont::SetDropShadowPosition(0);
    CFont::SetFontStyle(FONT_SUBTITLES);
    CFont::SetColor(CRGBA(225, 225, 225, 255));
    CFont::SetDropShadowPosition(2);
    CFont::SetDropColor(CRGBA(0, 0, 0, 255));

    isCutscene = TheCamera.m_bWideScreenOn || CCutsceneMgr::ms_running;

    if (TheCamera.m_bWideScreenOn) {
        byte_BAB214 = true;
        if (!FrontEndMenuManager.m_bPrefsShowSubtitles && CCutsceneMgr::ms_running) {
            CFont::SetDropShadowPosition(0);
            return;
        }

        if (isCutscene) {
            centerPos = SCREEN_WIDTH / 2.0f;
            availableWidth = SCREEN_WIDTH;
        } else {
            radarEdge = SCREEN_SCALE_X(config.SubtitleOffsetL);
            rightMargin = SCREEN_SCALE_X(config.SubtitleOffsetR);
            availableWidth = SCREEN_WIDTH - radarEdge - rightMargin;
            centerPos = radarEdge + (availableWidth / 2.0f);
        }

        wrapWidth = SCREEN_SCALE_X(config.SubtitleBoxWidth);
        if (wrapWidth > availableWidth)
            wrapWidth = availableWidth;
        if (wrapWidth < 10.0f)
            wrapWidth = 10.0f;

        CFont::SetCentreSize(wrapWidth);
        CFont::SetScale(SCREEN_SCALE_X(config.SubTextScale.x), SCREEN_SCALE_Y(config.SubTextScale.y));

        xPos = centerPos;
        yPos = SCREEN_SCALE_FROM_BOTTOM(config.SubtitleBottom);
    } else {
        if (byte_BAB214) {
            CHud::m_Message[0] = 0;
        }
        byte_BAB214 = false;

        CFont::SetScaleForCurrentlanguage(SCREEN_SCALE_X(config.SubTextScale.x), SCREEN_SCALE_Y(config.SubTextScale.y));

        if (CTheScripts::bUseMessageFormatting) {
            CFont::SetCentreSize(SCREEN_SCALE_X(CTheScripts::MessageWidth));
            xPos = SCREEN_SCALE_X(CTheScripts::MessageCentre);
            yPos = SCREEN_SCALE_FROM_BOTTOM(config.SubtitleBottom);
        } else {
            if (isCutscene) {
                centerPos = SCREEN_WIDTH / 2.0f;
                availableWidth = SCREEN_WIDTH;
            } else {
                radarEdge = SCREEN_SCALE_X(config.SubtitleOffsetL);
                rightMargin = SCREEN_SCALE_X(config.SubtitleOffsetR);
                availableWidth = SCREEN_WIDTH - radarEdge - rightMargin;
                centerPos = radarEdge + (availableWidth / 2.0f);
            }

            wrapWidth = SCREEN_SCALE_X(config.SubtitleBoxWidth);
            if (wrapWidth > availableWidth)
                wrapWidth = availableWidth;
            if (wrapWidth < 10.0f)
                wrapWidth = 10.0f;

            if (CHud::bDrawingVitalStats) {
                CFont::SetScaleForCurrentlanguage(SCREEN_SCALE_X(config.SubTextScale.x * 0.85f), SCREEN_SCALE_Y(config.SubTextScale.y));
                CFont::SetCentreSize(wrapWidth * 0.85f);
            } else {
                CFont::SetCentreSize(wrapWidth);
            }

            xPos = centerPos;
            yPos = SCREEN_SCALE_FROM_BOTTOM(config.SubtitleBottom);
        }
    }

    numLines = CFont::GetNumberLines(xPos, yPos, (char*)CHud::m_Message);
    if (numLines > 1) {
        yPos -= (numLines - 1) * SCREEN_SCALE_Y(18.0f * config.SubTextScale.y);
    }

    CFont::PrintString(xPos, yPos, (char*)CHud::m_Message);
    CFont::SetDropShadowPosition(0);
}

static void __cdecl DrawBarChartSA(float x, float y, unsigned short width, unsigned char height, float progress,
                                   signed char progressAdd, unsigned char drawPercentage, unsigned char drawBlackBorder,
                                   CRGBA color, CRGBA addColor) {
    float posX, posY, indicatorX, indicatorY;
    char indicatorStr[2];

    barWidth = static_cast<float>(width);
    barHeight = static_cast<float>(height);
    barProgress = progress;
    posX = (barX > 0.0f) ? barX : x;
    posY = (barY > 0.0f) ? barY : y;
    CSprite2d::DrawBarChart(posX, posY, width, height, progress, progressAdd, drawPercentage,
                            drawBlackBorder, color, addColor);

    if (CHud::m_pHelpMessageToPrint[0] == '+' || CHud::m_pHelpMessageToPrint[0] == '-') {
        indicatorStr[0] = CHud::m_pHelpMessageToPrint[0];
        indicatorStr[1] = '\0';
        indicatorX = posX + barWidth + SCREEN_SCALE_X(8.0f);
        indicatorY = posY - SCREEN_SCALE_Y(1.5f);
        CFont::SetScale(SCREEN_SCALE_X(config.HelpMessageScale.x), SCREEN_SCALE_Y(config.HelpMessageScale.y));
        CFont::SetOrientation(ALIGN_LEFT);
        CFont::SetBackground(false, false);
        CFont::SetDropShadowPosition(0);
        CFont::SetColor(CRGBA(255, 255, 255, 255));
        CFont::PrintString(indicatorX, indicatorY, indicatorStr);
    }
}

static void __cdecl PrintStatMessage(float x, float y, char *text) {
    float padding, statNameLength, barLength, indicatorLength, totalStatBoxWidth;

    if (!text || text[0] == '\0' || text[0] == '+' || text[0] == '-') {
        return;
    }

    CFont::SetScale(SCREEN_SCALE_X(config.HelpMessageScale.x), SCREEN_SCALE_Y(config.HelpMessageScale.y));

    padding = SCREEN_SCALE_X(8.0f);
    statNameLength = CFont::GetStringWidth(text, true);
    barLength = (barWidth > 0.0f) ? barWidth : SCREEN_SCALE_X(100.0f);
    indicatorLength = CFont::GetStringWidth("+", true);

    barX = x + statNameLength + SCREEN_SCALE_X(12.0f);
    barY = y + SCREEN_SCALE_Y(2.5f * config.HelpMessageScale.y);

    totalStatBoxWidth = statNameLength + SCREEN_SCALE_X(12.0f) + barLength + SCREEN_SCALE_X(8.0f) + indicatorLength + padding;

    CFont::SetWrapx(x + totalStatBoxWidth);
    CFont::SetBackgroundColor(config.HelpMessageBoxColour);
    CFont::SetEdge(1);
    CFont::PrintString(x, y, text);
}


#endif

void Hooks::TriggerHudInfoDisplay() {
#if defined(GTA3)
    if (CHud::m_pZoneName) {
        CHud::m_pLastZoneName = nullptr;
    }
    if (CHud::m_VehicleName) {
        CHud::m_pLastVehicleName = nullptr;
    }
#elif defined(GTAVC) || defined(GTASA)
    if (CHud::m_pZoneName) {
        CHud::m_pLastZoneName = nullptr;
    }
    if (CHud::m_pVehicleName) {
        CHud::m_pLastVehicleName = nullptr;
    }
#endif
}

void Hooks::TemplateHooksVC() { 
#ifdef GTAVC
    plugin::patch::ReplaceFunctionCall(0x55B113, PrintSubtitlesIIIVC);
    plugin::patch::ReplaceFunctionCall(0x5599C2, PrintZone);
    plugin::patch::ReplaceFunctionCall(0x559E8E, PrintVehicle);
    plugin::patch::ReplaceFunctionCall(0x5572E6, PrintMission);
    plugin::patch::ReplaceFunctionCall(0x5FA28A, PrintRadioStation);
    plugin::patch::ReplaceFunctionCall(0x55B66B, PrintHelpMessage);

#endif
}

void Hooks::TemplateHooksIII() { 
#ifdef GTA3
    plugin::patch::ReplaceFunctionCall(0x508953, PrintSubtitlesIIIVC); 
    plugin::patch::ReplaceFunctionCall(0x50719F, PrintZone); // 506C37
    plugin::patch::ReplaceFunctionCall(0x5070FA, PrintZone); // ZONE SHADOW
    plugin::patch::ReplaceFunctionCall(0x50763D, PrintVehicle);
    plugin::patch::ReplaceFunctionCall(0x507598, PrintVehicle); //VEHICLE SHADOW
    plugin::patch::ReplaceFunctionCall(0x50A1DC, PrintMission);
    plugin::patch::ReplaceFunctionCall(0x57EA80, PrintRadioStation);
    plugin::patch::ReplaceFunctionCall(0x509544, PrintHelpMessage);
#endif
}

void Hooks::TemplateHooksSA() {
#ifdef GTASA
    plugin::patch::RedirectJump(0x58C250, DrawSubtitlesSA);
    //plugin::patch::RedirectJump(0x58B6E0, DrawHelpTextSA);
    plugin::patch::ReplaceFunctionCall(0x58AE5D, PrintZone); // 506C37
    plugin::patch::ReplaceFunctionCall(0x58B156, PrintVehicle);
    plugin::patch::ReplaceFunctionCall(0x58D470, PrintMission);
    plugin::patch::ReplaceFunctionCall(0x4E9FF1, PrintRadioStation);
    plugin::patch::ReplaceFunctionCall(0x58C00F, PrintHelpMessage);
    plugin::patch::ReplaceFunctionCall(0x58BD67, PrintStatMessage);
    plugin::patch::ReplaceFunctionCall(0x58BFB8, DrawBarChartSA);
#endif
}

void Hooks::DebugTemplates() {

}
