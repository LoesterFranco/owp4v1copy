@echo off

rem Path configuration
rem ==================
set OWROOT=\OW
set WWWPATH=\www
set ARCH7Z=c:\Program Files\7-Zip\7z
set RM=%OWROOT%\build\bin\rm
set MD5=md5sum

rem Initialization
rem ==============

if exist %OWROOT%\pass1 goto prerequisite_ok
echo Missing %OWROOT%\pass1. Can't continue with rotation.
goto done

:prerequisite_ok

rem Build Archives
rem ==============
if exist %WWWPATH%\snapshots\ss.zip %RM% -f %WWWPATH%\snapshots\ss.zip
if exist %WWWPATH%\snapshots\ss.7z %RM% -f %WWWPATH%\snapshots\ss.7z
"%ARCH7Z%" a -tzip -r %WWWPATH%\snapshots\ss.zip %OWROOT%\pass1\*
"%ARCH7Z%" a -t7z -r %WWWPATH%\snapshots\ss.7z %OWROOT%\pass1\*

rem Move pass1 build
rem =================
if exist %WWWPATH%\snapshot move %WWWPATH%\snapshot %WWWPATH%\snapshot.bak
if exist %WWWPATH%\snapshot goto done
move %OWROOT%\pass1 %WWWPATH%\snapshot

rem Move Archives and calc md5
rem ==========================
if exist %WWWPATH%\snapshots\ow-snapshot.zip %RM% -f %WWWPATH%\snapshots\ow-snapshot.zip
if exist %WWWPATH%\snapshots\ow-snapshot.7z %RM% -f %WWWPATH%\snapshots\ow-snapshot.7z
move %WWWPATH%\snapshots\ss.zip %WWWPATH%\snapshots\ow-snapshot.zip
move %WWWPATH%\snapshots\ss.7z %WWWPATH%\snapshots\ow-snapshot.7z
cd %WWWPATH%\snapshots
"%MD5%" ow-snapshot.zip >ow-snapshot.zip.md5
"%MD5%" ow-snapshot.7z >ow-snapshot.7z.md5

rem Move installers and calc md5
rem ============================
if exist %WWWPATH%\installers\open-watcom-*.* %RM% -f %WWWPATH%\installers\open-watcom-*.*
move %OWROOT%\distrib\ow\open-watcom-*.* %WWWPATH%\installers\
cd %WWWPATH%/installers
if not exist md5 md md5
"%RM%" -f open-watcom-*.md5
"%RM%" -f md5\*.md5
for %%f in (open-watcom-*) do %MD5% %%f >md5\%%f.md5
move md5\*.*  .
rd md5

rem Final Cleanup
rem =============
%RM% -rf %WWWPATH%\snapshot.bak

:done
