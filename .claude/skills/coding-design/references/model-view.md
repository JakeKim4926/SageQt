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
| 열 폭 | `QHeaderView` 크기 조정 모드 (늘어나는 열 = `QHeaderView::Stretch`) |
| 패널 | view · proxy · model을 조립하고, 의미 있는 signal만 밖으로 낸다 |

- 패널은 행을 복사해 두지 않는다. "보이는 행"은 proxy에게 묻는다
- model은 데이터 변환만, 그리기는 delegate가 한다
- **core의 열 정의에는 픽셀을 넣지 않는다.** 라벨 · 정렬 · 늘어나는 열인지 같은 의미만 두고, ui가 그것을 role과 `QHeaderView` 모드로 옮긴다
- 필터 조건이 바뀌면 proxy의 필터 갱신 API(`beginFilterChange` / `endFilterChange`)로 알린다
