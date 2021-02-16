import math
def get_result_quadratic_equation(a,b,c):
    values=[]
    values.append((-b+math.sqrt(b**2-(4*a*c)))/(2*a))
    values.append((-b-math.sqrt(b**2-(4*a*c)))/(2*a))
    return values

print(get_result_quadratic_equation(1,-2,1))
