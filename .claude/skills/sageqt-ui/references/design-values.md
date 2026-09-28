# 디자인 값 목록

`sageqt-ui`의 상세 규격이다. `ui/style/SageDesignDefine.h`에 둘 값의 **전체 목록**이다. 파일에는 **쓰는 주제에서** 해당 값을 옮긴다 (T08은 팔레트 · 폰트 값부터 — 쓰이지 않는 상수를 미리 넣지 않는다). 스타일 · 위젯 · delegate를 그릴 때 값은 여기서 찾는다.

- 출처: SageSDI `SageSDI/SageDefine.h` — 값 · 줄 번호는 원문 그대로 (2026-09-27, 커밋 `2a6c179`). 분류 근거는 `docs/decisions/sageqt-ui/constants-classification.md`
- 이름은 SageSDI 이름을 그대로 쓴다 — 원본과 대조하기 쉽게. 단위는 **96 DPI 기준 논리 픽셀**이다 (SKILL.md *레이아웃 정책*)
- "쓰는 곳"은 SageSDI에서 그 이름을 쓰는 파일 수다. 이 목록에 없는 SageSDI 디자인 상수는 옮기지 않는다 (호출 0곳 · Win32 전용 — 분류표 참고)
- "쓰는 방식" 열: **고정**(그 값 그대로) · **최소값**(글자 폭 — 레이아웃 · `QFontMetrics`가 더 넓힐 수 있다) · 표 열 최소 폭(`QHeaderView`) · 최대값 · 옮기지 않음(값이 효과 없음). 기준은 SKILL.md *레이아웃 정책*

## 색 (41개)

