# T14 — 실행 흐름 · 진행 표시

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 실행 흐름 — 실행 버튼 → 백그라운드에서 핸들러 실행 → 결과 표시 — 과 상태 카드 · 진행 표시를 옮긴다. MFC 워커 스레드 + `PostMessage`를 `QtConcurrent::run` + `QFutureWatcher`로 바꾼다.

## 시작 전에
1. 선행 주제: T13
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/threads-and-db.md` (결과 전달 구조 · 종료 대기), `coding-rules/references/ownership-and-threads.md` (스레드 금지 사항 · 람다 connect 규약 4), `coding-design/references/ui-composition.md` (컨트롤러는 위젯 API를 부르지 않는다)
3. 결정 대기 — 사용자에게 확정받는다
   - **진행률 방식**: SageSDI의 진행률은 실제 진행이 아니라 **시간 기반 표시**다 (아래). 권장 — 이관할 때는 그대로 옮긴다 (패널의 `QTimer`). 실제 진행률은 핸들러 인터페이스를 바꿔야 하는 별도 기능이다. 이관과 섞으면 동작이 바뀐 원인을 가려낼 수 없다
4. 재확인할 사실
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
- [ ] 컨트롤러: 시작 · 백그라운드 작업 · 결과 전달 · 결과 상태 보존
- [ ] payload 생성 (선택적 키 규칙)
- [ ] 상태 카드 · 진행 표시 (결정에 따라)
- [ ] 완료 안내 · 오류 안내
- [ ] 테스트: payload 키 규칙, 핸들러 없음 · 예외 → 오류 응답
- [ ] 세 OS에서 샘플 업무 실행 → 결과 탭 (결과 표는 T15 전이면 응답 확인만)

## 완료 기준
- 3-OS CI 통과
- payload 테스트: 빈 저장 폴더 · 빈 행 번호일 때 키가 없다
- 실행 중 UI가 멈추지 않는다, 실행 중 재실행은 거절된다
- `new` · `delete` · `PostMessage` · 창 핸들이 이 기능 코드에 0개다

## 범위 밖
- 결과 표 — T15
- 실제 진행률 — 결정에 따라 별도 주제

## 확인한 사실
(진행 중 기록)
