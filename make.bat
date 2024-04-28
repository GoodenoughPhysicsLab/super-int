@echo off

set test_cl=false
set test_gcc=false
set test_clang=false

if exist ".\exe" (
    del /s /q .\exe
) else (
    mkdir exe
)

if "%~1"=="" (
    echo "|| release"
    set debug_cl=/D NDEBUG
    set debug=-D NDEBUG
) else (
    if "%~1"=="--debug" (
        echo "|| debug"
        set debug_cl=
        set debug=
    ) else (
        echo Unknown argument: "%~1"
        goto error
    )
)

echo "### MSVC ###"

cl /Feexe\test_cl.exe test.c /Ox /nologo /Ob2 /fp:fast /GL /MP /std:c11 /Wall %debug_cl%
if exist ".\exe\test_cl.exe" (
    set test_cl=true
)

echo "### GNU ###"

gcc -o .\exe\test_gcc.exe test.c -Ofast -march=native -ftree-vectorize -std=c99 -Wall %debug%
if exist ".\exe\test_gcc.exe" (
    set test_gcc=true
)

echo "### LLVM ###"

clang -o .\exe\test_clang.exe test.c -Ofast -march=native -ftree-vectorize -std=c99 -Wall %debug%
if exist ".\exe\test_clang.exe" (
    set test_clang=true
)


if %test_cl%==true (
    echo "[Success] test of MSVC"
    .\exe\test_cl.exe
) else (
    echo "[Fail] test of MSVC"
)

if %test_gcc%==true (
    echo "[Success] test of GNU"
    .\exe\test_gcc.exe
) else (
    echo "[Fail] test of GNU"
)

if %test_clang%==true (
    echo "[Success] test of LLVM"
    .\exe\test_clang.exe
) else (
    echo "[Fail] test of LLVM"
)
goto end

:error
echo "[error] exit"

:end