@echo off

md "build_windows" 2>NUL

cd build_windows

setlocal EnableExtensions EnableDelayedExpansion

rem set /a count = 1

SET vcpkg = String1 
SET firebaseDir = String1

for /f "tokens=*" %%a in (../build_win.config) do (

	set "line=%%a"
 
	if not "!line:vcpkg=!" == "!line!" ( SET vcpkg=%%a ) ELSE ( SET firebaseDir=%%a )
  
)

set excuteCMD=cmake .. -G^"Visual Studio 16 2019^" -Tv142 -A win32 -DCMAKE_TOOLCHAIN_FILE=%vcpkg%

echo %excuteCMD%

%excuteCMD%

PAUSE >nul




