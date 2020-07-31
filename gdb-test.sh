#!/usr/bin/env bash

sudo echo "Starting..."

/usr/lib/x86_64-linux-gnu/libexec/kscreenlocker_greet --testing &
sudo gdb /usr/lib/x86_64-linux-gnu/libexec/kscreenlocker_greet $!