| 이름 | 값 (원문) | hex | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_COLOR_APP_BACKGROUND` | `RGB(248, 246, 241)` | `#F8F6F1` | 248 | 12 files |  |
| `SAGE_COLOR_PANEL` | `RGB(255, 255, 255)` | `#FFFFFF` | 249 | 23 files | SAGE_COLOR_BUTTON_TEXT · SAGE_COLOR_SIDEBAR_SELECTED_TEXT와 같은 값 RGB(255, 255, 255) |
| `SAGE_COLOR_SIDEBAR` | `RGB(36, 31, 26)` | `#241F1A` | 250 | 4 files |  |
| `SAGE_COLOR_SIDEBAR_TEXT` | `RGB(205, 196, 185)` | `#CDC4B9` | 251 | 3 — SageSidebarTree.cpp, SageUiResources.cpp, SageSidebarPanel.cpp |  |
| `SAGE_COLOR_SIDEBAR_SELECTED` | `RGB(58, 49, 41)` | `#3A3129` | 252 | 1 — SageSidebarTree.cpp |  |
| `SAGE_COLOR_SIDEBAR_CATEGORY` | `RGB(130, 120, 108)` | `#82786C` | 253 | 2 — SageSidebarTree.cpp, SageUiResources.cpp |  |
| `SAGE_COLOR_SIDEBAR_DIVIDER` | `RGB(51, 44, 37)` | `#332C25` | 254 | 2 — SageSidebarPanel.cpp, SageSDIView.cpp |  |
| `SAGE_COLOR_BORDER` | `RGB(220, 214, 205)` | `#DCD6CD` | 255 | 15 files |  |
| `SAGE_COLOR_TEXT` | `RGB(47, 42, 36)` | `#2F2A24` | 256 | 23 files |  |
| `SAGE_COLOR_SECONDARY_TEXT` | `RGB(122, 112, 100)` | `#7A7064` | 257 | 11 files |  |
| `SAGE_COLOR_TEXT_MUTED` | `RGB(110, 101, 91)` | `#6E655B` | 258 | 7 files |  |
| `SAGE_COLOR_PRIMARY` | `RGB(154, 107, 63)` | `#9A6B3F` | 259 | 17 files |  |
| `SAGE_COLOR_PRIMARY_PRESS` | `RGB(118, 80, 42)` | `#76502A` | 260 | 2 — SageButton.cpp, SageListBox.cpp |  |
| `SAGE_COLOR_BUTTON_TEXT` | `RGB(255, 255, 255)` | `#FFFFFF` | 261 | 1 — SageButton.cpp | SAGE_COLOR_PANEL · SAGE_COLOR_SIDEBAR_SELECTED_TEXT와 같은 값 RGB(255, 255, 255) |
| `SAGE_COLOR_BUTTON_BORDER` | `RGB(201, 191, 177)` | `#C9BFB1` | 262 | 4 files |  |
| `SAGE_COLOR_FOCUS_RING_PRIMARY` | `RGB(240, 228, 213)` | `#F0E4D5` | 263 | 1 — SageButton.cpp |  |
| `SAGE_COLOR_FOCUS_RING_NEUTRAL` | `RGB(239, 235, 227)` | `#EFEBE3` | 264 | 1 — SageButton.cpp |  |
| `SAGE_COLOR_DANGER_BORDER` | `RGB(224, 189, 182)` | `#E0BDB6` | 266 | 2 — SageButton.cpp, SageStatusCard.cpp |  |
| `SAGE_COLOR_SUCCESS` | `RGB(95, 127, 95)` | `#5F7F5F` | 267 | 2 — SageStatusCard.cpp, SageUiResources.cpp |  |
| `SAGE_COLOR_WARNING` | `RGB(184, 135, 70)` | `#B88746` | 268 | 4 files |  |
| `SAGE_COLOR_ERROR` | `RGB(184, 92, 74)` | `#B85C4A` | 269 | 6 files |  |
| `SAGE_COLOR_LIST_ROW_ALT` | `RGB(250, 248, 244)` | `#FAF8F4` | 270 | 2 — SageListBox.cpp, SageListCtrl.cpp |  |
| `SAGE_COLOR_LIST_HEADER` | `RGB(242, 238, 231)` | `#F2EEE7` | 271 | 11 files |  |
| `SAGE_COLOR_LIST_GRID` | `RGB(237, 232, 224)` | `#EDE8E0` | 272 | 4 files |  |
| `SAGE_COLOR_LIST_ROW_SELECTED` | `RGB(241, 227, 205)` | `#F1E3CD` | 273 | 2 — SageListBox.cpp, SageListCtrl.cpp |  |
| `SAGE_COLOR_TEXT_PLACEHOLDER` | `RGB(180, 171, 160)` | `#B4ABA0` | 274 | 5 files |  |
| `SAGE_COLOR_INLINE_ERROR_TEXT` | `RGB(156, 68, 51)` | `#9C4433` | 275 | 3 — SageInlineError.cpp, SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp |  |
| `SAGE_COLOR_INLINE_WARN_TEXT` | `RGB(138, 106, 50)` | `#8A6A32` | 276 | 1 — SageInlineError.cpp |  |
| `SAGE_COLOR_INLINE_WARN_BG` | `RGB(251, 245, 238)` | `#FBF5EE` | 277 | 2 — SageInlineError.cpp, SageResultTablePanel.cpp | SAGE_COLOR_STATUS_BG_WARNING과 같은 값 RGB(251, 245, 238) |
| `SAGE_COLOR_INLINE_WARN_BORDER` | `RGB(235, 220, 198)` | `#EBDCC6` | 278 | 2 — SageInlineError.cpp, SageResultTablePanel.cpp |  |
| `SAGE_COLOR_ACCENT_SURFACE` | `RGB(247, 242, 234)` | `#F7F2EA` | 279 | 2 — SageFilterPillBar.cpp, SageUiResources.cpp |  |
| `SAGE_COLOR_STATUS_BG_SUCCESS` | `RGB(234, 244, 234)` | `#EAF4EA` | 280 | 1 — SageUiResources.cpp |  |
| `SAGE_COLOR_STATUS_BG_WARNING` | `RGB(251, 245, 238)` | `#FBF5EE` | 281 | 1 — SageUiResources.cpp | SAGE_COLOR_INLINE_WARN_BG와 같은 값 RGB(251, 245, 238) |
| `SAGE_COLOR_STATUS_BG_ERROR` | `RGB(248, 235, 233)` | `#F8EBE9` | 282 | 2 — SageUiResources.cpp, SageWorkflowHistoryPanel.cpp |  |
| `SAGE_COLOR_STATUS_CARD_BG_SUCCESS` | `RGB(241, 245, 240)` | `#F1F5F0` | 283 | 1 — SageStatusCard.cpp |  |
| `SAGE_COLOR_STATUS_CARD_BORDER_SUCCESS` | `RGB(213, 224, 211)` | `#D5E0D3` | 284 | 1 — SageStatusCard.cpp |  |
| `SAGE_COLOR_STATUS_CARD_TEXT_SUCCESS` | `RGB(65, 96, 63)` | `#41603F` | 285 | 2 — SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp |  |
| `SAGE_COLOR_STATUS_CARD_BG_ERROR` | `RGB(253, 246, 244)` | `#FDF6F4` | 286 | 2 — SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp |  |
| `SAGE_COLOR_BADGE_BG_SUCCESS` | `RGB(238, 244, 238)` | `#EEF4EE` | 287 | 1 — SageWorkflowHistoryPanel.cpp |  |
| `SAGE_COLOR_SIDEBAR_SELECTED_TEXT` | `RGB(255, 255, 255)` | `#FFFFFF` | 288 | 1 — SageSidebarTree.cpp | SAGE_COLOR_PANEL · SAGE_COLOR_BUTTON_TEXT와 같은 값 RGB(255, 255, 255) |
| `SAGE_COLOR_LIST_HEADER_BORDER` | `RGB(228, 223, 215)` | `#E4DFD7` | 289 | 3 — SageBadge.cpp, SageSearchBox.cpp, SageHeaderPanel.cpp |  |

