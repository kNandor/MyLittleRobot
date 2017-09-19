import numpy as np


# p_a=[[1.0,2.0,3.0],[4.0,5.0,6.0],[1.0,1.0,1.0],[2.0,3.0,4.4]]
Ts=0.25
w=2

def getCoeff(N,k,i):
	k+=1
	rad=np.pi*(2.0*k-1.0)/(2.0*N)
	if i==0:
		return 4+4*Ts*np.sin(rad)*w+w**2*Ts**2


		# return Ts**2*w**2+4*Ts*np.sin(rad)*w+4
		# return p_a[k][i]
	elif i==1:
		# return p_a[k][i]
		return 2*Ts**2*w**2-8
	else:
		# return p_a[k][i]
		return 4-4*Ts*np.sin(rad)*w+w**2*Ts**2



# def order(p_a,n,l):
def order(NN,N,n,l):
	if n==0:
		value=1.0
		for i in range(N-l):
			# value*=p_a[i+l][0]
			value*=getCoeff(NN,i+l,0)
		# print 'A'+str(l)
		return value
	elif n==1:
		if l==N-1:
			# print 'B'+str(l)
			# return p_a[l][1]
			return getCoeff(NN,l,1)
		else:
			# print 'C'+str(l)
			# return p_a[l][0]*order(p_a,n,l+1)+p_a[l][1]*order(p_a,n-1,l+1)
			return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)
	elif n==2:
		if l==N-1:
			# print 'D'+str(l)
			# return p_a[l][2]
			return getCoeff(NN,l,2)
		else:
			# print 'E'+str(l)
		 	# return p_a[l][0]*order(p_a,n,l+1)+p_a[l][1]*order(p_a,n-1,l+1)+p_a[l][2]*order(p_a,n-2,l+1)
		 	return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
	else:
		if n%2==1:
			if l>=N-((n+1)/2):
				return getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
			else:
				return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
		else:
			if l>=N-((n+1)/2):
				return getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
			else:
				return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)	
	return 1;


def order2(p_a,n,l):
	if n==0:
		value=1.0
		for i in range(len(p_a)-l):
			value*=p_a[i+l][0]
		# print 'A'+str(l)
		return value
	elif n==1:
		if l==len(p_a)-1:
			# print 'B'+str(l)
			return p_a[l][1]
		else:
			# print 'C'+str(l)
			return p_a[l][0]*order2(p_a,n,l+1)+p_a[l][1]*order2(p_a,n-1,l+1)
	elif n==2:
		if l==len(p_a)-1:
			# print 'D'+str(l)
			return p_a[l][2]
			# return getCoeff(NN,l,2)
		else:
			# print 'E'+str(l)
		 	return p_a[l][0]*order2(p_a,n,l+1)+p_a[l][1]*order2(p_a,n-1,l+1)+p_a[l][2]*order2(p_a,n-2,l+1)
		 	# return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
	else:
		if n%2==1:
			if l>=len(p_a)-((n+1)/2):
				return p_a[l][1]*order2(p_a,n-1,l+1)+p_a[l][2]*order2(p_a,n-2,l+1)
				# return getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
			else:
				return p_a[l][0]*order2(p_a,n,l+1)+p_a[l][1]*order2(p_a,n-1,l+1)+p_a[l][2]*order2(p_a,n-2,l+1)
				# return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
		else:
			if l>=len(p_a)-((n+1)/2):
				return p_a[l][2]*order2(p_a,n-2,l+1)
				# return getCoeff(NN,l,2)*order(NN,N,n-2,l+1)
			else:
				return p_a[l][0]*order2(p_a,n,l+1)+p_a[l][1]*order2(p_a,n-1,l+1)+p_a[l][2]*order2(p_a,n-2,l+1)
				# return getCoeff(NN,l,0)*order(NN,N,n,l+1)+getCoeff(NN,l,1)*order(NN,N,n-1,l+1)+getCoeff(NN,l,2)*order(NN,N,n-2,l+1)	
	return 0;

# def polynomialMultiplier(p_a):
def polynomialMultiplier(N,n):
	coeff_a=[]
	print(N,n)
	for i in range(2*n+1):
		coeff=order(N,n,i,0)
		coeff_a.append(coeff)
	return coeff_a

def polynomialMultiplier2(p_a):
	coeff_a=[]
	l_Nr=len(p_a)
	for i in range(2*l_Nr+1):
		coeff=order2(p_a,i,0)
		coeff_a.append(coeff)
	return coeff_a

def factorial(n):
	res=1
	for i in range(n):
		res*=(i+1)
	return res
#(1+z)^n
def num(n):
	n+=1
	# n=2*(n-1)+1
	if n==0 or n==1:
		return [1]
	coeff=np.zeros(n)
	Nr=int((n+1)/2)
	fac_n=factorial(n-1)
	for i in range(Nr):
		fac_i=factorial(i)
		fac_ni=factorial(n-1-i)
		coeff[i]=fac_n/fac_i/fac_ni
		coeff[n-i-1]=fac_n/fac_i/fac_ni
	return  coeff

# p_a=[[1.0,2.0,3.0],[4.0,5.0,6.0],[1.0,1.0,1.0],[2.0,3.0,4.4]]
# # # # print p_a[0][0]
# print polynomialMultiplier2(p_a)
# a=np.array(1)
# for p in p_a:
# 	a=np.polymul(a,p)
# print a

# # num(11)
