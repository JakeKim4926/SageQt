# 화면별 규격

`sageqt-ui`의 상세 규격이다. 화면 · 다이얼로그를 만들 때 읽는다. 값은 `design-values.md`의 상수 이름으로 가리킨다 — "쓰는 방식"(고정 · 최소값 · 표 열 최소 폭 · 최대값)도 그 파일에 있다. 패널 구성은 `coding-design/references/ui-composition.md`.

## 메인 창

| 항목 | 규격 |
|---|---|
| 구성 | 왼쪽 사이드바 · 1px 세로 구분선 `SAGE_COLOR_BORDER` (`SageSDIView.cpp` `OnDraw`) · 오른쪽에 헤더 → 탭 줄 → 작업 영역 (`ui-composition.md` *화면 클래스 구성*) |
| 배경 | `QPalette::Window` (`SAGE_COLOR_APP_BACKGROUND`) |
| 초기 · 최소 크기 | SageSDI에 없다 (`CMainFrame`은 제목만 바꾸고 `OnGetMinMaxInfo`도 없다). 초기 `SAGE_MAIN_WINDOW_WIDTH` × `SAGE_MAIN_WINDOW_HEIGHT` (1280 × 800, 사용자 결정 2026-09-28). 최소 크기는 모든 패널이 들어오는 T16에서 내용 기준으로 정한다 |
| 창 제목 | `SAGE_UI_MAIN_WINDOW_TITLE` (이미 있음) |

## 사이드바 (T11)

SageSDI `SageSidebarPanel.cpp` · `SageSidebarTree.cpp`에서 옮긴다 (2026-09-28 코드 확인).

| 항목 | 규격 |
|---|---|
| 폭 | `SAGE_SIDEBAR_WIDTH` 고정 |
| 면 | `SAGE_COLOR_SIDEBAR` — `SageSurface`(`Sidebar`)의 팔레트 (`style-scope.md` *변형 위젯의 polish*) |
| 앱 제목 | 높이 `SAGE_HEADER_HEIGHT`, 왼쪽 여백 `SAGE_SIDEBAR_PAD_X`, 세로 가운데, 폰트 *로고* 역할, 글자 `SAGE_COLOR_SIDEBAR_TEXT`. 표시 이름 `SAGE_UI_APP_TITLE` (앱 식별 정보, `MIGRATION_PLAN.md` 결정 기록) |
| 제목 아래 선 | 1px `SAGE_COLOR_SIDEBAR_DIVIDER`, 제목 바로 아래 (y = `SAGE_HEADER_HEIGHT`) |
| 트리 위치 | 위 `SAGE_HEADER_HEIGHT + SAGE_SIDEBAR_TREE_TOP_PAD`, 아래 여백 `SAGE_MARGIN`, 좌우 여백 없음(행은 전체 폭) |
| 항목 | 높이 `SAGE_SIDEBAR_ITEM_HEIGHT`, 글자 왼쪽 `SAGE_SIDEBAR_PAD_X` — 분류 · 업무 모두 같은 들여쓰기(들여쓰기 없음) |
| 분류(그룹) 줄 | 캡션 폰트, `SAGE_COLOR_SIDEBAR_CATEGORY`, 자간 `SAGE_SIDEBAR_CATEGORY_CHAR_EXTRA`. 선택 표시 없음, 선택되지 않는다 (SageSDI는 선택만 되고 표시가 없어 업무 선택 표시가 사라졌다 — 선택을 막아 업무 표시를 유지한다) |
| 업무 · 동작 줄 | 보통: 본문 폰트 · `SAGE_COLOR_SIDEBAR_TEXT`. 선택: 면 `SAGE_COLOR_SIDEBAR_SELECTED` + 왼쪽 바 `SAGE_SELECTION_ACCENT_WIDTH` `SAGE_COLOR_PRIMARY`, *본문 강조* 폰트 · `SAGE_COLOR_SIDEBAR_SELECTED_TEXT` |
| 스크롤바 | 숨김 (SageSDI `TVS_NOSCROLL`) |
| 그리기 | 트리 항목은 `SageSidebarDelegate` (`widgets.md`) |

## 헤더 (T12)

SageSDI `SageHeaderPanel.cpp` · `SageBadge.cpp` · `SageSDIView.cpp`에서 옮긴다 (2026-09-28 코드 확인).

