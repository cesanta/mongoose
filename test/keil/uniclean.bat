cd %*
C:\Keil_v5\UV4\UV4.exe -c device-dashboard.uvprojx -j0
del /S /Q DebugConfig Listings Objects
rd /S /Q DebugConfig
rd /S /Q Listings
rd /S /Q Objects
