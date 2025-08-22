
# API Manager Documentation

This file provides the **Extension API** for the code editor, allowing extensions to:
- Hook into editor events (keypress, mouse move, context menu, autocomplete, etc.).
- Modify editor behavior (insert text, scroll, update, etc.).
- Interact with the editor’s internal state safely.

---

## Overview
The API consists of:
1. **Callback registration & execution system**  
2. **Editor manipulation functions (text, caret, scrolling, themes, selection)**  
3. **Context menu and autocomplete management**  
4. **Utility methods for logging, file operations, and coordinate transformations**  

Namespace: **`ExtensionApi`**  
Main interface: **`EditorApiFunction`** (implements `EditorAPI`)  

---

## Callback Vectors

### 1. Character Typed Callbacks
```cpp
std::vector<std::function<void(wchar_t)>> charTypedCallbacks;
```
- **Purpose:** Holds callbacks triggered when a character is typed (before it is registered in the buffer).  
- **Example:** Auto-pairing brackets, syntax checks.

---

### 2. Mouse Move Callbacks
```cpp
std::vector<std::function<void(POINT*)>> mouseMoveCallbacks;
```
- **Purpose:** Holds callbacks triggered when the mouse moves over the editor.  
- **Example:** Real-time tooltip, cursor tracking.

---

### 3. Editor Context Menu Callbacks
```cpp
std::vector<std::function<void(std::vector<context_map*>*)>> editorContextMenuCallbacks;
```
- **Purpose:** Modify the context menu when it is created (add/remove items).  

---

### 4. Context Menu Item Click Callbacks
```cpp
std::vector<std::function<void(WPARAM, LPARAM)>> onEditorContextMenuItemClick;
```
- **Purpose:** Triggered when an item in the editor’s context menu is clicked.  
- **Parameters:**
  - `WPARAM wp`: The ID of the clicked item.  
  - `LPARAM lp`: Index of the clicked item.

---

### 5. Autocomplete Callbacks
```cpp
std::vector<std::function<void(
    std::vector<std::unique_ptr<AutoCompleteToken>>&,
    std::string
)>> autoCompleteCallBacks;
```
- **Purpose:** Modify autocomplete suggestions dynamically.

---

## Callback Invokers
These are called by the **backend** when specific events occur.

- `callCharTypedCallbacks(wchar_t ch)`
- `callEditorContextMenuCallbacks(std::vector<context_map*>* items)`
- `callMouseMoveInEditorCallbacks(POINT* p)`
- `callOnEditorContextMenuItemClick(WPARAM wp, LPARAM lp)`
- `callOnAutoCompleteCallBack(std::vector<std::unique_ptr<AutoCompleteToken>>& list, std::string current_word)`

Each of these iterates over the corresponding vector and invokes all registered functions.

---

## Main API Class: EditorApiFunction

This class inherits from `EditorAPI` and provides methods to **manipulate the editor**.

### Text Operations
- `void insertText(const std::wstring& text, int x, int y)`  
  Inserts text at given coordinates without moving the caret.  

- `void saveFile(std::string file_path, std::vector<std::wstring>& content)`  
  Saves editor content to a file.  

- `std::vector<std::wstring>& getEditorContent()`  
  Returns a reference to the text buffer.

---

### Caret and Cursor
- `void getCaretPosition(POINT* p)` – Get current caret position.  
- `void setCaretPosition(POINT* p)` – Set caret position and update internally.  
- `int getCurrentLineNo()` – Returns the current line number.  
- `void MouseToCursor(POINT* p)` – Convert mouse (pixel) coordinates to editor coordinates.  
- `void CursorToAppPosition(POINT* p)` – Convert editor coordinates to screen coordinates.

---

### Context Menu Management
- `void InsertEditorContextMenuItem(std::vector<context_map*>* items, context_map* data)`  
  Add an item to the context menu.  

- `context_map* newContextMapItem(std::wstring name, int id)`  
  Create a new context menu item (ID must be > `WM_USER + 100`).

---

### Scrolling & Viewing
- `void scrollParticularLine(int line_no)` – Scroll to a specific line.  
- `void RemoveMiniCodeViewer()` – Hide the mini code viewer.  
- `void SetMiniCodeViewer()` – Show the mini code viewer.

---

### Autocomplete
- `std::vector<std::unique_ptr<AutoCompleteToken>>& getAutoCompleteTokens()`  
  Access the autocomplete list (modifiable).

---

### Theme & Appearance
- `void setEditorTheme(std::string file_path)` – Load a theme from JSON file.  

- `int getFontWidth()` – Return current font width (monospace).  
- `int getLineHeight()` – Return line height.  

---

### Selection
- `void selectEditorText(int startX, int startY, int endX, int endY)`  
  Highlight text between two positions.  

- `int isTextSelected()`  
  Returns whether any text is selected.

---

### Utility Functions
- `std::string getCurrentFileExtension()` – Get extension of currently open file.  
- `std::string getCurrentFilePath()` – Get full file path.  
- `std::string wstringToString(std::wstring text)` – Convert wide to narrow string.  
- `std::wstring stringToWstring(std::string text)` – Convert string to wstring.  
- `void updateEditorContent()` – Redraw the editor after changes.  

- Logging:  
  - `info_log(message)`  
  - `warning_log(message)`  
  - `error_log(message)`  

---

## Registering Callbacks
Plugins can register callbacks via:
- `registerCharTypedCallback(std::function<void(wchar_t)> cb)`
- `registerOnMouseMoveCallback(std::function<void(POINT*)> cb)`
- `registerOnEditorContextMenuCreate(std::function<void(std::vector<context_map*>*)> cb)`

---

## Loading Extensions
```cpp
void LoadExtension(const wchar_t* dllPath, EditorAPI* api)
```
- Dynamically loads a plugin DLL.
- Calls its `RegisterExtension(EditorAPI* api)` function.
- Logs success or failure.

---

## Example Usage
### Add a Context Menu Item
```cpp
context_map* item = editorApi.newContextMapItem(L"My Custom Item", WM_USER + 120);
editorApi.InsertEditorContextMenuItem(contextMenuItems, item);
```

### Register a Typing Callback
```cpp
editorApi.registerCharTypedCallback([](wchar_t ch) {
    if (ch == L'(') {
        // Auto-insert closing parenthesis
    }
});
```

### Scroll to Line 50
```cpp
editorApi.scrollParticularLine(50);
```
