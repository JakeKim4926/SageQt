# T10 — 로그인 · 비밀번호 변경

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 로그인 다이얼로그와 비밀번호 변경 다이얼로그, 그리고 "로그인 성공 → 비밀번호 변경 강제" 흐름을 옮긴다. 헤더(T12) · 사이드바(T11)의 요청 signal을 두 다이얼로그에 연결하고, 로그인이 필요한 화면 동작을 실제 앱에서 확인한다.

## 시작 전에
1. 선행 주제: T12 (헤더의 로그인 요청 signal · 인증 표시), T11 (사이드바의 비밀번호 변경 요청 signal). 다이얼로그는 T09의 프레임리스 기반을 상속한다
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` · `threads-and-db.md` (UI 스레드에서 DB 작업 금지), `coding-rules/references/ownership-and-threads.md` · `api-shape.md`
3. 결정 — 없음
4. 재확인할 사실
   - `SagePasswordChangeDlg.cpp`의 검증 순서와 메시지 (현재 비밀번호 확인 여부, 새 비밀번호 · 확인 불일치 처리)
   - 인라인 오류(`SageInlineError`)와 입력칸 오류 상태(`SAGE_EDIT_*`)의 표시 규격 — `sageqt-ui`

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/ui/`

**로그인 흐름** — `dialogs/SageLoginDlg.cpp` `OnOK`, 순서 그대로
1. 인라인 오류를 지우고 두 입력칸을 정상 상태로
2. 아이디 앞뒤 공백 제거
3. 아이디가 비면 → 아이디 칸에 `SAGE_UI_LOGIN_EMPTY_ID` 오류 표시, 중단
4. 비밀번호가 비면 → 비밀번호 칸에 `SAGE_UI_LOGIN_EMPTY_PW` 오류 표시, 중단
5. 서비스 `Login` 호출이 오류 → 오류 메시지 상자, 중단
6. 로그인 실패 → 비밀번호 칸에 `SAGE_UI_LOGIN_FAILED` 오류 표시 + 비밀번호 전체 선택, 중단
7. 세션에 로그인 설정
8. `must_change_pw`가 필요(1)하면 → 경고 `SAGE_UI_MUST_CHANGE_PW_REQUIRED` → 비밀번호 변경 다이얼로그
   - 변경을 취소하면 → **로그아웃** → 경고 `SAGE_UI_MUST_CHANGE_PW_CANCELED` → 비밀번호 칸 비움 · 포커스, 중단 (로그인이 성립하지 않는다)
9. 수락

**규격** — 로그인 창 폭 320, 라벨 폭 64, 버튼 폭 96, 폰트 10pt (`SageDefine.h:421, 423, 425, 428`) / 비밀번호 창 폭 360, 라벨 폭 96 (`:422, 424`) — 최종 규격은 `sageqt-ui`를 따른다

**입력칸** — 로그인: 아이디 · 비밀번호 / 비밀번호 변경: 현재 · 새 비밀번호 · 확인

**여는 곳** — 헤더 · 사이드바는 T11 · T12에서 요청 signal만 낸다. 다이얼로그와의 연결은 이 주제에서 창의 앱 수준 연결로 한다 (`ui-composition.md`)
| 위치 | 동작 |
|---|---|
| `panels/SageHeaderPanel.cpp:152-156` `OnLogin` | 로그인 창 → 수락이면 헤더의 인증 표시 갱신 |
| `panels/SageHeaderPanel.cpp:158-` `OnLogout` | 세션 로그아웃 → 인증 표시 갱신 |
| `view/SageSDIView.cpp:195-200` `OnSidebarAction` | 사이드바 동작 → 비밀번호 변경 창 |
| `SageLoginDlg.cpp` 8단계 | 강제 변경 |

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `PreTranslateMessage`의 Enter · Tab · Shift+Tab 처리 | `QDialog` 기본 버튼 · 탭 순서가 대신한다 |
| `SageEdit`의 Ctrl+A 전체 선택 (`SageHandleEditSelectAll`) | `QLineEdit` 기본 기능 (macOS는 Cmd+A) |
| `sageDBMgr` · `sageAuth` 전역 참조 | 서비스 · 세션을 주입받는다 |

## 함정
- **SageSDI는 로그인을 UI 스레드에서 동기로 처리한다.** SageQt 규칙은 UI 스레드에서 DB 작업을 금지한다 → `QtConcurrent::run` + `QFutureWatcher`로 서비스를 호출하고, 결과가 오면 위 5~9단계를 이어서 한다. 기다리는 동안 버튼을 비활성화해 중복 제출을 막는다. **순서와 메시지는 바꾸지 않는다**
- 비밀번호 입력칸은 `QLineEdit::Password` 모드
- 탭 순서: macOS는 시스템 설정에 따라 Tab이 입력칸 사이만 이동할 수 있다 — Qt 기본 동작을 따르고 직접 바꾸지 않는다
- 강제 변경 취소 시 **로그아웃이 반드시 먼저** 일어나야 한다. 테스트나 수동 확인으로 고정한다

## 작업
PR 1~2개: `feature/login-dialog`, `feature/password-change-dialog`
- [ ] 로그인 다이얼로그 (프레임리스 기반 상속)
- [ ] 비밀번호 변경 다이얼로그
- [ ] 강제 변경 흐름
- [ ] 서비스 호출을 백그라운드로
- [ ] 헤더 로그인 요청 · 사이드바 비밀번호 변경 요청을 다이얼로그에 연결, 로그인 · 로그아웃 시 세션 변경 signal로 헤더 · 사이드바 갱신 (T12)
- [ ] 세 OS 확인 (Enter · Esc · 오류 표시 · 강제 변경 취소 · 로그인 후 헤더 · 로그아웃 · 사이드바 비밀번호 변경), 스크린샷

## 완료 기준
- 3-OS CI 통과
- 헤더의 로그인 버튼으로 연 창에서 위 9단계를 모두 수동 확인했다 (확인 표를 PR에 첨부)
- 초기 관리자로 처음 로그인하면 변경이 강제되고, 취소하면 로그인 상태가 아니다
- 로그인 중 UI가 멈추지 않는다 (서비스 호출이 UI 스레드 밖)
- 로그인 후 · 로그아웃 후 헤더 표시가 T12의 인증 표시 규칙과 같다 (관리자 · 일반 사용자 — 일반 사용자 계정을 만드는 방법은 T12의 사용자 관리 화면 재확인 결과를 따른다)
- 로그인한 상태에서 사이드바의 비밀번호 변경을 누르면 비밀번호 변경 창이 뜨고, 이전 업무 선택으로 되돌아간다 (T11 선택 처리 3단계)

## 범위 밖
- 헤더의 로그인 버튼 · 인증 표시 자체 — T12 (여기서는 연결과 확인만)
- 사이드바의 비밀번호 변경 항목 자체 — T11 (여기서는 연결과 확인만)

## 확인한 사실
(진행 중 기록)
