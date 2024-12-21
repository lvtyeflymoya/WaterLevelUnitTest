import cv2
import os

def save_frames_as_png(video_path, output_folder):
    # 获取视频文件名和同名文件夹
    video_name = os.path.splitext(os.path.basename(video_path))[0]
    
    # 创建文件夹
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    # 打开视频文件
    cap = cv2.VideoCapture(video_path)
    
    if not cap.isOpened():
        print("无法打开视频文件:", video_path)
        return
    
    frame_count = 0
    while True:
        ret, frame = cap.read()  # 读取一帧
        if not ret:
            break  # 如果没有帧了，结束
        
        # 保存帧为 PNG 文件
        frame_filename = os.path.join(output_folder, f"frame_{frame_count:04d}.png")
        cv2.imwrite(frame_filename, frame)
        
        frame_count += 1
        print(f"保存第 {frame_count} 帧: {frame_filename}")
    
    cap.release()  # 释放视频文件
    print("视频帧保存完毕！")

if __name__ == '__main__':
    video_file = 'F:/MasterGraduate/02-ReferenceCode/02-gpu-based-image-stitching/datasets/air-4cam-mp4/01.mp4'
    save_folder = './01/'
    save_frames_as_png(video_file, save_folder)
