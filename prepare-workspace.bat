@echo off
::-----------------------------------------------------------------------------
:: Paths
::-----------------------------------------------------------------------------
set SCRIPT_DIR=%~dp0%
set ProjectHome=%SCRIPT_DIR:~0,-1%
set WorkspaceDir=%ProjectHome%\workspace
set ThisProjTools=C:\tools

::-----------------------------------------------------------------------------
:: Load Script Environment / Configuration
::-----------------------------------------------------------------------------
for /f "delims=" %%x in (%ProjectHome%\env.config) do (set "%%x")

::-----------------------------------------------------------------------------
:: Configured Settings
::-----------------------------------------------------------------------------

:: Expected Project Location (Eclipse CDT cannot deal with relative paths)
:: set TestProjects=C:\git\test-projects ::to be set in env.config
set ExpectedProjectHome=%TestProjects%\%ProjectSubDir%

::-----------------------------------------------------------------------------
:: Get the OS Variant
::-----------------------------------------------------------------------------
IF "%PROCESSOR_ARCHITECTURE%;%PROCESSOR_ARCHITEW6432%"=="x86;" (
  set OsVariant=win32
) ELSE (
  set OsVariant=win64
)

::-----------------------------------------------------------------------------
:: Assert correct path
::-----------------------------------------------------------------------------
if not "%ExpectedProjectHome%"=="%ProjectHome%" (
  msg "%username%" Please install this project %ProjectHome% here: %ExpectedProjectHome% - otherwise you would not be able to compile.
  if not exist "%TestProjects%" (
    md "%TestProjects%"
  )
  goto end
)


::-----------------------------------------------------------------------------
:: Set the tools' paths
::-----------------------------------------------------------------------------
:: 7Zip
set Archiver=%ThisProjTools%\7za920\7za.exe

:: wget
set Wget=%ThisProjTools%\wget\wget.exe

:: git
if "%OsVariant%"=="win32" (
  set Git="%ProgramFiles%\Git\bin\git.exe"
) else (
  set Git="%ProgramFiles(x86)%\Git\bin\git.exe"
)

::-----------------------------------------------------------------------------
:: unpack Eclipse metadata in workspace
::-----------------------------------------------------------------------------
if not exist "%WorkspaceDir%\.metadata" (
  %Archiver% x -y -o%WorkspaceDir% "%WorkspaceDir%\.metadata.zip"
)

::-----------------------------------------------------------------------------
:: run build
::-----------------------------------------------------------------------------
call build.bat
if %errorlevel% == 0 goto end

:error
msg "%username%" An error occured!
::pause

:end
