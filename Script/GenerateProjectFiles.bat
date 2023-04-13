@echo off

echo run generate project files...

SET visualstudio=%1
SET project=%2
SET premake5=Bin\\premake5.exe

if "%visualstudio%" == "" (
    echo make a choice visual studio version vs2019 or vs2022...
    GOTO:EOF
)

if not exist Crash (
    echo can't find Crash directory...
    echo generate Crash directory...
    mkdir Crash
)

echo run premake5.exe...
%premake5% %visualstudio%

echo start visual studio solution...
start %project%\\%project%.sln
PAUSE