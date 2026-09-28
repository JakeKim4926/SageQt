# SageStyle 재정의 범위

`sageqt-ui`의 상세 규격이다. `SageStyle`(Fusion 기반 `QProxyStyle`) · `QPalette` · 앱 폰트를 만들거나 고칠 때 읽는다. 구조 규칙은 `coding-design/references/style.md`, 값은 `design-values.md`. 여기서는 값을 상수 이름으로만 가리킨다.

## 팔레트 역할

`main.cpp`에서 한 번 적용하는 `QPalette`의 역할 대응이다. 표에 없는 역할은 Fusion 기본값을 둔다.

| `QPalette` 역할 | 값 | SageSDI 용도 |
|---|---|---|
| `Window` | `SAGE_COLOR_APP_BACKGROUND` | 앱 배경 · 콘텐츠 영역 |
| `Base` | `SAGE_COLOR_PANEL` | 입력칸 · 표 · 패널 면 |
| `AlternateBase` | `SAGE_COLOR_LIST_ROW_ALT` | 표 교대 행 |
| `WindowText` · `Text` · `ButtonText` | `SAGE_COLOR_TEXT` | 본문 · 표 · Secondary 버튼 글자 |
| `PlaceholderText` | `SAGE_COLOR_TEXT_PLACEHOLDER` | 입력칸 안내 · 빈 값 `—` |
| `Button` | `SAGE_COLOR_PANEL` | Secondary 버튼 면 |
| `Highlight` | `SAGE_COLOR_LIST_ROW_SELECTED` | 표 선택 행 면 |
| `HighlightedText` | `SAGE_COLOR_TEXT` | 표 선택 행 글자 (SageSDI 표는 선택 행 글자색을 바꾸지 않는다 — `SageListCtrl.cpp:331`) |
| `Accent` | `SAGE_COLOR_PRIMARY` | 주요 액션 · 탭 인디케이터 · 진행 막대 |

## 재정의하는 표준 위젯 요소

| 위젯 | `QStyle` 요소 (후보) | 규격 | 변형 |
|---|---|---|---|
| `QPushButton` | `PE_PanelButtonCommand` · `CE_PushButtonLabel` | 높이 `SAGE_BUTTON_HEIGHT`, 테두리 `SAGE_BORDER_THICKNESS`. 면 · 테두리 · 글자 · 굵기는 변형별 (아래) | `SageButtonVariant` |
| `QToolButton` (아이콘 단독) | `PE_PanelButtonTool` | 정사각 — 크기는 쓰는 곳의 상수 (캡션 닫기 `SAGE_DLG_CAPTION_BTN_SIZE`). SageSDI 문서의 `SAGE_ICON_BUTTON_SIZE`(32)는 코드에 없다. 아이콘 `SAGE_ICON_SIZE` · 선 `SAGE_ICON_STROKE`, 툴팁 필수 | — |
| `QLineEdit` | `PE_PanelLineEdit` · `PE_FrameLineEdit` | 높이 `SAGE_EDIT_HEIGHT`, 테두리 `SAGE_EDIT_BORDER_WIDTH` `SAGE_COLOR_BORDER`, 텍스트 여백 `SAGE_EDIT_TEXT_LEFT_PAD` · `SAGE_EDIT_TEXT_TOP_PAD` | `SageLineEditVariant` (`Normal` · `Error` — 오류 테두리 `SAGE_COLOR_ERROR`) |
| `QComboBox` | `CC_ComboBox` | 높이 `SAGE_EDIT_HEIGHT`, 필드 여백 `SAGE_COMBO_FIELD_INSET`, 화살표 `SAGE_ICON_ARROW_HALF_WIDTH` · `SAGE_ICON_ARROW_HALF_HEIGHT` | — |
| `QTabBar` | `CE_TabBarTabShape` · `CE_TabBarTabLabel` | 줄 높이 `SAGE_TAB_HEIGHT`, 줄 하단 1px `SAGE_COLOR_BORDER`. 선택 탭: 면 `SAGE_COLOR_PANEL` · Bold · `SAGE_COLOR_TEXT` · 하단 인디케이터 `SAGE_TAB_INDICATOR_HEIGHT` `SAGE_COLOR_PRIMARY`. 비선택: Regular · `SAGE_COLOR_SECONDARY_TEXT` | — |
| `QHeaderView` | `CE_HeaderSection` · `CE_HeaderLabel` | 높이 `SAGE_LIST_HEADER_HEIGHT`, 면 `SAGE_COLOR_LIST_HEADER`, 글자 `SAGE_COLOR_TEXT_MUTED`, 세로 구분선 없음, 항상 가운데 | — |
| `QProgressBar` (상태 카드) | `CE_ProgressBarGroove` · `CE_ProgressBarContents` | 높이 `SAGE_STATUS_CARD_PROGRESS_HEIGHT`, 트랙 `SAGE_COLOR_LIST_GRID`, 채움 `SAGE_COLOR_PRIMARY` | — |
| `QFrame` (패널 경계) | `PE_Frame` | 1px `SAGE_COLOR_BORDER` | — |