| 항목 | 규격 |
|---|---|
| 높이 · 하단선 | 줄 `SAGE_HEADER_HEIGHT` + 아래 1px `SAGE_COLOR_BORDER` (사이드바 제목 아래 선과 같은 높이). 면 `SAGE_COLOR_PANEL` — `SageSurface`(`Panel`) |
| 좌우 여백 | `SAGE_CONTENT_PAD_X` |
| 배치 | 왼쪽: 제목 → `SAGE_HEADER_TITLE_GAP` → 분류. 오른쪽: 사용자 라벨 → `SAGE_HEADER_GAP` → 역할 배지 → `SAGE_HEADER_GAP` → 로그인 · 로그아웃 버튼. 모두 세로 가운데 |
| 제목 | *화면 제목* 폰트 · `SAGE_COLOR_TEXT`, 글은 현재 업무 핸들러의 `headerTitle()`. 폭은 레이아웃이 정한다 (글자 수로 계산하지 않는다) |
| 분류 | 캡션 폰트 · `SAGE_COLOR_SECONDARY_TEXT`, 글은 현재 업무 핸들러의 `category()`. 폭 `SAGE_HEADER_CATEGORY_WIDTH`는 **최소값** |
| 사용자 라벨 | 캡션 폰트 · `SAGE_COLOR_TEXT_MUTED`, 오른쪽 정렬, 로그인 아이디. 폭 `SAGE_USER_LABEL_WIDTH`는 **최소값** |
| 역할 배지 | `SageBadge`(`Neutral`) — 높이 `SAGE_BADGE_HEIGHT`, 좌우 여백 `SAGE_BADGE_PAD_X`, 폭 = 글자 폭 + 여백, 캡션 폰트. 면 `SAGE_COLOR_LIST_HEADER` · 테두리 1px `SAGE_COLOR_LIST_HEADER_BORDER` · 글자 `SAGE_COLOR_PRIMARY` — 관리자 · 사용자 같은 색. 모서리는 알약 모양: SageSDI가 `RoundRect`에 타원 지름 `SAGE_BADGE_HEIGHT`를 넘긴다 (`SAGE_BADGE_RADIUS`는 요약 막대 전용) |
| 로그인 · 로그아웃 | `SageButton` `Secondary`, 본문 폰트, 폭 `SAGE_LOGIN_BTN_WIDTH`는 **최소값** · 높이 `SAGE_BUTTON_HEIGHT` |
| 인증 표시 | 로그인 안 함: 로그인 버튼만. 로그인함: 사용자 라벨 · 역할 배지(관리자 `SAGE_UI_ROLE_ADMIN`, 아니면 `SAGE_UI_ROLE_USER`) · 로그아웃 버튼 |

## 탭 줄 · 작업 영역 (T13)

| 항목 | 규격 |
|---|---|
| 탭 줄 | 높이 `SAGE_TAB_HEIGHT`(아래 1px `SAGE_COLOR_BORDER` 포함), 면 `SAGE_COLOR_PANEL`(`SageSurface` `Panel`), 줄 전체 폭. 탭은 왼쪽 `SAGE_CONTENT_PAD_X`부터 (`SageWorkspacePanel.cpp` `LayoutTabRow` · `OnEraseBkgnd`) |
| 탭 | `QTabBar` — 그리기는 `style-scope.md`. **모든 탭 같은 폭** = 가장 긴 라벨을 *본문 강조*로 잰 폭 + 좌우 `SAGE_TAB_PAD_X` (SageSDI `TCS_FIXEDWIDTH`, 여백은 사용자 결정 2026-09-28). 선택이 바뀌어도 폭이 흔들리지 않는다 |
| 인디케이터 | 선택 탭 아래 `SAGE_TAB_INDICATOR_HEIGHT` `SAGE_COLOR_PRIMARY`. SageSDI는 줄 아래선 위에 겹쳐 그리지만, SageQt는 아래선이 별도 위젯이라 인디케이터가 아래선 바로 위에 온다 (1px 차이) |
| 탭 전환 | 탭 = `QStackedWidget`의 패널 교체 — 입력 · 결과 · 실행 기록 패널 (`ui-composition.md`). 탭 목록은 핸들러의 `tabs()`(보이는 순서 → 의미 종류). 저장된 종류가 핸들러 탭에 없으면 입력 탭 |
| 업무별 상태 | 업무를 바꿨다 돌아오면 선택 탭 · 입력 경로 · 저장 폴더를 되살린다 (결과 · 필터는 T14 · T15) |
| 콘텐츠 여백 | `SAGE_CONTENT_PAD_X` · `SAGE_CONTENT_PAD_Y`, 공통 여백 `SAGE_MARGIN`, 행 간격 `SAGE_ROW_GAP` |

