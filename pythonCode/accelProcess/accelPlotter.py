import matplotlib.pyplot as plt
import robot as Rob


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

def dataProcess(data):
	[dt,rpsR,rpsL,x_accel,y_accel,z_accel]=data
	robot=Rob.Robot(L=0.18,dt=0.01)
	X=[];Y=[]
	for i in range(len(rpsL)):
		l_rpsR=rpsR[i]/1.278
		l_rpsL=rpsL[i]/1.278
		robot.simulate(l_rpsL,l_rpsR)
		l_x=robot.X
		l_y=robot.Y
		X.append(l_x)
		Y.append(l_y)
	plt.figure()
	plt.plot(X,Y,'-x')
	plt.show()



def dataPlotter(data):
	[dt,rpsR,rpsL,x_accel,y_accel,z_accel]=data
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

FILE_NAMES=[	'Data_ac0'
				,'Data_ac1'
				,'Data_ac2'
				,'Data_ac_0'
				,'Data_ac_1'
				,'Data_ac_2']

def main():
	print('START MAIN')
	FOLDER='/home/nandi/Workspaces/MyProjects/arduino/MyLittleRobot/pythonCode/dataSaved/DataAccel/'
	FILE_NAME=FILE_NAMES[5]
	FILE_TYPE='.csv'
	data=readFromFile(FOLDER,FILE_NAME,FILE_TYPE)
	dataProcess(data)
	# dataPlotter(data)
	print('END MAIN')
if __name__ == "__main__":
    # execute only if run as a script
    main()