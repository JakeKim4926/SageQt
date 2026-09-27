# 화면별 규격

`sageqt-ui`의 상세 규격이다. 화면 · 다이얼로그를 만들 때 읽는다. 값은 `design-values.md`의 상수 이름으로 가리킨다 — "쓰는 방식"(고정 · 최소값 · 표 열 최소 폭 · 최대값)도 그 파일에 있다. 패널 구성은 `coding-design/references/ui-composition.md`.

## 메인 창

| 항목 | 규격 |
|---|---|
| 구성 | 왼쪽 사이드바 · 오른쪽에 헤더 → 탭 줄 → 작업 영역 (`ui-composition.md` *화면 클래스 구성*) |
| 배경 | `QPalette::Window` (`SAGE_COLOR_APP_BACKGROUND`) |
| 초기 · 최소 크기 | **SageSDI에 없다** (`CMainFrame`은 제목만 바꾸고 `OnGetMinMaxInfo`도 없다). T08에서 사용자에게 확인한다 |
| 창 제목 | `SAGE_UI_MAIN_WINDOW_TITLE` (이미 있음) |

## 사이드바 (T11)

| 항목 | 규격 |
|---|---|
| 폭 | `SAGE_SIDEBAR_WIDTH` 고정 |
| 면 · 글자 | `SAGE_COLOR_SIDEBAR` · `SAGE_COLOR_SIDEBAR_TEXT`, 분류 글자 `SAGE_COLOR_SIDEBAR_CATEGORY` · 글자 간격 `SAGE_SIDEBAR_CATEGORY_CHAR_EXTRA` |
| 여백 | 좌우 `SAGE_SIDEBAR_PAD_X`, 트리 위 `SAGE_SIDEBAR_TREE_TOP_PAD` |
| 항목 | 높이 `SAGE_SIDEBAR_ITEM_HEIGHT`. 선택: 면 `SAGE_COLOR_SIDEBAR_SELECTED` + 왼쪽 바 `SAGE_SELECTION_ACCENT_WIDTH` `SAGE_COLOR_PRIMARY` (밝은 카멜 채움 금지). 선택 글자색 미정 |
| 스크롤바 | 숨김. 업무가 넘칠 때의 규격은 없다 — T11에서 |
| 로고 (앱 제목) | 폰트 *로고* 역할 (SKILL.md *폰트*), 표시 이름은 앱 식별 정보 (`MIGRATION_PLAN.md` 결정 기록) |
| 그리기 | 트리 항목은 `SageSidebarDelegate` (`widgets.md`) |

## 헤더 (T12)

| 항목 | 규격 |
|---|---|
| 높이 · 하단선 | `SAGE_HEADER_HEIGHT` 고정 · 1px `SAGE_COLOR_BORDER` |
| 간격 | `SAGE_HEADER_GAP`, 제목과 분류 사이 `SAGE_HEADER_TITLE_GAP` |
| 제목 | 폰트 *화면 제목* — 폭은 레이아웃이 정한다 (글자 수로 계산하지 않는다) |
| 분류 · 사용자 라벨 · 로그인 버튼 | `SAGE_HEADER_CATEGORY_WIDTH` · `SAGE_USER_LABEL_WIDTH` · `SAGE_LOGIN_BTN_WIDTH`는 **최소값**. 사용자 라벨은 넘치면 말줄임 |
| 역할 배지 | `SageBadge` — 높이 `SAGE_BADGE_HEIGHT`, 좌우 여백 `SAGE_BADGE_PAD_X`, 반경 `SAGE_BADGE_RADIUS`(`widgets.md` 반경 주의). 폭 = 글자 폭 + 여백. 「관리자」 배지 색 미정 |

## 탭 줄 · 작업 영역 (T13)

| 항목 | 규격 |
|---|---|
| 탭 줄 | `QTabBar` — `style-scope.md` *재정의하는 표준 위젯 요소* |
| 선택 탭 Bold | 선택 · 비선택 굵기가 달라 탭 글자 폭이 바뀐다 — 탭 폭이 흔들리지 않게 Bold 기준 폭으로 잡을지 T13에서 확인 |
| 콘텐츠 여백 | `SAGE_CONTENT_PAD_X` · `SAGE_CONTENT_PAD_Y`, 공통 여백 `SAGE_MARGIN`, 행 간격 `SAGE_ROW_GAP` |

## 입력 패널 (T13)

