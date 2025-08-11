import os
import copy
import queue
import collections

class Process:
    def __init__(self, id, WaitingTime, TurnaroundTime):
        self.id = id
        self.WaitingTime = WaitingTime
        self.TurnaroundTime = TurnaroundTime
class Process_of_WaitingTime:
    def __init__(self, id, wfcfs, wrr, wsjf, wsrtf, whrrn, wpprr):
        self.id = id
        self.wfcfs = wfcfs
        self.wrr = wrr
        self.wsjf = wsjf
        self.wsrtf = wsrtf
        self.whrrn = whrrn
        self.wpprr = wpprr

class Process_of_TurnaroundTime:
    def __init__(self, id, tfcfs, trr, tsjf, tsrtf, thrrn, tpprr):
        self.id = id
        self.tfcfs = tfcfs
        self.trr = trr
        self.tsjf = tsjf
        self.tsrtf = tsrtf
        self.thrrn = thrrn
        self.tpprr = tpprr
    
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
    str1 ="==        FCFS=="
    gantt_chart = []
    current_time = 0
    process_list = []
    for process in data['processes']:
        if process['ArrivalTime'] > current_time: #目前時間下一個process還沒進來
            start_time = current_time
            end_time = process['ArrivalTime']
            num = end_time - start_time
            current_time = process['ArrivalTime']#直接跳到下一個來的時間
            gantt_chart.extend(["-"] * num)

        start_time = current_time
        end_time = start_time + process['CPUBurst']#做完一整個
        current_time = end_time
        if process['ID'] >= 10:
            word = chr(ord('A') + process['ID'] - 10)
        else:
            word = str(process['ID'])
        gantt_chart.extend([word] * process['CPUBurst'])
        item = Process(process['ID'],end_time - process['ArrivalTime'] - process['CPUBurst'], end_time - process['ArrivalTime'])
        process_list.append(item)
    print("".join(gantt_chart))
    str2 = "".join(gantt_chart)
    process_list = sorted(process_list, key=lambda x: (x.id))

    #for p in process_list:
    #    print(f'Process ID: {p.id}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')
    return str1,str2,process_list
def method_RR(data):
    print("==          RR==")
    str1 = "==          RR=="
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

            shortest_job = ready_process.get()

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
            if shortest_job['CPUBurst'] == 0:  # 做完了
                for p in data['processes']:
                    if p['ID'] == shortest_job['ID']:
                        runtime = p['CPUBurst']
                turnaround_time = current_time - shortest_job['ArrivalTime']
                waiting_time = current_time - shortest_job['ArrivalTime'] - runtime
                exist = False
                for p in process_list:
                    if shortest_job['ID'] == p.id: #已經在裡面了
                        exist = True
                if not exist: #不要重複加到
                    process_list.append(Process(shortest_job['ID'], waiting_time, turnaround_time))
            else: #還沒好
                for process in processes : # 先檢查有沒有新東西來了
                    if process['ArrivalTime'] <= current_time and process['CPUBurst'] != 0 and process['ID'] != shortest_job['ID']:
                        if not check_in_queue(ready_process,process):#如果不在queue裡，才能進去
                            ready_process.put(process)
                ready_process.put(shortest_job)
    

    print("".join(gantt_chart))
    str2 = "".join(gantt_chart)
    process_list = sorted(process_list, key=lambda x: (x.id))
    #for p in process_list:
    #    print(f'Process ID: {p.id}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')
    return str1, str2, process_list
def method_SJF(data):
    print("==         SJF==")
    str1 = "==         SJF=="
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
    str2 = "".join(gantt_chart)
    process_list = sorted(process_list, key=lambda x: (x.id))
    #for p in process_list:
    #    print(f'Process ID: {p.id}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')
    return str1, str2, process_list
def method_SRTF(data):
    print("==        SRTF==")
    str1 = "==        SRTF=="
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
            if shortest_job['CPUBurst'] == 0:  # 做完了
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

    print("".join(gantt_chart))
    str2 = "".join(gantt_chart)
    process_list = sorted(process_list, key=lambda x: (x.id))
    #for p in process_list:
    #    print(f'Process ID: {p.id}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')
    return str1,str2,process_list
def method_HRRN(data):
    print("==        HRRN==")
    str1 = "==        HRRN=="
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
            if shortest_job['CPUBurst'] == 0:  # 做完了
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

    print("".join(gantt_chart))
    str2 = "".join(gantt_chart)
    process_list = sorted(process_list, key=lambda x: (x.id))
    #for p in process_list:
    #    print(f'Process ID: {p.id}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')
    return str1,str2,process_list
