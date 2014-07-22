SpotiPi
=======

A small script to show your currently playing song on Spotify via a 16x2 LCD screen connected to your RPI.

Preview: 

![Screen](http://i.imgur.com/aAUOvfc.png)

To use:

Your raspberry Pi will have to be wired as such:
![Wiring](https://learn.adafruit.com/system/assets/assets/000/001/729/medium800/raspberry_pi_pi-char-lcd.gif?1396775803)
You'll need URLLib3 on your Raspberry PI, as well as RPI.GPIO from the python libraries.

Just run it on your PC first, make sure it's listening, and then edit the IP in spotipi.py.
You can then run "python spotipi.py". You will see your currently playing song from your PC on your 16x2 LCD.
