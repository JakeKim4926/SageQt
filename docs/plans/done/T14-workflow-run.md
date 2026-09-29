# T14 — 실행 흐름 · 진행 표시

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 실행 흐름 — 실행 버튼 → 백그라운드에서 핸들러 실행 → 결과 표시 — 과 상태 카드 · 진행 표시를 옮긴다. MFC 워커 스레드 + `PostMessage`를 `QtConcurrent::run` + `QFutureWatcher`로 바꾼다.

## 시작 전에
1. 선행 주제: 없음 (T13 완료 — 작업 영역 탭 · 입력 카드 · 드롭이 있다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/threads-and-db.md` (결과 전달 구조 · 종료 대기), `coding-rules/references/ownership-and-threads.md` (스레드 금지 사항 · 람다 connect 규약 4), `coding-design/references/ui-composition.md` (컨트롤러는 위젯 API를 부르지 않는다)
3. 결정 대기 — 사용자에게 확정받는다
   - **진행률 방식**: SageSDI의 진행률은 실제 진행이 아니라 **시간 기반 표시**다 (아래). 권장 — 이관할 때는 그대로 옮긴다 (패널의 `QTimer`). 실제 진행률은 핸들러 인터페이스를 바꿔야 하는 별도 기능이다. 이관과 섞으면 동작이 바뀐 원인을 가려낼 수 없다
4. 재확인할 사실
   - **T13에서 넘긴 것** — 실행 줄(실행 `Primary` · 입력 초기화 `Ghost`, 높이 `SAGE_CARD_ACTION_BUTTON_HEIGHT`)을 입력 카드 아래쪽에 넣는다 (`sageqt-ui/references/screens.md` *입력 패널*). 입력 경로 · 저장 폴더 검증 메시지(`SAGE_UI_INPUT_REQUIRED` · `SAGE_UI_OUTPUT_REQUIRED`). 드롭 · 파일 선택 뒤 입력 표 업무면 불러오기 자동 실행(`hasInputTable()`), 실행 중 드롭 무시
   - **T09 · T10 · T13에서 미룬 사용자 확인** — 캡션 닫기 버튼 흰 사각형 · 96 DPI 1:1 크기를 SageSDI와 대조, 실제 OS에서 파일 끌어 놓기 · 파일/폴더 선택 창 (`DEBT_LOG.md`)
   - `SageWorkflowController::Finish` · `CaptureResult` · `RestoreResult`의 나머지 본문 (`:103-`)
   - 로그인이 필요한 업무를 로그인 없이 실행할 때의 처리 위치 (사이드바에서 막히는지, 실행 시점에도 확인하는지)
   - 상태 카드(`drawing/SageStatusCard`)의 상태 종류와 "폴더 열기" 동작 (`SAGE_UI_STATUS_CARD_OPEN_FOLDER`, `SAGE_UI_OUTPUT_PATH_MISSING`)

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/ui/`

**컨트롤러** — `workflow/SageWorkflowController.h/.cpp`
- 공개 동작: `IsRunning` · `Start(request, strError)` · `Finish(workflowType, taskType, responseJson, bSuccess, bKeepResult)` · 마지막 결과 조회 · `GetRunningInputPath` · `ClearResult` · `CaptureResult` · `RestoreResult`
- 실행 요청: 업무 · 작업 종류 · 입력 경로 · 저장 폴더 · 선택 행 번호
- 결과 상태(업무별 보존용, T13): 업무 · 작업 종류 · 성공 여부 · 응답 JSON · 입력 경로

**시작** (`Start`, `:75-97`)
- 이미 실행 중이면 → `SAGE_UI_WORKFLOW_ALREADY_RUNNING`, 거절
- 시작 실패 → `SAGE_UI_WORKFLOW_START_FAILED`
- 성공하면 실행 중 입력 경로를 기억하고 실행 중 상태로

**백그라운드 작업** (`RunWorkflowWorker`, `:21-58`), 순서 그대로
1. payload 생성 (`BuildWorkflowPayload`): `inputPath`는 항상, `outputFolder`는 비어 있지 않을 때만, `rowNums`는 비어 있지 않을 때만
2. 핸들러가 없으면 → 오류 응답 (요청 ID `SAGE_REQUEST_UNKNOWN`, 코드 `SAGE_ERROR_CODE_WORKFLOW_NOT_FOUND`, 메시지 `SAGE_UI_WORKFLOW_NOT_FOUND`)
3. 핸들러 `RunTask(작업 종류, payload)` 실행. 예외가 나면 → 오류 응답 (핸들러의 요청 ID, 코드 `SAGE_ERROR_CODE_WORKFLOW_EXCEPTION`, 메시지 `SAGE_UI_WORKFLOW_EXCEPTION` = "작업 처리 중 예기치 못한 오류가 발생했습니다.")
4. 결과(업무 · 작업 종류 · 응답 JSON)를 UI 스레드로 전달

**진행 표시** — `panels/SageWorkflowInputPanel.cpp:315-321` · `OnTimer`
- 실행 시작: 상태 카드를 실행 중(`SAGE_UI_STATUS_CARD_RUNNING`)으로, 진행률 0, 타이머 시작
- 타이머 300ms마다 (`SAGE_PROGRESS_TIMER_MS = 300`): 실행 중이고 진행률이 95 미만이면 +3 (`SAGE_PROGRESS_STEP = 3`), 95를 넘지 않게 (`SAGE_PROGRESS_RUNNING_MAX = 95`)
- 완료: 100 (`SAGE_PROGRESS_COMPLETE = 100`), 타이머 정지
- `SageDefine.h:53-56`

**완료 안내** — 핸들러의 `FindGenerateCompletedMessage()` (샘플: "샘플 업무가 완료되었습니다.")를 정보 메시지로

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `AfxBeginThread` · `new SageWorkflowTask` · `PostMessage(WM_SAGE_WORKFLOW_COMPLETE)` · `delete` 쌍 | `QtConcurrent::run` + `QFutureWatcher`, 요청 · 결과는 값 |
| `::IsWindow(hWnd)`로 수신 창 생존 확인 | 컨트롤러가 `QFutureWatcher`를 부모로 소유 — 컨트롤러가 먼저 사라지면 결과는 버려진다 (`threads-and-db.md`) |
| `SetTimer(ID_SAGE_PROGRESS_TIMER)` · 타이머 ID | `QTimer` |
| `BuildWorkflowPayload`의 문자열 이어 붙이기 JSON | `QJsonObject` |

## 함정
- **payload의 선택적 키를 그대로 지킨다.** 비어 있는 `outputFolder`를 빈 문자열로 넣으면 샘플 핸들러의 "비면 입력 폴더 사용" 분기가 달라질 수 있다 — 키를 넣지 않는다. 테스트로 고정한다
- 핸들러는 UI 스레드가 아닌 곳에서 실행된다 → 핸들러 메서드는 상태를 바꾸지 않아야 한다 (`const`). DB가 필요한 핸들러는 작업 안에서 연결을 연다 (`threads-and-db.md`)
- 예외는 작업 안에서 잡아 결과 값으로 바꾼다 — `QFuture`로 새어 나가게 두지 않는다
- 람다 `connect`에는 수신 객체를 넘긴다 (규약 4). 가능하면 멤버 slot
- 앱 종료 시 실행 중인 작업이 끝나기를 기다리는 것은 `main.cpp` 몫이다 (T06에서 이미 구현) — 이 주제에서 확인만 한다
- 진행률 결정이 "실제 진행률"로 나면 핸들러 인터페이스가 바뀐다 → T03 결과물 수정, `sageqt-plan` 절차 5(재점검)

## 작업
PR 1~2개: `feature/workflow-controller`, `feature/status-card`
- [x] 컨트롤러: 시작 · 백그라운드 작업 · 결과 전달 · 결과 상태 보존
- [x] payload 생성 (선택적 키 규칙)
- [x] 상태 카드 · 진행 표시 (결정에 따라)
- [x] 완료 안내 · 오류 안내
- [x] 테스트: payload 키 규칙, 핸들러 없음 · 예외 → 오류 응답
- [x] 세 OS에서 샘플 업무 실행 → 결과 탭 (결과 표는 T15 전이면 응답 확인만)

## 완료 기준
- 3-OS CI 통과
- payload 테스트: 빈 저장 폴더 · 빈 행 번호일 때 키가 없다
- 실행 중 UI가 멈추지 않는다, 실행 중 재실행은 거절된다
- `new` · `delete` · `PostMessage` · 창 핸들이 이 기능 코드에 0개다

## 범위 밖
- 결과 표 — T15
- 실제 진행률 — 결정에 따라 별도 주제

## 확인한 사실
- `Finish(…, bKeepResult)`: 실행 중 해제 · 마지막 업무 · 성공 여부는 늘 바꾸고, `bKeepResult`면 작업 종류 · 응답을 그대로 둔다. 작업 영역은 입력 표 업무의 생성(Generate)일 때 `bKeepResult`를 켠다 (`DisplayResponse`)
- `CaptureResult` · `RestoreResult`는 업무 · 작업 종류 · 성공 여부 · 응답 · 실행 입력 경로를 통째로 복사한다. SageQt는 `resultState()` 하나(값 복사 = Capture)와 `restoreResult` · `clearResult`로 옮겼다. 결과 상태는 `SageWorkspaceState::m_result`에 업무별로 보존된다
- 로그인 확인은 실행 시점에 없다 — 사이드바에서만 막는다. SageQt도 실행 시점에 확인하지 않는다
- 실행 중에 다른 업무로 바꾸는 것을 SageSDI는 막지 않는다 (결과는 끝난 뒤 그때 보이는 업무 화면에 적용된다). 업무가 샘플 하나뿐이라 그대로 옮겼다
- 입력 경로 · 저장 폴더는 앞뒤 공백을 잘라 검증한다. 저장 폴더는 생성(Generate)에만 요구한다
- 결과 탭 선택: 입력 표 업무면 입력 탭, 아니면 문서 결과 탭. 완료 안내는 생성 성공일 때만
- 실행 버튼은 폼 두 번째 열(입력칸 열) 왼쪽, 입력칸 두 줄 아래 `SAGE_CARD_ROW_GAP` 간격 (`LayoutActionSection`). 입력 초기화 버튼은 입력 표 업무에만 보이므로 T15로 넘긴다
- **옮기지 않은 것**: `SAGE_UI_WORKFLOW_START_FAILED` — `QtConcurrent::run`은 시작 시점에 실패를 돌려주지 않는다 (스레드 풀 대기열에 넣는다). `SAGE_REQUEST_UNKNOWN` 값은 `"mfc-unknown"` → `"sageqt-unknown"`
- 결과 표 채우기(T15) · 실행 기록 추가(T16)는 `onRunFinished`에 아직 없다
- 사용자 결정 (2026-09-29): 진행률은 SageSDI처럼 시간 기반. 「폴더 열기」는 저장 경로가 파일이면 그 파일이 든 폴더를 연다 — SageSDI의 `explorer.exe /select,"경로"`(파일 선택된 탐색기)는 Windows 전용이라 `QDesktopServices::openUrl`(폴더)로 바꿨다. 경로가 없으면 `SAGE_UI_OUTPUT_PATH_MISSING` 경고
- 진행률: `SAGE_PROGRESS_COMPLETE`(100)는 SageSDI에서 진행 막대 채움 비율의 분모로만 쓰인다 (`SageStatusCard.cpp:202`). 끝날 때 100으로 올리는 코드는 없다 — 결과 상태로 바뀌며 막대가 사라진다. 이 파일 위의 "완료: 100" 설명은 코드와 다르다. SageQt는 `QProgressBar` 범위 최대값으로 쓴다
- 타이머: 실행 중이고 95 미만이면 +3, 95를 넘으면 95로 자른다 (`SageWorkflowInputPanel.cpp:372-383`). 실행이 끝나면 타이머만 멈춘다
- 상태 카드 결과 문구는 SageSDI의 `JsonExtractString`(JSON 문자열에서 키를 처음 찾는 곳의 값)으로 읽는다. SageQt는 응답 구조로 읽는다: 실패 사유 = `error.message`, 없으면 `error.code` / 저장 경로 = `payload.filePath`, 없으면 `payload.outputFolder`. 응답에 그 키가 다른 곳에 없어서 결과는 같다
- 결과 건수 = 결과 행 수. 입력 표 업무의 생성은 입력 표에서 체크한 행 수(`GetCheckedRowCount`)를 쓴다 — 입력 표가 T15라 지금은 결과 행 수 (T15로 넘김)
- 상태 카드는 업무를 바꿔도 그대로 남는다 (`RestoreWorkflowState`가 상태 카드를 건드리지 않는다). 입력 초기화(`ResetInput`)에서만 대기로 돌아간다 — T15
- 저장 경로 말줄임: `DT_PATH_ELLIPSIS`는 마지막 `\` 뒤(파일 이름)를 최대한 남긴다 (Microsoft `DrawText` 문서). SageQt는 파일 이름을 통째로 남기고 앞 폴더 부분만 가운데를 줄인다(`Qt::ElideMiddle`). 파일 이름만으로도 넘치면 전체를 가운데 줄임 — 이 경우의 Windows 동작은 재지 않았다
- 완료 안내 창이 떠 있는 동안 SageSDI는 상태 카드를 아직 "처리 중"으로 둔다 (`DisplayResponse`에서 안내 창 뒤에 상태 카드를 바꾼다). 순서를 그대로 옮겼다
- 입력 패널의 빈 상태 안내(`SAGE_UI_EMPTY_STATE_HINT`)는 입력 표 자리에 뜬다 (`UpdateInputTableVisibility`, 입력 표가 없고 실행 중이 아닐 때) — T15로 넘김. 상태 표시줄 문구(`NotifyStatus`)는 T16
- 스크린샷에 없는 처리 중 · 완료 · 실패 카드는 오프스크린으로 그려 확인했다 (색 · 막대 · 버튼 위치는 `SageStatusCardTest`가 픽셀로 고정)

## 결과
- PR 없이 두 브랜치로 `develop`에 squash merge: `feature/workflow-controller`(실행 흐름), `feature/status-card`(상태 카드 · 진행 표시 · 폴더 열기)
- core `SageWorkflowRunner`(payload 선택 키, 핸들러 없음 · 예외 → 오류 응답), ui `SageWorkflowController`(`QtConcurrent::run` + `QFutureWatcher`), `SageStatusCard`(4 상태, 진행 막대 `QProgressBar` + `SageStyle`), 입력 카드 실행 버튼 · 진행 타이머, 작업 영역 실행 · 결과 · 폴더 열기
- 완료 기준 확인: 3-OS CI 통과 / payload 테스트(`SageWorkflowRunnerTest`) / 실행 중 UI가 멈추지 않음(핸들러가 막혀 있는 동안 클릭이 돌아온다) · 재실행 거절(`SageWorkflowControllerTest`, 작업 영역 테스트) / `delete` · `PostMessage` · 창 핸들 0개, `new`는 부모를 받는 `QObject`뿐 (규약 1)
- 세 OS에서 샘플 업무 실행 → 결과 탭 · 완료 안내는 `SageWorkspacePanelTest`가 세 OS CI에서 확인한다
- 스크린샷 `docs/screenshots/T14/`: 세 OS 작업 영역(대기 카드, 실행 36534805326) + 오프스크린 카드 4 상태(Windows)
- 교훈
  - Qt 모듈 헤더(`<QtConcurrent>`)는 clazy `no-module-include`에 걸린다 — 클래스 헤더(`<QtConcurrentRun>`)를 쓴다
  - 오프스크린 `grab()`은 레이아웃 요청이 처리되기 전 배치를 그릴 수 있다 — 상태를 바꾼 직후 그림을 볼 때는 이벤트를 먼저 돌린다
