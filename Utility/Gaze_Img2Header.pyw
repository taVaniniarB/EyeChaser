import os
import tkinter as tk
from tkinter import messagebox
from PIL import Image

def get_image_data_as_c_array(image_path, prefix):
    try:
        with open(image_path, 'rb') as image_file:
            binary_data = image_file.read()
        
        with Image.open(image_path) as img:
            width = img.width
            
        data_array_lines = []
        for i, byte in enumerate(binary_data):
            if i % 12 == 0:
                data_array_lines.append("\n ")
            data_array_lines.append(f"0x{byte:02x}, ")
        
        array_content = "".join(data_array_lines).rstrip(", ")
        array_definition = f"unsigned char {prefix}imageData[] = {{{array_content}\n}};\n"
        size_definition = f"unsigned int {prefix}imageDataSize = {len(binary_data)};\n"
        width_definition = f"unsigned int {prefix}imageWidth = {width};\n"
        
        return array_definition, size_definition, width_definition
    except Exception:
        return None, None, None

def write_data_to_file(file_path, data_list):
    if not data_list:
        return False
    try:
        with open(file_path, 'w') as out_file:
            out_file.write("#pragma once\n\n")
            for data in data_list:
                out_file.write(data + "\n")
        return True
    except:
        return False

def start_conversion(event=None):  # event 매개변수 추가
    # 입력창에서 디렉토리 이름 가져오기
    directory_name = entry.get().strip()
    
    if not directory_name:
        messagebox.showwarning("입력 오류", "디렉토리 이름을 입력해주세요.")
        return
    
    # 기본 설정 (사용자 경로에 맞게 수정 필요)
    base_path = f"/Users/hayeon/Desktop/gaze/{directory_name}"
    prefixes = ["background", "sclera", "L", "R", "char"]
    bg_file_name = "background_data.txt"
    unified_file_name = "unified_image_data.txt"
    
    if not os.path.exists(base_path):
        messagebox.showerror("오류", f"경로를 찾을 수 없습니다:\n{base_path}")
        return
    
    background_data = []
    unified_data = []
    success_count = 0
    
    for prefix in prefixes:
        image_path = os.path.join(base_path, f"{prefix}.png")
        array_def, size_def, width_def = get_image_data_as_c_array(image_path, prefix)
        
        if array_def:
            success_count += 1
            if prefix == "background":
                background_data.extend([array_def, size_def, width_def])
            else:
                unified_data.extend([array_def, size_def, width_def])
    
    # 파일 쓰기
    bg_ok = write_data_to_file(os.path.join(base_path, bg_file_name), background_data)
    un_ok = write_data_to_file(os.path.join(base_path, unified_file_name), unified_data)
    
    if success_count > 0:
        messagebox.showinfo("완료", f"{success_count}개의 이미지 변환 성공!\n파일이 {base_path}에 저장되었습니다.")
    else:
        messagebox.showwarning("실패", "변환할 이미지 파일을 찾지 못했습니다.")

# --- GUI 설정 ---
root = tk.Tk()
root.title("이미지 C 배열 변환기")
root.geometry("400x200")

# 중앙 정렬을 위한 프레임
frame = tk.Frame(root, padx=20, pady=20)
frame.pack(expand=True)

label = tk.Label(frame, text="디렉토리 이름을 입력하세요:", font=("Arial", 10))
label.pack(pady=5)

entry = tk.Entry(frame, width=30)
entry.pack(pady=5)
entry.insert(0, "name") # 기본값 설정
entry.bind('<Return>', start_conversion)  # 엔터 키 이벤트 바인딩

convert_button = tk.Button(frame, text="변환 시작", command=start_conversion, 
                           bg="#4CAF50", fg="white", width=20, height=2)
convert_button.pack(pady=20)

root.mainloop()