## 폰트 (10개)

크기는 SageSDI `CreatePointFont` 인자(0.1pt 단위)다. SageQt는 **픽셀 크기**로 지정한다: px = 포인트 × 96 / 72, 반올림 (SKILL.md *폰트*). 서체 이름은 쓰지 않고 패밀리 + 굵기로 찾는다 (T02).

| 이름 | 값 (원문) | SageQt | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_TITLE_FONT_POINT_SIZE` | `143` | 14.3pt → **19px** | 150 | 1 — SageUiResources.cpp | 0.1pt 단위(14.3pt). font-probe ROLES title·logo에 같은 값 (측정 도구 전용) |
| `SAGE_CONTROL_FONT_POINT_SIZE` | `105` | 10.5pt → **14px** | 151 | 1 — SageUiResources.cpp | 0.1pt 단위(10.5pt). SAGE_CONTENT_FONT_POINT_SIZE와 같은 값. font-probe ROLES에 같은 값 (측정 도구 전용) |
| `SAGE_CONTENT_FONT_POINT_SIZE` | `105` | 10.5pt → **14px** | 152 | 3 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp, SageUiResources.cpp | 0.1pt 단위(10.5pt). SAGE_CONTROL_FONT_POINT_SIZE와 같은 값. font-probe ROLES에 같은 값 (측정 도구 전용) |
| `SAGE_HEADER_FONT_POINT_SIZE` | `113` | 11.3pt → **15px** | 153 | 1 — SageUiResources.cpp | 0.1pt 단위(11.3pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| `SAGE_LIST_FONT_POINT_SIZE` | `98` | 9.8pt → **13px** | 154 | 1 — SageUiResources.cpp | 0.1pt 단위(9.8pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| `SAGE_CAPTION_FONT_POINT_SIZE` | `90` | 9.0pt → **12px** | 155 | 1 — SageUiResources.cpp | 0.1pt 단위(9.0pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| `SAGE_SUMMARY_FONT_POINT_SIZE` | `128` | 12.8pt → **17px** | 156 | 1 — SageUiResources.cpp | 0.1pt 단위(12.8pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| `SAGE_TITLE_FONT_FACE` | `L"Pretendard SemiBold"` | 패밀리 + 굵기 (SKILL.md *폰트*) | 245 | 1 — SageUiResources.cpp | GDI식 이름("패밀리 + 굵기"). Qt는 패밀리 "Pretendard" + QFont::DemiBold (T02). font-probe LEGACY_FACES에 같은 값 (측정 도구 전용) |
| `SAGE_CONTROL_FONT_FACE` | `L"Pretendard"` | 패밀리 + 굵기 (SKILL.md *폰트*) | 246 | 4 files | GDI 패밀리 이름. font-probe SAGE_FONT_PROBE_FAMILY_PRETENDARD("Pretendard") · LEGACY_FACES에 같은 값 (측정 도구 전용) |
| `SAGE_LOGO_FONT_FACE` | `L"Gmarket Sans TTF Bold"` | 패밀리 + 굵기 (SKILL.md *폰트*) | 247 | 1 — SageUiResources.cpp | GDI식 이름. Qt는 "Gmarket Sans TTF" + QFont::Bold (T02). font-probe LEGACY_FACES에 같은 값 (측정 도구 전용) |

## 공통 — 여백 · 컨트롤 · 아이콘 (32개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_MARGIN` | `16` | 고정 | 47 | 4 files |  |
| `SAGE_BUTTON_WIDTH` | `120` | **최소값** (글자 폭) | 48 | 1 — SageWorkflowInputPanel.cpp |  |
| `SAGE_BUTTON_HEIGHT` | `32` | 고정 | 49 | 8 files |  |
| `SAGE_EDIT_HEIGHT` | `32` | 고정 | 50 | 7 files | SAGE_BUTTON_HEIGHT와 같은 값 32 |
| `SAGE_ROW_GAP` | `10` | 고정 | 51 | 4 files |  |
| `SAGE_BORDER_THICKNESS` | `1` | 고정 | 77 | 18 files |  |
| `SAGE_ICON_SIZE` | `15` | 고정 | 99 | 1 — SageButton.cpp |  |
| `SAGE_ICON_ADD_SIZE` | `14` | 고정 | 100 | 1 — SageButton.cpp |  |
| `SAGE_ICON_STROKE` | `2` | 고정 | 101 | 3 — SageButton.cpp, SageMessageBody.cpp, SageUiStyle.cpp |  |
| `SAGE_ICON_ADD_SPAN` | `10` | 고정 | 102 | 1 — SageButton.cpp |  |
| `SAGE_ICON_TEXT_GAP` | `6` | 고정 | 103 | 3 — SageButton.cpp, SageOptionCheck.cpp, SageSelectionBar.cpp |  |
| `SAGE_ICON_SEARCH_RADIUS` | `5` | 고정 | 104 | 1 — SageUiStyle.cpp |  |
| `SAGE_ICON_SEARCH_HANDLE` | `4` | 고정 | 105 | 1 — SageUiStyle.cpp |  |
| `SAGE_ICON_RESET_RADIUS` | `6` | 고정 | 106 | 1 — SageButton.cpp |  |
| `SAGE_ICON_RESET_ARROW` | `3` | 고정 | 107 | 1 — SageButton.cpp |  |
| `SAGE_ICON_CLOSE_SPAN` | `10` | 고정 | 108 | 1 — SageButton.cpp |  |
| `SAGE_ICON_ARROW_HALF_WIDTH` | `4` | 고정 | 109 | 1 — SageButton.cpp |  |
| `SAGE_ICON_ARROW_HALF_HEIGHT` | `2` | 고정 | 110 | 1 — SageButton.cpp |  |
| `SAGE_COMBO_FIELD_INSET` | `6` | 고정 | 141 | 2 — SageFilterComboBox.cpp, SageSearchBox.cpp |  |
| `SAGE_EDIT_TEXT_TOP_PAD` | `9` | 고정 | 143 | 3 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp, SageWorkflowInputPanel.cpp | EM_SETRECT 서식 사각형 안쪽 여백 → Qt는 textMargins/스타일로 |
| `SAGE_EDIT_TEXT_LEFT_PAD` | `10` | 고정 | 144 | 2 — SageSearchBox.cpp, SageWorkflowInputPanel.cpp | EM_SETRECT 서식 사각형 안쪽 여백 → Qt는 textMargins/스타일로 |
| `SAGE_BUTTON_VERT_ADJUST` | `2` | 고정 — 결과 표 띠 안의 버튼 배치 오프셋 (T15, `SageResultTablePanel.cpp`) | 148 | 1 — SageResultTablePanel.cpp | GDI 배치 보정값 — Qt에서 필요 여부 측정 후 판단 |
| `SAGE_BUTTON_TEXT_TOP_OFFSET` | `0` | 옮기지 않음 — 값 0 (글자 위치 보정 없음, T09) | 149 | 1 — SageButton.cpp | GDI 텍스트 위치 보정값(0) — Qt에서 필요 여부 측정 후 판단 |
| `SAGE_ACTION_GAP` | `8` | 고정 | 160 | 2 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp |  |
| `SAGE_CONTENT_PAD_X` | `24` | 고정 | 174 | 2 — SageHeaderPanel.cpp, SageWorkspacePanel.cpp |  |
| `SAGE_CONTENT_PAD_Y` | `20` | 고정 | 175 | 1 — SageWorkspacePanel.cpp |  |
| `SAGE_FORM_LABEL_WIDTH` | `64` | **최소값** (글자 폭) | 181 | 1 — SageWorkflowInputPanel.cpp | SAGE_LOGIN_DLG_LABEL_WIDTH와 같은 값 64 |
| `SAGE_EDIT_BORDER_WIDTH` | `1` | 고정 | 206 | 3 — SageSearchBox.cpp, SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | SAGE_BORDER_THICKNESS와 같은 값 1 |
| `SAGE_OPTION_CHECK_PADDING` | `8` | 고정 | 239 | 1 — SageOptionCheck.cpp |  |
| `SAGE_INPUT_RESET_WIDTH` | `72` | **최소값** (글자 폭) | 240 | 1 — SageWorkflowInputPanel.cpp |  |
| `SAGE_FOCUS_RING_WIDTH` | `2` | 고정 | 265 | 2 — SageMessageBoxDlg.cpp, SageButton.cpp |  |
| `SAGE_CO_COMPANY_EDIT_MIN_WIDTH` | `80` | **최소값** (글자 폭) | 508 | 1 — SageWorkflowInputPanel.cpp | 이름의 CO_COMPANY는 "법인 순서 데이터 관리" 잔재(주석 줄 506). 실제 용도는 입력 폼 편집칸 최소 폭 |

