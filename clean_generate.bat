@echo off
setlocal

if exist build (
	if exist build\CMakeCache.txt copy build\CMakeCache.txt CMakeCache.txt.backup
	if exist build\CMakeFiles xcopy /e /i build\CMakeFiles CMakeFiles_backup

    rmdir /s /q build
	mkdir build
	
	move /y CMakeCache.txt.backup build\CMakeCache.txt
	xcopy /e /i CMakeFiles_backup build\CMakeFiles
	rmdir /s /q CMakeFiles_backup
)

call generate.bat

endlocal