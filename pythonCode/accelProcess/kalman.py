import robot as roblib
from numpy.linalg import inv
import numpy as np

class KalmanFilter:
	def __init__(self,Sys,P,Q,R):
		self.Sys=Sys
		self.P=P
		self.Q=Q
		self.R=R
	def predict(self,U):
		Xk1k1=self.Sys.getStates()
		self.Sys.f(U)
		F_v=self.Sys.F(Xk1k1,U)
		self.P=F_v*self.P*F_v.T+self.Q
	def update(self,M):
		Y_sim=self.Sys.h()
		E=M-Y_sim
		Xkk1=self.Sys.getStates()
		H_v=self.Sys.H(Xkk1)
		S=H_v*self.P*H_v.T+self.R
		
		K=self.P*H_v.T*inv(S)
		# K=np.matrix([[1.0,0.0]
		# 			,[0.0,0.0]
		# 			,[0.0,1.0]
		# 			,[0.0,0.0]
		# 			,[0.0,0.0]
		# 			,[0.0,0.0]])
		# print("K*E:"+str(K*E))
		X=Xkk1+K*E
		temp=K*H_v
		t_shape=temp.shape[0]
		self.P=(np.eye(t_shape)-temp)*self.P
		self.Sys.setStates(X)
		return X
		
