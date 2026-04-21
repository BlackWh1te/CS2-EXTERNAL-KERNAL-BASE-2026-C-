#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "Downloader.h"

// Lightweight JSON-like parser for the dumper's specific output format
// Updated: April 2026 - Latest CS2 Build
struct Offsets {
    // Globals (from offsets.json) - NEW
    uintptr_t dwEntityList = 0x24D0D30;          // 38573840
    uintptr_t dwLocalPlayerCtrl = 0x22FC0C0;      // 36712064
    uintptr_t dwLocalPlayerPawn = 0x2050E10;      // 33879600
    uintptr_t dwViewMatrix = 0x2336B30;          // 36868304
    uintptr_t dwViewAngles = 0x2342D68;           // 36932968
    uintptr_t dwPlantedC4 = 0x2326FC8;            // 36902984
    uintptr_t dwGameRules = 0x19F1C00;            // 27171712
    uintptr_t dwNetworkGameClient = 0x909040;     // 9478336
    uintptr_t dwGlobalVars = 0x203E068;          // 33834216
    uintptr_t dwGlowManager = 0x2334A28;          // 36833192
    uintptr_t dwGlow = 0x0;                       // Reserved for glow

    // Schemas (client.dll) - NEW
    uintptr_t m_iHealth = 0x34C;                 // 844
    uintptr_t m_iTeamNum = 0x3EB;                // 1003
    uintptr_t m_pGameSceneNode = 0x330;          // 816
    uintptr_t m_vecViewOffset = 0xE70;           // 3696
    uintptr_t m_vecAbsVelocity = 0x3FC;          // 1020
    uintptr_t m_modelState = 0x150;              // 336
    uintptr_t m_hPlayerPawn = 0x904;            // 2308
    uintptr_t m_iszPlayerName = 0x6F0;          // 1776
    uintptr_t m_ArmorValue = 0x1CB4;             // 7348
    uintptr_t m_flC4Blow = 0x1190;              // 4496
    uintptr_t m_bBombTicking = 0x1160;            // 4448
    uintptr_t m_bBombDefused = 0x1190;           // 4496 (same as m_flC4Blow in new build)
    uintptr_t m_nBombSite = 0x1164;              // 4452
    uintptr_t m_flTimerLength = 0x1198;          // 4504
    uintptr_t m_bBeingDefused = 0x1168;         // 4456
    uintptr_t m_flDefuseCountDown = 0x11B0;      // 4528
    uintptr_t m_flDefuseLength = 0x11AC;         // 4524
    uintptr_t m_steamID = 0x778;                 // 1912
    uintptr_t m_iIDEntIndex = 0x3434;            // 13340
    uintptr_t m_flFlashDuration = 0x1400;        // 5120
    uintptr_t m_iShotsFired = 0x1CB4;            // 7348 (same as m_ArmorValue)
    uintptr_t m_aimPunchAngle = 0x14D4;           // 5332
    uintptr_t m_pObserverServices = 0x1208;       // 4600
    uintptr_t m_hObserverTarget = 0x4C;           // 76

    // Skin Changer & Others - NEW
    uintptr_t m_pWeaponServices = 0x11E0;         // 4576
    uintptr_t m_hMyWeapons = 0x48;               // 72
    uintptr_t m_hActiveWeapon = 0x60;             // 96
    uintptr_t m_flThrowVelocity = 0x1E70;        // 7792
    uintptr_t m_bPinPulled = 0x1F43;              // 8003
    uintptr_t m_fThrowTime = 0x1F48;              // 8008
    uintptr_t m_flThrowStrength = 0x1F50;       // 8016
    uintptr_t m_iItemDefinitionIndex = 0x1BA;    // 442
    uintptr_t m_entitySpottedState = 0x1CB8;     // 7368
    uintptr_t m_bSpotted = 0x8;                  // 8
    uintptr_t m_nFallbackPaintKit = 0x1658;       // 5720
    uintptr_t m_flFallbackWear = 0x1660;          // 5728
    uintptr_t m_nFallbackStatTrak = 0x1664;      // 5732
    uintptr_t m_iItemIDHigh = 0x1D0;              // 464
    uintptr_t m_iAccountID = 0x1D8;              // 472
    uintptr_t m_OriginalOwnerXuidLow = 0x166C;    // 5740
    uintptr_t m_hOwnerEntity = 0x528;             // 1320
    uintptr_t m_clrRender = 0xB80;               // 2944
    uintptr_t m_nDeltaTick = 0x24C;               // 588
    uintptr_t m_Glow = 0xCC0;                    // 3264 (CGlowProperty)

    // Custom
    uintptr_t m_nodeAbsOrigin = 0xD0;            // 208
    uintptr_t m_boneArray = 0x80;                // 128

    bool UpdateFromWeb() {
        std::cout << "[*] Downloading offsets from GitHub..." << std::endl;
        std::string offsetsUrl = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json";
        std::string clientUrl  = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client_dll.json";

        std::string offsetsJson = Downloader::DownloadString(offsetsUrl);
        if (offsetsJson.empty()) std::cout << "[!] Failed to download offsets.json" << std::endl;
        
        std::string clientJson  = Downloader::DownloadString(clientUrl);
        if (clientJson.empty()) std::cout << "[!] Failed to download client_dll.json" << std::endl;

        if (offsetsJson.empty() && clientJson.empty()) return false;

        if (!offsetsJson.empty()) ParseJSON(offsetsJson);
        if (!clientJson.empty())  ParseJSON(clientJson);

        return true;
    }