## 입력 패널 (T13)

| 항목 | 규격 |
|---|---|
SageSDI `SageWorkflowInputPanel.cpp` · `SageSectionLabel.cpp`에서 옮긴다 (2026-09-29 코드 확인).

| 항목 | 규격 |
|---|---|
| 입력 카드 | 면 `SAGE_COLOR_PANEL`, 테두리 1px `SAGE_COLOR_BORDER`, 콘텐츠 영역 맨 위 전체 폭 |
| 카드 제목 | `SAGE_UI_INPUT_CARD_TITLE`, 높이 `SAGE_CARD_HEADER_HEIGHT`(아래 1px `SAGE_COLOR_BORDER` 포함), 면 `SAGE_COLOR_LIST_HEADER`, *섹션 제목* 폰트 · `SAGE_COLOR_TEXT`, 왼쪽 `SAGE_CARD_PADDING` — `SageLabel` `Section` |
| 폼 줄 | 카드 안 여백 `SAGE_CARD_PADDING`. 줄 = 라벨 · 입력칸 · 버튼, 사이 `SAGE_CARD_ROW_GAP` (라벨과 입력칸 사이도 같다 — 문서의 `SAGE_LABEL_EDIT_GAP`은 코드에 없다). 줄 사이 `SAGE_CARD_ROW_GAP` |
| 폼 라벨 | 입력 파일 = 핸들러의 `inputSectionLabel()`, 저장 위치 = `SAGE_UI_SECTION_OUTPUT`. `SageLabel` `FormLabel`. 라벨 열은 가장 긴 라벨 폭, `SAGE_FORM_LABEL_WIDTH`는 최소값 |
| 경로 입력칸 | 읽기 전용 `SageLineEdit` — 면 `SAGE_COLOR_APP_BACKGROUND`(읽기 전용 Edit은 SageSDI에서 `CTLCOLOR_STATIC`으로 칠해진다), 테두리 `SAGE_COLOR_BORDER`, 글자 왼쪽 여백 `SAGE_EDIT_TEXT_LEFT_PAD`. 최소 폭 `SAGE_CO_COMPANY_EDIT_MIN_WIDTH`. 드롭을 받지 않는다 (창이 받는다) |
| 버튼 | 「파일 선택」 · 「폴더 선택」 `Secondary`, 폭 `SAGE_BUTTON_WIDTH`는 최소값 |
| 파일 선택 창 | 제목 = 핸들러의 `inputDialogTitle()`, 필터 = 핸들러의 `inputFileFilter()` (사용자 결정 2026-09-28). 폴더 선택 창 제목 `SAGE_UI_SELECT_OUTPUT_TITLE` |
| 실행 줄 (T14) | 실행 버튼 `Primary` · 입력 초기화 `Ghost`, 높이 `SAGE_CARD_ACTION_BUTTON_HEIGHT`, 사이 `SAGE_ACTION_GAP`, 폭 `SAGE_BUTTON_WIDTH` · `SAGE_INPUT_RESET_WIDTH`는 최소값 — T14에서 카드 아래쪽에 넣는다 |
| 파일 드롭 | 창 어디에 떨어뜨려도 첫 번째 파일이 입력 경로가 되고 입력 탭으로 간다 (`SageWorkspacePanel.cpp` `ApplyDroppedInputPaths`). 실행 중 무시 · 입력 표 업무의 자동 불러오기는 T14 |

## 실행 · 상태 카드 (T14)

| 항목 | 규격 |
|---|---|
| 카드 | `SageStatusCard` — 높이 `SAGE_STATUS_CARD_HEIGHT` 고정 |
| 상태 4개 | 대기 · 처리 중 · 완료 · 실패 — 면 · 테두리 · 점 · 아이콘 · 글자색은 `docs/decisions/sageqt-ui/sagesdi-ui-analysis.md` §4.7과 `design-values.md` *상태 카드* |
| 줄 높이 | 제목 `SAGE_STATUS_CARD_TITLE_LINE_HEIGHT` · 상세 `SAGE_STATUS_CARD_DETAIL_LINE_HEIGHT` · 진행 막대 `SAGE_STATUS_CARD_PROGRESS_HEIGHT` |
| 액션 칸 | 「폴더 열기」 — `SAGE_STATUS_CARD_ACTION_WIDTH`는 최소값. 진행률 `%` 글자 칸 `SAGE_PROGRESS_TEXT_WIDTH`도 최소값 |
| 저장 경로 | 넘치면 말줄임 (`widgets.md` *경로 말줄임*) |

