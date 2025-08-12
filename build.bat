@echo off
setlocal enabledelayedexpansion

REM 
set ROOT=%~dp0

REM 
set MINGW=%ROOT%mingw64

REM 
set SRC=
for %%f in ("%ROOT%src\*.cpp") do (
    set SRC=!SRC! "%%f"
)

REM 
"%MINGW%\bin\g++.exe" !SRC! ^
-I"%ROOT%include" ^
-L"%ROOT%lib" ^
-lsfml-graphics -lsfml-window -lsfml-system ^
-static-libgcc -static-libstdc++ ^
-o "%ROOT%USFighters.exe"

pause
