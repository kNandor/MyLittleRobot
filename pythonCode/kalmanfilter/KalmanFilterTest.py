import scipy.signal as signal
import filterpy.kalman as KalmanFilter

class Robot:
	def __init__(self,l,x,y,teta):
		self.l=l
		self.x=x
		self.y=y
		self.teta=teta
	def calc(self,velR,velL):
		R=self.l/2.0*(velL+velR)/(velL-velR)
		w=(velR-velL)/self.l
		ICC=[self.x-R*sin(self.teta),self.y+R*cos(self.teta)]
		