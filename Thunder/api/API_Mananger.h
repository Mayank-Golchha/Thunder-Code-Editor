// TODO : have a unordered map of context menu items value will be the function which will be performed

#ifndef API_MANAGER_H
#define API_MANAGER_H

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "../editor_resources.h"
#include "../window_handles.h"
#include "../string_operations.h"
#include "../colors.h"
#include "EditorAPI.h"

namespace ExtensionApi {

    // will be called before a character is written or deleted in the text buffer
    // if it is backspace then also check if text is selected or not
    std::vector<std::function<void(wchar_t)>> charTypedCallbacks; 
    
    // will contain all function that will be triggered when mouse is moving
    std::vector<std::function<void(POINT*)>> mouseMoveCallbacks; 
    
    // when a item in editors context menu is clicked it will call all function
    // wparam contains the id of the item clicked and lp is the item no which is click
    // the id you pass the create a new item in this context menu will be the wp
    std::vector<std::function<void(WPARAM,LPARAM)>> onEditorContextMenuItemClick; 
        
    std::vector<std::function<void(std::vector<context_map*>*)>> editorContextMenuCallbacks;


    void callCharTypedCallbacks(wchar_t ch) {
        // this function will call all function inside the vector
        // will be called before input text got register in the input buffer
        // this function will be called by backend 
        for (auto& cb : charTypedCallbacks) cb(ch);
    }

    void callEditorContextMenuCallbacks(std::vector<context_map*>* items){
        // this function will call all function inside the vector
        // will be called when inserting creating context menu window for editor
        // this function will be called by backend 
        for (auto& cb : editorContextMenuCallbacks) cb(items);
    }

    void callMouseMoveInEditorCallbacks(POINT* p){
        // this function will call all function inside the vector
        // this function will be called on mouse move inside the editor(writing part)
        // this function will be called by backend 
        for (auto& cb : mouseMoveCallbacks) cb(p);
    }

    void callOnEditorContextMenuItemClick(WPARAM wp,LPARAM lp){
        // when a item in editors context menu is clicked it will call all function
        // wparam contains the id of the item clicked and lp is the item no which is click
        // the id you pass to create a new item in this context menu will be the wp
        for (auto& cb : onEditorContextMenuItemClick) cb(wp,lp);
    }


    class EditorApiFunction : public EditorAPI{
        public:
            void insertText(const std::wstring& text,int x,int y) override{
                // to insert text at given x,y position
                // y is the line no
                // this function donot change carets position
                // u need to call updateEditorContent() to update the screen after inserting
                // will only insert if given x is less than lines size
                if (x <= editContent[y].size()){
                    editContent[y].insert(x,text);
                }
            }

            void InsertEditorContextMenuItem(std::vector<context_map*>* items,context_map* data) override{
                // will insert item in the editors context menu
                // context menu is the window which gets displayed when right mouse button is clicked
                items->push_back(data);
            }

            context_map* newContextMapItem(std::wstring name,int id){
                // id should be greater than WM_USER + 100
                // eg: WM_USER + 110 or WM_USER + 150
                context_map* c = new context_map{name,id};
                return c;
            }
    
            void getCaretPosition(POINT* p) override{
                // will get carrets position
                p->x = cursorX;
                p->y = cursorY;
            }

            void MouseToCursor(POINT* p) override {
                // converts mouse coordinates (cursor position) to editor coordinates
                // p contains the coor
                // p will be updated with the output
                // i.e line no and x coor
                int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
                int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);

                int line = (p->y-topY)/EDITOR_LINE_HEIGHT;
                int col = (p->x-leftX)/editorFontWidth;

                int topLine = scrolledY/EDITOR_LINE_HEIGHT;
                int left = scrolledX/editorFontWidth;


                p->y = line+topLine;
                p->x = col+left;

                // p->x = line+topLine;
                // p->y = col+left;
            }

