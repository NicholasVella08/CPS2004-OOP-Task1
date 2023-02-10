@echo off
cd /d %~dp0

set program=main.cpp

if not exist %program% (
    echo ERROR: Program %program% not found in this directory!
    echo Make sure to compile the program before running it.
    pause
    exit /b 1
)

%program% %*
pause
