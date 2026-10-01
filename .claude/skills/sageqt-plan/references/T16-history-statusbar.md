# T16 — 실행 기록 · 상태 표시줄

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 실행 기록 탭과 메인 창 상태 표시줄을 옮긴다. 이 주제가 끝나면 샘플 업무가 입력 → 실행 → 결과 → 기록까지 세 OS에서 동작한다 (UI 이관 완료).

## 시작 전에
1. 선행 주제: 없음 (T15 완료 — 결과 표 패널 · delegate · 표 규칙이 있다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/model-view.md` · `ui-composition.md` (창의 앱 수준 연결), `coding-rules/references/api-shape.md`
3. 결정 — 착수 시 사용자와 확정
   - 상태 표시줄의 키보드 표시기 (아래): MFC 마법사 기본값이다. 권장 — 옮기지 않고 상태 메시지만
4. **T15에서 정한 표 규칙** — 실행 기록 표도 같은 delegate 규칙(행 hover `SAGE_COLOR_LIST_ROW_HOVER`, 헤더 hover 없음, 스크롤바 Fusion 기본)과 열 폭 규칙(최소 폭 아래면 가로 스크롤, `coding-design/references/model-view.md` 예외)을 따를지 확인한다 — 배지 열 · 흐린 문구 · 행 상태 색은 T15 delegate에 없다
5. 재확인할 사실
   - **메인 창 최소 크기** — T11에서 초기 크기만 1280 × 800으로 정했다 (사용자 결정). 모든 패널이 들어온 이 주제에서 내용이 잘리지 않는 최소 크기를 재서 사용자에게 확인한다 (`sageqt-ui/references/screens.md` *메인 창*)
   - `SageWorkflowHistoryPanel.cpp`의 `AppendEntry`: 응답 JSON에서 출력 경로 · 사유를 어떻게 꺼내는지, 시각 형식
   - 기록 필터(`UpdateFilterLabels` · `RebuildVisibleRows`)의 기준, 빈 상태 표시(`UpdateEmptyState`)

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/ui/`

**실행 기록** — `panels/SageWorkflowHistoryPanel.h/.cpp`
- 행: 시각 · 입력 경로 · 출력 경로 · 사유 · 성공 여부 (`SageHistoryRow`, `.h:9-19`)
- 추가: 실행이 끝날 때마다 `AppendEntry(실행 중 입력 경로, 응답 JSON, 성공 여부)` (`SageWorkspacePanel.cpp:542`)
- **메모리에만 보관한다** (`std::vector<SageHistoryRow> m_arrRows`, `.h:56`) — 앱을 끄면 사라진다. 이관은 이 동작을 그대로 옮긴다
- 필터 · 빈 상태 표시가 있다 (재확인)

**상태 표시줄** — `frame/MainFrm.cpp:23-28, 49-54`
- 표시기: 메시지 칸 + `ID_INDICATOR_CAPS` · `NUM` · `SCRL` (MFC 마법사 기본값)
- 메시지 흐름: 작업 영역이 상태를 알림 (`NotifyStatus` → `WM_SAGE_WORKSPACE_STATUS`, `SageWorkspacePanel.cpp:633-637`) → View가 상태 표시줄에 표시 (`SageSDIView.cpp:61, 98`)
- 상태 문자열: 시작 시 `SAGE_UI_READY`, 실행 시작 `SAGE_UI_RUNNING`, 드롭 받음 `SAGE_UI_DROP_RECEIVED`, 끝나면 `SAGE_UI_COMPLETED` / `SAGE_UI_FAILED` (`SageWorkspacePanel.cpp:406, 503, 523, 563`)

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| 상태 표시줄 키보드 표시기 (CAPS · NUM · SCRL) | 마법사 기본값 (결정 대기 — 권장 옮기지 않음) |
| `SendMessage(WM_SAGE_WORKSPACE_STATUS, 문자열 포인터)` | signal (값 전달) |
| 실행 기록을 DB에 저장 | SageSDI에 없는 기능 — 요청 시 별도 주제 |

## 함정
- 상태 메시지 연결: 작업 영역의 상태 signal → `QMainWindow::statusBar()`. 이 연결은 창의 "앱 수준 연결"이다 — 창이 작업 영역 대신 받아 가공하는 중계 slot을 만들지 않는다 (`ui-composition.md`)
- 실행 기록도 표 데이터이므로 model이 보관한다 (`model-view.md`)
- 기록 시각 형식은 `QLocale`로 (직접 포맷하지 않는다)

## 작업
PR 1~2개: `feature/history-panel`, `feature/status-bar`
- [x] 실행 기록 model · 패널 · 필터 · 빈 상태
- [ ] 상태 표시줄과 상태 메시지 연결
- [ ] 샘플 업무 전 구간 확인 (세 OS): 사이드바 → 입력 → 실행 → 결과 → 기록, 스크린샷

## 완료 기준
- 3-OS CI 통과
- 세 OS에서 샘플 업무 전 구간이 동작한다 (Mac mini 포함, 확인 표)
- 상태 문자열 5종이 위 시점에 표시된다

## 범위 밖
- 배포 — T17 이후

## 확인한 사실
- 사용자 결정 (2026-10-01): (1) 상태 표시줄 키보드 표시기(CAPS · NUM · SCRL)는 옮기지 않는다. (2) 기록 시각은 SageSDI처럼 고정 형식 `MM-dd HH:mm:ss`(`SAGE_UI_HISTORY_TIME_FORMAT` `%m-%d %H:%M:%S`) — 이 파일의 "`QLocale`로" 함정은 적용하지 않는다. (3) 기록 표도 결과 표 규칙(행 hover · 헤더 hover 없음 · 스크롤바 Fusion · 좁으면 가로 스크롤)을 따른다. 실패 행 면은 SageSDI대로, hover · 선택이 이긴다
- 행 만들기 (`AppendEntry` · `BuildRow` · `BuildFileRow`): 응답의 `files` 배열이 비면 한 행, 있으면 파일마다 한 행. 새 행은 맨 위에 넣는다. 한 행: 성공이면 저장 경로 = `filePath`, 없으면 `outputFolder`, 둘 다 없으면 「미리보기 (저장 없음)」(흐린 글자 `TEXT_PLACEHOLDER`), 사유 「—」. 실패면 저장 경로 「—」, 사유 = `error.message`, 없으면 `error.code`, 없으면 「—」. 파일 행: 성공 여부 = 파일 `status`(대소문자 무시 `"success"`), 비면 실행 성공 여부. 실패 사유는 `message`만(코드 대체 없음). 입력 경로가 비면 「—». SageSDI는 JSON 문자열 검색(`JsonExtractString` · `JsonExtractArray`)이라 키를 어디서든 처음 찾는다 — SageQt는 `payload`(결과 · `files`) · `error` 아래에서 찾는다. **`files`를 내는 핸들러가 두 앱 모두 없어 위치(`payload.files`)는 가정이다**
- 행 데이터(`SageHistoryEntry`)는 원래 값만 두고, 「—」 · 「미리보기 (저장 없음)」 · 「성공/실패」 표시 문자열은 model이 만든다. 흐린 글자 · 실패 행 면 · 배지 색은 model role(`SageTableRole::Muted` · `RowTone` · `BadgeTone`) — SageSDI의 셀 문자열 비교(`SetMutedText`)를 옮기지 않았다 (`widgets.md`)
- 표 (`SageWorkflowHistoryPanel.cpp` · `SageListCtrl.cpp`): 열 = 실행 시각 124 · 결과 88 고정, 입력 파일 296 · 저장 경로 360 · 사유 208 늘어남(최소 폭 비율로 나눔), 모두 가운데. 가로선 켬, 교대 행, 첫 열 가운데. 결과 열은 배지: 높이 20 · 좌우 8 · 반경 4(`RoundRect` 지름 8), 캡션 폰트, 성공 면 `BADGE_BG_SUCCESS` · 글자 `STATUS_CARD_TEXT_SUCCESS`, 실패 면 `STATUS_BG_ERROR` · 글자 `INLINE_ERROR_TEXT`, 칸 가운데. 실패 행 면 `STATUS_CARD_BG_ERROR`
- 필 바 (`SageFilterPillBar.cpp`): 「전체 N」 「성공 N」 「실패 N」, 높이 28 · 좌우 12 · 사이 8 · 반경 14(지름 28 = 알약), 캡션 폰트. 선택: 면 `ACCENT_SURFACE` · 선 · 글자 `PRIMARY`, 아니면 면 `PANEL` · 선 `BORDER` · 글자 `TEXT_MUTED`. 같은 필을 다시 누르면 아무 일도 없다. 기록이 없으면 숨기고 표가 맨 위로, 있으면 표는 필 바 아래 12(`SAGE_CARD_ROW_GAP`)
- 빈 상태 (`SageEmptyState.cpp`): 면 `PANEL` · 1px `BORDER`. 가운데에 아이콘 상자 44(반경 8, 면 `LIST_HEADER`, 표 모양 아이콘 22 · 선 1 `PRIMARY`) · 12 · 제목(헤더 폰트 · `TEXT` · 높이 22) · 12 · 설명(본문 · `SECONDARY_TEXT`, 최대 폭 420에서 줄바꿈). 기록이 없으면 「아직 실행 기록이 없습니다 / 문서를 생성하면 여기에 쌓입니다」, 필터 결과가 없으면 「조건에 맞는 기록이 없습니다 / 다른 항목을 선택해 보세요」. 동작 버튼(`SetAction`)은 사용처가 없어 옮기지 않았다
- 실행 기록은 업무와 상관없이 하나다 (작업 영역에 패널 하나). 추가 시점은 실행이 끝난 직후, 결과 표 열 설정 뒤 · 행 채우기 앞 (`DisplayResponse`). 입력 경로는 실행을 시작할 때의 입력 경로(`GetRunningInputPath`)
- 공통 표 위젯 `SageTableView`(`QTableView`): 결과 표 · 기록 표가 같은 설정(테두리 · 행 높이 · 헤더 · 선택)과 delegate · 행 hover · 열 폭 규칙을 쓰도록 T15의 결과 표 패널에서 뽑아냈다
