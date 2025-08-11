import time
from multiprocessing import Process, Queue, Manager
from threading import Thread
import numpy as np
import datetime
import os
from numba import jit
@jit
def bubble_sort(data):
    n = len(data)
    for i in range(n):
        for j in range(0, n-i-1):
            if data[j] > data[j+1]:
                data[j], data[j+1] = data[j+1], data[j]
if __name__ == "__main__":
    file_name = input("Enter the file name: ")
    k = int(input("Enter the number of partitions: "))
    choice = int(input("Enter the method number (1, 2, 3, or 4): "))

    with open(file_name, 'r') as f:
        data = [int(line.strip()) for line in f]

    start_time = time.time()

    if choice == 1:
        sorted_data = bubble_sort(data)

    end_time = time.time()
    output_file_name = os.path.splitext(file_name)[0].split('_input')[0] + f"_output{choice}.txt"
    with open(output_file_name, 'w') as f:
        f.write("Sort:\n")
        for item in sorted_data:
            f.write(f"{item}\n")
        f.write(f"CPU Time : {end_time - start_time}\n")
        f.write(f"Output Time : {datetime.datetime.now()}")

    print(f"Sorted data has been saved to {output_file_name}")