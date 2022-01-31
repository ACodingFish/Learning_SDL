# Learning_SDL
A repo for learning SDL 2.0 in C

## Installation Instructions (Ubuntu)
* Clone this repo
* Install gcc and make
* Install SDL 2.0 - sudo apt-get install libsdl2-dev
* run make
* run application called ./Screen
* profit

## Current Features
* SDL Initialization
* Individual SDL Component Initialization and Cleanup
* Basic Drawing (Pixels, Rectangles, Lines)
* Threading (SDL Library)
* Graphics Update Thread (periodic refresh)
* Point mapping from one resolution to another with same aspect ratio (ex. 640x480 to 800x600)
  * Fixed size (Centered)
  * Scaled size (Stretch)
* Point mapping from one aspect ratio to (ex. 640x480 [4:3] to 1920x1080 [16:9])
  * Fixed Size (Centered)
  * Scaled Size (Stretched to smallest dimension to maintain Aspect Ratio)
  * Stretched Size (Stretched and Skewed)
* Generic, Thread Safe (probably) Queue Structure
* Graphics command processing and queueing (thread-safe to allow commands from other threads) GFX_SendCommand/GFX_ProcessCommand
  * Clear Screen to Black
  * Clear Canavas (Drawable Area) to Grey
  * Draw Pixels (Debug)
  * Scale Mode Switching (Centered, Scaled, Skewed/Stretched)

## Desired Features (Short Term)
* Event thread/event queue (SDL Library)
* Graphics Loading from file/buffer (SDL Library)
* Graphics Config Loading from file/buffer (SDL Library)
* Audio (SDL Library)
* Keyboard Input (SDL Library)
* Controller/Joystick Input (SDL Library)
* Probably More

## Desired Features (Long Term)
* Migrate to OpenGL
* Foreground/Background Rendering

## Resources
* https://libsdl.org/download-2.0.php
* https://wiki.libsdl.org/
* https://wiki.libsdl.org/Installation
* https://wiki.libsdl.org/Introduction
* https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md
* https://stackoverflow.com/questions/64396979/how-do-i-use-sdl2-in-my-programs-correctly