## 다이얼로그 · 메시지 상자 · 로그인 (22개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_DLG_CAPTION_HEIGHT` | `40` | 고정 | 112 | 2 — SageFramelessDialog.cpp, SageDialogCaptionBar.cpp |  |
| `SAGE_DLG_CAPTION_PAD` | `16` | 고정 | 113 | 1 — SageDialogCaptionBar.cpp |  |
| `SAGE_DLG_CAPTION_BTN_SIZE` | `28` | 고정 | 114 | 1 — SageDialogCaptionBar.cpp |  |
| `SAGE_DLG_CAPTION_BTN_PAD` | `8` | 고정 | 115 | 1 — SageDialogCaptionBar.cpp |  |
| `SAGE_MSGBOX_WIDTH` | `360` | **최소값** (글자 폭) | 119 | 1 — SageMessageBoxDlg.cpp |  |
| `SAGE_MSGBOX_MAX_TEXT_HEIGHT` | `200` | 최대값 | 122 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ICON_SIZE` | `22` | 고정 | 123 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ICON_RADIUS` | `10` | 고정 | 124 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ICON_TEXT_GAP` | `12` | 고정 | 125 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ICON_DOT_SIZE` | `2` | 고정 | 126 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ALERT_STEM_TOP` | `6` | 고정 | 127 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ALERT_STEM_BOTTOM` | `13` | 고정 | 128 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_ALERT_DOT_TOP` | `15` | 고정 | 129 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_INFO_DOT_TOP` | `5` | 고정 | 130 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_INFO_STEM_TOP` | `9` | 고정 | 131 | 1 — SageMessageBody.cpp |  |
| `SAGE_MSGBOX_INFO_STEM_BOTTOM` | `16` | 고정 | 132 | 1 — SageMessageBody.cpp |  |
| `SAGE_LOGIN_BTN_WIDTH` | `68` | **최소값** (글자 폭) | 419 | 1 — SageHeaderPanel.cpp |  |
| `SAGE_LOGIN_DLG_WIDTH` | `320` | **최소값** (글자 폭) | 421 | 1 — SageLoginDlg.cpp |  |
| `SAGE_PASSWORD_DLG_WIDTH` | `360` | **최소값** (글자 폭) | 422 | 1 — SagePasswordChangeDlg.cpp |  |
| `SAGE_LOGIN_DLG_LABEL_WIDTH` | `64` | **최소값** (글자 폭) | 423 | 1 — SageLoginDlg.cpp | SAGE_FORM_LABEL_WIDTH와 같은 값 64 |
| `SAGE_PASSWORD_DLG_LABEL_WIDTH` | `96` | **최소값** (글자 폭) | 424 | 1 — SagePasswordChangeDlg.cpp |  |
| `SAGE_LOGIN_DLG_BTN_WIDTH` | `96` | **최소값** (글자 폭) | 425 | 3 — SageLoginDlg.cpp, SageMessageBoxDlg.cpp, SagePasswordChangeDlg.cpp | MessageBoxDlg도 씀 (이름은 LOGIN_DLG) |

## 사이드바 (5개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_SIDEBAR_WIDTH` | `220` | 고정 | 157 | 1 — SageSDIView.cpp |  |
| `SAGE_SIDEBAR_PAD_X` | `20` | 고정 | 167 | 2 — SageSidebarTree.cpp, SageSidebarPanel.cpp |  |
| `SAGE_SIDEBAR_CATEGORY_CHAR_EXTRA` | `1` | 고정 | 168 | 1 — SageSidebarTree.cpp | SetTextCharacterExtra 자간(px) → QFont::setLetterSpacing |
| `SAGE_SIDEBAR_TREE_TOP_PAD` | `16` | 고정 | 169 | 1 — SageSidebarPanel.cpp |  |
| `SAGE_SIDEBAR_ITEM_HEIGHT` | `34` | 고정 | 170 | 1 — SageSidebarPanel.cpp |  |

