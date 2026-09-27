# T03 — 워크플로 core 이관 · 테스트 기반

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 워크플로 모델(핸들러 인터페이스 · 등록부 · 응답 · 결과 행 변환 · 샘플 핸들러)을 `sage_core`로 옮기고, 이 동작을 Qt Test로 세 OS에서 검증한다. 첫 core 코드이므로 `sage_core` 타깃과 `tests/` 기반을 이 주제에서 만든다.

## 시작 전에
1. 선행 주제: 없음 (T01 완료)
2. 스킬 로드: `git-workflow`, `coding-design`, `coding-rules`
   - 읽을 reference: `coding-design/references/cmake-targets.md`, `coding-rules/references/naming.md` · `api-shape.md` · `values-and-platform.md` · `ownership-and-threads.md`
3. 결정 대기 — 사용자에게 확정받는다
   - 요청 ID 이름: SageSDI는 `L"mfc-sample-run"` · `L"mfc-unknown"`처럼 `mfc-` 접두사를 쓴다. 권장 — `mfc-`를 뺀다 (`sample-run`). 요청 ID는 저장되지 않으므로 호환 문제가 없다
4. 재확인할 사실
   - 아래 원본 경로 · 줄 번호가 SageSDI 최신 커밋에서도 같은지

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/`

**핸들러 인터페이스** — `app/core/workflow/ISageWorkflowHandler.h`. 메서드 전부를 옮긴다 (이름은 `coding-rules` 네이밍으로)
| SageSDI | 역할 |
|---|---|
| `GetWorkflowType()` | 업무 식별자 |
| `GetHeaderTitle()` · `GetInputSectionLabel()` · `GetActionButtonLabel()` | 화면 라벨 |
| `GetTabCount()` · `GetTab(nVisualTabIndex)` | 탭 구성 (시각 순서 → 의미 인덱스) |
| `GetResultColumnCount(nTaskType)` · `GetResultColumn(nTaskType, i)` · `GetResultStyle(nTaskType)` · `UsesCustomResultTable(nTaskType)` | 결과 표 정의 |
| `BuildResultSummary(...)` · `BuildResultTotals(...)` | 요약 · 합계 |
| `GetFilterCriteriaCount()` · `GetFilterCriteria(i)` | 필터 기준 |
| `GetInputDialogTitle()` · `UsesInputTable()` · `FindGenerateCompletedMessage()` | 입력 · 완료 안내 |
| `ValidateSelectedRows(nSelectedCount, bHasSelectedRowNums, strError)` | 선택 검증 |
| `RequiresLogin()` | 로그인 필요 여부 |
| `GetRequestId(nTaskType)` · `RunTask(nTaskType, strPayloadJson)` · `BuildResultRows(nTaskType, strResponseJson, outRows)` | 실행과 결과 변환 |

**추가할 것** — 사이드바를 등록부에서 만들기 위한 표시 정보(라벨 · 분류). SageSDI는 사이드바 항목을 `app/ui/panels/SageSidebarPanel.cpp`의 `BuildTree()`에 하드코딩했다. `coding-design` 규칙(업무 추가 시 고칠 곳 4곳 — `coding-design/references/ui-composition.md` 완료 기준 D)을 지키려면 핸들러가 답해야 한다. `BuildTree()`의 현재 항목을 확인해서 필요한 필드를 정한다

**탭** — `app/core/workflow/SageWorkflowTab.h`: `{ nSemanticIndex, pszLabel }`. 의미 인덱스 상수 `SAGE_TAB_INDEX_INPUT = 0` · `SAGE_TAB_INDEX_DOCUMENT_RESULT = 1` · `SAGE_TAB_INDEX_DOCUMENT_HISTORY = 2` (`SageDefine.h:241-243`)

**결과 표 타입** — `app/core/workflow/SageWorkflowResultTable.h`
- `SageColumnAlign` (LEFT · RIGHT · CENTER), `SageResultField` (FIELD · VALUE · STATUS · REASON)
- `SageWorkflowColumn { pszLabel, nAlign, nWidth, bStretch, nField }` → **`nWidth`(픽셀)는 core로 옮기지 않는다** (`coding-design/references/model-view.md`: core 열 정의에는 의미만)
- `SageWorkflowFilterCriteria { nCriteria, pszLabel, nField }`, `SAGE_FILTER_CRITERIA_NONE = -1`
- `SageResultSummaryItem { label, value, unit, bHighlight, bBadge }`, `SageResultTotalCell { nColumn, strText, nRole }`, `SageResultTotalRole` (LABEL · COUNT · AMOUNT · AMOUNT_HIGHLIGHT)
- `SageWorkflowResultStyle { bCheckbox, bGridLines, nHighlightStart, nHighlightCount }`
- 범용 열 4개 (`SageWorkflowResultTable.cpp` 앞부분): 라벨 `항목` · `값` · `상태` · `사유`, 정렬 모두 CENTER, `값`만 늘어나는 열. 폭(140 · 220 · 110 · 320)은 UI 디자인 값이다 → T15
- `GetRowText(row, field)` — 필드별 텍스트 선택

**결과 행** — `app/core/workflow/SageWorkflowResultPresenter.h`: `SageResultRow { m_nSourceRowIndex, m_strField, m_strValue, m_strStatus, m_strReason }`

**결과 행 변환** — `SageWorkflowResultPresenter::BuildRows` (`.cpp`). 순서와 값을 그대로 옮긴다
1. `success`가 false → 행 2개: (`상태`, `실패`, `"failed"`, 빈 사유) · (`Error`, 오류 code, `"error"`, 오류 message). false 반환
2. 핸들러의 `BuildResultRows`가 true → 그 결과
3. 아니면 (`상태`, `완료`, `"success"`, 빈 사유) 추가 후 요약 행:
   - `status`가 있으면 (`Result`, status, status, 빈 사유)
   - `totalFiles`가 있으면 (`Total`, total, `"summary"`, `"Passed " + passed + ", Failed " + failed`)
   - `fileName`이 있으면 (`File`, fileName, `"output"`, 빈 사유)
   - `outputFolder`가 있으면 (`Folder`, outputFolder, `"output"`, 빈 사유)
- 문자열 상수: `SageDefine.h:317-318, 342-357` (`SAGE_UI_COMPLETED` · `SAGE_UI_FAILED` · `SAGE_UI_RESULT_*` · `SAGE_RESULT_STATUS_*`)

**응답 JSON** — `app/core/workflow/SageWorkflowResponse.cpp`
- 성공: `{"type":"response","requestId":"<id>","success":true,"payload":<payload 또는 {}>,"error":null}`
- 실패: `{"type":"response","requestId":"<id>","success":false,"payload":null,"error":{"code":"<code>","message":"<message>"}}`

**등록부** — `app/core/workflow/SageWorkflowRegistry.cpp`: 핸들러 배열에서 `GetWorkflowType()`이 같은 것을 찾고, 없으면 NULL

**샘플 핸들러** — `app/core/workflow/handlers/SageSampleWorkflowHandler.cpp`
- 업무 식별자 `SAGE_WORKFLOW_SAMPLE = 1`
- 라벨: 제목 `"샘플 업무"`, 입력 섹션 `"입력 파일"`, 실행 버튼 `"실행"`, 입력 창 제목 `"샘플 입력 파일 선택"`, 완료 안내 `"샘플 업무가 완료되었습니다."`
- 탭 3개: (INPUT, `"입력"`) · (DOCUMENT_RESULT, `"결과"`) · (DOCUMENT_HISTORY, `"실행 기록"`)
- 결과 열은 범용 4개, 결과 스타일 기본값, 커스텀 표 없음, 요약 · 합계 없음, 필터 기준 0개, 입력 표 없음, 선택 검증 항상 통과, 로그인 불필요
- `RunTask`: payload에서 `inputPath` · `outputFolder`를 읽고, `outputFolder`가 비면 입력 파일의 폴더를 쓴다. payload `{"status":"완료","fileName":"<입력 파일 이름>","outputFolder":"<폴더>","totalFiles":1,"passedFiles":1,"failedFiles":0}` (`SageDefine.h:383-385`)로 성공 응답
- `BuildResultRows`: false (범용 변환을 쓴다)

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `app/common/SageJson.*` | 문자열 검색식 JSON 처리. `QJsonDocument`로 대체한다 (`values-and-platform.md`: Qt 기능을 직접 구현하지 않는다) |
| `SageWorkflowResultTable::DistributeColumnWidths` | 픽셀 열 폭 계산, `::MulDiv`(Win32) 사용. `QHeaderView` 모드로 대체 (T15) |
| `SageWorkflowResultTable::FormatAmountNumber` | 호출 0곳 |
| `SAGE_WORKFLOW_DELIVERY = 2` | 핸들러 없는 SageTaechang 잔재 |
| 전역 핸들러 인스턴스 `g_handlerSample` | 전역 금지 규칙 — 등록부가 소유하고 `main.cpp`가 조립한다 |
| `SAGE_PATH_SEPARATOR = L'\\'`로 경로를 자르는 코드 | 크로스 플랫폼 규칙 위반. `QFileInfo`로 대체 |

## 함정
- **JSON 중첩을 명시적으로 따라가야 한다 (조용한 동작 변경 위험).** SageSDI의 `JsonExtractString` · `JsonExtractBool`은 문자열에서 `"키"`를 찾는 방식이라 **중첩을 무시한다.** 그래서 `payload` 안의 `status` · `fileName` · `outputFolder` · `totalFiles`와 `error` 안의 `code` · `message`를 최상위에서 꺼내듯 읽는다. `QJsonDocument`로 옮길 때 `payload` · `error` 객체를 명시적으로 따라가지 않으면 값이 비어 결과 행이 달라진다. **테스트로 고정한다**
- `totalFiles` 등 숫자는 SageSDI가 숫자 문자열로 읽어 그대로 행에 넣는다. `QJsonValue::toInt()` 후 문자열로 만들 때 결과가 같은지 테스트로 확인한다
- `RequiresLogin` · `UsesInputTable` 같은 `BOOL` 반환은 `bool`로, `CString&` 출력은 `QString&`으로 (`api-shape.md`)
- 사이드바 정보를 인터페이스에 추가하면 T11이 그것을 쓴다 — 필드를 T11 착수 전에 바꾸지 않는다
- `sage_core`는 `Qt::Core`만 링크한다. `sage_common`은 쓸 코드가 생길 때 만든다 (지금 `SageJson`을 대체하면 common에 둘 코드가 없다)

## 작업
PR 1개: `feature/workflow-core` (테스트 기반이 크면 `chore/test-foundation`으로 분리)
- [x] `sage_core` 타깃 생성 (`SageQt/core/`, include 루트 격리)
- [x] `tests/` 기반: 루트 `CMakeLists.txt`에 `enable_testing()`, `SageQt/CMakeLists.txt`에서 `tests/` 추가 (`cmake-targets.md`), Qt Test, `CMakePresets.json`에 테스트 프리셋, CI에서 테스트 실행
- [x] 핸들러 인터페이스 · 탭 · 결과 표 타입 · 결과 행 · 응답 · 결과 변환 · 등록부 · 샘플 핸들러 이관
- [x] 사이드바 표시 정보를 인터페이스에 추가 (`BuildTree()` 확인 후)
- [x] 쓰이는 문자열 상수를 `SageDefine.h`로 옮김 (쓰이지 않는 것은 옮기지 않는다)
- [x] 테스트: 결과 변환(성공 · 실패 · 요약 행 순서와 값), 샘플 핸들러 응답, 등록부 조회, JSON 중첩 읽기
- [x] 위반 검출 테스트: `core`에서 `<QWidget>`을 include하면 컴파일 에러가 나는지 확인하고 되돌린다

## 완료 기준
- 3-OS CI에서 빌드와 테스트가 모두 통과한다
- 결과 변환 테스트가 위 순서 · 값을 그대로 검증한다 (성공 · 실패 · 요약 각각)
- `sage_core`의 링크 대상이 `Qt::Core`와 `sage_define`뿐이다
- `core`에서 `<QWidget>` include 시 컴파일 에러 (위반 검출 테스트 기록)
- 샘플 업무가 `ui-composition.md` 완료 기준 D의 4곳에만 있다 — `git grep -il sample -- SageQt/`의 결과가 핸들러 파일 1쌍 · `SageQt/core/CMakeLists.txt` · `SageDefine.h` · 등록부뿐이다 (사이드바는 T11에서 확인)

## 범위 밖
- 실행 흐름(백그라운드 · 진행률) — T14
- 결과 표 화면 — T15

## 확인한 사실
- SageSDI 최신 커밋 `2a6c179`에서 원본 경로 · 줄 번호 · 값이 이 파일과 같다 (`SageDefine.h:241-243 · 317-318 · 342-357 · 383-385`)
- 사이드바 업무 항목 라벨은 헤더 제목과 같은 `SAGE_UI_SAMPLE_NAME`, 그룹은 `SAGE_UI_SIDEBAR_GROUP_SAMPLE = L"샘플"` (`SageDefine.h:496`). 그룹 라벨은 헤더의 분류(`SetCategory`)로도 쓰인다 (`SageSidebarPanel.cpp:75-84`) → 핸들러에 `sidebarLabel()` · `category()`
- SageSDI `JsonExtractIntText`는 숫자만 읽는다 (부호 없음). SageQt는 `QJsonValue::toInteger`라 음수를 `"-1"`로 읽는다 — SageSDI는 빈 문자열로 행을 건너뛴다. 음수 개수는 나오지 않으므로 그대로 둔다
- SageSDI `GetFolderPath`는 구분자가 없으면 빈 문자열, `QFileInfo::path()`는 `"."`. 입력은 파일 창 · 드롭의 절대 경로라 같다. 테스트는 절대 경로로 고정
- SageSDI의 문자열 검색식 JSON은 최상위 · 중첩을 구분하지 않는다. SageQt는 `payload` · `error` 안만 읽는다 — 최상위 키를 무시하는 테스트로 고정 (`buildRowsIgnoresSummaryKeysOutsidePayload`)
- `enable_testing()`은 루트, `tests/`는 `SageQt/CMakeLists.txt`에서 추가해도 빌드 루트의 `ctest --preset`이 테스트 4개를 찾는다
- `C:/Program Files/CMake`의 CMake 3.23.0-rc2는 `define_property(... INITIALIZE_FROM_VARIABLE)`를 잘못 거부해 `find_package(Qt6 COMPONENTS Test)`가 실패한다. VS 번들 CMake 4.3.1은 정상 (최소 재현으로 확인) → `sageqt-plan` 공통 함정에 추가
- 위반 검출 테스트: `SageWorkflowResponse.cpp`에 `#include <QWidget>` → MSVC `C1083: 포함 파일을 열 수 없습니다. 'QWidget'` (로컬 Windows, 되돌림)
- `git grep -il sample -- SageQt/` 결과: `SageQt/core/CMakeLists.txt` · `SageWorkflowRegistry.cpp` · `SageSampleWorkflowHandler.cpp/.h` · `SageDefine.h` — 완료 기준 D의 4곳

## 결과
- PR 없이 `develop`에 직접 머지 (사용자 결정). 머지 전 검증은 로컬 Windows — 빌드(경고 에러화) · 테스트 4개 17건 · clang-format · clang-tidy · 위반 검출 테스트. 세 OS 검증은 머지 후 `develop` CI
- `sage_core` 타깃과 워크플로 모델 13개 파일, `tests/` 기반(Qt Test · 테스트 프리셋 6개 · CI `ctest` 단계), 테스트 4개
- 규칙 추가: 인터페이스 기본 소멸자, 테스트 추가 위치, 테스트 데이터 리터럴 기준
- 교훈
  - 오류 원인을 "새 CMake의 새 검사"로 추정했다가 틀렸다 — 최소 재현을 두 버전에서 돌려 3.23-rc2 버그로 확정했다. 도구 오류는 먼저 어느 버전이 실행됐는지 확인한다
  - 규칙 문구를 쓴 직후 실제 코드에 적용해 보니 모호했다 (테스트 기대값 리터럴) — 규칙은 첫 적용 때 다시 읽는다
