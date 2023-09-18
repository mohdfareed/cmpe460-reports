import os

import matplotlib.pyplot as plt
import numpy as np

images_path = os.path.dirname(os.path.abspath(__file__)) + "/images/"
plt.rcParams["text.usetex"] = True

# data
distance = np.array(
    [
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
    ]
)
voltage1 = np.array(
    [
        4.98,
        1.65,
        0.73,
        0.67,
        0.63,
        0.62,
        0.64,
        0.67,
        0.69,
        0.71,
        0.72,
        0.74,
        0.75,
        0.76,
        0.77,
        0.79,
        0.87,
        2.45,
        3.28,
        3.77,
        3.96,
        3.96,
        3.65,
    ]
)
voltage2 = np.array(
    [
        4.96,
        2.61,
        0.68,
        0.53,
        0.17,
        0.17,
        0.19,
        0.55,
        0.64,
        0.67,
        0.69,
        0.70,
        0.72,
        0.73,
        0.74,
        0.75,
        0.80,
        0.86,
        1.98,
        2.80,
        2.90,
        2.96,
        2.60,
    ]
)
current1 = np.array(
    [
        0.002,
        0.335,
        0.427,
        0.433,
        0.437,
        0.438,
        0.436,
        0.433,
        0.431,
        0.429,
        0.428,
        0.426,
        0.425,
        0.424,
        0.423,
        0.421,
        0.413,
        0.255,
        0.172,
        0.123,
        0.104,
        0.104,
        0.135,
    ]
)
current2 = np.array(
    [
        0.002,
        0.1195,
        0.216,
        0.2235,
        0.2415,
        0.2415,
        0.2405,
        0.2225,
        0.218,
        0.2165,
        0.2155,
        0.22,
        0.214,
        0.2135,
        0.213,
        0.2125,
        0.21,
        0.207,
        0.151,
        0.11,
        0.11,
        0.102,
        0.12,
    ]
)

# plot voltage
plt.figure(1)
plt.plot(distance, voltage1, "bo-", label="$10k\\Omega$")
plt.plot(distance, voltage2, "ro-", label="$20k\\Omega$")
plt.xlabel("\\textrm{Distance (mm)}", fontsize=12)
plt.ylabel("\\textrm{Voltage (V)}", fontsize=12)
plt.title("\\textrm{Voltage Plot}", fontsize=15)
plt.grid(True)
plt.legend()
plt.savefig(images_path + "voltage_plot.pdf")

# plot current
plt.figure(2)
plt.plot(distance, current1, "bo-", label="$10k\\Omega$")
plt.plot(distance, current2, "ro-", label="$20k\\Omega$")
plt.xlabel("\\textrm{Distance (mm)}", fontsize=12)
plt.ylabel("\\textrm{Current (mA)}", fontsize=12)
plt.title("\\textrm{Current Plot}", fontsize=15)
plt.grid(True)
plt.legend()
plt.savefig(images_path + "current_plot.pdf")