## 결과 표 (T15) · 실행 기록 (T16)

| 항목 | 규격 |
|---|---|
| 행 | 높이 `SAGE_LIST_ROW_HEIGHT` 고정, 가로선 `SAGE_LIST_GRID_THICKNESS` `SAGE_COLOR_LIST_GRID`, 셀 여백 `SAGE_LIST_CELL_LEFT_PAD` · `SAGE_LIST_CELL_RIGHT_PAD`. 두 줄 셀 없음 |
| 헤더 | `style-scope.md` *QHeaderView* |
| 선택 행 | 면 `SAGE_COLOR_LIST_ROW_SELECTED` + 왼쪽 바 `SAGE_LIST_SELECTION_ACCENT_WIDTH` `SAGE_COLOR_PRIMARY` (가로선 위). 포커스 표시 없음 |
| 마우스를 올린 행 | 행 전체 면 `SAGE_COLOR_LIST_ROW_HOVER` (`#F8F1E6` — 흰색과 선택 행 색의 중간). 선택 행은 선택 색이 이긴다. **SageSDI에는 없다** — T15 세 OS 스크린샷과 후보 3개를 보고 사용자 결정 (2026-10-01) |
| 열 폭 | 결과 표 `SAGE_RESULT_*_WIDTH`, 실행 기록 `SAGE_HISTORY_*_WIDTH`. 고정 열은 정의 폭, 늘어나는 열은 남는 폭 — 정의 폭보다 작아지면 모든 열을 정의 폭으로 두고 가로 스크롤 (SageSDI와 같다, `coding-design/references/model-view.md` 예외, 사용자 결정 2026-10-01) |
| 늘어나는 열이 둘 이상 | 정의 폭 비율로 나누고 마지막 늘어나는 열이 나머지를 받는다 (`DistributeColumnWidths` 그대로) |
| 배지 열 | 높이 `SAGE_LIST_BADGE_HEIGHT`, 좌우 여백 `SAGE_LIST_BADGE_PAD_X`, 반경 `SAGE_LIST_BADGE_RADIUS`, 캡션 폰트 |
| 요약 · 합계 · 선택 바 | `SAGE_SUMMARY_*` · `SAGE_TOTAL_BAR_HEIGHT` · `SAGE_SELECTION_*`. 흰 박스 금지, 폭은 표 폭을 따른다 |
| 검색 · 필 바 | `SAGE_SEARCH_*` · `SAGE_RESULT_FILTER_*` · `SAGE_PILL_*` |
| 빈 상태 | `SageEmptyState` — `SAGE_EMPTY_*`. 설명 폭 `SAGE_EMPTY_DESC_MAX_WIDTH`는 최대값 |
| 표 최소 높이 | `SAGE_RESULT_MIN_HEIGHT` |

## 프레임리스 다이얼로그 · 메시지 상자 (T09)

