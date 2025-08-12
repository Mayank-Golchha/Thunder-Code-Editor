//TODO : sort items by name
//TODO : need to change selected index if sorted
//TODO : dont have index in visible items because folder may be collapsed and its file may be active

#ifndef FILE_EXPLORER_H
#define FILE_EXPLORER_H

#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <gdiplus.h>
#include <fstream>


#include "string_operations.h"
#include "dimensions.h"
#include "colors.h"
#include "Animation_library/animation.h"
#include "InputWindow.h"
#include "window_id.h"
#include "window_handles.h"
#include "context_menu.h"
#include "FileTab.h"
#include "editor_resources.h"
#include "config_handler.h"

namespace fs = std::filesystem;


struct Explorer_Items{
    //these are rectangle showing name of file|folder and path
    // RECT rect; //this is the dimensions of this item
    int x;
    std::string name; //file or folder name
    std::string path; //path
    bool isFolder; //whether it is a folder
    //using shared_ptr so memory mangement is easy
    std::vector<std::shared_ptr<Explorer_Items>> folder_items; // if folder then its sub items    -> it will only contain items if it need to be displayed
    int isExpanded; //if it is a directory
};

class FileExplorer{
    public:
        std::shared_ptr<Explorer_Items> project; // the main folder|file or the top folder|file
        std::shared_ptr<Explorer_Items> selected_project; // the current folder|file ->it is the selected one
        std::vector<std::shared_ptr<Explorer_Items>> visible_item; // the file and folders that will be visible as it will be requred to know in which item user clicked
        std::string project_path;
        int selected_index;
        int scrolled;
        HWND hExplorer,hparentWindow;
        int hover_index = -1;
        // HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

        
        HFONT font = CreateFont(-FILE_TAB_FONT_SIZE, 0, 0, 0,560,FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN,L"Segoe UI");

        FileExplorer(std::string project_path){
            scrolled = 0;
            project = std::make_shared<Explorer_Items>();
            selected_index = 0;
            // project->isExpanded = 0;
            if (project_path != ""){
                if (!fs::exists(project_path)) {
                    logger.warning_log("Invalid Project Path");
                    return;
                } 

                set_explorer_items_struct(project,project_path,0);
                visible_item.push_back(project);
            }
            selected_project = project;
            // expand_directory(project);
        }

        void open_project(std::string project_path){
            project = std::make_shared<Explorer_Items>();
            selected_index = 0;
            set_explorer_items_struct(project,project_path,0);
            visible_item.push_back(project);
            selected_project = project;
            selected_index = 0;
        }

        void push_item(std::string name,std::string path,bool isFolder){
            //will push item in selected slected folder
            std::shared_ptr<Explorer_Items> item(new Explorer_Items);
            item->path = path;
            item->name = name;
            item->isExpanded = 0;
            item->isFolder = isFolder;
            item->x = selected_project->x + 5;
            selected_project->folder_items.push_back(item);
            // visible_item.insert(visible_item.begin()+selected_index+1,item);
            visible_item.clear();
            sort_directory(selected_project);
            set_visible_items(project);
        }


        void draw_item(HDC hdc,std::shared_ptr<Explorer_Items>&item,COLORREF bkcolor,COLORREF txtcolor,int width,int y){
            HBRUSH bg = CreateSolidBrush(bkcolor);
            RECT r = {item->x,y,item->x+width,y + FILE_EXPLORER_ITEM_HEIGHT};
            int x = r.left;
            r.left = 0;
            FillRect(hdc, &r, bg);
            SetBkMode(hdc, TRANSPARENT);  // background drawing
            // SetBkColor(hdc, txtcolor); // blue highlight
            SetTextColor(hdc,txtcolor);
            r.left = x;
            std::wstring wstr = string_to_wstring(item->name);
            // TextOut(hdc, r.left + 5 , r.top+2 ,wstr.c_str(), wcslen(wstr.c_str()));
            TextOut(hdc, r.left + 5 , r.top+3 ,wstr.c_str(), wcslen(wstr.c_str()));

            // visible_item.push_back(item); //push in visible items

            DeleteObject(bg);
        }

