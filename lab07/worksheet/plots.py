import os
import numpy as np
import matplotlib.pyplot as plt

# Data for PART6
frequency_PART6 = np.array(
    [100, 500, 5000, 10000, 50000, 80000, 100000, 200000, 300000, 500000]
)
gain_dB_PART6 = np.array(
    [7.0, 6.7, 6.4, 6.4, 5.1, 0.2, -1.6, -10.1, -16.9, -24.9]
)
phase_angle_PART6 = np.array(
    [5.7, 5.5, 9, 13.5, 148.2, 175.4, 177.7, 191, 205.3, 237]
)

# Data for PART7
frequency_PART7 = np.array(
    [100, 500, 5000, 10000, 50000, 80000, 100000, 200000, 300000, 500000]
)
gain_dB_PART7 = np.array(
    [-20.47, -20.28, -5.58, 5.11, 5.16, 4.69, 4.47, 2.38, 0.27, -3.03]
)
phase_angle_PART7 = np.array([-33, -42, -14, 0, -10, 10, 20, 42, 54, 72])

# Data for PART8
frequency_PART8 = np.array(
    [500, 1000, 10000, 20000, 50000, 80000, 100000, 200000, 500000, 1000000]
)
gain_dB_PART8 = np.array(
    [-26.62, -22.69, -7.47, -3.52, -0.57, -0.76, -1.58, -5.30, -11.37, -16.67]
)
phase_angle_PART8 = np.array(
    [134, 130, 120, 140, 180, 210, 220, 240, 280, 310]
)

# config
images_path = os.path.dirname(os.path.abspath(__file__)) + "/images/"
plt.rcParams["text.usetex"] = True
plt.figure(figsize=(20, 10))

# plot Gain for PART6, PART7, and PART8

plt.subplot(2, 3, 1)
plt.plot(frequency_PART6, gain_dB_PART6, "bo-", label="PART6")
plt.xscale("log")  # Logarithmic scale for frequency
plt.axvline(x=100000, color="k", linestyle="--", label="$f_c=100kHz$")
plt.xlabel("\\textrm{Frequency (Hz)}", fontsize=12)
plt.ylabel("\\textrm{Gain (dB)}", fontsize=12)
plt.title("\\textrm{Low Pass Gain vs Frequency}", fontsize=15)
plt.grid(True, which="both", ls="-")  # Grid for both major and minor ticks
plt.legend()

plt.subplot(2, 3, 2)
plt.plot(frequency_PART7, gain_dB_PART7, "bo-", label="PART7")
plt.xscale("log")  # Logarithmic scale for frequency
plt.axvline(x=10000, color="k", linestyle="--", label="$f_c=10kHz$")
plt.xlabel("\\textrm{Frequency (Hz)}", fontsize=12)
plt.ylabel("\\textrm{Gain (dB)}", fontsize=12)
plt.title("\\textrm{High Pass Gain vs Frequency}", fontsize=15)
plt.grid(True, which="both", ls="-")  # Grid for both major and minor ticks
plt.legend()

plt.subplot(2, 3, 3)
plt.plot(frequency_PART8, gain_dB_PART8, "bo-", label="PART8")
plt.xscale("log")  # Logarithmic scale for frequency
plt.axvline(x=10000, color="k", linestyle="--", label="$f_c=10kHz$")
plt.xlabel("\\textrm{Frequency (Hz)}", fontsize=12)
plt.ylabel("\\textrm{Gain (dB)}", fontsize=12)
plt.title("\\textrm{Band Pass Gain vs Frequency}", fontsize=15)
plt.grid(True, which="both", ls="-")  # Grid for both major and minor ticks
plt.legend()

# plot Phase for PART6, PART7, and PART8
plt.subplot(2, 3, 4)
plt.plot(frequency_PART6, phase_angle_PART6, "bo-", label="PART6")
plt.xscale("log")  # Logarithmic scale for frequency
plt.axvline(x=100000, color="k", linestyle="--", label="$f_c=100kHz$")
plt.xlabel("\\textrm{Frequency (Hz)}", fontsize=12)
plt.ylabel("\\textrm{Phase Angle (Degrees)}", fontsize=12)
plt.title("\\textrm{Low Pass Phase Angle vs Frequency}", fontsize=15)
plt.grid(True, which="both", ls="-")  # Grid for both major and minor ticks
plt.legend()

plt.subplot(2, 3, 5)
plt.plot(frequency_PART7, phase_angle_PART7, "bo-", label="PART7")
plt.xscale("log")  # Logarithmic scale for frequency
plt.axvline(x=10000, color="k", linestyle="--", label="$f_c=10kHz$")
plt.xlabel("\\textrm{Frequency (Hz)}", fontsize=12)
plt.ylabel("\\textrm{Phase Angle (Degrees)}", fontsize=12)
plt.title("\\textrm{High Pass Phase Angle vs Frequency}", fontsize=15)
plt.grid(True, which="both", ls="-")  # Grid for both major and minor ticks
plt.legend()

plt.subplot(2, 3, 6)
plt.plot(frequency_PART8, phase_angle_PART8, "bo-", label="PART8")
plt.xscale("log")  # Logarithmic scale for frequency
plt.axvline(x=10000, color="k", linestyle="--", label="$f_c=10kHz$")
plt.xlabel("\\textrm{Frequency (Hz)}", fontsize=12)
plt.ylabel("\\textrm{Phase Angle (Degrees)}", fontsize=12)
plt.title("\\textrm{Band Pass Phase Angle vs Frequency}", fontsize=15)
plt.grid(True, which="both", ls="-")  # Grid for both major and minor ticks
plt.legend()

# Adjust layout to prevent overlap
plt.tight_layout()
# Save the full figure
plt.savefig(images_path + "plots.pdf")
