# T07 입력 — SageSDI `app/ui/drawing/` 25개 단위 조사

- 대상: `D:/Projects/SageSDI/SageSDI/app/ui/drawing/` (.h/.cpp 25쌍, 합계 3935줄 — `wc -l`)
- 상수 출처: `D:/Projects/SageSDI/SageSDI/SageDefine.h` (값은 파일 그대로 옮김)
- 사용처 수 = `app/` 아래에서 `ui/drawing/` · `external/`을 뺀 파일 중 클래스 이름(단어 단위 grep)이 나오는 **파일 수** (.h/.cpp 각각 1개로 셈). drawing 폴더 내부 사용은 따로 적었다
- 줄 번호는 CRLF 제거 후 `cat -n` 기준
- 판단 근거: `coding-design/references/style.md`(SageStyle 전역 · QSS 금지 · 기본 위젯 서브클래싱 금지 · 변형은 `Q_ENUM` `Q_PROPERTY`), `model-view.md`(표 = QTableView + model/proxy + QHeaderView + delegate), `ui-composition.md`(좌표 지정 금지 · 레이아웃)

## 요약표

| # | 이름 | 분류 | Qt 대응 | 변형 후보 | 사용처 수 | 이관 주제 |
|---|---|---|---|---|---|---|
| 1 | SageBadge | 커스텀 위젯 | 없음 (QLabel은 둥근 배지 모양 없음) | `SageBadgeVariant { Neutral, Warning }` | 1 (+drawing 1) | T12, T15 |
| 2 | SageButton | 기본 위젯 + SageStyle | QPushButton (아이콘 전용은 QToolButton) | `SageButtonVariant { Secondary, Primary, Ghost, Danger }` | 8 (+drawing 4) | T08, T09, T10, T12, T13, T14, T15, T16 |
| 3 | SageComboBox | 필요 없음 | QComboBox | — | 0 | — |
| 4 | SageDialogCaptionBar | 커스텀 위젯 | 없음 (프레임리스 제목 표시줄) | 없음 | 1 | T09 |
| 5 | SageEdit | 기본 위젯 + SageStyle | QLineEdit | `SageLineEditVariant { Normal, Error }` | 4 (+`SageHandleEditSelectAll` 1) | T08, T10 |
| 6 | SageEmptyState | 커스텀 위젯 | 없음 | 없음 | 1 | T16 |
| 7 | SageFilterComboBox | 기본 위젯 + SageStyle | QComboBox | 없음 (확인 못함 — 아래 참고) | 0 (+drawing 1: SageSearchBox) | T15 |
| 8 | SageFilterPillBar | 커스텀 위젯 | 없음 | 없음 (선택은 상태) | 1 | T16 |
| 9 | SageHeaderCtrl | 기본 위젯 + SageStyle | QHeaderView | 없음 | 2 | T15, T16 |
| 10 | SageInlineError | 커스텀 위젯 | 없음 | `SageInlineMessageVariant { Error, Warning }` | 2 | T10 |
| 11 | SageLabel | 기본 위젯 + SageStyle | QLabel | `SageLabelVariant { Title, SecondaryCaption, MutedCaption, FormLabel, Hint, SidebarLogo }` | 10 | T10, T11, T12, T13 |
| 12 | SageListBox | 필요 없음 | QListView (+delegate) | — | 0 | — |
| 13 | SageListCtrl | delegate | QTableView + model/proxy + QHeaderView + QStyledItemDelegate | (delegate — 변형 대신 model role) | 2 | T15, T16 |
| 14 | SageMessageBody | 기본 위젯 + SageStyle | QLabel ×2 (아이콘 · 본문) + `SageStyle::standardIcon` | 없음 (`QStyle::StandardPixmap`로 대체) | 1 | T09 |
| 15 | SageOptionCheck | 필요 없음 | QCheckBox | — | 0 | — |
| 16 | SageSearchBox | 커스텀 위젯 | QComboBox + QLineEdit(+trailing QAction) 조합 | 없음 | 1 | T15 |
| 17 | SageSectionLabel | 기본 위젯 + SageStyle | QLabel | `SageLabelVariant`에 `Section` 추가 | 2 | T13, T15 |
| 18 | SageSelectionBar | 기본 위젯 + SageStyle | QCheckBox + QLabel + QPushButton 조합 | 없음 (자식이 기존 변형 사용) | 1 | T15 |
| 19 | SageSidebarTree | delegate | QTreeView + QStyledItemDelegate | (delegate) | 1 | T11 |
| 20 | SageStatusCard | 커스텀 위젯 | 없음 (내부 진행 막대만 QProgressBar 후보) | `SageStatusCardVariant { Idle, Running, Completed, Failed }` | 1 | T14 |
| 21 | SageSummaryBar | 커스텀 위젯 | 없음 | 없음 (항목 단위 강조 · 배지) | 1 | T15 |
| 22 | SageTabCtrl | 기본 위젯 + SageStyle | QTabBar (+QStackedWidget) | 없음 | 1 | T13 |
| 23 | SageTableTotalBar | 커스텀 위젯 | 없음 (표 합계 바닥줄) | 없음 (셀 종류 enum은 따로) | 1 | T15 |
| 24 | SageUiResources | 기타 | 앱 폰트 · QFont 역할 · QPalette · `SageDesignDefine.h` | — | 9 (+루트 `SageSDI.cpp` 1) | T08 |
| 25 | SageUiStyle | 기타 | `SageStyle` primitive + `ui/style/` 공용 그리기 조각 | — | 0 (+drawing 6) | T08 |

행 수: 25 (확인함).

분류별 수: 기본 위젯 + SageStyle 9 · 커스텀 위젯 9 · delegate 2 · 필요 없음 3 · 기타 2.

---

## 단위별 상세

### 1. SageBadge

1. **파일 · 줄 수**: `SageBadge.h` 25 + `SageBadge.cpp` 78 = 103
2. **역할**: 둥근 사각형 배지 + 가운데 한 줄 텍스트. `DrawItem`(cpp 50–78): 전체를 `m_clrSurface`로 채우고, 세로 가운데에 높이 `SAGE_BADGE_HEIGHT` 사각형을 `RoundRect`(66)로 그린 뒤 `SAGE_FONT_CAPTION`으로 텍스트(73). `GetContentWidth`(39–48)는 텍스트 폭 + `SAGE_BADGE_PAD_X*2` — 호출자가 이 값으로 크기를 정함
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**: enum 없음. 색 3개(배경 · 테두리 · 글자)를 호출자가 `SetBadge`로 넘긴다. 실제 조합 2가지:
   - 헤더 역할 배지(`SageHeaderPanel.cpp` 86–90): `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)` / `SAGE_COLOR_LIST_HEADER_BORDER = RGB(228, 223, 215)` / `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)` — 생성자 기본값과 같음(cpp 10–12)
   - 요약줄 배지(`SageSummaryBar.cpp` 50–56, 값은 `SageResultTablePanel.cpp` 412–414): `SAGE_COLOR_INLINE_WARN_BG = RGB(251, 245, 238)` / `SAGE_COLOR_INLINE_WARN_BORDER = RGB(235, 220, 198)` / `SAGE_COLOR_WARNING = RGB(184, 135, 70)`, 모서리 `SAGE_BADGE_RADIUS = 4`, 바탕 `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`
   - 기본 바탕 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, 기본 모서리 `SAGE_BADGE_HEIGHT = 20`, 패딩 `SAGE_BADGE_PAD_X = 8`, 테두리 `SAGE_BORDER_THICKNESS = 1`
5. **사용처**: 1 — `ui/panels/SageHeaderPanel.h`. drawing 내부: `SageSummaryBar.h`(자식 멤버)
6. **Qt 대응**: 없음
7. **분류**: 커스텀 위젯
8. **변형 후보**: `SageBadgeVariant { Neutral, Warning }` (위 두 색 조합에서만 도출. 모서리 차이는 변형에 묶을지 확인 못함)
9. **이관 주제**: T12 (헤더 역할 배지), T15 (요약줄 배지)
10. **주의**:
   - `RoundRect`의 두 번째 인자는 **타원 폭·높이**다. 배지는 `m_nCornerRadius`를 그대로 넘겨(66) 기본 20 → 반지름 10(높이 20이므로 알약 모양), 요약줄 4 → 실제 반지름 2. 다른 단위(필 바 · 인라인 오류 · 빈 상태)는 `RADIUS * 2`를 넘긴다. Qt `drawRoundedRect(r, radius, radius)`로 옮길 때 상수를 그대로 쓰면 요약줄 배지 모서리가 두 배로 둥글어진다
   - 폭은 호출자가 `GetContentWidth`로 계산해 `MoveWindow` — Qt에서는 `sizeHint()`로 옮겨야 한다

### 2. SageButton

1. **파일 · 줄 수**: `SageButton.h` 50 + `SageButton.cpp` 219 = 269
2. **역할**: owner-draw 버튼. `DrawItem`(cpp 62–130): 변형별 배경 · 테두리 · 글자색, 포커스 링, 아이콘+텍스트 그룹 가운데 정렬. 아이콘은 GDI 선으로 직접 그림(`DrawAddIcon` 155, `DrawCloseIcon` 165, `DrawArrowIcon` 179, `DrawResetIcon` 198, 검색은 `SageUiStyle::DrawSearchIcon` 152). 툴팁은 자체 `CToolTipCtrl` + `PreTranslateMessage`의 `RelayEvent`(45–49)
3. **MFC 기반**: `CButton` (BS_OWNERDRAW)
4. **상태 · 변형**:
   - 변형 enum `SageButtonVariant`(h 3–8): `SAGE_BUTTON_SECONDARY`(기본), `SAGE_BUTTON_PRIMARY`, `SAGE_BUTTON_GHOST`, `SAGE_BUTTON_DANGER`
   - 상태: pressed(`ODS_SELECTED`), disabled(`ODS_DISABLED`), focus(`ODS_FOCUS`, `m_bFocusRing`일 때만). **hover 없음**
   - Primary(74–78): 배경 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`, pressed `SAGE_COLOR_PRIMARY_PRESS = RGB(118, 80, 42)`, disabled `SAGE_COLOR_BORDER = RGB(220, 214, 205)`; 글자 `SAGE_COLOR_BUTTON_TEXT = RGB(255, 255, 255)`, disabled `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`
   - Ghost(79–81): 배경 `m_clrSurface`(기본 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`), pressed `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`; 글자 `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`, disabled `SAGE_COLOR_BORDER`
   - Secondary/Danger(82–93): 배경 `SAGE_COLOR_PANEL`, pressed·disabled `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`; 테두리 `SAGE_COLOR_BUTTON_BORDER = RGB(201, 191, 177)` / Danger `SAGE_COLOR_DANGER_BORDER = RGB(224, 189, 182)` / disabled `SAGE_COLOR_BORDER`; 글자 `SAGE_COLOR_TEXT = RGB(47, 42, 36)` / Danger `SAGE_COLOR_ERROR = RGB(184, 92, 74)` / disabled `SAGE_COLOR_SECONDARY_TEXT`
   - 포커스 링(57–60, 68–72): Primary `SAGE_COLOR_FOCUS_RING_PRIMARY = RGB(240, 228, 213)`, 그 외 `SAGE_COLOR_FOCUS_RING_NEUTRAL = RGB(239, 235, 227)`, 폭 `SAGE_FOCUS_RING_WIDTH = 2`
   - 아이콘 enum `SageButtonIcon`(h 10–18): NONE, SEARCH, RESET, ADD, CLOSE, MOVE_UP, MOVE_DOWN. 치수 `SAGE_ICON_SIZE = 15`, `SAGE_ICON_ADD_SIZE = 14`, `SAGE_ICON_STROKE = 2`, `SAGE_ICON_ADD_SPAN = 10`, `SAGE_ICON_TEXT_GAP = 6`, `SAGE_ICON_RESET_RADIUS = 6`, `SAGE_ICON_RESET_ARROW = 3`, `SAGE_ICON_CLOSE_SPAN = 10`, `SAGE_ICON_ARROW_HALF_WIDTH = 4`, `SAGE_ICON_ARROW_HALF_HEIGHT = 2`, `SAGE_BUTTON_TEXT_TOP_OFFSET = 0`
   - 실제 쓰이는 변형: PRIMARY(로그인 · 비번 · 메시지 상자 · 생성 · 빈 상태 동작), SECONDARY(메시지 상자 거절 · 상태 카드 폴더 열기), GHOST(입력 초기화 · 결과 초기화 · 캡션 닫기 · 선택 해제), DANGER(확인형 메시지 상자 수락, `SageMessageBoxDlg.cpp` 98)
   - 실제 쓰이는 아이콘: RESET(`SageResultTablePanel.cpp` 93), CLOSE(`SageDialogCaptionBar.cpp` 28)뿐. SEARCH · ADD · MOVE_UP · MOVE_DOWN은 사용처 0
