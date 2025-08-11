import threading
import argparse
import time
from pathlib import Path
import torch
from ultralytics import YOLO
import os
os.environ["KMP_DUPLICATE_LIB_OK"] = 'True'
import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from queue import Queue, Empty
import warnings
from scipy.spatial.distance import euclidean
warnings.filterwarnings("ignore")
from utils.utils import (
    time_synchronized, select_device, increment_path,
    scale_coords, xyxy2xywh, non_max_suppression, split_for_trace_model,
    driving_area_mask, lane_line_mask, plot_one_box, show_seg_result,
    AverageMeter, LoadImages
)
from nyolopv2 import a 
from nyolov8 import b

import os
import queue
# a 是 yolopv2
# b 是 yolopv8
# c 是 rein

# 創建三個佇列，每個模型對應一個佇列
queue_a = queue.Queue()
queue_b = queue.Queue()
queue_c = queue.Queue()

# 創建三個模型的結果存放區
results_a = []
results_b = []
results_b_detail = []
results_c = []
# 停止訊號
stop_signal = False
def make_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--weights', nargs='+', type=str, default="C:/Users/USER/Desktop/project/button/button/yolopv2.pt", help='model.pt path(s)')
    parser.add_argument('--source', type=str, default= 'carvideo.mp4', help='source')  # file/folder, 0 for webcam
    parser.add_argument('--img-size', type=int, default=640, help='inference size (pixels)')
    parser.add_argument('--conf-thres', type=float, default=0.3, help='object confidence threshold')
    parser.add_argument('--iou-thres', type=float, default=0.45, help='IOU threshold for NMS')
    parser.add_argument('--device', default='0', help='cuda device, i.e. 0 or 0,1,2,3 or cpu')
    parser.add_argument('--save-conf', action='store_true', help='save confidences in --save-txt labels')
    parser.add_argument('--save-txt', action='store_true', help='save results to *.txt')
    parser.add_argument('--nosave', action='store_true', help='do not save images/videos')
    parser.add_argument('--classes', nargs='+', type=int, help='filter by class: --class 0, or --class 0 2 3')
    parser.add_argument('--agnostic-nms', action='store_true', help='class-agnostic NMS')
    parser.add_argument('--project', default='runs/detect', help='save results to project/name')
    parser.add_argument('--name', default='exp', help='save results to project/name')
    parser.add_argument('--exist-ok', action='store_true', help='existing project/name ok, do not increment')
    parser.add_argument('--save-path', type=str, default='new_combined_result_confv10.mp4', help='path to save the combined result')
    #以下是TRY
    parser.add_argument("--config", default= "C:\\Users\\user\\rein\\rein_dinov2_mask2former_512x512_bs1x4\\rein_dinov2_mask2former_512x512_bs1x4.py",help="Path to the training configuration file.")
    parser.add_argument("--checkpoint", default="C:\\Users\\user\\rein\\tools\\iter_40000.pth",help="Path to the checkpoint file for both the REIN and head models.")
    parser.add_argument("--images", default="pic20.jpg", help="Directory or file path of images to be processed.")

    return parser

# 計算每對點之間的距離
def calculate_distances(points):
    num_points = len(points)
    distances = {}
    for i in range(num_points):
        for j in range(i + 1, num_points):
            dist = euclidean(points[i], points[j])
            distances[(i, j)] = dist
    return distances

# 找出最小距離
def find_min_distance(distances):
    if not distances:
        return float('inf')  # 如果沒有距離，返回無窮大
    return min(distances.values())

# 找出大於五倍最小距離的點對
def find_points_to_remove(distances, min_distance):
    #threshold = 20 * min_distance
    threshold2 = 640
    pairs_to_check = [pair for pair, dist in distances.items() if dist > threshold2]
    #pairs_to_check = [pair for pair, dist in distances.items() if dist > threshold or dist > threshold2]
    print(pairs_to_check)
    if not pairs_to_check:
        return set()  # 如果沒有符合條件的點對，返回空集合

    point_frequency = {}

    for (i1, j1) in pairs_to_check:
        if i1 in point_frequency:
            point_frequency[i1] += 1
        else:
            point_frequency[i1] = 1
        if j1 in point_frequency:
            point_frequency[j1] += 1
        else:
            point_frequency[j1] = 1
    print(point_frequency)
    # 根據頻率篩選需要排除的點
    max_frequency = max(point_frequency.values())
    points_to_remove = {point for point, freq in point_frequency.items() if freq == max_frequency}

    return points_to_remove

# 遞迴排除點
def recursive_removal(points):
    iteration = 0
    while True:
        iteration += 1
        print(f"Iteration {iteration}: Current points: {points}")

        distances = calculate_distances(points)
        min_distance = find_min_distance(distances)
        print(f"Iteration {iteration}: Distances: {distances}")
        print(f"Iteration {iteration}: Minimum distance: {min_distance}")

        if min_distance == float('inf'):  # 如果最小距離是無窮大，則沒有可排除的點
            break

        points_to_remove = find_points_to_remove(distances, min_distance)
        print(f"Iteration {iteration}: Points to remove: {points_to_remove}")

        if not points_to_remove:
            break

        points = np.array([point for i, point in enumerate(points) if i not in points_to_remove])
    return points        
        
def combine_results(res_a, res_b, res_bd, res_c,counter):
    yolopv2_img = res_a.pop(0)
    yolopv2_pil = Image.fromarray(cv2.cvtColor(yolopv2_img, cv2.COLOR_BGR2RGB))
    draw = ImageDraw.Draw(yolopv2_pil)
    font = ImageFont.load_default()
    combined_img = cv2.cvtColor(np.array(yolopv2_pil), cv2.COLOR_RGB2BGR)
     # 將 PIL Image 轉換為 numpy 數組
    return combined_img

def display_results(counter):
    if results_a:
        final_frame = combine_results(results_a, results_b, results_b_detail, results_c,counter) 
        print("有進來嗎????")
        cv2.imshow('Result', final_frame)
        #if cv2.waitKey(1) & 0xFF == ord('q'):
        return
# 主程式
def do(input_source,check_running):
    #載入原始設定
    global running
    running = True
    start = time.time()
    opt = make_parser().parse_args()
    cap = None
    counter = 0
    #thread_c = threading.Thread(target=c, args=(opt,queue_c, results_c,counter))
    #thread_c.start()
    cap = cv2.VideoCapture(input_source)
    if cap:
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break
            if not check_running():
                print("功能一停止")
                cv2.destroyWindow('Result')
                return
            counter += 1
            print("這是我的第"+str(counter)+"張")
            # 把frame放到佇列中，讓對應的模型進行處理
            frame = cv2.resize(frame,(1280,720))
            queue_a.put(frame)
            queue_a.put(None)
            a(opt,queue_a,results_a,counter)
            display_results(counter)

            # 顯示結果
            #display_results()
        cap.release()
        cv2.destroyAllWindows()

