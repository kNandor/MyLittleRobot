import matplotlib.pyplot as plt
import numpy as np
class Robot:
	def __init__(self,L=10,dt=0.1,X=0.0,Y=0.0,Teta=0.0):
		self.L=L
		self.dt=dt
		self.X=X
		self.Y=Y
		self.Teta=Teta
		# self.Vf=0.0
		# self.prevVf=0.0
		# self.dTeta=0.0
		# self.Af=0.0
	def simulate(self,Vl,Vr):
		# self.prevVf=self.Vf
		# self.Vf=(Vr+Vl)/2.0
		# self.Af=(self.Vf-self.prevVf)/self.dt
		# self.dTeta=(Vr-Vl)/self.L
		self.X=self.X+(Vr+Vl)/2.0*np.cos(self.Teta)*self.dt
		self.Y=self.Y+(Vr+Vl)/2.0*np.sin(self.Teta)*self.dt
		self.Teta=self.Teta+(Vr-Vl)/self.L*self.dt

Nr=100
Vl=10.0
Vr=6.0
coor_x=[]
coor_y=[]

rob=Robot(10,0.1,0.0,0.0,0)

for i in range(Nr):
	# Vf.append(rob.Vf)
	# Af.append(rob.Af)
	# dTeta.append(rob.dTeta)
	coor_x.append(rob.X)
	coor_y.append(rob.Y)
	rob.simulate(Vl,Vr)

plt.figure()
plt.plot(coor_x,coor_y,'--o')


plt.show()

