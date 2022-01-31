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
* Graphics command processing and queueing (thread-safe to allow commands from other threads)
  * Clear Screen to Black
  * Clear Canavas (Drawable Area) to Grey
  * Draw Pixels (Debug)
  * Scale Mode Switching (Centered, Scaled, Skewed/Stretched)

## Desired Features (Short Term)
* Event thread/event queue (SDL Library)
* Graphics Config Loading from file/buffer (SDL Library)
  * Display Detection - scr_w/scr_h (For first launch) - Possibly do this elsewhere (script, gui, etc)
* Graphics Loading from file/buffer (SDL Library)
  * Store as (x, y, color (32 bit - r,g,b,a) (CSV - Library or Write)
    * x and y are in original resolution - orig_w, orig_h
  * Clear file on Screen/Canvas Clear
  * Graphics Update Command
    * Lock Mutex
    * Read in file to buffer in ram
    * Unlock Mutex
    * Update pixels from buffer
  * Screen Size Change Command
    * Update pixels from buffer
  * Pixel Change/Animation
    * Lock Mutex
    * Write Pixels to File
    * Unlock Mutex
    * Send Graphics Update Command
* Keyboard Input (SDL Library)
* Controller/Joystick Input (SDL Library)
* Probably More

## Desired Features (Long Term)
* Migrate to OpenGL
* Audio (SDL Library)
* Foreground/Background Rendering

## Resources
* https://libsdl.org/download-2.0.php
* https://wiki.libsdl.org/
* https://wiki.libsdl.org/Installation
* https://wiki.libsdl.org/Introduction
* https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md
* https://stackoverflow.com/questions/64396979/how-do-i-use-sdl2-in-my-programs-correctly
