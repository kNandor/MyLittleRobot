#!/usr/bin/env python
import serial
import sys
import time
import datetime

if len(sys.argv)!=2:
	print ("Too much or too few argument!")
	sys.exit()
#RECEIVER PROGRAM
arduino=serial.Serial('/dev/ttyACM0', baudrate=921600, timeout=3.0)
print (sys.argv[1])
mod=str(sys.argv[1])
datastr='{:%Y%m%d_%H%M}'.format(datetime.datetime.now())
fileout=open('./DataAccel/Data_'+mod+'.csv','w')

start_time=time.time()

startSent=False
endSent=False
arduino.flushInput()
try:
	while True:
		if arduino.inWaiting():
			i=arduino.read()
			fileout.write(i)
			sys.stdout.write(i)
		#
		#current_time=time.time()
		#if current_time-start_time>1.0 and not startSent:
		#	arduino.write(bytes(chr(1)))
		#	startSent=True
		#if current_time-start_time>10.0 and not endSent:
		#	arduino.write(bytes(chr(0)))
		#	endSent=True


except KeyboardInterrupt:
	pass

arduino.close()
fileout.close()
