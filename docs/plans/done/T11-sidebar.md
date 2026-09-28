# T11 — 사이드바

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 사이드바(앱 제목 · 업무 트리 · 기타 동작)를 옮긴다. 업무 항목은 하드코딩하지 않고 **등록부의 핸들러 목록에서 만든다.** 이 주제에서 `SageMainWindow`의 첫 패널 배치가 시작된다.

## 시작 전에
1. 선행 주제: 없음 (T09 완료 — `SageButton` · 메시지 상자 · `SageStyle` 버튼 그리기가 있다) (로그인 필요 경고가 메시지 상자로 뜬다. T03의 핸들러 사이드바 정보를 쓴다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` (창 역할 · 소유 규칙 · 작성 형태) · `style.md` · `model-view.md`, `coding-rules/references/api-shape.md` (signal/slot)
3. 결정 — 없음. 사이드바 맨 위 앱 제목은 T04에서 정한 앱 식별 정보의 표시 이름을 따른다 (`MIGRATION_PLAN.md` 결정 기록)
4. 재확인할 사실
   - **메인 창 초기 · 최소 크기** — SageSDI에 없다. T08 스크린샷에서 내용 없는 창은 Qt 기본값(약 200×100)으로 떴다. 첫 패널을 넣을 때 사용자에게 확인한다 (`sageqt-ui/references/screens.md`)
   - T03에서 핸들러에 추가한 사이드바 정보 필드: `sidebarLabel()`(샘플 "샘플 업무") · `category()`(샘플 "샘플" — 트리의 그룹). 등록부는 지금 `findHandler`만 있다 — 트리를 만들 핸들러 목록 접근은 이 주제에서 추가한다

## SageSDI에서 옮길 것
원본: `D:/Projects/SageSDI/SageSDI/app/ui/panels/SageSidebarPanel.cpp`

**트리 구성** (`BuildTree`)
```
샘플                 (그룹, SAGE_UI_SIDEBAR_GROUP_SAMPLE — 동작 없음)
└ 샘플 업무           (업무 SAGE_WORKFLOW_SAMPLE)
기타                 (그룹, SAGE_UI_SIDEBAR_GROUP_ETC — 동작 없음)
└ 비밀번호 변경        (동작 SAGE_SIDEBAR_ACTION_CHANGE_PASSWORD = 10001, SAGE_UI_CHANGE_PW_MENU)
```
- 두 그룹 모두 펼친 상태, 처음에는 "샘플 업무" 선택

**선택 처리** (`OnSelectionChanged`, `:135-166`), 순서 그대로
1. 그룹(동작 없음)이면 무시
2. 로그인이 필요한 항목인데 로그인하지 않았으면 → 경고 `SAGE_UI_LOGIN_REQUIRED` ("로그인 상태에서만 사용가능합니다.") → **이전 업무 선택으로 되돌림**
   - 로그인 필요 여부: 비밀번호 변경은 항상 필요, 업무는 핸들러의 로그인 필요 여부 (`IsLoginRequired`, `:113-120`)
3. 비밀번호 변경이면 → 비밀번호 변경 요청을 알림 → 이전 업무 선택으로 되돌림
4. 업무면 → 마지막 업무 항목 기억 → 이미 선택된 업무와 같으면 아무것도 하지 않음 → 다르면 업무 변경을 알림

**앱 제목** — 트리 위 라벨 (`:37`), 로고 폰트 `SAGE_LOGO_FONT_FACE = L"Gmarket Sans TTF Bold"`, 크기 `SAGE_TITLE_FONT_POINT_SIZE` (143 = 14.3pt) (`SageUiResources.cpp:70`)

**규격** — 폭 220 · 좌우 여백 20 · 분류 글자 간격 +1 · 트리 위 여백 16 · 항목 높이 34 (`SageDefine.h:157, 167-170`) — 최종은 `sageqt-ui`

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `BuildTree()`의 업무 항목 하드코딩 | 등록부에서 만든다 (`coding-design`: 업무 추가 = 핸들러 + 등록부) |
| 항목 데이터에 업무 번호와 동작 번호를 같은 정수로 섞기 (`SetItemData`, 동작 10001) | 역할(role)을 분리한다 — 항목 종류 · 업무 · 동작 |
| `SendMessage(WM_SAGE_SIDEBAR_WORKFLOW / ACTION)`로 부모에 알림 | 의미 있는 signal (`workflowSelected` · `passwordChangeRequested` 등) |
| `sageAuth` 전역 참조 | 세션을 주입받는다 |
| `CSageSidebarTree` 커스텀 그리기 | `QTreeView` + `SageSidebarDelegate` (`sageqt-ui/references/widgets.md`) |

## 함정
- "이전 선택으로 되돌림"을 선택 변경 signal 안에서 하면 **signal이 다시 발생해 재진입**할 수 있다. 되돌리는 동안 신호를 막거나 선택 모델로 처리한다
- 창(`SageMainWindow`)에 중계 slot을 만들지 않는다. 사이드바 signal ↔ 작업 영역 · 다이얼로그 연결은 창의 "앱 수준 연결"로 한다 (`ui-composition.md`)
- 로고 폰트는 패밀리 `Gmarket Sans TTF` + Bold로 찾는다. GDI식 이름 `"Gmarket Sans TTF Bold"`는 macOS · Linux에서 다른 폰트로 잡힌다 (T02)
- 사이드바는 핸들러 목록을 읽기만 한다 — 업무별 분기를 넣지 않는다

## 작업
PR 1개: `feature/sidebar`
- [x] 사이드바 패널: 앱 제목 · 업무 트리(등록부 기반) · 기타 그룹
- [x] 선택 처리 4단계
- [x] `SageMainWindow`에 배치 (창의 최상위 레이아웃 시작)
- [x] 세 OS 스크린샷

## 완료 기준
- 3-OS CI 통과
- 업무를 하나 더 등록하면(테스트용 핸들러) 사이드바 코드 수정 없이 트리에 나타난다
- 선택 처리 1 · 2 · 4단계를 수동 확인했다 (로그인 필요 경고 · 되돌림 포함). 3단계는 로그인해야 확인할 수 있으므로 T10에서 확인한다
- 창에 중계 slot이 0개다

## 범위 밖
- 비밀번호 변경 창 자체 — T10 (여기서는 요청 signal만)
- 로그인한 상태에서의 선택 처리 3단계 확인 — T10 (로그인 창이 T10에서 생긴다)
- 업무 변경 시 작업 영역 갱신 — T13

## 확인한 사실
- SageSDI 코드 대조 (2026-09-28, `SageSidebarPanel.cpp` · `SageSidebarTree.cpp`): 선택 글자색은 `SAGE_COLOR_SIDEBAR_SELECTED_TEXT`(흰색 — 스킬의 "미정"은 틀렸다), 분류 줄은 캡션 폰트 · `SAGE_COLOR_SIDEBAR_CATEGORY` · 선택 표시 없음, 선택 줄은 `SAGE_FONT_CONTENT_SEMIBOLD`. 제목 칸 높이는 `SAGE_HEADER_HEIGHT`(56)이고 바로 아래 1px `SAGE_COLOR_SIDEBAR_DIVIDER`, 트리는 56 + 16에서 시작하고 아래 `SAGE_MARGIN`. 분류 · 업무 모두 글자 왼쪽 `SAGE_SIDEBAR_PAD_X`(들여쓰기 없음). 스크롤 없음(`TVS_NOSCROLL`)
- 샘플 업무는 로그인이 필요 없다 (`isLoginRequired` false) — 시작 시 경고가 뜨지 않는다
- 분류 줄: SageSDI는 선택만 되고 표시가 없어 업무 선택 표시가 사라졌다. SageQt는 분류 줄을 선택할 수 없게 하고, Qt가 선택을 비우면 마지막 업무로 되돌린다 (`clickingGroupKeepsWorkflowSelected`)
- 되돌림 재진입: 되돌리면 선택 signal이 다시 오지만 같은 업무라 4단계에서 끝난다 — 신호를 막지 않았다 (막으면 view의 선택 표시도 갱신되지 않는다)
- 등록부에 `handlers()` · `registerHandler` 추가. 테스트 핸들러 2개(`tests/ui/panels/SageTestWorkflowHandler.h`)를 등록하면 사이드바 코드 수정 없이 분류별로 트리에 나타난다
- 선택 처리 1 · 2 · 3 · 4단계는 자동 테스트로 확인했다 — 3단계는 테스트에서 로그인 상태를 만들어 T10을 기다리지 않았다. 사람이 누르는 수동 확인은 하지 않았다
- 그린 값 (`drawsSidebarSurfaceDividerAndSelection`, offscreen): 폭 220, 선 y = 56, 트리 y = 72, 행 34, 왼쪽 바 · 선택 면 · 면 색, 로고 `Gmarket Sans TTF`
- 세 OS 스크린샷 (`docs/screenshots/T11/`, 실행 36388325677): 사이드바 · 로고 · 분류 · 선택 줄이 같게 그려진다. 러너 화면이 Windows · macOS 1024 × 768이라 1280 × 800 창은 화면 밖으로 잘린다
- `sage_ui`가 `sage_core`를 PUBLIC으로 링크 — `cmake-targets.md` 타깃 표에 있던 의존. `SageSidebarPanel.h`가 `SageDefine.h`를 include해 `sage_define`도 PUBLIC으로
- 메인 창 초기 크기 1280 × 800 (사용자 결정), 최소 크기는 T16

## 결과
- 작업 브랜치 CI(build · static-analysis · screenshots) 통과 후 `develop`에 squash merge. PR 없음
- `ui/panels/SageSidebarPanel`(선택 처리 4단계, signal `workflowSelected` · `passwordChangeRequested`), `ui/models/SageSidebarModel`(등록부 → 분류 그룹 + 기타), `ui/widgets/SageSidebarDelegate` · `SageSurface` · `SageLabel`, `SageStyle::polish`(면 팔레트 · 로고 폰트) · 구분선. `SageMainWindow`가 사이드바를 배치하고 등록부 · 세션을 주입받는다
- 테스트: `tests/ui/panels/SageSidebarPanelTest`(8), 등록부 목록. 로컬 16/16
- 교훈
  - 스킬의 "미정"도 착수 때 원본 코드를 먼저 본다 — 사이드바 선택 글자색은 코드에 있었다
  - 되돌림 재진입은 막기보다 멱등(같은 업무면 끝)으로 두는 편이 view 갱신을 해치지 않는다
