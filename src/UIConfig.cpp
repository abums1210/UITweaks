#include "../includes/UIConfig.h"

// Standard config paths
#define UI_INI_PATH PLUGIN_PATH("ui_config.ini")

// Global config instance
UIConfig config;

UIConfig::UIConfig() {

    SubTextScale = CVector2D(0.4f, 0.8f);
    SubtitleBottom = 35.0f;
    SubtitleOffsetL = 140.0f;
    SubtitleOffsetR = 60.0f;
    SubtitleBoxWidth = 400.0f;

    VCSlantToggle = 0;
#ifdef GTAVC
    ZoneVehicleMissionScale = CVector2D(0.7f, 1.4f);
    ZoneBottom = 15.0f;
    VehicleBottom = 35.0f;
    MissionBottom = 55.0f;
    RadioStationBottom = 120.0f;
    RadioStationScale = CVector2D(0.5f, 1.0f);
    RadioStationFontStyle = 0;
#elif defined(GTA3)
    ZoneVehicleMissionScale = CVector2D(0.6f, 1.2f);
    ZoneBottom = 40.0f;
    VehicleBottom = 60.0f;
    MissionBottom = 80.0f;
    RadioStationBottom = 140.0f;
    RadioStationScale = CVector2D(0.45f, 0.9f);
    RadioStationFontStyle = 0;
#else
    ZoneVehicleMissionScale = CVector2D(0.7f, 1.4f);
    ZoneBottom = 15.0f;
    VehicleBottom = 35.0f;
    MissionBottom = 55.0f;
    RadioStationBottom = 120.0f;
    RadioStationScale = CVector2D(0.5f, 1.0f);
    RadioStationFontStyle = 1;
#endif
    ZoneVehicleMissionRight = 20.0f;

    HelpMessageScale = CVector2D(0.35f, 0.7f);
    HelpMessageTop = 35.0f;
    HelpMessageBoxWidth = 220.0f;
    HelpMessageBoxColour = CRGBA(0, 0, 0, 128);

    RadioStationPosition = 1; // 0 = default, 1 = over radar
    RadioStationTop = 20.0f;
    RadioStationLeft = 40.0f;

    ReloadKey = 162; // VK_LCONTROL
}

bool UIConfig::Read() {
    std::string uiPath = UI_INI_PATH;
    ConfigHelper cfgUI(uiPath);

    if (!cfgUI.Exists())
        return false;

    ReloadKey = cfgUI.ReadInt("Settings", "ReloadKey", ReloadKey);
    SubTextScale = cfgUI.ReadVec("Settings", "SubTextScale", SubTextScale);
    SubtitleBottom = cfgUI.ReadInt("Settings", "SubtitleBottom", SubtitleBottom);
    SubtitleBoxWidth = cfgUI.ReadInt("Settings", "SubtitleBoxWidth", SubtitleBoxWidth);
    SubtitleOffsetL = cfgUI.ReadInt("Settings", "SubtitleOffsetL", SubtitleOffsetL);
    SubtitleOffsetR = cfgUI.ReadInt("Settings", "SubtitleOffsetR", SubtitleOffsetR);
    VCSlantToggle = cfgUI.ReadInt("Settings", "VCSlantToggle", VCSlantToggle);
    ZoneVehicleMissionScale = cfgUI.ReadVec("Settings", "ZoneVehicleMissionScale", ZoneVehicleMissionScale);
    ZoneBottom = cfgUI.ReadInt("Settings", "ZoneBottom", ZoneBottom);
    VehicleBottom = cfgUI.ReadInt("Settings", "VehicleBottom", VehicleBottom);
    MissionBottom = cfgUI.ReadInt("Settings", "MissionBottom", MissionBottom);
    ZoneVehicleMissionRight = cfgUI.ReadInt("Settings", "ZoneVehicleMissionRight", ZoneVehicleMissionRight);
    HelpMessageScale = cfgUI.ReadVec("Settings", "HelpMessageScale", HelpMessageScale);
    HelpMessageTop = cfgUI.ReadInt("Settings", "HelpMessageTop", HelpMessageTop);
    HelpMessageBoxWidth = cfgUI.ReadInt("Settings", "HelpMessageBoxWidth", HelpMessageBoxWidth);
    HelpMessageBoxColour = cfgUI.ReadRGBA("Settings", "HelpMessageBoxColour", HelpMessageBoxColour);
    RadioStationPosition = cfgUI.ReadInt("Settings", "RadioStationPosition", RadioStationPosition);
    RadioStationFontStyle = cfgUI.ReadInt("Settings", "RadioStationFontStyle", RadioStationFontStyle);
    RadioStationBottom = cfgUI.ReadInt("Settings", "RadioStationBottom", RadioStationBottom);
    RadioStationTop = cfgUI.ReadInt("Settings", "RadioStationTop", RadioStationTop);
    RadioStationLeft = cfgUI.ReadInt("Settings", "RadioStationLeft", RadioStationLeft);
    RadioStationScale = cfgUI.ReadVec("Settings", "RadioStationScale", RadioStationScale);
    return true;
}

