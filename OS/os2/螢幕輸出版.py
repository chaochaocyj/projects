import os
import copy
import queue
import collections

class Process:
    def __init__(self, id, WaitingTime, TurnaroundTime):
        self.id = id
        self.WaitingTime = WaitingTime
        self.TurnaroundTime = TurnaroundTime

    
def check_in_queue(q, target):
    found = False
    new_q = queue.Queue() # 拿來檢查
    while not q.empty():
        item = q.get()
        new_q.put(item)
    while not new_q.empty():
        item = new_q.get()
        if item == target:
            found = True
        q.put(item)
    return found
def view_queue(q):
    elements = collections.deque(list(q.queue))
    print(elements)

def check_in_list(list,target):
    found = False
    new_list = copy.deepcopy(list)
    for i in new_list:
        if i['ID'] == int(target['ID']):
            found = True
    return found
def calculate_response_ratio(process, current_time):
        waiting_time = current_time - process['ArrivalTime']
        response_ratio = (waiting_time + process['CPUBurst']) / process['CPUBurst'] if process['CPUBurst'] != 0 else float('inf')
        return response_ratio, -process['ArrivalTime'], -process['ID']
def method_FCFS(data):
    print("==        FCFS==")
    gantt_chart = []
    current_time = 0
    process_list = []
    for process in data['processes']:
        # 模擬執行進程
        if process['ArrivalTime'] > current_time: #目前時間下一個process還沒進來
            start_time = current_time
            end_time = process['ArrivalTime']
            num = end_time - start_time
            current_time = process['ArrivalTime']
            gantt_chart.extend(["-"] * num)

        start_time = current_time
        end_time = start_time + process['CPUBurst']
        # 更新目前時間
        current_time = end_time
        # 加入到Gantt chart
        item = Process(process['ID'],end_time - process['ArrivalTime'] - process['CPUBurst'], end_time - process['ArrivalTime'])
        process_list.append(item)
        if process['ID'] >= 10:
            word = chr(ord('A') + process['ID'] - 10)
        else:
            word = str(process['ID'])
        gantt_chart.extend([word] * process['CPUBurst'])
    print("".join(gantt_chart))
    '''
    for i in range(len(process_list)):
        print(str(process_list[i].id)+" "+str(process_list[i].WaitingTime)+" "+str(process_list[i].TurnaroundTime))
    '''
def method_RR(data):
    print("==        RR==")
    gantt_chart = []
    current_time = 0
    time_slice = data['time_slice']
    processes = []
    process_list = []
    processes = copy.deepcopy(data['processes']) 
    ready_process = queue.Queue()
    while any(process['CPUBurst'] > 0 for process in processes):
        for process in processes : # 先檢查有沒有新東西來了
            if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0 and process['ID']:
                    if not check_in_queue(ready_process,process):#如果不在queue裡，才能進去
                        ready_process.put(process)
        if ready_process.empty(): #沒東西在裡面
            current_time += 1
            gantt_chart.append('-')
        else:
            '''
            print("我的queue長怎樣")
            view_queue(ready_process)
            '''
            shortest_job = ready_process.get()

            #print("我拿到啥")
            #print(shortest_job)

            if shortest_job['CPUBurst'] >= time_slice : #比時間刻度大 
                current_time =  current_time + time_slice 
                if shortest_job['ID'] >= 10:
                    word = chr(ord('A') + shortest_job['ID'] - 10)
                else:
                    word = str(shortest_job['ID'])
                gantt_chart.extend([word] * time_slice) 
            else:
                current_time =  current_time + shortest_job['CPUBurst']
                if shortest_job['ID'] >= 10:
                    word = chr(ord('A') + shortest_job['ID'] - 10)
                else:
                    word = str(shortest_job['ID'])
                gantt_chart.extend([word] * shortest_job['CPUBurst'])
            for process in processes:
                if process['ID'] == shortest_job['ID']:
                    if shortest_job['CPUBurst'] >= time_slice:
                        process['CPUBurst'] = process['CPUBurst'] - time_slice
                    else:
                        process['CPUBurst'] = 0
            if ( shortest_job['CPUBurst'] == 0 ): #執行完畢
                item = Process(shortest_job['ID'],current_time -shortest_job['ArrivalTime'] - shortest_job['CPUBurst'], current_time - shortest_job['ArrivalTime'])
                process_list.append(item)
            else: #還沒好
                for process in processes : # 先檢查有沒有新東西來了
                    if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0 and process['ID'] != shortest_job['ID']:
                        if not check_in_queue(ready_process,process):#如果不在queue裡，才能進去
                            ready_process.put(process)
                ready_process.put(shortest_job)
            '''
            print("我的queue長怎樣")
            view_queue(ready_process)
            '''
    

    print("".join(gantt_chart))

