from math import sqrt
from random import randint
from sys import argv
import threading
from time import time

num_threads = int(argv[1])
upper_bound = int(argv[2])
primary_numbers = []
if num_threads < 1:
    raise Exception("Minimum 1 thread must be created")
if upper_bound < 1:
    raise Exception("Minimum 1 thread must be created")
if upper_bound < num_threads:
    raise Exception("Upper bound must be equal or greater than number of threads")


sum_mutex = threading.Lock()
barrier = threading.Barrier(num_threads + 1) 

def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(sqrt(num)) + 1):
        if num % i == 0:
            return False
    return True

def prime_array(start, end, i):
    local_primes = []

    for num in range(start, end  + 1):
        if is_prime(num):
            local_primes.append(num)

    print(f"Thread  {i} found {len(local_primes)} primes in <{start};{end}>")

    with sum_mutex:
        primary_numbers.extend(local_primes)
    barrier.wait()


threads = []
start = 2
chunk_size = (upper_bound - start + 1) // num_threads

for i in range(num_threads):
    thread_start = start + i * chunk_size
    thread_end = thread_start + chunk_size -1 if i != num_threads -1 else upper_bound


    thread = threading.Thread(target=prime_array, args=(thread_start, thread_end, i))
    threads.append(thread)
    thread.start()

time_start = time()
barrier.wait()
time_end = time()

duration = time_end - time_start

print(f"Found {len(primary_numbers)} in {duration}. Array: {primary_numbers}")