        void draw_explorer(HDC hdc,RECT explorer_rect,COLORREF bkcolor){
            //explorer_rect -> dimensions of the explorer
            //topline from where drawing will start
            HBRUSH bg = CreateSolidBrush(bkcolor);
            SelectObject(hdc,font);

            // try{
                // if (selectedFilePath != "" && selected_project->path != selectedFilePath){
                    // setFileTabSelectedItem(project); 
                    // int id_ = getSelectedProjectIndex(selectedFilePath);
                    // std::cout << "index : " << id_ << "\n";
                    // if (id_ >= 0) selected_index = id_;
                // }
                
            // }
            // catch (...){
                // std::cout << "error "; 
            // }

            int y = -scrolled;
            FillRect(hdc,&explorer_rect,bg);
            for (int i = 0;i<visible_item.size();i++){
                COLORREF color = colorTheme.fileExplorerItemBk;
                if (selected_index == i) draw_item(hdc,visible_item[i],colorTheme.fileExplorerItemSelectedBk,colorTheme.fileExplorerItemSelectedTc,explorer_rect.right,y);
                else if (hover_index == i){
                    draw_item(hdc,visible_item[i],colorTheme.fileExplorerHoverItemBk,colorTheme.fileExplorerHoverItemTc,explorer_rect.right,y);
                }
                else draw_item(hdc,visible_item[i],color,colorTheme.fileExplorerItemTc,explorer_rect.right,y);
                y+=FILE_EXPLORER_ITEM_HEIGHT;
            }
            DeleteObject(bg);
        }

        void sort_directory(std::shared_ptr<Explorer_Items>& folder){
            //will sort directory by putting folders at top i.e before files
            int i = -1,j = 0;
            for (j;j<folder->folder_items.size();j++){
                if (folder->folder_items[j]->isFolder){
                    i++;
                    std::swap(folder->folder_items[i],folder->folder_items[j]);
                    // if (selected_index == j) selected_index = i;
                }
            }
        }

        int getIndex(std::string file_name){
            //will get index in visible item
            //problem because name can be same
            for (int i = 0;i<visible_item.size();i++){
                if (visible_item[i]->name == file_name) return i;
            }
            return -1;
        }

        bool isParent(std::string parent_path,std::string file_path){
            //will check if files parent is parent path
            //eg :: c:/users/user -> parent path and c:/users/user/hello/world/hi.txt ->file path --> will return true
            if (file_path == parent_path) return true;
            try{
                std::filesystem::path parent = std::filesystem::canonical(parent_path);
                std::filesystem::path child = std::filesystem::canonical(file_path);
                while (child.has_parent_path()) {
                    child = child.parent_path();
                    if (child == parent)
                        return true;
                }
            }
            catch(...){
                return false;
            }
            return false;
        }

        int getSelectedProjectIndex(std::string path){
            //will get index in visible item if folder is collapsed and a file inside it is selected the folder will be the index
            for (int i = 0;i<visible_item.size();i++){
                if (visible_item[i]->path == path || (!visible_item[i]->isExpanded && isParent(visible_item[i]->path,path))) return i;
            }
            return -1;
        }

        void set_explorer_items_struct(std::shared_ptr<Explorer_Items>& folder,std::string path,int x){
            //will set value of explorer item struct
            //will not set folder->items
            fs::path filePath(path);
            // fs::item filePat
            folder->path = path;
            folder->isExpanded = 0;
            folder->name = filePath.filename().string();
            folder->isFolder = fs::is_directory(filePath);
            folder->x = x;
        }

        int explore(std::shared_ptr<Explorer_Items>& folder){
            //will push sub directories and files in item
            if (!fs::exists(folder->path) || !folder->isFolder) {
                logger.warning_log("Invalid Project Path");
                return 0; //either it is a file or path dont exist 
            } 

            int count = 0;
            for (const auto& entry : fs::directory_iterator(folder->path)) {
                std::shared_ptr<Explorer_Items> item(new Explorer_Items);
                item->name = entry.path().filename().string();
                item->path = entry.path().string();
                item->x = folder->x;
                item->isExpanded = 0;
                // item->rect = {folder->rect.left,folder->rect.bottom + count*20,folder->rect.left+100,folder->rect.bottom + (count+1)*20};
                count++;
                item->isFolder = false;
                if (fs::is_directory(entry.status())) {
                    //folder
                    item->isFolder = true;
                }
                //will draw little right from parent
                item->x = folder->x + 5;
                folder->folder_items.push_back(item);            
            }
            if (folder->isFolder) sort_directory(folder);
            return count;
        }

