import numpy as np
import matplotlib.pyplot as plt

class CDiscreteSystem:
	def __init__(self,num,denum,n,m):
		self.Num=num
		self.Denum=denum
		self.n=n
		self.m=m
		self.U=np.zeros((self.n,1))
		self.Y=np.zeros((self.m-1,1))
	def calc(self,u):
		self.U=np.roll(self.U,1)
		self.U[0][0]=u
		num=np.matrix(self.Num)*np.matrix(self.U)
		dem=np.matrix(self.Denum[0][1:self.m])*np.matrix(self.Y)
		y=(num-dem)[0][0]/self.Denum[0][0]
		self.Y=np.roll(self.Y,1)
		self.Y[0][0]=y
		return y
	def reset(self):
		self.U=np.zeros((self.n,1))
		self.Y=np.zeros((self.m-1,1))
		

class CPi(CDiscreteSystem):
	def __init__(self,kp,ki,ts):
		num=[[2*kp+ki*ts,ki*ts-2*kp]]
		dem=[[2,-2]]
		CDiscreteSystem.__init__(self,num,dem,2,2)
class CPi2(CDiscreteSystem):
	def __init__(self,kp,ki,ts):
		num=[[kp,ki*ts-kp]]
		dem=[[1,-1]]
		CDiscreteSystem.__init__(self,num,dem,2,2)
class CPid(CDiscreteSystem):
	def __init__(self,kp,ki,kd,N,ts):
		a0=(1+N*ts/2)*(kp+ki*ts/2)
		a1=-2*kp+ki*ts/2*N*ts+N
		a2=(1-N*ts/2)*(kp-ki*ts/2)-N
		b0=(1+N*ts/2)
		b1=-2
		b2=(1-N*ts/2)
		num=[[a0,a1,a2]]
		dem=[[b0,b1,b2]]
		CDiscreteSystem.__init__(self,num,dem,3,3)
class CPid2(CDiscreteSystem):
	def __init__(self,f_kp,f_ki,f_kd,f_df,f_dt):
		num=[[f_kp*f_df+f_kd,f_kp*(f_dt-2*f_df)+f_ki*f_dt*f_df-2*f_kd,f_kp*(f_df-f_dt)+(f_dt-f_df)*f_ki*f_dt+f_kd]]
		dem=[[f_df,(f_dt-2*f_df),(f_df-f_dt)]]
		CDiscreteSystem.__init__(self,num,dem,3,3)
class CPi3:
	def __init__(self,f_kp,f_ki,f_dt):
		f_num_ki=[[f_ki*f_dt,0]]
		f_denum_ki=[[1,-1]]
		self.Kp=f_kp
		self.Ki=CDiscreteSystem(f_num_ki,f_denum_ki,2,2)
	def calc(self,u):
		return u*self.Kp+self.Ki.calc(u)

class CPid3:
	def __init__(self,f_kp,f_ki,f_kd,f_N,f_dt):
		f_num_ki=[[f_ki*f_dt,0]]
		f_denum_ki=[[1,-1]]
		f_num_kd=[[f_kd*f_N,-f_kd*f_N]]
		f_denum_kd=[[1,-1+f_N*f_dt]]
		self.Kp=f_kp
		self.Ki=CDiscreteSystem(f_num_ki,f_denum_ki,2,2)
		self.Kd=CDiscreteSystem(f_num_kd,f_denum_kd,2,2)
	def calc(self,u):
		return u*self.Kp+self.Ki.calc(u)+self.Kd.calc(u)
