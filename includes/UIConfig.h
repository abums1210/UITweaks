#pragma once
#include "ConfigHelper.h"
#include "CVector2D.h"

class UIConfig {
  public:
    CVector2D SubTextScale;
    float SubtitleBottom;
    float SubtitleBoxWidth;
    float SubtitleOffsetL;
    float SubtitleOffsetR;

    int VCSlantToggle;
    CVector2D ZoneVehicleMissionScale;
    float ZoneBottom;
    float VehicleBottom;
    float MissionBottom;
    float ZoneVehicleMissionRight;

    CVector2D HelpMessageScale;
    float HelpMessageTop;
    float HelpMessageBoxWidth;
    CRGBA HelpMessageBoxColour;

    int RadioStationPosition;
    int RadioStationFontStyle;
    float RadioStationBottom;
    float RadioStationTop;
    float RadioStationLeft;
    CVector2D RadioStationScale;

    int ReloadKey;
    
    UIConfig();

    CRGBA GetRGB(CRGBA colour, unsigned char alpha);

    bool Read();
    void Write();
    void GenerateDefault();
};
extern UIConfig config;