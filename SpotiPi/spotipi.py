#!/usr/bin/python

from LCD import LCD
from subprocess import * 
from time import sleep, strftime
from datetime import datetime
import RPi.GPIO as GPIO
import urllib3

GPIO.setwarnings(0)
lcd = LCD()
lcd.begin(16,1)

ip = "192.168.1.7"
http = urllib3.PoolManager()

while 1:
	lcd.clear()
	try:
		r = http.request('GET', 'http://'+ip+":5555/")
		strx = r.data.split('\n')
		artist = strx[0]
		songname = strx[1]
		lcd.message(songname + "\n")
		lcd.message(artist)
		if len(songname) > 16 or len(artist) > 16:
			if len(songname) > len(artist):
				lenc = len(songname)
			else: 
				lenc = len(artist)
			count = 0
			sleep(2)
			while count < lenc - 16:
				lcd.DisplayLeft()
				count += 1
				sleep(0.5)
	except Exception as inst:
		lcd.message("Error\n")
		lcd.message("Cannot Connect.")
	sleep(3)
