# T13 — 작업 영역 탭 · 입력 패널

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 작업 영역(업무별 탭 · 업무 전환 시 상태 보존)과 입력 패널(입력 파일 · 저장 폴더 · 파일 드롭)을 옮긴다.

## 시작 전에
1. 선행 주제: T12
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` (탭은 패널 단위 · 한 위젯이 두 탭 역할 금지) · `style.md`, `coding-rules/references/values-and-platform.md` (`QFileDialog` · 경로 규칙) · `api-shape.md`
3. 결정 — 착수 시 사용자와 확정
   - 입력 파일 필터: SageSDI는 엑셀 필터를 쓴다 (아래). 샘플 업무는 어떤 파일이든 받는다. 권장 — 필터를 핸들러가 정하게 한다 (업무마다 입력 형식이 다르다). 핸들러 인터페이스 변경이므로 T03 결과와 맞춘다
   - 탭 구현: `QTabWidget` 또는 `QTabBar` + `QStackedWidget` — `sageqt-ui`의 탭 규격에 맞는 쪽
4. 재확인할 사실
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
- [ ] 작업 영역: 핸들러 탭 → 패널, 의미 ↔ 시각 인덱스
- [ ] 업무별 상태 저장 · 복원 (결과 · 필터 부분은 T14 · T15에서 채운다 — 자리만 비우지 말고 해당 주제에서 추가)
- [ ] 입력 패널: 파일 · 폴더 선택, 검증 메시지
- [ ] 파일 드롭: 창 어디든 → 입력 경로
- [ ] 세 OS 확인 (파일 선택 · 폴더 선택 · 드롭), 스크린샷

## 완료 기준
- 3-OS CI 통과
- 세 OS에서 창의 서로 다른 세 곳(입력칸 · 결과 영역 · 빈 곳)에 파일을 떨어뜨려 입력 경로가 채워진다
- 업무를 바꿨다 돌아오면 탭 · 입력 경로 · 저장 폴더가 그대로다
- Win32 드롭 API · MFC 파일 창이 0개다

## 범위 밖
- 실행 버튼 · 상태 카드 · 진행 표시 — T14
- 결과 표 · 입력 표 — T15

## 확인한 사실
(진행 중 기록)
