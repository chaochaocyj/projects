class Page:
    def __init__(self, id, time, counter):
        self.id = id
        self.time = time
        self.counter = counter
def checkin(item,list):
    return item in list    
def pagefaultlength(pagefault):
    count = 0
    for i in pagefault:
        if i == "F":
            count += 1
    return count
def FIFO(frame_size, page_string):
    pageframe = [] #用來記錄要輸出Page Frames
    pagefault = [] #用來記錄要輸出Page Fault
    pagelist = []  #用來記錄每個page的info
    replaces = 0
    time = 0
    for i in range(len(page_string)):
        time +=1
        if i == 0: #第一次進來
            string = str(page_string[i])
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("F")
        elif checkin(page_string[i],pageframe[i-1]): #已經在裡面了
            string = str(pageframe[i-1])
            pageframe.append(string)
            pagefault.append("")    
        elif not checkin(page_string[i],pageframe[i-1]) and len(pageframe[i-1]) < frame_size: #不在frame裡，frame也還沒滿
            string = str(page_string[i]) + pageframe[i-1]
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("F")
        elif not checkin(page_string[i],pageframe[i-1]) and  len(pageframe[i-1]) == frame_size: #不在frame裡，frame滿了，要置換
            target = min(pagelist, key=lambda i: (i.time)) # 看time
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("F")
            replaces += 1
    for i in range(len(pageframe)):
        print(str(page_string[i])+"\t"+str(pageframe[i])+"\t"+str(pagefault[i]))

    print("Page Fault = " + str(pagefaultlength(pagefault))+"\tPage Replaces = "+str(replaces)+"\tPage Frames = "+str(frame_size))
    return pageframe,pagefault,replaces
def LRU(frame_size, page_string):
    pageframe = [] #用來記錄要輸出Page Frames
    pagefault = [] #用來記錄要輸出Page Fault
    pagelist = []  #用來記錄每個page的info
    replaces = 0
    time = 0
    for i in range(len(page_string)):
        time +=1
        if i == 0: #第一次進來
            string = str(page_string[i])
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("F")
        elif checkin(page_string[i],pageframe[i-1]): #已經在裡面了
            for p in range(len(pagelist)): # 先找到他
                if page_string[i] == pagelist[p].id:
                    target = pagelist[p]
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("")    
        elif not checkin(page_string[i],pageframe[i-1]) and len(pageframe[i-1]) < frame_size: #不在frame裡，frame也還沒滿
            string = str(page_string[i]) + pageframe[i-1]
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("F")
        elif not checkin(page_string[i],pageframe[i-1]) and  len(pageframe[i-1]) == frame_size: #不在frame裡，frame滿了，要置換
            target = min(pagelist, key=lambda i: i.time)
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,0))
            pageframe.append(string)
            pagefault.append("F")
            replaces += 1  
    
    for i in range(len(pageframe)):
        print(str(page_string[i])+"\t"+str(pageframe[i])+"\t"+str(pagefault[i]))

    print("Page Fault = " + str(pagefaultlength(pagefault))+"\tPage Replaces = "+str(replaces)+"\tPage Frames = "+str(frame_size))
    return pageframe,pagefault,replaces
def LFU_FIFO(frame_size, page_string):
    pageframe = [] #用來記錄要輸出Page Frames
    pagefault = [] #用來記錄要輸出Page Fault
    pagelist = []  #用來記錄每個page的info
    replaces = 0
    time = 0
    for i in range(len(page_string)):
        time +=1
        if i == 0: #第一次進來
            string = str(page_string[i])
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
        elif checkin(page_string[i],pageframe[i-1]): #已經在裡面了
            for p in range(len(pagelist)): # 先找到他
                if page_string[i] == pagelist[p].id:
                    pagelist[p].counter = pagelist[p].counter + 1 # 又被reference
            string = str(pageframe[i-1])
            pageframe.append(string)
            pagefault.append("")    
        elif not checkin(page_string[i],pageframe[i-1]) and len(pageframe[i-1]) < frame_size: #不在frame裡，frame也還沒滿
            string = str(page_string[i]) + pageframe[i-1]
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
        elif not checkin(page_string[i],pageframe[i-1]) and  len(pageframe[i-1]) == frame_size: #不在frame裡，frame滿了，要置換
            target = min(pagelist, key=lambda i: (i.counter, i.time)) # 先看count再看time
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
            replaces += 1  
    
    for i in range(len(pageframe)):
        print(str(page_string[i])+"\t"+str(pageframe[i])+"\t"+str(pagefault[i]))

    print("Page Fault = " + str(pagefaultlength(pagefault))+"\tPage Replaces = "+str(replaces)+"\tPage Frames = "+str(frame_size))
    return pageframe,pagefault,replaces