## 헤더 (8개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_BADGE_HEIGHT` | `20` | 고정 | 161 | 1 — SageBadge.cpp | SAGE_LIST_BADGE_HEIGHT와 같은 값 20 |
| `SAGE_BADGE_PAD_X` | `8` | 고정 | 162 | 2 — SageBadge.cpp, SageSummaryBar.cpp | SAGE_LIST_BADGE_PAD_X와 같은 값 8 |
| `SAGE_BADGE_RADIUS` | `4` | 고정 | 163 | 1 — SageSummaryBar.cpp | SAGE_LIST_BADGE_RADIUS와 같은 값 4. 사용처는 SummaryBar뿐 (SageBadge는 안 씀) |
| `SAGE_HEADER_GAP` | `12` | 고정 | 164 | 1 — SageHeaderPanel.cpp |  |
| `SAGE_HEADER_TITLE_GAP` | `10` | 고정 | 165 | 1 — SageHeaderPanel.cpp |  |
| `SAGE_HEADER_CATEGORY_WIDTH` | `80` | **최소값** (글자 폭) | 166 | 1 — SageHeaderPanel.cpp |  |
| `SAGE_HEADER_HEIGHT` | `56` | 고정 | 173 | 2 — SageSidebarPanel.cpp, SageSDIView.cpp |  |
| `SAGE_USER_LABEL_WIDTH` | `150` | **최소값** (글자 폭) | 420 | 1 — SageHeaderPanel.cpp |  |

