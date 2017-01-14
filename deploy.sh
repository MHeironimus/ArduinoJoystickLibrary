#!/usr/bin/env bash
INSTALLPATH="$HOME/Arduino/libraries/Joystick"

if [ ! -d "$INSTALLPATH" ]; then
    mkdir "$INSTALLPATH"
fi

if [ ! -d "$INSTALLPATH/examples" ]; then
     mkdir "$INSTALLPATH/examples"
fi

cp "Joystick/"* "$INSTALLPATH"
cp "Joystick/src/"* "$INSTALLPATH"
cp -r "Joystick/examples" "$INSTALLPATH"
