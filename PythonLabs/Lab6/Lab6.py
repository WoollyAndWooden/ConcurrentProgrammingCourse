
foo1 = lambda a, b, c : print("Prostokatny") if a*a + b*b == c*c else print("Nieprostokatny")
foo1(3, 4, 5)
foo1(3, 3, 8)

def foo2():
    d = {"a": 1, "b": 2, "c": 3, "d": 4, "e": 5}
    for i in d.keys():
        print(f"{i} {d[i]}")

foo2()

def foo3(*a):
    res = 1
    for i in a:
        res*=i
    return res

print(foo3(1, 2, 3, 4, 5 ,6))

def foo4(x):
    i=1
    x=abs(x)
    while(i*i <= x):
        if i*i == x:
            return True
        i+=1
    return False

print(foo4(25))
print(foo4(10))
print(foo4(-64))

def foo6(x, *a):
    if len(a) == 1:
        return a[0]

    return a[len(a)-1]*pow(x, len(a)-1) + foo6(x, a[0:len(a)-2])

print(foo6(1, 1, 2, 3))

list = [13, 21, 0, 22]

new_list = [num for num in list if num % 2 == 0]
print(new_list)
random.ra