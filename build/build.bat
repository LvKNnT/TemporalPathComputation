@echo off
setlocal enabledelayedexpansion

:: Collect all .cpp files in src and subdirectories
set sources=
for /R ../src %%f in (*.cpp) do (
    set sources=!sources! "%%f"
)

:: Compile using g++
g++ !sources! -o Run.exe -O0 -I ../include/

:: Pause to see errors (if any)
pause