def MFU_FIFO(frame_size, page_string):
    pageframe = [] #用來記錄要輸出Page Frames
    pagefault = [] #用來記錄要輸出Page Fault
    pagelist = []  #用來記錄每個page的info
    replaces = 0
    time = 0
    for i in range(len(page_string)):
        time +=1
        if i == 0: #第一次進來
            string = str(page_string[i])
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
        elif checkin(page_string[i],pageframe[i-1]): #已經在裡面了
            for p in range(len(pagelist)): # 先找到他
                if page_string[i] == pagelist[p].id:
                    pagelist[p].counter = pagelist[p].counter + 1 # 又被reference
            string = str(pageframe[i-1])
            pageframe.append(string)
            pagefault.append("")    
        elif not checkin(page_string[i],pageframe[i-1]) and len(pageframe[i-1]) < frame_size: #不在frame裡，frame也還沒滿
            string = str(page_string[i]) + pageframe[i-1]
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
        elif not checkin(page_string[i],pageframe[i-1]) and  len(pageframe[i-1]) == frame_size: #不在frame裡，frame滿了，要置換
            target = max(pagelist, key=lambda i: (i.counter, -i.time)) # 先看count再看time
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
            replaces += 1  
    
    for i in range(len(pageframe)):
        print(str(page_string[i])+"\t"+str(pageframe[i])+"\t"+str(pagefault[i]))

    print("Page Fault = " + str(pagefaultlength(pagefault))+"\tPage Replaces = "+str(replaces)+"\tPage Frames = "+str(frame_size))
    return pageframe,pagefault,replaces
def LFU_LRU(frame_size, page_string):
    pageframe = [] #用來記錄要輸出Page Frames
    pagefault = [] #用來記錄要輸出Page Fault
    pagelist = []  #用來記錄每個page的info
    replaces = 0
    time = 0
    for i in range(len(page_string)):
        time +=1
        if i == 0: #第一次進來
            string = str(page_string[i])
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
        elif checkin(page_string[i],pageframe[i-1]): #已經在裡面了
            for p in range(len(pagelist)): # 先找到他
                if page_string[i] == pagelist[p].id:
                    target = pagelist[p]
            count = target.counter + 1 # 又被reference
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,count))# time要重設!!
            pageframe.append(string)
            pagefault.append("")  
        elif not checkin(page_string[i],pageframe[i-1]) and len(pageframe[i-1]) < frame_size: #不在frame裡，frame也還沒滿
            string = str(page_string[i]) + pageframe[i-1]
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
        elif not checkin(page_string[i],pageframe[i-1]) and  len(pageframe[i-1]) == frame_size: #不在frame裡，frame滿了，要置換
            target = min(pagelist, key=lambda i: (i.counter, i.time)) # 先看count再看time
            pagelist.remove(target)
            s = target.id
            s = str(pageframe[i-1]).replace(s, "")
            string = str(page_string[i]) + s
            id = str(page_string[i])
            pagelist.append(Page(id,time,1))
            pageframe.append(string)
            pagefault.append("F")
            replaces += 1  
    
    for i in range(len(pageframe)):
        print(str(page_string[i])+"\t"+str(pageframe[i])+"\t"+str(pagefault[i]))

    print("Page Fault = " + str(pagefaultlength(pagefault))+"\tPage Replaces = "+str(replaces)+"\tPage Frames = "+str(frame_size))
    return pageframe,pagefault,replaces
