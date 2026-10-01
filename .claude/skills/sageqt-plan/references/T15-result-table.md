# T15 — 결과 표

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 결과 표 패널(`SageResultTablePanel` — 입력 표와 결과 표에 같은 클래스를 두 인스턴스로 쓴다)을 Qt Model/View로 옮긴다. SageSDI에서 패널 하나가 들고 있던 행 · 보이는 행 · 검색어 · 필터 기준 · 체크 상태를 model · proxy · header · delegate로 나눈다.

## 시작 전에
1. 선행 주제: 없음 (T14 완료 — 실행 흐름 · 상태 카드 · 진행 표시가 있다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: **`coding-design/references/model-view.md`** (책임 분담표 — 이 주제의 기준), `ui-composition.md` (한 위젯이 두 탭 역할 금지 → 인스턴스 둘), `style.md`, `coding-rules/references/api-shape.md` (컨테이너 · `std::as_const`)
3. 결정 — `sageqt-ui`의 표 규격을 따른다
4. 재확인할 사실 — 착수 시 원문을 읽는다
   - **T14에서 넘긴 것** — 입력 초기화 버튼(`Ghost`, 폭 `SAGE_INPUT_RESET_WIDTH` 72 최소값, 실행 버튼 옆 `SAGE_ACTION_GAP` 8, 입력 표 업무만)과 `ResetInput`(입력 경로 비움 · 상태 카드 대기 · 필터 · 행 · 결과 상태 초기화). 입력 패널 빈 상태 안내 `SAGE_UI_EMPTY_STATE_HINT`(입력 표가 없고 실행 중이 아닐 때, `UpdateInputTableVisibility`). 입력 표 업무 생성의 상태 카드 건수 = 체크한 행 수(`GetCheckedRowCount`) — 지금은 결과 행 수. 실행 요청의 선택 행 번호(`BuildSelectedRowNums`) · `validateSelectedRows`
   - **T08에서 미룬 판정** — T08은 사용자가 화면을 볼 수 없어 규격 없는 상태를 정하지 않았다. 헤더 · 체크 상자 · 스크롤바의 hover · focus · disabled 규격과 스크롤바를 Fusion 기본으로 둘지를 세 OS 스크린샷으로 사용자와 정한다 (`sageqt-ui/references/style-scope.md` *미정*)
   - 검색 · 필터 기준 적용 방식 (`SageResultTablePanel.cpp`의 `RefreshRows` · `GetEffectiveCriteria` · `GetDefaultCriteria`): 필터 기준이 없을 때 어느 열을 검색하는지, 대소문자 · 부분 일치
   - 행 강조 범위(`nHighlightStart` · `nHighlightCount`)의 의미
   - 체크된 행 번호 문자열 형식 (`GetCheckedRowNums` · `RestoreCheckedRowNums`) — payload의 `rowNums`로 핸들러에 전달된다 (T14)
   - 요약 막대 · 합계 막대 · 선택 막대의 표시 규칙

## SageSDI에서 옮길 것
원본: `D:/Projects/SageSDI/SageSDI/app/ui/panels/SageResultTablePanel.h/.cpp`

**공개 동작** (`.h`)
| 영역 | 동작 |
|---|---|
| 표시 | `SetTitle` · `ShowSelectAll` · `ShowFilter` · `EnableSelectionControls` |
| 스키마 | `SetColumns(columns, style)` · `SetFilterCriteria(criteria)` |
| 행 | `SetRows` · `ClearRows` · `BeginBatchUpdate` / `EndBatchUpdate` · `GetVisibleRows` |
| 요약 · 합계 | `SetSummaryItems` · `ClearSummary` · `SetTotalCells` · `ClearTotals` |
| 체크 | `GetRowCount` · `GetCheckedRowCount` · `IsRowChecked` · `SetRowChecked` · `GetCheckedRowNums` · `RestoreCheckedRowNums` |
| 필터 | `GetFilterKeyword` · `GetFilterCriteria` · `RestoreFilter(keyword, criteria)` |
| 알림 | 표 상태 변경 · 선택 변경을 부모에 알림 (`NotifyStateChanged` · `NotifySelectionChanged`) |

**구성 요소** — 제목(`SageSectionLabel`) · 선택 막대(`SageSelectionBar`) · 검색창(`SageSearchBox`, Enter로 검색) · 초기화 버튼(`"초기화"`) · 요약 막대(`SageSummaryBar`) · 합계 막대(`SageTableTotalBar`) · 헤더(`SageHeaderCtrl`) · 목록(`SageListCtrl`, 가상 목록 `LVN_GETDISPINFO`)

**열** — 핸들러가 준다 (T03). 범용 열: `항목` 140 · `값` 최소 220(늘어남) · `상태` 110 · `사유` 320, 모두 가운데 정렬 (`SageDefine.h:221-224`) — 폭은 `sageqt-ui`가 정한 디자인 값으로

**결과 스타일** — 체크박스 사용 · 격자선 · 강조 행 범위 (`SageWorkflowResultStyle`, T03)

**검색창 문구** — `"검색어 입력"` (`SAGE_UI_RESULT_FILTER_PLACEHOLDER`)

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| 패널이 행 사본을 보관 (`m_arrRows` · `m_arrVisibleRows`) | model이 유일한 보관처, 보이는 행은 proxy (`model-view.md`) |
| 열 폭 계산 (`UpdateColumnWidths` · core의 `DistributeColumnWidths`) | `QHeaderView` 크기 조정 모드 |
| 가상 목록 콜백 (`OnListGetDispInfo`) | model의 `data()` |
| `PreTranslateMessage`의 Enter 처리 | `QLineEdit::returnPressed` |
| 커스텀 헤더 · 목록 그리기 (`SageHeaderCtrl` · `SageListCtrl`) | `QHeaderView` · `QTableView` + `SageStyle` · delegate (`sageqt-ui` 분류) |
| 부모에 `PostMessage`로 알림 | signal |

## 함정
- **"보이는 행"은 proxy 기준이다.** 요약 · 합계를 계산하는 핸들러(`BuildResultSummary` · `BuildResultTotals`)는 SageSDI에서 **보이는 행**을 받는다 — proxy를 통해 매핑한 원본 행을 넘긴다
- **체크 상태의 행 번호는 원본(model) 기준인지 보이는 행 기준인지** 원문으로 확인하고 그대로 지킨다. 필터가 걸린 상태에서 어긋나기 쉽다
- 필터 조건이 바뀌면 proxy의 필터 갱신 API(`beginFilterChange` / `endFilterChange`)로 알린다
- 입력 표와 결과 표는 **같은 패널 클래스의 두 인스턴스**다. 한 인스턴스가 두 탭을 겸하지 않는다
- `BeginBatchUpdate` / `EndBatchUpdate`는 model의 리셋(`beginResetModel` / `endResetModel`)으로 대응한다
- const가 아닌 Qt 컨테이너를 range-for로 돌 때 `std::as_const`

## 작업
PR 4개: `feature/result-table-model` (model · proxy), `feature/result-table-panel` (패널 · delegate · 헤더 · 결과 탭 연결 → 세 OS 스크린샷으로 모양 결정), `feature/result-table-controls` (표 패널 안의 검색창 · 초기화 · 선택 막대 · 요약 · 합계 막대), `feature/input-table` (입력 패널의 입력 표 · 입력 초기화 · 빈 상태 안내 · 작업 영역 연결 · 업무별 상태)
- [x] 결과 행 model (열은 핸들러 정의, 정렬 · 체크는 role)
- [x] 필터 proxy (검색어 · 필터 기준)
- [x] delegate · 헤더 모드
- [ ] 패널: 제목 · 검색 · 초기화 · 선택 막대 · 요약 · 합계
- [ ] 업무별 상태 보존에 필터 · 선택 행 연결 (T13 자리)
- [ ] 테스트: model 행 · 열 · role, proxy 필터 결과, 체크 행 번호 왕복
- [ ] 세 OS 스크린샷

## 완료 기준
- 3-OS CI 통과
- 패널이 행을 복사해 보관하는 멤버가 0개다
- 필터를 건 상태에서 체크한 행 번호가 SageSDI 규칙과 같다 (테스트)
- 결과 표와 입력 표가 같은 클래스의 두 인스턴스다

## 범위 밖
- 실행 기록 — T16

## 확인한 사실
- 사용자 결정 (2026-09-29): (1) 체크 상자 · 검색/필터 · 선택 막대 · 요약 · 합계 막대 · 입력 표를 **전부 옮긴다** — 샘플 업무(두 앱 모두 유일한 업무)는 제목 · 4열 · 행만 쓰므로 나머지는 테스트용 핸들러와 오프스크린 그림으로만 확인한다. (2) **필터가 바뀌면 체크를 모두 푼다** (SageSDI와 같게). (3) 헤더 · 체크 상자 · 스크롤바는 **SageSDI대로(hover · 포커스 표시 없음, 스크롤바 Fusion 기본) 먼저 만들고** 세 OS 스크린샷을 보고 바꿀지 정한다
- 필터 (`RefreshRows`): 검색어는 앞뒤 공백을 자르고 소문자로 바꿔 **부분 일치**. 대상 열은 선택한 기준(`GetEffectiveCriteria` — 목록에 없으면 첫 기준 `GetDefaultCriteria`)의 필드, 기준이 없으면 `값`(`SAGE_RESULT_FIELD_VALUE`). SageQt는 `QString::contains(…, Qt::CaseInsensitive)` — SageSDI `MakeLower`(로캘 소문자)와 한글 · 영문 결과는 같다. 열이 없으면 행도 없다
- 체크: SageSDI는 `RefreshRows`가 목록을 지우고 다시 넣어 **검색 · 기준 변경 · 행 설정마다 체크가 모두 풀린다**. 체크 수 · 행 번호 · 전체 선택은 목록(= 보이는 행)만 센다. 행 번호 = 원본 행의 `m_nSourceRowIndex`(0이면 뺀다), `","`로 잇는다(`SAGE_UI_ROW_NUM_FORMAT` `"%lu"` → `QString::number`). 복원(`RestoreCheckedRowNums`)은 `,`로 자르고 앞뒤 공백을 잘라 비교, **보이는 행에만** 체크한다. 샘플 결과 행은 `m_nSourceRowIndex`가 늘 0이다 (`SageWorkflowResultPresenter.cpp:37`)
- SageQt 대응: 체크는 model(`Qt::CheckStateRole`, 첫 열), 보이는 행 기준 동작(체크 수 · 행 번호 · 복원 · 전체 선택 · 보이는 행 목록)은 proxy(`SageResultFilterProxyModel`). `setFilter`가 체크를 먼저 모두 푼다
- 첫 열 정렬: 결과 표 패널은 늘 `SetFirstColumnAlign(SAGE_LIST_FIRST_COLUMN_CENTER)` — 첫 열은 열 정의와 상관없이 가운데. model의 `Qt::TextAlignmentRole`로 옮겼다
- `SetColumns`는 목록 항목을 모두 지운다 (행은 다음 `SetRows` · `RefreshRows`까지 비어 보인다). SageQt `setColumns`는 행을 비운다
- 업무를 바꿨다 돌아올 때(`RebuildResultTable`) SageSDI는 사용자 결과 표 업무(`UsesCustomResultTable`)만 행을 다시 채운다 — 샘플은 열만 다시 설정돼 표가 빈다. 업무가 샘플 하나라 실제로는 일어나지 않는다 (PR 2에서 확인)
- 표 그리기 (`SageListCtrl.cpp`, 결과 표가 쓰는 부분만 — 배지 열 · 흐린 문구 · 행 상태 색은 실행 기록 T16): 행 높이 34, 교대 행(홀수 행 `LIST_ROW_ALT`), 선택 행 면 `LIST_ROW_SELECTED` + 행 왼쪽 4px `PRIMARY` 막대(가로선 위에 그린다), 가로선은 행 맨 아래 1px `LIST_GRID`(`bGridLines`일 때, 선택 행에도). 포커스 표시 없음. 첫 열은 글자 `TEXT` 고정 · 가운데 · 끝 말줄임, 체크 상자가 있으면 글자 칸을 **양쪽에서** 20씩 줄인다(`DeflateRect`). 강조 열(`nHighlightStart` 부터 `nHighlightCount`개)은 굵은 목록 폰트 + `PRIMARY`, 강조가 있을 때만 `"—"` 칸은 `TEXT_PLACEHOLDER`. 나머지 열의 글자 여백은 Win32 기본값이라 코드에 없다 — `screens.md`의 `SAGE_LIST_CELL_LEFT_PAD` · `RIGHT_PAD`(6, 합계 막대가 표 글자에 맞추려고 쓰는 값)를 썼다
- 체크 상자: 칸 = 행 왼쪽 +4(선택 막대) +2 부터 +20 까지(`SAGE_LIST_CHECK_IMAGE_WIDTH` — CImageList 폭이지만 체크 칸 폭이라 `SAGE_LIST_CHECK_CELL_WIDTH`로 옮겼다), 상자 14 가운데. 해제: 면 `PANEL` · 1px `BUTTON_BORDER`. 체크: 면 `PRIMARY` · 흰 체크 선 2 (`SageUiStyle::DrawCheckBox`). 상자 칸을 누르거나 Space로 바꾼다
- 헤더 (`SageHeaderCtrl.cpp`): 높이 36, 면 `LIST_HEADER`, 글자 `TEXT_MUTED` · 목록 폰트 · 가운데 · 말줄임, 구분선 · 아래선 없음. 누를 수 없게(`setSectionsClickable(false)`) 해 hover도 없다
- 제목 (`SageResultTablePanel`): 섹션 제목과 같은 그림(면 `LIST_HEADER` · 아래 1px `BORDER` · 왼쪽 여백 16)이지만 폰트는 **본문**(`SAGE_FONT_CONTENT`) — 입력 카드 제목은 `SAGE_FONT_HEADER`. `SageLabel` 변형 `TableTitle`로 옮겼다. 위치: 위 12(`SAGE_RESULT_FILTER_TOP_LIFT` 8 + `BOX_PAD` 4), 높이 26, 표는 바로 아래(간격 없음)
- 표 테두리: `WS_BORDER`라 색은 Windows가 정한다 → 메시지 상자와 같이 1px `SAGE_COLOR_BORDER`(`QFrame::Box`). **`QTableView`의 기본 그림자가 Sunken이라 Box 테두리가 2px이 됐다** → `Plain` (픽셀 테스트로 발견)
- 열 폭: 늘어나지 않는 열은 정의 폭 고정, 늘어나는 열이 나머지를 받는다. **차이 → 사용자 결정 (2026-10-01): SageSDI처럼 가로 스크롤** — 패널이 viewport 크기 변경 때 `DistributeColumnWidths` 규칙으로 폭을 준다 (`model-view.md` 예외 추가): SageSDI는 남는 폭이 늘어나는 열의 정의 폭(값 220)보다 작으면 모든 열을 정의 폭으로 두고 가로 스크롤한다(`DistributeColumnWidths`). `QHeaderView::Stretch`는 헤더 전체의 최소 폭(28)까지 줄어든다. 헤더 최소 폭을 220으로 주면 고정 열(140 · 110)까지 220이 된다(테스트로 확인). 두 앱 모두 창 최소 폭이 없어 실제로 좁아질 수 있다
- 열 설정 시점: SageSDI는 업무를 고를 때마다(`CSageSDIView::OnWorkflowChanged` → `RebuildResultTable` → `ApplyResultTableSchema`) 열을 설정한다 — **실행 전 빈 표에도 헤더가 보인다**. 첫 CI 스크린샷에서 SageQt 빈 표에 헤더가 없어 발견, `showWorkflow`에서 열을 설정하게 고쳤다. 열을 묻는 작업 종류는 컨트롤러의 마지막 작업 종류이고, SageSDI 기본값은 `0`(없음 — `SageWorkflowController.cpp:70`). SageQt `SageTaskType`에는 없음 값이 없어 **실행 전에는 `Generate`로 묻는다 (가정 — 샘플은 작업 종류와 무관하게 같은 열)**. 사용자 결과 표 업무의 행 다시 채우기는 PR 3에서
- CI 캡처: 결과 탭 단계 추가(`6-result-tab.png`). 앱 배경색 띠(`workArea`)는 상태 카드가 생긴 뒤 카드 아래 영역이 잡혀 탭 줄 위치 계산에 못 쓴다 → 인디케이터를 "높이 3px 이하 카멜 띠"로 찾아 탭 폭을 잰다. 기존 `closedByEnter` · `loginClosedByEscape`가 `false`인 것은 캡션 색(`LIST_HEADER`)과 같은 섹션 제목 · 헤더 띠를 캡션으로 잘못 찾기 때문이다 (T14 결과에도 같다 — 화면으로는 닫혔다)
- 모양 결정 (2026-10-01, 세 OS 결과 탭 스크린샷 + 오프스크린 표를 보고): 헤더 hover 없음 · 스크롤바 Fusion 기본은 그대로, **마우스를 올린 행에 `#F8F1E6`(`SAGE_COLOR_LIST_ROW_HOVER`)** — 후보 A `#F2EEE7` · B `#F8F1E6` · C `#EDE8E0` 중 B. Qt는 마우스가 올라간 칸 하나에만 `State_MouseOver`를 준다(테스트로 확인) → 패널이 `entered` 신호로 행 번호를 delegate에 알리고, viewport를 떠나거나 행이 바뀌면(리셋 · 필터) 해제한다. viewport를 떠날 때 해제되는지는 오프스크린에서 `Leave` 이벤트를 만들 수 없어 테스트하지 않았다
- 표 패널 안 띠 (`LayoutBandRow` · `Layout` · `LayoutTableArea`): 위 12(`TOP_LIFT` 8 + `BOX_PAD` 4)에서 버튼류는 2 위(`SAGE_BUTTON_VERT_ADJUST`) — 선택 막대 · 요약 막대 · 초기화 · 검색창은 위 10, 높이 32. 왼쪽은 선택 막대(보이면) > 요약 막대(항목이 있으면) > 제목 순으로 하나만. 오른쪽 끝에 [초기화 84][8][검색창 92+150+32], 왼쪽 띠와 사이 10(`SAGE_ROW_GAP`). 표 위치 = max(12+26, 띠 아래 + 10) — 선택 · 요약 막대가 있으면 52, 없으면 38. **차이**: 제목과 검색창이 함께 보이면 SageSDI는 표를 38에 두어 검색창 아래 4px이 표에 가려진다 — SageQt는 레이아웃이라 표가 42에서 시작한다 (실제 업무에는 없는 조합)
- 검색창 (`SageSearchBox.cpp`): 면 `PANEL`, 테두리 1px `BUTTON_BORDER`, 왼쪽 기준 칸 92(면 `APP_BACKGROUND`, 오른쪽 1px `LIST_HEADER_BORDER`), 오른쪽 아이콘 칸 32(같은 면 · 왼쪽 1px 선, 돋보기 `TEXT_MUTED` 반지름 5 · 손잡이 4 · 선 2). 입력칸은 테두리 없이 기준 칸 뒤 +1+10, 아이콘 칸 앞 10. 아이콘 칸을 누르거나 Enter면 검색, 다른 곳을 누르면 입력칸에 포커스. 검색어는 최대 20자, 안내문 「검색어 입력」. 기준 콤보(`SageFilterComboBox.cpp`): 필드 면 `APP_BACKGROUND`(검색창에서 지정), 글자 가운데, 화살표 `PRIMARY` 삼각형(±4 · −2 · +3), 펼친 목록은 항목 높이 24(32−2−6) · 선택 항목 `PRIMARY` 면 + 흰 글자 · 8행. **화살표 칸 폭은 Windows 시스템 값이라 코드에 없다** → Fusion 기본
- 검색 동작: 검색 · 초기화 · 기준 변경은 적용된 검색어(입력 중인 글자가 아니다)와 기준으로 다시 거르고 부모에 알린다(`NotifyStateChanged` → `filterChanged`). 기준 목록과 고른 기준은 proxy가 보관한다(`model-view.md` — 패널이 필터 기준을 보관하지 않는다). 고른 기준이 목록에 없으면 첫 기준, 목록이 비면 `값` 열
- 선택 막대 (`SageSelectionBar.cpp`): [체크 상자 + 「전체 선택」][12][「N건 중」 `SECONDARY` · 6 · 「N건」 본문 강조 `PRIMARY` · 6 · 「선택됨」 `SECONDARY`][12][「선택 해제」 Ghost, 폭 = 글자 + 12×2]. 「전체 선택」은 모두 체크돼 있으면 모두 풀고, 아니면 모두 체크. 체크 상자 표시는 「보이는 행이 모두 체크」일 때만. **SageSDI 체크 상자는 Windows 기본 체크 상자(`BS_AUTOCHECKBOX`)라 규격이 없다** → 표 체크 상자와 같은 그림(14 · `PE_IndicatorCheckBox`, 글자와 사이 6 = `SAGE_SELECTION_CHECK_GLYPH_WIDTH` 20 − 14). **「선택 해제」 Ghost 면: SageSDI는 `SetSurfaceColor`를 부르지 않아 흰색 사각형** — SageQt Ghost는 놓인 면(팔레트 `Window` = 앱 배경)을 칠한다. **사용자 결정 (2026-10-01): 배경과 같게, 체크 상자도 표와 같게**
- 요약 막대 (`SageSummaryBar.cpp`): 항목 = 라벨(캡션 `SECONDARY`) 6 값(요약 폰트, 강조면 `PRIMARY`) 6 단위(캡션 `SECONDARY`), 항목 사이 16 + 1px `BORDER` 세로선(높이 16) + 16. 폭을 넘는 항목부터 그리지 않는다. 배지 항목은 첫 번째 것만 그린다(「라벨 값단위」, 20 높이 · 반경 `SAGE_BADGE_RADIUS` 4를 `RoundRect` 지름으로 → Qt 반지름 2 · 면 `INLINE_WARN_BG` · 선 `INLINE_WARN_BORDER` · 글자 `WARNING`), 나머지 배지 항목은 자리만 차지 — SageSDI 그대로
- 합계 막대 (`SageTableTotalBar.cpp`): 표 바로 아래 40, 면 `LIST_HEADER` · 위 1px `BORDER`. 칸은 열 위치 · 폭에 맞춘다 — 왼쪽 정렬 +6 · 오른쪽 정렬 −6. 건수는 굵은 목록 폰트(SemiBold) `SECONDARY`, 나머지 Bold: 라벨 `TEXT_MUTED` · 금액 `TEXT` · 강조 금액 `PRIMARY`. SageSDI는 열 폭이 바뀔 때만 칸을 다시 맞춘다 — SageQt는 가로 스크롤 때도 맞춘다
- 초기화 버튼: Ghost + 되돌리기 아이콘(반지름 6 · 화살 3 · 선 2, 아이콘과 글자 사이 `SAGE_ICON_TEXT_GAP` 6), 놓인 면 `APP_BACKGROUND`(`SetSurfaceColor`), 폭 84
- 테스트 앱 폰트: 표 패널 테스트가 `main.cpp`처럼 앱 폰트를 본문으로 두지 않아 버튼 · 콤보 글자가 굵게 그려졌다 → `initTestCase`에서 맞춤 (오프스크린 그림으로 발견)

