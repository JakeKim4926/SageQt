# T15 — 결과 표

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 결과 표 패널(`SageResultTablePanel` — 입력 표와 결과 표에 같은 클래스를 두 인스턴스로 쓴다)을 Qt Model/View로 옮긴다. SageSDI에서 패널 하나가 들고 있던 행 · 보이는 행 · 검색어 · 필터 기준 · 체크 상태를 model · proxy · header · delegate로 나눈다.

## 시작 전에
1. 선행 주제: T14
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: **`coding-design/references/model-view.md`** (책임 분담표 — 이 주제의 기준), `ui-composition.md` (한 위젯이 두 탭 역할 금지 → 인스턴스 둘), `style.md`, `coding-rules/references/api-shape.md` (컨테이너 · `std::as_const`)
3. 결정 — `sageqt-ui`의 표 규격을 따른다
4. 재확인할 사실 — 착수 시 원문을 읽는다
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
PR 2~3개: `feature/result-table-model` (model · proxy), `feature/result-table-panel` (패널 · delegate · 막대들)
- [ ] 결과 행 model (열은 핸들러 정의, 정렬 · 체크는 role)
- [ ] 필터 proxy (검색어 · 필터 기준)
- [ ] delegate · 헤더 모드
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
(진행 중 기록)
