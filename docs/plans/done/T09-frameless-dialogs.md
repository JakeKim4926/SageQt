# T09 — 프레임리스 다이얼로그 · 메시지 상자

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 프레임리스 다이얼로그 기반과 메시지 상자를 옮기고, T06에서 미뤄 둔 시작 시 안내(DB 오류 · 초기 관리자 비밀번호)를 이 메시지 상자로 연결한다.

## 시작 전에
1. 선행 주제: 없음 (T08 완료 — `SageStyle` · 팔레트 · 폰트가 `main.cpp`에서 적용된다)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` (화면 클래스 작성 형태) · `style.md`, `coding-rules/references/api-shape.md` · `ownership-and-threads.md` (모달은 스택 값), `sageqt-ui`의 다이얼로그 규격
3. 결정 — 없음 (`sageqt-ui` 규격을 따른다)
4. 재확인할 사실
   - Qt 6.11의 `QWindow::startSystemMove()` 지원 범위 (Windows · macOS · X11 · Wayland)
   - Mac mini 준비 여부 — macOS에서 프레임리스 창의 이동 · 모양을 눈으로 확인해야 한다
   - **T08에서 미룬 판정** — T08은 사용자가 화면을 볼 수 없어 규격 없는 상태를 정하지 않았다. 버튼 · 캡션의 hover · focus · disabled 규격, 변형 위젯의 그리기 방법, Primary 버튼 흰색 글자 상수, GDI 보정값(`SAGE_BUTTON_VERT_ADJUST` · `SAGE_BUTTON_TEXT_TOP_OFFSET`)의 필요 여부를 세 OS 스크린샷으로 사용자와 정한다 (`sageqt-ui/references/style-scope.md` *미정*)
   - 96 DPI 1:1 대응 — 다이얼로그를 SageSDI 화면과 나란히 놓고 크기가 같은지 사용자가 확인한다 (`sageqt-ui` SKILL.md *단위*)

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/ui/`

**프레임리스 기반** — `dialogs/SageFramelessDialog.cpp`
- 창 스타일 `WS_POPUP | WS_BORDER | DS_SETFONT | DS_CENTER` → 테두리 1px, 제목 표시줄 없음, 부모 기준 가운데
- 크기 고정 (리사이즈 경로 없음)
- 상단 캡션 영역(`y < SAGE_DLG_CAPTION_HEIGHT`)을 끌면 창이 이동 (`OnNcHitTest` → `HTCAPTION`)
- 캡션 바는 클릭을 통과시킨다 (`drawing/SageDialogCaptionBar.cpp` `OnNcHitTest` → `HTTRANSPARENT`)
- 캡션의 닫기 버튼 → 취소와 같음 (`OnCaptionClose` → `IDCANCEL`)
- 규격: 캡션 높이 40 · 좌우 여백 16 · 버튼 28 · 버튼 여백 8 (`SageDefine.h:112-115`), 제목 폰트 `SAGE_FONT_CONTENT_SEMIBOLD` (14px SemiBold, `SageDialogCaptionBar.cpp:58` — 2026-09-28 코드 확인. 이전 기록의 9.0pt 캡션 폰트는 틀렸다)

**메시지 상자** — `dialogs/SageMessageBoxDlg.h/.cpp`
- 생성: 메시지 · 종류 · 부모. 편의 함수 `ShowSageMessageBox(text, type = MB_OK, parent = NULL)`
- 종류는 Win32 플래그로 받는다: 아이콘 `MB_ICONERROR` · `MB_ICONWARNING` · `MB_ICONINFORMATION`, 확인형 여부(`IsConfirm`), 기본 버튼이 거부인지(`IsDefaultReject`)
- 확인형(`MB_YESNO`)은 호출 0곳이다 (2026-09-28 — `ShowSageMessageBox` 호출 12곳 모두 아이콘 + `MB_OK`). 확인형 · `Danger` 버튼 · 포커스 링 · 기본 버튼 거부는 옮기지 않는다 (`sageqt-ui` *값 출처 원칙* 2)
- 버튼 2개(수락 · 거부), 본문(`drawing/SageMessageBody`), 아이콘(`SageMessageIcon`), 캡션 제목은 종류에 따라 (`GetCaptionTitle`)
- 규격: 폭 360, 본문 최대 높이 200, 아이콘 22 · 반지름 10 · 아이콘-본문 간격 12 (`SageDefine.h:119-130`의 `SAGE_MSGBOX_*` 전체)
- 앱 전체 호출: 오류(`MB_ICONERROR`) 2곳, 경고(`MB_ICONWARNING`) 5곳, 정보(`MB_ICONINFORMATION`) 1곳 (2026-09-23)

