import sympy
import numpy as np
from sympy import init_printing
from sympy import init_session
import numpy.polynomial.polynomial as poly
import scipy.signal as signal
import matplotlib.pyplot as plt
import PolynomialMultiplicationTest as myPoli


def ButterworthC(N,w,Ts):
	Nr=int(N/2)
	i=1j
	a=[w**N]
	b=[1]
	# print poly.polymul(a,b)
	for k in range(1,Nr+1):
		rad=np.pi*(2*k-1)/(2*N)
		n=[1,2*w*np.sin(rad),w**2]
		b=poly.polymul(b,n)
	if N%2==1:
		b=poly.polymul(b,[1,w])
	print a,b
		

def ButterworthD(N,w,Ts):
	Nr=int(N/2)
	i=1j
	a=[w**N]
	b=[1.0]
	# print poly.polymul(a,b)
	for k in range(1,Nr+1):
		rad=np.pi*(2*k-1)/(2*N)
		n=[Ts**2,2*Ts**2,Ts**2]
		d=[Ts**2*w**2+4*Ts*np.sin(rad)*w+4,2*Ts**2*w**2-8,4-4*Ts*np.sin(rad)*w+w**2*Ts**2]
		b=poly.polymul(b,d)
		a=poly.polymul(a,n)
	if N%2==1:
		n=[w*Ts+2,w*Ts-2]
		d=[Ts,Ts]
		b=poly.polymul(b,n)
		a=poly.polymul(a,d)
	return (a,b)
		 
def ButterworthD2(N,w,Ts):
	Nr=int(N/2)
	a=np.array(myPoli.num(2*Nr))*Ts**(2*Nr)*w**N
	b=np.array(myPoli.polynomialMultiplier(N,Nr))
	if N%2==1:
		n=[w*Ts+2,w*Ts-2]
		d=[w*Ts,w*Ts]
		b=poly.polymul(b,n)
		a=poly.polymul(a,d)
	print a,b




# # print 2j
# # init_session()
# init_printing()

# w,s,sink=sympy.symbols('w s sink')
# z,Ts=sympy.symbols('z Ts')

# denum=(s**2+2*s*w*sink+w**2)
# denum_=denum.subs(s,2/Ts*(z-1)/(z+1))


# # print denum_
# # print sympy.simplify(denum_)
# print "New Butterworth"
# ButterworthD2(7,2,0.25)
# print "Old Butterworth"
# ButterworthD(7,2,0.25)
# b, a = signal.butter(3,1, 'low', analog=True)
# b,a=signal.bilinear(b, a, fs=1/0.1)
# print b,a
# # print signal.butter(9,0.01, 'low', analog=False)

# # print("ssd",myPoli.getCoeff(5,2,0),myPoli.getCoeff(5,2,1),myPoli.getCoeff(5,2,2))
# # a=[myPoli.getCoeff(4,2,0),myPoli.getCoeff(4,2,1),myPoli.getCoeff(4,2,2)]
# # b=[myPoli.getCoeff(4,1,0),myPoli.getCoeff(4,1,1),myPoli.getCoeff(4,1,2)]
# # res=np.polymul(a,b)
# # print(res)


# t = np.linspace(-1, 1, 201)
# x = (np.sin(2*np.pi*0.75*t*(1-t) + 2.1) +0.1*np.sin(2*np.pi*1.25*t + 1) +0.18*np.cos(2*np.pi*3.85*t))
# xn = x + np.random.randn(len(t)) * 0.08
# order=3
# period=15
# dt=0.05

# (b,a)=ButterworthD(order,period,dt)
# # print('Coeffince',b,a)
# # b, a = signal.butter(1,5, 'low', analog=True)
# # b,a=signal.bilinear(b, a, fs=1/0.05)
# # print('Good:',b,a)


# zi = signal.lfilter_zi(b, a)
# print(zi*0)
# z, _ = signal.lfilter(b, a, xn, zi=zi*0)


# plt.plot(t,x)
# plt.plot(t,xn)
# plt.plot(t,z)
# plt.show()


# fileout=open("Butterworth.test","w")
# line1="%.2f"%order+",%.2f"%period+",%.2f"%dt+"\n"
# fileout.write(line1)
# l_nr=len(t)

# for i in range(l_nr):
# 	line="%.4f"%t[i]+",%.4f"%xn[i]+",%.4f"%z[i]+"\n"
# 	fileout.write(line)
# fileout.close()


# # dt=0.01
# # (a,b)=ButterworthD(8,1,dt)
# # tf=signal.TransferFunction(a,b,dt=dt)

