# T11 — 사이드바

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 사이드바(앱 제목 · 업무 트리 · 기타 동작)를 옮긴다. 업무 항목은 하드코딩하지 않고 **등록부의 핸들러 목록에서 만든다.** 이 주제에서 `SageMainWindow`의 첫 패널 배치가 시작된다.

## 시작 전에
1. 선행 주제: T08 (T03의 핸들러 사이드바 정보를 쓴다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` (창 역할 · 소유 규칙 · 작성 형태) · `style.md` · `model-view.md`, `coding-rules/references/api-shape.md` (signal/slot)
3. 결정 — 착수 시 사용자와 확정
   - 사이드바 맨 위 앱 제목 문자열: SageSDI는 `SAGE_UI_APP_TITLE = L"SageSDI"`. SageQt의 표시 이름을 정한다 (T06의 앱 이름 결정과 맞춘다)
4. 재확인할 사실
   - T03에서 핸들러에 추가한 사이드바 정보 필드

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
| `CSageSidebarTree` 커스텀 그리기 | `sageqt-ui` 분류에 따른다 (기본 트리 + `SageStyle` 우선) |

## 함정
- "이전 선택으로 되돌림"을 선택 변경 signal 안에서 하면 **signal이 다시 발생해 재진입**할 수 있다. 되돌리는 동안 신호를 막거나 선택 모델로 처리한다
- 창(`SageMainWindow`)에 중계 slot을 만들지 않는다. 사이드바 signal ↔ 작업 영역 · 다이얼로그 연결은 창의 "앱 수준 연결"로 한다 (`ui-composition.md`)
- 로고 폰트 이름은 T02에서 확인한 OS별 이름으로 찾는다
- 사이드바는 핸들러 목록을 읽기만 한다 — 업무별 분기를 넣지 않는다

## 작업
PR 1개: `feature/sidebar`
- [ ] 사이드바 패널: 앱 제목 · 업무 트리(등록부 기반) · 기타 그룹
- [ ] 선택 처리 4단계
- [ ] `SageMainWindow`에 배치 (창의 최상위 레이아웃 시작)
- [ ] 세 OS 스크린샷

## 완료 기준
- 3-OS CI 통과
- 업무를 하나 더 등록하면(테스트용 핸들러) 사이드바 코드 수정 없이 트리에 나타난다
- 선택 처리 4단계를 수동 확인했다 (로그인 필요 경고 · 되돌림 포함)
- 창에 중계 slot이 0개다

## 범위 밖
- 비밀번호 변경 창 자체 — T10 (여기서는 요청 signal만)
- 업무 변경 시 작업 영역 갱신 — T13

## 확인한 사실
(진행 중 기록)
