# T20 — 규칙 위반 정리

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
2026-10-03 전체 코드 점검에서 확정된 규칙 위반을 고친다. 기계로 잡을 수 있는 규칙은 같은 PR에서 CI 검사로 막아, 다시 생기지 않게 한다. 동작은 바꾸지 않는다.

## 시작 전에
1. 선행 주제: 없음 (규칙 문서 정비 `docs/skill-compliance-gate` 완료 — 결정 기록 2026-10-03 세 줄)
2. 스킬 로드: `coding-rules`(reference 다섯 파일 모두), `coding-design`, `sageqt-ui`, `git-workflow`, `code-review-expert`
   - 이 주제에 고유한 reference: `coding-design/references/ui-composition.md`(생성자 형태), `sageqt-ui/references/design-values.md`(*쓰는 방식* 열)
3. 결정 — 모두 확정됨 (`MIGRATION_PLAN.md` 결정 기록 2026-10-03). 새로 판단이 필요한 것이 나오면 멈추고 묻는다
4. 재확인할 사실 — PR마다 착수 시 아래 *위반 목록*의 줄 번호를 다시 grep으로 확인한다 (앞 PR이 줄을 옮긴다)

## 위반 목록 (2026-10-03 점검, 직접 확인한 것)

**플랫폼 · 경로**
- `SageStatusCard.cpp:297` 경로를 `/` · `\\`로 직접 자름. 원인은 `SageWorkspacePanel.cpp:382`가 `QDir::toNativeSeparators`로 바꾼 경로를 넘기는 것 → 내부 경로를 넘기고 `QFileInfo`로 나눈 뒤 표시 직전에만 바꾼다
- `SageStatusCardTest.cpp:98, 127` 입력 `C:\\work\\out`

**소유권**
- 설정 뒤 넘기는 레이아웃: `SageWorkflowInputPanel.cpp:229`, `SageStatusCard.cpp:115`
- 함수 밖으로 돌려주는 `new QStandardItem`: `SageSidebarModel.cpp:51, 59, 69` (`makeGroup` · `makeWorkflow` · `makeChangePassword`)
- `QObject`를 `std::unique_ptr`로: `SageLoginDlgTest.cpp:49`, `SagePasswordChangeDlgTest.cpp:47` (`SageAuthSession`)

**이름 · 선언**
- `bool` 멤버 10개: `m_success`(`SageWorkflowHistory.h:14`, `SageWorkflowRunResult.h:21`), `SageResultTablePanel.h:99-101`, `SageWorkflowInputPanel.h:72, 75, 78`, `SageResultTableDelegate.h:41`, `SageWorkflowController.h:36`
- slot 이름: `updateAuthState`, `openLoginDialog`, `openPasswordChangeDialog`, `syncSelectionBar`, `updateTotalBarCells`, 테스트 `handleActiveModal` · `tick`
- `slots` 구역 밖 slot: `SageResultTablePanel.h:64-76`, `SageWorkflowHistoryPanel.h:30`, `SageSearchBox.h:42`
- 위젯 멤버 접미사: `m_inputTable` · `m_resultTable`(`SageResultTablePanel*` → `…Panel`), `m_filterPills`(→ `…Bar`)
- `create*` → `add*`: `createFormLabel` · `createPasswordEdit`(`SagePasswordChangeDlg`), `createPathEdit`(`SageWorkflowInputPanel`), `appendGroup`(`SageSidebarModel`)은 이름 확인 후 결정 기록대로
- null 계약: `panelFor`(`SageWorkspacePanel.cpp:308`) · `contentWidget`(`SageFramelessDlg.h:21`)은 `T&`, `showInputError(SageLineEdit*)`(`SageLoginDlg.h:41`, `SagePasswordChangeDlg.h:45`)는 `T&` 매개변수, 테스트 보조 함수 13개는 `find*`
- `QVariant`에 담는 클래스 밖 enum: `SageSidebarItemKind`(`SageSidebarModel.h`), `SageTableTone`(`SageTableRole.h`) → 쓰는 클래스 안 + `Q_ENUM`
- 생성자 형태: `SageWorkflowResultPanel.cpp:8-15`
- include: 쓰는 타입의 헤더 직접 — `SageResultTableDelegate.h`(`QColor` · `QSize`), `SageStatusCard.h`(`QColor`), `SageSummaryBar.h`(`QColor` · `QFont`), `SageTableTotalBar.h`(`QColor`), `SageResultTablePanel.h`(`QModelIndex`), `SageDialogCaptionBar.cpp`(`QFont` · `QSizePolicy`), `SageSearchBox.cpp`(`QBrush`), `SageStyle.cpp`(`QPen` · `QRectF` · `QFont`), `SageTestModalDriver.h`(`<utility>`). 순서: `SageWorkspacePanelTest.cpp:19`
- 중복 전방 선언 `class SageLabel;` (`SageWorkflowInputPanel.h:11, 14`)
- 값 타입 멤버 선언부 기본값 — 약 150곳 (스칼라 `= 값`, 클래스 타입 `{}`, 참조 제외)

