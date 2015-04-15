::-----------------------------------------------------------------------------
:: Paths
::-----------------------------------------------------------------------------
set SCRIPT_DIR=%~dp0%
set ProjectHome=%SCRIPT_DIR:~0,-1%
set WorkspaceDir=%ProjectHome%\workspace

::-----------------------------------------------------------------------------
:: Load Script Environment / Configuration
::-----------------------------------------------------------------------------
for /f "delims=" %%x in (%ProjectHome%\env.config) do (set "%%x")

::-----------------------------------------------------------------------------
:: Run the workbench
::-----------------------------------------------------------------------------
start %EclipseExe% -data %WorkspaceDir%