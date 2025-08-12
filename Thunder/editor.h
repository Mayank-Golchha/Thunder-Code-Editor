// //TODO : when you click above scrollbar check whether the thumb will go above the given height of scrollbar
// //TODO : when you click at the near end of scrollbar the thumb dont move ,handle it
// //TODO : when you click at thumb of scrollbar it goes little down add cursor position to it
// //TODO : hide horizontal scrollbar if no scrollable content
// //TODO : if no of lines is alot then the thumb height may become too small
// //TODO : if no of lines is alot then line no may not be written properly



//all calculation of scrolling are done by scrolled_X and scrolled_Y not by vertThumbY and horzThumbX
//TODO : if text is  selected and scrolling the selected text gets removed
//TODO : smoother scrolling
//TODO : backward selection of text is not happening
//TODO : the auto complete window is not created by cursor position so if horizontal or vertical scrolling happens the autocomplete window is drown in wrong place
//TODO : in autocomplete window handle upper case letters also and better match function
//TODO : in mini code viewer it diplays the content from the top line that is seen handle it and displays only one color
//TODO : free memory from plugin of tokens -> Tokens struct
//TODO : when double click the word gets selected but need to double click again to select line instead of one click
//TODO : in animation i am currently redrawing complete window but i should draw only a certain region for more efficiency
//TODO : while animation is on if u click autocomplete vanishes dont know why
//TODO : in multiple click handling only redraw a certain region for more efficency
//TODO : add scroll_down and up in wm_char
//TODO : when u have scrolled and did ctrl + a + x then u scrolled doesnot get 0
//TODO : if a style token is tooo large larger than the screen then display token will be wrong
//TODO : an optimization currently i am iterating over all the code for make mini code viewer use ony one for loop when writing characters
//TODO : editor doesnot support emojis and if encounter different encoded character then it behaves differently
//TODO : use below struct to draw in editor
// struct TextColorStructure{
//     std::wstring text;
//     int startX;
//     int startY;
//     int endX;
//     int endY;
//     COLORREF bkcolor; // background color
//     COLORREF txtcolor; // text color
// };

//TODO : handle context menu function



#include "window_handles.h"
#include "dimensions.h"
#include "colors.h"
#include "editorFunctions.h"
#include "string_operations.h"
#include "font.h"
#include "autoCompleteWindow.h"
#include "mini_code_viewer.h"
#include "run_program_file.h"

#include <iostream>
#include <thread>

// using namespace std;

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif


int Max(int a,int b){
    return (a>b)?a:b;
}

int Min(int a,int b){
    return (a<b)?a:b;
}