**값 · 문자열**
- 숫자 리터럴 0 · 1 · 2 · -1 포함 전부 (앱 코드) — 공통 상수(`values-and-platform.md`), 그리드 행 · 열 enum(`SageLoginDlg` · `SagePasswordChangeDlg` · `SageMessageBoxDlg` · `SageWorkflowInputPanel`), 표 열 enum(체크 열 `index.column() == 0`)
- `SAGE_RESULT_STATUS_*`(`SageDefine.h:93-97`) → `SageResultStatus` enum + `SAGE_UI_RESULT_STATUS_*`
- `SAGE_UI_ROW_NUM_SEPARATOR`(`SageDefine.h:114`) → 화면 문자열이 아니므로 `SAGE_UI_` 제거
- `"4~15자"` 류(`SageDefine.h:56, 201, 202`) → `%1` · `%2` + `arg`
- `SageStatusCard.cpp:23-30` 익명 namespace의 파생 디자인 상수 → `SageDesignDefine.h`
- 테스트: 픽셀 색 기대값 상수 → 리터럴(`SageResultTablePanelTest.cpp` 약 25곳), 열 번호 → enum(`SageWorkflowHistoryPanelTest.cpp:91-99`)

**UI**
- 최소값 상수를 고정 폭으로: `SageResultTablePanel.cpp:188`(`SAGE_RESULT_RESET_WIDTH`), `SageSearchBox.cpp:21`(`SAGE_SEARCH_CRITERIA_CELL_WIDTH`), `SageStatusCard.cpp:164, 174`(`SAGE_PROGRESS_TEXT_WIDTH`를 그리기 고정 폭으로)
- 체크 상자 그리기 두 벌: `SageResultTableDelegate.cpp:133-160`, `SageStyle.cpp:282-303` → `SageStyle` 하나로

**테스트 · 저장소**
- core 클래스 `SageWorkflowResultTable`에 `tests/core/workflow/` 테스트 없음
- `tools/dialog-capture/__pycache__/capture.cpython-311.pyc`가 git에 있음 → 지우고 `.gitignore`

## 함정
- 이름 · 시그니처를 바꾸면 모든 호출부와 테스트를 함께 고친다. 문자열로 이름을 찾는 곳(`findChild` 객체 이름, `QDesktopServices::setUrlHandler`의 slot 이름 `openUrl`)은 컴파일러가 잡지 못한다
- `SageStatusCard`의 경로 말줄임은 T14 테스트(`SageStatusCardTest`)가 잡는다. 표시 문자열은 지금과 같아야 한다 (Windows에서는 `\`로 보인다)
- 고정 폭 → 최소 폭은 화면이 바뀔 수 있다. 바꾼 뒤 세 OS 스크린샷(`screenshots.yml`)을 이전과 비교한다
- 숫자 상수화는 수백 곳이라 같은 뜻에 다른 상수를 쓰기 쉽다. 공통 상수 목록을 `values-and-platform.md`에 먼저 확정한 뒤 바꾼다

## 작업
PR마다 develop 반영 전 규칙 점검(`git-workflow` *develop 반영 절차* 1)을 하고 PR_LOG에 적는다. CI 검사는 `tools/rule-check/check_rules.py` 하나에 모으고 `build.yml` static-analysis job에서 돌린다.
- [x] PR1 플랫폼 · 소유권 — 경로 · 레이아웃 · `QStandardItem` · 테스트 `unique_ptr`, `.pyc` 정리. CI: `\\` 경로, 부모 없는 `new`
- [ ] PR2 이름 · slot — `bool` 멤버, slot 이름 · 구역, 위젯 멤버 접미사, `add*`. CI: `bool` 멤버 이름
- [ ] PR3 선언 — null 계약, enum 위치, 생성자 형태, include, 중복 전방 선언, core 테스트
- [ ] PR4 UI — 최소 폭, 체크 상자 한 벌, 상태 카드 파생 상수. CI: 최소값 상수의 `setFixedWidth`
- [ ] PR5 문자열 · enum — 결과 상태 enum, 구분자 이름, 문구 속 숫자, 테스트 색 · 열 번호
- [ ] PR6 멤버 기본값. CI: 기본값 없는 값 타입 멤버
- [ ] PR7 숫자 리터럴 — 공통 상수 확정 후 전체

## 완료 기준
- 3-OS CI 통과, `check_rules.py`의 검사가 모두 0건
- 위 *위반 목록*의 항목마다 PR 번호(또는 커밋)가 *결과*에 적혀 있다
- 각 PR의 규칙 점검 Blocker · Major 0 (PR_LOG)
- 세 OS 스크린샷이 PR4 전후로 의도한 차이(최소 폭)만 있다

## 범위 밖
- `SageWorkspacePanel` 책임 분리 · 손자 패널 signal 연결 · 상태 표시줄 패널화 — 구조 변경이라 별도 주제로 제안한다 (사용자가 아직 추가하지 않음)
- `resources/GmarketSansTTFLight.ttf` · `GmarketSansTTFMedium.ttf`(qrc 미등록) — 원래부터 쓰이지 않던 파일이라 보고만 한다 (CLAUDE.md 3)

## 확인한 사실
- PR1 (2026-10-03): `check_rules.py`를 고치기 전 코드에 돌리면 정확히 8건(`\` 3 · 부모 없는 `new` 5), 고친 뒤 0건. 공간 사이 `QSpacerItem` 4곳은 결정 기록대로 만든 다음 줄에서 넘기므로 걸리지 않는다
- PR1: 상태 카드 경로 말줄임은 `QFileInfo::path()`로 나눈다. 드라이브 바로 아래 경로(`C:/out`)가 칸보다 길 때만 나누는 위치가 `C:` + `\out`에서 `C:\` + `out`으로 바뀐다 — 표시 문자열 전체는 같다
- PR1: 사이드바 항목은 `add*`가 만들어 바로 넣은 뒤 설정한다. 설정 중 `dataChanged`가 나지만 모델 생성 중이라 연결된 view가 없다
