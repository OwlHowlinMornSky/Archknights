mkdir publish
mkdir publish\debug-x64

copy ..\x64\debug\Archknights.exe /Y publish\debug-x64\

copy %OHMS_LIB_DIR%\sfml\2.6.0\vc17-x64\bin\openal32.dll /Y publish\debug-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.0\vc17-x64\bin\sfml-audio-d-2.dll /Y publish\debug-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.0\vc17-x64\bin\sfml-graphics-d-2.dll /Y publish\debug-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.0\vc17-x64\bin\sfml-window-d-2.dll /Y publish\debug-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.0\vc17-x64\bin\sfml-system-d-2.dll /Y publish\debug-x64\

copy %OHMS_LIB_DIR%\opengl\glew-2.2.0\bin\Release\x64\glew32.dll /Y publish\debug-x64\

copy publish\COPYING /Y publish\debug-x64\
copy publish\LICENSE /Y publish\debug-x64\

pause
