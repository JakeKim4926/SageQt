# T12 — 헤더

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 헤더(업무 제목 · 분류 · 로그인 상태 · 로그인/로그아웃 버튼)를 옮긴다.

## 시작 전에
1. 선행 주제: T10 (로그인 창), T11 (창 배치 시작)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` · `style.md` (배지 등 변형은 `Q_PROPERTY`), `coding-rules/references/api-shape.md`
3. 결정 — 없음
4. 재확인할 사실
   - SageSDI에 사용자 관리 화면(사용자 추가 · 삭제 · 역할 변경)이 있는지 — `SageUserService`에 `AddUser` · `LoadAll` · `RemoveUser`가 있고 저장소에 역할 변경 SQL이 있다. 화면이 없으면 이관 범위에 넣지 않는다 (사용자에게 보고)
   - `SetCategory`에 들어가는 값이 무엇인지 (업무 분류? T03 사이드바 정보와의 관계)

## SageSDI에서 옮길 것
원본: `D:/Projects/SageSDI/SageSDI/app/ui/panels/SageHeaderPanel.h/.cpp`

**구성** — 제목 라벨 · 분류 라벨 · 사용자 라벨 · 역할 배지 · 로그인 버튼 · 로그아웃 버튼
- 공개 동작: `SetTitle` · `SetCategory` · `UpdateAuthState`

**인증 표시** (`UpdateAuthState`)
- 로그인 안 함: 로그인 버튼만 보임
- 로그인함: 로그아웃 버튼 · 사용자 라벨(로그인 아이디) · 역할 배지 보임
- 역할 배지: 관리자면 `SAGE_UI_ROLE_ADMIN`, 아니면 `SAGE_UI_ROLE_USER`. 색은 목록 헤더 배경 · 목록 헤더 테두리 · 주 색 (`SAGE_COLOR_LIST_HEADER` · `SAGE_COLOR_LIST_HEADER_BORDER` · `SAGE_COLOR_PRIMARY`) — 최종은 `sageqt-ui`

**버튼 동작** (`:152-161`)
- 로그인 → 로그인 창(T10) → 수락이면 인증 표시 갱신
- 로그아웃 → 세션 로그아웃 → 인증 표시 갱신

**규격** — 높이 56 · 간격 12 · 제목 간격 10 · 분류 폭 80 · 헤더 폰트 113(11.3pt) · 로그인 버튼 폭 68 · 사용자 라벨 폭 150 (`SageDefine.h:153, 164-166, 173, 419-420`) — 최종은 `sageqt-ui`

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
- [ ] 헤더 패널 · 인증 표시 · 로그인/로그아웃
- [ ] 세션 변경 signal과 구독 (사이드바 포함)
- [ ] `SageMainWindow`에 배치
- [ ] 세 OS 스크린샷 (로그인 전 · 후, 관리자 · 일반 사용자)

## 완료 기준
- 3-OS CI 통과
- 로그인 전 · 후 표시가 위 규칙과 같다 (수동 확인)
- 헤더가 다른 패널의 메서드를 직접 부르는 곳이 0개다

## 범위 밖
- 사용자 관리 화면 — SageSDI에 있을 때만, 별도 주제로 (재확인 결과를 사용자에게 보고)

## 확인한 사실
(진행 중 기록)
