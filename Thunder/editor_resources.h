//this file was created to remove circular dependency between editor_function.h and file_explorer.h file

#ifndef EDITOR_RESOURCES_H
#define EDITOR_RESOURCES_H

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

#include "Plugins/language_type.h"
#include "dimensions.h"
#include "string_operations.h"
#include "logger.h"

float horzThumbX = 0,vertThumbY = 0;
float horzThumbWidth = 0,vertThumbHeight = 0;

int editorWidth = 0,editorHeight = 0;

float scrolledY = 0,scrolledX = 0;

int hScrolling = 0,vScrolling = 0;

int cursorX = 0,cursorY = 0;

int maxLineWidthValue = 0;

static DWORD last_click_time = 0; //to find the last time when clicked

//selecting word = 1 while selecting 
//wordselected = 1 while a word is selected
int isselectingWord = 0,isWordSelected = 0;

int startSelectionX = -1,startSelectionY = -1;
int endSelectionX = -1,endSelectionY = -1;

int drawMiniCodeViewer = 1; // should mini code viewer be drawn

std::atomic<int> plugin_task_id = 0;

HWND hautoComplete = NULL;

LANGUAGE_TYPE language_type = LANGUAGE_TYPE::TEXT;

std::vector<std::wstring> editContent;

HWND hEditorContextMenu = NULL; // the context menu window



HFONT editorFont = CreateFont(
    -EDITOR_FONT_HEIGHT, 0, 0, 0,           // Height, Width (0 = auto)
    // FW_NORMAL,              // Weight
    EDITOR_FONT_WEIGHT,
    FALSE, FALSE, FALSE,    // Italic, Underline, StrikeOut
    ANSI_CHARSET,           // Character set
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY,
    FIXED_PITCH | FF_MODERN,// Fixed-pitch monospaced
    L"Consolas"             // Font name
);


int editorFontWidth = 0;

std::string selectedFilePath = ""; //will be used by file explorer to find active file

// int fileContentSaved = 0; //will be used to save content in file -> this req because using thread
std::atomic<bool> fileContentSaved = false;

// Logger logger("log_file.txt"); //bug put dont declare file name here ->hardcoded
// Logger logger("logger_file.txt"); //bug put dont declare file name here ->hardcoded
Logger logger(".thunder/logs/logs.txt"); //bug put dont declare file name here ->hardcoded


void SetCaretPosition2(int x,int y){
    //x,y are caret position
    int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);
    int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
    int topLine = scrolledY/EDITOR_LINE_HEIGHT;
    // int l = scrolledX/editorFontWidth;
    int line = y-topLine;
    // int lx = x-l;
    int lx = x-(int)(scrolledX/editorFontWidth);
    SetCaretPos(leftX + lx*editorFontWidth,line*EDITOR_LINE_HEIGHT+topY);
}


void WriteFileContent(std::string path){
    //will write content of file in editor
    std::wifstream inpfile(path);
    // inpfile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // if UTF-8 file

    if (inpfile.is_open()){
        std::wstring line;
        editContent.clear();
        scrolledY = 0;
        scrolledX = 0;
        cursorX = 0;
        cursorY = 0;
        maxLineWidthValue = 0;
        while (std::getline(inpfile,line)){
            editContent.push_back(line);
        }
        SetCaretPosition2(cursorX,cursorY);
    }
    inpfile.close();

}


void save_file(std::string file_path,std::vector<std::wstring> content){
    //will save file with given content;
    if (fileContentSaved) return ; //a thread is already running
    try{
        std::thread([=]() {
            std::ofstream file;
            file.open(file_path);
            fileContentSaved = 1;

            for (int i = 0;i<content.size();i++){
                std::string line = wstring_string(content[i]);
                file << line << "\n";
            }
            file.close();
            fileContentSaved = 0;
        }).detach(); //run parallel
    }
    catch(...) {
        fileContentSaved = 0;
        return;
    }

}




#endif