import matplotlib.pyplot as plt
import robot as Rob
import numpy as np
import scipy.signal as signal

g_dis=np.array([1.52,1.42,0.88,0.99,0.93,0.925,0.655,0.65])
g_dur=np.array([10.0,10.0,10.0,10.0,5.0,5.0,3.0,2.0])
g_rpsSet=np.array([2.0,2.0,1.5,1.5,2.5,2.5,3.0,4.0])
g_k=0.0717618055556

def readFromFile(FOLDER,FILE_NAME,FILE_TYPE):
	fileInput=open(FOLDER+FILE_NAME+FILE_TYPE,'r')
	firstLine=fileInput.readline()
	data=[]
	for number_str in firstLine.split(','):
		num=[float(number_str)]
		data.append(num)
	for row in fileInput:
		numbers=row.split(',')
		i=0
		for number_str in numbers:
			number=float(number_str)
			data[i].append(number)
			i+=1
	fileInput.close()
	return data

def calcRps(data):
	[rpsR,rpsL,x_accel,y_accel,z_accel,x_gyro,y_gyro,z_gyro]=data
	robot=Rob.Robot(L=0.18,dt=0.05)
	X=[];Y=[]
	Af=[]
	for i in range(len(rpsL)):
		l_rpsR=rpsR[i]*g_k
		l_rpsL=rpsL[i]*g_k
		robot.simulate(l_rpsL,l_rpsR)
		l_x=robot.X
		l_y=robot.Y
		X.append(l_x)
		Y.append(l_y)
		l_af=robot.Af
		Af.append(l_af)
	plt.figure()
	plt.plot(X,Y,'-x')
	plt.figure()
	plt.plot(Af)
	plt.plot(x_accel)
	plt.show()

def calcAcc(data):
	[rpsR,rpsL,x_accel,y_accel,z_accel,x_gyro,y_gyro,z_gyro]=data
	# rad/s
	# dt=10
	# wn=2.0*np.pi/dt
	wn=0.025
	bt_num,bt_den=signal.butter(1,wn,'low')
	# z_num,z_den=signal.bilinear()
	zi = signal.lfilter_zi(bt_num,bt_den)
	z, _ = signal.lfilter(bt_num, bt_den, x_accel, zi=zi*0)
	plt.plot(x_accel)
	plt.plot(z)
	plt.show()


def dataPlotter(data):
	[rpsR,rpsL,x_accel,y_accel,z_accel,x_gyro,y_gyro,z_gyro]=data
	figure_rot=1
	figure_accel=2
	figure_gyro=3
	# Ploting the motor rotation
	plt.figure(figure_rot)
	plt.plot(rpsR)
	plt.plot(rpsL)
	# Ploting the acceleration
	plt.figure(figure_accel)
	plt.plot(x_accel)
	plt.plot(y_accel)
	plt.plot(z_accel)
	plt.show()

FILE_NAMES=[	'Data_A3'
				,'Data_A4'
				,'Data_B1'
				,'Data_B2'
				,'Data_C1'
				,'Data_C2'
				,'Data_D1'
				,'Data_E1']

def main():
	print('START MAIN')
	FOLDER='/home/nandi/Workspaces/MyProjects/arduino/MyLittleRobot/pythonCode/dataSaved/DataAccel/'
	FILE_NAME=FILE_NAMES[1]
	FILE_TYPE='.csv'
	data=readFromFile(FOLDER,FILE_NAME,FILE_TYPE)
	# calcRps(data)
	calcAcc(data)
	# dataPlotter(data)
	
	# vel=g_dis/g_dur
	# coef=vel/g_rpsSet
	# k=np.sum(coef)/len(coef)
	# print(k)
	# plt.plot(g_rpsSet,vel)
	# plt.plot(g_rpsSet,g_rpsSet*k)
	# plt.show()
	print('END MAIN')
if __name__ == "__main__":
    # execute only if run as a script
    main()