5. **사용처**: 8 — `ui/dialogs/SageLoginDlg.h`, `ui/dialogs/SageMessageBoxDlg.cpp`, `ui/dialogs/SageMessageBoxDlg.h`, `ui/dialogs/SagePasswordChangeDlg.h`, `ui/panels/SageHeaderPanel.h`, `ui/panels/SageResultTablePanel.h`, `ui/panels/SageWorkflowInputPanel.cpp`, `ui/panels/SageWorkflowInputPanel.h`. drawing 내부: `SageDialogCaptionBar`, `SageEmptyState`, `SageSelectionBar`, `SageStatusCard`
6. **Qt 대응**: QPushButton (아이콘만 있는 닫기 버튼은 QToolButton 후보)
7. **분류**: 기본 위젯 + SageStyle
8. **변형 후보**: `SageButtonVariant { Secondary, Primary, Ghost, Danger }`. 아이콘은 enum 대신 `QIcon`(RESET · CLOSE 두 개만 필요)
9. **이관 주제**: T08(스타일 기반), T09(캡션 닫기 · 메시지 상자), T10(로그인 · 비번), T12(로그인/로그아웃), T13(파일 선택 · 생성 · 초기화), T14(폴더 열기), T15(결과 초기화 · 선택 해제), T16(빈 상태 동작 — 현재 미사용)
10. **주의**:
   - hover 모양이 없다. Fusion 기반 SageStyle은 hover를 그릴 수 있으므로 넣을지 뺄지 정해야 한다(그대로 두면 모양이 바뀜)
   - 포커스 표시는 `SetFocusRing(TRUE)`인 버튼(메시지 상자 기본 버튼, `SageMessageBoxDlg.cpp` 106–109)에만 있다. Fusion은 모든 버튼에 포커스 표시를 그린다
   - 포커스 링이 켜진 버튼은 포커스가 없어도 바깥 2px를 surface 색으로 칠하고 본체를 줄인다(68–72) → 같은 크기라도 본체가 4px 작다
   - Ghost의 바탕은 `SetSurfaceColor`로 부모 배경을 흉내낸다(`SageResultTablePanel.cpp` 94 APP_BACKGROUND, `SageWorkflowInputPanel.cpp` 81 PANEL). Qt에서는 투명 바탕이면 이 설정이 필요 없음
   - 텍스트+아이콘 그룹은 `GetTextExtent`로 직접 가운데 계산(116–126) — Qt는 스타일의 `CE_PushButtonLabel` 배치로 바뀜
   - 툴팁 중계(`RelayEvent`)는 Qt `setToolTip`으로 대체 — 표시 지연·위치가 달라질 수 있음

### 3. SageComboBox

1. **파일 · 줄 수**: `SageComboBox.h` 20 + `SageComboBox.cpp` 103 = 123
2. **역할**: `OnPaint`(cpp 87–103) 필드 영역을 `SAGE_COLOR_PANEL`, 버튼 영역을 `SAGE_COLOR_APP_BACKGROUND`로 채우고 `SageUiStyle::DrawComboArrow`. `ApplyFieldHeight`(16–45)는 닫힌 높이를 `SAGE_EDIT_HEIGHT`에 맞출 때까지 `SetItemHeight(-1)`를 최대 `SAGE_COMBO_FIT_MAX_PASS`회 반복, `ApplyTextRect`(47–72)는 편집 자식 창을 글꼴 높이만큼 세로 가운데로 옮김
3. **MFC 기반**: `CComboBox`
4. **상태 · 변형**: 없음. `SAGE_EDIT_HEIGHT = 32`, `SAGE_COMBO_FIT_MAX_PASS = 3`, `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`
5. **사용처**: 0 (저장소 전체에서 drawing 자기 파일과 `SageSDI.vcxproj`에만 나옴)
6. **Qt 대응**: QComboBox
7. **분류**: 필요 없음 (사용처 0, 높이 맞추기는 Win32 콤보 구조 때문)
8. **변형 후보**: —
9. **이관 주제**: — (콤보 모양 자체는 T08 SageStyle이 담당)
10. **주의**: 없음(미사용)

### 4. SageDialogCaptionBar

1. **파일 · 줄 수**: `SageDialogCaptionBar.h` 25 + `SageDialogCaptionBar.cpp` 72 = 97
2. **역할**: 프레임리스 다이얼로그의 제목 띠. `OnPaint`(cpp 45–64): 배경 `SAGE_COLOR_LIST_HEADER`, 아래 1px `SAGE_COLOR_BORDER`, 제목을 `SAGE_FONT_CONTENT_SEMIBOLD` · `SAGE_COLOR_TEXT`로 왼쪽 정렬 · 말줄임. 오른쪽에 Ghost + CLOSE 아이콘 버튼(26–29, 툴팁 `SAGE_UI_TIP_CLOSE`). `OnNcHitTest`(66–68)가 `HTTRANSPARENT`를 돌려 부모 `SageFramelessDialog::OnNcHitTest`(dialogs/SageFramelessDialog.cpp 68–78)가 캡션 높이 안을 `HTCAPTION`으로 처리 → 끌어서 이동. 닫기 클릭은 부모에 `WM_COMMAND(nCloseCommandId)` 전송(70–72). `Layout`(33–39)이 좌표를 직접 지정
3. **MFC 기반**: `CWnd`
4. **상태 · 변형**: 없음. `SAGE_DLG_CAPTION_HEIGHT = 40`, `SAGE_DLG_CAPTION_PAD = 16`, `SAGE_DLG_CAPTION_BTN_SIZE = 28`, `SAGE_DLG_CAPTION_BTN_PAD = 8`, `ID_SAGE_DLG_CLOSE = 41200`, `SAGE_UI_TIP_CLOSE = L"닫기"`, `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`, `SAGE_COLOR_BORDER = RGB(220, 214, 205)`, `SAGE_COLOR_TEXT = RGB(47, 42, 36)`
5. **사용처**: 1 — `ui/dialogs/SageFramelessDialog.h`
6. **Qt 대응**: 없음 (Qt::FramelessWindowHint 창의 제목 표시줄은 직접 만든다. 이동은 `QWindow::startSystemMove` 후보)
7. **분류**: 커스텀 위젯
8. **변형 후보**: 없음
9. **이관 주제**: T09
10. **주의**:
   - 끌기 이동이 "자식은 HTTRANSPARENT, 부모는 HTCAPTION"이라는 Win32 히트 테스트에 기대고 있다. Qt에서는 마우스 누름에서 이동을 시작해야 하며 macOS · Linux(Wayland 포함) 동작은 확인 못함
   - 닫기 버튼은 `SetSurfaceColor`를 부르지 않아 Ghost 기본 바탕 `SAGE_COLOR_PANEL`(흰색)로 칠해진다 — 캡션 띠(`LIST_HEADER`) 위에 흰 사각형으로 보일 것으로 코드상 읽힘. 의도인지 확인 못함
   - 제목 폭은 오른쪽 버튼 자리만큼 뺀 고정 계산(55–56)

### 5. SageEdit

1. **파일 · 줄 수**: `SageEdit.h` 31 + `SageEdit.cpp` 85 = 116
2. **역할**: 테마 · 기본 테두리 제거(`OnCreate` 27–33, `SetWindowTheme` · `ModifyStyle(WS_BORDER)`), 비클라이언트 1px(`OnNcCalcSize` 35–39) 테두리를 `OnNcPaint`(62–71)에서 상태별 색으로 그림. `CtlColor`(50–60, 반사) 활성/비활성 글자·바탕색. 자유 함수 `SageHandleEditSelectAll`(73–85): Ctrl+A → `SetSel(0,-1)`, `PreTranslateMessage`(21–25)에서 호출
3. **MFC 기반**: `CEdit`
4. **상태 · 변형**:
   - enum `SageEditState`(h 3–7): `SAGE_EDIT_NORMAL`, `SAGE_EDIT_ERROR`
   - 테두리: 오류 `SAGE_COLOR_ERROR = RGB(184, 92, 74)`, 보통 `SAGE_COLOR_BORDER = RGB(220, 214, 205)`, 두께 `SAGE_BORDER_THICKNESS = 1`
   - 활성: 글자 `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, 바탕 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`; 비활성: 글자 `SAGE_COLOR_TEXT_PLACEHOLDER = RGB(180, 171, 160)`, 바탕 `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`
   - focus · hover 상태는 없음
   - `SAGE_KEY_SELECT_ALL = 'A'`
5. **사용처**: 4 — `ui/dialogs/SageLoginDlg.cpp`, `ui/dialogs/SageLoginDlg.h`, `ui/dialogs/SagePasswordChangeDlg.cpp`, `ui/dialogs/SagePasswordChangeDlg.h`. `SageHandleEditSelectAll`: 1 — `ui/view/SageSDIView.cpp`(84, 모든 CEdit에 적용)
6. **Qt 대응**: QLineEdit
7. **분류**: 기본 위젯 + SageStyle
8. **변형 후보**: `SageLineEditVariant { Normal, Error }` (오류 표시는 상태에 가깝지만 규칙상 `Q_ENUM` `Q_PROPERTY`로 SageStyle이 읽음)
9. **이관 주제**: T08, T10 (`ShowInputError`에서 ERROR, 초기화에서 NORMAL — `SageLoginDlg.cpp` 148/158–159, `SagePasswordChangeDlg.cpp` 171/182–184)
10. **주의**:
   - Fusion은 포커스된 입력칸 테두리를 강조한다. MFC는 포커스 표시가 없다 → 그대로 두면 모양이 바뀜
   - Ctrl+A 전체 선택은 QLineEdit 기본 동작(`QKeySequence::SelectAll`, macOS는 Cmd+A)이라 `SageHandleEditSelectAll`은 필요 없음. 단, MFC는 View 전체의 CEdit에 걸었으므로 Qt 기본 동작과 같은지 여러 줄 입력칸이 생기면 다시 확인
   - 부모 다이얼로그의 `OnCtlColor`가 `IsKindOf(RUNTIME_CLASS(CSageEdit))`로 건너뛰는 구조(`SageLoginDlg.cpp` 217) — Qt에는 없음
   - 비활성 바탕을 `LIST_HEADER`로 바꾸는 규칙은 Qt `QPalette::Disabled` 그룹 값으로 옮겨야 한다

### 6. SageEmptyState

