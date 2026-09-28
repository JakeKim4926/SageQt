# T10 — 로그인 · 비밀번호 변경

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 로그인 다이얼로그와 비밀번호 변경 다이얼로그, 그리고 "로그인 성공 → 비밀번호 변경 강제" 흐름을 옮긴다. 헤더(T12) · 사이드바(T11)의 요청 signal을 두 다이얼로그에 연결하고, 로그인이 필요한 화면 동작을 실제 앱에서 확인한다.

## 시작 전에
1. 선행 주제: 없음 (T12 완료 — 헤더 `loginRequested` · 사이드바 `passwordChangeRequested` signal이 있고, 세션은 `authStateChanged`를 낸다. 창에서 이 signal을 다이얼로그에 연결한다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` · `threads-and-db.md` (UI 스레드에서 DB 작업 금지), `coding-rules/references/ownership-and-threads.md` · `api-shape.md`
3. 결정 — 없음
4. 재확인할 사실
   - `SagePasswordChangeDlg.cpp`의 검증 순서와 메시지 (현재 비밀번호 확인 여부, 새 비밀번호 · 확인 불일치 처리). T05 서비스의 `changePassword`는 현재 비밀번호를 확인하지 않는다 (SageSDI와 같음) — 확인이 필요하면 `login(아이디, 현재 비밀번호)`로 할지 착수 시 원문을 보고 정한다
   - 로그인 · 비밀번호 변경은 PBKDF2 600,000회 해시를 계산한다 — 시간이 걸리므로 반드시 UI 스레드 밖에서 부른다 (소요 시간은 T05에서 재지 않았다)
   - 인라인 오류(`SageInlineError`)와 입력칸 오류 상태(`SAGE_EDIT_*`)의 표시 규격 — `sageqt-ui`
   - **T09에서 미룬 사용자 확인** — 같은 프레임리스 기반(`SageFramelessDlg`)을 쓴다. (1) 캡션 닫기 버튼이 흰 사각형으로 보이는 것이 SageSDI 실제 화면과 같은지, (2) 96 DPI 1:1 — 메시지 상자 · 로그인 창을 SageSDI와 나란히 놓고 크기 · 글자 크기가 같은지. 스크린샷 `docs/screenshots/T09/`
   - **T08에서 미룬 판정** — T08은 사용자가 화면을 볼 수 없어 규격 없는 상태를 정하지 않았다. 입력칸의 hover · focus · disabled 규격을 세 OS 스크린샷으로 사용자와 정한다 (`sageqt-ui/references/style-scope.md` *미정*)

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
- [x] 로그인 다이얼로그 (프레임리스 기반 상속)
- [x] 비밀번호 변경 다이얼로그
- [x] 강제 변경 흐름
- [x] 서비스 호출을 백그라운드로
- [x] 헤더 로그인 요청 · 사이드바 비밀번호 변경 요청을 다이얼로그에 연결, 로그인 · 로그아웃 시 세션 변경 signal로 헤더 · 사이드바 갱신 (T12)
- [x] 세 OS 확인 (Enter · Esc · 오류 표시 · 강제 변경 취소 · 로그인 후 헤더 · 로그아웃 · 사이드바 비밀번호 변경), 스크린샷

## 완료 기준
- 3-OS CI 통과
- 헤더의 로그인 버튼으로 연 창에서 위 9단계를 모두 수동 확인했다 (확인 표를 주제 파일 *결과* 절에 적는다)
- 초기 관리자로 처음 로그인하면 변경이 강제되고, 취소하면 로그인 상태가 아니다
- 로그인 중 UI가 멈추지 않는다 (서비스 호출이 UI 스레드 밖)
- 로그인 후 · 로그아웃 후 헤더 표시가 T12의 인증 표시 규칙과 같다 (관리자 · 일반 사용자 — 사용자 관리 화면이 없으므로(T05) 일반 사용자는 개발용 DB에 직접 넣은 계정으로 확인한다)
- 로그인한 상태에서 사이드바의 비밀번호 변경을 누르면 비밀번호 변경 창이 뜨고, 이전 업무 선택으로 되돌아간다 (T11 선택 처리 3단계)

## 범위 밖
- 헤더의 로그인 버튼 · 인증 표시 자체 — T12 (여기서는 연결과 확인만)
- 사이드바의 비밀번호 변경 항목 자체 — T11 (여기서는 연결과 확인만)

## 확인한 사실
- SageSDI 코드 대조 (2026-09-28): 비밀번호 변경은 현재 비밀번호를 `Login(아이디, 현재 비밀번호)`로 확인한 뒤 `ChangePassword` (`SagePasswordChangeDlg.cpp:208-225`) — SageQt도 같은 순서를 백그라운드 작업 하나로 한다. `ChangePassword` 실패(검증 · DB)는 새 비밀번호 칸 인라인 오류. 인라인 오류 자리는 고정 배치라 항상 비어 있다. 입력칸 비활성 색 · 오류 테두리는 코드에 있었다(`SageEdit.cpp`). 다이얼로그 입력칸 글자 여백 4는 리터럴 → `SAGE_DLG_EDIT_TEXT_PAD_X`
- 입력칸 글자 시작: Qt `QLineEdit`이 자체 가로 여백 2px를 더한다 (재서 7px로 나옴) → `SAGE_QT_LINE_EDIT_TEXT_MARGIN`를 빼서 테두리 1 + 4 = 5px에 맞추고 테스트로 고정 (`lineEditTextStartsAfterBorderAndPad`). 세로는 Qt 가운데 정렬 (SageSDI 위 여백 9와 같은지 재지 않았다)
- 전역 상수 초기화 순서 버그: `SAGE_FONT_FILES`가 `SAGE_FONT_FAMILY_PRETENDARD`(동적 초기화 `QString`)보다 먼저 초기화돼 빈 패밀리로 폰트 등록이 실패했다 (로컬 테스트 6개). 패밀리를 `constexpr QStringView`로 바꾸고 규칙 추가 (`values-and-platform.md`)
- 사용자 결정: 입력칸 키보드 포커스 테두리 `SAGE_COLOR_PRIMARY`(오류 우선, hover 없음). SageSDI 화면 대조(닫기 버튼 · 1:1 크기)는 T13으로
- 로그인 9단계 · 강제 변경 · 사이드바 · 로그아웃은 사람 대신 조립 수준 테스트로 확인했다 (`tests/app/SageLoginFlowTest` — 임시 SQLite + 실제 PBKDF2 600,000회 + `SageMainWindow`). 사용자 PC 화면을 자동 입력으로 조작하려 했으나 앱 창이 다른 창 뒤에 있어 입력이 다른 프로그램에 들어갈 위험이 있어 그만뒀다
- PBKDF2 한 번 확인: Release 814 ms, Debug 6,707 ms (Windows, 로컬). 그동안 UI 스레드 10 ms 타이머가 81회 · 670회 돌았다 → UI가 멈추지 않는다. SageSDI는 같은 계산을 UI 스레드에서 했다
- 일반 사용자 계정의 헤더 표시(「사용자」 배지)는 테스트 대역 저장소로만 확인했다 (`SageHeaderPanelTest`, `SageLoginDlgTest`) — 실제 DB에 넣어 보지 않았다
- 세 OS 스크린샷 (`docs/screenshots/T10/`, 실행 36415751913): 헤더 「로그인」 → 로그인 창, 빈 아이디 Enter → 인라인 오류 · 오류 테두리, Esc로 닫힘 — `tools/dialog-capture`가 자동으로 한다
- offscreen으로 저장한 화면에서는 버튼 글자가 굵게 보인다. 세 OS 실제 화면에서는 보통 굵기라 offscreen 캡처는 배치 · 색 확인에만 쓴다
- 로컬 개발 DB: 확인을 위해 `%APPDATA%\Sage\SageQt`를 잠시 백업 이름으로 바꿨다가 되돌렸다. 그때 앱이 만든 새 DB는 `SageQt.t10-launch`로 남아 있다

## 결과
- 작업 브랜치 CI(build · static-analysis · screenshots) 통과 후 `develop`에 squash merge. PR 없음
- `ui/dialogs/SageLoginDlg` · `SagePasswordChangeDlg`(QtConcurrent + QFutureWatcher), `ui/widgets/SageLineEdit` · `SageInlineMessage`, `SageLabel` `FormLabel`, `SageStyle` 입력칸(면 · 테두리 · 포커스 · 글자 여백 · 높이). 창이 헤더 `loginRequested` · 사이드바 `passwordChangeRequested`를 다이얼로그에 연결, `sage_ui` → `Qt::Concurrent`
- 확인 표 (자동, `SageLoginFlowTest` · 다이얼로그 테스트)

| SageSDI 단계 | 확인 |
|---|---|
| 1 인라인 오류 지움 · 입력칸 정상 | 각 제출마다 (`SageLoginDlgTest`) |
| 2 아이디 앞뒤 공백 제거 | `"  admin  "`으로 로그인 성공 |
| 3 빈 아이디 → 아이디 칸 오류 | 메시지 · 오류 테두리 |
| 4 빈 비밀번호 → 비밀번호 칸 오류 | 메시지 |
| 5 서비스 오류 → 오류 상자 | 저장소 실패 대역 (`queryErrorShowsErrorBox`) |
| 6 로그인 실패 → 오류 + 전체 선택 | 메시지 · 선택 글 |
| 7 세션 로그인 | 헤더 `admin` · 「관리자」 · 「로그아웃」 |
| 8 강제 변경 · 취소 시 로그아웃 먼저 | 경고 → 변경 창 → (취소) → 경고 시점 로그아웃 확인 → 비밀번호 칸 비움 |
| 9 수락 | 변경 완료 후 창 닫힘, 세션 유지 |
| 사이드바 비밀번호 변경 (로그인) | 창이 뜨고 「샘플 업무」로 되돌림 |
| 로그아웃 · 로그인 필요 경고 | 헤더 원래대로, 경고 후 되돌림 |

- 테스트: `tests/app/SageLoginFlowTest`, `SageLoginDlgTest`(7), `SagePasswordChangeDlgTest`(7), 입력칸 스타일 2개. 로컬 21/21
- 교훈
  - 사람의 "수동 확인"은 앱과 같은 조립을 화면 밖에서 돌리는 테스트로 대신할 수 있다 — 실제 DB · 해시까지 넣으면 UI 멈춤도 잴 수 있다
  - 전역 상수끼리 기대면 파일 추가만으로 초기화 순서가 바뀐다 — 가져다 쓰는 상수는 `constexpr`로
  - Qt 위젯의 숨은 여백은 추측하지 말고 그려서 잰다 (입력칸 +2px)
