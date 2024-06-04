from os.path import exists
from os import remove
from time import sleep
from sys import argv

if "-s" in argv:
    open("close.txt", "w").write("To shut down the server remove this file")
    print("To shut down the server remove close.txt")
    last_change = 0
    while exists("close.txt"):
        sleep(2)
        if exists("data.txt"):
            data = open("data.txt", "r").read()
            if last_change != data:
                open("result.txt", "w").write(f"{pow(2, int(data))}")
                last_change = data
    if exists("data.txt"):
        remove("data.txt")
    if exists("result.txt"):
        remove("result.txt")

else:
    open("data.txt", "w").write(f"{argv[1]}")
