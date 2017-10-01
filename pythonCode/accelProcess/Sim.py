import numpy as np
import robot as robo
import kalman as kalman
import matplotlib.pyplot as plt


errorA=100.0
errorW=0.5
def robotSim(robot):
	global errorA
	global errorW
	U=np.matrix([[1.0],[0.5]])
	Y_a=np.matrix([[0],[0]])
	X_a=np.zeros((6,1))
	U_a=np.zeros((2,1))
	for i in range(20):
		U_i=U+np.random.rand(2,1)*0.0
		U_a=np.concatenate((U_a,U_i),axis=1)
		robot.f(U_i)
		# robot.Af+=(np.random.random()-0.5)*2*4.0
		# robot.W+=(np.random.random()-0.5)*2*0.5
		X=robot.getStates()
		E=(np.random.rand(2,1)-0.5)
		Y=robot.h()+np.matrix([[errorA,0.0],[0.0,errorW]])*E
		X+=np.matrix([[0.0,0.0],[errorA,0],[0.0,errorW],[0,0],[0,0],[0,0]])*E
		robot.setStates(X)
		Y_a=np.concatenate((Y_a,Y),axis=1)
		X_a=np.concatenate((X_a,X),axis=1)
	# print(X_a[3],X_a[4])
	X_a=X_a.tolist()
	return (U_a,Y_a,X_a)

def plotting(X_f_l,X_a):
	plt.figure(1)
	plt.plot(X_f_l[3],X_f_l[4],'--o')
	plt.plot(X_a[3],X_a[4],'--^')
	plt.title('Position')
	plt.legend(["fil",'mes'])
	plt.figure(2)
	plt.plot(X_f_l[5],'--o')
	plt.plot(X_a[5],'--^')
	plt.title('Angle')
	plt.legend(["fil",'mes'])
	plt.figure(3)
	plt.subplot(311)
	plt.plot(X_f_l[0],'--o')
	plt.plot(X_a[0],'--^')
	plt.title('Forward velocity')
	plt.legend(["fil",'mes'])
	plt.subplot(312)
	plt.plot(X_f_l[2],'--o')
	plt.plot(X_a[2],'--^')
	plt.title('Angular velocity')
	plt.legend(["fil",'mes'])
	plt.subplot(313)
	plt.plot(X_f_l[1],'--o')
	plt.plot(X_a[1],'--^')
	plt.title('Acceleration')
	plt.legend(["fil",'mes'])
	plt.show()

def robotFilter(robot,U_a,Y_a,X_a):
	global errorA
	global errorW
	shapeY=Y_a.shape
	init_X=np.zeros((6,1))
	robot.setStates(init_X)
	P=np.zeros((6,6))
	dt=robot.dt
	phi=10.0
	Q=np.matrix([[0, 0, 0, 0, 0, 0]
				,[0, 10000.0*dt*phi, 0, -1.0*dt*phi, 0, 0]
				, [0, 0, 5.e-2, 0, 0, 0]
				, [0, -1.0*dt*phi, 0, 1.00020100163329*dt*phi, -0.0100995766634669*dt*phi, -0.000100498325008375*dt*phi]
				, [0, 0, 0, -0.0100995766634669*dt*phi, 2.01002500336671*dt*phi, 0.0200994975041875*dt*phi]
				, [0, 0, 0, -0.000100498325008375*dt*phi, 0.0200994975041875*dt*phi, 1.0001*dt*phi]])
	# Q=robot.F((0.0,0.0,0.0,0,0,0),(0,0))
	R=np.matrix([[errorA**2,0.0],[0.0,errorW**2]])
	kfilter=kalman.KalmanFilter(robot,P,Q,R)
	X_f=np.zeros((6,1))
	for index in range(shapeY[1]):
		U=np.matrix([[U_a.item(0,index)],[U_a.item(1,index)]])
		Y=np.matrix([[Y_a.item(0,index)],[Y_a.item(1,index)]])
		kfilter.predict(U)
		X=kfilter.update(Y)
		X_f=np.concatenate((X_f,X),axis=1)
	X_f_l=X_f.tolist()
	plotting(X_f_l,X_a)
	# plt.figure(1)
	# plt.plot(X_f_l[3],X_f_l[4],'--o')
	# plt.plot(X_a[3],X_a[4],'--^')
	# plt.legend(['Filt','Mes'])
	# plt.show()


		

def main():
	print("Start main")
	dt=0.01
	robot=robo.RobotSimulate(dt=dt)
	acc=robo.Accel(dt=dt)
	Q=robot.F((1.0,1.0,1.0,0,0,0),(0,0))
	print(Q)
	(U_a,Y_a,X_a)=robotSim(robot)
	robotFilter(robot,U_a,Y_a,X_a)
	print("End main")

if __name__=="__main__":
	main()