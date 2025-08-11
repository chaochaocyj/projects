import copy

class Process:
    def __init__(self, id, waiting_time, turnaround_time):
        self.ID = id
        self.WaitingTime = waiting_time
        self.TurnaroundTime = turnaround_time

def check_in_list(lst, process):
    for p in lst:
        if p['ID'] == process['ID']:
            return True
    return False

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
                    if shortest_job['ID'] == p.ID: #已經在裡面了
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
    
    # Print the process list if needed
    process_list = sorted(process_list, key=lambda x: (x.ID))
    for p in process_list:
        print(f'Process ID: {p.ID}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')

# Test data
data1 = {
    'time_slice': 1,
    'processes': [
        {'ID': 5, 'CPUBurst': 6, 'ArrivalTime': 26, 'Priority': 13},
        {'ID': 13, 'CPUBurst': 1, 'ArrivalTime': 7, 'Priority': 2},
        {'ID': 6, 'CPUBurst': 5, 'ArrivalTime': 1, 'Priority': 7},
        {'ID': 27, 'CPUBurst': 6, 'ArrivalTime': 3, 'Priority': 7},
        {'ID': 2, 'CPUBurst': 3, 'ArrivalTime': 30, 'Priority': 13},
        {'ID': 1, 'CPUBurst': 2, 'ArrivalTime': 13, 'Priority': 5},
        {'ID': 9, 'CPUBurst': 4, 'ArrivalTime': 1, 'Priority': 6},
        {'ID': 10, 'CPUBurst': 8, 'ArrivalTime': 2, 'Priority': 13},
        {'ID': 0, 'CPUBurst': 4, 'ArrivalTime': 36, 'Priority': 1},
        {'ID': 8, 'CPUBurst': 2, 'ArrivalTime': 23, 'Priority': 12},
        {'ID': 7, 'CPUBurst': 1, 'ArrivalTime': 3, 'Priority': 16},
        {'ID': 29, 'CPUBurst': 6, 'ArrivalTime': 20, 'Priority': 8},
        {'ID': 4, 'CPUBurst': 3, 'ArrivalTime': 18, 'Priority': 10},
        {'ID': 20, 'CPUBurst': 3, 'ArrivalTime': 15, 'Priority': 14},
        {'ID': 3, 'CPUBurst': 4, 'ArrivalTime': 22, 'Priority': 3}
    ] 
}
data2 = {
    'time_slice': 3,
    'processes': [
        {'ID': 1, 'CPUBurst': 11, 'ArrivalTime': 0, 'Priority': 1},
        {'ID': 3, 'CPUBurst': 3, 'ArrivalTime': 3, 'Priority': 8},
        {'ID': 2, 'CPUBurst': 2, 'ArrivalTime': 1, 'Priority': 12},
        {'ID': 5, 'CPUBurst': 6, 'ArrivalTime': 7, 'Priority': 11},
        {'ID': 4, 'CPUBurst': 2, 'ArrivalTime': 5, 'Priority': 11},
    ]      
}
data3 = {
    'time_slice': 10,
    'processes': [
        {'ID': 1, 'CPUBurst': 20, 'ArrivalTime': 0, 'Priority': 1},
        {'ID': 2, 'CPUBurst': 25, 'ArrivalTime': 25, 'Priority': 3},
        {'ID': 3, 'CPUBurst': 25, 'ArrivalTime': 30, 'Priority': 3},
        {'ID': 4, 'CPUBurst': 15, 'ArrivalTime': 60, 'Priority': 2},
        {'ID': 5, 'CPUBurst': 10, 'ArrivalTime': 100, 'Priority': 5},
        {'ID': 6, 'CPUBurst': 10, 'ArrivalTime': 105, 'Priority': 4},
    ]
}
data4 = {
    'time_slice': 2,
    'processes': [
        {'ID': 1, 'CPUBurst': 6, 'ArrivalTime': 0, 'Priority': 3},
        {'ID': 2, 'CPUBurst': 3, 'ArrivalTime': 2, 'Priority': 1},
        {'ID': 3, 'CPUBurst': 6, 'ArrivalTime': 5, 'Priority': 3},
        {'ID': 4, 'CPUBurst': 5, 'ArrivalTime': 8, 'Priority': 2},
    ]   
}

# Run the method
method_PPRR(data1)
method_PPRR(data2)
method_PPRR(data3)
method_PPRR(data4)
