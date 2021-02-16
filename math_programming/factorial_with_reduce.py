from functools import reduce

def factorial_with_reduce(n):
    return reduce(lambda x,y: x*y ,range(1,n+1))


print(factorial_with_reduce(5))
print(factorial_with_reduce(20))