## 탭 (2개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_TAB_HEIGHT` | `40` | 고정 | 158 | 2 — SageTabCtrl.cpp, SageWorkspacePanel.cpp |  |
| `SAGE_TAB_INDICATOR_HEIGHT` | `2` | 고정 | 159 | 1 — SageTabCtrl.cpp |  |

## 상태 카드 · 진행 · 카드 (29개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_PROGRESS_TEXT_WIDTH` | `54` | **최소값** (글자 폭) | 52 | 1 — SageStatusCard.cpp |  |
| `SAGE_CARD_HEADER_HEIGHT` | `38` | 고정 | 176 | 1 — SageWorkflowInputPanel.cpp |  |
| `SAGE_CARD_PADDING` | `16` | 고정 | 177 | 3 — SageSectionLabel.cpp, SageStatusCard.cpp, SageWorkflowInputPanel.cpp |  |
| `SAGE_CARD_ROW_GAP` | `12` | 고정 | 178 | 3 — SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp, SageWorkflowInputPanel.cpp |  |
| `SAGE_CARD_GAP` | `16` | 고정 | 179 | 1 — SageWorkflowInputPanel.cpp |  |
| `SAGE_CARD_ACTION_BUTTON_HEIGHT` | `34` | 고정 | 180 | 1 — SageWorkflowInputPanel.cpp |  |
| `SAGE_STATUS_CARD_HEIGHT` | `70` | 고정 | 182 | 1 — SageWorkflowInputPanel.cpp |  |
| `SAGE_STATUS_CARD_DOT_SIZE` | `8` | 고정 | 183 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_DOT_GAP` | `8` | 고정 | 184 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ICON_SIZE` | `18` | 고정 | 185 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ICON_GAP` | `12` | 고정 | 186 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ICON_RADIUS` | `7` | 고정 | 187 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ICON_THICKNESS` | `2` | 고정 | 188 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_CHECK_START_X` | `-3` | 고정 | 189 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_CHECK_START_Y` | `0` | 고정 | 190 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_CHECK_MID_X` | `-1` | 고정 | 191 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_CHECK_MID_Y` | `3` | 고정 | 192 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_CHECK_END_X` | `3` | 고정 | 193 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_CHECK_END_Y` | `-2` | 고정 | 194 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ALERT_STEM_TOP` | `-4` | 고정 | 195 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ALERT_STEM_BOTTOM` | `1` | 고정 | 196 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ALERT_DOT_TOP` | `3` | 고정 | 197 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ALERT_DOT_SIZE` | `2` | 고정 | 198 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_PROGRESS_HEIGHT` | `6` | 고정 | 199 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_TITLE_LINE_HEIGHT` | `20` | 고정 | 200 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_DETAIL_LINE_HEIGHT` | `16` | 고정 | 201 | 1 — SageStatusCard.cpp |  |
| `SAGE_STATUS_CARD_ACTION_WIDTH` | `80` | **최소값** (글자 폭) | 202 | 1 — SageStatusCard.cpp | SAGE_STATUS_CARD_ACTION_AREA_WIDTH 계산에도 쓰임 |
| `SAGE_STATUS_CARD_ACTION_GAP` | `12` | 고정 | 203 | 0 | 외부 파일 사용 0곳이지만 SageDefine.h 안에서 SAGE_STATUS_CARD_ACTION_AREA_WIDTH 계산에 쓰임 → 함께 옮김 |
| `SAGE_STATUS_CARD_ACTION_AREA_WIDTH` | `SAGE_STATUS_CARD_ACTION_WIDTH + SAGE_STATUS_CARD_ACTION_GAP` | **최소값** (글자 폭) | 204 | 1 — SageStatusCard.cpp | = 80 + 12 = 92 (두 상수로 계산) |

