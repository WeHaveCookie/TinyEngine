@echo off
Tools\premake5.exe --file=tinyEngine_premake.lua vs2015
if %ERRORLEVEL% NEQ 0 PAUSE