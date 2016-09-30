# Pedro Ferrusca A01203776 pedro.ferrusca2b@gmail.com
# Codigo en python para hacer el control de la trayectorias y
# la communicacion con las bases de datos para el product mover
import serial #Usamos la libreria serial para comunicarnos con el Arduino
from time import sleep #funcion que actua como delay
import numpy as np
import socket

SERVER_IP = '10.12.174.85'
TCP_PORT = 50008
BUF_SIZE = 1024

pCOM = serial.Serial("/dev/ttyACM1", 9600, timeout=3)
sleep(3)
#map = [[2, 3, 3, 3, 2], [3, 4, 4, 4, 3], [3, 4, 4, 4, 3],[3, 4, 4, 4, 3],[2, 3, 3, 3, 2]]
wR = " NOSE"
class Map:
	def __init__(self, rows, columns):
		if (rows > 1) and (columns > 1):
			self.rows = rows
			self.columns = columns 
			self.matrix = np.zeros(rows, columns)
			for j in Range(0, rows):
				pass
		else:
			return None 
class Trayectoria:
	def __init__(self, sp, fp):
		sIn = sp.index(',')
		fIn = fp.index(',')
		if (sIn > 0) and (fIn > 0):
			if (len(sp) == 3) and (sIn == 1):
				self.sp = sp
				self.spx = ord(sp[0])-48
				self.spy = ord(sp[2])-48
				print "spx: ", self.spx
				print "spy: ", self.spy
			else:
				self.sp = sp
				self.spx = int(sp[:(sIn -1)])
				self.spy = int(sp[sIn+1:])
				print "spx: ", self.spx
				print "spy: ", self.spy
			if (len(sp) == 3) and (sIn == 1):
				self.fp = fp
				self.fpx = ord(fp[0])-48
				self.fpy = ord(fp[2])-48
				print "fpx: ", self.fpx
				print "fpy: ", self.fpy
			else:
				self.fp = fp
				self.fpx = int(fp[:(fIn-1)])
				self.fpy = int(fp[fIn+1:])	
				print "fpx: ", self.fpx
				print "fpy: ", self.fpy
#				self.fpx = self.fpy = self.spx = self.spy = 0
				#it = [len(faux1), len(faux2), len(saux1), len(saux2)]
				#for j in Range(0, len[faux1])
				#	self.fpx += (ord(faux1[j])-48)*(10**j)
				#for j in Range(0, len[faux2])
				#	num.fpy += (ord(faux2[j])-48)*(10**j)
				#for j in Range(0
				
		else:
			print "No trajectory made"
			return None
	def getCurrentPos(self):
		return self.sp
	def getNextPos(self):
		return self.fp
	def getDirection(self):
		dx = self.fpx - self.spx
		dy = self.fpy - self.spy
		if dy != 0:
			return dy + 2
		elif dx != 0: 
			return dx + 3
		return 0
	def setNextPos(self, nextPos):
		if (len(nextPos) >=3) and (nextPos.index(',') > 1) and (nextPos.index(',') < len(nextPos)-2):
			self.fp = nextPos
			self.fpx = int(self.fp[:(nextPos.index(',')-1)])
			self.fpy = int(self.fp[(nextPos.index(',')+1):])
		else:
			print "Incorrect trajectory"
	def setCurrentPos(self, currentPos):
		if (currentPos.index(',') >=0) and (currentPos.index(',') < len(currentPos)-2) and (len(currentPos) >= 3):
			self.sp = currentPos
			self.spx = int(currentPos[:currentPos.index(',')-1])
			self.spy = int(currentPos[currentPos.index(',')+1:])
		else:
			print "Incorrect trajectory"	

class ComandoControl: # Clase comando para la comunicacion Control-Rasp
	def __init__(self, serverIp, tcpPort, bufSize):
		self.serverIp = serverIp
		self.tcpPort = tcpPort
		self.bufSize = bufSize
		while True:
			try:
				self.conexionControl = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				self.conexionControl.connect((self.serverIp, self.tcpPort))
				break
			except:
				print "Timeout! Retry"
	def __del__(self):
		self.conexionControl.close()
	def send(self, data):
		print data
		self.conexionControl.send(data)

	def read(self):	
		return self.conexionControl.recv(self.bufSize)
	

class ComandoArduino: # Clase comando para indicar los comandos neesarios del robot
	
	def __init__(self, code, times, serial1):
		self.code = code
		self.times = times
		self.serialDev = serial1
		print "command created"  #quitar primer comentario para debuggear
	def send(self):
		print "send" #DEBUG
		self.serialDev.flushInput()
		self.serialDev.flushOutput()
		for i in range(0, self.times):
			self.serialDev.write(chr(self.code))
			sleep(0.5)
			print "Sent : ", self.code
			res = 0
			while res != chr(self.code):
				while self.serialDev.inWaiting() <= 0:
				#	print self.serialDev.inWaiting()
					pass
				res = self.serialDev.read(1)
				print res, " OK"
				if(res != chr(self.code)):
					self.serialDev.write(chr(self.code))
				return res
	def setTimes(self, times):
		self.times = times
