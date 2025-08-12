// it handles the config 

#ifndef CONFIG_HANDLER
#define CONFIG_HANDLER

#include <vector>
#include <string>
#include <fstream>

#include "editor_resources.h"

namespace ConfigHandler{
    // handles all tabs config like scrolledx selected text
    struct FileTabConfigurations{
        int scrolledX;
        int scrolledY;
        int startSelectionX;
        int startSelectionY;
        int endSelectionX;
        int endSelectionY;
        int isTextSelected;
        int isSelectingText;
        int maxLineWidthValue;
    };

    // std::vector<std::string>

    std::vector<std::string> readOpenedFoldersPath(){
        // handle recent.txt file
        // will read all the projects opened recently
        std::vector<std::string> projects;
        std::string line;
        std::string file_path = ".thunder/config/recent.txt";
        std::ifstream file(file_path);

        if (!file) {
            logger.warning_log("Unable to open recent.txt file");
            return {};
        }

        while (std::getline(file,line)){
            projects.push_back(line);
        }
        if (!projects.size()) projects.push_back("");
        
        file.close();
        
        return projects;
    }

    void appendInRecentFolders(std::string path){
        // handle recent.txt file
        // append the recent opened folder in the recent.txt
        std::string file_path = ".thunder/config/recent.txt";
        std::ofstream file(file_path,std::ios::app);
        file << path << "\n";
        file.close();
    }

    void saveOpenedFoldersPath(std::vector<std::string>& recent){
        // handle recent.txt file
        // will write the vector in the recent.txt
        std::string file_path = ".thunder/config/recent.txt";
        std::ofstream file(file_path);

        if (!file) {
            logger.warning_log("Unable to open recent.txt file");
            return;
        }
        int m = (recent.size()-10 < 0) ? 0:recent.size()-10;
        // for (int i = 0;i<recent.size();i++){
        for (int i = m;i<recent.size();i++){
            file << recent[i] << "\n";
        }

        file.close();
    }
};

#endif