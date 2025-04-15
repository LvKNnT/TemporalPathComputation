rem    *** tests ***
g++ -std=c++17 -O2 -Wall generator/gen.cpp header/testlib.h -o generator/gen.exe
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
) else (
    echo Compilation succeeded.
)
md input
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=40 -max_d=10 -bias_t=100000 1" "input\01" 1
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=40 -max_d=10 -bias_t=100000 1 -mode smart" "input\02" 2
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=40 -max_d=10 -bias_t=100000 2" "input\03" 3
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=40 -max_d=10 -bias_t=100000 2 -mode smart" "input\04" 4
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=100 -max_d=10 -bias_t=100000 1 -mode smart" "input\05" 5
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=100 -max_d=10 -bias_t=100000 1" "input\06" 6
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=100 -max_d=10 -bias_t=100000 2 -mode smart" "input\07" 7
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -n=10 -m=100 -max_d=10 -bias_t=100000 2" "input\08" 8
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=100 -bias_t=1000 1" "input\09" 9
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=100 -bias_t=1000 1 -mode smart" "input\10" 10
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=1000 -bias_t=1000 1" "input\11" 11
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=1000 -bias_t=1000 1 -mode smart" "input\12" 12
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=100 -bias_t=1000 2" "input\13" 13
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=100 -bias_t=1000 2 -mode smart" "input\14" 14
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=1000 -bias_t=1000 2" "input\15" 15
call scripts\gen-input-via-stdout.bat ""generator/gen.exe" -max_d=1000 -bias_t=1000 2 -mode smart" "input\16" 16