1. **파일 · 줄 수**: `SageEmptyState.h` 33 + `SageEmptyState.cpp` 173 = 206
2. **역할**: 빈 목록 안내. `DrawItem`(cpp 139–173): 흰 바탕 + 1px 테두리, 가운데에 둥근 아이콘 상자(`DrawIconBox` 102–137 — 표 모양 선 그림), 제목(`SAGE_FONT_HEADER`), 설명(`SAGE_FONT_CONTENT`, 줄바꿈, 최대 폭 420), 선택적 Primary 동작 버튼(`OnCreate` 16–25, `LayoutActionButton` 85–100, 클릭은 부모에 `WM_COMMAND` 45–49). 블록 전체 세로 가운데(`MeasureBlockHeight` 71–83)
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**: enum 없음. 설명 있음/없음, 동작 버튼 있음/없음(내용 차이). 색: 바탕 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, 테두리 `SAGE_COLOR_BORDER = RGB(220, 214, 205)`, 아이콘 상자 `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`, 아이콘 선 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`, 제목 `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, 설명 `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`. 치수 `SAGE_EMPTY_ICON_BOX_SIZE = 44`, `SAGE_EMPTY_ICON_BOX_RADIUS = 8`, `SAGE_EMPTY_ICON_SIZE = 22`, `SAGE_EMPTY_ICON_INSET_X = 3`, `SAGE_EMPTY_ICON_INSET_Y = 4`, `SAGE_EMPTY_ICON_HEADER_OFFSET = 5`, `SAGE_EMPTY_ICON_DIVIDER_OFFSET = 6`, `SAGE_EMPTY_BLOCK_GAP = 12`, `SAGE_EMPTY_TITLE_HEIGHT = 22`, `SAGE_EMPTY_DESC_MAX_WIDTH = 420`, `SAGE_EMPTY_ACTION_WIDTH = 110`, `SAGE_BUTTON_HEIGHT = 32`, `ID_EMPTY_STATE_ACTION = 41029`
5. **사용처**: 1 — `ui/panels/SageWorkflowHistoryPanel.h` (`SetContent`만 호출: `SageWorkflowHistoryPanel.cpp` 68, 192. `SetAction`은 사용처 0 → 동작 버튼은 현재 한 번도 보이지 않음)
6. **Qt 대응**: 없음
7. **분류**: 커스텀 위젯
8. **변형 후보**: 없음
9. **이관 주제**: T16
10. **주의**:
   - 설명 높이는 `DT_WORDBREAK | DT_CALCRECT`로 측정 — Qt 줄바꿈 규칙(한글 단어 경계)과 결과가 다를 수 있음
   - 동작 버튼 경로는 미사용이므로 옮길지 결정 필요 (CLAUDE.md 2 — 요청 없는 기능 금지)

### 7. SageFilterComboBox

1. **파일 · 줄 수**: `SageFilterComboBox.h` 20 + `SageFilterComboBox.cpp` 77 = 97
2. **역할**: owner-draw 고정 높이 콤보(`CBS_OWNERDRAWFIXED`). `MeasureItem`(cpp 25–27) 항목 높이 = `m_nFieldHeight`. `DrawItem`(29–47) 목록 항목: 선택 항목은 `SAGE_COLOR_PRIMARY` 바탕 · `SAGE_COLOR_PANEL` 글자, 그 외 `SAGE_COLOR_PANEL` 바탕 · `SAGE_COLOR_TEXT` 글자, 가운데 정렬. `OnPaint`(49–77) 닫힌 필드: `m_clrField`로 채우고 화살표 + 선택 텍스트 가운데 정렬
3. **MFC 기반**: `CComboBox`
4. **상태 · 변형**: 목록 항목 selected/보통, 필드(`ODS_COMBOBOXEDIT`). 필드 색 기본 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, 검색 상자 안에서는 `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`(`SageSearchBox.cpp` 42). 선택 바탕 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`, 글자 `SAGE_COLOR_TEXT = RGB(47, 42, 36)`. 항목 높이 기본 `SAGE_EDIT_HEIGHT - SAGE_COMBO_FIELD_INSET`(32 − 6), 검색 상자에서 `SAGE_EDIT_HEIGHT - SAGE_EDIT_BORDER_WIDTH * 2 - SAGE_COMBO_FIELD_INSET`(`SageSearchBox.cpp` 33–34; `SAGE_EDIT_BORDER_WIDTH = 1`). disabled · hover 처리 없음
5. **사용처**: 0 (drawing 밖). drawing 내부 1 — `SageSearchBox.h`(멤버 `m_wndCriteria`)
6. **Qt 대응**: QComboBox (목록 항목은 SageStyle의 item view 그리기 또는 콤보 전용 delegate)
7. **분류**: 기본 위젯 + SageStyle (SageSearchBox를 통해 간접 사용)
8. **변형 후보**: 없음. 필드 색 차이(PANEL / APP_BACKGROUND)를 변형으로 둘지, 검색 상자 안 위치에서 오는 것으로 볼지 확인 못함
9. **이관 주제**: T15
10. **주의**:
   - 필드 · 목록 모두 텍스트 **가운데 정렬**(44, 73). Fusion 기본은 왼쪽 정렬
   - 목록 선택 강조가 PRIMARY 바탕 + 흰 글자 — `QPalette::Highlight`/`HighlightedText` 값으로 옮기면 앱 전체 선택색과 묶이므로 따로 둘지 확인 필요
   - 드롭다운 높이를 `MoveWindow(..., SAGE_EDIT_HEIGHT * nDropRows)`(`SageSearchBox.cpp` 43, `SAGE_RESULT_CRITERIA_DROP_ROWS = 8`)로 정함 — Qt는 `setMaxVisibleItems`

### 8. SageFilterPillBar

1. **파일 · 줄 수**: `SageFilterPillBar.h` 28 + `SageFilterPillBar.cpp` 117 = 145
2. **역할**: 가로로 늘어선 알약 모양 단일 선택 필터. `BuildPillRects`(cpp 39–53) 글자 폭 + 패딩으로 알약 사각형 계산, `DrawPill`(86–101) 둥근 사각형 + 가운데 글자, `DrawItem`(103–117). `OnLButtonDown`(72–84) → `FindPillAt`(55–70) 히트 테스트, 선택이 바뀔 때만 부모에 `WM_COMMAND`
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**: selected / 보통. selected: 채움 `SAGE_COLOR_ACCENT_SURFACE = RGB(247, 242, 234)`, 테두리 · 글자 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`. 보통: 채움 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, 테두리 `SAGE_COLOR_BORDER = RGB(220, 214, 205)`, 글자 `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`. 바탕 `SAGE_BG_APP`(= `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`). 치수 `SAGE_PILL_HEIGHT = 28`, `SAGE_PILL_PAD_X = 12`, `SAGE_PILL_GAP = 8`, `SAGE_PILL_RADIUS = 14`, 글꼴 `SAGE_FONT_CAPTION`. hover · disabled · focus 없음
5. **사용처**: 1 — `ui/panels/SageWorkflowHistoryPanel.h` (라벨 "전체 %d" / "성공 %d" / "실패 %d", `SageWorkflowHistoryPanel.cpp` 90–96; 선택 인덱스를 `SAGE_HISTORY_FILTER_SUCCESS = 1`, `SAGE_HISTORY_FILTER_FAILED = 2`와 비교, 99–105)
6. **Qt 대응**: 없음 (QButtonGroup + 체크형 QPushButton 조합으로도 가능하나 지시문 기준 필 바는 커스텀 위젯)
7. **분류**: 커스텀 위젯
8. **변형 후보**: 없음 (선택은 상태 — `currentIndex` 속성 + `currentIndexChanged` signal 후보)
9. **이관 주제**: T16
10. **주의**:
   - 키보드로 선택 불가(CStatic, 포커스 없음). Qt에서 포커스 · 방향키를 넣으면 동작이 늘어남 — 결정 필요
   - 필터 의미를 **인덱스 번호**로 전달(패널이 1/2와 비교) — Qt에서도 인덱스 계약을 유지할지 확인
   - 라벨 수가 바뀌어 선택이 범위를 벗어나면 0으로 되돌림(21–22), 이때 signal은 보내지 않음

### 9. SageHeaderCtrl

1. **파일 · 줄 수**: `SageHeaderCtrl.h` 9 + `SageHeaderCtrl.cpp` 52 = 61
2. **역할**: 목록 머리글. `OnHeaderLayout`(cpp 10–19)이 `HDM_LAYOUT`에서 높이를 `SAGE_LIST_HEADER_HEIGHT`로 고정, `OnPaint`(21–52)가 전체를 `SAGE_COLOR_LIST_HEADER`로 칠하고 각 칸 텍스트를 `SAGE_COLOR_TEXT_MUTED`로 **가운데 정렬 · 말줄임**. 칸 구분선 없음
3. **MFC 기반**: `CHeaderCtrl` (패널이 `SubclassWindow`로 목록의 머리글에 붙임 — `SageResultTablePanel.cpp` 104, `SageWorkflowHistoryPanel.cpp` 63)
4. **상태 · 변형**: 없음(hover · pressed · 정렬 표시 없음). `SAGE_LIST_HEADER_HEIGHT = 36`, `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`, `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`. 글꼴은 패널이 `SAGE_FONT_LIST`로 지정(`SageResultTablePanel.cpp` 114)
5. **사용처**: 2 — `ui/panels/SageResultTablePanel.h`, `ui/panels/SageWorkflowHistoryPanel.h`
6. **Qt 대응**: QHeaderView
7. **분류**: 기본 위젯 + SageStyle (`CE_HeaderSection` / `CE_HeaderLabel`)
8. **변형 후보**: 없음
9. **이관 주제**: T15, T16
10. **주의**:
   - 열 정렬 형식(LVCFMT)을 무시하고 전부 가운데 — Qt는 `Qt::TextAlignmentRole`(헤더) 또는 `setDefaultAlignment`로 옮김
   - 칸 사이 구분선 · 누름 모양 · 정렬 화살표가 없음. Fusion은 셋 다 그린다 → 끄지 않으면 모양이 바뀜
   - 텍스트 버퍼 255자(44)

### 10. SageInlineError

1. **파일 · 줄 수**: `SageInlineError.h` 36 + `SageInlineError.cpp` 107 = 143
2. **역할**: 입력칸 아래 한 줄 메시지. `DrawItem`(cpp 79–107): 바탕 역할 색으로 채움, 경고면 둥근 상자(`DrawWarningBox` 48–59), 원 + 세로선 + 점 아이콘(`DrawMessageIcon` 61–77), `SAGE_FONT_CAPTION` 한 줄 말줄임 텍스트
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**:
   - enum `SageInlineMessageVariant`(h 5–9): `SAGE_INLINE_ERROR`, `SAGE_INLINE_WARNING`
   - 아이콘: 오류 `SAGE_COLOR_ERROR = RGB(184, 92, 74)`, 경고 `SAGE_COLOR_WARNING = RGB(184, 135, 70)`
   - 글자: 오류 `SAGE_COLOR_INLINE_ERROR_TEXT = RGB(156, 68, 51)`, 경고 `SAGE_COLOR_INLINE_WARN_TEXT = RGB(138, 106, 50)`
   - 경고 상자: `SAGE_COLOR_INLINE_WARN_BG = RGB(251, 245, 238)`, `SAGE_COLOR_INLINE_WARN_BORDER = RGB(235, 220, 198)`, `SAGE_INLINE_MSG_BOX_RADIUS = 4`, `SAGE_INLINE_MSG_BOX_PAD_X = 10`
   - 메시지 없음 상태(`ClearMessage` / `HasMessage`) → 바탕만 칠함
   - 치수 `SAGE_INLINE_MSG_HEIGHT = 24`(호출자), `SAGE_INLINE_MSG_ICON_SIZE = 14`, `SAGE_INLINE_MSG_ICON_GAP = 8`, `SAGE_INLINE_ICON_RADIUS = 5`, `SAGE_INLINE_ICON_STEM_TOP = 4`, `SAGE_INLINE_ICON_STEM_BOTTOM = 8`, `SAGE_INLINE_ICON_DOT_TOP = 10`, `SAGE_INLINE_ICON_DOT_SIZE = 2`
   - 바탕 역할: 기본 `SAGE_BG_APP`, 다이얼로그는 `SAGE_BG_PANEL`
