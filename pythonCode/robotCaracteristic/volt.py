import matplotlib.pyplot as plt
import numpy as np
 # v=[1.0, 2.0,2.9,3.8,4.6,5.3,5.9,6.5,7,7.35,7.7,8,8.3,8.48,8.68,8.86,9.04,9.2,9.35,9.52,9.97]
 # pwm=[50:10:250];
 # plot(pwm,v)

pwm_a=[50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250];
volt_a=[1.08,1.98,2.894,3.76,4.56,5.25,5.86,6.36,6.84,7.24,7.58,7.83,8.08,8.37,8.50,8.70,8.86,9.04,9.18,9.37,9.75];


pwm_np=np.array(pwm_a)
volt_np=np.array(volt_a)


volt_r=pwm_np**5*-1.386e-11+pwm_np**4*1.491e-08+pwm_np**3*-4.484e-06+pwm_np**2*0.0002072+pwm_np*0.1056-4.28

pwm_full=np.linspace(0,255,256)
volt_full=pwm_full**5*-1.386e-11+pwm_full**4*1.491e-08+pwm_full**3*-4.484e-06+pwm_full**2*0.0002072+pwm_full*0.1056-4.28

plt.figure()
plt.plot(pwm_np,volt_np)
plt.plot(pwm_np,volt_r)
plt.plot(pwm_full,volt_full)



pwm_r=volt_np**4*(-0.001843)+volt_np**3*(0.473)+volt_np**2*(-4.503)+volt_np*24.36+27.24


plt.figure()
plt.plot(volt_np,pwm_np)
plt.plot(volt_np,pwm_r)

plt.legend(['Oroginal','Recalculated'])




plt.show()

# volt_aa=pwm_a.^5*-1.386e-11+pwm_a.^4*1.491e-08+pwm_a.^3*-4.484e-06+pwm_a.^2*0.0002072+pwm_a*0.1056-4.28;
# figure;
# plot(pwm_a,volt_a);hold on;
# plot(pwm_a,volt_aa);hold off;

# pwm_aa=volt_a.^5*-0.0269+volt_a.^4*0.7405+volt_a.^3*-7.096+volt_a.^2*30.23+volt_a.*-44.56+71.75

# figure;
# plot(volt_a,pwm_a);hold on;
# plot(volt_a,pwm_aa);hold off;.
