@echo off

@REM set Targets=debug
@REM set DEPENDENCIES_PATH=%cd%\..\..
set Targets=%1
set DEPENDENCIES_PATH=%2
set DEPENDENCIES_PATH=%DEPENDENCIES_PATH:/=\%
echo %Targets%
echo %DEPENDENCIES_PATH%

set DESTDIR=%cd%\%Targets%

xcopy /E /D /Y /I %DEPENDENCIES_PATH%\geos\build\bin\%Targets%\*.dll %DESTDIR%
xcopy /E /D /Y /I %DEPENDENCIES_PATH%\libgit2\build\%Targets%\*.dll %DESTDIR%
xcopy /E /D /Y /I %DEPENDENCIES_PATH%\librdkafka\build\src\%Targets%\*.dll %DESTDIR%
xcopy /E /D /Y /I %DEPENDENCIES_PATH%\librdkafka\build\src-cpp\%Targets%\*.dll %DESTDIR%
