mkdir publish
mkdir publish\limde-x64

copy ..\x64\lim-de\Archknights-d.exe /Y publish\limde-x64\
copy ..\x64\lim-de\MysteryEngine.dll /Y publish\limde-x64\
copy ..\x64\lim-de\Client.dll /Y publish\limde-x64\

copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\openal32.dll /Y publish\limde-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-audio-d-2.dll /Y publish\limde-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-graphics-d-2.dll /Y publish\limde-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-window-d-2.dll /Y publish\limde-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-system-d-2.dll /Y publish\limde-x64\

copy %OHMS_LIB_DIR%\opengl\glew-2.2.0\bin\Release\x64\glew32.dll /Y publish\limde-x64\

copy "assets\GNU AGPLv3.txt" /Y publish\limde-x64\
copy assets\LICENSE.txt /Y publish\limde-x64\
copy assets\README.txt /Y publish\limde-x64\

pause
