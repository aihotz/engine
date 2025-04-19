@echo off
setlocal

if exist build (
    rmdir /s /q build
)

call generate.bat

endlocal