// when u change this remember to recompile all extensions

#define CLOSEBUTTON 10
#define FILE_ 20
#define OPENFILE 21
#define OPENFOLDER 22
#define SAVEFILE 23
#define CHANGEEDITORTHEME 24
#define EXIT 25
#define EDIT 30
#define FINDEDIT 31
#define REPLACEEDIT 32
#define AUTO_COMPLETE_OUTPUT WM_USER + 1 //will be send by autocomplete window to editor
#define AUTO_COMPLETE_DOWN_ARROW WM_USER + 2 //will be send by editor to autocomplete window to move down
#define AUTO_COMPLETE_UP_ARROW WM_USER + 3 //will be send by editor to autocomplete window to move down
#define AUTO_COMPLETE_CREATE WM_USER + 4 //will be send to editor to create autocomplete window
#define AUTO_COMPLETE_ENTER WM_USER + 5 //will be send by editor to autocomplete window when enter clicked
#define LOAD_EDITOR_PLUGIN WM_USER + 6 //will be send to editor to load the plugin -> will happen in parallel
#define FILE_EXPLORER_NEW_FILE WM_USER + 1 //will be send to file explorer when need to create a new file
#define FILE_EXPLORER_NEW_FOLDER WM_USER + 2 //will be send to file explorer when need to create a new fOLDER
#define INPUT_WINDOW_MESSAGE (WM_USER + 10) //will be send by input window to its parent window,will return the input it got from user in wp -> NOTE DONT CHANGE ITS VALUE
#define CREATE_NEW_FILE WM_USER + 11 //will be send to explorer to create a new file
#define CREATE_NEW_FOLDER WM_USER + 12 //will be send to explorer to create a new folder
#define RENAME_FOLDER WM_USER + 13 //will be send to explorer to create a new folder
#define RENAME_FILE WM_USER + 14 //will be send to explorer to create a new folder

//CONTEXT_MENU
//for file explorer
#define C_M_CREATE_NEW_FOLDER (WM_USER + 20)
#define C_M_CREATE_NEW_FILE (WM_USER + 21)
#define C_M_RENAME_FILE (WM_USER + 22)
#define C_M_RENAME_FOLDER (WM_USER + 23)
#define C_M_DELETE_FILE (WM_USER + 24)
#define C_M_DELETE_FOLDER (WM_USER + 25)
#define C_M_COPY_PATH (WM_USER + 26)


#define MULTIPLE_CLICK_TIMER 1 //timer for multiple clicks
#define ANIMATION_TIMER 2 //timer for animation
#define FILE_CONTENT_SAVE_TIMER 3 //timer for file content save



//file tab 100->150 for tab 150->200 for tab close button
#define FILETAB 100


#define CLICK_INTERVAL GetDoubleClickTime() //for handling double clicks
