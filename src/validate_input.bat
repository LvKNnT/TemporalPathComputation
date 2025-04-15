g++ -std=c++17 -O2 -Wall validator/validator.cpp header/testlib.h -o validator/validator.exe
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
) else (
    echo Compilation succeeded.
)

set "FOLDER=input"
set "VALIDATOR=validator\validator.exe"

for %%F in ("%FOLDER%\*") do (
    echo Validating: %%F
    %VALIDATOR% < "%%F"
)
pause
