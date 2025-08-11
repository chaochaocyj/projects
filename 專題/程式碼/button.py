import tkinter as tk
import threading
import time
from simple_pv2 import do
from simple_pv2_v8 import do2
from simple_f3 import do3
import cv2
# 控制變數
running = False
current_thread = None  # 保存當前執行緒

def check_running():
    global running
    return running    
def execute_function1(func, input_path):
    global running
    running = True
    do(input_path,check_running)
def execute_function2(func, input_path):
    global running
    running = True
    do2(input_path,check_running)
def execute_function3(func, input_path):
    global running
    running = True
    do3(input_path,check_running)

def function_one(input_value):
    execute_function1(function_one, input_value)

def function_two(input_value):
    execute_function2(function_two, input_value)

def function_three(input_value):
    execute_function3(function_three, input_value)

def stop_function():
    global running
    running = False
    print("功能已中斷")

def run_function_in_thread(func, input_value):
    global running, current_thread
    stop_function()  # 停止當前執行緒內的功能

    if current_thread and current_thread.is_alive():
        current_thread.join()  # 等待當前執行緒結束

    # 啟動新的執行緒
    current_thread = threading.Thread(target=func, args=(input_value,))
    current_thread.start()

# 建立主視窗
window = tk.Tk()
window.title("功能暫停與切換範例")
window.geometry("300x300")

# 創建輸入框
input_label = tk.Label(window, text="請輸入參數：")
input_label.pack(pady=5)

input_entry = tk.Entry(window)
input_entry.pack(pady=5)

# 創建按鈕
button1 = tk.Button(window, text="功能一:道路分割", command=lambda: run_function_in_thread(function_one, input_entry.get()))
button1.pack(pady=10)

button2 = tk.Button(window, text="功能二:道路分割+物件辨識", command=lambda: run_function_in_thread(function_two, input_entry.get()))
button2.pack(pady=10)

button3 = tk.Button(window, text="功能三:道路分割+物件辨識+施工區域分割", command=lambda: run_function_in_thread(function_three, input_entry.get()))
button3.pack(pady=10)

stop_button = tk.Button(window, text="中斷功能", command=stop_function)
stop_button.pack(pady=20)

# 啟動主事件迴圈
window.mainloop()
