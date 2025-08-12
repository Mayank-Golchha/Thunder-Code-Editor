@echo off
setlocal

:: Paths
set EXT_DIR=extension
set EXT_SRC=%EXT_DIR%\hello_world.cpp
set EXT_DLL=hello_world.dll

set EDITOR_SRC=main.cpp
set EDITOR_EXE=main.exe

:: Step 1: Compile main editor executable
echo [1] Compiling editor...
g++ %EDITOR_SRC% -lgdi32 -lmsimg32 -ldwmapi -lgdiplus -lshell32 -lole32 -luuid -lcomdlg32 -o %EDITOR_EXE%

if errorlevel 1 (
    echo ❌ Editor compilation failed!
    exit /b 1
)

:: Step 2: Compile extension as DLL
echo [2] Compiling extension DLL...
g++ -std=c++17 -shared -I. -DBUILDING_DLL %EXT_SRC% -o %EXT_DLL% -lgdi32

if errorlevel 1 (
    echo ❌ DLL compilation failed!
    exit /b 1
)

:: Step 3: Run the editor
echo [3] Running the editor...
%EDITOR_EXE%

endlocal
