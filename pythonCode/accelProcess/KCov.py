import sympy
from sympy import (init_printing, Matrix,MatMul,integrate, symbols,cos,sin)
import robot as robot


robo=robot.RobotSimulate(dt=0.01)
F=robo.F((1.0,1.0,1.0,0,0,0),(0,0))
# print(F)
init_printing(use_latex='mathjax')
dt, phi = symbols('dt phi')
Vf, Teta,Af,W = symbols('Vf Teta Af W')
F_k=Matrix(
	[
	 [1.0,dt,0.0,0.0,0.0,0.0]
	,[0.0,0.0,0.0,0.0,0.0,0.0]
	,[0.0,0.0,0.0,0.0,0.0,0.0]
	,[0,0,0,1.0,0.0,0]
	,[0,0,0,0.0,1.0,0]
	,[0.0,0.0,dt,0.0,0.0,1.0]]
	)


Q_c=Matrix([[1,0,0,0,0,0],
			[0,1,0,0,0,0],
			[0,0,1,0,0,0],
			[0,0,0,1,0,0],
			[0,0,0,0,1,0],
			[0,0,0,0,0,1]])*phi


Q=sympy.integrate(F*Q_c*F.T,(dt,0,dt))


print(Q)

