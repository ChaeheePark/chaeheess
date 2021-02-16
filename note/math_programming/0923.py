def calculate_rectangle_area(x,y):
    return x*y
rectangle_x=10
rectangle_y=20
print("사각형 x의 길이: ",rectangle_x)
print("사각형 y의 길이: ",rectangle_y)
print("사각형의 넓이: ",calculate_rectangle_area(rectangle_x,rectangle_y))

def f(x):
    return 2*x+7
def g(x):
    return x**2

x=2
print(f(x)+g(x)+f(g(x))+g(f(x)))
