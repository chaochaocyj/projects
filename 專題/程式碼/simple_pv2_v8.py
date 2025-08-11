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
    parser.add_argument("--config", default= "C://Users//user//rein//rein_dinov2_mask2former_512x512_bs1x4//rein_dinov2_mask2former_512x512_bs1x4.py",help="Path to the training configuration file.")
    parser.add_argument("--checkpoint", default="C://Users//user//rein//tools//iter_40000.pth",help="Path to the checkpoint file for both the REIN and head models.")
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
    yolov8_result = res_b.pop(0)
    yolov8_result_detail = res_bd.pop(0)
    yolopv2_pil = Image.fromarray(cv2.cvtColor(yolopv2_img, cv2.COLOR_BGR2RGB))
    draw = ImageDraw.Draw(yolopv2_pil)
    font = ImageFont.load_default()
    bottom_points = []
    bboxes1, bboxes2 = yolov8_result
    result1,result2 = yolov8_result_detail
    boxes = result2.boxes  # 这应该是包含检测框和置信度的对象
    names = result2.names  # 这是包含类别名的字典
    for bbox in bboxes1:
        x1, y1, x2, y2 = map(int, bbox[:4])
        bottom_points.append([x1, y2])
        bottom_points.append([x2, y2])
        draw.rectangle([(x1, y1), (x2, y2)], outline="green", width=2)
    for bbox in bboxes2:
        x1, y1, x2, y2 = map(int, bbox[:4])
        bottom_points.append([x1, y2])
        bottom_points.append([x2, y2])
        draw.rectangle([(x1, y1), (x2, y2)], outline="green", width=2)
    if boxes is not None:
        for i in range(len(boxes)):
            class_id = int(boxes.cls[i].item())  # 類別ID
            conf = boxes.conf[i].item()  #拿出conf
            bbox = boxes.xyxy[i]  #拿出邊界框
                    
            # 在全部的類別中找出他
            classname = names[class_id]
            # 如果置信度小於0.4，跳過
            if conf < 0.5:
                continue                    
            # 繪製邊界圖
            x1, y1, x2, y2 = map(int, bbox)
            draw.rectangle(((x1, y1), (x2, y2)), outline="red", width=2) 
                    
            # 顯示標籤
            label = f"{classname}: {conf:.2f}"
                    
            # 計算標籤位置
            label_bbox = draw.textbbox((x1, y1), label, font=font)
            label_width = label_bbox[2] - label_bbox[0]
            label_height = label_bbox[3] - label_bbox[1]
            label_y = y1 - label_height - 3 if y1 - label_height - 3 > 0 else y1 + 3
                    
            # 繪製背景
            draw.rectangle(((x1, label_y), (x1 + label_width, label_y + label_height)), fill="red")
                    
            # 繪製標籤
            draw.text((x1, label_y), label, fill="black", font=font)
    combined_img = cv2.cvtColor(np.array(yolopv2_pil), cv2.COLOR_RGB2BGR)

    if bottom_points:
        # 將底部點轉換為 NumPy 數組
        bottom_centers_np = np.array(bottom_points, dtype=np.int32)
        if len(bottom_centers_np) < 5:
            # 如果座標點少於五個，只標示點
            for (x, y) in bottom_centers_np:
                cv2.circle(combined_img, (x, y), 3, (255, 0, 0), -1)
        else:
            # 如果座標點大於等於五個
            # 將所有點顯示出來
            for (x, y) in bottom_centers_np:
                cv2.circle(combined_img, (x, y), 3, (255, 0, 0), -1)

            remaining_points = recursive_removal(bottom_centers_np)
            if len(remaining_points) != 0:
                # 畫出剩餘點的凸包
                hull_remaining_points = cv2.convexHull(np.array(remaining_points))

                # 創建一個透明圖層
                overlay = combined_img.copy()

                # 填充凸包
                cv2.fillPoly(overlay, [hull_remaining_points], color=(0, 0, 255))  # 填充多邊形，顏色紅色 (0, 0, 255)

                # 調整透明度
                alpha = 0.5  # 透明度 (0.0 完全透明, 1.0 完全不透明)
                cv2.addWeighted(overlay, alpha, combined_img, 1 - alpha, 0, combined_img)
                # 繪製多邊形邊界
                cv2.polylines(combined_img, [hull_remaining_points], isClosed=True, color=(0, 0, 255), thickness=2)

    output_path = "output_combine_"+str(counter)+".jpg"
    cv2.imwrite(output_path, combined_img)
    #cv2.imshow('Result_pv2', combined_img)
    return combined_img

def display_results(counter):
    if results_a and results_b:
        final_frame = combine_results(results_a, results_b, results_b_detail, results_c,counter) 
        print("有進來嗎????")
        cv2.imshow('Result', final_frame)
        #if cv2.waitKey(1) & 0xFF == ord('q'):
        return
# 主程式
def do2(input_source,check_running):
    #載入原始設定
    start = time.time()
    opt = make_parser().parse_args()
    cap = None
    counter = 0
    cap = cv2.VideoCapture(input_source)
    if cap:
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break
            if not check_running():
                print("功能二停止")
                cv2.destroyWindow('Result')
                return
            counter += 1
            print("這是我的第"+str(counter)+"張")
            # 把frame放到佇列中，讓對應的模型進行處理
            frame = cv2.resize(frame,(1280,720))
            queue_a.put(frame)
            queue_a.put(None)
            a(opt,queue_a,results_a,counter)
            queue_b.put(frame)
            queue_b.put(None)
            b(opt,queue_b, results_b, results_b_detail,counter)
            display_results(counter)

        cap.release()
        cv2.destroyAllWindows()

