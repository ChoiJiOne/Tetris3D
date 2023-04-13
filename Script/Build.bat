@echo off

echo run build...

SET visualstudio=%1
SET project=%2
SET option=%3
SET build=msbuild.exe
SET premake5=Bin\\premake5.exe

if "%option%" == "" (
    echo make a choice build option...
    GOTO:EOF
)

if not exist %project% (
    echo can't find %project% visual studio solution
    echo generate project files...
)

echo run premake5.exe...
%premake5% %visualstudio%

%build% %project%\\%project%.sln -property:Configuration=%option% -target:Rebuild