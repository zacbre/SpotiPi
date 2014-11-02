SpotiPi
=======

A small script to show your currently playing song on Spotify via a 16x2 LCD screen connected to your RPI.

Note, I'm not great with python. It's terrible looking code and could definitely be optimized. I wrote it to
my best knowledge with python.

Preview: 

![Screen](http://i.imgur.com/aAUOvfc.png)

To use:

Your raspberry Pi will have to be wired as such:
![Wiring](https://learn.adafruit.com/system/assets/assets/000/001/729/medium800/raspberry_pi_pi-char-lcd.gif?1396775803)
You'll need URLLib3 on your Raspberry PI, as well as RPI.GPIO from the python libraries.

Windows instructions:
Compile with Visual Studio, then run SpotiPi_Windows on your PC.
On your raspberry pi, you'll need to edit the IP in spotipi.py.
You can then run "python spotipi.py". You will see your currently playing song from Spotify on your 16x2 LCD.

Linux instructions:

To Compile the SpotiPi Linux version, you'll need QTCreator and LibCURL installed.
LibCURL: http://curl.haxx.se/docs/install.html
QtCreator: http://qt-project.org/downloads

You'll also need to grab the version of Spotify for Linux.
https://www.spotify.com/us/download/previews/

Follow the instructions, then compile SpotiPi_Linux in QtCreator.

Once you're done, in the build directory run:

```
./spotipi 4381
```

On your raspberry pi, you'll need to edit the IP in spotipi.py.
You can then run "python spotipi.py". You will see your currently playing song from Spotify on your 16x2 LCD.