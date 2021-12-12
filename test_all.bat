@echo off
setlocal enabledelayedexpansion

FOR %%D IN (2021\day*.c) DO (
    set BASE=%%~nD
    set DAY_N=!BASE:~-2!
    echo ----------------- DAY !DAY_N! ------------------
    python intcode_cc.py -i math.c string.c sort.c 2021/!BASE!.c -o 2021/!BASE!.txt
    intcode_vm 2021/!BASE!.txt < 2021/input!DAY_N!.txt
)
