# T16 — 실행 기록 · 상태 표시줄

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 실행 기록 탭과 메인 창 상태 표시줄을 옮긴다. 이 주제가 끝나면 샘플 업무가 입력 → 실행 → 결과 → 기록까지 세 OS에서 동작한다 (UI 이관 완료).

## 시작 전에
1. 선행 주제: T15
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/model-view.md` · `ui-composition.md` (창의 앱 수준 연결), `coding-rules/references/api-shape.md`
3. 결정 — 착수 시 사용자와 확정
   - 상태 표시줄의 키보드 표시기 (아래): MFC 마법사 기본값이다. 권장 — 옮기지 않고 상태 메시지만
4. 재확인할 사실
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
- [ ] 실행 기록 model · 패널 · 필터 · 빈 상태
- [ ] 상태 표시줄과 상태 메시지 연결
- [ ] 샘플 업무 전 구간 확인 (세 OS): 사이드바 → 입력 → 실행 → 결과 → 기록, 스크린샷

## 완료 기준
- 3-OS CI 통과
- 세 OS에서 샘플 업무 전 구간이 동작한다 (Mac mini 포함, 확인 표)
- 상태 문자열 5종이 위 시점에 표시된다

## 범위 밖
- 배포 — T17 이후

## 확인한 사실
(진행 중 기록)