def method_SJF(data):
    print("==        SJF==")
    current_time = 0
    gantt_chart = []
    processes = []
    process_list = []
    processes = copy.deepcopy(data['processes']) 
    ready_process = [process for process in processes if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0]
    while any(process['CPUBurst'] > 0 for process in processes):
        ready_process = [process for process in processes if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0]
        if len(ready_process) == 0: #沒東西在裡面
            current_time += 1
            gantt_chart.append('-')
        else:
            shortest_job = min(ready_process, key=lambda x: (x['CPUBurst'], x['ArrivalTime'], x['ID']))
            #先比CPUBurst，再比ArrivalTime，最後比ID
            current_time = current_time + shortest_job['CPUBurst']#執行完他
            if shortest_job['ID'] >= 10:
                word = chr(ord('A') + shortest_job['ID'] - 10)
            else:
                word = str(shortest_job['ID'])
            gantt_chart.extend([word] * shortest_job['CPUBurst'])
            item = Process(shortest_job['ID'],current_time -shortest_job['ArrivalTime'] - shortest_job['CPUBurst'], current_time - shortest_job['ArrivalTime'])
            process_list.append(item)
            for process in processes:
                if process['ID'] == shortest_job['ID']:
                    process['CPUBurst'] = 0 # 讓他=0不會再進到ready_process裡面
    print("".join(gantt_chart))

def method_SRTF(data):
    print("==        SRTF==")
    current_time = 0
    gantt_chart = []
    processes = []
    process_list = []
    processes = copy.deepcopy(data['processes']) 
    ready_process = [process for process in processes if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0]
    while any(process['CPUBurst'] > 0 for process in processes):
        ready_process = [process for process in processes if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0]
        if len(ready_process) == 0: #沒東西在裡面
            current_time += 1
            gantt_chart.append('-')
        else:
            shortest_job = min(ready_process, key=lambda x: (x['CPUBurst'], x['ArrivalTime'], x['ID']))
            #先比CPUBurst，再比ArrivalTime，最後比xID
            current_time += 1   #執行他一個時間單位
            for process in processes:
                if process['ID'] == shortest_job['ID']:
                    process['CPUBurst'] -= 1 # 讓他=0時不會再進到ready_process裡面
            if shortest_job['ID'] >= 10:
                word = chr(ord('A') + shortest_job['ID'] - 10)
            else:
                word = str(shortest_job['ID'])           
            gantt_chart.extend([word] * 1)
            if ( shortest_job['CPUBurst'] == 0 ): #執行完畢
                item = Process(shortest_job['ID'],current_time -shortest_job['ArrivalTime'] - shortest_job['CPUBurst'], current_time - shortest_job['ArrivalTime'])
                process_list.append(item)

    print("".join(gantt_chart))
def method_HRRN(data):
    print("==        HRRN==")
    current_time = 0
    gantt_chart = []
    processes = []
    process_list = []
    processes = copy.deepcopy(data['processes']) 
    ready_process = [process for process in processes if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0]
    while any(process['CPUBurst'] > 0 for process in processes):
        ready_process = [process for process in processes if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0]
        if len(ready_process) == 0: #沒東西在裡面
            current_time += 1
            gantt_chart.append('-')
        else:
            shortest_job = max(ready_process, key=lambda p: calculate_response_ratio(p, current_time))
            #先比Ratio，再比ArrivalTime，最後比ID
            current_time = current_time + shortest_job['CPUBurst']   #執行他一個時間單位
            num = shortest_job['CPUBurst']
            for process in processes:
                if process['ID'] == shortest_job['ID']:
                    process['CPUBurst'] = 0 # 讓他=0時不會再進到ready_process裡面
            if shortest_job['ID'] >= 10:
                word = chr(ord('A') + shortest_job['ID'] - 10)
            else:
                word = str(shortest_job['ID'])           
            gantt_chart.extend([word] * num)
            if ( shortest_job['CPUBurst'] == 0 ): #執行完畢
                item = Process(shortest_job['ID'],current_time -shortest_job['ArrivalTime'] - shortest_job['CPUBurst'], current_time - shortest_job['ArrivalTime'])
                process_list.append(item)

    print("".join(gantt_chart))
