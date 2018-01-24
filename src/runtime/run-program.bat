@echo off

::
:: Helper script to run a bundled/distributable application.
::

setlocal
set SCRIPT_DIR=%~dp0
set PATH=%SCRIPT_DIR%\lib;%PATH%

%*
