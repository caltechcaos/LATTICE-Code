import re
millis_list =[]
with open("scripts/dt/log_out.log") as f:
    for line in f.readlines():
        line = re.sub("[^!-~ ]+", "", line).strip()
        if line and line.isnumeric():
            millis_list.append(int(line))

diff = 0
for i in range(len(millis_list)- 1):
    diff += millis_list[i + 1] - millis_list[i]

print(diff/(len(millis_list) - 1))