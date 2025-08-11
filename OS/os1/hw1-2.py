from multiprocessing import Process, Queue, Manager
from threading import Thread
import random
# Bubble Sort
def bubble_sort(data, q):
    #print("BubbleSortProcess PID:", os.getpid())
    n = len(data)
    for i in range(n):
        for j in range(0, n-i-1):
            if data[j] > data[j+1]:
                data[j], data[j+1] = data[j+1], data[j]
    q.put(data)

def merge_sort(left, right, q):
    nL = len(left)
    nR = len(right)
    
    result = []
    i = j = 0

    while i < nL and j < nR:
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    while i < nL:
        result.append(left[i])
        i += 1

    while j < nR:
        result.append(right[j])
        j += 1

    q.put(result)
def method1(data):
    manager = Manager()
    q = manager.Queue()
    bubble_sort(data,q)
    sorted_data = q.get()
    return sorted_data
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

def method3(data, k):
    data_parts = split_data_into_k_parts(data, k)
    process_list = [] 
    merge_list = []
    manager = Manager()
    q = manager.Queue()

    for part in data_parts:
        p = Process(target=bubble_sort, args=(part, q))
        p.start()
        process_list.append(p)

    for p in process_list:
        p.join()

    for _ in range(k-1):
        left = q.get()
        right = q.get()
        merge_process = Process(target=merge_sort, args=(left, right, q))
        merge_process.start()
        merge_list.append(merge_process)

    for merge_process in merge_list:
        merge_process.join()

    sorted_data = q.get()
    manager.shutdown()
    return sorted_data
def method2(data, k):
    manager = Manager()
    q = manager.Queue()
    data_parts = split_data_into_k_parts(data, k)
    # Bubble Sort each sub-array
    for part in data_parts:
        bubble_sort(part,q)
    # Merge Sort the sorted sub-arrays
    for _ in range(k-1):
        left = q.get()
        right = q.get()
        merge_sort( left, right, q )
    sorted_data = q.get()
    return sorted_data
def method4(data, k):
    manager = Manager()
    q = manager.Queue()
    data_parts = split_data_into_k_parts(data, k)
    threads_list = []
    merge_list =[]
    for part in data_parts:
        t = Thread(target=bubble_sort, args=(part, q))
        threads_list.append(t)
        t.start()
    for t in threads_list:
        t.join()
    for _ in range(k-1):
        left = q.get()
        right = q.get()
        merge_thread = Thread(target=merge_sort, args=(left, right, q))
        merge_thread.start()
        merge_list.append(merge_thread)

    for merge_thread in merge_list:
        merge_thread.join()

    sorted_data = q.get()
    return sorted_data
# 測試
if __name__ == "__main__":
    '''
    # 生成 N 筆數字
    N = 50000
    data = [random.randint(1, 10000) for _ in range(N)]
    # 將數字分為 K 份，K=4
    k = 4
    data2 = data
    data = data2
    sorted_data = method2(data,k)
    print("Sorted Data for 2:", sorted_data[:10])
    data = data2
    sorted_data = method4(data,k)
    print("Sorted Data for 4:", sorted_data[:10])
    sorted_data = method3(data,k)
    print("Sorted Data for 3:", sorted_data[:10])
'''
    file_name = input("請輸入檔案名稱：")
    k = int(input("請輸入要切成幾份："))
    choice = int(input("請輸入方法編號(方法1,方法2,方法3,方法4):"))

    with open(file_name, 'r') as f:
        data = [int(line.strip()) for line in f]

    if choice == 1:
        sorted_data = method1(data)
    elif choice == 2:
        sorted_data = method2(data, k)
    elif choice == 3:
        sorted_data = method3(data, k)
        data = sorted_data
    elif choice == 4:
        sorted_data = method4(data, k)
    print(sorted_data[:10])
