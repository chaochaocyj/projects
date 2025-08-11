class Page:
    def __init__(self, page_id, time=0, counter=0):
        self.id = page_id
        self.time = time
        self.counter = counter

    def __repr__(self):
        return f"Page(id={self.id}, time={self.time}, counter={self.counter})"
class PageReplacementAlgorithm:
    def __init__(self, frame_size):
        self.frame_size = frame_size
        self.page_frames = []
        self.page_faults = 0
        self.page_replaces = 0

    def replace(self, pages):
        raise NotImplementedError

    def get_results(self):
        return self.page_faults, self.page_replaces, [page.id for page in self.page_frames]

class FIFO(PageReplacementAlgorithm):
    def replace(self, pages):
        page_faults = []
        time = 0
        for page_id in pages:
            time += 1
            if not any(page.id == page_id for page in self.page_frames):
                self.page_faults += 1
                if len(self.page_frames) >= self.frame_size:
                    self.page_frames.pop(0)
                    self.page_replaces += 1
                self.page_frames.append(Page(page_id, time))
                page_faults.append("F")
            else:
                page_faults.append(" ")
        return page_faults

class LRU(PageReplacementAlgorithm):
    def replace(self, pages):
        page_faults = []
        time = 0
        for page_id in pages:
            time += 1
            existing_page = next((page for page in self.page_frames if page.id == page_id), None)
            if not existing_page:
                self.page_faults += 1
                if len(self.page_frames) >= self.frame_size:
                    lru_page = min(self.page_frames, key=lambda page: page.time)
                    self.page_frames.remove(lru_page)
                    self.page_replaces += 1
                self.page_frames.append(Page(page_id, time))
                page_faults.append("F")
            else:
                existing_page.time = time
                page_faults.append(" ")
        return page_faults

class LFU_FIFO(PageReplacementAlgorithm):
    def replace(self, pages):
        page_faults = []
        time = 0
        for page_id in pages:
            time += 1
            existing_page = next((page for page in self.page_frames if page.id == page_id), None)
            if not existing_page:
                self.page_faults += 1
                if len(self.page_frames) >= self.frame_size:
                    lfu_page = min(self.page_frames, key=lambda page: (page.counter, page.time))
                    self.page_frames.remove(lfu_page)
                    self.page_replaces += 1
                self.page_frames.append(Page(page_id, time, 1))
                page_faults.append("F")
            else:
                existing_page.counter += 1
                page_faults.append(" ")
        return page_faults

class MFU_FIFO(PageReplacementAlgorithm):
    def replace(self, pages):
        page_faults = []
        time = 0
        for page_id in pages:
            time += 1
            existing_page = next((page for page in self.page_frames if page.id == page_id), None)
            if not existing_page:
                self.page_faults += 1
                if len(self.page_frames) >= self.frame_size:
                    mfu_page = max(self.page_frames, key=lambda page: (page.counter, page.time))
                    self.page_frames.remove(mfu_page)
                    self.page_replaces += 1
                self.page_frames.append(Page(page_id, time, 1))
                page_faults.append("F")
            else:
                existing_page.counter += 1
                page_faults.append(" ")
        return page_faults

class LFU_LRU(PageReplacementAlgorithm):
    def replace(self, pages):
        page_faults = []
        time = 0
        for page_id in pages:
            time += 1
            existing_page = next((page for page in self.page_frames if page.id == page_id), None)
            if not existing_page:
                self.page_faults += 1
                if len(self.page_frames) >= self.frame_size:
                    lfu_pages = [page for page in self.page_frames if page.counter == min(page.counter for page in self.page_frames)]
                    lru_lfu_page = min(lfu_pages, key=lambda page: page.time)
                    self.page_frames.remove(lru_lfu_page)
                    self.page_replaces += 1
                self.page_frames.append(Page(page_id, time, 1))
                page_faults.append("F")
            else:
                existing_page.counter += 1
                page_faults.append(" ")
        return page_faults
def read_input(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        method = int(lines[0].split()[0])
        frame_size = int(lines[0].split()[1])
        pages = list(lines[1].strip())
    return method, frame_size, pages

def execute_algorithm(method, frame_size, pages):
    if method == 1:
        algo = FIFO(frame_size)
    elif method == 2:
        algo = LRU(frame_size)
    elif method == 3:
        algo = LFU_FIFO(frame_size)
    elif method == 4:
        algo = MFU_FIFO(frame_size)
    elif method == 5:
        algo = LFU_LRU(frame_size)
    elif method == 6:
        results = []
        for m in range(1, 6):
            algo = globals()[['FIFO', 'LRU', 'LFU_FIFO', 'MFU_FIFO', 'LFU_LRU'][m-1]](frame_size)
            page_faults = algo.replace(pages)
            results.append((algo.get_results(), page_faults))
        return results
    else:
        raise ValueError("Invalid method")
    
    page_faults = algo.replace(pages)
    return algo.get_results(), page_faults

def write_output(input_filename, method, frame_size, pages, results, page_faults):
    method_names = ["FIFO", "LRU", "LFU+FIFO", "MFU+FIFO", "LFU+LRU", "ALL"]
    output_filename = f"out_{input_filename}"
    with open(output_filename, 'w') as file:
        if method == 6:
            for i, (result, faults) in enumerate(results):
                page_faults_count, page_replaces, page_frames = result
                file.write(f"Method: {method_names[i]}\n")
                file.write(f"Page Reference String: {' '.join(map(str, pages))}\n")
                file.write(f"Page Frames: {' '.join(map(str, page_frames))}\n")
                file.write(f"Page Faults: {' '.join(faults)}\n")
                file.write(f"Page Fault: {page_faults_count}, Page Replaces: {page_replaces}, Page Frames: {frame_size}\n\n")
        else:
            page_faults_count, page_replaces, page_frames = results
            file.write(f"Method: {method_names[method-1]}\n")
            file.write(f"Page Reference String: {' '.join(map(str, pages))}\n")
            file.write(f"Page Frames: {' '.join(map(str, page_frames))}\n")
            file.write(f"Page Faults: {' '.join(page_faults)}\n")
            file.write(f"Page Fault: {page_faults_count}, Page Replaces: {page_replaces}, Page Frames: {frame_size}\n")

# 主函数
def main():
    input_filename = input("plz enter:")
    method, frame_size, pages = read_input(input_filename)
    results, page_faults = execute_algorithm(method, frame_size, pages)
    write_output(input_filename, method, frame_size, pages, results, page_faults)

# 运行主函数
if __name__ == "__main__":
    main()