        void setFileTabSelectedItem(std::shared_ptr<Explorer_Items>& folder){
            //will get current file name and check if explorers current file is same else will change explorers selected folder
            for (int i = 0;i<folder->folder_items.size();i++){
                if (folder->folder_items[i]->path == selectedFilePath){
                    selected_project = folder->folder_items[i];
                    return;
                }
                setFileTabSelectedItem(folder->folder_items[i]);
            }
        }

        int set_current_item(POINT p){
            //will check which rectangle is clicked and will set it current
            //p is the corrdinates of the point mouse clicked
            int y = -scrolled;
            for (int i = 0;i<visible_item.size();i++){
                RECT r = {0,y,FILE_EXPLORER_WIDTH,y+FILE_EXPLORER_ITEM_HEIGHT};
                y += FILE_EXPLORER_ITEM_HEIGHT;
                if (PtInRect(&r,p)){
                    selected_index = i;
                    selected_project = visible_item[i];
                    if (!selected_project->isFolder){
                        // checking if a tab already exists
                        int found = -1;
                        for (int i = 0;i<fileTabContainer.file_tabs.size();i++){
                            if (fileTabContainer.file_tabs[i]->FilePath == selected_project->path) {
                                found = i;
                                break;
                            }
                        }
                        if (found < 0){
                            struct FileTab* newItem = new FileTab{ NULL,selected_project->name,selected_project->path,NULL,0 ,-1,-1};
                            fileTabContainer.file_tabs.push_back(newItem);
                            fileTabContainer.selectedIndex = fileTabContainer.file_tabs.size()-1;
                            WriteFileContent(newItem->FilePath);
                        }
                        else{
                            fileTabContainer.selectedIndex = found;
                            WriteFileContent(selected_project->path);
                        }
                        
                        // load_plugin_safe()
                        InvalidateRect(hFileTabContainer,NULL,TRUE);

                        SendMessage(hEditor,LOAD_EDITOR_PLUGIN,0,0);
                        InvalidateRect(hEditor,NULL,TRUE);
                    }

                    return 1;
                }
            }
            //no update happened
            //selected index and selectected project remains old one only

            // selected_index = 0;
            // selected_project = NULL;
            return 0;
        }

        void set_visible_items(std::shared_ptr<Explorer_Items>&folder){
            //it will scan the project folder and make visible item list again
            //make sure to call visible_items.clear() before this function
            if (!folder->isFolder || !folder->folder_items.size()) {
                visible_item.push_back(folder);
                return;
            }
            visible_item.push_back(folder);
            for (int i = 0;i<folder->folder_items.size();i++){
                set_visible_items(folder->folder_items[i]);
            }
        }

        void clear_folder(std::shared_ptr<Explorer_Items>&folder){
            //will clear memory of all items of folder
            if (!folder->isFolder || !folder->folder_items.size()) return;
            for (int i = 0;i<folder->folder_items.size();i++){
                set_visible_items(folder->folder_items[i]);
            }
            folder->folder_items.clear();
        }

        void collapse_director_items(std::shared_ptr<Explorer_Items>&folder){
            //will remove items of a folder
            //will not delete files/folder
            int size = folder->folder_items.size();
            // folder->folder_items.clear();
            clear_folder(folder);
            visible_item.clear();
            set_visible_items(project);
        }

        void expand_directory(std::shared_ptr<Explorer_Items>&folder){
            //it will expand the folder and put that in visible array
            //it is assumed the folder path provided is correct
            if (!folder->isFolder) return;
            int size = explore(folder);
            if (size){
                // visible_item.resize(visible_item.size()+size);
                visible_item.insert(visible_item.begin()+selected_index+1,folder->folder_items.begin(),folder->folder_items.end());
            }
        }

