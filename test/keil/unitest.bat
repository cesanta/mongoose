cd %*
start /WAIT /B C:\Keil_v5\UV4\UV4.exe -cr device-dashboard.uvprojx -j0 -o output.txt
if %ERRORLEVEL% NEQ 0 (
	type output.txt
	exit /B %ERRORLEVEL%
)
del output.txt
C:\Keil_v5\UV4\UV4.exe -c device-dashboard.uvprojx -j0
