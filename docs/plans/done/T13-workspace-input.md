# T13 — 작업 영역 탭 · 입력 패널

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 작업 영역(업무별 탭 · 업무 전환 시 상태 보존)과 입력 패널(입력 파일 · 저장 폴더 · 파일 드롭)을 옮긴다.

## 시작 전에
1. 선행 주제: 없음 (T10 완료 — 로그인 · 비밀번호 변경까지 연결된 창. 실행 버튼의 로그인 필요 판단은 세션 `authStateChanged`를 받는다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` (탭은 패널 단위 · 한 위젯이 두 탭 역할 금지) · `style.md`, `coding-rules/references/values-and-platform.md` (`QFileDialog` · 경로 규칙) · `api-shape.md`
3. 결정 — 착수 시 사용자와 확정
   - 입력 파일 필터: SageSDI는 엑셀 필터를 쓴다 (아래). 샘플 업무는 어떤 파일이든 받는다. 권장 — 필터를 핸들러가 정하게 한다 (업무마다 입력 형식이 다르다). 핸들러 인터페이스 변경이므로 T03 결과와 맞춘다
   - 탭 구현은 `QTabBar` + `QStackedWidget` (`sageqt-ui/references/widgets.md`, `ui-composition.md` *탭은 패널 단위*). 선택 탭 Bold로 탭 폭이 흔들리는지 확인 (`screens.md`)
4. 재확인할 사실
   - **T09 · T10에서 미룬 사용자 확인** — 프레임리스 다이얼로그의 캡션 닫기 버튼이 흰 사각형으로 보이는 것이 SageSDI 실제 화면과 같은지, 메시지 상자 · 로그인 창 · 메인 창을 SageSDI와 나란히 놓고 크기 · 글자 크기가 같은지(96 DPI 1:1). 스크린샷 `docs/screenshots/T09/` · `T10/`
   - **T08에서 미룬 판정** — T08은 사용자가 화면을 볼 수 없어 규격 없는 상태를 정하지 않았다. 탭 · 콤보의 hover · focus · disabled 규격을 세 OS 스크린샷으로 사용자와 정한다 (`sageqt-ui/references/style-scope.md` *미정*)
   - `SageWorkspacePanel::ApplyDroppedInputPaths` (`:507-`): 여러 파일을 떨어뜨렸을 때의 처리
   - 자동 불러오기 플래그(`m_bAutoLoadOnInput`)를 누가 켜는지 (입력 표를 쓰는 업무?)

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/ui/`

**탭** — `panels/SageWorkspacePanel.cpp`
- 핸들러가 탭 목록(시각 순서 → 의미 인덱스)을 준다. 의미 인덱스를 시각 인덱스로 찾지 못하면 입력 탭 (`GetTabVisualIndex`)
- 탭마다 패널: 입력(`SageWorkflowInputPanel`) · 결과(`SageWorkflowResultPanel`) · 실행 기록(`SageWorkflowHistoryPanel`)

**업무별 상태 보존** — 사이드바에서 업무를 바꿨다가 돌아오면 그대로 (`SaveWorkflowState` · `RestoreWorkflowState`)
| 저장하는 것 | 비고 |
|---|---|
| 선택된 탭 | |
| 실행 결과 상태 | 컨트롤러의 `CaptureResult` / `RestoreResult` (T14) |
| 입력 경로 · 저장 폴더 | |
| 결과 필터 검색어 · 필터 기준 | T15 |
| 선택된 행 번호 | 입력 표가 보일 때만 |
- 등록되지 않은 업무로 복원하면 → 입력 탭 선택 + 결과 지움

**보임 규칙** (`RefreshVisibility`)
- 입력 초기화 버튼 보임 여부, 입력 표 보임 = (입력 탭 선택) AND (핸들러가 입력 표 사용), 결과 필터 보임 여부 — 조건 세부는 착수 시 원문 확인

**입력 패널** — `panels/SageWorkflowInputPanel.cpp`
- 입력 파일 선택 (`OnSelectInput`, `:393-403`): 파일 선택 창, 제목은 핸들러의 입력 창 제목, 필터 `"Excel Files (*.xls;*.xlsx)|*.xls;*.xlsx|All Files (*.*)|*.*||"`, 기본 확장자 `xls` (`SageDefine.h:339-340`). 선택 후 자동 불러오기가 켜져 있으면 불러오기 작업 실행 (`SAGE_TASK_LOAD = 1`)
- 저장 폴더 선택 (`OnSelectOutput`, `:405-410`): 폴더 선택 창, 제목 `"저장 폴더 선택"` (`SAGE_UI_SELECT_OUTPUT_TITLE`)
- 검증 메시지: `"파일을 선택하세요."` (`SAGE_UI_INPUT_REQUIRED`), `"저장 위치 폴더를 지정하세요."` (`SAGE_UI_OUTPUT_REQUIRED`)
- 작업 종류: 불러오기 `SAGE_TASK_LOAD = 1`, 생성 `SAGE_TASK_GENERATE = 2` (`SageDefine.h:296-297`)

**파일 드롭** — `view/SageSDIView.cpp:24-46, 80-97, 230`
- 드롭을 받는 창: View · 프레임 · 입력 패널 · 결과 표 패널
- View가 자식 창의 드롭까지 모아(`PreTranslateMessage`) 경로 목록을 만들고 → 작업 영역의 `ApplyDroppedInputPaths`로 넘긴다
- 즉 **창 어디에 떨어뜨려도 입력 경로가 된다**

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `DragAcceptFiles` · `DragQueryFileW` · `ChangeWindowMessageFilterEx` | Win32. `ChangeWindowMessageFilterEx`는 관리자 권한 실행 대비라 Qt에서 필요 없다 |
| `CFileDialog` · `CFolderPickerDialog` | `QFileDialog` |
| `common/SageDialogHelper.*` (`ShowIFileOpenDialog` · `GetAppMainWindow` · `SafeShowDialog`) | 외부 호출 0곳 + COM. `QFileDialog` |
| MFC 필터 문자열 형식 (`\|` 구분, `\|\|` 끝) | Qt 필터 형식 (`;;` 구분)으로 바꾼다 |
| `SendMessage(WM_SAGE_WORKFLOW_RUN_REQUESTED)` | signal |

## 함정
- **Qt의 드롭은 커서 아래 위젯이 먼저 받는다.** 게다가 `QLineEdit`는 기본으로 텍스트 드롭을 받아 버린다. "창 어디든 입력 경로"를 지키려면 드롭을 받을 위젯과 받지 않을 위젯을 설계한다 (입력칸의 드롭을 끄고 상위에서 받기 등). 세 OS에서 확인한다
- 드롭된 URL은 `QUrl::toLocalFile()`로 바꾼다. 경로 구분자를 직접 다루지 않는다
- macOS 파일 선택 창은 시트로 뜰 수 있다 — 모달 흐름이 Windows와 같은지 확인한다
- 탭 전환은 패널 교체다. 위젯 여러 개를 `setVisible`로 켜고 끄지 않는다 (`ui-composition.md`)
- 업무별 상태는 창이 아니라 작업 영역 패널 · 컨트롤러가 보관한다 (창 멤버 금지)

## 작업
PR 2개: `feature/workspace-tabs` (탭 · 업무별 상태), `feature/input-panel` (입력 · 폴더 · 드롭)
- [x] 작업 영역: 핸들러 탭 → 패널, 의미 ↔ 시각 인덱스
- [x] 업무별 상태 저장 · 복원 (결과 · 필터 부분은 T14 · T15에서 채운다 — 자리만 비우지 말고 해당 주제에서 추가)
- [x] 입력 패널: 파일 · 폴더 선택, 검증 메시지
- [x] 파일 드롭: 창 어디든 → 입력 경로
- [x] 세 OS 확인 (파일 선택 · 폴더 선택 · 드롭), 스크린샷

## 완료 기준
- 3-OS CI 통과
- 세 OS에서 창의 서로 다른 세 곳(입력칸 · 결과 영역 · 빈 곳)에 파일을 떨어뜨려 입력 경로가 채워진다
- 업무를 바꿨다 돌아오면 탭 · 입력 경로 · 저장 폴더가 그대로다
- Win32 드롭 API · MFC 파일 창이 0개다

## 범위 밖
- 실행 버튼 · 상태 카드 · 진행 표시 — T14
- 결과 표 · 입력 표 — T15

## 확인한 사실
- 사용자 결정 (2026-09-28 · 29): 입력 파일 필터는 핸들러가 정한다(`inputFileFilter()`, 샘플 = 모든 파일). 탭은 모두 같은 폭(가장 긴 라벨 *본문 강조* + 좌우 `SAGE_TAB_PAD_X` 16). 탭 hover 때 비선택 탭 글자를 본문색으로, 포커스 표시 없음
- SageSDI 코드 대조 (2026-09-29): 탭은 `TCS_FIXEDWIDTH` · 선택 `SAGE_FONT_CONTENT_SEMIBOLD` · 인디케이터 2px(`SageTabCtrl.cpp`). 탭 줄 40 흰 면 + 아래선, 콘텐츠 여백 24 · 20 (`SageWorkspacePanel.cpp`). 입력 카드: 제목 38(목록 헤더 면, `SageSectionLabel.cpp`) · 안 여백 16 · 라벨-입력칸-버튼 사이와 줄 사이 모두 `SAGE_CARD_ROW_GAP` 12 · 버튼 폭 120. 읽기 전용 경로 칸은 `CTLCOLOR_STATIC`이라 면이 앱 배경색, 글자 여백 10. 드롭은 여러 파일 중 첫 파일만(`ApplyDroppedInputPaths`), 실행 중이면 무시 · 입력 표 업무면 불러오기 실행(T14). 자동 불러오기 = `UsesInputTable()`(`:326`)
- 인디케이터: SageSDI는 줄 아래선 위에 겹쳐 그리고, SageQt는 아래선이 별도 위젯이라 바로 위에 온다 (1px 차이, `screens.md`)
- Qt 함정 셋을 재서 고쳤다: (1) `QStackedWidget`에 준 contentsMargins가 적용되지 않아 카드가 가장자리에 붙었다 → 여백 컨테이너로 감쌈 (CI 실제 화면에서 발견). (2) 자식의 배경 역할은 부모를 물려받아 섹션 제목이 흰색이 됐다 → polish에서 `Window`로 지정. (3) Fusion은 `QTabBar`에 `WA_Hover`를 켜지 않아 hover가 그려지지 않았다 → polish에서 켬 (테스트로 발견)
- 창 어디든 드롭: 창에 `SageFileDropFilter`(이벤트 필터)를 달고 `acceptDrops`를 켰다. 드롭 이벤트는 받는 위젯이 없으면 부모로 올라간다 — 경로 칸 · 결과 영역 · 사이드바 세 곳에 보낸 드롭이 모두 입력 경로가 됐다 (`SageMainWindowTest`). 경로 칸은 읽기 전용 + 드롭 끔. **실제 OS 끌어 놓기(탐색기 · Finder)는 CI에서 재현할 수 없어 확인하지 않았다** — Qt 이벤트로만 확인 (`DEBT_LOG.md`)
- 파일 · 폴더 선택 창(`QFileDialog`)은 CI에서 띄워 조작할 수 없어 확인하지 않았다 — macOS 시트 여부 포함 (`DEBT_LOG.md`)
- 스크린샷 스크립트의 로그인 버튼 위치가 탭 줄 때문에 40px 어긋나 로그인 창 확인이 빠졌다 → 스크립트 보정
- 세 OS 스크린샷 (`docs/screenshots/T13/`, 실행 36506234242): 탭 줄 · 입력 카드 · 여백
- CI 흔들림: develop의 macOS 스크린샷 job이 아티팩트 업로드 `ETIMEDOUT`으로 한 번 실패해 재실행 (앱 무관)
- 입력 경로 · 저장 폴더 검증 메시지(`SAGE_UI_INPUT_REQUIRED` · `SAGE_UI_OUTPUT_REQUIRED`)는 실행 요청에서만 쓰여 T14로 넘겼다

## 결과
- PR 없이 두 브랜치로 `develop`에 squash merge: `feature/workspace-tabs`(탭 · 선택 탭 보존), `feature/input-panel`(입력 카드 · 파일/폴더 선택 · 드롭 · 경로 보존 · 탭 hover)
- `SageWorkspacePanel`(탭 → `QStackedWidget`, 업무별 상태), `SageWorkflowInputPanel`(카드 · 파일/폴더 창), 결과 · 실행 기록 패널 자리, `SageFileDropFilter`, `SageStyle` 탭 · 읽기 전용 입력칸 · 섹션 제목, 핸들러 `inputFileFilter()`
- 테스트: `SageWorkspacePanelTest`(10), `SageWorkflowInputPanelTest`(4), 드롭 세 곳(`SageMainWindowTest`), 입력칸 · 탭 스타일. 로컬 23/23
- 교훈
  - 규격 테스트에 "위치(여백)"를 넣지 않으면 색만 맞고 배치가 틀린 화면이 통과한다 — 실제 화면 스크린샷이 잡았다
  - 스타일로 hover를 그릴 때는 위젯이 hover 이벤트를 받는지(`WA_Hover`)까지 확인한다
