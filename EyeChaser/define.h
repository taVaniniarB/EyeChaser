#pragma once
// 창 텍스트
#define FOCUSED_TEXT L"포커스 타이틀"
#define UNFOCUSED_TEXT L"포커스 해제 타이틀"

// 종료창 …… ⋯⋯
#define EXIT_WINDOW true
#define CLOSE_DIALOG_TITLE L"종료창 타이틀"
#define CLOSE_DIALOG L"정말 종료하시겠습니까?"

// 눈 이동영역 너비, 높이
// 40 / 35
#define LEYE_WIDTH 38.f
#define LEYE_HEIGHT 33.f

// 값을 올릴 수록 미포커스 시 눈동자 아래로 내려감
#define EYE_UNFOCUS_Y_OFFSET 0.f

#define LEYE_CENTERPOS_Y 147.f // 눈 높이
#define EYEOFFSET 38.f // 미간
#define IRIS_RADIUS 12 // 눈 반지름

// 시스템 아이콘 사용 시, rc 파일을 주석 처리
enum class EIconType
{
	Windows,
	Error,
	Warning,
	Image,
};
#define ICON_TYPE EIconType::Image

// 눈동자 텍스처 변화
#define CHANGE_IRIS false

// 캔버스 사이즈
#define CANVAS_SIZE 300
#define WINDOW_SIZE 200

#define HALF_CANVAS (CANVAS_SIZE/2)

// 왼쪽 눈 중앙좌표
#define LEYE_CENTERPOS_X (HALF_CANVAS - EYEOFFSET)

// 오른쪽 눈 중앙좌표
#define REYE_CENTERPOS_X (HALF_CANVAS + EYEOFFSET)
#define REYE_CENTERPOS_Y LEYE_CENTERPOS_Y

#define SCALE 1
#define TEXTURE 2
//#define IRIS_ADJUST_SCALE 0.7f
#define IRIS_CHANGE_MODE TEXTURE
#define IRIS_TEX_NUM ((CHANGE_IRIS && (IRIS_CHANGE_MODE == TEXTURE)) ? 2 : 1)

// Singleton 매크로
#define SINGLE(type) public:\
						 static type* GetInst() \
						 {\
							 static type mgr;\
							 return &mgr;\
						 }\
					 private:\
						type();\
						~type();


#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInst()->GetMouseCursor()

#ifndef SW_NOZIZE
#define SW_NOZIZE 0x0008
#endif
