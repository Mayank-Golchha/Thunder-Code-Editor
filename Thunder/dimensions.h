#ifndef DIMENSIONS_H
#define DIMENSIONS_H

//window
int WIDTH = 1400;
int HEIGHT = 800;
int WINDOW_X = 100;
int WINDOW_Y = 50;

//animation time per frame
int ANIMATION_FRAME_TIME = 25; //25ms
int FILE_SAVE_TIME = 3000; //after every given time save content

//constant to multiply with default width of font
// int default_font_width = 10;
int default_font_width = 8;

//title bar
int TITLE_BAR_HEIGHT = 30;

//window control button
int WINDOW_CLOSE_BUTTON_L = 50;

//hover window
int hoverwindowFontSize = 14;
int hoverwindowFontWidth = 6;


//menubar
int FILE_MENU_X = 0;
int FILE_MENU_Y = 0;
int APP_LOGO_X = 5;
int APP_LOGO_Y = 0;
int FILE_MENU_ITEMS_X = 40; // from here drawing will start
int SUB_MENU_WIDTH = 120;
// int SUB_MENU_HEIGHT = 20; //i think no file is using this variable


//file_explorer
int FILE_EXPLORER_X = 0;
int FILE_EXPLORER_Y = TITLE_BAR_HEIGHT;
// int FILE_EXPLORER_ITEM_HEIGHT = 20;
int FILE_EXPLORER_ITEM_HEIGHT = 25;
int FILE_EXPLORER_WIDTH = 250;

//file_explorer icons
int NEW_FOLDER_ICON = 22; //from right
int NEW_FILE_ICON = 22 + NEW_FOLDER_ICON; //from right


//highlighting width -> the blue line that appears in visual studio code above current tab
int HIGHTLIGHT_WIDTH = 4;

//file tab
// int FILE_TAB_X = 0;
int FILE_TAB_X = FILE_EXPLORER_WIDTH + FILE_EXPLORER_X;
int FILE_TAB_Y = TITLE_BAR_HEIGHT;
int FILE_TAB_HEIGHT = 30;
int FILE_TAB_WIDTH = 100;
int FILE_TAB_CLOSE_BUTTON_WIDTH = 30;
int FILE_TAB_FONT_SIZE = 14;
int FILE_TAB_FONT_WEIGHT = 500;
int FILE_TAB_HOVER_WINDOW_WIDTH = 200;
int FILE_TAB_HOVER_WINDOW_HEIGHT = 20;


//editor
// int EDITOR_X = 0;
int EDITOR_X = FILE_EXPLORER_WIDTH + FILE_EXPLORER_X;
int EDITOR_Y = FILE_TAB_Y + FILE_TAB_HEIGHT;
int EDITOR_HEIGHT_MAX = 1000;
int EDITOR_FONT_HEIGHT = 17;
int EDITOR_FONT_WIDTH = 7;
int EDITOR_FONT_WEIGHT = 560;
int EDITOR_LINE_HEIGHT = EDITOR_FONT_HEIGHT*1.2;

int EDITOR_WRITING_PART_X = 60;
int EDITOR_WRITING_PART_Y = EDITOR_LINE_HEIGHT; //should be greater the line height else will give bug
int CARRET_HEIGHT = 18;
int CARRET_WIDTH = 1;

//line no
//from here drawing of line no will start
int EDITOR_LINE_NO_X = EDITOR_X + 15; 

//scrollbar
int SCROLL_BAR_WIDTH = 15;
float SCROLL_BAR_SENSITIVITY = 0.15;
int ADDITIONAL_SCROLL_LINES = 50;
// int ADDITIONAL_SCROLL_WIDTH = 50; //addition width that will be added to lineMaxWidth 
int ADDITIONAL_SCROLL_WIDTH = 200; //addition width that will be added to lineMaxWidth 
int SCROLL_BAR_HORIZONTAL_MOVEMENT_WIDTH = 15; //how much will scroll bar move -> 10 

int TAB_SPACES = 3; //3 space in editor

//mini code viewer
int MINI_CODE_VIEWER_WIDTH = 100;
int MINI_CODE_VIEWER_LINE_BUFFER = 50; //means -> top_line - mini_code_viewer_line_buffer -> will become the top line to display

//autocomplete
int AUTO_COMPLETE_MAX_OPTIONS = 5; //maximum options that will be seen
int AUTO_COMPLETE_OPTION_HEIGHT = 25; //autocomplete option height
int AUTO_COMPLETE_OPTION_FONT_HEIGHT = 17;
int AUTO_COMPLETE_OPTION_FONT_WEIGHT = 550;

//inputwindow
int INPUT_WINDOW_WIDTH = 150; 
int INPUT_WINDOW_HEIGHT = 25; 
int INPUT_WINDOW_X = 200; 
int INPUT_WINDOW_y = 100; 
int INPUT_WINDOW_WRITING_PART_X = 0; 
int INPUT_WINDOW_WRITING_PART_Y = 3; 
int INPUT_WINDOW_FONT_SIZE = 16; 

//context_menu
int CONTEXT_MENU_WIDTH = 300;
int CONTEXT_MENU_FONT_HEIGHT = 16;
int CONTEXT_MENU_LINE_HEIGHT = 30;
int CONTEXT_MENU_X_OFFSET = 50; //from her writing will start


const int MAXINPUTBUF = 1000000;

#endif