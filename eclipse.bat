::-----------------------------------------------------------------------------
:: Paths
::-----------------------------------------------------------------------------
set SCRIPT_DIR=%~dp0%
set ProjectHome=%SCRIPT_DIR:~0,-1%
set WorkspaceDir=%ProjectHome%\workspace
::set ThisProjTools=%ProjectHome%\tools
set ThisProjTools=C:\tools

:: Run the workbench
start %ThisProjTools%\eclipse-luna\eclipse.exe -data %WorkspaceDir%
