@echo off
setlocal

if "%~1"=="" (
    echo Uso: %~nx0 caminho\para\arquivo.c
    exit /b 1
)

where gcc >nul 2>nul
if errorlevel 1 (
    echo Erro: GCC nao encontrado. Instale um compilador C e configure-o no PATH.
    exit /b 1
)

if not exist "%~1" (
    echo Erro: arquivo nao encontrado: %~1
    exit /b 1
)

set "BUILD_DIR=%~dp0build"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

for %%F in ("%~1") do set "OUTPUT=%BUILD_DIR%\%%~nF.exe"

gcc "%~1" -o "%OUTPUT%"
if errorlevel 1 exit /b 1

"%OUTPUT%"
exit /b %errorlevel%
