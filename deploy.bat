@ECHO OFF
IF EXIST "%HOMEDRIVE%%HOMEPATH%\Documents\Arduino\libraries\Joystick\" rmdir /Q /S "%HOMEDRIVE%%HOMEPATH%\Documents\Arduino\libraries\Joystick\"
xcopy /E /I Joystick\*.* "%HOMEDRIVE%%HOMEPATH%\Documents\Arduino\libraries\Joystick"