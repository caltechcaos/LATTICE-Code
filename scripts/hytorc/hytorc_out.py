import re
import matplotlib.pyplot as plt


power_list = []
pos_list = []
with open("scripts/hytorc/a.txt") as f:
    for line in f.readlines():
        line = re.sub("[^!-~ ]+", "", line).strip()
        if line:
            power, pos = [float(num) for num in line.split(", ")]
            power_list.append(power)
            pos_list.append(pos)

plt.figure()
plt.plot(range(len(power_list)), power_list)
plt.figure()
plt.plot(range(len(pos_list)), pos_list)
plt.show()

