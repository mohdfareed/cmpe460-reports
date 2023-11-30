x1 = 0.025
y1 = 0.25
y2 = 0.4

# find constants for 2nd order equation ()
# y = 1/a (x - yc) + xc
a = -(y1 - y2) / (x1**2)

print(f"y = -{a}x^2 + {y2}")
# print(f"a = -({x1}^2 / ({y1} - {y2}) = {a}")
