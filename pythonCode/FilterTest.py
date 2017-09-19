import Butterworth as BW
import matplotlib.pyplot as plt
import scipy.signal as signal

dt=0.01
wn=12.5
order=2

savedData=open("/home/nandi/Workspaces/MyProjects/PythonWorkSpace/DataSaver/DataFiltered2.out",'r')


pwm_a=[]
rps_a=[]
rps_filt_c=[]
for row in savedData:
	words=row.split(',')
	if len(words)==3:
		try:
			pwm=float(words[0])
			rps=float(words[1])
			rps_f_v=float(words[2][:-1])
			pwm_a.append(pwm)
			rps_a.append(rps)
			rps_filt_c.append(rps_f_v)
		except:
			pass
savedData.close()

b, a = signal.butter(order,wn, 'low', analog=True)
b,a=signal.bilinear(b, a, fs=1/dt)
# (b,a)=BW.ButterworthD(order,wn,dt)
zi = signal.lfilter_zi(b, a)
rps_filt, _ = signal.lfilter(b, a, rps_a, zi=zi*0)



plt.plot(rps_a)
plt.plot(rps_filt_c)
plt.plot(rps_filt)
plt.legend(['Rps','Rps_controller','Rps_calc'])
plt.show()