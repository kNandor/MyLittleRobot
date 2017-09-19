import matplotlib.pyplot as plt
import scipy.signal as signal
import sys
sys.path.append("/home/nandi/Workspaces/MyProjects/PythonWorkSpace/DataSaver")
import Motor
import numpy as np
import PidTest


def MotorTransferFunction(Kv,b,R,J,L,dt):
	n_c=[Kv]
	d_c=[J*L,J*R+b*L,b*R+Kv**2]
	tf=signal.TransferFunction(n_c,d_c)
	(n_d,d_d)=signal.bilinear(n_c,d_c,1/dt)
	return (n_d,d_d)



Kv=1.33522554013
Ka=1*Kv
b=0.0120399629169
R=9.9
J=5.35935654e-04
L=4.20867688e+01

dt=0.005
(n_d,d_d)=MotorTransferFunction(Kv,b,R,J,L,dt)

motor1=PidTest.CDiscreteSystem([n_d],[d_d],3,3)
motor2=PidTest.CDiscreteSystem([n_d],[d_d],3,3)

print(n_d,d_d)
# mot1=Motor.MotorSim(b,J,Kv,Ka,L,R,dt)
# mot2=Motor.MotorSim(b,J,Kv,Ka,L,R,dt)


teta=0.050;
a0=Kv;
b0=J*L;
b1=J*R+L*b;
b2=R*b+Kv**2;

Kd=b0/a0/teta
Kp=b1/a0/teta
Ki=b2/a0/teta
N=50
pid=PidTest.CPid3(Kp,Ki,Kd,N,dt)
pi=PidTest.CPid3(Kp,Ki,0,0,dt)
print('Pid:',Kp,Ki,Kd)

# y=[ float(mot1.simulate(5.22)[0][0]) for _ in range(3000)]
y=[ float(motor1.calc(1.42)) for _ in range(3000)]
motor1.reset()

ypi=[]
ypid=[]
y_pi=0
y_pid=0
y_opt=1

# uu=[]
uu_pid=[]
error_pid=[]
for i in range(3000):
	error_p=y_opt-y_pi
	u=pi.calc(error_p)
	# uu.append(float(u))
	y_pi=float(motor1.calc(u))
	# y_p=y_
	ypi.append(y_pi)

	error_pd=y_opt-y_pid
	error_pid.append(error_pd)
	u=pid.calc(error_pd)
	if u>12:
		u=12
	elif u<0:
		u=0
	uu_pid.append(u)
	y_pid=float(motor2.calc(u))
	ypid.append(y_pid)


# fileMotor=open('Pid.test','w')

first_line="%f"%Kp+",%f"%Ki+",%f"%Kd+",%f"%N+",%f"%dt+"\n"
print(first_line)
# fileMotor.write(first_line)
# for i in range(len(ypid)):
# 	line="%.3f"%error_pid[i]+",%.2f"%uu_pid[i]+"\n"
# 	fileMotor.write(line)
# fileMotor.close()



plt.grid()
plt.plot(y)
# plt.plot(y1)
plt.plot(ypi)
plt.plot(ypid)
plt.legend(['Original','Pi','Pid'])


plt.figure();
plt.plot(uu_pid)
plt.show()