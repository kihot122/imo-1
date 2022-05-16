from math import ceil
from tkinter import font
from turtle import color
import matplotlib.pyplot as plt
import math
import numpy as np


for i in range(0, 3):
   plt.cla()
   plt.clf()
   name = 'Chain-'+str(i)
   plt.rcParams["figure.figsize"] = [7.50, 3.50]
   plt.rcParams["figure.autolayout"] = True

   points = []

   with open(name) as fp:
      line = fp.readline()
      while line:
         line_sp = line.split(" ")
         points.append([(int)(line_sp[0]), (int)(line_sp[1])])
         line = fp.readline()

   poins_count = len(points)
   group_count = math.ceil(poins_count/2)

   g1 = np.array(points[:group_count])
   g2 = np.array(points[group_count:])

   g1 = np.append(g1, [g1[0]], axis=0).T
   g2 = np.append(g2, [g2[0]], axis=0).T

   plt.plot(g1[0], g1[1], color="red")
   plt.plot(g2[0], g2[1], color="blue")

   plt.plot(g1[0], g1[1], 'o', color="red")
   plt.plot(g2[0], g2[1], 'o', color="blue")

   font = {'family' : 'normal',
         'size'   : 7}

   # for i, j in zip(g1[0], g1[1]):
   #    plt.text(i, j+0.5, '({}, {})'.format(i, j), font=font)

   # for i, j in zip(g2[0], g2[1]):
   #    plt.text(i, j+0.5, '({}, {})'.format(i, j), font=font)


   plt.savefig(name)



# plt.plot(x, y)


# plt.axis([0, 16, 0, 20])

# x = [2, 3, 6, 2]
# y = [2, 7, 6, 2]



# plt.plot(x, y, 'r*')
# plt.plot(x, y)

# plt.axis([0, 16, 0, 20])

# # for i, j in zip(x, y):
# #    plt.text(i, j+0.5, '({}, {})'.format(i, j))

# plt.show()