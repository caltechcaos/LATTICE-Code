import json
import re
import numpy as np
from scipy.signal import medfilt

filename = "scripts/loggingtest.txt"
outfile = "test.json"

rotational_conversion_factor = 0.5
rotation_to_measurement_factor = 1
test_type = "Simple"
unit = "Rotations"

test_start = False
test_name = ""
data_dict = dict()
with open(filename, "r") as f:
    for line in f.readlines():
        line = re.sub("[^!-~ ]+", "", line).strip()
        if "STARTING" in line:
            print(f"Parsing:{line}")
            test_start = True
            _, name = line.split("-")
            test_name = (
                name.replace("QUASISTATIC", "slow")
                .replace("DYNAMIC", "fast")
                .replace("FORWARD", "forward")
                .replace("BACKWARD", "backward")
                .replace(" ", "-")
            )
            data_dict[test_name] = list()
        elif "DISABLING" in line:
            test_start = False
            test_name = ""
        elif test_start:  # data
            if "," in line:
                data = [float(x) for x in line.split(", ")]
                data_dict[test_name].append(data)


def process_velocity(testdata):
    pos_col = 2
    dt = 0.005

    for i in range(2, len(testdata) - 2):
        x_b2 = testdata[i - 2][pos_col]
        x_b1 = testdata[i - 1][pos_col]
        x_f1 = testdata[i + 1][pos_col]
        x_f2 = testdata[i + 2][pos_col]
        deriv = (x_b2 - 8 * x_b1 + 8 * x_f1 - x_f2) / (12 * dt)
        testdata[i].append(deriv)
    data = np.matrix(testdata[2:-2])
    data[:, pos_col + 1] = (
        medfilt(data[:, pos_col + 1].flatten().tolist()[0], 5)
    ).reshape(-1, 1)
    data[:, pos_col] *= rotational_conversion_factor
    data[:, pos_col + 1] *= rotational_conversion_factor

    return data.tolist()


for test in data_dict.keys():
    data_dict[test] = process_velocity(data_dict[test])

data_dict["sysid"] = True
data_dict["test"] = test_type
data_dict["units"] = unit
data_dict["unitsPerRotation"] = rotation_to_measurement_factor

with open(outfile, "w+") as f:
    json.dump(data_dict, f)