표 행 · 사이드바 항목은 스타일이 아니라 delegate가 그린다 (`widgets.md`).

### 버튼 변형 (`SageButtonVariant`)

| 값 | 면 | 테두리 | 글자 | 굵기 | 용도 |
|---|---|---|---|---|---|
| `Secondary` (기본) | `SAGE_COLOR_PANEL` | `SAGE_COLOR_BUTTON_BORDER` | `SAGE_COLOR_TEXT` | Regular | 파일 · 폴더 선택, 취소, 폴더 열기, 메시지 상자 「아니오」 |
| `Primary` | `SAGE_COLOR_PRIMARY` | `SAGE_COLOR_PRIMARY` | 흰색 (상수 없음 — 버튼을 만드는 T09에서 `SageDesignDefine.h`에 추가) | Bold | 실행 · 저장 · 확인 — **화면당 1개** |
| `Ghost` | 없음 | 없음 | `SAGE_COLOR_TEXT_MUTED` | Regular | 초기화 · 선택 해제 |
| `Danger` | `SAGE_COLOR_PANEL` | `SAGE_COLOR_DANGER_BORDER` | `SAGE_COLOR_ERROR` | Bold | 삭제 · 메시지 상자 「예」(확인형) |

## 폰트 적용

- 앱 기본 폰트(본문 14px Regular)는 `main.cpp`에서 `QApplication::setFont`로 한 번 적용한다
- 역할 폰트(제목 · 섹션 · 캡션 · 요약 수치 · 로고)는 **`SageStyle::polish(QWidget*)`가 위젯의 변형을 보고 적용한다.** 화면 · 위젯 코드에서 `setFont`를 부르지 않는다 (`style.md`)
- delegate와 커스텀 위젯의 `paintEvent`는 `ui/style/`의 공용 함수로 역할 폰트를 얻는다 — 역할 → 폰트 대응을 한 곳에 둔다
- 변형이 바뀌면 위젯이 `SageStyle`에 다시 polish를 요청해야 폰트가 바뀐다 (`unpolish` → `polish`, 또는 `QEvent::StyleChange`). 방법은 변형 위젯을 처음 만드는 T09에서 정한다

## 미정 — 처음 쓰는 주제에서 사용자가 화면을 보고 정한다

SageSDI에 규격이 없다. 추측으로 채우지 않는다 (SKILL.md *값 출처 원칙*). T08은 사용자 확인 없이 진행해 이 항목을 정하지 않았다 — 그 위젯을 처음 만드는 주제에서 세 OS 스크린샷을 사용자에게 보여 주고 정한다 (버튼 · 캡션 T09, 입력칸 T10, 탭 · 콤보 T13, 헤더 · 체크 상자 · 스크롤바 T15).

| 항목 | 상태 |
|---|---|
| hover · focus · disabled — 버튼 · 입력칸 · 콤보 · 탭 · 헤더 | SageSDI는 hover가 없고 포커스 표시는 일부 버튼에만 있다. Fusion은 그린다 |
| Primary 버튼 pressed 면 | `SAGE_COLOR_PRIMARY_PRESS`를 쓰는지 SageSDI 코드에서 확인 |
| 읽기 전용 입력칸 면 (입력 경로 · 저장 위치) | 규격 없음 |
| 버튼 · 입력칸 · 카드 반경 | 규격 없음. **주의**: SageSDI `RoundRect`는 타원 폭 · 높이를 받는다 — SageBadge는 반지름 상수를 그대로, 다른 곳은 `RADIUS * 2`를 넘긴다. Qt `drawRoundedRect`는 반지름을 받으므로 상수를 그대로 쓰면 배지 모서리가 두 배로 둥글어진다 |
| 표 행 상태 색 (성공 · 실패 행 면 · 배지) | 실패 행 `#FDF6F4`만 문서에 있다. 코드(`SetRowStyle`)에서 확인 |
| 「관리자」 배지 색 | 문서 · 코드 값 확인 필요 (T12) |
| 사이드바 선택 항목 글자색 | 규격 없음 (T11) |
| 검색 박스 면 · 필 바 선택 상태 | 규격 없음 (T15 · T16) |
| 스크롤바 | 사이드바는 숨김(T11). 나머지는 Fusion 기본으로 둘지 T15에서 |
| 체크 상자 (표 · 선택 바) | `SAGE_LIST_CHECK_BOX_SIZE` · `SAGE_LIST_CHECK_MARK_THICKNESS` 외 규격 없음 |
| 흰색 글자 상수 | Primary 버튼 글자 — T09에서 `SageDesignDefine.h`에 추가 |
