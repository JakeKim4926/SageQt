# 위젯 · 변형 · delegate

`sageqt-ui`의 상세 규격이다. 위젯 · delegate를 만들 때 읽는다. SageSDI `app/ui/drawing/` 25종의 조사 원문(줄 번호 · 상태별 색 · 사용처)은 `docs/decisions/sageqt-ui/controls-classification.md`.

## 분류 기준

`coding-design/references/style.md`를 따른다.
- **기본 위젯 + `SageStyle`** — Qt 위젯이 하는 일이면 모양만 `SageStyle`이 바꾼다. 변형이 필요하면 **변형 속성만 가진 얇은 서브클래스**(`paintEvent` 없음)
- **커스텀 위젯** — Qt에 없는 요소. `ui/widgets/`에 `Sage*` 위젯, 자기 `paintEvent`
- **delegate** — view 안의 항목 그리기. `ui/widgets/`에 `Sage*Delegate`
- **옮기지 않음** — SageSDI에서 사용처 0, 또는 MFC · GDI 때문에만 있던 것

## SageSDI 컨트롤 25종

| SageSDI | 분류 | SageQt | 변형 enum | 주제 |
|---|---|---|---|---|
| `SageButton` | 기본 위젯 + SageStyle | `SageButton : QPushButton` (아이콘 단독은 `QToolButton`) | `SageButtonVariant { Secondary, Primary, Ghost }` (`Danger`는 호출 0곳 — `style-scope.md`) | T08 · T09 · T10 · T12 ~ T16 |
| `SageEdit` | 기본 위젯 + SageStyle | `SageLineEdit : QLineEdit` | `SageLineEditVariant { Normal, Error }` | T08 · T10 |
| `SageLabel` | 기본 위젯 + SageStyle | `SageLabel : QLabel` | `SageLabelVariant { Title, Section, SecondaryCaption, MutedCaption, FormLabel, Hint, SidebarLogo }` — 값은 쓰는 주제에서 추가 (T11: `SidebarLogo`, T12: `Title` · `SecondaryCaption` · `MutedCaption`) | T10 ~ T13 · T15 |
| `SageSectionLabel` | 기본 위젯 + SageStyle | `SageLabel` (`Section`) | 위 enum의 `Section` | T13 · T15 |
| `SageTabCtrl` | 기본 위젯 + SageStyle | `QTabBar` + `QStackedWidget` | — | T13 |
| `SageHeaderCtrl` | 기본 위젯 + SageStyle | `QHeaderView` | — | T15 · T16 |
| `SageFilterComboBox` | 기본 위젯 + SageStyle | `QComboBox` | 필드 색 차이를 변형으로 둘지 T15에서 | T15 |
| `SageMessageBody` | 기본 위젯 + SageStyle | `QLabel` ×2 (아이콘 · 본문) + `SageStyle::standardIcon` | — | T09 |
| `SageSelectionBar` | 기본 위젯 + SageStyle | `QCheckBox` + `QLabel` + `QPushButton` 조합 패널 | — | T15 |
| `SageBadge` | 커스텀 위젯 | `SageBadge` | `SageBadgeVariant { Neutral, Warning }` — 값은 쓰는 주제에서 추가 (T12: `Neutral`) | T12 · T15 |
| `SageDialogCaptionBar` | 커스텀 위젯 | `SageDialogCaptionBar` | — | T09 |
| `SageInlineError` | 커스텀 위젯 | `SageInlineMessage` | `SageInlineMessageVariant { Error }` (경고는 사용처 0) | T10 |
| `SageStatusCard` | 커스텀 위젯 | `SageStatusCard` (진행 막대는 `QProgressBar`) | `SageStatusCardVariant { Idle, Running, Completed, Failed }` | T14 |
| `SageSummaryBar` | 커스텀 위젯 | `SageSummaryBar` | — (항목 단위 강조 · 배지) | T15 |
| `SageTableTotalBar` | 커스텀 위젯 | `SageTableTotalBar` | — | T15 |
| `SageSearchBox` | 커스텀 위젯 | `SageSearchBox` (`QComboBox` + `QLineEdit` 조합) | — | T15 |
| `SageFilterPillBar` | 커스텀 위젯 | `SageFilterPillBar` | — (선택은 상태) | T16 |
| `SageEmptyState` | 커스텀 위젯 | `SageEmptyState` | — | T16 |
| `SageListCtrl` | delegate | `QTableView` + model/proxy + `QHeaderView` + `SageResultTableDelegate` | — (model role) | T15 · T16 |
| `SageSidebarTree` | delegate | `QTreeView` + `SageSidebarDelegate` | — | T11 |
| (SageSDI의 패널 `OnEraseBkgnd` 면 칠하기) | 기본 위젯 + SageStyle | `SageSurface : QWidget` | `SageSurfaceVariant { Sidebar, Header }` | T11 · T12 |
| `SageUiResources` | 공용 자원 | 앱 폰트 · 역할 폰트 · `QPalette` · `SageDesignDefine.h` | — | T08 |
| `SageUiStyle` | 공용 자원 | `SageStyle` 요소 그리기 + `ui/style/` 공용 조각 | — | T08 |
| `SageComboBox` | 옮기지 않음 | — | — | 사용처 0 |
| `SageListBox` | 옮기지 않음 | — | — | 사용처 0 (근거 화면 삭제) |
| `SageOptionCheck` | 옮기지 않음 | — | — | 사용처 0 (`SageResultTablePanel.h`가 include만) |