## 인라인 메시지 (10개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_INLINE_MSG_HEIGHT` | `24` | 고정 | 78 | 2 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp |  |
| `SAGE_INLINE_MSG_ICON_SIZE` | `14` | 고정 | 79 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_MSG_ICON_GAP` | `8` | 고정 | 80 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_MSG_BOX_PAD_X` | `10` | 고정 | 81 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_MSG_BOX_RADIUS` | `4` | 고정 | 82 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_ICON_RADIUS` | `5` | 고정 | 83 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_ICON_STEM_TOP` | `4` | 고정 | 84 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_ICON_STEM_BOTTOM` | `8` | 고정 | 85 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_ICON_DOT_TOP` | `10` | 고정 | 86 | 1 — SageInlineError.cpp |  |
| `SAGE_INLINE_ICON_DOT_SIZE` | `2` | 고정 | 87 | 1 — SageInlineError.cpp |  |

## 표 · 결과 · 요약 · 합계 · 선택 · 검색 · 필 바 (37개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_LIST_ROW_HEIGHT` | `34` | 고정 | 58 | 1 — SageListCtrl.cpp |  |
| `SAGE_LIST_HEADER_HEIGHT` | `36` | 고정 | 59 | 1 — SageHeaderCtrl.cpp |  |
| `SAGE_LIST_CHECK_BOX_SIZE` | `14` | 고정 | 62 | 2 — SageListCtrl.cpp, SageOptionCheck.cpp |  |
| `SAGE_LIST_CHECK_MARK_THICKNESS` | `2` | 고정 | 65 | 1 — SageUiStyle.cpp | 사용처 UiStyle (이름은 LIST) |
| `SAGE_LIST_CHECK_ACCENT_GAP` | `2` | 고정 | 66 | 1 — SageListCtrl.cpp |  |
| `SAGE_LIST_GRID_THICKNESS` | `1` | 고정 | 68 | 2 — SageListBox.cpp, SageListCtrl.cpp |  |
| `SAGE_LIST_CELL_RIGHT_PAD` | `6` | 고정 | 71 | 2 — SageListCtrl.cpp, SageTableTotalBar.cpp |  |
| `SAGE_LIST_CELL_LEFT_PAD` | `6` | 고정 | 72 | 1 — SageTableTotalBar.cpp |  |
| `SAGE_LIST_BADGE_HEIGHT` | `20` | 고정 | 74 | 1 — SageListCtrl.cpp | SAGE_BADGE_HEIGHT와 같은 값 20 |
| `SAGE_LIST_BADGE_PAD_X` | `8` | 고정 | 75 | 1 — SageListCtrl.cpp | SAGE_BADGE_PAD_X와 같은 값 8 |
| `SAGE_LIST_BADGE_RADIUS` | `4` | 고정 | 76 | 1 — SageListCtrl.cpp | SAGE_BADGE_RADIUS와 같은 값 4 |
| `SAGE_RESULT_MIN_HEIGHT` | `160` | 고정 | 140 | 3 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp |  |
| `SAGE_SEARCH_ICON_CELL_WIDTH` | `32` | 고정 | 145 | 2 — SageSearchBox.cpp, SageResultTablePanel.cpp |  |
| `SAGE_SEARCH_CRITERIA_CELL_WIDTH` | `92` | **최소값** (글자 폭) | 146 | 2 — SageSearchBox.cpp, SageResultTablePanel.cpp |  |
| `SAGE_SELECTION_ACCENT_WIDTH` | `3` | 고정 | 171 | 1 — SageSidebarTree.cpp | SAGE_LIST_SELECTION_ACCENT_WIDTH(4)와 값이 다름(3) |
| `SAGE_LIST_SELECTION_ACCENT_WIDTH` | `4` | 고정 | 172 | 2 — SageListBox.cpp, SageListCtrl.cpp | SAGE_SELECTION_ACCENT_WIDTH(3)와 값이 다름(4) |
| `SAGE_RESULT_HEADER_HEIGHT` | `26` | 고정 | 207 | 3 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp |  |
| `SAGE_PILL_HEIGHT` | `28` | 고정 | 217 | 2 — SageFilterPillBar.cpp, SageWorkflowHistoryPanel.cpp |  |
| `SAGE_PILL_PAD_X` | `12` | 고정 | 218 | 1 — SageFilterPillBar.cpp |  |
| `SAGE_PILL_GAP` | `8` | 고정 | 219 | 1 — SageFilterPillBar.cpp |  |
| `SAGE_PILL_RADIUS` | `14` | 고정 | 220 | 1 — SageFilterPillBar.cpp |  |
| `SAGE_RESULT_FIELD_WIDTH` | `140` | 표 열 최소 폭 | 221 | 1 — SageWorkflowResultTable.cpp | WorkflowResultTable 열 폭 → DistributeColumnWidths(T03 미이관, T15에서 QHeaderView로 대체). 픽셀 폭 필요 여부 T15 확인 |
| `SAGE_RESULT_STATUS_WIDTH` | `110` | 표 열 최소 폭 | 222 | 1 — SageWorkflowResultTable.cpp | 위와 같음 (T15 확인) |
| `SAGE_RESULT_REASON_WIDTH` | `320` | 표 열 최소 폭 | 223 | 1 — SageWorkflowResultTable.cpp | 위와 같음 (T15 확인) |
| `SAGE_RESULT_MIN_VALUE_WIDTH` | `220` | 표 열 최소 폭 | 224 | 1 — SageWorkflowResultTable.cpp | 위와 같음 (T15 확인). stretch 열의 최소 폭 |
| `SAGE_RESULT_FILTER_WIDTH` | `150` | **최소값** (글자 폭) | 225 | 1 — SageResultTablePanel.cpp |  |
| `SAGE_RESULT_FILTER_BOX_PAD` | `4` | 고정 | 226 | 1 — SageResultTablePanel.cpp |  |
| `SAGE_RESULT_FILTER_TOP_LIFT` | `8` | 고정 | 227 | 1 — SageResultTablePanel.cpp |  |
| `SAGE_RESULT_RESET_WIDTH` | `84` | **최소값** (글자 폭) | 230 | 1 — SageResultTablePanel.cpp |  |
| `SAGE_SUMMARY_BAR_HEIGHT` | `32` | 고정 | 231 | 1 — SageResultTablePanel.cpp |  |
| `SAGE_SUMMARY_ITEM_GAP` | `16` | 고정 | 232 | 1 — SageSummaryBar.cpp |  |
| `SAGE_SUMMARY_TEXT_GAP` | `6` | 고정 | 233 | 1 — SageSummaryBar.cpp |  |
| `SAGE_SUMMARY_DIVIDER_HEIGHT` | `16` | 고정 | 234 | 1 — SageSummaryBar.cpp |  |
| `SAGE_TOTAL_BAR_HEIGHT` | `40` | 고정 | 235 | 1 — SageResultTablePanel.cpp |  |
| `SAGE_SELECTION_BAR_GAP` | `12` | 고정 | 236 | 1 — SageSelectionBar.cpp |  |
| `SAGE_SELECTION_CHECK_GLYPH_WIDTH` | `20` | 고정 | 237 | 1 — SageSelectionBar.cpp |  |
| `SAGE_SELECTION_CLEAR_PAD` | `12` | 고정 | 238 | 1 — SageSelectionBar.cpp |  |

