import numpy as np
import sys
from matplotlib import pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

data = np.genfromtxt(sys.argv[1], delimiter=", ")
fig = plt.figure()
X, Y = np.meshgrid(np.arange(0, 10, 0.05),np.arange(0, 15, 0.05))
Z1 = np.zeros(np.shape(X))
#Z2 = np.zeros(np.shape(X))
for ix in range(len(X.T)):
    for iy in range(len(X)):
        Z1[iy,ix] = data.T[2,(len(Y)) * ix + iy]
#        Z2[iy,ix] = data.T[3,(len(Y)) * ix + iy]

ax1 = fig.add_subplot(111, projection='3d')
#ax2 = fig.add_subplot(122, projection='3d')
ax1.plot_surface(X, Y, Z1, cmap=cm.coolwarm, antialiased=False)
#ax2.plot_surface(X, Y, Z2, cmap=cm.coolwarm, antialiased=False)
fig.savefig("Zifkin_CA_6_Results/plot.png")
plt.show()