5. **사용처**: 2 — `ui/dialogs/SageLoginDlg.h`, `ui/dialogs/SagePasswordChangeDlg.h` (둘 다 `SAGE_INLINE_ERROR`만 사용. `SAGE_INLINE_WARNING` 사용처 0)
6. **Qt 대응**: 없음
7. **분류**: 커스텀 위젯
8. **변형 후보**: `SageInlineMessageVariant { Error, Warning }` — Warning은 현재 미사용이라 옮길지 결정 필요
9. **이관 주제**: T10
10. **주의**:
   - 한 줄 말줄임(`DT_END_ELLIPSIS`) — 긴 메시지가 잘린다. Qt에서 줄바꿈으로 바꾸면 높이 계약(24px)이 깨짐
   - 점은 `FillSolidRect(ptCenter.x, ...)`로 중심에서 오른쪽으로 2px(72) — 원 중심보다 1px 오른쪽에 치우침. 같은 점을 `SageMessageBody`는 `- SIZE/2`로 보정(55). 픽셀 그대로 옮길지 확인 필요
   - 바탕 역할(`SetBackgroundRole`)은 부모 배경 흉내 — 투명 바탕이면 불필요

### 11. SageLabel

1. **파일 · 줄 수**: `SageLabel.h` 23 + `SageLabel.cpp` 35 = 58
2. **역할**: 정적 텍스트의 글자색 · 바탕색 · 글꼴을 역할로 지정. `CtlColor`(cpp 31–35, `ON_WM_CTLCOLOR_REFLECT`)가 역할 색 적용, `SetFontRole`(27–29)이 `SetFont`
3. **MFC 기반**: `CStatic` (`DECLARE_DYNAMIC` — 부모들이 `IsKindOf`로 판별)
4. **상태 · 변형**: 글자 역할(`SageTextRole`), 바탕 역할(`SageBackgroundRole`), 글꼴 역할(`SageFontRole`) 조합. 실제 쓰인 조합:
   - `SAGE_TEXT_DEFAULT` + `SAGE_FONT_TITLE` — 헤더 제목(`SageHeaderPanel.cpp` 37–39)
   - `SAGE_TEXT_SECONDARY` + `SAGE_FONT_CAPTION` — 헤더 분류(41–43), 비번 힌트(`SagePasswordChangeDlg.cpp` 148–150)
   - `SAGE_TEXT_MUTED` + `SAGE_FONT_CAPTION` — 헤더 사용자(45–47)
   - `SAGE_TEXT_MUTED` + 본문 글꼴 — 입력 라벨(`SageWorkflowInputPanel.cpp` 104–110, `SAGE_FONT_CONTENT`), 로그인 · 비번 라벨(`SageLoginDlg.cpp` 121–128, `CreatePointFont(SAGE_CONTENT_FONT_POINT_SIZE, SAGE_CONTROL_FONT_FACE)` — CONTENT와 같은 값)
   - `SAGE_TEXT_SECONDARY` + `SAGE_FONT_CONTENT` — 입력 빈 상태 힌트(`SageWorkflowInputPanel.cpp` 86–87)
   - `SAGE_TEXT_SIDEBAR` + `SAGE_FONT_LOGO` — 사이드바 로고(`SageSidebarPanel.cpp` 48–50)
   - 색 값: `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`, `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`, `SAGE_COLOR_SIDEBAR_TEXT = RGB(205, 196, 185)`
   - 바탕 역할은 `SAGE_BG_PANEL` / `SAGE_BG_SIDEBAR` / 기본 `SAGE_BG_APP` — 부모 배경과 같게 맞추는 용도
5. **사용처**: 10 — `ui/dialogs/SageLoginDlg.cpp`, `ui/dialogs/SageLoginDlg.h`, `ui/dialogs/SagePasswordChangeDlg.cpp`, `ui/dialogs/SagePasswordChangeDlg.h`, `ui/panels/SageHeaderPanel.h`, `ui/panels/SageResultTablePanel.cpp`(IsKindOf만), `ui/panels/SageSidebarPanel.h`, `ui/panels/SageWorkflowInputPanel.cpp`, `ui/panels/SageWorkflowInputPanel.h`, `ui/view/SageSDIView.cpp`(IsKindOf만)
6. **Qt 대응**: QLabel
7. **분류**: 기본 위젯 + SageStyle
8. **변형 후보**: `SageLabelVariant { Title, SecondaryCaption, MutedCaption, FormLabel, Hint, SidebarLogo }` (위 조합 6개에서 도출; 이름은 제안)
9. **이관 주제**: T10(다이얼로그 라벨 · 힌트), T11(사이드바 로고), T12(헤더 제목 · 분류 · 사용자), T13(입력 라벨 · 빈 상태 힌트)
10. **주의**:
   - 부모 4곳의 `OnCtlColor`가 `IsKindOf(RUNTIME_CLASS(CSageLabel))`이면 건너뛰는 규약에 의존(`SageSDIView.cpp` 248, `SageLoginDlg.cpp` 214, `SagePasswordChangeDlg.cpp` 239, `SageResultTablePanel.cpp` 259, `SageWorkflowInputPanel.cpp` 254). Qt에는 해당 구조가 없음
   - MFC는 역할 바탕 브러시로 **불투명**하게 칠한다. Qt QLabel은 기본 투명 → 부모 배경이 같으면 결과 동일, 다르면 달라짐
   - QLabel 글꼴 · 색을 변형 속성으로 SageStyle이 정하는 방법(polish 시점, 속성 변경 후 재적용)은 확인 못함

### 12. SageListBox

1. **파일 · 줄 수**: `SageListBox.h` 20 + `SageListBox.cpp` 85 = 105
2. **역할**: owner-draw 목록 상자. 테마 · 테두리 제거 후 1px 비클라이언트 테두리(`OnNcPaint` cpp 33–43), 고정 행 높이(`MeasureItem` 45–47), `DrawItem`(55–85) 줄무늬 · 선택 강조 · 왼쪽 강조 막대 · 아래 격자선
3. **MFC 기반**: `CListBox`
4. **상태 · 변형**: selected / 보통, 짝·홀 행. 선택 `SAGE_COLOR_LIST_ROW_SELECTED = RGB(241, 227, 205)` + 강조 막대 `SAGE_COLOR_PRIMARY` 폭 `SAGE_LIST_SELECTION_ACCENT_WIDTH = 4`, 선택 글자 `SAGE_COLOR_PRIMARY_PRESS = RGB(118, 80, 42)` · `SAGE_FONT_CONTENT_SEMIBOLD`; 홀수 행 `SAGE_COLOR_LIST_ROW_ALT = RGB(250, 248, 244)`; 격자 `SAGE_COLOR_LIST_GRID = RGB(237, 232, 224)`; `SAGE_LIST_BOX_ROW_HEIGHT = 32`, `SAGE_LIST_BOX_TEXT_PAD_X = 12`
5. **사용처**: 0
6. **Qt 대응**: QListView (+QStyledItemDelegate)
7. **분류**: 필요 없음 (사용처 0)
8. **변형 후보**: —
9. **이관 주제**: —
10. **주의**: 없음(미사용)

### 13. SageListCtrl

1. **파일 · 줄 수**: `SageListCtrl.h` 75 + `SageListCtrl.cpp` 410 = 485
2. **역할**: 보고서형 목록의 행 · 셀 그리기 전부. `OnNMCustomDraw`(cpp 341–410) 단계별 처리:
   - `CDDS_ITEMPREPAINT`(350–372): 포커스 상태 제거, 선택 행 바탕 · 글자, 줄무늬
   - `CDDS_ITEMPOSTPAINT`(374–382): 행 구분선(`DrawRowSeparator` 200–207), 선택 강조 막대(`DrawSelectionAccent` 209–216)
   - `CDDS_SUBITEM | CDDS_ITEMPREPAINT`(384–408): 강조 열 글자색 · 굵은 글꼴, 특정 문자열 흐린 색, 배지 열(`DrawBadgeColumn` 250–298), 첫 열 정렬(`DrawFirstColumn` 309–339)
   - 고정 행 높이: 1px 폭 이미지 목록을 `LVSIL_SMALL`에 붙이는 방식(`ApplyFixedRowHeight` 78–84)
   - 체크 상자: 상태 이미지 목록을 직접 그려 만듦(`BuildCheckStateImages` 94–116, `SetCheckboxes` 156–177), 첫 열 · 배지 열에서는 직접 다시 그림(`DrawCheckBox` 86–92 → `SageUiStyle::DrawCheckBox`)
   - 선택 변경 시 해당 행만 무효화(`OnSelectionChanged` 14–21)