        void update_path(std::shared_ptr<Explorer_Items>&folder,std::filesystem::path p){
            //will update path of every item of folder again
            for (int i = 0;i<folder->folder_items.size();i++){
                // folder->folder_items[i]->path = p.parent_path().string();
                std::filesystem::path temp = p / folder->folder_items[i]->name;
                folder->folder_items[i]->path = temp.string();
                update_path(folder->folder_items[i],temp);
            }
        }

        void reset(){
            clear_folder(project);
            project = NULL;
            selected_project = NULL;
            scrolled = 0;
            selected_index = -1;
            visible_item.clear();
        }
};

// FileExplorer explorer("E:/Text Editor - Copy");
// std::vector<std::string> readOpenedFoldersPath(){
//     // will read all the projects opened recently
//     std::vector<std::string> projects;
//     std::string line;
//     std::string file_path = ".thunder/config/recent.txt";
//     std::ifstream file(file_path);

//     if (!file) logger.warning_log("Unable to open recent.txt file");

//     while (std::getline(file,line)){
//         projects.push_back(line);
//     }
//     if (!projects.size()) projects.push_back("");
    
//     file.close();
    
//     return projects;
// }

// void appendInRecentFolders(std::string path){
//     // append the recent opened folder in the recent.txt
//     std::string file_path = ".thunder/config/recent.txt";
//     std::ofstream file(file_path,std::ios::app);
//     file << path << "\n";
//     file.close();
// }

// void saveOpenedFoldersPath(std::vector<std::string>& recent){
//     // will write the vector in the recent.txt
//     std::string file_path = ".thunder/config/recent.txt";
//     std::ofstream file(file_path);

//     if (!file) logger.warning_log("Unable to open recent.txt file");

//     for (int i = 0;i<recent.size();i++){
//         file << recent[i] << "\n";
//     }

//     file.close();
// }

// FileExplorer explorer("");
FileExplorer explorer(ConfigHandler::readOpenedFoldersPath().back());
// Animation lightning(L"E:/Text Editor/Animation_library/resources/lightning");
Animation lightning(L"Animation_library/resources/lightning");