LRESULT CALLBACK CustomEditWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg){
        case WM_SYSKEYDOWN:
        {
            if (wp == 121){
                // shift + f10   --> to run program
                RunProgram program(getCurrentLanguage());
                program.run(selectedFilePath);
            }
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (GetKeyState(VK_CONTROL) & 0X8000){
                //ctrl key is clicked
                deleteAutoCompleteWindow(); //will delete the auto complete window
                if (wp == 'A'){
                    //ctrl+a --> will select all
                    startSelectionX = 0;
                    startSelectionY = 0;
                    endSelectionY = editContent.size()-1;
                    endSelectionX = editContent[endSelectionY].size();
                    isWordSelected = 1;
                    cursorX = endSelectionX;
                    cursorY = endSelectionY;
                    SetCaretPosition2(cursorX,cursorY);
                    InvalidateRect(hWnd,NULL,TRUE);
                }

                else if (wp == 'K'){
                    scrollVerticalParticularLine(50);
                    InvalidateRect(hWnd,NULL,TRUE);
                }

                else if (wp == 'C'){
                    //ctrl+a --> will copy
                    std::wstring copyText;
                    if (isWordSelected) copyText = copySelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
                    else copyText = editContent[cursorY] + L"\n"; //line selected
                    setClipBoardText(copyText);
                }

                else if (wp == 'V'){
                    //ctrl + v --> will paste
                    std::wstring clipboardText = GetClipboardText();
                    if (isWordSelected){
                        //may need to move screen
                        cursorX = startSelectionX;
                        cursorY = startSelectionY;
                        removeSelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
                        startSelectionX = -1;
                        startSelectionY = -1;
                        endSelectionX = -1;
                        endSelectionY = -1;
                        SetCaretPosition2(cursorX,cursorY);
                        // scrollUpLine();//may give bug
                    }
                    
                    InsertClipBoardText(&editContent,clipboardText);
                    DeselectText();
                    setClipBoardText(clipboardText);

                        // scrollUpLine();//may give bug

                    // editContent.insert(editContent.begin()+1,GetClipboardText());
                    SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);

                    InvalidateRect(hWnd,NULL,TRUE);
                }

                else if (wp == 'X'){
                    //ctrl+x --> will cut
                    if (editContent.size() == 1 && !editContent[0].size()) return 0;
                    if (isWordSelected){
                        //may need to move screen
                        cursorX = startSelectionX;
                        cursorY = startSelectionY;
                        removeSelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
                        startSelectionX = -1;
                        startSelectionY = -1;
                        endSelectionX = -1;
                        endSelectionY = -1;
                        SetCaretPosition2(cursorX,cursorY);
                        scrollUpLine();//may give bug
                        // std::thread([](){load_plugin();}).detach(); 
                        InvalidateRect(hWnd,NULL,TRUE);
                        // cursorX = startSelectionX;
                        // cursorY = startSelectionY;
                        // SetCaretPosition2(cursorX,cursorY);

                        // auto contentCopy = editContent;
                        // int cx = cursorX;
                        // int cy = cursorY;

                        // load_plugin_safe(contentCopy,cx,cy);
                        SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);

                        return 0;
                    }
                    cursorX = 0;
                    scrolledX = 0;
                    if (!cursorY && !editContent.size()) return 0;
                    else if (cursorY && cursorY == editContent.size()-1){
                        scrollUpLine();
                        setClipBoardText(editContent[cursorY] + L"\n"); //\n is required to insert a the content in editContent
                        editContent.erase(editContent.begin()+cursorY);
                        cursorY--;
                    }
                    else {
                        setClipBoardText(editContent[cursorY] + L"\n"); //\n is required to insert a the content in editContent
                        editContent.erase(editContent.begin()+cursorY);
                    }
        
                    SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);
        
                    SetCaretPosition2(cursorX,cursorY);
                    InvalidateRect(hWnd,NULL,TRUE);
                }

                else if (wp == 'S'){
                    //ctrl + s -> will save content of file
                    std::vector<std::wstring> content = editContent;
                    save_file(selectedFilePath,content);
                }

                else if (wp == 191){
                    // ctrl + /    -> for comment
                    int change = 0;
                    if (isWordSelected){
                        int all_comment = 1; //are all lines commented
                        for (int i = startSelectionY;i<=endSelectionY;i++){
                            if (!plugin.isLineCommented(editContent[i])){
                                all_comment = 0;
                                break;
                            }
                        }
                        int cc = all_comment;

                        for (int i = startSelectionY;i<=endSelectionY;i++){
                            change += plugin.comment_line(editContent[i],all_comment);
                        }
                        DeselectText();
                    }
                    else {
                        change = plugin.comment_line(editContent[cursorY]);
                        if (change){
                            InvalidateRect(hWnd,NULL,TRUE);
                            SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);
                            if (cursorY+1 != editContent.size()){
                                cursorY++;
                                cursorX = 0;
                                SetCaretPosition2(cursorX,cursorY);
                                scrollDownLine();
                            }
                            return 0;
                        }
                    }
                    if (change) {
                        InvalidateRect(hWnd,NULL,TRUE);
                        SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);
                    }

                }

                return 0;
            }

            else if ((GetKeyState(VK_SHIFT) & 0x8000)){
                //shift key is pressed
                deleteAutoCompleteWindow(); //will delete the auto complete window
                if (wp == VK_TAB){
                    //shift + tab --> will move text tab_spaces back
                    if (isWordSelected){
                        int st = countChar(editContent[startSelectionY],0,TAB_SPACES,L' ');
                        st = Min(st,TAB_SPACES);
                        editContent[startSelectionY].erase(0,st);
                        startSelectionX -= st;

                        for (int i = startSelectionY+1;i<=endSelectionY;i++){
                            st = countChar(editContent[i],0,TAB_SPACES,L' ');
                            st = Min(st,TAB_SPACES);
                            editContent[i].erase(0,st);
                        }
                        endSelectionX -= st;
                        if (cursorX + st >= 0) cursorX -= st;
                        else cursorX = 0;
                        SetCaretPosition2(cursorX,cursorY);
                        SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);
                        InvalidateRect(hWnd,NULL,TRUE);

                        return 0;

                    }
                    if (editContent[cursorY] == L"" || editContent[cursorY][0] != L' ') return 0;
                    int c = countChar(editContent[cursorY],0,TAB_SPACES,L' ');
                    c = Min(c,TAB_SPACES);
                    editContent[cursorY].erase(0,c);
                    if (cursorX + c >= 0) cursorX -= c;
                    else cursorX = 0;
                    startSelectionX -= c;
                    endSelectionX -= c;
                    SetCaretPosition2(cursorX,cursorY);

                    SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);

                    InvalidateRect(hWnd,NULL,TRUE);
                    return 0;
                }

            }

            else if (wp == VK_UP){
                //up arrow
                if (IsWindow(hautoComplete)){
                    SendMessage(hautoComplete,AUTO_COMPLETE_UP_ARROW,0,0);
                    return 0;
                }
                int c = 0;
                if (cursorY){
                    int topLine = scrolledY/EDITOR_LINE_HEIGHT;
                    cursorY--;
                    if ((cursorY - topLine)*EDITOR_LINE_HEIGHT < editorHeight*0.2){
                        if (scrolledY - EDITOR_LINE_HEIGHT >= 0){
                            scrolledY -= EDITOR_LINE_HEIGHT;
                        }
                        else{
                            scrolledY = 0;
                        }
                        c = 1;
                    }
                    if (cursorX > editContent[cursorY].size()){
                        cursorX = editContent[cursorY].size();
                        int visible = (editorWidth-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH)/editorFontWidth;
                        scrolledX = (cursorX-visible > 0)?(cursorX-visible)*editorFontWidth:0;
                        c = 1;
                    }                    
                }
                else {
                    cursorX = 0;
                    if (scrolledX){
                        scrolledX = 0;
                        c = 1;
                    }
                }
                
                if (c) InvalidateRect(hWnd,NULL,TRUE);

                SetCaretPosition2(cursorX,cursorY);
                return 0;
            }
            
            else if (wp == VK_DOWN){
                //down arrow
                if (IsWindow(hautoComplete)){
                    SendMessage(hautoComplete,AUTO_COMPLETE_DOWN_ARROW,0,0);
                    return 0;
                }
                int c = 0;//need to redraw again
                if (cursorY != editContent.size()){
                    int topLine = scrolledY/EDITOR_LINE_HEIGHT;
                    cursorY++;
                    if ((cursorY - topLine)*EDITOR_LINE_HEIGHT > editorHeight*0.7){
                        // int k = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y+SCROLL_BAR_WIDTH;
                        int k = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH;

                        //0.7 -> if we go down beyond 70% screen will also scroll down
                        // if (k > 0 && scrolledY + EDITOR_LINE_HEIGHT < editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y + ADDITIONAL_SCROLL_LINES + SCROLL_BAR_WIDTH){
                        if (k > 0 && scrolledY + EDITOR_LINE_HEIGHT < k + ADDITIONAL_SCROLL_LINES){
                            scrolledY += EDITOR_LINE_HEIGHT;
                        }
                        else if(k > 0){
                            // scrolledY = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight + EDITOR_WRITING_PART_Y + ADDITIONAL_SCROLL_LINES + SCROLL_BAR_WIDTH;
                            scrolledY = k + ADDITIONAL_SCROLL_LINES;
                        }
                        c = 1;
                    }
                    if (cursorY >= editContent.size()){
                        cursorY--;
                        return 0;
                    }

                    if (cursorX > editContent[cursorY].size()){
                        cursorX = editContent[cursorY].size();
                        int visible = (editorWidth-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH)/editorFontWidth;
                        scrolledX = (cursorX-visible > 0)?(cursorX-visible)*editorFontWidth:0;
                        c = 1;
                    }
                }
                else {
                    cursorX = 0;
                    if (scrolledX){
                        scrolledX = 0; //need to scroll
                        c = 1;
                    }
                }
                if (c) InvalidateRect(hWnd,NULL,TRUE);

                SetCaretPosition2(cursorX,cursorY);
                return 0;
            }

            else if (wp == VK_LEFT){
                //left arrow
                deleteAutoCompleteWindow(); //will delete the auto complete window

                if (cursorX) {
                    int leftX = scrolledX/editorFontWidth;
                    cursorX--;
                    if ((cursorX - leftX)*editorFontWidth < editorWidth*0.1){   
                        if (scrolledX - editorFontWidth >= 0){
                            scrolledX -= editorFontWidth;
                            InvalidateRect(hWnd,NULL,TRUE);
                        }
                        else{
                            scrolledX = 0;
                            InvalidateRect(hWnd,NULL,TRUE);
                        }
                    }

                }
                else{
                    if (cursorY){
                        int visible = (editorWidth-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH)/editorFontWidth;
                        cursorY--;
                        cursorX = editContent[cursorY].size();
                        if (editContent[cursorY].size() > visible)
                            scrolledX = (editContent[cursorY].size()-visible)*editorFontWidth + ADDITIONAL_SCROLL_WIDTH;
                        if (scrolledY > EDITOR_LINE_HEIGHT) scrolledY -= EDITOR_LINE_HEIGHT; //scroll only if needed
                        else scrolledY = 0;
                        InvalidateRect(hWnd,NULL,TRUE);

                    }
                }
                SetCaretPosition2(cursorX,cursorY);
                return 0;
            }

            else if(wp == VK_RIGHT){
                //right arrow
                deleteAutoCompleteWindow();                
                if (cursorX != editContent[cursorY].size()){
                    //cursor not at the end
                    int leftX = scrolledX/editorFontWidth;
                    cursorX++;
                    if ((cursorX-leftX)*editorFontWidth > editorWidth*0.9){
                        // if (scrolledX+editorFontWidth < maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X + ADDITIONAL_SCROLL_WIDTH){
                        if (scrolledX+editorFontWidth < maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X + ADDITIONAL_SCROLL_WIDTH-SCROLL_BAR_WIDTH){
                            scrolledX += editorFontWidth;
                            InvalidateRect(hWnd,NULL,TRUE);
                        }
                        else{
                            // scrolledX = maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X  + ADDITIONAL_SCROLL_WIDTH;
                            scrolledX = maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X  + ADDITIONAL_SCROLL_WIDTH-SCROLL_BAR_WIDTH;
                            InvalidateRect(hWnd,NULL,TRUE);
                        }
                    }
                }
                else{
                    //cursor at the end
                    // if (cursorY != editContent.size()){
                    if (cursorY+1 != editContent.size()){
                        int topLine = scrolledY/EDITOR_LINE_HEIGHT;
                        cursorY++;
                        cursorX = 0;
                        scrolledX = 0;
                        if ((cursorY - topLine)*EDITOR_LINE_HEIGHT > editorHeight*0.7) scrolledY += EDITOR_LINE_HEIGHT;
                        InvalidateRect(hWnd,NULL,TRUE);

                    }
                }
                SetCaretPosition2(cursorX,cursorY);

                return 0;
            }

            else if (wp == VK_RETURN){
                //enter key
                if (IsWindow(hautoComplete)){
                    SendMessage(hautoComplete,AUTO_COMPLETE_ENTER,0,0);
                    return 0;
                }
                if (clearSelection(hWnd)) return 0; //text is selected and will be removed
                std::wstring wsubstr = L"";
                    
                if (cursorX != editContent[cursorY].size()){
                    //will put the content after cursor to next line
                    //clear
                    wsubstr = editContent[cursorY].substr(cursorX);
                    // wsubstr = SUBSTR(editContent[cursorY],cursorX,editContent[cursorY].size(),__FILE__,__LINE__);
                    
                }
                //clear
                editContent[cursorY] = editContent[cursorY].substr(0, cursorX);
                // editContent[cursorY] = SUBSTR(editContent[cursorY],0, cursorX,__FILE__,__LINE__);
                cursorY++;
                editContent.insert(editContent.begin() + cursorY,wsubstr);

                cursorX = 0;
                scrolledX = 0;

                scrollDownLine();
                SetCaretPosition2(cursorX,cursorY);
            
                // load_plugin_safe(contentCopy, cx, cy);
                SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);

                InvalidateRect(hWnd,NULL,TRUE);
                return 0;

            }

            else if (wp == VK_BACK){
                //backspace key
                deleteAutoCompleteWindow();                
    
                ExtensionApi::callCharTypedCallbacks((wchar_t)wp); 

                if (isWordSelected){
                    //may need to move screen
                    cursorX = startSelectionX;
                    cursorY = startSelectionY;
                    removeSelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
                    startSelectionX = -1;
                    startSelectionY = -1;
                    endSelectionX = -1;
                    endSelectionY = -1;
                    SetCaretPosition2(cursorX,cursorY);
                    scrollUpLine();//may give bug
                    SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);
                    InvalidateRect(hWnd,NULL,TRUE);
                    return 0;
                }

                if (cursorX){
                    cursorX--;
                    removeAtIndex(editContent[cursorY],cursorX);
                    scrollLeftLine(); //will move left if if below some limit
                    InvalidateRect(hWnd,NULL,TRUE);
                    SetCaretPosition2(cursorX,cursorY);
                }

                else{
                    if (cursorY){
                        cursorY--;                                
                        std::wstring c = editContent[cursorY+1];
                        int w = editContent[cursorY].size()*editorFontWidth;
                        cursorX = editContent[cursorY].size();
                        editContent[cursorY] += c;
                        editContent.erase(editContent.begin()+cursorY+1);
                        
                        scrollUpLine();//may give bug
                        InvalidateRect(hWnd,NULL,TRUE);

                        scrollRightMostPart(cursorX,cursorY);
                        SetCaretPosition2(cursorX,cursorY);
                    }
                }
                SendMessage(hWnd,LOAD_EDITOR_PLUGIN,0,0);

                return 0;
            }

            else if (wp == VK_TAB){
                //tab key will add tab_spaces
                //TODO : add tab in stop eg:if 2 space is given then tab should give tab_space-2 no of spaces
                deleteAutoCompleteWindow();                
                
                if (isWordSelected){
                    if (startSelectionY == endSelectionY){
                        //single line
                        editContent[startSelectionY].insert(editContent[startSelectionY].begin()+startSelectionX,TAB_SPACES,L' ');
                    }
                    else{
                        for (int y = startSelectionY;y<=endSelectionY;y++) editContent[y].insert(editContent[y].begin(),TAB_SPACES,L' ');
                    }
                        
                    startSelectionX += TAB_SPACES;
                    endSelectionX += TAB_SPACES;
                    cursorX += TAB_SPACES;
                    SetCaretPosition2(cursorX,cursorY);
                    InvalidateRect(hWnd,NULL,TRUE);
                    return 0;
                }
                editContent[cursorY].insert(editContent[cursorY].begin()+cursorX,TAB_SPACES,L' ');
                cursorX += TAB_SPACES;
                SetCaretPosition2(cursorX,cursorY);
                InvalidateRect(hWnd,NULL,TRUE);
                return 0;
            }
            return 0;
            
            // break;
        }

        case WM_CHAR:
        {
            //all input will be recieved here
            if (wp < VK_SPACE) break;
            POINT p;
            GetCaretPos(&p);

            // api calls    --> will tell all the extension functions that something is written
            // and you can do whatever u want to do with it
            ExtensionApi::callCharTypedCallbacks((wchar_t)wp); 

            if (!editContent.size()) editContent.push_back(L"");

            if (isWordSelected){
                cursorX = startSelectionX;
                cursorY = startSelectionY;
                removeSelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
                startSelectionX = -1;
                startSelectionY = -1;
                endSelectionX = -1;
                endSelectionY = -1;
            }
            if (!editContent.size()) {
                editContent.push_back(L"");
                cursorY = 0;
                cursorX = 0;
            }
            editContent[cursorY].insert(editContent[cursorY].begin()+cursorX,wp);

            scrollRightLine();

            cursorX++;

            auto contentCopy = editContent;
            int cx = cursorX;
            int cy = cursorY;

            load_plugin_safe(contentCopy,cx,cy);

            InvalidateRect(hWnd,NULL,TRUE);
            UpdateWindow(hWnd); // Force WM_PAINT to execute right now

            SetCaretPosition2(cursorX,cursorY);
            // PostMessage(hWnd,AUTO_COMPLETE_CREATE,0,0);

            SendMessage(hWnd,AUTO_COMPLETE_CREATE,0,0);
            
            // hautoComplete = createAutoCompleteWindow(hmainWindow,p.x+editorFontWidth+EDITOR_X,p.y+EDITOR_Y+EDITOR_LINE_HEIGHT,10,tokens);

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r;
            GetClientRect(hWnd, &r);

            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);

            // r.right -= 12;
            // r.bottom -= 12;

            int width = r.right - r.left;
            int height = r.bottom - r.top;

            int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
            int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)editorFontWidth;
            
            maxLineWidthValue = maxLineWidth();
            int maxLines = editContent.size(); //total Lines
            int totalContentHeight = maxLines*EDITOR_LINE_HEIGHT + ADDITIONAL_SCROLL_LINES; //total content height
            int totalContentWidth = maxLineWidthValue*editorFontWidth + ADDITIONAL_SCROLL_WIDTH;
            totalContentHeight = Max(totalContentHeight,height-SCROLL_BAR_WIDTH-EDITOR_WRITING_PART_Y);
            totalContentWidth = Max(totalContentWidth,width-SCROLL_BAR_WIDTH-EDITOR_WRITING_PART_X);

            int visibleHeight = height-EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH; //not considered editor_writing_part_y
            int visibleWidth = width-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH;

            float vertRatio = (float)visibleHeight/(float)totalContentHeight; //this ratio will be multiplied with scoll area height to get thumb height
            float horzRatio = (float)visibleWidth/(float)totalContentWidth;

            vertThumbHeight = vertRatio*(height-SCROLL_BAR_WIDTH); //scroll area height
            horzThumbWidth = horzRatio*(width-SCROLL_BAR_WIDTH); //scroll area width

            vertThumbY = scrolledY*height/totalContentHeight;
            horzThumbX = scrolledX*width/totalContentWidth;


            HBRUSH bg = CreateSolidBrush(colorTheme.editorbk);
            
            //double buffering method to reduce flickering
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, editorWidth, editorHeight);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            //background
            FillRect(memDC, &r, bg);

            int topLine = scrolledY/EDITOR_LINE_HEIGHT;
            int startindex = scrolledX/editorFontWidth;


            //if scrolled is 5 the we will draw from editor_writing_part_y - 5 so it looks clipped

            WriteContentInEditor(memDC,&editContent,topLine,topLine + visibleHeight/EDITOR_LINE_HEIGHT + 1,topY,totalContentWidth,startindex,startindex+visibleWidth/editorFontWidth,leftX);

            // a rectangle is drawn on top to make the text look clipped
            // RECT lineRect = {r.left,r.top,r.right - r.left - SCROLL_BAR_WIDTH,EDITOR_WRITING_PART_Y};
            // FillRect(memDC, &lineRect, bg);


            // lineRect = {r.left,r.top-EDITOR_WRITING_PART_Y,r.left+EDITOR_WRITING_PART_X,r.bottom-r.top-EDITOR_WRITING_PART_Y};
            // FillRect(memDC, &lineRect, bg);

            // draw_line_no(memDC,topLine,topLine + visibleHeight/EDITOR_LINE_HEIGHT + 1,topY);


            // a rectangle is drawn on top to make the text look clipped
            RECT lineRect = {r.left,r.top-EDITOR_WRITING_PART_Y,r.left+EDITOR_WRITING_PART_X,r.bottom-r.top-EDITOR_WRITING_PART_Y};
            FillRect(memDC, &lineRect, bg);
            draw_line_no(memDC,topLine,topLine + visibleHeight/EDITOR_LINE_HEIGHT + 1,topY);

            lineRect = {r.left,r.top,r.right - r.left - SCROLL_BAR_WIDTH,EDITOR_WRITING_PART_Y};
            FillRect(memDC, &lineRect, bg);


            
