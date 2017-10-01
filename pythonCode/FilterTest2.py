import Butterworth as BW
import matplotlib.pyplot as plt
import scipy.signal as signal

dt=0.0075
wn=25.5
order=1

savedData=open("/home/nandi/Workspaces/MyProjects/arduino/MyLittleRobot/pythonCode/dataSaved/DataAccel/Data_Pwm70.csv",'r')


rpsR_a=[]
rpsR_filt_c=[]
rpsL_a=[]
rpsL_filt_c=[]
for row in savedData:
	words=row.split(',')
	print(words)
	if len(words)==4:
		try:
			rpsL_f_v=float(words[0])
			rpsR_f_v=float(words[1])
			rpsR=float(words[2])
			rpsL=float(words[3][:-1])
			rpsL_filt_c.append(rpsL_f_v)
			rpsR_filt_c.append(rpsR_f_v)
			rpsR_a.append(rpsR)
			rpsL_a.append(rpsL)
		except:
			pass
savedData.close()

b, a = signal.butter(order,wn, 'low', analog=True)
b_f,a_f=signal.bilinear(b, a, fs=1.0/dt)
# (b,a)=BW.ButterworthD(order,wn,dt)
zi = signal.lfilter_zi(b, a)
rpsR_filt, _ = signal.lfilter(b_f, a_f, rpsR_a, zi=zi*0)
rpsL_filt, _ = signal.lfilter(b_f, a_f, rpsL_a, zi=zi*0)



plt.plot(rpsR_a)
plt.plot(rpsL_a)
plt.plot(rpsR_filt_c)
plt.plot(rpsL_filt_c)
plt.plot(rpsR_filt)
plt.plot(rpsL_filt)
plt.legend(['RpsR','RpsL','RpsR_A','RpsL_A','RpsR_C','RpsL_C'])
plt.show()

b_f,a_f=signal.bilinear(b, a, fs=1.0/0.0075)
print('Num'+str(b_f)+'Den'+str(a_f))