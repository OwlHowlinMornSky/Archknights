mkdir publish
mkdir publish\release-x64

copy ..\x64\release\Archknights.exe /Y publish\release-x64\
copy ..\x64\release\MysteryEngine.dll /Y publish\release-x64\
copy ..\x64\release\Client.dll /Y publish\release-x64\

copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\openal32.dll /Y publish\release-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-audio-2.dll /Y publish\release-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-graphics-2.dll /Y publish\release-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-window-2.dll /Y publish\release-x64\
copy %OHMS_LIB_DIR%\sfml\2.6.1\vc17-x64\bin\sfml-system-2.dll /Y publish\release-x64\

copy %OHMS_LIB_DIR%\opengl\glew-2.2.0\bin\Release\x64\glew32.dll /Y publish\release-x64\

copy "assets\GNU AGPLv3.txt" /Y publish\release-x64\
copy assets\LICENSE.txt /Y publish\release-x64\
copy assets\README.txt /Y publish\release-x64\

pause