3. **MFC 기반**: `CListCtrl` (LVS_REPORT, `LVS_SINGLESEL` — `SageResultTablePanel.cpp` 99)
4. **상태 · 변형**:
   - 선택: 바탕 `SAGE_COLOR_LIST_ROW_SELECTED = RGB(241, 227, 205)`, 글자 `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, 강조 막대 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)` 폭 `SAGE_LIST_SELECTION_ACCENT_WIDTH = 4`
   - 줄무늬(`SetAlternateRowColor`): 홀수 행 `SAGE_COLOR_LIST_ROW_ALT = RGB(250, 248, 244)`, 짝수 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`
   - 행 구분선(`SetRowSeparator`): `SAGE_COLOR_LIST_GRID = RGB(237, 232, 224)`, `SAGE_LIST_GRID_THICKNESS = 1`
   - 첫 열 정렬 enum `SageListFirstColumnAlign`(h 3–8): DEFAULT / CENTER / RIGHT (RIGHT는 `SAGE_LIST_CELL_RIGHT_PAD = 6`). 사용: CENTER만(두 패널). RIGHT 사용처 0
   - 강조 열(`SetHighlightColumns`): 글자 `SAGE_COLOR_PRIMARY` + `SAGE_FONT_LIST_BOLD`, 나머지 `SAGE_FONT_LIST`
   - 빈 금액 표시 `SAGE_UI_AMOUNT_EMPTY_MARK = L"—"` → `SAGE_COLOR_TEXT_PLACEHOLDER = RGB(180, 171, 160)`
   - 흐린 문자열(`SetMutedText`): 실행 기록에서 `SAGE_UI_HISTORY_NO_OUTPUT = L"미리보기 (저장 없음)"` → `SAGE_COLOR_TEXT_PLACEHOLDER`(`SageWorkflowHistoryPanel.cpp` 58). 기본색 `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`
   - 행 상태 스타일(`SetRowStyle`, 행 상태 = `GetItemData` 값): 성공 배지 `SAGE_COLOR_BADGE_BG_SUCCESS = RGB(238, 244, 238)` / 글자 `SAGE_COLOR_STATUS_CARD_TEXT_SUCCESS = RGB(65, 96, 63)`; 실패 행 바탕 `SAGE_COLOR_STATUS_CARD_BG_ERROR = RGB(253, 246, 244)`, 배지 `SAGE_COLOR_STATUS_BG_ERROR = RGB(248, 235, 233)` / 글자 `SAGE_COLOR_INLINE_ERROR_TEXT = RGB(156, 68, 51)` (`SageWorkflowHistoryPanel.cpp` 129–140)
   - 배지 치수 `SAGE_LIST_BADGE_HEIGHT = 20`, `SAGE_LIST_BADGE_PAD_X = 8`, `SAGE_LIST_BADGE_RADIUS = 4`, 글꼴 `SAGE_FONT_CAPTION`
   - 체크: `SAGE_LIST_CHECK_IMAGE_WIDTH = 20`, `SAGE_LIST_CHECK_BOX_SIZE = 14`, `SAGE_LIST_CHECK_STATE_COUNT = 2`, `SAGE_LIST_CHECK_STATE_CHECKED = 1`, `SAGE_LIST_CHECK_ACCENT_GAP = 2`, 마스크 `SAGE_COLOR_IMAGE_MASK = RGB(255, 0, 255)`
   - 행 높이 `SAGE_LIST_ROW_HEIGHT = 34`, `SAGE_LIST_ROW_SPACER_WIDTH = 1`
   - 패널별 설정: 결과 표 — 줄무늬 · 첫 열 가운데 · 체크 · 구분선 · 강조 열은 워크플로 스타일에 따름(`SageResultTablePanel.cpp` 100–101, 281–283). 실행 기록 — 줄무늬 · 구분선 · 첫 열 가운데 · 배지 열 · 흐린 문자열 · 행 스타일(`SageWorkflowHistoryPanel.cpp` 54–58, 134–140)
5. **사용처**: 2 — `ui/panels/SageResultTablePanel.h`, `ui/panels/SageWorkflowHistoryPanel.h`
6. **Qt 대응**: QTableView + `Sage*Model`(QAbstractTableModel) + QSortFilterProxyModel + QHeaderView + QStyledItemDelegate
7. **분류**: delegate
8. **변형 후보**: 변형 enum 대신 model role — 정렬 `Qt::TextAlignmentRole`, 체크 `Qt::CheckStateRole`, 강조 · 흐림 · 빈 금액 · 행 상태는 model이 내는 role(구체 role 이름은 확인 못함)을 delegate가 읽음. 줄무늬는 `QAbstractItemView::alternatingRowColors` + `QPalette::AlternateBase`, 행 높이는 `verticalHeader()->setDefaultSectionSize`
9. **이관 주제**: T15 (결과 표), T16 (실행 기록)
10. **주의**:
   - 흐린 색을 **셀 문자열 비교**로 정한다(304, `strText == m_strMutedText`, `== SAGE_UI_AMOUNT_EMPTY_MARK`). Qt에서는 model이 의미로 알려야 하며 문자열을 바꾸면 조용히 깨짐
   - 행 상태를 `GetItemData` 정수 인덱스로 스타일 배열에서 찾음(69–76) — model role로 옮김
   - 포커스 사각형을 항상 지움(356) → 키보드 포커스 위치가 안 보임. Qt delegate가 `State_HasFocus`를 그리면 모양이 바뀜
   - 체크 상자는 원래 상태 이미지 자리가 아니라 `강조 막대 폭 + 간격` 오른쪽에 다시 그림(148–152). 클릭 판정은 네이티브 상태 이미지 영역 → 그린 위치와 클릭 영역이 다를 수 있음(실측 확인 못함). Qt는 delegate `editorEvent`로 판정이 그림과 일치
   - `DrawFirstColumn` · `DrawBadgeColumn`은 줄무늬 설정과 무관하게 `GetRowBackColor`(홀수 ALT)를 씀(256, 315) — 두 패널 모두 줄무늬를 켜서 현재 차이 없음
   - 첫 열만 정렬을 바꾸는 특수 규칙 — 다른 열은 LVCFMT 기본(실행 기록은 전 열 `LVCFMT_CENTER`, `SageWorkflowHistoryPanel.cpp` 124)
   - 배지는 텍스트가 비면 그리지 않음(270–271)

### 14. SageMessageBody

1. **파일 · 줄 수**: `SageMessageBody.h` 33 + `SageMessageBody.cpp` 120 = 153
2. **역할**: 메시지 상자 본문. `DrawItem`(cpp 88–120): 흰 바탕, 왼쪽 위 원형 아이콘(`DrawMessageIcon` 69–86 — 정보는 점+세로선 `DrawInfoGlyph` 60–67, 경고·오류는 세로선+점 `DrawAlertGlyph` 51–58), 오른쪽 여러 줄 텍스트 세로 가운데. `MeasureHeight`(20–35)는 줄바꿈 높이를 아이콘 크기 이상 · 200 이하로 제한
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**: enum `SageMessageIcon`(h 3–8): NONE, INFO, WARNING, ERROR. 아이콘 색: ERROR `SAGE_COLOR_ERROR = RGB(184, 92, 74)`, WARNING `SAGE_COLOR_WARNING = RGB(184, 135, 70)`, 그 외 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`. 글자 `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, 바탕 `SAGE_BG_PANEL`. 치수 `SAGE_MSGBOX_ICON_SIZE = 22`, `SAGE_MSGBOX_ICON_RADIUS = 10`, `SAGE_MSGBOX_ICON_TEXT_GAP = 12`, `SAGE_MSGBOX_ICON_DOT_SIZE = 2`, `SAGE_MSGBOX_ALERT_STEM_TOP = 6`, `SAGE_MSGBOX_ALERT_STEM_BOTTOM = 13`, `SAGE_MSGBOX_ALERT_DOT_TOP = 15`, `SAGE_MSGBOX_INFO_DOT_TOP = 5`, `SAGE_MSGBOX_INFO_STEM_TOP = 9`, `SAGE_MSGBOX_INFO_STEM_BOTTOM = 16`, `SAGE_MSGBOX_MAX_TEXT_HEIGHT = 200`, `SAGE_ICON_STROKE = 2`. 호출자 매핑(`SageMessageBoxDlg.cpp` 35–44): `MB_ICONERROR`→ERROR, `MB_ICONWARNING`·`MB_ICONQUESTION`→WARNING, `MB_ICONINFORMATION`→INFO
5. **사용처**: 1 — `ui/dialogs/SageMessageBoxDlg.h`
6. **Qt 대응**: QLabel ×2 (아이콘 pixmap · 줄바꿈 본문). 아이콘 모양은 `SageStyle::standardIcon(SP_MessageBoxInformation / SP_MessageBoxWarning / SP_MessageBoxCritical)` 재정의 후보
7. **분류**: 기본 위젯 + SageStyle (아래 확인 못함 참고 — 커스텀 위젯으로 갈 수도 있음)
8. **변형 후보**: 없음 (`QStyle::StandardPixmap`으로 표현)
9. **이관 주제**: T09
10. **주의**:
   - 본문 높이를 200px로 자르고 `DT_END_ELLIPSIS`(118) — 긴 메시지는 잘린다. QLabel은 자르지 않음 → 다이얼로그가 커짐
   - 질문(`MB_ICONQUESTION`)이 경고 아이콘으로 합쳐짐 — 옮길 때 유지 여부 확인
   - 텍스트 세로 가운데 계산(111–116)은 아이콘보다 짧은 한 줄 메시지를 아이콘 중심에 맞추는 효과

### 15. SageOptionCheck

1. **파일 · 줄 수**: `SageOptionCheck.h` 28 + `SageOptionCheck.cpp` 105 = 133
2. **역할**: owner-draw 체크 버튼. 테두리(선택) + 체크 상자(`SageUiStyle::DrawCheckBox`) + 라벨 + 보조 힌트(`DrawItem` cpp 65–105). 클릭 시 자체 토글(`OnClicked` 60–63)
3. **MFC 기반**: `CButton` (owner-draw)
4. **상태 · 변형**: checked / 보통, disabled(라벨 `SAGE_COLOR_TEXT_PLACEHOLDER = RGB(180, 171, 160)`), 테두리 표시 여부(`SAGE_COLOR_BUTTON_BORDER = RGB(201, 191, 177)`, `SAGE_OPTION_CHECK_PADDING = 8`), 힌트 `SAGE_COLOR_SECONDARY_TEXT`
5. **사용처**: 0 (`SageResultTablePanel.h` 11에서 헤더를 include하지만 클래스는 쓰지 않음)
6. **Qt 대응**: QCheckBox
7. **분류**: 필요 없음 (사용처 0)
8. **변형 후보**: —
9. **이관 주제**: —
10. **주의**: `SageResultTablePanel.h`의 불필요한 include는 SageSDI 쪽 사용하지 않는 코드(수정 대상 아님)

### 16. SageSearchBox

1. **파일 · 줄 수**: `SageSearchBox.h` 48 + `SageSearchBox.cpp` 248 = 296
2. **역할**: 결과 표 검색 상자. 한 테두리 안에 [검색 기준 콤보 | 입력칸 | 돋보기 칸]. `CreateBox`(cpp 17–29) 입력칸 생성, `CreateCriteriaCell`(31–47) 기준 콤보(`CSageFilterComboBox`) 생성, `DrawItem`(225–248) 바탕 · 기준 칸 · 돋보기 칸 · 바깥 테두리, `OnLButtonDown`(201–213) 돋보기 칸 클릭 → 부모에 검색 명령, 그 밖 클릭 → 입력칸 포커스, `OnCommand`(87–95) 자식 알림을 부모로 전달, `OnCtlColor`(215–223) 입력칸 색, `LayoutEdit`(175–193) 입력칸 높이를 글꼴 높이로 세로 가운데
3. **MFC 기반**: `CStatic` (owner-draw) + 자식 `CEdit` · `CSageFilterComboBox`
4. **상태 · 변형**: 기준 칸 있음/없음(`CreateCriteriaCell` 호출 여부). 색: 바탕 `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, 기준 · 돋보기 칸 `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`, 칸 경계 `SAGE_COLOR_LIST_HEADER_BORDER = RGB(228, 223, 215)`, 돋보기 `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`, 바깥 테두리 `SAGE_COLOR_BUTTON_BORDER = RGB(201, 191, 177)`, 입력 글자 `SAGE_COLOR_TEXT`. 치수 `SAGE_SEARCH_ICON_CELL_WIDTH = 32`, `SAGE_SEARCH_CRITERIA_CELL_WIDTH = 92`, `SAGE_EDIT_BORDER_WIDTH = 1`, `SAGE_EDIT_TEXT_LEFT_PAD = 10`. focus · hover · disabled 모양 없음. 호출 설정: 자리표시 `SAGE_UI_RESULT_FILTER_PLACEHOLDER = L"검색어 입력"`, 최대 길이 `SAGE_RESULT_FILTER_MAX_LENGTH = 20`, 드롭 행 `SAGE_RESULT_CRITERIA_DROP_ROWS = 8`
5. **사용처**: 1 — `ui/panels/SageResultTablePanel.h`
6. **Qt 대응**: QComboBox + QLineEdit(`setPlaceholderText`, `setMaxLength`, `addAction(..., TrailingPosition)`로 돋보기, `returnPressed`) 조합
7. **분류**: 커스텀 위젯 (기본 위젯 두 개를 한 테두리로 묶는 합성 위젯)
8. **변형 후보**: 없음
9. **이관 주제**: T15
10. **주의**:
   - Enter 검색은 이 위젯이 아니라 부모 패널의 `PreTranslateMessage`가 `IsEditMessage`로 가로챈다(`SageResultTablePanel.cpp` 56–62). Qt에서는 위젯이 `returnPressed`를 받아 의미 있는 signal(`searchRequested`)로 내야 함
   - `OnCommand`가 자식의 모든 알림(EN_CHANGE, CBN_SELCHANGE 등)을 원래 ID 그대로 부모에 중계 — Qt 규칙상 중계 slot 금지, 의미 있는 signal로 바꿈
   - 테두리 아무 곳 클릭 → 입력칸 포커스(206–208), 돋보기는 히트 테스트(`PtInRect`) — Qt `QAction`이면 키보드로도 누를 수 있게 됨
   - 입력칸 높이를 글꼴 `tmHeight`에 맞춰 수동 배치 — Qt 레이아웃으로 바뀌면 세로 위치가 달라질 수 있음
   - 한 테두리로 묶은 모양을 SageStyle만으로 낼 수 있는지(자식 프레임 끄기 등)는 확인 못함

