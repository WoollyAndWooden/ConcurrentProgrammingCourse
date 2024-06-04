from random import randint

# 1
# tuple, str and int. also works with bool, and float. As far as I know not wilth lists.


# 2
def foo11(grades: list[int]) -> list[str]:
    res = []
    for i in grades:
        if i >= 90:
            res.append("A")
        elif i >= 80:
            res.append("B")
        elif i >= 70:
            res.append("C")
        else:
            res.append("D")
    return res


print(foo11([85, 92, 78, 65, 95, 88]))


def foo1(n: int | float | dict, *a: int | float) -> list[int | float]:
    return [num for num in a if num > n]


print(foo1(10, 5, 2, 20, 10, 11, 23, 6))
print(foo1(9.5, 5, 2, 20, 10, 11, 23, 6, 3.2, 11.8))


def foo2(a: int | float, b: int | float, c: int | float) -> dict[str, bool]:
    return {
        "Prostakatny": False if a * a + b * b != c * c else True,
        "Rownoramienny": False if a != b else True,
        "Rownoboczny": True if a == b and a == c and b == c else False,
    }


print(foo2(3, 4, 5))
print(foo2(3, 3, 3))
print(foo2(4, 4, 5))


def foo3(a: int, b: int) -> tuple[int, int]:
    NWa, NWb = [a], [b]
    GCD = 1

    for i in range(1, max(a, b)):
        NWa.append(i * a)
        NWb.append(i * b)
        if a % i == 0 and b % i == 0:
            GCD = i

    NWa.sort()
    NWb.sort()
    print(NWa, NWb)
    i, j = 0, 0
    while i < a * b or j < a * b:
        if NWa[i] > NWb[j]:
            j += 1
        if NWa[i] < NWb[j]:
            i += 1
        if NWa[i] == NWb[j]:
            return NWa[i], GCD
    return a * b, GCD


print(foo3(15, 5))


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    # Przeładowanie operatora porównania byłoby lepsze, ale nie jestem pewny czy python to wspiera, jak wspiera
    # nie korzystałem z tego.
    def isEqual(self, b: "Point") -> True:
        return True if self.x == b.x and self.y == b.y else False

    # def CalcDistance(self, b: "Point") -> float:
    # Mój problem z matmą jest taki, że bardzo szybko zapominam wzorów. Musialbym poszukać wzór na przeciwprostokątną,
    # który wiem że wykorzystuje pierwiastek (sqrt jest w cmath). Zadania następnego nie zrobiłem,
    # ponieważ trygonometria to moja pięta achillesowa. Nie umiem.


def foo6(n: int) -> None:
    def printstats(l: list[int], sum: int):

        print(f"suma {sum}")
        d = [0, 0, 0, 0]

        for i in l:
            d[i - 1] += 1
        lowest = [1, d[0]]
        highest = [1, d[0]]
        for j in range(1, 4):
            if d[j - 1] < lowest[1]:
                lowest = [j, d[j - 1]]
            if d[j - 1] > highest[1]:
                highest = [j, d[j - 1]]
        print(d)
        print(l)
        print(f"lowest: {lowest}, highest: {highest}")

    sum = 0
    res = []
    for i in range(0, n - 1):
        res.append(randint(1, 4))
        sum += res[i]
        if (
            sum == 121
        ):  # original requirement was to have >= here. Changed it for testing and higher randomness
            print("won")
            return printstats(res, sum)

    return printstats(res, sum)


foo6(392)


def genFib(n: int):
    a, b = 0, 1
    yield a
    yield b
    for i in range(2, n):
        yield a + b
        c = b
        b += a
        a = c


gen = genFib(15)

for i in range(0, 15):
    print(next(gen))