class Robot:
	
	def __init__(self, number, currentPos, nextPos, serialDev, serverIp, tcpPort, bufSize):
		self.number = number
		self.currentPos = currentPos
		self.nextPos = nextPos
		self.tray = Trayectoria(currentPos, nextPos)
		self.orientation = 1
		self.serialDev = serialDev
		self.serialDev.flushInput()
		self.serialDev.flushOutput()
		self.__giroDerecha__ = ComandoArduino(70, 0, serialDev) 
		self.__giroIzquierda__ = ComandoArduino(71, 0, serialDev) 
		self.__avanza__ = ComandoArduino(72, 0, serialDev) 
		self.__subirActuador__ = ComandoArduino(73, 0, serialDev) ###print "success" 
		self.__bajarActuador__ = ComandoArduino(74, 0, serialDev)
		self.__encendido__ = ComandoArduino(83, 1, serialDev)
		self.__encendido__.send()
		self.__conexionControl__ = ComandoControl(serverIp, tcpPort, bufSize);
		self.__conexionControl__.send(currentPos);
	def getNumber(self):
		return self.number
	def getCurrentPos(self):
		return self.currentPos
	def getNextPos(self):
		#print 'position update'
		return self.nextPos
	def setNextPos(self, nextPos):
		self.nextPos = nextPos
		self.tray = Trayectoria(self.currentPos, nextPos)
	def setCurrentPos(self, currentPos):
		self.currentPos = currentPos
		self.tray = Trayectoria(currentPos, self.nextPos) 
	def forward(self, times):
		self.__avanza__.setTimes(times)
		self.__avanza__.send()
	def turnR(self):
		self.__giroDerecha__.setTimes(1)
		self.__giroDerecha__.send()
		if self.orientation == 1:
			self.orientation += 3
		else:
			self.orientation -= 1
	def turnL(self):
		self.__giroIzquierda__.setTimes(1)
		self.__giroIzquierda__.send()
		if self.orientation == 4:
			self.orientation -= 3
		else:
			self.orientation += 1
	def turnAll(self):
		self.__giroDerecha__.setTimes(2)
		self.__giroDerecha__.send()
		if self.orientation <= 2:
			self.orientation += 2
		else: 
			self.orientation -= 2
	def changePosition(self):
		print "Current Position: ", self.tray.getCurrentPos()
		print "Next Position: ", self.tray.getNextPos()
		direction = self.tray.getDirection() - self.orientation
		if self.tray.getDirection() == 0:
			print "Final point same as Start point"
			#return #enviar comando a central 
		else:
			print "different trajectory ", direction
			if direction == 0:
				self.forward(1)
				#self.__avanza__.setTimes(1)
				#self.__avanza__.send()
				#return
			elif (direction == -1) or (direction == 3): 
				self.turnL()
				self.forward(1)
				#self.__giroIzquierda__.setTimes(1)
				#self.__giroIzquierda__.send()
				#return
			elif abs(direction) == 2:
				self.turnAll()
				self.forward(1)
				#self.__giroDerecha__.setTimes(2)
				#self.__avanza__.setTimes(1)
				#self.__giroDerecha__.send()
				#self.__avanza__.send()
			else:
				self.turnR()
				self.forward(1)
				#self.__giroDerecha__.setTimes(1)
				#self.__giroDerecha__.send()
		#self.setOrientation
		self.setCurrentPos(self.nextPos)
		
	#def getOrientation(self):
		#return 
mover1 = Robot(1, "5,1", "5,1", pCOM, SERVER_IP, TCP_PORT, BUF_SIZE)
#mover1.forward(3)
#mover1.turnL()
#mover1.turnR()
#mover1.turnAll()
#mover1.setNextPos("33,32")
#print mover1.getOrientation(), mover1.getCurrentPos()
#mover1.changePosition()
#print mover1.getOrientation(), mover1.getCurrentPos()
while True:
	current = mover1.getCurrentPos()
	print 'Current pos', current
	mover1.__conexionControl__.send(current)
	dato = mover1.__conexionControl__.read()
	print 'dato recibido:', dato
	if len(dato) < 1:
		print 'one character'
		if dato == 'u':
			mover1.__subirActuador__()
		elif dato == 'd':
			mover1.__bajarActuador__()
		else:
			mover1.__conexionControl__.send('1')
	else:		
		print 'coordinate'
		mover1.setNextPos(dato)
		mover1.changePosition()
		
