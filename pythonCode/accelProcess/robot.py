import numpy as np
class Robot:
	def __init__(self,L=10,dt=0.1,X=0.0,Y=0.0,Teta=0.0):
		self.L=L
		self.dt=dt
		self.X=X
		self.Y=Y
		self.Teta=Teta
	def simulate(self,Vl,Vr):
		self.X=self.X+(Vr+Vl)/2.0*np.cos(self.Teta)*self.dt
		self.Y=self.Y+(Vr+Vl)/2.0*np.sin(self.Teta)*self.dt
		self.Teta=self.Teta+(Vr-Vl)/self.L*self.dt

