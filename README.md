## Eye Chaser

캐릭터의 시선이 마우스를 따라가는 프로그램

<img src="https://github.com/user-attachments/assets/92edb03b-c9b6-4e0b-8d67-70a2502e2c50" width="600">


### 기능

**마우스트래킹**

* 타원 방정식과 Lerp 사용하여 눈동자의 위치를 결정

**클라이언트 창 포커스 여부에 따른 텍스처 변화**

**우클릭 메뉴**

<img src="https://imgur.com/ZmDbAm7.png" width="400">

* Always On Top 속성 토글 (창을 항상 최상단에 표시)
* 마우스트래킹 토글

**방향키로 배경 변경**
<img src="https://github.com/user-attachments/assets/e23f09c9-2c8c-4596-863b-03da9c14d3a5" width="400">


### 특징

**포터블 exe**
- 사용자가 텍스처에 쉽게 접근할 수 없게 하기 위해 이미지를 바이너리 헤더로 변환 후 하드코딩하고, 런타임에 이를 png로 로드 > 배포 시 별도의 폴더가 필요하지 않으며 exe 단일 배포 가능

### 유틸리티
- Gaze_Img2Header.pyw
    - PNG를 C 스타일 unsigned char 배열로 변환하는 Tkinter 기반 GUI 도구 (배경/통합 데이터 파일 작성 및 기본 경로/접두사 설정 기능 포함)
- make_bg.py
    - 고정 배경과 폴더 내 bg*.png 파일들을 합성하여 하나의 background.png 생성 (250×250 크기 조정 및 소스 폴더 저장)
- PNG2ICO.exe
    - PNG 파일을 ICO 형식으로 변환