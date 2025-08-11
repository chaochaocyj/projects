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
    AverageMeter, LoadImages, letterbox
)

modelv2 = None
d = None #device
h = None #half

def initialize_model(opt):
    global modelv2, d, h
    if modelv2 is None and d is None and h is None:
        weights = opt.weights
        d = select_device(opt.device)
        h = d.type != 'cpu'
        modelv2 = torch.jit.load(weights).to(d)
        if h:
            modelv2.half()
        modelv2.eval()
        
    return modelv2, d, h


def a(opt,queue, yolop_result, counter,img_size=640, conf_thres=0.3, iou_thres=0.45, classes=None, agnostic_nms=False):
    '''
    weights = opt.weights
    device = select_device(opt.device)
    half = device.type != 'cpu'
    model = torch.jit.load(weights).to(device)
    if half:
        model.half()
    model.eval()
    '''
    '''
    source = queue.get()
    img = Image.fromarray(cv2.cvtColor(source, cv2.COLOR_BGR2RGB))
    img.save("temp_pv2.jpg")  # 保存臨時圖片
    source = "temp_pv2.jpg"
    dataset = LoadImages(source, img_size=img_size)
    '''
    
    model, device, half = initialize_model(opt)
    
    while True:
        # 從佇列中取得 frame
        start = time.time()
        print("yolopv2________________________________________________________________________start")
        im0 = queue.get()
        if im0 is None:
            break  # 結束條件，當收到 None 時結束推論
        # 檢查 im0 是否有效（不是空值）
        if im0 is None or im0.size == 0:  
            print("Invalid frame detected, skipping...")  # 如果 frame 無效，跳過該 frame
            continue  # 跳過無效的 frame
        # 調整圖像大小，準備進行推論
        img = letterbox(im0, img_size, stride=32, auto=True)[0]  # YOLOP 要求的尺寸
        
        # 檢查 letterbox 返回的 img 是否有效
        if img is None or img.size == 0:
            print("Letterbox failed, skipping frame...")
            continue
        img = img.transpose(2, 0, 1)  # HWC to CHW
        img = np.ascontiguousarray(img)

        # 將圖像轉換為 tensor 並移動到裝置（如 GPU）
        img = torch.from_numpy(img).to(device)
        img = img.half() if half else img.float()  # 如果使用半精度，則進行轉換
        img /= 255.0  # 歸一化

        if img.ndimension() == 3:
            img = img.unsqueeze(0)

        # 進行推論
        with torch.no_grad():
            [pred, anchor_grid], seg, ll = model(img)

        pred = split_for_trace_model(pred, anchor_grid)
        pred = non_max_suppression(pred, conf_thres, iou_thres, classes=classes, agnostic=agnostic_nms)

        # 處理輸出，生成駕駛區域和車道線掩碼
        da_seg_mask = driving_area_mask(seg)
        ll_seg_mask = lane_line_mask(ll)

        # 繪製結果
        for i, det in enumerate(pred):
            if len(det):
                det[:, :4] = scale_coords(img.shape[2:], det[:, :4], im0.shape).round()
            im0=cv2.resize(im0,(da_seg_mask.shape[1],da_seg_mask.shape[0]) ,interpolation=cv2.INTER_AREA)

            show_seg_result(im0, (da_seg_mask, ll_seg_mask), is_demo=True)
        over = time.time()
        print("yolopv2________________________________________________________________________over"+str(start-over))
        # 將結果存入 yolop_result
        yolop_result.append(im0)
        output_path = "output_pv2"+str(counter)+".jpg"
        cv2.imwrite(output_path, im0)
        #cv2.imshow('Result_pv2', im0)
        # 檢查退出條件
        if cv2.waitKey(1) == ord('q'):
            break