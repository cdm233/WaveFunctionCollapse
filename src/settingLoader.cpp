#include "settingLoader.h"
#include "utilities.h"

settingLoader::settingLoader(string path) {
    std::ifstream iniFile(path);
    numSettings = 0;

    while (!iniFile.eof()) {
        string temp;
        iniFile >> temp;
        auto settingPair = parseSettingString(temp);
        setting.insert(settingPair);
    
        numSettings ++;
    }
}

std::pair<string, vector<int>> settingLoader::parseSettingString(std::string setting) {
    std::pair<string,string> tempRe = {};
    std::pair<string,vector<int>> result = {};

    int index = setting.find("=");

    if(index != -1){
        tempRe.first = setting.substr(0, index);
        tempRe.second = setting.substr(index + 1);
    }else{
        return {};
    }
    
    result.first = tempRe.first;
    result.second = string2vec(tempRe.second, ",");

    return result;
}

vector<int> settingLoader::getSetting(string settingLabel){
    return setting[settingLabel];
}