#pragma once
#include "fstream"
#include "utilities.h"
#include "unordered_map"

class settingLoader {
   public:
    string path;
    std::unordered_map<string, vector<int>> setting;

   public:
    int numSettings;
    settingLoader(string path);
    std::pair<string, vector<int>> parseSettingString(std::string setting);
    vector<int> getSetting(string setting);
};

class singleSettingLoader {
   private:
    string path;
    std::unordered_map<string, string> setting;

   public:
    singleSettingLoader(string path);
    std::pair<string, string> parseSettingString(std::string setting);
    string getSetting(string setting);
};
