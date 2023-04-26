@echo off

set ROOT_DIR=%1
set PRINT_INFO=%2
set USER_VERSION_TXT=%ROOT_DIR%\prj\txt\UserVersion.txt
set FULL_VERSION_TXT=%ROOT_DIR%\prj\txt\FullVersion.txt
set ADX_VERSION_H_TXT=%ROOT_DIR%\prj\txt\AdxVersion.h.txt
set ADX_VERSION_H=%ROOT_DIR%\src\AntiDuplX\AdxVersion.h
set FIND_AND_REPLACE_CMD=%ROOT_DIR%\prj\cmd\FindAndReplace.cmd

if not "%PRINT_INFO%" == "0" ( echo Extract project version: )

set /p USER_VERSION=<%USER_VERSION_TXT%

if exist %FULL_VERSION_TXT% (
	set /p FULL_VERSION=<%FULL_VERSION_TXT%
) else (
	set FULL_VERSION=UNKNOWN
)

set LAST_VERSION=%FULL_VERSION%

where /Q git > nul
if not errorlevel 1 (
	git -C %ROOT_DIR% rev-parse 2>nul
	if not errorlevel 1 (
		git -C %ROOT_DIR% rev-parse --short HEAD>%FULL_VERSION_TXT%
		set /p GIT_REVISION=<%FULL_VERSION_TXT%
		git -C %ROOT_DIR% rev-parse --abbrev-ref HEAD>%FULL_VERSION_TXT%
		set /p GIT_BRANCH=<%FULL_VERSION_TXT%
	)
)

echo %USER_VERSION%>%FULL_VERSION_TXT%
if not "%GIT_BRANCH%" == "" if not "%GIT_REVISION%" == "" (	
	echo %USER_VERSION%.%GIT_BRANCH%-%GIT_REVISION%>%FULL_VERSION_TXT%
)

set /p FULL_VERSION=<%FULL_VERSION_TXT%

set NEED_TO_UPDATE=0
if not %LAST_VERSION% == %FULL_VERSION% (
	if not "%PRINT_INFO%" == "0" ( echo Last project version '%LAST_VERSION%' is not equal to current version '%FULL_VERSION%'. )
	set NEED_TO_UPDATE=1
) else (
	if not "%PRINT_INFO%" == "0" ( echo Last project version '%LAST_VERSION%' is equal to current version '%FULL_VERSION%'. )
)
if not exist %ADX_VERSION_H% set NEED_TO_UPDATE=1

if %NEED_TO_UPDATE% == 1 (
	if not "%PRINT_INFO%" == "0" ( echo Create or update file '%ADX_VERSION_H%'. )
	call %FIND_AND_REPLACE_CMD% @VERSION@ %FULL_VERSION% %ADX_VERSION_H_TXT%>%ADX_VERSION_H%
) else (
	if not "%PRINT_INFO%" == "0" ( echo Skip updating of file '%ADX_VERSION_H%' because there are not any changes. )
)

