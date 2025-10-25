@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"
title Git Branch Manager
color 0A

:MAIN_MENU
cls
echo ==================================================
echo                 Git Branch Manager
echo ==================================================
echo.
echo 1. Create multiple local branches
echo 2. Create and push branches to remote
echo 3. Delete local branches
echo 4. Delete remote branches
echo 5. Show branch info
echo 6. Exit
echo.
set /p choice=Select an option (1-6): 

if "%choice%"=="1" goto CREATE_LOCAL
if "%choice%"=="2" goto CREATE_AND_PUSH
if "%choice%"=="3" goto DELETE_LOCAL
if "%choice%"=="4" goto DELETE_REMOTE
if "%choice%"=="5" goto SHOW_BRANCHES
if "%choice%"=="6" goto EXIT

echo Invalid option.
timeout /t 2 >nul
goto MAIN_MENU

:CHECK_ENV
where git >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Git not found. Please install Git and add it to PATH.
    pause
    exit /b
)

git rev-parse --is-inside-work-tree >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Not inside a Git repository.
    pause
    exit /b
)

set remote=
for /f "delims=" %%R in ('git remote 2^>nul') do (
    if "!remote!"=="" set remote=%%R
)
if "!remote!"=="" (
    set has_remote=false
) else (
    set has_remote=true
)
exit /b

:CREATE_LOCAL
call :CHECK_ENV
echo.
set /p branches=Enter branch names (separated by spaces): 
if "%branches%"=="" (
    echo [ERROR] No branch names entered.
    pause
    goto MAIN_MENU
)
for %%b in (!branches!) do (
    echo Creating branch: %%b ...
    git branch %%b
)
echo All local branches created.
pause
goto MAIN_MENU

:CREATE_AND_PUSH
call :CHECK_ENV
if "!has_remote!"=="false" (
    echo [ERROR] No remote repository detected.
    pause
    goto MAIN_MENU
)
echo.
set /p branches=Enter branch names (separated by spaces): 
if "%branches%"=="" (
    echo [ERROR] No branch names entered.
    pause
    goto MAIN_MENU
)
set /p confirm=Push to remote (Y/N): 
if /i "%confirm%"=="Y" (
    for %%b in (!branches!) do (
        echo Creating and pushing branch: %%b ...
        git branch %%b
        git push !remote! %%b
    )
    echo All branches pushed to remote.
) else (
    for %%b in (!branches!) do (
        echo Creating local branch: %%b ...
        git branch %%b
    )
    echo Branches created locally only.
)
pause
goto MAIN_MENU

:DELETE_LOCAL
call :CHECK_ENV
echo.
set /p branches=Enter local branch names to delete: 
if "%branches%"=="" (
    echo [ERROR] No branch names entered.
    pause
    goto MAIN_MENU
)
for %%b in (!branches!) do (
    echo Deleting local branch: %%b ...
    git branch -D %%b
)
echo Done.
pause
goto MAIN_MENU

:DELETE_REMOTE
call :CHECK_ENV
if "!has_remote!"=="false" (
    echo [ERROR] No remote repository detected.
    pause
    goto MAIN_MENU
)
echo.
set /p branches=Enter remote branch names to delete: 
if "%branches%"=="" (
    echo [ERROR] No branch names entered.
    pause
    goto MAIN_MENU
)
for %%b in (!branches!) do (
    echo Deleting remote branch: %%b ...
    git push !remote! --delete %%b
)
echo Done.
pause
goto MAIN_MENU

:SHOW_BRANCHES
call :CHECK_ENV
echo.
echo ==== Local branches ====
git branch
echo.
echo ==== Remote branches ====
git branch -r
echo.
pause
goto MAIN_MENU

:EXIT
echo.
echo Goodbye!
timeout /t 1 >nul
exit
