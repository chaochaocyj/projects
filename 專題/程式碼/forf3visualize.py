# Copyright (c) OpenMMLab. All rights reserved.
# Copyright (c) OpenMMLab. All rights reserved.
import argparse
import os
import os.path as osp
import cv2
os.chdir(osp.abspath(osp.dirname(osp.dirname(__file__))))
import sys

sys.path.append(os.curdir)
import time
from mmengine.config import Config
from mmseg.utils import get_classes, get_palette
from mmengine.runner.checkpoint import _load_checkpoint
from rein.utils import init_model
from mmseg.apis import inference_model
import rein
import tqdm
import mmengine
import torch
import numpy as np
from PIL import Image

def parse_args():
    parser = argparse.ArgumentParser(description="MMSeg test (and eval) a model")
    parser.add_argument("--config", default="C:/Users/USER/Desktop/Rein/Rein-train/configs/dinov2/rein_dinov2_mask2former_512x512_bs1x4.py",help="Path to the training configuration file.")
    parser.add_argument("--checkpoint", default="C:/Users/USER/Desktop/Rein/Rein-train/work_dirs/rein_dinov2_mask2former_512x512_bs1x440000/iter_40000.pth",help="Path to the checkpoint file for both the REIN and head models.")
    parser.add_argument("--images", default="pic20.jpg", help="Directory or file path of images to be processed.")
    parser.add_argument("--suffix", default=".jpg", help="File suffix to filter images in the directory. Default is '.png'.")
    parser.add_argument("--not-recursive", action='store_false', help="Whether to search images recursively in subfolders. Default is recursive.")
    parser.add_argument("--search-key", default="", help="Keyword to filter images within the directory. Default is no filtering.")
    parser.add_argument(
        "--backbone",
        #default="checkpoints/dinov2_vitl14_converted_1024x1024.pth",
        default="C:/Users/USER/Desktop/Rein/Rein-train/checkpoints/dinov2_converted.pth",
        help="Path to the backbone model checkpoint. Default is 'work_dirs/rein_dinov2_mask2former_512x512_bs1x4/iter_40000.pth'."
    )
    parser.add_argument("--save_dir", default="C:\\Users\\user\\rein\\tools\\show", help="Directory to save the output images. Default is 'work_dirs/show'.")
    parser.add_argument("--tta", action="store_true", help="Enable test time augmentation. Default is disabled.")
    parser.add_argument("--device", default="cuda:0", help="Device to use for computation. Default is 'cuda:0'.")
    args = parser.parse_args()
    return args

def load_backbone(checkpoint: dict, backbone_path: str) -> None:
    converted_backbone_weight = _load_checkpoint(backbone_path, map_location="cpu")
    if "state_dict" in checkpoint:
        checkpoint["state_dict"].update(
            {f"backbone.{k}": v for k, v in converted_backbone_weight.items()}
        )
    else:
        checkpoint.update(
            {f"backbone.{k}": v for k, v in converted_backbone_weight.items()}
        )


classes = get_classes("cityscapes")
palette = get_palette("cityscapes")

