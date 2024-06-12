from random import randint
from sys import argv
import threading

num_threads = int(argv[1])
array_size = 1000000
if num_threads < 1:
    raise Exception("Minimum 1 thread must be created")

array = list(range(array_size))
sum_mutex = threading.Lock()
sum_value = 0

def sum_array(start, end, i):
    global sum_value

    local_sum = sum(array[start:end])

    print(f"Thread  {i} counted from {start} to {end}")

    with sum_mutex:
        sum_value += local_sum


threads = []

chunk_size = array_size // num_threads

for i in range(num_threads):
    start = i * chunk_size
    end = (i + 1) * chunk_size if i != num_threads - 1 else array_size

    thread = threading.Thread(target=sum_array, args=(start, end, i))
    threads.append(thread)
    thread.start()

for thread in threads:
    thread.join()

print("Sum:", sum_value)