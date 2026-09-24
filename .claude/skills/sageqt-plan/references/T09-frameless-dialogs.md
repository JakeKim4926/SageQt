# T09 — 프레임리스 다이얼로그 · 메시지 상자

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 프레임리스 다이얼로그 기반과 메시지 상자를 옮기고, T06에서 미뤄 둔 시작 시 안내(DB 오류 · 초기 관리자 비밀번호)를 이 메시지 상자로 연결한다.

## 시작 전에
1. 선행 주제: T08
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/ui-composition.md` (화면 클래스 작성 형태) · `style.md`, `coding-rules/references/api-shape.md` · `ownership-and-threads.md` (모달은 스택 값), `sageqt-ui`의 다이얼로그 규격
3. 결정 — 없음 (`sageqt-ui` 규격을 따른다)
4. 재확인할 사실
   - Qt 6.11의 `QWindow::startSystemMove()` 지원 범위 (Windows · macOS · X11 · Wayland)
   - Mac mini 준비 여부 — macOS에서 프레임리스 창의 이동 · 모양을 눈으로 확인해야 한다

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/ui/`

**프레임리스 기반** — `dialogs/SageFramelessDialog.cpp`
- 창 스타일 `WS_POPUP | WS_BORDER | DS_SETFONT | DS_CENTER` → 테두리 1px, 제목 표시줄 없음, 부모 기준 가운데
- 크기 고정 (리사이즈 경로 없음)
- 상단 캡션 영역(`y < SAGE_DLG_CAPTION_HEIGHT`)을 끌면 창이 이동 (`OnNcHitTest` → `HTCAPTION`)
- 캡션 바는 클릭을 통과시킨다 (`drawing/SageDialogCaptionBar.cpp` `OnNcHitTest` → `HTTRANSPARENT`)
- 캡션의 닫기 버튼 → 취소와 같음 (`OnCaptionClose` → `IDCANCEL`)
- 규격: 캡션 높이 40 · 좌우 여백 16 · 버튼 28 · 버튼 여백 8 (`SageDefine.h:112-115`), 캡션 폰트 `SAGE_CAPTION_FONT_POINT_SIZE = 90` (9.0pt)

**메시지 상자** — `dialogs/SageMessageBoxDlg.h/.cpp`
- 생성: 메시지 · 종류 · 부모. 편의 함수 `ShowSageMessageBox(text, type = MB_OK, parent = NULL)`
- 종류는 Win32 플래그로 받는다: 아이콘 `MB_ICONERROR` · `MB_ICONWARNING` · `MB_ICONINFORMATION`, 확인형 여부(`IsConfirm`), 기본 버튼이 거부인지(`IsDefaultReject`)
- 버튼 2개(수락 · 거부), 본문(`drawing/SageMessageBody`), 아이콘(`SageMessageIcon`), 캡션 제목은 종류에 따라 (`GetCaptionTitle`)
- 규격: 폭 360, 본문 최대 높이 200, 아이콘 22 · 반지름 10 · 아이콘-본문 간격 12 (`SageDefine.h:119-130`의 `SAGE_MSGBOX_*` 전체)
- 앱 전체 호출: 오류(`MB_ICONERROR`) 2곳, 경고(`MB_ICONWARNING`) 5곳, 정보(`MB_ICONINFORMATION`) 1곳 (2026-09-23)

**시작 시 안내** (T06에서 연결 대기)
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
- [ ] 프레임리스 다이얼로그 기반: 캡션 영역 · 제목 · 닫기 버튼 · 끌어서 이동 · 고정 크기 · 부모 기준 가운데
- [ ] 메시지 상자: 아이콘 종류 · 버튼 구성 `enum class`, 본문, 규격
- [ ] `main.cpp`의 시작 시 안내 두 가지를 메시지 상자로 연결
- [ ] Windows · Linux · macOS에서 이동 · Esc · Enter 확인, 스크린샷

## 완료 기준
- 3-OS CI 통과
- 세 OS에서 캡션을 끌면 창이 이동한다 (Linux는 X11 · Wayland 각각, 확인한 환경을 기록)
- Esc로 닫히고 Enter로 기본 버튼이 눌린다
- DB 준비 실패와 초기 비밀번호 안내가 이 메시지 상자로 뜬다
- `MB_` · `DLGTEMPLATE`가 SageQt 코드에 0개다

## 범위 밖
- 로그인 · 비밀번호 변경 다이얼로그 — T10

## 확인한 사실
(진행 중 기록)