**시작 시 안내** (T06에서 연결 대기) — `main.cpp`는 실패 시 `sage.app` 로그를 남기고 `EXIT_FAILURE`, 초기 관리자 비밀번호는 `std::optional<QString> initialAdminPassword`에 받아 둔다. 메시지 상자는 이 두 지점에 연결한다
- DB 준비 실패 시 오류 메시지 → 종료 (`SageSDI.cpp:93-95`)
- 초기 관리자 비밀번호를 `SAGE_UI_INITIAL_ADMIN_PW_FORMAT`으로 한 번 안내 (`SageSDI.cpp:98-106`, `SageDefine.h:465`)

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| 메모리에서 대화상자 템플릿을 만드는 코드 (`BuildFramelessTemplate`, `DLGTEMPLATE`) | MFC 전용. Qt는 위젯을 코드로 만든다 |
| `MB_*` 플래그 | Win32. `enum class`로 대체 (아이콘 종류 · 버튼 구성) |
| `SageDialogSizer::SizeToClient` | 창 틀 크기를 빼는 Win32 계산. Qt 레이아웃과 고정 크기로 대체 |
| 시작 시 안내에 시스템 메시지 상자(`AfxMessageBox`) 사용 | SageQt는 스타일을 먼저 적용하므로 처음부터 이 메시지 상자로 통일 |

## 함정
- 창 이동은 마우스 좌표를 직접 계산하지 않고 **`QWindow::startSystemMove()`**를 쓴다. Wayland는 앱이 창 위치를 직접 정할 수 없어서 좌표 계산 방식은 동작하지 않는다
- **Wayland에서는 "부모 기준 가운데"도 앱이 정할 수 없다.** 부모를 지정한 다이얼로그로 만들어 컴포지터에 맡긴다
- 제목 표시줄이 없어도 Esc → 취소, Enter → 기본 버튼이 동작해야 한다 (`QDialog` 기본 동작 — 직접 구현하지 않는다)
- macOS: 프레임리스 창의 그림자 · 모서리가 다르게 보일 수 있다 — Mac mini로 확인하고 스크린샷을 남긴다
- 모달 다이얼로그는 스택 값으로 만든다. 부모가 먼저 생성되어 있어야 한다 (`ownership-and-threads.md` 규약 1)
- 테두리 · 캡션 색은 `SageStyle` · 디자인 값에서 온다. 다이얼로그에 `paintEvent`를 넣지 않는다 (`style.md`)

## 작업
PR 1~2개: `feature/frameless-dialog`, (크면) `feature/message-box`
- [x] 프레임리스 다이얼로그 기반: 캡션 영역 · 제목 · 닫기 버튼 · 끌어서 이동 · 고정 크기 · 부모 기준 가운데
- [x] 메시지 상자: 아이콘 종류 · 버튼 구성 `enum class`, 본문, 규격
- [x] `main.cpp`의 시작 시 안내 두 가지를 메시지 상자로 연결
- [x] Windows · Linux · macOS에서 이동 · Esc · Enter 확인, 스크린샷

## 완료 기준
- 3-OS CI 통과
- 세 OS에서 캡션을 끌면 창이 이동한다 (Linux는 X11 · Wayland 각각, 확인한 환경을 기록)
- Esc로 닫히고 Enter로 기본 버튼이 눌린다
- DB 준비 실패와 초기 비밀번호 안내가 이 메시지 상자로 뜬다
- `MB_` · `DLGTEMPLATE`가 SageQt 코드에 0개다

## 범위 밖
- 로그인 · 비밀번호 변경 다이얼로그 — T10