| 항목 | 규격 |
|---|---|
| 캡션 | 높이 `SAGE_DLG_CAPTION_HEIGHT`, 좌우 여백 `SAGE_DLG_CAPTION_PAD`, 닫기 버튼 `SAGE_DLG_CAPTION_BTN_SIZE` · 오른쪽 여백 `SAGE_DLG_CAPTION_BTN_PAD`, 면 `SAGE_COLOR_LIST_HEADER`, 아래 1px `SAGE_COLOR_BORDER`, 제목 *본문 강조* `SAGE_COLOR_TEXT` 말줄임 |
| 면 | `SAGE_COLOR_PANEL` (`SageMessageBoxDlg.cpp:62`) |
| 테두리 | 1px `SAGE_COLOR_BORDER`. SageSDI는 `WS_BORDER`라 색을 Windows가 정한다 — 상수가 없어 이 값을 쓴다 |
| 메시지 상자 폭 | `SAGE_MSGBOX_WIDTH`는 최소값 |
| 본문 | 최대 높이 `SAGE_MSGBOX_MAX_TEXT_HEIGHT`(최대값), 넘치면 자른다 — 말줄임 없음 (사용자 결정 2026-09-28: 쓰는 안내문이 모두 이 높이 안에 든다). 폭 `SAGE_MSGBOX_WIDTH`에서 줄을 바꾼다 |
| 아이콘 | `SAGE_MSGBOX_ICON_SIZE` · `SAGE_MSGBOX_ICON_RADIUS`, 본문과 간격 `SAGE_MSGBOX_ICON_TEXT_GAP`, 선 `SAGE_ICON_STROKE` |
| 종류 · 제목 | 정보 「알림」 · 경고 「경고」 · 오류 「오류」. 아이콘 없음 · 확인형(예 · 아니오)은 호출 0곳이라 옮기지 않는다 |
| 배치 | 여백 `SAGE_MARGIN` — 캡션 아래 · 본문 아래 · 버튼 아래 · 좌우. 본문 높이는 아이콘 크기 이상, 한 줄이면 아이콘과 세로 가운데 |
| 버튼 | 「확인」 1개 `Primary`, 오른쪽 정렬, 폭 `SAGE_LOGIN_DLG_BTN_WIDTH`는 최소값 · 높이 `SAGE_BUTTON_HEIGHT`. Enter = 확인, Esc · 닫기 = 취소 |

## 로그인 · 비밀번호 변경 (T10)

SageSDI `SageLoginDlg.cpp` · `SagePasswordChangeDlg.cpp` · `SageInlineError.cpp` · `SageEdit.cpp`에서 옮긴다 (2026-09-28 코드 확인).

| 항목 | 규격 |
|---|---|
| 창 폭 | `SAGE_LOGIN_DLG_WIDTH` · `SAGE_PASSWORD_DLG_WIDTH`는 최소값. 면 `SAGE_COLOR_PANEL` (프레임리스 기반) |
| 배치 | 여백 `SAGE_MARGIN`. 입력 줄 사이 `SAGE_ROW_GAP`, 마지막 입력칸 바로 아래 인라인 오류 줄(간격 없음), 비밀번호 변경은 그 아래 안내 줄(간격 없음), 그 아래 `SAGE_ROW_GAP` 뒤 버튼 줄 |
| 라벨 열 | `SAGE_LOGIN_DLG_LABEL_WIDTH` · `SAGE_PASSWORD_DLG_LABEL_WIDTH`는 최소값, 가장 긴 라벨(「새 비밀번호 확인」 등)에 맞춘다. 라벨과 입력칸 사이 `SAGE_ROW_GAP`. 라벨은 `SageLabel` `FormLabel`(본문 폰트 · `SAGE_COLOR_TEXT_MUTED`), 세로 가운데 |
| 입력칸 | `SageLineEdit` — 비밀번호는 `QLineEdit::Password`. 새 비밀번호 · 확인은 최대 `SAGE_USER_PW_MAX_LEN`자 |
| 인라인 오류 | `SageInlineMessage` — 입력칸 열에 맞춰 높이 `SAGE_INLINE_MSG_HEIGHT` 자리를 **항상 비워 둔다** (SageSDI는 고정 배치, 메시지가 없으면 빈 줄). 아이콘 `SAGE_INLINE_MSG_ICON_SIZE`(원 반지름 `SAGE_INLINE_ICON_RADIUS` · 선 `SAGE_BORDER_THICKNESS` · 줄기 `SAGE_INLINE_ICON_STEM_TOP`~`BOTTOM` · 점 `SAGE_INLINE_ICON_DOT_TOP` · `SAGE_INLINE_ICON_DOT_SIZE`, 색 `SAGE_COLOR_ERROR`), 아이콘과 글자 사이 `SAGE_INLINE_MSG_ICON_GAP`, 캡션 폰트 `SAGE_COLOR_INLINE_ERROR_TEXT`, 한 줄 말줄임 |
| 안내 (비밀번호 변경) | `SAGE_UI_CHANGE_PW_HINT`, `SageLabel` `SecondaryCaption`, 높이 `SAGE_INLINE_MSG_HEIGHT` |
| 버튼 | 「로그인」 · 「변경」 `Primary`, 「취소」 `Secondary`. 오른쪽 정렬, 사이 `SAGE_ROW_GAP`, 폭 `SAGE_LOGIN_DLG_BTN_WIDTH`는 최소값. Enter = 기본 버튼, Esc · 닫기 = 취소 |