'''
def draw_sem_seg(sem_seg: torch.Tensor):
    num_classes = len(classes)
    sem_seg = sem_seg.data.squeeze(0)
    H, W = sem_seg.shape
    ids = torch.unique(sem_seg).cpu().numpy()
    legal_indices = ids < num_classes
    ids = ids[legal_indices]
    labels = np.array(ids, dtype=np.int64)
    colors = [palette[label] for label in labels]
    colors = [torch.tensor(color, dtype=torch.uint8).view(1, 1, 3) for color in colors]
    result = torch.zeros([H, W, 3], dtype=torch.uint8)
    for label, color in zip(labels, colors):
        result[sem_seg == label, :] = color
    with open("prediction3.txt", "w") as f:
       f.write(str(sem_seg.data.squeeze(0)))
    return result.cpu().numpy()


def draw_sem_seg(sem_seg: torch.Tensor):
    num_classes = len(classes)
    sem_seg = sem_seg.data.squeeze(0)
    H, W = sem_seg.shape
    ids = torch.unique(sem_seg).cpu().numpy()
    legal_indices = ids < num_classes
    ids = ids[legal_indices]
    labels = np.array(ids, dtype=np.int64)
    
    # 创建一个RGBA图像，初始化为全透明
    #result = np.zeros((H, W, 4), dtype=np.uint8)
    result = torch.zeros([H, W, 3], dtype=torch.uint8)
    
    # 红色的RGBA值 (R=255, G=0, B=0, A=255)
    red_color = np.array([0, 0, 255, 255], dtype=np.uint8)
    
    # 为每个非背景标签填充红色
    for label in labels:
        if label != 0:  # 假设0是背景标签
            mask = (sem_seg == label).cpu().numpy()
            result[mask] = red_color
    
    return result.cpu().numpy()
'''

_global_model = None
_global_cfg = None

#初始化
def initialize_model(args):
    global _global_model, _global_cfg
    
    # 只有在模型尚未載入時才進行載入
    if _global_model is None:
        # load config
        cfg = Config.fromfile(args.config)
        
        if "test_pipeline" not in cfg:
            cfg.test_pipeline = [
                dict(type="LoadImageFromFile"),
                dict(
                    keep_ratio=True,
                    scale=(1920, 1080),
                    type="Resize",
                ),
                dict(type="PackSegInputs"),
            ]
        
        model = init_model(cfg, args.checkpoint, device=args.device)
        model = model.cuda(args.device)
        
        state_dict = model.state_dict()
        load_backbone(state_dict, args.backbone)
        model.load_state_dict(state_dict)
        
        # 儲存模型和配置到全域變量
        _global_model = model
        _global_cfg = cfg
    
    return _global_model, _global_cfg


def draw_sem_seg(pred_seg):
    #如果 pred_seg 有 'data' 屬性，則取得該屬性
    if hasattr(pred_seg, 'data'):
        sem_seg = pred_seg.data
    else:
        sem_seg = pred_seg
    
    #轉numpy 陣列
    if isinstance(sem_seg, torch.Tensor):
        sem_seg = sem_seg.cpu().numpy()
    
    #如果 sem_seg 是三維的，則去掉第一個維度
    if sem_seg.ndim == 3:
        sem_seg = sem_seg.squeeze(0)
    
    H, W = sem_seg.shape
    
    # 創建一個具有透明度的白色遮罩 (4 個通道)
    result = np.zeros((H, W, 4), dtype=np.uint8)
    #遮罩的條件是 sem_seg 大於 0 的部分
    mask = sem_seg > 0
    #在遮罩的地方設置為紅色 (BGR)，50% 不透明度
    result[mask] = [0, 0, 255, 128]  # White with 50% opacity
    
    return result

def apply_mask_to_image(original_image, mask):    
    #將 PIL 圖像轉換為 NumPy 陣列
    open_cv_image = np.array(original_image)
    #RGB轉BGR
    open_cv_image = open_cv_image[:, :, ::-1].copy()
    
    
    #如果圖像是灰階的，則轉換為 BGR 格式
    if len(open_cv_image.shape) == 2:
        original_bgr = cv2.cvtColor(open_cv_image, cv2.COLOR_GRAY2BGR)
    else:
        original_bgr = open_cv_image

    #resize
    if original_bgr.shape[:2] != mask.shape[:2]:
        mask = cv2.resize(mask, (original_bgr.shape[1], original_bgr.shape[0]), interpolation=cv2.INTER_NEAREST)
    
    #創建一個遮罩，當遮罩的 alpha 通道大於 0 時有效
    alpha_mask = mask[:, :, 3] > 0
    
    #創建一個與原圖像相同大小的覆蓋層
    overlay = np.zeros_like(original_bgr)
    #在遮罩有效的地方設置覆蓋層為紅色
    overlay[alpha_mask] = [0, 0, 255]  

    #混合原始圖像與覆蓋層
    alpha = 0.5  # 50% opacity
    result = cv2.addWeighted(original_bgr, 1, overlay, alpha, 0)
    
    return result


