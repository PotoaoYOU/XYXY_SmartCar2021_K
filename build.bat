:START
@echo off
cls
C:\Keil_v5\UV4\UV4.exe -b -j0 LQDDCdemo.uvproj -o ./log.txt
type log.txt
del .\log.txt