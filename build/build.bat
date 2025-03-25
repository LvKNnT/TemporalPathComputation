@echo off
setlocal enabledelayedexpansion

:: Collect all .cpp files in src and subdirectories
set sources=
for /R ../src %%f in (*.cpp) do (
    set sources=!sources! "%%f"
)

:: Compile using g++
g++ ../src/main.cpp -o main.exe -O3 --std=c++20

:: Pause to see errors (if any)
pause