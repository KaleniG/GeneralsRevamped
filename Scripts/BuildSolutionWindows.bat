@ECHO OFF
PUSHD ..\
CALL Scripts\premake5.exe --file=premake5.lua vs2022
IF %ERRORLEVEL% NEQ 0 ( PAUSE )
POPD