### 17. SageSectionLabel

1. **파일 · 줄 수**: `SageSectionLabel.h` 11 + `SageSectionLabel.cpp` 43 = 54
2. **역할**: 카드 · 구역 머리 띠. `DrawItem`(cpp 12–43): 바탕 `SAGE_COLOR_LIST_HEADER`, 아래 1px `SAGE_COLOR_BORDER`, 창 텍스트를 왼쪽 패딩 16으로, 선택적 오른쪽 힌트(`SAGE_FONT_CAPTION`, `SAGE_COLOR_SECONDARY_TEXT`)
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**: 힌트 있음/없음 — `SetHintText` 사용처 0. 색 `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`, `SAGE_COLOR_BORDER = RGB(220, 214, 205)`, `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`, 패딩 `SAGE_CARD_PADDING = 16`. 글꼴은 위젯 글꼴(`GetFont`) — 입력 카드 머리 `SAGE_FONT_HEADER`(`SageWorkflowInputPanel.cpp` 94), 결과 제목 `SAGE_FONT_CONTENT`(`SageResultTablePanel.cpp` 110)
5. **사용처**: 2 — `ui/panels/SageResultTablePanel.h`, `ui/panels/SageWorkflowInputPanel.h`
6. **Qt 대응**: QLabel
7. **분류**: 기본 위젯 + SageStyle (띠 바탕 · 아래선을 SageStyle이 QFrame 그리기 단계에서 낼 수 있다는 전제 — 확인 못함)
8. **변형 후보**: `SageLabelVariant`에 `Section` 추가 (두 사용처의 글꼴 차이를 하나로 볼지 `SectionLarge`/`Section`으로 나눌지 확인 못함)
9. **이관 주제**: T13 (입력 카드 머리), T15 (결과 제목)
10. **주의**:
   - 힌트 경로는 미사용
   - 텍스트 한 줄 · 말줄임 없음(31) — 좁아지면 잘림 방식이 Qt와 다름

### 18. SageSelectionBar

1. **파일 · 줄 수**: `SageSelectionBar.h` 43 + `SageSelectionBar.cpp` 174 = 217
2. **역할**: 결과 표 위 "[전체 선택] N건 중 M건 선택됨 [선택 해제]". `OnCreate`(cpp 21–35) 네이티브 자동 체크 상자 + Ghost 버튼 생성, `DrawItem`(156–174) 개수 문구를 세 조각으로 직접 그림, `LayoutChildren`(125–140) 글자 폭 측정으로 자식 좌표 지정, `GetContentWidth`(117–123)로 부모가 폭 결정, 클릭은 부모에 `WM_COMMAND` 중계(63–73)
3. **MFC 기반**: `CStatic` (owner-draw) + 자식 `CButton`(BS_AUTOCHECKBOX, 네이티브) · `CSageButton`
4. **상태 · 변형**: 전체 체크 on/off(`SetAllChecked`), 활성/비활성(`EnableControls`). 문구 색: "N건 중" · "선택됨" `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)` + `SAGE_FONT_CONTENT`, "M건" `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)` + `SAGE_FONT_CONTENT_SEMIBOLD`. 바탕 `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`, 체크 상자 글자 `SAGE_COLOR_TEXT`(`OnCtlColor` 75–80). 문자열 `SAGE_UI_SELECT_ALL_BUTTON = L"전체 선택"`, `SAGE_UI_SELECTION_CLEAR_BUTTON = L"선택 해제"`, `SAGE_UI_SELECTION_TOTAL_FORMAT = L"%d건 중"`, `SAGE_UI_SELECTION_SELECTED_FORMAT = L"%d건"`, `SAGE_UI_SELECTION_SUFFIX = L"선택됨"`. 치수 `SAGE_SELECTION_BAR_GAP = 12`, `SAGE_SELECTION_CHECK_GLYPH_WIDTH = 20`, `SAGE_SELECTION_CLEAR_PAD = 12`, `SAGE_ICON_TEXT_GAP = 6`. ID `ID_SAGE_SELECTION_CHECK = 41076`, `ID_SAGE_SELECTION_CLEAR = 41077`
5. **사용처**: 1 — `ui/panels/SageResultTablePanel.h`
6. **Qt 대응**: QCheckBox + QLabel(조각별) + QPushButton(Ghost) 을 레이아웃으로 조합
7. **분류**: 기본 위젯 + SageStyle (모든 요소가 기본 위젯. 별도 위젯 클래스로 둘지, `SageResultTablePanel`에 직접 둘지는 T15 설계 결정)
8. **변형 후보**: 없음 — 자식이 `SageButtonVariant::Ghost`, `SageLabelVariant`(세 조각 색 · 굵기용 값 추가 필요할 수 있음)를 씀
9. **이관 주제**: T15
10. **주의**:
   - 체크 상자가 네이티브 Windows 모양(owner-draw 아님)이라 목록 안 체크(`SageUiStyle::DrawCheckBox`)와 모양이 다르다. Qt에서는 SageStyle 하나로 통일되어 **모양이 바뀜**(의도에 맞는지 확인)
   - 부분 선택(tristate) 없음 — 전체일 때만 체크(`SageResultTablePanel.cpp` 620)
   - `BS_AUTOCHECKBOX`는 클릭 즉시 스스로 토글한 뒤 부모가 처리 → Qt `QCheckBox::clicked`와 순서 동일한지 확인 필요
   - 폭을 글자 측정으로 계산해 부모가 `MoveWindow`(`SageResultTablePanel.cpp` 142–145) — Qt는 `sizeHint`/레이아웃

### 19. SageSidebarTree

