import time
from multiprocessing import Process, Manager
from threading import Thread
import datetime
import os

def bubble_sort(data, q):
    for i in range(len(data)):
        for j in range(0, len(data)-i-1):
            if data[j] > data[j+1]:
                temp = data[j]
                data[j] = data[j+1]
                data[j+1] = temp
    q.put(data)

def method1(data):
    manager = Manager()
    q = manager.Queue()
    bubble_sort(data,q)
    sorted_data = q.get()
    return sorted_data

def merge_sort(left, right, q):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    while i < len(left):
        result.append(left[i])
        i += 1
    while j < len(right):
        result.append(right[j])
        j += 1
    q.put(result)

def split_data_into_k_parts(data, k):
    data_parts = []
    data_len = len(data)
    part_size = data_len // k
    for i in range(k):
        if i == k-1:
            data_parts.append(data[i*part_size:])
        else:
            data_parts.append(data[i*part_size:(i+1)*part_size])
    return data_parts

def method2(data, k):
    manager = Manager()
    q = manager.Queue()
    data_parts = split_data_into_k_parts(data, k)
    for part in data_parts:
        bubble_sort(part,q)
    for _ in range(k-1):
        l = q.get()
        r = q.get()
        merge_sort( l, r, q )
    sorted_data = q.get()
    return sorted_data

def method3(data, k):
    manager = Manager()
    q = manager.Queue()
    data_parts = split_data_into_k_parts(data, k)
    process_list = [] 
    process_list2 = []

    for part in data_parts: #由K個processes各別進行BubbleSort
        bp = Process(target=bubble_sort, args=(part, q))
        bp.start()
        process_list.append(bp)

    for bp in process_list:
        bp.join()

    for _ in range(k-1): #再用K-1個process(es)作MergeSort
        l = q.get()
        r = q.get()
        mp = Process(target=merge_sort, args=(l, r, q))
        mp.start()
        process_list2.append(mp)
        print(mp.pid)
    print(len(process_list2))
    print(len(process_list))

    for mp in process_list2:
        mp.join()

    sorted_data = q.get()
    return sorted_data

def method4(data, k):
    manager = Manager()
    q = manager.Queue()
    data_parts = split_data_into_k_parts(data, k)
    threads_list = []
    threads_list2 =[]

    for part in data_parts: #由K個threads各別進行BubbleSort
        bt = Thread(target=bubble_sort, args=(part, q))
        threads_list.append(bt)
        bt.start()

    for bt in threads_list:
        bt.join()

    for _ in range(k-1): #再用K-1個thread(s)作MergeSort
        l = q.get()
        r = q.get()
        mt = Thread(target=merge_sort, args=(l, r, q))
        mt.start()
        threads_list2.append(mt)
        print(mt.pid)
    print(len(threads_list2))
    print(len(threads_list))
    for mt in threads_list2:
        mt.join()

    sorted_data = q.get()
    return sorted_data



if __name__ == "__main__":
    file_name = input("請輸入檔案名稱：")
    k = int(input("請輸入要切成幾份："))
    choice = int(input("請輸入方法編號(方法1,方法2,方法3,方法4):"))

    with open(file_name, 'r') as f:
        data = [int(line.strip()) for line in f]

    start_time = time.time()

    if choice == 1:
        sorted_data = method1(data)
    elif choice == 2:
        sorted_data = method2(data, k)
    elif choice == 3:
        sorted_data = method3(data, k)
        data = sorted_data
    elif choice == 4:
        sorted_data = method4(data, k)

    end_time = time.time()
    output_file_name = os.path.splitext(file_name)[0].split('_input')[0] + f"_output{choice}.txt"
    with open(output_file_name, 'w') as f:
        f.write("Sort：\n")
        for item in sorted_data:
            f.write(f"{item}\n")
        f.write(f"CPU Time : {end_time - start_time}\n")
        f.write(f"Output Time : {datetime.datetime.now()}")

    print(f"Sorted data has been saved to {output_file_name}")
