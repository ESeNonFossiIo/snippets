import serial
import time
print "Starting....."
arduino = serial.Serial('/dev/cu.usbmodem1411', 9600, timeout=0)
while True:
          print arduino.readline()