1. **파일 · 줄 수**: `SageSidebarTree.h` 12 + `SageSidebarTree.cpp` 68 = 80
2. **역할**: 사이드바 업무 트리의 행 그리기. `OnNMCustomDraw`(cpp 53–68) 모든 항목을 `DrawTreeItem`(10–51)으로 직접 그리고 `CDRF_SKIPDEFAULT`. 최상위 = 그룹 머리, 그 아래 = 업무 항목. 행 폭을 클라이언트 전체로 넓힘(20–21)
3. **MFC 기반**: `CTreeCtrl`
4. **상태 · 변형**:
   - 그룹 머리: 글꼴 `SAGE_FONT_CAPTION`, 글자 `SAGE_COLOR_SIDEBAR_CATEGORY = RGB(130, 120, 108)`, 자간 `SAGE_SIDEBAR_CATEGORY_CHAR_EXTRA = 1`, 선택돼도 강조 안 함
   - 항목 보통: `SAGE_FONT_CONTROL`, `SAGE_COLOR_SIDEBAR_TEXT = RGB(205, 196, 185)`, 바탕 `SAGE_COLOR_SIDEBAR = RGB(36, 31, 26)`
   - 항목 선택: `SAGE_FONT_CONTENT_SEMIBOLD`, `SAGE_COLOR_SIDEBAR_SELECTED_TEXT = RGB(255, 255, 255)`, 바탕 `SAGE_COLOR_SIDEBAR_SELECTED = RGB(58, 49, 41)`, 왼쪽 막대 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)` 폭 `SAGE_SELECTION_ACCENT_WIDTH = 3`
   - 왼쪽 패딩 `SAGE_SIDEBAR_PAD_X = 20` (그룹 · 항목 동일)
   - hover · focus 없음
5. **사용처**: 1 — `ui/panels/SageSidebarPanel.h`
6. **Qt 대응**: QTreeView (+model) + QStyledItemDelegate
7. **분류**: delegate
8. **변형 후보**: (delegate) — 그룹 머리 여부는 model에서(부모 없음) 판정
9. **이관 주제**: T11
10. **주의**:
   - 펼침 표시(+/−, 화살표) · 들여쓰기 · 연결선을 그리지 않는다. 그룹과 항목의 텍스트 시작 위치가 같다 → QTreeView 기본 들여쓰기 · branch 표시를 꺼야 같은 모양
   - 그룹 머리도 선택 가능한 항목이라 키보드 이동 시 그룹에 선택이 머무를 수 있음(강조 없이) — Qt에서 그룹을 `ItemIsSelectable` 없이 둘지 확인
   - 자간은 `SetTextCharacterExtra`(GDI) → Qt `QFont::setLetterSpacing(QFont::AbsoluteSpacing, 1)` 후보
   - 행 높이 `SAGE_SIDEBAR_ITEM_HEIGHT = 34`는 이 파일이 아니라 패널에서 설정(설정 위치는 확인 안 함)

### 20. SageStatusCard

1. **파일 · 줄 수**: `SageStatusCard.h` 53 + `SageStatusCard.cpp` 300 = 353
2. **역할**: 실행 상태 카드. `DrawItem`(cpp 290–300) → `DrawCardSurface`(146–150) + 대기·실행 중이면 `DrawPendingContent`(208–250: 상태 점 · 메시지 · 진행률 % · 진행 막대), 완료·실패면 `DrawResultContent`(252–288: 원형 체크/느낌표 아이콘 · 메시지 · 세부 문구). 완료 + 세부 문구가 있으면 Secondary "폴더 열기" 버튼 표시(`LayoutOpenFolderButton` 91–108), 클릭은 부모에 `WM_COMMAND`(39–43)
3. **MFC 기반**: `CStatic` (owner-draw) + 자식 `CSageButton`
4. **상태 · 변형**: enum `SageStatusCardState`(h 5–11): IDLE, RUNNING, COMPLETED, FAILED
   - 바탕: COMPLETED `SAGE_COLOR_STATUS_CARD_BG_SUCCESS = RGB(241, 245, 240)`, FAILED `SAGE_COLOR_STATUS_CARD_BG_ERROR = RGB(253, 246, 244)`, 그 외 `SAGE_COLOR_PANEL`
   - 테두리: COMPLETED `SAGE_COLOR_STATUS_CARD_BORDER_SUCCESS = RGB(213, 224, 211)`, FAILED `SAGE_COLOR_DANGER_BORDER = RGB(224, 189, 182)`, 그 외 `SAGE_COLOR_BORDER`
   - 강조(점 · 아이콘): RUNNING `SAGE_COLOR_WARNING = RGB(184, 135, 70)`, COMPLETED `SAGE_COLOR_SUCCESS = RGB(95, 127, 95)`, FAILED `SAGE_COLOR_ERROR = RGB(184, 92, 74)`, IDLE `SAGE_COLOR_TEXT_PLACEHOLDER = RGB(180, 171, 160)`
   - 메시지 글자: IDLE `SAGE_COLOR_SECONDARY_TEXT`, COMPLETED `SAGE_COLOR_STATUS_CARD_TEXT_SUCCESS = RGB(65, 96, 63)`, FAILED `SAGE_COLOR_INLINE_ERROR_TEXT = RGB(156, 68, 51)`, RUNNING `SAGE_COLOR_TEXT`; 세부 문구 `SAGE_COLOR_TEXT_MUTED` + `SAGE_FONT_CAPTION`; 진행률 % `SAGE_COLOR_PRIMARY`
   - 진행 막대: 바탕 `SAGE_COLOR_LIST_GRID = RGB(237, 232, 224)`, 채움 `SAGE_COLOR_PRIMARY`, 높이 `SAGE_STATUS_CARD_PROGRESS_HEIGHT = 6`, 100% 기준 `SAGE_PROGRESS_COMPLETE = 100`, 형식 `SAGE_UI_PROGRESS_FORMAT = L"%d%%"`, 글자 폭 `SAGE_PROGRESS_TEXT_WIDTH = 54`
   - 치수: `SAGE_STATUS_CARD_DOT_SIZE = 8`, `SAGE_STATUS_CARD_DOT_GAP = 8`, `SAGE_STATUS_CARD_ICON_SIZE = 18`, `SAGE_STATUS_CARD_ICON_GAP = 12`, `SAGE_STATUS_CARD_ICON_RADIUS = 7`, `SAGE_STATUS_CARD_ICON_THICKNESS = 2`, 체크 좌표 `SAGE_STATUS_CARD_CHECK_START_X = -3` / `_START_Y = 0` / `_MID_X = -1` / `_MID_Y = 3` / `_END_X = 3` / `_END_Y = -2`, 느낌표 `SAGE_STATUS_CARD_ALERT_STEM_TOP = -4` / `_STEM_BOTTOM = 1` / `_DOT_TOP = 3` / `_DOT_SIZE = 2`, `SAGE_STATUS_CARD_TITLE_LINE_HEIGHT = 20`, `SAGE_STATUS_CARD_DETAIL_LINE_HEIGHT = 16`, `SAGE_STATUS_CARD_ACTION_WIDTH = 80`, `SAGE_STATUS_CARD_ACTION_GAP = 12`, `SAGE_STATUS_CARD_ACTION_AREA_WIDTH = SAGE_STATUS_CARD_ACTION_WIDTH + SAGE_STATUS_CARD_ACTION_GAP`, `SAGE_CARD_PADDING = 16`, `SAGE_CARD_ROW_GAP = 12`, `SAGE_BUTTON_HEIGHT = 32`, `SAGE_STATUS_CARD_HEIGHT = 70`(호출자), 문구 `SAGE_UI_STATUS_CARD_OPEN_FOLDER = L"폴더 열기"`, `ID_STATUS_CARD_OPEN_FOLDER = 41090`
5. **사용처**: 1 — `ui/panels/SageWorkflowInputPanel.h` (`SetIdle` / `SetRunning` / `SetResult` / `SetProgressPercent`, `SageWorkflowInputPanel.cpp` 84, 315, 360–369)
6. **Qt 대응**: 없음 (진행 막대만 QProgressBar로 둘 수 있으나 카드 전체는 없음)
7. **분류**: 커스텀 위젯
8. **변형 후보**: `SageStatusCardVariant { Idle, Running, Completed, Failed }` (실제로는 상태 — 이름 규칙을 지시문에 맞춤)
9. **이관 주제**: T14
10. **주의**:
   - 완료 세부 문구(출력 경로)는 `DT_PATH_ELLIPSIS`(경로 중간 생략, 284), 실패는 끝 생략. Qt `QFontMetrics::elidedText`의 `Qt::ElideMiddle`은 경로 구분자 기준이 아니어서 결과가 다름
   - `SetProgressPercent`는 RUNNING일 때만 반영(68) — 상태 전이 순서 계약
   - 느낌표 점은 중심에서 오른쪽으로 치우침(191–193, `- SIZE/2` 보정 없음)
   - 폴더 열기 버튼 표시 조건 = COMPLETED + 세부 문구 비어 있지 않음(95)
   - 메시지 · 진행률 폭을 고정값(`SAGE_PROGRESS_TEXT_WIDTH`)으로 뺌 — 글꼴이 OS마다 달라지면 "100%"가 54px 안에 들어가는지 확인 필요

### 21. SageSummaryBar

1. **파일 · 줄 수**: `SageSummaryBar.h` 64 + `SageSummaryBar.cpp` 166 = 230
2. **역할**: 결과 표 위 요약 한 줄 "라벨 값 단위 | 라벨 값 단위 …". `DrawItem`(cpp 129–166) 항목을 세로 구분선(`DrawDivider` 124–127)으로 나눠 조각별로 그림. 배지 항목은 그리지 않고 자리만 비운 뒤 자식 `CSageBadge`를 그 위치에 `MoveWindow`(`ApplyBadgeItem` 39–59, `LayoutBadge` 61–81)
3. **MFC 기반**: `CStatic` (owner-draw) + 자식 `CSageBadge`
4. **상태 · 변형**: 항목 단위 — 강조(`bHighlight`: 값 `SAGE_COLOR_PRIMARY`, 아니면 `SAGE_COLOR_TEXT`), 배지(색 3개가 `CLR_NONE`이 아니면). 라벨 · 단위 `SAGE_FONT_CAPTION` + `SAGE_COLOR_SECONDARY_TEXT`, 값 `SAGE_FONT_SUMMARY`. 바탕 `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`, 구분선 `SAGE_COLOR_BORDER` 높이 `SAGE_SUMMARY_DIVIDER_HEIGHT = 16`. 간격 `SAGE_SUMMARY_ITEM_GAP = 16`, `SAGE_SUMMARY_TEXT_GAP = 6`, 배지 `SAGE_BADGE_RADIUS = 4`, `SAGE_BADGE_PAD_X = 8`, 배지 문구 `SAGE_UI_SUMMARY_BADGE_FORMAT = L"%s %s%s"`, 높이 `SAGE_SUMMARY_BAR_HEIGHT = 32`(호출자), `ID_SAGE_SUMMARY_BADGE = 41099`
5. **사용처**: 1 — `ui/panels/SageResultTablePanel.h` (배지 색은 경고 계열 고정, `SageResultTablePanel.cpp` 411–415)
6. **Qt 대응**: 없음
7. **분류**: 커스텀 위젯
8. **변형 후보**: 없음 (항목 구조체에 강조 · 배지 여부)
9. **이관 주제**: T15
10. **주의**:
   - 폭이 모자라면 그 항목부터 **말없이 버림**(146–147) — Qt 레이아웃으로 바꾸면 잘림/말줄임으로 동작이 바뀜
   - 배지는 **첫 번째** 배지 항목 하나만(`FindBadgeItemIndex` 22–28). 두 번째 배지 항목은 자리만 비고 아무것도 안 그려짐
   - 배지 문구에서 값과 단위 사이 공백 없음(`%s %s%s`)
   - 배지를 자식 창으로 겹쳐 놓는 방식 → Qt에서는 한 위젯 안에서 그리거나 `SageBadge`를 레이아웃에 넣음

### 22. SageTabCtrl

1. **파일 · 줄 수**: `SageTabCtrl.h` 11 + `SageTabCtrl.cpp` 63 = 74
2. **역할**: 작업 영역 탭. `OnPaint`(cpp 21–63) 흰 바탕 + 아래 1px 선, 선택 탭 아래 2px 표시줄, 선택 탭 굵은 글꼴. `ApplyTabHeight`(10–19) 첫 탭 높이가 `SAGE_TAB_HEIGHT`보다 작으면 `SetItemSize`
3. **MFC 기반**: `CTabCtrl`
4. **상태 · 변형**: selected / 보통. 선택: 표시줄 `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)` 높이 `SAGE_TAB_INDICATOR_HEIGHT = 2`, `SAGE_FONT_CONTENT_SEMIBOLD`, `SAGE_COLOR_TEXT = RGB(47, 42, 36)`. 보통: `SAGE_FONT_CONTENT`, `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`. 바탕 `SAGE_COLOR_PANEL`, 아래선 `SAGE_COLOR_BORDER`, `SAGE_TAB_HEIGHT = 40`. hover · disabled · focus 없음
5. **사용처**: 1 — `ui/panels/SageWorkspacePanel.h` (`ApplyTabHeight` — `SageWorkspacePanel.cpp` 119)
6. **Qt 대응**: QTabBar (+ 패널 전환은 QStackedWidget — `ui-composition.md`)
7. **분류**: 기본 위젯 + SageStyle (`CE_TabBarTabShape` / `CE_TabBarTabLabel`, `PE_FrameTabBarBase`)
8. **변형 후보**: 없음
9. **이관 주제**: T13
10. **주의**:
   - 패널이 `SetFont(SAGE_FONT_CONTENT)`를 해도(`SageWorkspacePanel.cpp` 41) `OnPaint`가 자기 글꼴을 고른다 — 선택 탭만 SemiBold. Qt에서 선택 탭 글꼴만 바꾸면 탭 폭 계산(`sizeFromContents`)과 어긋날 수 있음
   - 탭 폭은 네이티브 계산 그대로 — Qt는 `tabSizeHint`/스타일로 바뀌어 폭이 달라짐
   - QTabBar는 방향키 · Ctrl+Tab 전환, 넘칠 때 스크롤 버튼이 있다(MFC 쪽 키 동작은 확인 못함)
   - 텍스트 버퍼 63자(53)

### 23. SageTableTotalBar

1. **파일 · 줄 수**: `SageTableTotalBar.h` 43 + `SageTableTotalBar.cpp` 57 = 100
2. **역할**: 결과 표 아래 합계 줄. `DrawItem`(cpp 24–57) 바탕 `SAGE_COLOR_LIST_HEADER`, 위 1px `SAGE_COLOR_BORDER`, 호출자가 준 셀 목록(`nLeft` · `nWidth` · 정렬 · 종류)을 그 좌표에 그림
3. **MFC 기반**: `CStatic` (owner-draw)
4. **상태 · 변형**: 셀 종류 enum `SageTotalBarCellStyle`(h 6–12): LABEL(`SAGE_COLOR_TEXT_MUTED`), COUNT(`SAGE_COLOR_SECONDARY_TEXT`, `SAGE_FONT_LIST_SEMIBOLD`), AMOUNT(`SAGE_COLOR_TEXT`), AMOUNT_HIGHLIGHT(`SAGE_COLOR_PRIMARY`); COUNT 외 글꼴 `SAGE_FONT_LIST_BOLD`. 정렬 `SAGE_COLUMN_ALIGN_LEFT/CENTER/RIGHT`(core `SageColumnAlign`), 패딩 `SAGE_LIST_CELL_LEFT_PAD = 6`, `SAGE_LIST_CELL_RIGHT_PAD = 6`, 높이 `SAGE_TOTAL_BAR_HEIGHT = 40`(호출자). 색 값 `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`, `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`, `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`, `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`
5. **사용처**: 1 — `ui/panels/SageResultTablePanel.h` (셀 종류 변환 `SageResultTablePanel.cpp` 41–48, `SetCells` 354)
6. **Qt 대응**: 없음 (QTableView에 바닥 합계 행이 없음)
7. **분류**: 커스텀 위젯
8. **변형 후보**: 위젯 변형 없음. 셀 종류 enum(`Label, Count, Amount, AmountHighlight`)은 데이터 쪽
9. **이관 주제**: T15
10. **주의**:
   - 셀 x 좌표를 패널이 열 폭으로 계산해 넘김 — Qt에서는 `QHeaderView::sectionViewportPosition` · `sectionResized` · 가로 스크롤에 맞춰야 하며, 안 맞추면 스크롤 시 합계가 열과 어긋남
   - 헤더가 core 헤더(`app/core/workflow/SageWorkflowResultTable.h`)를 include — ui → core 의존(SageQt 계층 규칙과 맞는지 T15에서 확인)
   - 한 줄, 말줄임 없음

### 24. SageUiResources

1. **파일 · 줄 수**: `SageUiResources.h` 47 + `SageUiResources.cpp` 122 = 169
2. **역할**: 앱 전역 GDI 자원 보관. 글꼴 11개(`Create` cpp 60–75), 바탕 브러시 9개, 바탕색 · 글자색 표(21–46). `GetFont` / `GetBrush` / `GetBackgroundColor` / `GetTextColor`
3. **MFC 기반**: 없음 (namespace + 전역 `CFont` · `CBrush`)
4. **상태 · 변형**:
   - `SageFontRole`(h 3–15): CONTROL(`SAGE_CONTROL_FONT_POINT_SIZE = 105`, `SAGE_CONTROL_FONT_FACE = L"Pretendard"`), CONTENT(`SAGE_CONTENT_FONT_POINT_SIZE = 105`, Pretendard), CONTENT_SEMIBOLD(105, `SAGE_TITLE_FONT_FACE = L"Pretendard SemiBold"`), TITLE(`SAGE_TITLE_FONT_POINT_SIZE = 143`, SemiBold), HEADER(`SAGE_HEADER_FONT_POINT_SIZE = 113`, SemiBold), CAPTION(`SAGE_CAPTION_FONT_POINT_SIZE = 90`, Pretendard), SUMMARY(`SAGE_SUMMARY_FONT_POINT_SIZE = 128`, SemiBold), LIST(`SAGE_LIST_FONT_POINT_SIZE = 98`, Pretendard), LIST_SEMIBOLD(98, SemiBold), LIST_BOLD(LIST + `FW_BOLD`, 48–54), LOGO(143, `SAGE_LOGO_FONT_FACE = L"Gmarket Sans TTF Bold"`)
   - `SageBackgroundRole`(h 28–38) → `SAGE_COLOR_APP_BACKGROUND = RGB(248, 246, 241)`, `SAGE_COLOR_PANEL = RGB(255, 255, 255)`, `SAGE_COLOR_SIDEBAR = RGB(36, 31, 26)`, `SAGE_COLOR_STATUS_BG_SUCCESS = RGB(234, 244, 234)`, `SAGE_COLOR_STATUS_BG_WARNING = RGB(251, 245, 238)`, `SAGE_COLOR_STATUS_BG_ERROR = RGB(248, 235, 233)`, `SAGE_COLOR_ACCENT_SURFACE = RGB(247, 242, 234)`, `SAGE_COLOR_LIST_HEADER = RGB(242, 238, 231)`, `SAGE_COLOR_LIST_GRID = RGB(237, 232, 224)`
   - `SageTextRole`(h 17–26) → `SAGE_COLOR_TEXT = RGB(47, 42, 36)`, `SAGE_COLOR_SECONDARY_TEXT = RGB(122, 112, 100)`, `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)`, `SAGE_COLOR_SUCCESS = RGB(95, 127, 95)`, `SAGE_COLOR_ERROR = RGB(184, 92, 74)`, `SAGE_COLOR_SIDEBAR_TEXT = RGB(205, 196, 185)`, `SAGE_COLOR_SIDEBAR_CATEGORY = RGB(130, 120, 108)`, `SAGE_COLOR_TEXT_MUTED = RGB(110, 101, 91)`
5. **사용처**: 9 (`SageUiResources::` 호출) — `ui/dialogs/SageMessageBoxDlg.cpp`, `ui/dialogs/SagePasswordChangeDlg.cpp`, `ui/panels/SageHeaderPanel.cpp`, `ui/panels/SageResultTablePanel.cpp`, `ui/panels/SageSidebarPanel.cpp`, `ui/panels/SageWorkflowHistoryPanel.cpp`, `ui/panels/SageWorkflowInputPanel.cpp`, `ui/panels/SageWorkspacePanel.cpp`, `ui/view/SageSDIView.cpp`. `app/` 밖: `SageSDI.cpp`(69 `Create()`, 120 `Destroy()`). 역할 enum만 쓰는 파일(`SageLoginDlg.cpp` 등)은 이 수에 없음
6. **Qt 대응**: 앱 폰트(`QApplication::setFont`) + 글꼴 역할 표 + `QPalette` + 디자인 값 헤더(`SageDesignDefine.h`)
7. **분류**: 기타 — 색은 `SageDesignDefine.h`(디자인 값), 글꼴 역할은 SageStyle이 쓰는 글꼴 표 + 앱 폰트 등록, 브러시는 불필요
8. **변형 후보**: —
9. **이관 주제**: T08
10. **주의**:
   - `CreatePointFont`의 크기 단위는 1/10 pt(105 = 10.5 pt). Qt `QFont::setPointSizeF`로 옮길 때 10으로 나눠야 함. 또 Windows(96 DPI 기준)와 macOS(72 기준)에서 같은 pt가 다른 픽셀 크기가 된다 — 화면 판정 필요
   - SemiBold를 **별도 글꼴 이름**("Pretendard SemiBold")으로 고른다. Qt에서는 family "Pretendard" + `QFont::DemiBold`가 일반적이며, 등록된 폰트 파일의 family/style 이름이 OS별로 어떻게 잡히는지 확인 못함
   - LIST_BOLD는 LIST에 `FW_BOLD`를 인위 적용(합성 굵게 가능성)
   - `default:`가 CONTROL로 떨어진다(106) — 역할 추가 시 조용히 CONTROL이 됨
   - 역할 → 색이 **배열 인덱스**로 연결(21–46) — enum 순서를 바꾸면 조용히 색이 섞임

### 25. SageUiStyle

1. **파일 · 줄 수**: `SageUiStyle.h` 7 + `SageUiStyle.cpp` 61 = 68
2. **역할**: 여러 컨트롤이 공유하는 그리기 조각. `DrawComboArrow`(cpp 7–22) 채운 삼각형, `DrawCheckBox`(24–40) 체크 상자, `DrawSearchIcon`(42–59) 돋보기
3. **MFC 기반**: 없음 (namespace 자유 함수)
4. **상태 · 변형**:
   - 콤보 화살표: `SAGE_COLOR_PRIMARY = RGB(154, 107, 63)` 채움, 꼭짓점 오프셋 ±4 / −2 / +3이 **코드 안 숫자**(10–14, SageDefine 상수 아님)
   - 체크 상자: 해제 = `SAGE_COLOR_PANEL = RGB(255, 255, 255)` 채움 + `SAGE_COLOR_BUTTON_BORDER = RGB(201, 191, 177)` 테두리; 체크 = `SAGE_COLOR_PRIMARY` 채움 + `SAGE_COLOR_PANEL` 체크 선 두께 `SAGE_LIST_CHECK_MARK_THICKNESS = 2`. disabled · 부분 체크 없음
   - 돋보기: `SAGE_ICON_STROKE = 2`, `SAGE_ICON_SEARCH_RADIUS = 5`, `SAGE_ICON_SEARCH_HANDLE = 4`
5. **사용처**: 0 (drawing 밖). drawing 내부 6 — `SageButton.cpp`, `SageComboBox.cpp`, `SageFilterComboBox.cpp`, `SageListCtrl.cpp`, `SageOptionCheck.cpp`, `SageSearchBox.cpp`
6. **Qt 대응**: `SageStyle`의 primitive 재정의(`PE_IndicatorArrowDown`, `PE_IndicatorCheckBox`, `PE_IndicatorItemViewItemCheck`) + 돋보기는 `QIcon`(검색 칸 · 필요 시 버튼)
7. **분류**: 기타 — `style.md`의 "여러 위젯이 공유하는 그리기 조각은 `ui/style/`" 자리에 해당. SageStyle · delegate · 커스텀 위젯만 호출
8. **변형 후보**: —
9. **이관 주제**: T08
10. **주의**:
   - 화살표 좌표의 숫자(±4, −2, +3)가 상수 없이 박혀 있다 — SageQt 하드코딩 금지 규칙상 디자인 값으로 올려야 함
   - 체크 상자 disabled 모양이 없음 → Qt에서 비활성 체크 상자 모양을 새로 정해야 함
   - GDI 펜 선은 반픽셀 보정 없이 정수 좌표로 그림 — QPainter 안티에일리어싱을 켜면 선이 번져 보일 수 있음

---

## 확인 못함

1. **SageDialogCaptionBar 닫기 버튼 바탕**: `SetSurfaceColor` 호출이 없어 Ghost 기본 바탕(`SAGE_COLOR_PANEL`, 흰색)이 캡션 띠(`SAGE_COLOR_LIST_HEADER`) 위에 보인다고 코드상 읽힘. 실제 화면 · 의도 확인 못함
2. **프레임리스 창 끌기 이동**(T09): Win32 `HTTRANSPARENT`/`HTCAPTION` 조합을 Qt(`startSystemMove`)로 바꿨을 때 macOS · Linux(Wayland) 동작 확인 못함
3. **SageListCtrl 체크 상자 클릭 영역**: 다시 그린 위치(강조 막대 + 간격 오른쪽)와 네이티브 상태 이미지 클릭 판정 영역이 같은지 실측 안 함
4. **SageListCtrl model role 이름**: 강조 · 흐림 · 빈 금액 · 행 상태를 어떤 role로 낼지 — T15/T16 설계 몫, 이 조사에서 정하지 않음
5. **SageMessageBody 분류**: `SageStyle::standardIcon` 재정의 + QLabel 두 개로 충분한지(기본 위젯 + SageStyle), 아니면 본문 위젯이 필요한지(커스텀 위젯). 200px 자르기 · 말줄임을 유지할지도 결정 필요
6. **SageSectionLabel 분류**: QLabel의 띠 바탕 · 아래선을 SageStyle이 `CE_ShapedFrame` 단계에서 그릴 수 있다는 전제는 시험 안 함. 안 되면 커스텀 위젯. 두 사용처의 글꼴(HEADER / CONTENT)을 변형 하나로 볼지 둘로 볼지도 미정
7. **SageLabel 변형 적용 방식**: QLabel 글꼴 · 색을 `Q_PROPERTY` 변형으로 SageStyle이 정하는 시점(polish, 속성 변경 후 재적용) 확인 못함
8. **SageFilterComboBox 필드 색**: PANEL / APP_BACKGROUND 차이를 변형으로 둘지, 검색 상자 안 배치에서 오는 것으로 볼지 미정
9. **SageBadge 모서리**: 헤더(알약형, 실제 반지름 10)와 요약줄(실제 반지름 2) 차이를 `SageBadgeVariant`에 묶을지 미정
10. **SageSearchBox 한 테두리 모양**: 자식 QComboBox · QLineEdit의 프레임을 끄고 바깥 한 테두리로 그리는 방식이 SageStyle만으로 되는지 확인 못함
11. **폰트**: Pretendard SemiBold를 별도 family로 찾는 현재 방식이 Qt 폰트 등록 후 OS별로 어떤 family/style 이름으로 잡히는지, 1/10 pt 값이 macOS에서 어떤 픽셀 크기가 되는지 확인 못함 (T08)
12. **SageTabCtrl 키보드 동작**: MFC 쪽 방향키 · Ctrl+Tab 동작을 실행해 보지 않음 — QTabBar와 같은지 모름
13. **SageSidebarTree 행 높이 설정 위치**: `SAGE_SIDEBAR_ITEM_HEIGHT`를 어디서 적용하는지 이 조사에서 보지 않음
14. **미사용 경로를 옮길지**: `SageButtonIcon`의 SEARCH · ADD · MOVE_UP · MOVE_DOWN, `SAGE_INLINE_WARNING`, `SAGE_LIST_FIRST_COLUMN_RIGHT`, `SageEmptyState::SetAction`, `SageSectionLabel::SetHintText` — 모두 사용처 0. CLAUDE.md 2(요청 없는 기능 금지)에 따라 빼는 것이 기본이나 결정은 사용자 몫
