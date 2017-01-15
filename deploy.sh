#!/usr/bin/env bash
INSTALLPATH="$HOME/Arduino/libraries/Joystick"

rm -r -d "$INSTALLPATH"/*
cp -r "Joystick" "$INSTALLPATH"
