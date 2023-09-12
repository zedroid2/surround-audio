# Surround audio app

this program is supposed to simulate 3D audio from a set of headphones.
please make sure to have both headphones in.

-> In order to run these, you will need the .exe file, and the sofa-files folder, so your filesystem would look like this:
- program folder
	- main.exe
	- guitartune.wav
	- sofa-files
		-MRT01.sofa

main.exe provides a 3D environment, where the camera spans around a cube - in this case the red cube is the listener, the blue cube is the front of the listener, and the audio comes from the camera's pov.
To exit, press escape.

test.exe provides a constant anticlockwise rotation of the sound.

## Building

If you don't trust me and want to build these yourself, you'll need the following dependencies:
- raylib (main.exe only)
- libmysofa
- portaudio
- libasound2 (linux only)
	- libasound2-dev
	- alsa-tools
	- alsamixer


## Windows Installation
For windows, i'd recommend installing msys2, setting it up, and using its gcc compiler
Once you open msys2:
````console
pacman -S mingw-w64-x86_64-gcc
````









 
