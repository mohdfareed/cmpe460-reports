import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import LogFormatter, LogLocator
from numpy import pi

# Data

# high pass
# frequency = np.array([0.2, 0.3, 0.5, 0.7, 1, 2, 5, 10, 15, 20])
# vin = 38  # mV (constant for all frequencies)
# vout = np.array([4, 6.5, 10, 12, 14.5, 16.5, 17.5, 18, 19, 19])
# time_difference = np.array(
#     [-1.04, -0.653, -0.28, -0.179, -0.1, -0.03, -0.004, -0.001, 0, 0]
# )
# cutoff_frequency = 0.7  # Hz
# filename = "high_pass_plot.pdf"


# low pass
# frequency = np.array([0.2, 0.3, 0.5, 1, 2, 3.5, 5, 10, 15, 20])
# vin = np.array([9, 13, 20, 28, 33.5, 34, 34, 36, 36, 36])
# vout = np.array([9, 13, 19.5, 26, 27, 24, 20, 12, 8, 5.5])
# time_difference = np.array(
#     [0, 0, 0.04, 0.036, 0.040, 0.030, 0.026, 0.018, 0.010, 0.009]
# )
# cutoff_frequency = 3.5  # Hz
# filename = "low_pass_plot.pdf"

# band pass
frequency = np.array([0.3, 0.5, 0.7, 1, 2, 3, 3.5, 5, 10, 15])
vin = 36.5  # Vin is constant at 36.5 mV for all frequencies
vout = np.array([6.5, 9.5, 12, 13.5, 14.5, 13, 12, 10, 6, 4])
time_difference = np.array(
    [-3.90, -2.25, -1.55, -1.05, -0.49, -0.32, -0.266, -0.181, -0.084, -0.0547]
)
cutoff_frequency = 0.7
cutoff_frequency2 = 3.5
filename = "band_pass_plot.pdf"

# Calculations
gain = 20 * np.log10(vout / vin)  # gain in dB
phase = (time_difference * frequency) * 2 * pi  # phase in degrees

# Plotting
plt.rcParams.update({"font.size": 10, "text.usetex": True})
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))

# Gain plot
ax1.semilogx(frequency, gain, "b-o")
ax1.set_title("$$\\textrm{Gain (dB) vs Frequency (Hz)}$$")
ax1.set_xlabel("$$\\textrm{Frequency (Hz)}$$")
ax1.set_ylabel("$$\\textrm{Gain (dB)}$$")
ax1.grid(True, which="both", ls="--")
ax1.axvline(
    x=cutoff_frequency,
    color="k",
    linestyle="--",
    label="$$\\textrm{Cutoff frequency}$$",
)


# Phase plot
ax2.semilogx(frequency, phase, "r-o")
ax2.set_title("$$\\textrm{Phase (Radians) vs Frequency (Hz)}$$")
ax2.set_xlabel("$$\\textrm{Frequency (Hz)}$$")
ax2.set_ylabel("$$\\textrm{Phase (Radians)}$$")
ax2.grid(True, which="both", ls="--")
ax2.axvline(
    x=cutoff_frequency,
    color="k",
    linestyle="--",
    label="$$\\textrm{Cutoff frequency}$$",
)

if "cutoff_frequency2" in globals():
    ax1.axvline(x=cutoff_frequency2, color="k", linestyle="--")
    ax2.axvline(x=cutoff_frequency2, color="k", linestyle="--")

ax1.legend()
ax2.legend()

# save plot to file
plt.tight_layout()
plt.savefig(filename)
