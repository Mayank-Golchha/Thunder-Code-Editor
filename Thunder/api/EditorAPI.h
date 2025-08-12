// contains api of editor
#ifndef EDITORAPI_H
#define EDITORAPI_H

#include <functional>

// #include "../editor_resources.h"
#include "../window_handles.h"
#include "../window_id.h"
#include "../language_type.h"
#include "../hoverWindow.h" // will be used to make windows to display text
#include "../context_menu.h"


class EditorAPI{
    public:
        virtual void insertText(const std::wstring& text,int x,int y) = 0;
        
        virtual void getCaretPosition(POINT* p) = 0;

        virtual void setCaretPosition(POINT* p) = 0;

        virtual int getCurrentLineNo() = 0;

        virtual std::string getCurrentFileExtension() = 0;

        virtual void scrollParticularLine(int line_no) = 0;

        virtual std::vector<std::wstring>& getEditorContent() = 0;

        virtual std::string getCurrentFilePath() = 0;

        virtual std::string wstringToString(std::wstring text) = 0;

        virtual void updateEditorContent() = 0;

        virtual std::wstring stringToWstring(std::string text) = 0;

        virtual int info_log(std::string message) = 0;

        virtual int warning_log(std::string message) = 0;

        virtual int error_log(std::string message) = 0;

        virtual int isTextSelected() = 0;

        virtual void selectEditorText(int startX,int startY,int endX,int endY) = 0;

        virtual void saveFile(std::string file_path,std::vector<std::wstring>& content) = 0;

        virtual int getFontWidth() = 0;

        virtual void GetAppDimensions(RECT* r) = 0;

        virtual void GetEditorDimensions(RECT* r) = 0;

        virtual void GetWritingPartCoor(POINT* p) = 0;

        virtual int getLineHeight() = 0;


        virtual void MouseToCursor(POINT* p) = 0;

        virtual void CursorToAppPosition(POINT* p) = 0;

        virtual void InsertEditorContextMenuItem(std::vector<context_map*>* items,context_map* data) = 0;
        
        virtual void setEditorTheme(std::string file_path) = 0;

        virtual void RemoveMiniCodeViewer() = 0;

        virtual void SetMiniCodeViewer() = 0;


        // to register function inside that are called inside backend
        virtual void registerCharTypedCallback(std::function<void(wchar_t)> cb) = 0;

        virtual void registerOnMouseMoveCallback(std::function<void(POINT*)> cb) = 0;

        virtual void registerOnEditorContextMenuCreate(std::function<void(std::vector<context_map*>*)> cb) = 0;



        virtual ~EditorAPI() {}
};

#endif