행 수 26 (SageSDI 25종 + 패널 면 `SageSurface`). 이름(`SageButton` · `SageLineEdit` 등)은 후보다 — 착수 주제에서 `coding-rules/references/naming.md`로 확정한다.

### 옮기지 않는 경로 (SageSDI 사용처 0)

- `SageButton` 아이콘 `SEARCH` · `ADD` · `MOVE_UP` · `MOVE_DOWN` — 쓰는 것은 `RESET` · `CLOSE`뿐
- `SageEmptyState::SetAction` — 빈 상태의 동작 버튼은 한 번도 표시되지 않았다
- `SageSectionLabel::SetHintText`, enum 값 `SAGE_INLINE_WARNING`(`SageInlineError.h`) · `SAGE_LIST_FIRST_COLUMN_RIGHT`(`SageListCtrl.h`)

## 이관할 때 조용히 바뀌는 것

| 지점 | SageSDI | SageQt에서 할 일 |
|---|---|---|
| hover · 포커스 표시 | hover 없음, 포커스 표시는 일부 버튼에만 | Fusion은 그린다 — 규격은 그 위젯을 처음 만드는 주제에서 사용자와 정한다 (`style-scope.md` *미정*) |
| 모서리 반경 | `RoundRect`는 타원 폭 · 높이 — 배지는 상수 그대로, 나머지는 `RADIUS * 2` | `drawRoundedRect`는 반지름 — 배지만 절반으로 |
| 흐린 글자색 | 셀 문자열 비교로 정한다 (`"—"`, `"미리보기 (저장 없음)"`) | model role로 — 문자열 비교를 옮기지 않는다 (T15) |
| 요약 바 폭 부족 | 넘치는 항목부터 말없이 버린다, 배지는 첫 항목에만 | 동작을 그대로 옮길지 T15에서 확인 |
| 경로 말줄임 | `DT_PATH_ELLIPSIS` | `Qt::ElideMiddle`은 결과가 다르다 — T14에서 비교 |
| 메시지 본문 | 200px(`SAGE_MSGBOX_MAX_TEXT_HEIGHT`)에서 자르고 말줄임 | 최대값으로 옮기고 말줄임은 하지 않는다 (T09, 사용자 결정) |
| 검색창 Enter · 자식 알림 | 부모 `PreTranslateMessage` · `OnCommand`가 중계 | 위젯이 의미 있는 signal을 낸다 — 중계 금지 (`ui-composition.md`) |
| Ctrl+A 전체 선택 | `SageHandleEditSelectAll` | `QLineEdit` 기본 (macOS Cmd+A) — 옮기지 않는다 |
| SemiBold 선택 | 별도 서체 이름 `"Pretendard SemiBold"` | 패밀리 + 굵기 (SKILL.md *폰트*) |
| 콤보 화살표 좌표 | 코드에 박힌 ±4 · −2 · +3 | 상수로 (`SAGE_ICON_ARROW_*`) |
| 캡션 닫기 버튼 면 | `SetSurfaceColor`를 부르지 않아 `SAGE_COLOR_PANEL` — 캡션 위 흰 사각형 (코드 확인 2026-09-28) | 코드대로 옮기고 스크린샷으로 사용자가 SageSDI 화면과 대조 (T09) |
