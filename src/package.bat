@ECHO OFF

REM 
REM package.bat
REM by Keith Gaughan <hereticmessiah@users.sourceforge.net>
REM 
REM A short batch script for building a distributable archive on Windows.
REM 
REM This file is in the Public Domain.
REM 
SET ARCHIVE=blogping.zip
IF EXIST %ARCHIVE% DEL /Q %ARCHIVE%
7z a %ARCHIVE% -tzip -r -x!.\*.zip -x!.\package.bat -x!*~ -mx=9 -- .