// for (int i = topLine;i<topLine + visibleHeight/EDITOR_LINE_HEIGHT + 1;i++){
                // TextOut(memDC,10,topY + (i-topLine)*EDITOR_LINE_HEIGHT,std::to_wstring(i).c_str(),wcslen(std::to_wstring(i).c_str()));
            // }

            
            //scrollbar
            DrawScrollBar(memDC,r.left,height-SCROLL_BAR_WIDTH,width-SCROLL_BAR_WIDTH,r.top,width-SCROLL_BAR_WIDTH,height-SCROLL_BAR_WIDTH
            ,colorTheme.scrollbarbk,colorTheme.scrollbarthumb);

            //mini code viewer
            int top_line_mini_code_viewer = ((topLine-MINI_CODE_VIEWER_LINE_BUFFER)<0)?0:topLine-MINI_CODE_VIEWER_LINE_BUFFER; //mini code viewer will show line from some line before you are
            
            if (drawMiniCodeViewer) createMiniCodeViewer(memDC,&editContent,top_line_mini_code_viewer,r.right-r.left-SCROLL_BAR_WIDTH-MINI_CODE_VIEWER_WIDTH,0,MINI_CODE_VIEWER_WIDTH,r.bottom-r.top-SCROLL_BAR_WIDTH,colorTheme.miniCodeViewerbk);
            
            //shadow of mini code viewer
            HBRUSH hShadowBrush = CreateSolidBrush(RGB(20, 20, 20));
            RECT shadowRect = { r.right-SCROLL_BAR_WIDTH-MINI_CODE_VIEWER_WIDTH- 3,0, r.right-SCROLL_BAR_WIDTH -MINI_CODE_VIEWER_WIDTH, r.bottom-SCROLL_BAR_WIDTH};

            FillRect(memDC, &shadowRect, hShadowBrush);
            DeleteObject(hShadowBrush);

            if (lightning_animation.showAnimation && !lightning_animation.isAnimationOver && lightning_animation.lastFrame != lightning_animation.index){
                lightning_animation.show(memDC,p.x-30,p.y-118); //-30,-118 from coor because each animation may have different size and coor can be different from mouse coor
            }



            // moving_dino.show(memDC);

            BitBlt(hdc, 0, 0, editorWidth, editorHeight, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, oldBitmap);

            DeleteDC(memDC);
            DeleteObject(bg);

            EndPaint(hWnd, &ps);

            return 0;        
        }

        case AUTO_COMPLETE_CREATE:
        {
            //custom message sent by wm_char
            //will create auto complete window
            POINT p;
            GetCaretPos(&p);

            //will delete autocomplete window if exist
            deleteAutoCompleteWindow();
            // hautoComplete = createAutoCompleteWindow(hmainWindow,p.x+editorFontWidth+EDITOR_X,p.y+EDITOR_Y+EDITOR_LINE_HEIGHT,plugin.tokens);
            hautoComplete = createAutoCompleteWindow(hmainWindow,p.x+editorFontWidth+EDITOR_X,p.y+EDITOR_Y+EDITOR_LINE_HEIGHT,*plugin.auto_complete_tokens,plugin.current_typed_word);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            POINT p;
            RECT r;
            // Handle_Multipe_Clicks(hWnd);
            
            deleteAutoCompleteWindow();                

            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);

            GetClientRect(hWnd,&r);
            // r.right -= 12;
            // r.bottom -= 12;
            
            SetFocus(hWnd); //setting keyboard focus

            //vertical scrollbar
            if (p.x >= r.right-r.left-SCROLL_BAR_WIDTH){
                if (p.y >= vertThumbY && p.y < vertThumbY + vertThumbHeight){
                    //clicked on the thumb
                    vScrolling = 1;
                }
                else if (p.y >=0 && p.y < r.bottom-r.top-SCROLL_BAR_WIDTH){
                    //clicked on scroll area outside thumb
                    vertThumbY = p.y;
                    if (vertThumbY + vertThumbHeight > r.bottom - r.top - SCROLL_BAR_WIDTH){
                        vertThumbY = r.bottom-r.top-SCROLL_BAR_WIDTH-vertThumbHeight;
                    }
                    else vertThumbY -= vertThumbHeight/4; //just to look good
                    SetScrolledY(r);
                    InvalidateRect(hWnd,NULL,TRUE);

                }
            }

            //horizontal scrollbar
            else if (p.y >= r.bottom-r.top-SCROLL_BAR_WIDTH){
                if (p.x >= horzThumbX && p.x < horzThumbX + horzThumbWidth){
                    //clicked on the thumb
                    hScrolling = 1;
                }
                else if (p.x >= 0 && p.x < r.right - r.left - SCROLL_BAR_WIDTH){
                    //clicked on scroll area outside thumb
                    horzThumbX = p.x;

                    if (horzThumbX + horzThumbWidth > r.right-r.left-SCROLL_BAR_WIDTH){
                        horzThumbX = r.right-r.left-SCROLL_BAR_WIDTH-horzThumbWidth;
                    }
                    else horzThumbX -= horzThumbWidth/4; //just to look good
                    SetScrolledX(r);

                    InvalidateRect(hWnd,NULL,TRUE);
                }
            }

            //editor area
            if (p.x > EDITOR_WRITING_PART_X && p.x < editorWidth - SCROLL_BAR_WIDTH && p.y > EDITOR_WRITING_PART_Y && p.y < editorHeight - SCROLL_BAR_WIDTH){
                //set animation
                // SetTimer(hWnd,1,ANIMATION_FRAME_TIME,NULL); //will start timer for animation
                
                // SetTimer(hWnd,ANIMATION_TIMER,ANIMATION_FRAME_TIME,NULL); //will start timer for animation
                // lightning_animation.init();
                SetCaretPosition(p.x,p.y);
                    // GetColRow(&p);
                isselectingWord = 1; //if selected then deselect
                
                if (!isWordSelected){
                    //starting selection
                    startSelectionX = cursorX;
                    startSelectionY = cursorY;
                    endSelectionX = -1;
                    endSelectionY = -1;
                }
                else{
                    isWordSelected = 0;
                    startSelectionX = -1;
                    startSelectionY = -1;
                    endSelectionX = -1;
                    endSelectionY = -1;
                    InvalidateRect(hWnd,NULL,TRUE);
                }
                SetTimer(hWnd,ANIMATION_TIMER,ANIMATION_FRAME_TIME,NULL); //will start timer for animation
                lightning_animation.init();
            }  

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            POINT p;
            GetCursorPos(&p);
            std::vector<context_map*> data;
            context_map* find_ = new context_map{L"Find",C_M_RENAME_FOLDER};
            context_map* copyContent_ = new context_map{L"Copy",C_M_CREATE_NEW_FILE};
            context_map* cut_ = new context_map{L"Cut",C_M_CREATE_NEW_FILE};
            context_map* paste_ = new context_map{L"Paste",C_M_RENAME_FOLDER};
           
            ExtensionApi::callEditorContextMenuCallbacks(&data); 

            data = {find_,copyContent_,cut_,paste_};
            
            if (IsWindow(hEditorContextMenu)) DestroyWindow(hEditorContextMenu);
            hEditorContextMenu = createContextMenuWindow(p.x,p.y,hWnd,hmainWindow,data);
            
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            tme.dwHoverTime = HOVER_DEFAULT;
            TrackMouseEvent(&tme);

            if (vScrolling){
                int y = HIWORD(lp);
                RECT r;
                GetClientRect(hWnd,&r);
                if (y + vertThumbHeight <= r.bottom - r.top - SCROLL_BAR_WIDTH){
                    vertThumbY = y;
                    int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
                    // int visibleHeight = r.bottom - r.top - EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH; //not considered editor_writing_part_y
                    int visibleHeight = r.bottom - r.top - topY-SCROLL_BAR_WIDTH; //not considered editor_writing_part_y
                    float ratio = visibleHeight/vertThumbHeight;
                    scrolledY = ratio*vertThumbY;
                    InvalidateRect(hWnd,NULL,TRUE);
                }
            }
            else if (hScrolling){
                int x = LOWORD(lp);
                RECT r;
                GetClientRect(hWnd,&r);
                // r.right -= 12;
                // r.bottom -= 12;
                if (x + horzThumbWidth < r.right - r.left - SCROLL_BAR_WIDTH){
                    horzThumbX = x;
                    int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)editorFontWidth;
                    // int visibleWidth = r.right-r.left-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH;
                    int visibleWidth = r.right-r.left-leftX-SCROLL_BAR_WIDTH;
                    float ratio = visibleWidth/horzThumbWidth;
                    scrolledX = ratio*horzThumbX;
                    InvalidateRect(hWnd,NULL,TRUE);
                    // RECT updateRect = { EDITOR_X, EDITOR_Y, WIDTH - SCROLL_BAR_WIDTH, HEIGHT };
                    // InvalidateRect(hWnd, &updateRect, FALSE);
                }
            }
            else if (isselectingWord){
                POINT p;
                GetCursorPos(&p);
                ScreenToClient(hWnd,&p);
                // SetCaretPosition(p.x,p.y);
                endSelectionX = cursorX;
                endSelectionY = cursorY;
                lightning_animation.isAnimationOver = 1;
                lightning_animation.showAnimation = 0;
                InvalidateRect(hWnd,NULL,TRUE);
                SetCaretPosition(p.x,p.y);
            }
            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            RECT r;
            GetClientRect(hWnd,&r);
            // r.right -= 12;
            // r.bottom -= 12;
            float scrolled = GET_WHEEL_DELTA_WPARAM(wp)*SCROLL_BAR_SENSITIVITY;
            int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
            int visibleHeight = r.bottom - r.top - EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH; //not considered editor_writing_part_y
            float ratio = visibleHeight/vertThumbHeight;
            int ll = (editorHeight-SCROLL_BAR_WIDTH-EDITOR_WRITING_PART_Y)/EDITOR_LINE_HEIGHT;
            if (editorHeight-EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH > editContent.size()*EDITOR_LINE_HEIGHT) return 0; //no scrolling
            scrolledY -= scrolled;


            if (scrolledY < 0) {
                scrolledY = 0;
                InvalidateRect(hWnd,NULL,TRUE);
            }

            else if(scrolledY > editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y + ADDITIONAL_SCROLL_LINES - SCROLL_BAR_WIDTH){
                scrolledY = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y + ADDITIONAL_SCROLL_LINES - SCROLL_BAR_WIDTH;
                InvalidateRect(hWnd,NULL,TRUE);
            }
            else if(scrolledY >=0 && scrolledY <= editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y + ADDITIONAL_SCROLL_LINES - SCROLL_BAR_WIDTH){
                InvalidateRect(hWnd,NULL,TRUE);
            }

            SetCaretPosition2(cursorX,cursorY);

            return 0;
        }
        
        case WM_MOUSEHWHEEL:
        {
            RECT r;
            GetClientRect(hWnd,&r);
            // r.right -= 12;
            // r.bottom -= 12;
            float scrolled = GET_WHEEL_DELTA_WPARAM(wp)*SCROLL_BAR_SENSITIVITY;
            int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)editorFontWidth;
            int visibleWidth = r.right-r.left-leftX-SCROLL_BAR_WIDTH;
            float ratio = visibleWidth/horzThumbWidth;
            int v_line = visibleWidth/editorFontWidth;

            // if (editorWidth-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH > editContent[cursorY].size()*editorFontWidth) return 0; //no scrolling
            if (editorWidth-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH >maxLineWidthValue*editorFontWidth) return 0; //no scrolling

            scrolledX += scrolled;

            if (scrolledX < 0) {
                scrolledX = 0;
                InvalidateRect(hWnd,NULL,TRUE);
            }

            else if (scrolledX > (maxLineWidthValue-v_line)*editorFontWidth + ADDITIONAL_SCROLL_WIDTH-SCROLL_BAR_WIDTH){
                scrolledX = (maxLineWidthValue-v_line)*editorFontWidth + ADDITIONAL_SCROLL_WIDTH-SCROLL_BAR_WIDTH;
                InvalidateRect(hWnd,NULL,TRUE);
            }
            else InvalidateRect(hWnd,NULL,TRUE);
            SetCaretPosition2(cursorX,cursorY);

            
            return 0;
        }

        case WM_LBUTTONUP:
        {
            if (vScrolling | hScrolling){
                hScrolling = 0;
                vScrolling = 0;
                ReleaseCapture();
            }
            else if (isselectingWord && endSelectionX >= 0 && endSelectionY >= 0 && startSelectionX >= 0 && endSelectionX >= 0){
                isWordSelected = 1;
                ReleaseCapture();
            }
            isselectingWord = 0;

            
            return 0;
        }

        case WM_TIMER:
        {
            if (wp == 1) {
                RemoveProp(hWnd, L"DoubleClickCount");
                KillTimer(hWnd, 1);
            }
            else if (wp == ANIMATION_TIMER){
                //click animation
                lightning_animation.update();
                if (lightning_animation.isAnimationOver || !lightning_animation.showAnimation){
                    KillTimer(hWnd,ANIMATION_TIMER);
                    int x_max = (lightning_animation.x_-200 > 0)?lightning_animation.x_-200:0;
                    int y_max = (lightning_animation.y_-200 > 0)?lightning_animation.y_-200:0;
                    RECT r = {x_max,y_max,lightning_animation.x_+200,lightning_animation.y_+200};
                    InvalidateRect(hWnd,NULL,TRUE);
                    // InvalidateRect(hWnd,&r,TRUE);
                    // ShowCaret(hWnd);
                    // SetCaretPosition2(cursorX,cursorY);

                }
                else{
                    int x_max = (lightning_animation.x_-200 > 0)?lightning_animation.x_-200:0;
                    int y_max = (lightning_animation.y_-200 > 0)?lightning_animation.y_-200:0;
                    RECT r = {x_max,y_max,lightning_animation.x_+200,lightning_animation.y_+200};
                    InvalidateRect(hWnd,NULL,TRUE);
                    // InvalidateRect(hWnd,&r,TRUE);
                    // InvalidateRect(hWnd,NULL,TRUE);
                }
            }

            else if (wp == FILE_CONTENT_SAVE_TIMER && selectedFilePath != "" && editContent.size()){
                // save file content
                std::vector<std::wstring> content = editContent;
                save_file(selectedFilePath,content);
            }



            // else if (wp == 3){
                // moving_dino.update();
                // RECT r = {
    // moving_dino.x - 10, // add padding if needed
    // moving_dino.y - 10,
    // moving_dino.x + 74, // 64 + padding
    // moving_dino.y + 74
// };
                // InvalidateRect(hWnd,&r,TRUE);
            // }
            return 0;
        }

        case WM_MOUSEHOVER:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd, &p);

            ExtensionApi::callMouseMoveInEditorCallbacks(&p); 

            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER;
            tme.hwndTrack = hWnd;
            // tme.dwHoverTime = 500;
            tme.dwHoverTime = 250;
            TrackMouseEvent(&tme);
            return 0;
        }

        case WM_LBUTTONDBLCLK:
        {
            Handle_Multiple_Clicks(hWnd);
            return 0;
        }

        case WM_COMMAND:
        {
            ExtensionApi::callOnEditorContextMenuItemClick(wp,lp);
            return 0;
        }

        case WM_CREATE:
        {
            RECT r;
            GetClientRect(hWnd,&r);
            logger.info_log("Editor created");
            // r.right -= 12;
            // r.bottom -= 12;
            editorWidth = r.right - r.left;
            editorHeight = r.bottom - r.top;
            // moving_dino.init(0,10,r.right,10);
            
            //getting the constant width of font
            HDC hdc = GetDC(hWnd);
            SIZE sz;
            SelectObject(hdc,editorFont);
            GetTextExtentPoint32(hdc, L"a", 1, &sz); //choosed "a" because every character will have same width
            editorFontWidth = sz.cx;
            ReleaseDC(hWnd,hdc);

            

            // WriteFileContent("E:/Text Editor - Copy/demo.cpp");


            // WriteFileContent("E:/Text Editor - Copy(2)/file_explorer.h");
            // WriteFileContent("E:/Text Editor - Copy (2)/file_explorer.h");
            // WriteFileContent("E:/Text Editor - Copy (2)/demo.cpp");

            auto contentCopy = editContent;
            int cx = cursorX;
            int cy = cursorY;

            load_plugin_safe(contentCopy, cx, cy);
            // load_plugin(contentCopy, cx, cy);
// }).detach();

            // for (int i = 0;i<100;i++){
                // editContent.push_back(string_to_wstring(std::to_string(i)) + L"ASDADDskldnhsakdjadlkasdkjsabdlasdksakjdnaskldldnkjasdklsamdklasndkjasldkjasjdaskjdaslkdnaskjhdalsjdklasndkjasjdlkasjdljasdkjajdlkajdjadladkljldaj");
            // }

            lightning_animation.load_images(); //load images of animation

            SetTimer(hWnd,FILE_CONTENT_SAVE_TIMER,FILE_SAVE_TIME,NULL);
           
    //         TextDisplayWindow myTextWindow;
    // myTextWindow.create(hmainWindow, 200, 200, 300, 150);
    // myTextWindow.setText(L"Hello, World!\nThis is your custom text window.");
    // // myTextWindow.show();

            // moving_dino.load_images();
            // SetTimer(hWnd,3,ANIMATION_FRAME_TIME,NULL); //will start timer for animation
            // th.join();
            break;
        }

        case LOAD_EDITOR_PLUGIN:
        {
            auto contentCopy = editContent;
            int cx = cursorX;
            int cy = cursorY;
            load_plugin_safe(contentCopy, cx, cy);
            return 0;
        }

        case WM_SETFOCUS:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            int ly = (p.y-EDITOR_WRITING_PART_Y)/EDITOR_LINE_HEIGHT; //line no
            cursorY = ly;
            if (cursorY >= editContent.size()) cursorY = editContent.size()-1;
            else if (cursorY < 0) cursorY = 0;
            // cursorX = 0;
            drawCarret(hWnd,0 + EDITOR_WRITING_PART_X,0*EDITOR_LINE_HEIGHT + EDITOR_WRITING_PART_Y,EDITOR_LINE_HEIGHT);
            //will check for if the selected point is within writing part
            // if (ly < LinesWidth.size() && p.x < LinesWidth[ly]) drawCarret(hWnd,p.x,p.y,CARRET_HEIGHT);
            // else drawCarret(hWnd,cursorX + EDITOR_WRITING_PART_X,cursorY + EDITOR_WRITING_PART_Y,CARRET_HEIGHT);
        }
            return FALSE;

        case WM_KILLFOCUS:
            HideCaret(hWnd);
            DestroyCaret(); 
            break;
        
        case WM_GETDLGCODE:
            // Sent to the window procedure associated with a control
            // By default, the system handles all keyboard input to the control;
            // the system interprets certain types of keyboard input as dialog box navigation keys.
            // To override this default behavior, the control can respond to the 
            // WM_GETDLGCODE message to indicate the types of input it wants to process itself.
            return DLGC_WANTCHARS | DLGC_WANTARROWS;

        case WM_SETCURSOR:
        {
            //to set type of cursor
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            if (p.x < EDITOR_WRITING_PART_X) SetCursor(LoadCursor(NULL, IDC_ARROW));
            else if (p.x > editorWidth-SCROLL_BAR_WIDTH) SetCursor(LoadCursor(NULL,IDC_ARROW));
            else if (p.y > editorHeight-SCROLL_BAR_WIDTH) SetCursor(LoadCursor(NULL,IDC_ARROW));
            else SetCursor(LoadCursor(NULL, IDC_IBEAM));
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            if (hScrolling | vScrolling){
                hScrolling = 0;
                vScrolling = 0;
                ReleaseCapture();
            }
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_DESTROY:
            PostQuitMessage(0);
            logger.info_log("Editor closed");
            return 0;
    }


    return DefWindowProc(hWnd, msg, wp, lp);
}



void createEditor(HWND hWnd, int x, int y, int width, int height) {
    // HWND hwnd = CreateWindowEx(0, L"CustomEditWindowClass", L"",
    // HWND hEditor = CreateWindowEx(0, L"CustomEditWindowClass", L"",
    hEditor = CreateWindowEx(0, L"CustomEditWindowClass", L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER , x, y, width, height,
        hWnd, NULL, NULL, NULL);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)&plugin);
    int double_click_count = 0;
    // SetProp(hEditor, L"DoubliClickCount", (HANDLE)&double_click_count);
    SetProp(hEditor, L"DoubleClickCount", (HANDLE)(INT_PTR)double_click_count);
}