def c(opt, queue,rein_result,counter): #opt, rein_result, stop_event
    args = parse_args()
    #args = argparse.Namespace()
    # load config
    '''
    原本的
    cfg = Config.fromfile(args.config)
    if "test_pipeline" not in cfg:
        cfg.test_pipeline = [
            dict(type="LoadImageFromFile"),
            dict(
                keep_ratio=True,
                scale=(
                    1920,
                    1080,
                ),
                type="Resize",
            ),
            dict(type="PackSegInputs"),
        ]
    model = init_model(cfg, args.checkpoint, device=args.device)
    model=model.cuda(args.device)
    state_dict = model.state_dict()
    load_backbone(state_dict, args.backbone)
    model.load_state_dict(state_dict)
    mmengine.mkdir_or_exist(args.save_dir)
    '''
    
    model, cfg = initialize_model(args)
    
    mmengine.mkdir_or_exist(args.save_dir)
    
    while True:
        start = time.time()
        print("rein________________________________________________________________________start")
        frame = queue.get()
        if frame is None:
            break
        img = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
        if isinstance(img, Image.Image):  # 如果 img 是 PIL Image
            img.save("temp_image.jpg")  # 保存臨時圖片
            img_path = "temp_image.jpg"  # 保存路徑
            result = inference_model(model, img_path) #因為他的模型要用路徑跑
        else:
            result = inference_model(model, img)

        #result = inference_model(model, img)
        pred = draw_sem_seg(result.pred_sem_seg)
        #pred = apply_mask_to_image(img, pred)
        #cv2.imwrite("../images/imgSave.png", pred)
        
        
        print( result.pred_sem_seg )
        #with open("prediction.txt", "w") as f:
        #    f.write(str(result.pred_sem_seg))

        #img = Image.open(img).convert("RGB")
        #img = img.convert("RGB")
        
        #pred = Image.fromarray(pred).resize(
        #    [img.width, img.height], resample=Image.NEAREST
        #)
        # 將 pred 轉換為 RGBA 模式
        #pred = pred.convert("RGBA")

        # 設置透明度
        #alpha = int(0.5 * 255)
        #alpha_channel = Image.new("L", pred.size, alpha)  # 建立一個全透明的 alpha 通道
        #pred.putalpha(alpha_channel)

        # 將 pred 和 img 轉換為相同模式並融合
        #img_with_alpha = img.convert("RGBA")
        #vis = Image.alpha_composite(img_with_alpha, pred)

        # 儲存融合後的圖像，轉換為 RGB 模式以避免存儲為帶有 alpha 通道的圖像
        #vis = vis.convert("RGB")
        # 將 PIL 圖像轉換為 NumPy 數組
        #cv_image = np.array(vis)

        # 如果需要，將 RGB 轉換為 BGR（OpenCV 使用 BGR 順序）
        #cv_image = cv2.cvtColor(cv_image, cv2.COLOR_RGB2BGR)
        pred = apply_mask_to_image(img, pred)
        
        cv_image = np.array(pred)

        # 如果需要，將 RGB 轉換為 BGR（OpenCV 使用 BGR 順序）
        #cv_image = cv2.cvtColor(cv_image, cv2.COLOR_RGB2BGR)
        over = time.time()
        print("rein________________________________________________________________________over"+str(start-over))
        print(str(type(pred))+"my checkpoint~~~~~~")
        output_path = "output_rein"+str(counter)+".jpg"
        cv2.imwrite(output_path, cv_image)
        #cv2.imshow('Result_rein', cv_image)
        # 檢查退出條件
        if cv2.waitKey(1) == ord('q'):
            break
        rein_result.append(pred)

    


if __name__ == "__main__":
    c()