def read_input(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        method = int(lines[0].split()[0])
        frame_size = int(lines[0].split()[1])
        pages = list(lines[1].strip())
    return method, frame_size, pages

input_filename = input("Please enter File Name:")
method, frame_size, page_string = read_input(input_filename)
if method == 1:
    fifo_pageframe,fifo_pagefalut, fifo_replaces = FIFO(frame_size, page_string)
elif method == 2:
    lru_pageframe,lru_pagefalut, lru_replaces = LRU(frame_size, page_string)
elif method == 3:
    lfu_pageframe,lfu_pagefalut, lfu_replaces = LFU_FIFO(frame_size, page_string)
elif method == 4:
    mfu_pageframe,mfu_pagefalut, mfu_replaces = MFU_FIFO(frame_size, page_string)
elif method == 5:
    l2u_pageframe,l2u_pagefalut, l2u_replaces = LFU_LRU(frame_size, page_string)
elif method == 6:
    fifo_pageframe,fifo_pagefalut, fifo_replaces = FIFO(frame_size, page_string)
    lru_pageframe,lru_pagefalut, lru_replaces = LRU(frame_size, page_string)
    lfu_pageframe,lfu_pagefalut, lfu_replaces = LFU_FIFO(frame_size, page_string)
    mfu_pageframe,mfu_pagefalut, mfu_replaces = MFU_FIFO(frame_size, page_string)
    l2u_pageframe,l2u_pagefalut, l2u_replaces = LFU_LRU(frame_size, page_string)
else:
    raise ValueError("Invalid method")
output_filename = f"out_{input_filename}"
with open(output_filename, 'w') as file:
    if method == 1:
        file.write("--------------FIFO-----------------------\n")
        for i in range(len(fifo_pageframe)):
            if fifo_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(fifo_pageframe[i])+"\t"+str(fifo_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(fifo_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(fifo_pagefalut))+"  Page Replaces = "+str(fifo_replaces)+"  Page Frames = "+str(frame_size)+"\n")
    elif method == 2:
        file.write("--------------LRU-----------------------\n")
        for i in range(len(lru_pageframe)):
            if lru_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(lru_pageframe[i])+"\t"+str(lru_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(lru_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(lru_pagefalut))+"  Page Replaces = "+str(lru_replaces)+"  Page Frames = "+str(frame_size)+"\n")
    elif method == 3:
        file.write("--------------Least Frequently Used Page Replacement-----------------------\n")
        for i in range(len(lfu_pageframe)):
            if lfu_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(lfu_pageframe[i])+"\t"+str(lfu_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(lfu_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(lfu_pagefalut))+"  Page Replaces = "+str(lfu_replaces)+"  Page Frames = "+str(frame_size)+"\n")
    elif method == 4:
        file.write("--------------Most Frequently Used Page Replacement -----------------------\n")
        for i in range(len(mfu_pageframe)):
            if mfu_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(mfu_pageframe[i])+"\t"+str(mfu_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(mfu_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(mfu_pagefalut))+"  Page Replaces = "+str(mfu_replaces)+"  Page Frames = "+str(frame_size)+"\n")
    elif method == 5:
        file.write("--------------Least Frequently Used LRU Page Replacement-----------------------\n")
        for i in range(len(l2u_pageframe)):
            if l2u_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(l2u_pageframe[i])+"\t"+str(l2u_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(l2u_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(l2u_pagefalut))+"  Page Replaces = "+str(l2u_replaces)+"  Page Frames = "+str(frame_size)+"\n")
    elif method == 6:
        file.write("--------------FIFO-----------------------\n")
        for i in range(len(fifo_pageframe)):
            if fifo_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(fifo_pageframe[i])+"\t"+str(fifo_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(fifo_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(fifo_pagefalut))+"  Page Replaces = "+str(fifo_replaces)+"  Page Frames = "+str(frame_size)+"\n")
        file.write("\n")
        file.write("--------------LRU-----------------------\n")
        for i in range(len(lru_pageframe)):
            if lru_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(lru_pageframe[i])+"\t"+str(lru_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(lru_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(lru_pagefalut))+"  Page Replaces = "+str(lru_replaces)+"  Page Frames = "+str(frame_size)+"\n")
        file.write("\n")
        file.write("--------------Least Frequently Used Page Replacement-----------------------\n")
        for i in range(len(lfu_pageframe)):
            if lfu_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(lfu_pageframe[i])+"\t"+str(lfu_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(lfu_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(lfu_pagefalut))+"  Page Replaces = "+str(lfu_replaces)+"  Page Frames = "+str(frame_size)+"\n")
        file.write("\n")
        file.write("--------------Most Frequently Used Page Replacement -----------------------\n")
        for i in range(len(mfu_pageframe)):
            if mfu_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(mfu_pageframe[i])+"\t"+str(mfu_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(mfu_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(mfu_pagefalut))+"  Page Replaces = "+str(mfu_replaces)+"  Page Frames = "+str(frame_size)+"\n")
        file.write("\n")
        file.write("--------------Least Frequently Used LRU Page Replacement-----------------------\n")
        for i in range(len(l2u_pageframe)):
            if l2u_pagefalut[i] != "":
                file.write(str(page_string[i])+"\t"+str(l2u_pageframe[i])+"\t"+str(l2u_pagefalut[i])+"\n")
            else:
                file.write(str(page_string[i])+"\t"+str(l2u_pageframe[i])+"\n")
        file.write("Page Fault = " + str(pagefaultlength(l2u_pagefalut))+"  Page Replaces = "+str(l2u_replaces)+"  Page Frames = "+str(frame_size)+"\n")
