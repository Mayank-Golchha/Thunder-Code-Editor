// //will contain all file tabs that are active

#ifndef OPEN_FILE_TABS_H
#define OPEN_FILE_TABS_H

#include <vector>
#include <windows.h>
#include <string>

struct FileTab {
    HWND handle;
    std::string FileName;
    std::string FilePath;
    HFONT font;
    int hover;
    int id;
    int x;
};


std::vector<FileTab*> fileTabs;


int activeFileTabId = -1;
int totalTabs = 0;

HWND hoverWindowFilePath;



#endif