## 확인한 사실
- SageSDI 코드 대조 (2026-09-28): 확인형 메시지 상자 호출 0곳 → 확인형 · `Danger` · 포커스 링 제외. Primary 버튼 글자는 Regular. 버튼 눌림 · 비활성 색과 흰 글자(`SAGE_COLOR_BUTTON_TEXT`)는 `SageButton.cpp:68-93`에 있었다. 캡션 제목은 `SAGE_FONT_CONTENT_SEMIBOLD`(14px SemiBold) → 폰트 역할 `BodyStrong` 추가. `SAGE_BUTTON_TEXT_TOP_OFFSET`(0)은 옮기지 않고, `SAGE_BUTTON_VERT_ADJUST`는 결과 표 배치 값이라 T15로
- `startSystemMove`의 지원 범위는 Qt 6.11 문서에 OS별로 적혀 있지 않다 ("Returns true if the operation was supported") — 실패하면 `sage.ui` 경고 로그에 플랫폼 이름을 남기게 했다. CI 세 OS 로그에 경고 없음
- 끌어서 이동 (CI, `tools/dialog-capture`, 실행 36373636749): Windows · macOS는 (150, 100) 요청에 정확히 (150, 100). Linux(Xvfb + openbox, xcb)는 (600, 340)으로 튄다 — 두 번 같음, 원인 미확인 (`DEBT_LOG.md`). Wayland 미확인
- Enter로 닫힘: 세 OS 모두 (CI). Esc · Enter · 닫기 버튼 · 창 틀 없음 · 폭 · 테두리 · 캡션 · 면 색은 `SageMessageBoxDlgTest`(offscreen)
- 줄바꿈 라벨 크기: `setSizePolicy`를 통째로 바꾸면 heightForWidth 표시가 지워져 본문이 잘렸다 (84 / 필요 98). 가로 정책만 바꾸고, 창 크기 힌트를 최소 폭 이상으로 두어 초기 관리자 안내가 360×240으로 뜬다 (본문 292×118)
- `QIconEngine`의 기본 `pixmap`은 배경을 지우지 않아 세 OS에서 아이콘에 잡음이 생겼다 → `pixmap` · `scaledPixmap`을 투명 배경 · 화면 배율로 재정의
- clang-tidy 이름 검사가 Qt 클래스 전방 선언(`class QLabel;`)에 `Sage` 접두사를 요구해 `ClassIgnoredRegexp: '^Q[A-Z][A-Za-z]*$'` 추가. clazy `qproperty-without-notify` → `variantChanged` signal
- DB 준비 실패 안내는 코드 경로만 연결했다 — 실제 실패를 띄워 보지 않았다. 부모 기준 가운데는 부모 없는 시작 안내라 화면 가운데로만 확인했다 (부모 있는 경우는 T10)
- 사용자 결정 (2026-09-28): 버튼 hover · 포커스 표시 없음(SageSDI와 같다), 본문 200px 초과 시 말줄임 없이 자른다. 캡션 닫기 버튼의 흰 사각형 · 96 DPI 1:1 크기 비교는 T10에서 사용자가 SageSDI 화면과 대조한다

## 결과
- 작업 브랜치 CI(build · static-analysis · screenshots) 통과 후 `develop`에 squash merge. PR 없음
- `ui/dialogs/`: `SageFramelessDlg`(1px 테두리 `QFrame` · 캡션 바 · 끌어서 이동 · Esc/닫기 = 취소 · 크기 힌트 ≥ 최소 폭), `SageMessageBoxDlg`(정보 · 경고 · 오류, 「확인」 Primary 1개, 평문 본문). `ui/widgets/`: `SageButton`(변형 `Secondary` · `Primary`), `SageDialogCaptionBar`. `ui/style/`: `SageIconEngine`, `SageStyle` 버튼 · 도구 버튼 · 테두리 · 표준 아이콘
- `main.cpp`: DB 준비 실패 · 관리자 생성 실패는 오류 상자 후 종료, 초기 관리자 비밀번호는 알림 상자
- 테스트: `tests/ui/dialogs/SageMessageBoxDlgTest`, `SageStyleTest` 버튼 · 아이콘, 폰트 `BodyStrong`. 로컬 15/15
- `screenshots.yml`을 `tools/dialog-capture/capture.py`(스크린샷 → 캡션 색 띠 찾기 → 가짜 끌기 → 위치 비교 → Enter)로 교체. 스크린샷 `docs/screenshots/T09/`
- 교훈
  - 스킬 문서의 규격도 착수 때 원본 코드와 다시 대조해야 한다 — T07 문서에서 Primary Bold · 캡션 9pt · 흰색 상수 없음이 틀렸다
  - "눈으로 확인"의 일부는 CI에서 잴 수 있다 — 끌기 이동은 스크린샷 전후 비교로, 크기 · 색은 offscreen `grab()`으로 확인했다