def method_PPRR(data):
    print("==        PPRR==")
    str1 = "==        PPRR=="
    gantt_chart = []
    current_time = 0
    time_slice = data['time_slice']
    processes = copy.deepcopy(data['processes'])
    ready_queue = []
    process_list = []

    while any(process['CPUBurst'] > 0 for process in processes):
        # 檢查有沒有新來的
        for process in processes:
            if process['ArrivalTime'] <= current_time and process['CPUBurst'] > 0:
                if not check_in_list(ready_queue, process):
                    ready_queue.append(process)
                    ready_queue = sorted(ready_queue, key=lambda x: x['Priority'])
        if len(ready_queue) == 0:  #沒東西在裡面
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

            if shortest_job['CPUBurst'] == 0:  # 做完了
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
    str2 = "".join(gantt_chart)
    process_list = sorted(process_list, key=lambda x: (x.id))
    #for p in process_list:
    #    print(f'Process ID: {p.id}, Waiting Time: {p.WaitingTime}, Turnaround Time: {p.TurnaroundTime}')
    return str1,str2,process_list

def tidy(process_list_fcfs, process_list_rr, process_list_sjf,
        process_list_srtf, process_list_hrrn, process_list_pprr):
    list_id = []
    wtlist = []
    ttlist = []
    for i in process_list_fcfs:
        list_id.append(i.id)
        target = i.id
        for a in process_list_fcfs:
            if target == a.id:
                wfcfs = a.WaitingTime
        for a in process_list_rr:
            if target == a.id:
                wrr = a.WaitingTime
        for a in process_list_sjf:
            if target == a.id:
                wsjf = a.WaitingTime
        for a in process_list_srtf:
            if target == a.id:
                wsrtf = a.WaitingTime
        for a in process_list_hrrn:
            if target == a.id:
                whrrn = a.WaitingTime
        for a in process_list_pprr:
            if target == a.id:
                wpprr = a.WaitingTime
        item = Process_of_WaitingTime(i.id,wfcfs,wrr,wsjf,wsrtf,whrrn,wpprr)
        wtlist.append(item)

    for i in process_list_fcfs:
        list_id.append(i.id)
        target = i.id
        for a in process_list_fcfs:
            if target == a.id:
                wfcfs = a.TurnaroundTime
        for a in process_list_rr:
            if target == a.id:
                wrr = a.TurnaroundTime
        for a in process_list_sjf:
            if target == a.id:
                wsjf = a.TurnaroundTime
        for a in process_list_srtf:
            if target == a.id:
                wsrtf = a.TurnaroundTime
        for a in process_list_hrrn:
            if target == a.id:
                whrrn = a.TurnaroundTime
        for a in process_list_pprr:
            if target == a.id:
                wpprr = a.TurnaroundTime
        item = Process_of_TurnaroundTime(i.id,wfcfs,wrr,wsjf,wsrtf,whrrn,wpprr)
        ttlist.append(item)
    return wtlist,ttlist

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
        str_title = "FCFS"
        str_fcfs, str_gantt_fcfs, process_list_fcfs = method_FCFS(data)
    elif data['method'] == 2:
        str_title = "RR"
        str_rr, str_gantt_rr, process_list_rr =method_RR(data)
    elif data['method'] == 3:
        str_title = "SJF"
        str_sjf, str_gantt_sjf, process_list_sjf =method_SJF(data)
    elif data['method'] == 4:
        str_title = "SRTF"
        str_srtf, str_gantt_srtf, process_list_srtf =method_SRTF(data)
    elif data['method'] == 5:
        str_title = "HRRN"
        str_hrrn, str_gantt_hrrn, process_list_hrrn =method_HRRN(data)
    elif data['method'] == 6:
        str_title = "Priority RR"
        str_pprr, str_gantt_pprr, process_list_pprr =method_PPRR(data)
    elif data['method'] == 7:
        str_title = "All"
        str_fcfs, str_gantt_fcfs, process_list_fcfs = method_FCFS(data)
        str_rr, str_gantt_rr, process_list_rr =method_RR(data)
        str_sjf, str_gantt_sjf, process_list_sjf =method_SJF(data)
        str_srtf, str_gantt_srtf, process_list_srtf =method_SRTF(data)
        str_hrrn, str_gantt_hrrn, process_list_hrrn =method_HRRN(data)
        str_pprr, str_gantt_pprr, process_list_pprr =method_PPRR(data)
        wtlist,ttlist = tidy(process_list_fcfs, process_list_rr, process_list_sjf,
                                process_list_srtf, process_list_hrrn, process_list_pprr)

    input_file_name = os.path.splitext(file_name)[0].split('_input')[0]
    output_file_name = f"out_{input_file_name}.txt"
    with open(output_file_name, 'w') as f:
            f.write(str_title+"\n")
            if data['method'] == 1:
                f.write(str_fcfs+"\n")
                f.write(str_gantt_fcfs+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tFCFS\n")
                f.write("===========================================================\n")
                for i in process_list_fcfs:
                    f.write(str(i.id)+"\t"+str(i.WaitingTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tFCFS\n")
                f.write("===========================================================\n")
                for i in process_list_fcfs:
                    f.write(str(i.id)+"\t"+str(i.TurnaroundTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
            elif data['method'] == 2:
                f.write(str_rr+"\n")
                f.write(str_gantt_rr+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tRR\n")
                f.write("===========================================================\n")
                for i in process_list_rr:
                    f.write(str(i.id)+"\t"+str(i.WaitingTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tRR\n")
                f.write("===========================================================\n")
                for i in process_list_rr:
                    f.write(str(i.id)+"\t"+str(i.TurnaroundTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
            elif data['method'] == 3:
                f.write(str_sjf+"\n")
                f.write(str_gantt_sjf+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tSJF\n")
                f.write("===========================================================\n")
                for i in process_list_sjf:
                    f.write(str(i.id)+"\t"+str(i.WaitingTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tSJF\n")
                f.write("===========================================================\n")
                for i in process_list_sjf:
                    f.write(str(i.id)+"\t"+str(i.TurnaroundTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
            elif data['method'] == 4:
                f.write(str_srtf+"\n")
                f.write(str_gantt_srtf+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tSRTF\n")
                f.write("===========================================================\n")
                for i in process_list_srtf:
                    f.write(str(i.id)+"\t"+str(i.WaitingTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tSRTF\n")
                f.write("===========================================================\n")
                for i in process_list_srtf:
                    f.write(str(i.id)+"\t"+str(i.TurnaroundTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
            elif data['method'] == 5:
                f.write(str_hrrn+"\n")
                f.write(str_gantt_hrrn+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tHRRN\n")
                f.write("===========================================================\n")
                for i in process_list_hrrn:
                    f.write(str(i.id)+"\t"+str(i.WaitingTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tHRRN\n")
                f.write("===========================================================\n")
                for i in process_list_hrrn:
                    f.write(str(i.id)+"\t"+str(i.TurnaroundTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
            elif data['method'] == 6:
                f.write(str_pprr+"\n")
                f.write(str_gantt_pprr+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tPPRR\n")
                f.write("===========================================================\n")
                for i in process_list_pprr:
                    f.write(str(i.id)+"\t"+str(i.WaitingTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tPPRR\n")
                f.write("===========================================================\n")
                for i in process_list_pprr:
                    f.write(str(i.id)+"\t"+str(i.TurnaroundTime)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
            elif data['method'] == 7:
                f.write(str_fcfs+"\n")
                f.write(str_gantt_fcfs+"\n")                
                f.write(str_rr+"\n")
                f.write(str_gantt_rr+"\n")
                f.write(str_sjf+"\n")
                f.write(str_gantt_sjf+"\n")
                f.write(str_srtf+"\n")
                f.write(str_gantt_srtf+"\n")
                f.write(str_hrrn+"\n")
                f.write(str_gantt_hrrn+"\n")
                f.write(str_pprr+"\n")
                f.write(str_gantt_pprr+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Waiting Time\n")
                f.write("ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n")
                f.write("===========================================================\n")
                for i in wtlist:
                    f.write(str(i.id)+"\t"+str(i.wfcfs)+"\t"+str(i.wrr)+"\t"+str(i.wsjf)+"\t"+str(i.wsrtf)+"\t"+str(i.whrrn)+"\t"+str(i.wpprr)+"\n")
                f.write("===========================================================\n")
                f.write("\n")
                f.write("Turnaround Time\n")
                f.write("ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n")
                f.write("===========================================================\n")
                for i in ttlist:
                    f.write(str(i.id)+"\t"+str(i.tfcfs)+"\t"+str(i.trr)+"\t"+str(i.tsjf)+"\t"+str(i.tsrtf)+"\t"+str(i.thrrn)+"\t"+str(i.tpprr)+"\n")
                f.write("===========================================================\n")
                f.write("\n")



    print(f"Finish {output_file_name}")
