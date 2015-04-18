@echo off
::-----------------------------------------------------------------------------
:: Paths
::-----------------------------------------------------------------------------
set SCRIPT_DIR=%~dp0%
set ProjectHome=%SCRIPT_DIR:~0,-1%
set WorkspaceDir=%ProjectHome%\workspace
set TestTools=%ProjectHome%\..\Tools
set ThisProjTools=%ProjectHome%\tools

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
  set OsVariant2=win32
) ELSE (
  set OsVariant=win64
  set OsVariant2=win32-x86_64
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
:: Eclipse Workbench
set EclipseRevs=%TestTools%\eclipse_revs
set CurEclipse=%EclipseRevs%\%EclipseVer%-%OsVariant2%

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
:: Assert untouched .project file
::-----------------------------------------------------------------------------
:: ensure that src/.project has not been changed
set statusResult=
for /f "delims=" %%a in ('%%Git%% status --porcelain %%ProjectHome%%\src\.project') do @set statusResult=%%a
echo "%statusResult%"
if not "%statusResult%"=="" (
  msg "%username%" The file %ProjectHome%\src\.project is already touched. This script shall only be run on a vanilla project just cloned before.
  goto end
)

::-----------------------------------------------------------------------------
:: Eclipse Workbench
::-----------------------------------------------------------------------------
if not exist "%CurEclipse%" (
  if not exist "%EclipseRevs%" (
    md "%EclipseRevs%"
  )
  md "%CurEclipse%"
  
  if not exist "%EclipseRevs%\%EclipseVer%-%OsVariant2%.zip" (
    %Wget% --tries=0 --output-document="%EclipseRevs%\%EclipseVer%-%OsVariant2%.zip" "%EclipseDownloadUrl%/%EclipseVer%-%OsVariant2%.zip"
  )
  %Archiver% x -y -o%CurEclipse% %EclipseRevs%\%EclipseVer%-%OsVariant2%.zip
  if %errorlevel% == 0 (
    del %EclipseRevs%\%EclipseVer%-%OsVariant2%.zip
  )
)
:: create softlink (junction) for Eclipse Workbench in current project tools
rmdir %ThisProjTools%\eclipse
mklink /J %ThisProjTools%\eclipse %CurEclipse%\eclipse

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