## 빈 상태 (11개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_EMPTY_ICON_BOX_SIZE` | `44` | 고정 | 88 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ICON_BOX_RADIUS` | `8` | 고정 | 89 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ICON_SIZE` | `22` | 고정 | 90 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ICON_INSET_X` | `3` | 고정 | 91 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ICON_INSET_Y` | `4` | 고정 | 92 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ICON_HEADER_OFFSET` | `5` | 고정 | 93 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ICON_DIVIDER_OFFSET` | `6` | 고정 | 94 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_BLOCK_GAP` | `12` | 고정 | 95 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_TITLE_HEIGHT` | `22` | 고정 | 96 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_DESC_MAX_WIDTH` | `420` | 최대값 | 97 | 1 — SageEmptyState.cpp |  |
| `SAGE_EMPTY_ACTION_WIDTH` | `110` | **최소값** (글자 폭) | 98 | 1 — SageEmptyState.cpp |  |

## 실행 기록 (5개)

| 이름 | 값 (원문) | 쓰는 방식 | 줄 | 쓰는 곳 | 비고 |
|---|---|---|---|---|---|
| `SAGE_HISTORY_TIME_WIDTH` | `124` | 표 열 최소 폭 | 208 | 1 — SageWorkflowHistoryPanel.cpp | DistributeColumnWidths용 열 폭 — T16에서 QHeaderView로 대체 시 필요 여부 확인 |
| `SAGE_HISTORY_RESULT_WIDTH` | `88` | 표 열 최소 폭 | 209 | 1 — SageWorkflowHistoryPanel.cpp | 위와 같음 (T16 확인) |
| `SAGE_HISTORY_INPUT_WIDTH` | `296` | 표 열 최소 폭 | 210 | 1 — SageWorkflowHistoryPanel.cpp | 위와 같음 (T16 확인) |
| `SAGE_HISTORY_OUTPUT_WIDTH` | `360` | 표 열 최소 폭 | 211 | 1 — SageWorkflowHistoryPanel.cpp | 위와 같음 (T16 확인) |
| `SAGE_HISTORY_REASON_WIDTH` | `208` | 표 열 최소 폭 | 212 | 1 — SageWorkflowHistoryPanel.cpp | 위와 같음 (T16 확인) |

합계: 색 41 · 폰트 10 · 여백 · 크기 161 = **212개** (분류표의 `SageDesignDefine.h` 행 수와 같다)

## SageQt에서 정한 값

SageSDI에 없어 사용자가 정한 값이다. 위 합계에 들지 않는다.

| 이름 | 값 | 쓰는 방식 | 정한 곳 |
|---|---|---|---|
| `SAGE_MAIN_WINDOW_WIDTH` | `1280` | 초기 크기 | T11, 사용자 결정 2026-09-28 |
| `SAGE_MAIN_WINDOW_HEIGHT` | `800` | 초기 크기 | T11, 사용자 결정 2026-09-28 |