void UIConfig::Write() {
    std::string uiPath = UI_INI_PATH;
    ConfigHelper cfgUI(uiPath);

    cfgUI.CreateParentFolder();

    // Write settings
    cfgUI.WriteInt("Settings", "ReloadKey", ReloadKey, "Key to reload font settings in-game (Virtual Key code)");
    cfgUI.WriteVec("Settings", "SubTextScale", SubTextScale, "Subtitle text scale (x = width, y = height)");
    cfgUI.WriteInt("Settings", "SubtitleBottom", SubtitleBottom, "Subtitle bottom position");
    cfgUI.WriteInt("Settings", "SubtitleBoxWidth", SubtitleBoxWidth, "Subtitle box width");
    cfgUI.WriteInt("Settings", "SubtitleOffsetL", SubtitleOffsetL, "Subtitle left offset");
    cfgUI.WriteInt("Settings", "SubtitleOffsetR", SubtitleOffsetR, "Subtitle right offset");
    cfgUI.WriteInt("Settings", "VCSlantToggle", VCSlantToggle, "VC Slant toggle (0 = off, 1 = on)");
    cfgUI.WriteVec("Settings", "ZoneVehicleMissionScale", ZoneVehicleMissionScale,
                   "Zone/Vehicle/Mission text scale (x = width, y = height)");
    cfgUI.WriteInt("Settings", "ZoneBottom", ZoneBottom, "Zone bottom position");
    cfgUI.WriteInt("Settings", "VehicleBottom", VehicleBottom, "Vehicle bottom position");
    cfgUI.WriteInt("Settings", "MissionBottom", MissionBottom, "Mission bottom position");
    cfgUI.WriteInt("Settings", "ZoneVehicleMissionRight", ZoneVehicleMissionRight, "Zone/Vehicle/Mission right position");
    cfgUI.WriteVec("Settings", "HelpMessageScale", HelpMessageScale, "Help message text scale (x = width, y = height)");
    cfgUI.WriteInt("Settings", "HelpMessageTop", HelpMessageTop, "Help message top position");
    cfgUI.WriteInt("Settings", "HelpMessageBoxWidth", HelpMessageBoxWidth, "Help message box width");
    cfgUI.WriteRGBA("Settings", "HelpMessageBoxColour", HelpMessageBoxColour, "Help message box colour");
    cfgUI.WriteInt("Settings", "RadioStationPosition", RadioStationPosition,
                   "Radio station position (0 = default, 1 = over radar)");
    cfgUI.WriteInt("Settings", "RadioStationFontStyle", RadioStationFontStyle, "Radio station font style");
    cfgUI.WriteInt("Settings", "RadioStationBottom", RadioStationBottom, "Radio station bottom position");
    cfgUI.WriteInt("Settings", "RadioStationTop", RadioStationTop, "Radio station top position");
    cfgUI.WriteInt("Settings", "RadioStationLeft", RadioStationLeft, "Radio station left position");
    cfgUI.WriteVec("Settings", "RadioStationScale", RadioStationScale, "Radio station text scale (x = width, y = height)");
    cfgUI.Format("; ui_config.ini - UI configuration\n"
                  "; Settings, parameters for UI rendering \n");
}

void UIConfig::GenerateDefault() {
    std::string uiPath = UI_INI_PATH;
    ConfigHelper cfgUI(uiPath);

    if (!cfgUI.Exists()) {
        Write();
    }
    Read();
}
