#!/usr/bin/python

from LCD import LCD
from subprocess import *
from time import sleep, strftime
from datetime import datetime
from threading import Thread
import RPi.GPIO as GPIO
import urllib3
import threading
import signal 
import os
import sys

GPIO.setwarnings(0)
lcd = LCD()
lcd.begin(16,1)

class SpotiPi:
	keeprunning = True
	oldsong = ""
	songname = ""
	artist = ""
	ip = "192.168.1.4"
	http = urllib3.PoolManager()
	printit = 0
	lock = threading.RLock()
	def printSong(self):
		while(1):
			if self.keeprunning == True:
				with self.lock:
					yolo = self.printit
				if yolo == 1:
					lcd.clear()
					#print "Printing", self.songname, "-", self.artist
					lcd.message(self.songname + "\n")
					lcd.message(self.artist)
					self.oldsong = self.songname + "\n" + self.artist
					if len(self.songname) > 16 or len(self.artist) > 16:
							if len(self.songname) > len(self.artist):
									lenc = len(self.songname)
							else:
									lenc = len(self.artist)
							count = 0
							sleep(5)
							while count < lenc - 16:
								#print self.oldsong, "-", self.artist, "/", self.songname
								if not self.songname + "\n" + self.artist == self.oldsong:
									break
								lcd.DisplayLeft()
								count += 1
								sleep(0.5)
					sleep(1)
				else:
					sleep(1)

	def GrabSong(self):
		while(1):
			if self.keeprunning == True:
				try:
					r = self.http.request('GET', 'http://'+self.ip+":5555/")
					strx = r.data.split('\n')
					self.artist = strx[0]
					self.songname = strx[1]
					if self.songname == "Not Playing.":
						with self.lock:
							self.printit = 0
						lcd.clear()
						lcd.message("Not Playing.")
						self.songname = ""
					else:
						with self.lock:
							self.printit = 1
							#print "Set songname and artist to ", self.artist, "-", self.songname
					sleep(1)
				except Exception as inst:
					with self.lock:
						self.printit = 0
					self.songname = ""
					self.artist = ""
					lcd.clear()
					lcd.message("Error\n")
					lcd.message("Cannot Connect.")
					sleep(2)
threads = []

spotipi = SpotiPi()

try:
	print "Starting SpotiPi..."
	thread1 = Thread(target=spotipi.GrabSong)
	#thread1.daemon = True
	thread1.start()
	threads.append(thread1)
	thread = Thread(target=spotipi.printSong)
	#thread.daemon = True
	thread.start()
	threads.append(thread)
	print "SpotiPi Daemon Started!"
	while len(threads) > 0:
		try:
			# Join all threads using a timeout so it doesn't block
			# Filter out threads which have been joined or are None
			threads = [t.join(1) for t in threads if t is not None and t.isAlive()]
		except KeyboardInterrupt:
			print "Killing process..."
			for t in threads:
				t.keeprunning = False

except Exception as detail:
	print "Error: unable to start threads: ", detail
