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
warnings.filterwarnings("ignore")
from utils.utils import (
    time_synchronized, select_device, increment_path,
    scale_coords, xyxy2xywh, non_max_suppression, split_for_trace_model,
    driving_area_mask, lane_line_mask, plot_one_box, show_seg_result,
    AverageMeter, LoadImages
)

modela = None
modelb = None
modelc = None


def set_cz(pipeline_instance, b):
    # 直接修改實例的屬性
    #print('5.3.5.3.5.3')
    pipeline_instance.cz = b
    #print('5.5.5.5.5.5')
    return pipeline_instance.cz



def initialize_model():
    global modela, modelb, modelc
    if modela is None and modelb is None and modelc is None:
        device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
        #三角錐
        modela = YOLO("C:/Users/USER/Desktop/project/button/button/best_cones.pt")
        #工程車
        modelb = YOLO("C:/Users/USER/Desktop/project/button/button/best_mix.pt")
        #紐澤西圍欄
        modelc = YOLO("C:/Users/USER/Desktop/project/button/button/best_barrie.pt")
        modela.to(device)
        modelb.to(device)
        modelc.to(device)
        
    return modela, modelb, modelc

def b(opt, queue,v8_result, v8_result_detail,counter, pipeline_instance):
    '''
    device = torch.device("cuda:0")# if torch.cuda.is_available() else "cpu")
    #三角錐
    model1 = YOLO("C:/Users/USER/Desktop/project/letsgo/call function/best_cones.pt")
    #工程車
    model2 = YOLO("C:/Users/USER/Desktop/project/letsgo/call function/best_mix.pt")
    #紐澤西圍欄
    model3 = YOLO("C:/Users/USER/Desktop/project/letsgo/call function/best_barrie.pt")
    model1.to(device)
    model2.to(device)
    model3.to(device)
    '''
    
    model1, model2, model3 = initialize_model()

    while True:
        detected = False
        start = time.time()
        print("yolov8_______________________________________________________________________start")
        frame = queue.get()
        if frame is None:
            break

        img = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
        results1 = model1.predict(source=img)
        results2 = model2.predict(source=img)
        results3 = model3.predict(source=img)
        bboxes1 = results1[0].boxes.xyxy
        bboxes2 = results2[0].boxes.xyxy
        bboxes3 = results3[0].boxes.xyxy
        v8_result.append((bboxes1, bboxes3))#因為需要算底部點的只有圍欄跟三角錐
        v8_result_detail.append((results1[0],results2[0]))#工程車需要CONF值>0.5才納入，所以把詳細結果都丟回去
        # 將 PIL 圖像轉換回 NumPy 格式
        image = np.array(img)
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        bottom_points = []
        print("11111")
        for bbox in bboxes1:
            print("22222")
            detected = True
            x1, y1, x2, y2 = map(int, bbox[:4])
            bottom_points.append([x1, y2])
            bottom_points.append([x2, y2])   
    
            # 繪製框
            cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)
        for bbox in bboxes2:
            print("33333")
            detected = True
            x1, y1, x2, y2 = map(int, bbox[:4])
            bottom_points.append([x1, y2])
            bottom_points.append([x2, y2])   
    
            # 繪製框
            cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)

        if bottom_points:
            print("44444")
            #pipeline_instance.cz = True
            bottom_centers_np = np.array(bottom_points, dtype=np.int32).reshape(-1, 1, 2)
    
            # 計算凸包
            hull = cv2.convexHull(bottom_centers_np)
            # 創建一個透明圖層
            overlay = image.copy()
            # 填充凸包
            cv2.fillPoly(overlay, [hull], color=(0, 0, 255))  # 填充多邊形，顏色紅色 (0, 0, 255)

            # 調整透明度
            alpha = 0.5  # 透明度 (0.0 完全透明, 1.0 完全不透明)
            cv2.addWeighted(overlay, alpha, image, 1 - alpha, 0, image)

            # 繪製多邊形邊界
            cv2.polylines(image, [hull], isClosed=True, color=(0, 0, 255), thickness=2)  # 顏色紅色 (0, 0, 255)
            # 保存圖
            detected = True
            print("55555")
       
        if detected == True:
            cz = True
            print("在v8內設cz為true")
           
            
        set_cz(pipeline_instance, detected)
        #if detected == True:
        #    queue_c.put(frame)
        #    queue_c.put(None)

        print("66666")
        output_path = 'output_bottom_convex_hull2.jpg'
        output_path = "output_v8"+str(counter)+".jpg"
        cv2.imwrite(output_path, image)
            # 顯示圖
        #cv2.imshow('Detected Image with Contours', image)
        over = time.time()
        print("yolov8_______________________________________________________________________over"+str(start-over))