# Model/View

`coding-design`의 상세 규칙이다. 표를 보여줄 때 읽는다.

## 표 데이터 — Model/View (CRITICAL)

표를 이루는 책임은 model · proxy · header · delegate가 나눠 맡는다.
**패널이 행 · 보이는 행 · 검색어 · 필터 기준 · 체크 상태를 직접 보관하거나 열 폭을 계산하지 않는다.**

| 책임 | 담당 |
|---|---|
| 행 데이터 보관 | `Sage*Model` (`QAbstractTableModel`) — **유일한 보관처** |
| 검색 · 필터 기준 · 정렬 | `QSortFilterProxyModel` (조건이 복잡하면 `filterAcceptsRow` 재정의) |
| 체크 상태 | model의 `Qt::CheckStateRole` |
| 셀 정렬 (왼쪽 · 가운데 · 오른쪽) | model의 `Qt::TextAlignmentRole` |
| 셀 그리기 | `Sage*Delegate` (`QStyledItemDelegate`) |
| 열 폭 | `QHeaderView` 크기 조정 모드. **예외**: 늘어나는 열에 최소 폭이 있으면 패널이 viewport 크기가 바뀔 때 폭을 준다 (아래) |
| 패널 | view · proxy · model을 조립하고, 의미 있는 signal만 밖으로 낸다 |

- 패널은 행을 복사해 두지 않는다. "보이는 행"은 proxy에게 묻는다
- model은 데이터 변환만, 그리기는 delegate가 한다
- **core의 열 정의에는 픽셀을 넣지 않는다.** 라벨 · 정렬 · 늘어나는 열인지 같은 의미만 두고, ui가 그것을 role과 `QHeaderView` 모드로 옮긴다
- 필터 조건이 바뀌면 proxy의 필터 갱신 API(`beginFilterChange` / `endFilterChange`)로 알린다
- **늘어나는 열의 최소 폭 (예외, T15 사용자 결정 2026-10-01)**: `QHeaderView`에는 열마다 최소 폭이 없다 — `Stretch`는 헤더 전체 최소 폭까지 줄고, 헤더 최소 폭을 올리면 고정 열까지 커진다. 그래서 모든 열을 `Fixed`로 두고, 패널이 viewport 크기 변경(`QEvent::Resize`) 때 SageSDI `DistributeColumnWidths` 규칙으로 폭을 준다: 고정 열은 정의 폭, 남는 폭이 늘어나는 열들의 최소 폭 합보다 작으면 모두 최소 폭(가로 스크롤), 크면 최소 폭 비율로 나누고 마지막 늘어나는 열이 나머지를 받는다. 폭 값은 열 정의(의미)를 보고 ui가 정한다