def method_PPRR(data):
    print("==        PPRR==")
    gantt_chart = []
    current_time = 0
    time_slice = data['time_slice']
    processes = copy.deepcopy(data['processes'])
    ready_queue = []
    process_list = []

    while any(process['CPUBurst'] > 0 for process in processes):
        # Check for new arrivals
        for process in processes:
            if process['ArrivalTime'] <= current_time and process['CPUBurst'] > 0:
                if not check_in_list(ready_queue, process):
                    ready_queue.append(process)
                    ready_queue = sorted(ready_queue, key=lambda x: x['Priority'])
        if len(ready_queue) == 0:  # No processes are ready
            current_time += 1
            gantt_chart.append('-')
        else:
            shortest_job = ready_queue.pop(0)
            executed_time = 0
            while executed_time < time_slice and shortest_job['CPUBurst'] > 0:
                # 檢查有沒有新來的
                for process in processes:
                    if process['ArrivalTime'] <= current_time and process['CPUBurst'] > 0 and process['ID'] != shortest_job['ID']:
                        if not check_in_list(ready_queue, process):
                            ready_queue.append(process)
                            ready_queue = sorted(ready_queue, key=lambda x: x['Priority'])

                # 如果有人優先順序比較大，跳出迴圈
                if len(ready_queue) > 0 and ready_queue[0]['Priority'] < shortest_job['Priority']:
                    ready_queue.append(shortest_job)
                    ready_queue = sorted(ready_queue, key=lambda x: x['Priority'])
                    break
                #沒有的話再繼續執行原本的
                current_time += 1
                executed_time += 1
                shortest_job['CPUBurst'] -= 1
                word = chr(ord('A') + shortest_job['ID'] - 10) if shortest_job['ID'] >= 10 else str(shortest_job['ID'])
                gantt_chart.append(word)

            if shortest_job['CPUBurst'] == 0:  # Process is complete
                for p in data['processes']:
                    if p['ID'] == shortest_job['ID']:
                        runtime = p['CPUBurst']
                turnaround_time = current_time - shortest_job['ArrivalTime']
                waiting_time = current_time - shortest_job['ArrivalTime'] - runtime
                exist = False
                for p in process_list:
                    if shortest_job['ID'] == p.id: #已經在裡面了
                        exist = True
                if not exist:
                    process_list.append(Process(shortest_job['ID'], waiting_time, turnaround_time))
            elif executed_time == time_slice and shortest_job['CPUBurst'] > 0:  # 時間執行完但還沒完成，回去排隊
                for process in processes: #回去排隊前先看看有沒有新來的
                    if process['ArrivalTime'] <= current_time and process['CPUBurst'] > 0 and process['ID'] != shortest_job['ID']:
                        if not check_in_list(ready_queue, process):
                            ready_queue.append(process)
                            ready_queue = sorted(ready_queue, key=lambda x: x['Priority'])
                if not check_in_list(ready_queue, shortest_job):
                    ready_queue.append(shortest_job)
                    ready_queue = sorted(ready_queue, key=lambda x: x['Priority'])

    print("".join(gantt_chart))
def method_ALL(data):
    method_FCFS(data)
    method_RR(data)
    method_SJF(data)
    method_SRTF(data)
    method_HRRN(data)
    method_PPRR(data)
    #print(data['processes'])

     
if __name__ == "__main__":
    file_name = input("Please enter File Name:")
    data ={}
    with open(file_name, 'r') as file:
        method, time_slice = map(int, file.readline().split())
        data['method'] = method
        data['time_slice'] = time_slice
        headers = file.readline().split()
        
        headers[1] += headers.pop(2)  # 合併第二項與第三項
        headers[2] += headers.pop(3)
        data['headers'] = headers
        processes = []
        for line in file:
            values = line.split()
            if len(values) == len(headers):
                process = {headers[i]: int(values[i]) for i in range(len(headers))}
                processes.append(process)
        sorted_processes = sorted(processes, key=lambda x: (x['ArrivalTime'], x['ID']))
        #抵達時間排序，若抵達時間相同則process['ID']小者在前
        data['processes'] = sorted_processes
    if data['method'] == 1:
        print("FCFS")
        method_FCFS(data)
    elif data['method'] == 2:
        print("RR")
        method_RR(data)
    elif data['method'] == 3:
        print("SJF")
        method_SJF(data)
    elif data['method'] == 4:
        print("SRTF")
        method_SRTF(data)
    elif data['method'] == 5:
        print("HRRN") 
        method_HRRN(data)
    elif data['method'] == 6:
        print("PPRR")
        method_PPRR(data)
    elif data['method'] == 7:
        print("ALL")
        method_ALL(data)

    input_file_name = os.path.splitext(file_name)[0].split('_input')[0]
    output_file_name = f"out_{input_file_name}.txt"
    with open(output_file_name, 'w') as f:
            #他們用來確定有切對而已
            f.write(str(data['method'])+" ")
            f.write(str(data['time_slice'])+"\n")
            f.write(str(data['headers'])+"\n")
            for process in data['processes']:
                f.write('\t'.join(map(str, [process['ID'], process['CPUBurst'], process['ArrivalTime'], process['Priority']])))
                f.write('\n')
            


    print(f"Finish {output_file_name}")
