import sympy
import numpy as np
import numpy.polynomial.polynomial as poly
import scipy.signal as signal
import matplotlib.pyplot as plt

num=np.array([ 9.18181818, -9.        ])
den=np.array([  1.        , -0.81818182])
dt=0.01

Hd=signal.TransferFunction(num,den,dt=dt)
print Hd
endTime=0.5
startTime=0
Nr=int((endTime-startTime)/dt)
input_v=1.5
timestamp=np.linspace(startTime,endTime,Nr)
input_vec=np.zeros(Nr)+input_v

tout,y=signal.dlsim(Hd,input_vec,timestamp)
plt.plot(tout,y)
plt.show()

testFile=open('DiscreteSystem.test','w')
num_str=''
for i in range(len(num)):
	num_str+="%.2f"%num[i]
	if i!=len(num)-1:
		num_str+=','
	else:
		num_str+='\n'
testFile.write(num_str)
print num_str
den_str=''
for i in range(len(den)):
	den_str+="%.2f"%den[i]
	if i!=len(den)-1:
		den_str+=','
	else:
		den_str+='\n'
print den_str
testFile.write(den_str)
print len(input_vec),len(y)
print y[0]

for index in range(len(input_vec)):
	y_str='%.2f'%input_vec[index]+',%.2f'%y[index]+'\n'
	testFile.write(y_str)
testFile.close()