| 항목 | 규격 |
|---|---|
| 폼 라벨 | 한 폼의 라벨 열은 가장 긴 라벨 폭으로 맞춘다. `SAGE_FORM_LABEL_WIDTH`는 최소값 |
| 라벨과 입력칸 사이 | SageSDI 문서의 `SAGE_LABEL_EDIT_GAP`(4)은 코드에 상수가 없다 — T13에서 코드의 실제 배치를 확인 |
| 입력칸 · 버튼 | 높이 `SAGE_EDIT_HEIGHT` · `SAGE_BUTTON_HEIGHT` 고정. 버튼 폭 `SAGE_BUTTON_WIDTH` · `SAGE_INPUT_RESET_WIDTH`는 최소값 |
| 버튼 사이 | `SAGE_ACTION_GAP` |

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
| 선택 행 | 면 `QPalette::Highlight` + 왼쪽 바 `SAGE_LIST_SELECTION_ACCENT_WIDTH` `SAGE_COLOR_PRIMARY` |
| 열 폭 | 결과 표 `SAGE_RESULT_*_WIDTH`, 실행 기록 `SAGE_HISTORY_*_WIDTH` — **표 열 최소 폭**. 늘어나는 열은 `QHeaderView::Stretch` |
| 늘어나는 열이 둘 이상 | SageSDI는 정의 폭 비율로 나눴다 (`DistributeColumnWidths`). `QHeaderView::Stretch`는 균등 분배라 결과가 다르다 — 결과 표는 늘어나는 열이 하나라 해당 없음. 둘 이상인 표가 생기면 그때 정한다 |
| 배지 열 | 높이 `SAGE_LIST_BADGE_HEIGHT`, 좌우 여백 `SAGE_LIST_BADGE_PAD_X`, 반경 `SAGE_LIST_BADGE_RADIUS`, 캡션 폰트 |
| 요약 · 합계 · 선택 바 | `SAGE_SUMMARY_*` · `SAGE_TOTAL_BAR_HEIGHT` · `SAGE_SELECTION_*`. 흰 박스 금지, 폭은 표 폭을 따른다 |
| 검색 · 필 바 | `SAGE_SEARCH_*` · `SAGE_RESULT_FILTER_*` · `SAGE_PILL_*` |
| 빈 상태 | `SageEmptyState` — `SAGE_EMPTY_*`. 설명 폭 `SAGE_EMPTY_DESC_MAX_WIDTH`는 최대값 |
| 표 최소 높이 | `SAGE_RESULT_MIN_HEIGHT` |

## 프레임리스 다이얼로그 · 메시지 상자 (T09)

| 항목 | 규격 |
|---|---|
| 캡션 | 높이 `SAGE_DLG_CAPTION_HEIGHT`, 좌우 여백 `SAGE_DLG_CAPTION_PAD`, 닫기 버튼 `SAGE_DLG_CAPTION_BTN_SIZE` · 여백 `SAGE_DLG_CAPTION_BTN_PAD`, 면 `SAGE_COLOR_LIST_HEADER` |
| 테두리 | 1px `SAGE_COLOR_BORDER` |
| 메시지 상자 폭 | `SAGE_MSGBOX_WIDTH`는 최소값 |
| 본문 | 최대 높이 `SAGE_MSGBOX_MAX_TEXT_HEIGHT`(최대값), 넘치면 말줄임 |
| 아이콘 | `SAGE_MSGBOX_ICON_SIZE` · `SAGE_MSGBOX_ICON_RADIUS`, 본문과 간격 `SAGE_MSGBOX_ICON_TEXT_GAP`, 선 `SAGE_ICON_STROKE` |
| 버튼 | 확인형: 「예」 `Danger` · 「아니오」 `Secondary`. 알림형: 확인 1개 |

## 로그인 · 비밀번호 변경 (T10)

| 항목 | 규격 |
|---|---|
| 창 폭 | `SAGE_LOGIN_DLG_WIDTH` · `SAGE_PASSWORD_DLG_WIDTH`는 최소값 |
| 라벨 열 | `SAGE_LOGIN_DLG_LABEL_WIDTH` · `SAGE_PASSWORD_DLG_LABEL_WIDTH`는 최소값, 가장 긴 라벨(「변경할 비밀번호」 등)에 맞춘다 |
| 버튼 | 폭 `SAGE_LOGIN_DLG_BTN_WIDTH`는 최소값 |
| 인라인 오류 | `SageInlineMessage` — 높이 `SAGE_INLINE_MSG_HEIGHT`, 글자 `SAGE_COLOR_INLINE_ERROR_TEXT`, 캡션 폰트. 자리를 비워 두는지는 SageSDI 코드의 동작을 따른다 (SKILL.md *값 출처 원칙*) |
| 입력칸 오류 상태 | `SageLineEditVariant::Error` |
