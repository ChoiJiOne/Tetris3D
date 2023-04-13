@echo off

echo run hot reload...

SET visualstudio=%1
SET project=%2
SET premake5=Bin\\premake5.exe

if not exist %project% (
	echo don't exist %project%...
    echo generate project files...
)

echo run premake5.exe...
%premake5% %visualstudio%

echo done...