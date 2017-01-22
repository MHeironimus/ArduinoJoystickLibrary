#!/usr/bin/env bash
INSTALLPATH="$HOME/Arduino/libraries/Joystick"

rm -r -d -f "$INSTALLPATH"
cp -r "Joystick" "$INSTALLPATH"