    bool LoadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return false;

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        ParseJSON(content);
        return true;
    }

private:
    void ParseJSON(const std::string& json) {
        std::stringstream ss(json);
        std::string line;
        while (std::getline(ss, line)) {
            size_t colon = line.find(':');
            if (colon == std::string::npos) continue;

            size_t firstQuote = line.find('"');
            size_t secondQuote = line.find('"', firstQuote + 1);
            if (firstQuote == std::string::npos || secondQuote == std::string::npos) continue;

            std::string key = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
            std::string valStr = line.substr(colon + 1);

            size_t firstDigit = valStr.find_first_of("0123456789");
            if (firstDigit == std::string::npos) continue;
            size_t lastDigit = valStr.find_last_of("0123456789");
            valStr = valStr.substr(firstDigit, lastDigit - firstDigit + 1);

            try {
                uintptr_t val = (uintptr_t)std::stoll(valStr);
                UpdateValue(key, val);
            } catch (...) {}
        }
    }

private:
    void UpdateValue(const std::string& key, uintptr_t val) {
        if (key == "dwEntityList") dwEntityList = val;
        else if (key == "dwLocalPlayerController" || key == "dwLocalPlayerCtrl") dwLocalPlayerCtrl = val;
        else if (key == "dwLocalPlayerPawn") dwLocalPlayerPawn = val;
        else if (key == "dwViewMatrix") dwViewMatrix = val;
        else if (key == "dwViewAngles") dwViewAngles = val;
        else if (key == "dwPlantedC4") dwPlantedC4 = val;
        else if (key == "dwGameRules") dwGameRules = val;
        else if (key == "dwNetworkGameClient") dwNetworkGameClient = val;
        else if (key == "dwGlobalVars") dwGlobalVars = val;
        else if (key == "dwGlowManager") dwGlowManager = val;
        else if (key == "m_iHealth") m_iHealth = val;
        else if (key == "m_iTeamNum") m_iTeamNum = val;
        else if (key == "m_pGameSceneNode") m_pGameSceneNode = val;
        else if (key == "m_vecViewOffset") m_vecViewOffset = val;
        else if (key == "m_vecAbsVelocity") m_vecAbsVelocity = val;
        else if (key == "m_modelState") m_modelState = val;
        else if (key == "m_hPlayerPawn") m_hPlayerPawn = val;
        else if (key == "m_iszPlayerName") m_iszPlayerName = val;
        else if (key == "m_ArmorValue") m_ArmorValue = val;
        else if (key == "m_flC4Blow") m_flC4Blow = val;
        else if (key == "m_bBombTicking") m_bBombTicking = val;
        else if (key == "m_bBombDefused") m_bBombDefused = val;
        else if (key == "m_nBombSite") m_nBombSite = val;
        else if (key == "m_flTimerLength") m_flTimerLength = val;
        else if (key == "m_bBeingDefused") m_bBeingDefused = val;
        else if (key == "m_flDefuseCountDown") m_flDefuseCountDown = val;
        else if (key == "m_flDefuseLength") m_flDefuseLength = val;
        else if (key == "m_steamID") m_steamID = val;
        else if (key == "m_iIDEntIndex") m_iIDEntIndex = val;
        else if (key == "m_flFlashDuration") m_flFlashDuration = val;
        else if (key == "m_iShotsFired") m_iShotsFired = val;
        else if (key == "m_aimPunchAngle") m_aimPunchAngle = val;
        else if (key == "m_pWeaponServices") m_pWeaponServices = val;
        else if (key == "m_pObserverServices") m_pObserverServices = val;
        else if (key == "m_hObserverTarget") m_hObserverTarget = val;
        else if (key == "m_hMyWeapons") m_hMyWeapons = val;
        else if (key == "m_hActiveWeapon") m_hActiveWeapon = val;
        else if (key == "m_iItemDefinitionIndex") m_iItemDefinitionIndex = val;
        else if (key == "m_nFallbackPaintKit") m_nFallbackPaintKit = val;
        else if (key == "m_flFallbackWear") m_flFallbackWear = val;
        else if (key == "m_nFallbackStatTrak") m_nFallbackStatTrak = val;
        else if (key == "m_iItemIDHigh") m_iItemIDHigh = val;
        else if (key == "m_iAccountID") m_iAccountID = val;
        else if (key == "m_OriginalOwnerXuidLow") m_OriginalOwnerXuidLow = val;
        else if (key == "m_hOwnerEntity") m_hOwnerEntity = val;
        else if (key == "m_clrRender") m_clrRender = val;
        else if (key == "m_nDeltaTick") m_nDeltaTick = val;
        else if (key == "m_flThrowVelocity") m_flThrowVelocity = val;
        else if (key == "m_bPinPulled") m_bPinPulled = val;
        else if (key == "m_fThrowTime") m_fThrowTime = val;
        else if (key == "m_flThrowStrength") m_flThrowStrength = val;
        else if (key == "m_entitySpottedState") m_entitySpottedState = val;
        else if (key == "m_bSpotted") m_bSpotted = val;
        else if (key == "m_Glow") m_Glow = val;
    }
};

extern Offsets g_Off;
