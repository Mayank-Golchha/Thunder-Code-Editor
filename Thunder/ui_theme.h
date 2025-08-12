#ifndef UI_THEME_H
#define UI_THEME_H

#include <fstream>
#include <vector>
#include <filesystem>

#include "JsonParser/parser.h"
#include "color_structure.h"


COLORREF hexToColor(const std::string& hex) {
    unsigned int rgb = std::stoul(hex.substr(1), nullptr, 16);
    return RGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}

Theme load_json_ui_them(JsonNode& json,Json* content){
    Theme theme;
    theme.mainWindow = hexToColor(std::string(json["mainWindow"]));
    theme.titlebar = hexToColor(std::string(json["titlebar"]));
    theme.windowControlButtonsbk = hexToColor(std::string(json["windowControlButtonsbk"]));
    theme.windowControlButtonstc = hexToColor(std::string(json["windowControlButtonstc"]));
    theme.hoverButtonColor = hexToColor(std::string(json["hoverButtonColor"]));
    theme.hoverWindowColorbk = hexToColor(std::string(json["hoverWindowColorbk"]));
    theme.hoverWindowColortc = hexToColor(std::string(json["hoverWindowColortc"]));
    theme.menucolorbk = hexToColor(std::string(json["menucolorbk"]));
    theme.menucolortc = hexToColor(std::string(json["menucolortc"]));
    theme.menucolorhoverbk = hexToColor(std::string(json["menucolorhoverbk"]));;
    theme.menucolorhovertc = hexToColor(std::string(json["menucolorhovertc"]));
    theme.hoverwindowhighlightline = hexToColor(std::string(json["hoverwindowhighlightline"]));
    theme.filetabContainer = hexToColor(std::string(json["filetabContainer"]));
    theme.filetabbk = hexToColor(std::string(json["filetabbk"]));
    theme.filetabtc = hexToColor(std::string(json["filetabtc"]));
    theme.filetabhoverbk = hexToColor(std::string(json["filetabhoverbk"]));
    theme.filetabhovertc = hexToColor(std::string(json["filetabhovertc"]));
    theme.scrollbarbk = hexToColor(std::string(json["scrollbarbk"]));
    theme.scrollbarthumb = hexToColor(std::string(json["scrollbarthumb"]));
    theme.editorbk = hexToColor(std::string(json["editorbk"]));
    theme.editortc = hexToColor(std::string(json["editortc"]));
    theme.editorSelectedLinebk = hexToColor(std::string(json["editorSelectedLinebk"]));
    theme.editorSelectedTextbk = hexToColor(std::string(json["editorSelectedTextbk"]));
    theme.autoCompleteOptionbk = hexToColor(std::string(json["autoCompleteOptionbk"]));
    theme.autoCompleteOptiontc = hexToColor(std::string(json["autoCompleteOptiontc"]));
    theme.autoCompleteOptionHoverbk = hexToColor(std::string(json["autoCompleteOptionHoverbk"]));
    theme.autoCompleteOptionHovertc = hexToColor(std::string(json["autoCompleteOptionHovertc"]));
    theme.miniCodeViewerbk = hexToColor(std::string(json["miniCodeViewerbk"]));
    theme.miniCodeViewertc = hexToColor(std::string(json["miniCodeViewertc"]));
    theme.fileExplorerBk = hexToColor(std::string(json["fileExplorerBk"]));
    theme.fileExplorerItemBk = hexToColor(std::string(json["fileExplorerItemBk"]));
    theme.fileExplorerItemTc = hexToColor(std::string(json["fileExplorerItemTc"]));
    theme.fileExplorerHoverItemBk = hexToColor(std::string(json["fileExplorerHoverItemBk"]));
    theme.fileExplorerHoverItemTc = hexToColor(std::string(json["fileExplorerHoverItemTc"]));
    theme.fileExplorerItemSelectedBk = hexToColor(std::string(json["fileExplorerItemSelectedBk"]));
    theme.fileExplorerItemSelectedTc = hexToColor(std::string(json["fileExplorerItemSelectedTc"]));
    theme.inputWindowBk = hexToColor(std::string(json["inputWindowBk"]));
    theme.inputWindowTc = hexToColor(std::string(json["inputWindowTc"]));
    theme.contextMenuBk = hexToColor(std::string(json["contextMenuBk"]));
    theme.contextMenuTc = hexToColor(std::string(json["contextMenuTc"]));
    theme.contextMenuHighLightBk = hexToColor(std::string(json["contextMenuHighLightBk"]));
    theme.contextMenuHighLightTc = hexToColor(std::string(json["contextMenuHighLightTc"]));
    
    content->freeMemory(); // freeing memory of parsed json
    
    return theme;
}

Theme loadTheme(std::string path,Theme &original){
    // to load a theme
    std::string code,line;
    // std::ifstream file(".thunder/ui/ui_colors.json");
    std::ifstream file(path);
    while (std::getline(file, line)) {
        code += line + "\n";
    }
    file.close();
    try{
        Json json_str(code);

        JsonNode json =  *json_str.parse();
        // freeing memory
        // json_str.clear();
        return load_json_ui_them(json,&json_str);
    }
    catch (...){
        return original;
    }
}

struct ThemeStructure{
    std::wstring name;
    std::string path;
};

std::vector<ThemeStructure*> load_all_themes(){
    std::filesystem::path folder(".thunder/ui");
    try {
        std::vector<ThemeStructure*> themes;
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (std::filesystem::is_regular_file(entry.path())) {
                ThemeStructure* t = new ThemeStructure{entry.path().stem().wstring(),entry.path().string()};
                themes.push_back(t);
            }
        }
        return themes;

    } 
    catch (...) {
        return {};
    }
    return {};
}

std::string getEditorThemePath(){
    // will read the ui config json file and and see the theme
    // remember the path written in the ui config file will be different because the file the loads it is not in the same level i.e ui_theme and ui_config
    std::string file_path = "../.thunder/ui_config.json";
    std::string code,line;
    // std::ifstream file(".thunder/ui/ui_colors.json");
    std::ifstream file(file_path);
    while (std::getline(file, line)) {
        code += line + "\n";
    }
    file.close();
    Json json(code);
    JsonNode node = json.parse();
    std::string path = node["app_theme"];
    json.freeMemory();
    return path;
}

std::vector<ThemeStructure*> availableEditorThemes = load_all_themes();



#endif