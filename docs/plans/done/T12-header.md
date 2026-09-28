# T12 — 헤더

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 헤더(업무 제목 · 분류 · 로그인 상태 · 로그인/로그아웃 버튼)를 옮긴다.

## 시작 전에
1. 선행 주제: 없음 (T11 완료 — `SageMainWindow`에 사이드바가 배치됐다. 헤더는 오른쪽 영역 맨 위에 둔다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` · `style.md` (배지 등 변형은 `Q_PROPERTY`), `coding-rules/references/api-shape.md`
3. 결정 — 없음
4. 재확인할 사실
   - 사용자 관리 화면은 **없다** — `AddUser` · `LoadAll` · `RemoveUser` · `UpdateRole`의 호출이 0곳이다 (T05에서 확인, 결정 기록). 이 기능은 옮기지 않았다
   - `SetCategory`의 값은 사이드바에서 선택한 업무 항목의 부모 그룹 라벨이다 (`SageSidebarPanel.cpp:75-84` `GetSelectedCategory`, T03에서 확인) → 핸들러의 `category()`를 쓴다

## SageSDI에서 옮길 것
원본: `D:/Projects/SageSDI/SageSDI/app/ui/panels/SageHeaderPanel.h/.cpp`

**구성** — 제목 라벨 · 분류 라벨 · 사용자 라벨 · 역할 배지 · 로그인 버튼 · 로그아웃 버튼
- 공개 동작: `SetTitle` · `SetCategory` · `UpdateAuthState`

**인증 표시** (`UpdateAuthState`)
- 로그인 안 함: 로그인 버튼만 보임
- 로그인함: 로그아웃 버튼 · 사용자 라벨(로그인 아이디) · 역할 배지 보임
- 역할 배지: 관리자면 `SAGE_UI_ROLE_ADMIN`, 아니면 `SAGE_UI_ROLE_USER`. 색은 목록 헤더 배경 · 목록 헤더 테두리 · 주 색 (`SAGE_COLOR_LIST_HEADER` · `SAGE_COLOR_LIST_HEADER_BORDER` · `SAGE_COLOR_PRIMARY`) — 최종은 `sageqt-ui`

**버튼 동작** (`:152-161`)
- 로그인 → 로그인 요청 signal. 로그인 창을 열고 수락 시 인증 표시를 갱신하는 연결은 T10
- 로그아웃 → 세션 로그아웃 → 인증 표시 갱신

**규격** — 높이 56 · 간격 12 · 제목 간격 10 · 분류 폭 80 · 제목 폰트 `SAGE_FONT_TITLE`(14.3pt → 19px SemiBold — 2026-09-28 코드 확인, 이전 기록의 11.3pt는 틀렸다) · 로그인 버튼 폭 68 · 사용자 라벨 폭 150 (`SageDefine.h:153, 164-166, 173, 419-420`) — 최종은 `sageqt-ui`

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `ShowWindow(SW_HIDE/SW_SHOW)` 후 수동 `LayoutChildren` | Qt 레이아웃이 보임 · 숨김을 반영한다 |
| `sageAuth` 전역 참조 | 세션을 주입받는다 |
| 역할 배지 색을 호출부에서 넘기는 방식 (`SetBadge(text, 색, 색, 색)`) | 배지 변형을 `Q_PROPERTY`로 두고 색은 `SageStyle` · 디자인 값이 정한다 (`style.md`) |

## 함정
- 로그인 · 로그아웃으로 **다른 화면도 바뀌어야 한다** (사이드바의 로그인 필요 항목, 실행 버튼의 로그인 필요 업무). 헤더가 직접 다른 패널을 고치지 않는다 — 세션 변경을 signal로 알리고 각 패널이 받는다
- 제목 폭을 글자 수로 계산하지 않는다 (`GetTitleWidth` 방식) — 레이아웃과 폰트 메트릭에 맡긴다 (T02)

## 작업
PR 1개: `feature/header`
- [x] 헤더 패널 · 인증 표시 · 로그인/로그아웃
- [x] 세션 변경 signal과 구독 (사이드바 포함)
- [x] `SageMainWindow`에 배치
- [x] 세 OS 스크린샷 (로그인 전). 로그인 후 · 역할별 화면은 T10

## 완료 기준
- 3-OS CI 통과
- 로그인 전 표시가 위 규칙과 같다 (수동 확인). 로그인 후 표시는 T10에서 확인한다
- 헤더가 다른 패널의 메서드를 직접 부르는 곳이 0개다

## 범위 밖
- 로그인 창 열기 · 로그인 후 표시 확인 — T10 (로그인 창이 T10에서 생긴다)
- 사용자 관리 화면 — SageSDI에 없다 (T05). 필요하면 별도 주제로

## 확인한 사실
- SageSDI 코드 대조 (2026-09-28, `SageHeaderPanel.cpp` · `SageBadge.cpp` · `SageSDIView.cpp`): 제목은 `SAGE_FONT_TITLE`(19px SemiBold — 지시서의 11.3pt는 틀렸다), 분류는 캡션 · `SAGE_COLOR_SECONDARY_TEXT`, 사용자 라벨은 캡션 · `SAGE_COLOR_TEXT_MUTED` · 오른쪽 정렬. 역할 배지는 관리자 · 사용자 같은 색(목록 헤더 면 · 테두리 · 주 색), 모서리는 `RoundRect`에 `SAGE_BADGE_HEIGHT`를 넘긴 알약 모양(`SAGE_BADGE_RADIUS`는 요약 막대 전용). 헤더는 줄 56 + 아래선 1px, 좌우 `SAGE_CONTENT_PAD_X`. 사이드바와 오른쪽 사이 1px `SAGE_COLOR_BORDER` 세로선 (T11에서 빠뜨렸다)
- `SageAuthSession`을 `QObject`로 바꾸고 `authStateChanged`를 낸다 — core의 첫 `QObject`(Qt::Core). 헤더는 세션 신호로 표시를 갱신하고, 다른 패널 메서드를 부르지 않는다
- 사이드바는 선택 순간에 세션을 읽으므로 새로 그릴 것이 없어 구독하지 않았다
- 그린 값 (offscreen): 헤더 57, 아래선 y = 56(사이드바 제목 아래 선과 같은 줄), 배지 20 · y = 18(SageSDI 12 + 6과 같다), 버튼 32 · 폭 ≥ 68, 세로선 x = 220, 헤더 x = 221
- 사용자 라벨 말줄임은 하지 않았다 — 최소 폭 150에서 넓어진다
- 세 OS 스크린샷 (`docs/screenshots/T12/`, 실행 36402551588): 로그인 전 제목 · 분류 · 로그인 버튼. Windows job은 py7zr `Bad7zFile`로 한 번 실패해 재실행 (세 번째, `DEBT_LOG.md`)

## 결과
- 작업 브랜치 CI 통과 후 `develop`에 squash merge. PR 없음
- `ui/panels/SageHeaderPanel`(slot `showWorkflow`, signal `loginRequested`, 로그아웃은 세션), `ui/widgets/SageBadge`, `SageLabel` `Title` · `SecondaryCaption` · `MutedCaption`, `SageSurface` `Header`, 앱 팔레트 `Mid` = `SAGE_COLOR_BORDER`, 세로 구분선. `SageAuthSession` → `QObject`
- 테스트: `SageHeaderPanelTest`(6), `SageMainWindowTest`(3), 세션 신호. 로컬 18/18
- 교훈
  - 지시서의 규격 숫자도 착수 때 원본과 다시 대조한다 — 제목 폰트 · 배지 반경이 틀려 있었다
  - 이웃 패널과 맞닿는 선(사이드바 · 헤더 아래선, 세로선)은 창 단위 그린 값 테스트로 함께 확인한다
