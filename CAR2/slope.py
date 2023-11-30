xc = 63.5
x1 = 61.0
x2 = 66

yc = 0.075
y1 = 0.1
y2 = 0.05

# find constants for 2nd order equation ()
# y = 1/a (x - yc) + xc
a = ((x1 - xc) ** 2) / (y1 - yc)

print(f"y = 1/{a}(x - {xc})^2 + {yc}")
# print(f"a = ({x1} - {xc})^2 / ({y1} - {yc}) = {a}")