LRESULT CALLBACK FileExplorerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg){
        case WM_CREATE:
        {
            lightning.load_images();
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r;
            GetClientRect(hWnd, &r);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, r.right, r.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            explorer.draw_explorer(memDC,r,colorTheme.fileExplorerBk);
            
            // Gdiplus::Image newfoldericon(L"C:\\Users\\Mayank\\Downloads\\new-folder.png");
            if (explorer.visible_item.size()){
                Gdiplus::Graphics g(memDC);
                Gdiplus::Image newfoldericon(L"Images\\new-folder.png");
                Gdiplus::Image newfileicon(L"Images\\new-file.png");
                g.DrawImage(&newfoldericon, r.right - NEW_FOLDER_ICON, 3); //new folder icon
                g.DrawImage(&newfileicon, r.right - NEW_FILE_ICON, 3); //new file icon
            }

            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);

            if (lightning.showAnimation && !lightning.isAnimationOver){
                lightning.show(memDC,p.x-20,p.y-70); //-20,-70 from coor because each animation may have different size and coor can be different from mouse coor
            }

            BitBlt(hdc, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);
           
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);

            RECT r;
            GetClientRect(hWnd,&r);

            //timer will be called every 25ms
            SetTimer(hWnd,1,ANIMATION_FRAME_TIME,NULL); //will start timer for animation
            // lightning.showAnimation = 1;
            lightning.init();
            

            if (p.x >= r.right-NEW_FILE_ICON && p.y <= FILE_EXPLORER_ITEM_HEIGHT){
                if (p.x >= r.right-NEW_FILE_ICON && p.x < r.right-NEW_FOLDER_ICON){
                    //new file icon clicked
                    HWND nw = createInputWindow(INPUT_WINDOW_X,INPUT_WINDOW_y,INPUT_WINDOW_WIDTH,INPUT_WINDOW_HEIGHT,hEditor,CREATE_NEW_FILE);
                    SetFocus(nw);
                }
                else{
                    //new folder icon clicked
                    HWND nw = createInputWindow(INPUT_WINDOW_X,INPUT_WINDOW_y,INPUT_WINDOW_WIDTH,INPUT_WINDOW_HEIGHT,hEditor,CREATE_NEW_FOLDER);
                    SetFocus(nw);

                }
                return 0;
            }
            explorer.set_current_item(p);

            if (explorer.selected_index >= 0){
                //for first time when you click anywhere inside explorer it will open whole stuff since selected index is 0 
                explorer.selected_project->isExpanded ^= 1;
                if (!explorer.selected_project->isFolder){
                    selectedFilePath = explorer.selected_project->path;
                }
                if (explorer.selected_project->isExpanded){
                    //not selected expand it
                    explorer.expand_directory(explorer.selected_project);
                    InvalidateRect(hWnd,NULL,TRUE);
                }
                else{
                    //its already selected collapse it
                    explorer.collapse_director_items(explorer.selected_project);
                    InvalidateRect(hWnd,NULL,TRUE);
                }
            }
            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            POINT p;
            GetCursorPos(&p);
            std::vector<context_map*> data;
            if (explorer.selected_project->isFolder){
                context_map* new_file_ = new context_map{L"New File",C_M_CREATE_NEW_FILE};
                context_map* new_folder_ = new context_map{L"New Folder",C_M_CREATE_NEW_FILE};
                context_map* rename_folder_ = new context_map{L"Rename Folder",C_M_RENAME_FOLDER};
                context_map* copy_path_ = new context_map{L"Copy Path",C_M_COPY_PATH};
                data = {new_file_,new_folder_,rename_folder_,copy_path_};
            }
            else{
                context_map* new_file_ = new context_map{L"New File",C_M_CREATE_NEW_FILE};
                context_map* rename_file_ = new context_map{L"Rename File",C_M_RENAME_FILE};
                context_map* copy_path_ = new context_map{L"Copy Path",C_M_COPY_PATH};
                
                data = {new_file_,rename_file_,copy_path_};
            }
            // ScreenToClient(hmainWindow,&p);
            
            if (IsWindow(hContextMenuWindow)) DestroyWindow(hContextMenuWindow); 
            hContextMenuWindow = createContextMenuWindow(p.x,p.y,hWnd,hmainWindow,data);
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            int y = -explorer.scrolled;
            for (int i = 0;i<explorer.visible_item.size();i++){
                RECT r = {0,y,FILE_EXPLORER_WIDTH,y+FILE_EXPLORER_ITEM_HEIGHT};
                y += FILE_EXPLORER_ITEM_HEIGHT;
                if (PtInRect(&r,p)) {
                    explorer.hover_index = i;
                    InvalidateRect(hWnd,NULL,TRUE);
                    break;
                }
            }

            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            explorer.hover_index = -1;
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        }

        case WM_COMMAND:
        {
            switch (wp)
            {
                case C_M_CREATE_NEW_FILE:
                {
                    HWND nw = createInputWindow(INPUT_WINDOW_X,INPUT_WINDOW_y,INPUT_WINDOW_WIDTH,INPUT_WINDOW_HEIGHT,hEditor,CREATE_NEW_FILE);
                    SetFocus(nw);
                    return 0;
                }

                case C_M_CREATE_NEW_FOLDER:
                {
                    HWND nw = createInputWindow(INPUT_WINDOW_X,INPUT_WINDOW_y,INPUT_WINDOW_WIDTH,INPUT_WINDOW_HEIGHT,hEditor,CREATE_NEW_FOLDER);
                    SetFocus(nw);
                    return 0;
                }

                case C_M_RENAME_FILE:
                {
                    HWND nw = createInputWindow(INPUT_WINDOW_X,INPUT_WINDOW_y,INPUT_WINDOW_WIDTH,INPUT_WINDOW_HEIGHT,hEditor,RENAME_FILE);
                    SetFocus(nw);
                    return 0;
                }

                case C_M_RENAME_FOLDER:
                {
                    HWND nw = createInputWindow(INPUT_WINDOW_X,INPUT_WINDOW_y,INPUT_WINDOW_WIDTH,INPUT_WINDOW_HEIGHT,hEditor,RENAME_FOLDER);
                    SetFocus(nw);
                    return 0;
                }
            }
            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            RECT r;
            GetClientRect(hWnd,&r);
            int delta = GET_WHEEL_DELTA_WPARAM(wp); // typically 120 or -120
            float scrollAmount = delta * 0.5f; // You can tune this multiplier

            explorer.scrolled -= scrollAmount;

            if (explorer.scrolled < 0) explorer.scrolled = 0;
            
            int m = explorer.visible_item.size()*FILE_EXPLORER_ITEM_HEIGHT-r.bottom;
            int maxScroll = (0>m)?0:(m);
            if (explorer.scrolled > maxScroll)
                explorer.scrolled = maxScroll;

            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }

        case WM_TIMER:
        {
            lightning.update();
            if (lightning.isAnimationOver || !lightning.showAnimation){
                KillTimer(hWnd,1);
                InvalidateRect(hWnd,NULL,TRUE);
            }
            else InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        }

        case INPUT_WINDOW_MESSAGE:
        {
            if (lp == CREATE_NEW_FILE){
                const wchar_t* file_name = reinterpret_cast<const wchar_t*>(wp);
                std::filesystem::path file_path(explorer.selected_project->path);
                if (std::filesystem::is_directory(file_path)){
                    //selected project is a folder
                    file_path /= file_name; 
                    std::filesystem::create_directories(file_path.parent_path()); //add directories based on the object path (without this line it will not work)
                    std::ofstream new_file(file_path);
                    explorer.push_item(file_path.filename().string(),file_path.string(),0);
                    new_file.close();
                }
                else{
                    //selected project is a file
                    file_path = file_path.parent_path();
                    file_path /= file_name;
                    std::filesystem::create_directories(file_path.parent_path()); //add directories based on the object path (without this line it will not work)
                    std::ofstream new_file(file_path);
                    explorer.push_item(file_path.filename().string(),file_path.string(),0);
                    new_file.close();
                }
                InvalidateRect(hWnd,NULL,TRUE);
            }
            else if (lp == CREATE_NEW_FOLDER){
                //selected project is a folder
                const wchar_t* folder_name = reinterpret_cast<const wchar_t*>(wp); 
                std::filesystem::path file_path(explorer.selected_project->path);
                if (std::filesystem::is_directory(file_path)){
                    //selected project is a folder
                    file_path /= folder_name; 
                    std::filesystem::create_directories(file_path); //add directories based on the object path (without this line it will not work)
                    explorer.push_item(file_path.filename().string(),file_path.string(),1);
                }
                else{
                    //selected project is a file
                    file_path = file_path.parent_path();
                    file_path /= folder_name;
                    std::filesystem::create_directories(file_path); //add directories based on the object path (without this line it will not work)
                    explorer.push_item(file_path.filename().string(),file_path.string(),1);
                }

                // std::filesystem::create_directories(folder_name);
                InvalidateRect(hWnd,NULL,TRUE);
            }
            else if (lp == RENAME_FILE){
                const wchar_t* file_name = reinterpret_cast<const wchar_t*>(wp); 
                std::filesystem::path old_path(explorer.selected_project->path);
                std::filesystem::path new_path = old_path.parent_path() / file_name;
                if (std::filesystem::exists(new_path)) return 0;

                std::filesystem::rename(old_path,new_path);
                explorer.selected_project->name = new_path.filename().string();
                explorer.selected_project->path = new_path.string();
                InvalidateRect(hWnd,NULL,TRUE);
            }

            else if (lp == RENAME_FOLDER){
                const wchar_t* folder_name = reinterpret_cast<const wchar_t*>(wp); 
                std::filesystem::path old_path(explorer.selected_project->path);
                std::filesystem::path new_path = old_path.parent_path() / folder_name;
                if (std::filesystem::exists(new_path)) return 0;

                std::filesystem::rename(old_path,new_path);
                explorer.selected_project->name = new_path.filename().string();
                explorer.selected_project->path = new_path.string();
                //updating all sub items path
                explorer.update_path(explorer.selected_project,new_path);
                InvalidateRect(hWnd,NULL,TRUE);

            }
            return 0;
        }
        
        case WM_ERASEBKGND:
            return 1;
    }
    return DefWindowProc(hWnd, msg, wp, lp);
}

HWND createFileExplorer(HWND hparent,int x,int y,int width,int height){
    HWND w = CreateWindowEx(0, L"FileExplorerWindowClass", L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER , x, y, width,height,
        hparent, NULL, NULL, NULL);
    return w;
}

#endif