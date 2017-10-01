import numpy as np
class Robot:
	def __init__(self,L=10,dt=0.1,X=0.0,Y=0.0,Teta=0.0):
		self.L=L
		self.dt=dt
		self.X=X
		self.Y=Y
		self.Teta=Teta
		self.Vf=0.0
		self.W=0.0
		self.Af=0.0
	def simulate(self,Vl,Vr):
		Vf=(Vr+Vl)/2.0
		self.W=(Vr-Vl)/self.L
		self.X=self.X+Vf*np.cos(self.Teta)*self.dt
		self.Y=self.Y+Vf*np.sin(self.Teta)*self.dt
		self.Teta=self.Teta+self.W*self.dt
		self.Af=Vf-self.Vf
		self.Vf=Vf
class RobotSimulate:
	def __init__(self,L=10,dt=0.1,X=0.0,Y=0.0,Teta=0.0):
		self.L=L
		self.dt=dt
		self.X=X
		self.Y=Y
		self.Teta=Teta
		self.Vf=0.0
		self.Af=0.0
		self.W=0.0
	def f(self,U):
		Vl=U.item((0,0))
		Vr=U.item((1,0))
		self.X=self.X+(self.Vf*self.dt+self.Af*self.dt**2/2)*np.cos(self.Teta+self.dt*self.W)
		self.Y=self.Y+(self.Vf*self.dt+self.Af*self.dt**2/2)*np.sin(self.Teta+self.dt*self.W)
		self.Teta=self.Teta+self.W*self.dt
		# Vf_=0.0
		Vf=(Vr+Vl)/2.0
		Af=(Vf-self.Vf)/self.dt
		self.Vf=Vf
		self.Af=Af
		self.W=(Vr-Vl)/self.L
		return (self.Vf,self.Af,self.W,self.X,self.Y,self.Teta)
	def getStates(self):
		return np.matrix([[self.Vf],[self.Af],[self.W],[self.X],[self.Y],[self.Teta]])
	def setStates(self,states):
		self.Vf=states.item((0,0))
		self.Af=states.item((1,0))
		self.W=states.item((2,0))
		self.X=states.item((3,0))
		self.Y=states.item((4,0))
		self.Teta=states.item((5,0))
	def h(self):
		return np.matrix([[self.Af],[self.W]])
	def F(self,(Vf,Af,W,X,Y,Teta),(Vl,Vr)):
		F_matrix=np.matrix([
							 [0.0,0.0,0.0,0.0,0.0,0.0]
							,[-1.0/self.dt,0.0,0.0,0.0,0.0,0.0]
							,[0.0,0.0,0.0,0.0,0.0,0.0]
							,[np.cos(Teta)*self.dt,self.dt**2/2*np.cos(Teta),-(Vf*self.dt+Af*self.dt**2/2)*np.sin(W*self.dt)/self.dt,1.0,0.0,-(Vf*self.dt+Af*self.dt**2/2)*np.sin(Teta)]
							,[np.sin(Teta)*self.dt,self.dt**2/2*np.sin(Teta),(Vf*self.dt+Af*self.dt**2/2)*np.cos(W*self.dt)/self.dt,0.0,1.0,(Vf*self.dt+Af*self.dt**2/2)*np.cos(Teta)]
							,[0.0,0.0,self.dt,0.0,0.0,1.0]])
		return F_matrix
	def H(self,(Vf,Af,W,X,Y,Teta)):
		H_matrix=np.matrix([
							 [0.0,1.0,0.0,0.0,0.0,0.0]
							,[0.0,0.0,1.0,0.0,0.0,0.0]])
		return H_matrix

class Accel:
	def __init__(self,dt):
		self.dt=dt
		self.A=0.0
		self.V=0.0
		self.W=0.0
		self.X=0.0
		self.Y=0.0
		self.Teta=0.0
	def f(self,U):
		self.X=self.X+(self.V*self.dt+self.A*self.dt**2/2)*np.cos(self.Teta+self.W*self.dt)
		self.Y=self.Y+(self.V*self.dt+self.A*self.dt**2/2)*np.sin(self.Teta+self.W*self.dt)
		self.Teta=self.Teta+self.W*self.dt
		self.V=self.V+self.A*self.dt
	def h(self):
		return np.matrix([[self.A],[self.W]])
	def getStates(self):
		return np.matrix([	[self.A]
							,[self.V]
							,[self.W]
							,[self.X]
							,[self.Y]
							,[self.Teta]])
	def setStates(self,State):
		self.A=State.item((0,0))
		self.V=State.item((1,0))
		self.W=State.item((2,0))
		self.X=State.item((3,0))
		self.Y=State.item((4,0))
		self.Teta=State.item((5,0))
	def F(self,State,U):
		A=State.item((0,0))
		V=State.item((1,0))
		W=State.item((2,0))
		X=State.item((3,0))
		Y=State.item((4,0))
		Teta=State.item((5,0))
		F_matrix=np.matrix(	[[1.0		,0.0	,0.0	,0.0	,0.0	,0.0]
							,[self.dt	,1.0	,0.0	,0.0	,0.0	,0.0]
							,[0.0		,0.0	,1.0	,0.0	,0.0	,0.0]
							,[self.dt**2/2*np.cos(Teta+W*self.dt)
								,self.dt*np.cos(Teta+W*self.dt)
								,-(V*self.dt+A*self.dt**2/2)*np.sin(self.dt*W)
								,1.0,0.0
								,-(V*self.dt+A*self.dt**2/2)*np.sin(Teta)]
							,[self.dt**2/2*np.sin(Teta+W*self.dt)
								,self.dt*np.sin(Teta+W*self.dt)
								,(V*self.dt+A*self.dt**2/2)*np.cos(self.dt*W)
								,0.0,1.0
								,(V*self.dt+A*self.dt**2/2)*np.cos(Teta)]
							,[0.0,0.0,self.dt,0.0,0.0,1.0]])
		return F_matrix
	def H(self,State):
		H_matrix=np.matrix([
							 [0.0,1.0,0.0,0.0,0.0,0.0]
							,[0.0,0.0,1.0,0.0,0.0,0.0]])
		return H_matrix		