            void CursorToAppPosition(POINT* p) override {
                // convert cursorx,cursory of apps coor
                int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);
                int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
                int topLine = scrolledY/EDITOR_LINE_HEIGHT;
                int line = p->y-topLine;
                int lx = p->x-(int)(scrolledX/editorFontWidth);
                p->x = EDITOR_X +  leftX + lx*editorFontWidth;
                p->y = EDITOR_Y + line*EDITOR_LINE_HEIGHT+topY;
            }

            void setCaretPosition(POINT* p) override{
                // will set carrets position
                cursorX = p->x;
                cursorY = p->y;
                SetCaretPosition2(cursorX,cursorY);
            }

            int getCurrentLineNo() override{
                // will return the current line no
                return cursorY;
            }

            std::string getCurrentFileExtension() override{
                // will return the current files extension
                std::string file_name = selectedFilePath;
                int pos = -1;
                std::string type = "";
                for (int i = file_name.size()-1;i>=0;i--) if (file_name[i] == '.'){pos = i;break;}
                if (pos >= 0) {
                    type = file_name.substr(pos + 1);
                    return type;
                }
                return type;
            }

            void scrollParticularLine(int line_no) override{
                // to scroll at a particular line in the editor
                if (cursorY != editContent.size()){
                    int topLine = scrolledY/EDITOR_LINE_HEIGHT;
                    int k = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH;

                    if (k > 0 && line_no*EDITOR_LINE_HEIGHT < k + ADDITIONAL_SCROLL_LINES){
                        scrolledY = line_no*EDITOR_LINE_HEIGHT;
                        cursorX = 0;
                        cursorY = line_no;
                        SetCaretPosition2(cursorX,cursorY);
                    }
                    else if(k > 0){
                        scrolledY = k + ADDITIONAL_SCROLL_LINES;
                    }
                    return ;
                }
                return ;
            }

            std::vector<std::wstring>& getEditorContent() override{
                // will return editors content
                // all lines are stored in vector
                return editContent;
            }

            void saveFile(std::string file_path,std::vector<std::wstring>& content) override{
                // will save the content in the given file
                save_file(file_path,content);
            }

            std::string getCurrentFilePath() override{
                // to get the path of the current file
                return selectedFilePath;
            }

            std::string wstringToString(std::wstring text) override{
                // converts wstring to string
                return wstring_string(text);
            }

            std::wstring stringToWstring(std::string text) override{
                // converts string to wstring
                return string_to_wstring(text);
            }

            void updateEditorContent() override {
                // call this whenever u make changes in the content
                // this will update the content displayed
                InvalidateRect(hEditor,NULL,TRUE);
                SendMessage(hEditor,LOAD_EDITOR_PLUGIN,0,0);
            }

            int info_log(std::string message) override {
                // for logging information
                return logger.info_log(message);
            }

            int warning_log(std::string message) override {
                // for logging warning
                return logger.warning_log(message);
            }

            int error_log(std::string message) override {
                // for logging error
                return logger.error_log(message);
            }

            int isTextSelected() override{
                // will return if text is selected or not
                return isWordSelected;
            }

            int getFontWidth() override{
                // will return the current fonts character width
                HDC hdc = GetDC(hEditor);
                SIZE sz;
                SelectObject(hdc,editorFont);
                GetTextExtentPoint32(hdc, L"a", 1, &sz); //choosed "a" because every character will have same width
                ReleaseDC(hEditor,hdc);
                return sz.cx;
            }

            int getLineHeight() override{
                // will return the height of the line in editor
                return EDITOR_LINE_HEIGHT;
            }

            void GetAppDimensions(RECT* r) override{
                // will get the apps dimension
                // r.left and r.top is 0,0 and r.right = wth width and r.bottom = height
                GetClientRect(hmainWindow,r);
            }

            void GetEditorDimensions(RECT* r) override {
                // will get the editors dimension
                // r.left and r.top is 0,0 and r.right = wth widht and r.bottom = height
                GetClientRect(hEditor,r);
            }

            void GetWritingPartCoor(POINT* p) override{
                // inside the editor at p->x and p->y the writing starts relative to app
                p->x = EDITOR_X + EDITOR_WRITING_PART_X;
                p->y = EDITOR_Y + EDITOR_WRITING_PART_Y;
            }

            void selectEditorText(int startX,int startY,int endX,int endY) override{
                // will highlight text between line starty and endy and between startx and endx like any editor do
                startSelectionX = startX;
                startSelectionY = startY;
                endSelectionX = endX;
                endSelectionY = endY;
                InvalidateRect(hEditor,NULL,TRUE);
            }

            void setEditorTheme(std::string file_path) override{
                // to change the theme of the app
                // file_path is the path of a json file which contains the theme color
                // if it fails to load then the default theme will be set
                colorTheme = loadTheme(file_path,default_theme);
                RedrawWindow(hmainWindow, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
            }

            void RemoveMiniCodeViewer() override{
                // removes the mini code viewer
                drawMiniCodeViewer = 0;
                InvalidateRect(hEditor,NULL,TRUE);
                UpdateWindow(hEditor);
            }

            void SetMiniCodeViewer() override{
                // draws the mini code viewer
                drawMiniCodeViewer = 1;
                InvalidateRect(hEditor,NULL,TRUE);
                UpdateWindow(hEditor);
            }

            // context_map* insertContextMenuItem(std::string name,int id,std){
                // name represents the name of the item
                // every item in a context menu has a unique id
            // }



            // below function are used to register function that are called by backend
            
            void registerCharTypedCallback(std::function<void(wchar_t)> cb) override {
                // this function takes functions as parameter
                // and put them in a vector
                // so all functions can be called in wm_char (when a character is typed)
                // so use this to call function that needs to be triggered when user types something
                // enter is clicked
                try{
                    charTypedCallbacks.push_back(cb);
                }
                catch (...){
                    logger.error_log("Problem in char typed call function of extension");
                }
            }

            void registerOnMouseMoveCallback(std::function<void(POINT*)> cb) override {
                // this function takes functions as parameter
                // and put them in a vector
                // so all functions can be called in on mouse move (when a character is typed)
                // so use this to call function that needs to be triggered when user moves mouse
                try{
                    mouseMoveCallbacks.push_back(cb);
                }
                catch (...){
                    logger.error_log("Problem in mouse move call function of extension");
                }
            }

            void registerOnEditorContextMenuCreate(std::function<void(std::vector<context_map*>*)> cb) override {
                // this function takes functions as parameter
                // and put them in a vector
                // so all functions can be called in on (when a context menu of editor is created)
                // so use this to call function that needs to be triggered when you want to delete a item from a context menu or insert a new item
                try{
                    editorContextMenuCallbacks.push_back(cb);
                }
                catch (...){
                    logger.error_log("Problem in onEditorContextMenuCreate function of extension");
                }
            }
    };

}

ExtensionApi::EditorApiFunction editorApi;

void LoadExtension(const wchar_t* dllPath, EditorAPI* api) {
    try{
        HMODULE plugin = LoadLibraryW(dllPath);
        if (!plugin) {
            logger.error_log(wstring_string(dllPath) + " Extension not able load");
            return;
        }

        using RegisterFunc = void (*)(EditorAPI*);
        RegisterFunc registerExtension = (RegisterFunc)GetProcAddress(plugin, "RegisterExtension"); //register plugin should be function in the extension file

        if (registerExtension) {
            registerExtension(api);
            // logger.info_log("Extension loaded");
            logger.info_log(wstring_string(dllPath) + " Extension loaded");
        }
    }
    catch(...){
        logger.error_log(wstring_string(dllPath) + " Extension not able load");
    }
}


#endif 