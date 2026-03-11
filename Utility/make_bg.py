from PIL import Image
import os
import sys

# 고정 경로
FIXED_BG_PATH = r"C:\Users\hayeon\Desktop\gaze\배경\background.png"

def main():
    # 1. 사용자로부터 폴더 이름 입력 받기
    folder_name = input("bg 파일들이 있는 폴더 이름을 입력하세요 (예: test1, batchA 등): ").strip()
    if not folder_name:
        print("폴더 이름을 입력해야 합니다.")
        sys.exit(1)

    target_dir = r"C:\Users\hayeon\Desktop\gaze"
    source_folder = os.path.join(target_dir, folder_name)

    # 2. 폴더 존재 여부 확인
    if not os.path.isdir(source_folder):
        print(f"폴더를 찾을 수 없습니다: {source_folder}")
        sys.exit(1)

    # 3. background.png 읽기
    if not os.path.isfile(FIXED_BG_PATH):
        print(f"background 파일이 없습니다: {FIXED_BG_PATH}")
        sys.exit(1)

    try:
        base_img = Image.open(FIXED_BG_PATH).convert("RGBA")
    except Exception as e:
        print(f"background 이미지를 열 수 없습니다: {e}")
        sys.exit(1)

    if base_img.size != (1250, 250):
        print(f"경고: background 크기가 1250×250이 아닙니다 (현재: {base_img.size})")
        # 필요하면 여기서 강제 리사이즈 가능
        # base_img = base_img.resize((1250, 250), Image.Resampling.LANCZOS)

    # 4. bg1.png ~ bgN.png 파일들 수집 (png만)
    bg_files = []
    for file in sorted(os.listdir(source_folder)):
        if file.lower().endswith('.png') and file.lower().startswith('bg'):
            full_path = os.path.join(source_folder, file)
            bg_files.append(full_path)

    if not bg_files:
        print("해당 폴더에 bg로 시작하는 .png 파일이 없습니다.")
        sys.exit(1)

    print(f"찾은 bg 파일 수: {len(bg_files)}개")

    # 5. 결과 이미지 준비 (배경 + bg 개수만큼 오른쪽으로 확장)
    total_width = base_img.width + (len(bg_files) * 250)
    result_height = 250

    result = Image.new("RGBA", (total_width, result_height), (0, 0, 0, 0))  # 투명 배경

    # 6. base 이미지 붙이기 (왼쪽)
    result.paste(base_img, (0, 0))

    # 7. 각 bg 이미지 → 250×250으로 축소 후 순서대로 붙이기
    x_offset = base_img.width

    for bg_path in bg_files:
        try:
            img = Image.open(bg_path).convert("RGBA")
            
            # 300×300 → 250×250 (비율 유지 + 중앙 크롭 또는 단순 리사이즈)
            # 방법1: 단순 리사이즈 (비율 깨짐 허용)
            resized = img.resize((250, 250), Image.Resampling.LANCZOS)
            
            # 방법2: 비율 유지 + 중앙 크롭 원하면 아래 주석 해제
            # img.thumbnail((250, 250), Image.Resampling.LANCZOS)
            # resized = Image.new("RGBA", (250, 250), (0,0,0,0))
            # offset = ((250 - img.size[0]) // 2, (250 - img.size[1]) // 2)
            # resized.paste(img, offset)

            result.paste(resized, (x_offset, 0))
            x_offset += 250

        except Exception as e:
            print(f"이미지 처리 실패: {bg_path} → {e}")
            continue

    # 8. 결과 저장 (원본 폴더 안에)
    output_path = os.path.join(source_folder, "background.png")
    
    try:
        result.save(output_path, "PNG")
        print(f"완료! 저장 위치:\n{output_path}")
        print(f"최종 크기: {result.size}")
    except Exception as e:
        print(f"저장 실패: {e}")

if __name__ == "__main__":
    main()
