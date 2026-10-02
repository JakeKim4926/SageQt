# 기술부채 로그 (DEBT_LOG)

이번 작업 범위 밖이라 남겨둔 위험 요소를 기록한다. 즉시 해결이 아니라 추적이 목적이다.
해결한 항목은 `## 해결됨` 섹션으로 옮긴다.

## 열린 항목

### [2026-10-02] 검증누락 — 샘플 업무 전 구간(입력 → 실행 → 결과 → 기록)을 사람이 세 OS에서 실행해 보지 않았다
- 위치: SageQt/ui/ui/panels/SageWorkspacePanel.cpp, SageWorkflowHistoryPanel.cpp, SageQt/ui/ui/window/SageMainWindow.cpp
- 설명: T16 완료 기준의 "세 OS에서 샘플 업무 전 구간 동작 (Mac mini 포함, 확인 표)"은 세 OS CI의 오프스크린 테스트(`SageWorkspacePanelTest` — 실행 · 결과 표 · 상태 카드 · 기록 · 상태 문자열)로만 확인했다. CI 스크린샷은 실행 전 화면(입력 · 결과 · 기록 탭)뿐이다 — 파일 선택 창을 CI에서 조작할 수 없어 실제 실행 후 화면은 없다. Mac mini가 아직 없다
- 위험도: 중 — UI 이관의 마지막 확인이다. 실제 실행 후 결과 표 · 기록 표 · 상태 표시줄을 사람이 보지 않았다
- 후속: Mac mini 준비 뒤 세 OS에서 샘플 업무를 한 번씩 실행하고 확인 표(사이드바 → 입력 → 실행 → 결과 → 기록 · 상태 표시줄)를 채운다. 파일 끌어 놓기 · 선택 창 확인(2026-09-29 항목)과 함께 한다

### [2026-10-02] 가정 — 실행 기록의 파일별 행(`files` 배열) 위치를 `payload.files`로 가정했다
- 위치: SageQt/core/core/workflow/SageWorkflowHistory.cpp
- 설명: SageSDI는 응답 JSON 문자열에서 `"files"`를 처음 찾는 곳을 썼다. 이 배열을 내는 핸들러가 두 앱 모두 없어 실제 위치를 확인할 수 없었다
- 위험도: 낮음 — 지금 업무에는 영향이 없다. `files`를 내는 업무를 붙이면 기록이 파일별로 나뉘지 않을 수 있다
- 후속: 파일별 결과를 내는 업무를 추가하는 주제에서 응답 형식을 정하고 `SageWorkflowHistoryTest`를 맞춘다

### [2026-10-01] 검증누락 — 입력 표 · 검색 · 선택 막대 · 요약 · 합계를 실제 업무로 확인하지 않았다
- 위치: SageQt/ui/ui/panels/SageResultTablePanel.cpp, SageQt/ui/ui/panels/SageWorkspacePanel.cpp, SageQt/ui/ui/widgets/SageSearchBox.cpp · SageSelectionBar.cpp · SageSummaryBar.cpp · SageTableTotalBar.cpp
- 설명: 두 앱의 유일한 업무(샘플)는 이 기능들을 켜지 않는다. 테스트용 핸들러(오프스크린)로만 확인했고, 세 OS 실제 화면은 빈 결과 표와 빈 상태 안내뿐이다. 기준 콤보를 펼친 목록 · 검색창 포커스 · 가로 스크롤 시 합계 칸 맞춤은 실제 화면에서 보지 않았다
- 위험도: 중 — 입력 표 업무를 처음 붙일 때 모양 · 동작 차이가 한꺼번에 드러날 수 있다
- 후속: 입력 표 업무를 추가하는 주제에서 세 OS 실제 화면으로 확인한다 (입력 표 · 선택 · 검색 · 요약 · 합계 · 업무 전환 복원)

### [2026-10-01] 검증누락 — 표에서 마우스가 나갈 때 hover 행이 풀리는지 확인하지 않았다
- 위치: SageQt/ui/ui/panels/SageResultTablePanel.cpp (`eventFilter`의 `QEvent::Leave`)
- 설명: 오프스크린 테스트에서는 viewport `Leave` 이벤트를 만들 수 없어 hover 해제를 테스트하지 않았다. 행이 바뀔 때(리셋 · 필터)의 해제는 코드로만 넣었다
- 위험도: 낮음 — 틀리면 마우스가 표 밖으로 나가도 마지막 행이 옅게 남는다
- 후속: 실제 화면에서 결과 표에 행이 있을 때 마우스를 넣었다 빼 본다

### [2026-09-29] 검증누락 — 실제 OS 파일 끌어 놓기와 파일 · 폴더 선택 창을 사람이 확인하지 않았다
- 위치: SageQt/ui/ui/window/SageFileDropFilter.cpp, SageQt/ui/ui/panels/SageWorkflowInputPanel.cpp
- 설명: 드롭은 Qt 드래그 이벤트를 위젯에 보내는 테스트로만 확인했다 (탐색기 · Finder · 파일 관리자에서 실제로 끌어 놓지 않았다). `QFileDialog` 파일 · 폴더 선택 창은 CI에서 띄워 조작할 수 없어 확인하지 않았다 (macOS 시트 여부 포함)
- 위험도: 중 — 입력 경로를 넣는 두 경로라 동작하지 않으면 업무를 시작할 수 없다
- 후속: 세 OS 실제 화면에서 창의 세 곳에 파일을 끌어 놓고, 파일 · 폴더 선택 창을 한 번씩 띄운다 (Mac mini 준비 뒤 macOS)

### [2026-09-28] 임시구현 — design-values.md 생성 스크립트와 입력 분류표가 저장소 밖(임시 폴더)에 있다
- 위치: .claude/skills/sageqt-ui/references/design-values.md (생성물), 생성 스크립트 gen_design_values.py · t07-constants.md (`D:\ClaudeWork\SageQt\scratch\29665854-ab1b-4256-8f1f-688b9eeceb19\`)
- 설명: T07부터 design-values.md를 스크립트로 생성해 왔는데, 스크립트와 입력 분류표가 세션 임시 폴더에만 있다. 임시 폴더가 지워지면 표를 다시 생성할 수 없고, 손으로 고치면 생성 규칙과 어긋난다
- 위험도: 중 — 다음 규격 수정 때 생성 경로를 잃을 수 있다
- 후속: 스크립트와 분류표를 tools/design-values/로 옮기거나, 생성을 그만두고 표를 직접 관리하는 쪽으로 정한다 (사용자 확인)

### [2026-09-28] 검증누락 — Linux에서 캡션 끌어서 이동이 요청한 거리만큼 움직이는지 확인하지 못했다
- 위치: SageQt/ui/ui/widgets/SageDialogCaptionBar.cpp (`startSystemMove`), tools/dialog-capture/capture.py
- 설명: CI(Xvfb + openbox + pyautogui)에서 (150, 100)을 끌면 창이 (600, 340) 움직여 화면 끝으로 밀려난다 (실행 36372865888 · 36373636749, 두 번 같음). Windows · macOS는 같은 스크립트로 정확히 (150, 100) 움직였다. 앱 문제인지 가짜 창 관리자 · 가짜 입력 문제인지 가르지 못했다. Wayland는 CI 환경이 없어 재지 않았다
- 위험도: 중 — Linux 사용자가 다이얼로그를 옮길 때 창이 튈 수 있다
- 후속: 실제 Linux 데스크톱(X11 · Wayland 각각)에서 메시지 상자를 끌어 본다. 튀면 `startSystemMove`의 X11 경로를 조사한다

### [2026-09-27] 검증누락 — Windows CI의 Qt 새 설치가 간헐적으로 실패한다
- 위치: .github/workflows/build.yml (Install Qt, aqtinstall 076e165 + py7zr 1.1.3)
- 설명: 캐시가 없을 때 aqtinstall이 py7zr Bad7zFile로 실패한 적이 있다 (실행 36318817652 windows-x64-debug). 같은 조건의 release job은 성공했다. 캐시는 브랜치 범위라 새 브랜치의 첫 실행은 항상 새로 설치한다.
- 위험도: 중 — 코드와 무관한 실패가 머지 전 검증을 막을 수 있다
- 재발: [2026-09-28] screenshots 워크플로 windows job에서 같은 오류 (실행 36329540896, 재실행으로 통과)
- 재발: [2026-09-28] screenshots windows job 세 번째 (실행 36402551588, 재실행으로 통과) — 모두 캐시 없는 새 브랜치 첫 실행
- 후속: 두 번 나왔다 — py7zr 버전 고정이나 재시도를 검토한다. aqtinstall 정식 릴리스 전환(기존 DEBT)과 함께 본다

### [2026-09-27] 임시구현 — CI가 매 실행마다 clazy를 소스에서 빌드한다
- 위치: .github/workflows/build.yml (Build clazy 단계)
- 설명: Ubuntu apt의 clazy 1.11은 GCC 14 헤더를 파싱하지 못해 v1.17.1을 LLVM 22로 매번 빌드한다. 정적 분석 job이 약 2분 30초 길어진다.
- 위험도: 낮음 — 결과는 맞고 시간만 든다
- 후속: 빌드 결과를 캐시하거나, 배포판에 LLVM 22 기반 clazy가 나오면 apt로 되돌린다

### [2026-09-23] 임시구현 — CI가 정식 릴리스가 아닌 aqtinstall 커밋을 쓴다
- 위치: .github/workflows/build.yml (AQT_SOURCE)
- 설명: Qt 6.11의 새 Windows 저장소 구조를 aqtinstall 3.3.0이 지원하지 않아 main 커밋 076e165(PR #1000 포함)로 고정했다. 정식 릴리스가 아니다.
- 위험도: 중 — 고정 커밋이라 갑자기 깨지진 않지만, 릴리스 전 코드라 검증 범위가 좁다
- 후속: aqtinstall 3.4.0 이상이 릴리스되면 aqtsource를 지우고 aqtversion으로 되돌린다

## 해결됨

### [2026-10-02] 임시구현 — 앱 아이콘이 없다 (고해상도 원본 대기)
- 위치: SageQt/packaging/ (windows `SageQt.rc.in` · linux `SageQt.desktop.in`), SageQt/CMakeLists.txt (macOS `.icns`)
- 설명: SageSDI 아이콘은 32×32 한 장뿐이라 macOS(최대 1024) · Linux(256)에 부족하다. 사용자가 고해상도 원본을 주기로 했다 (T17 결정). 그전까지 세 OS 모두 기본 아이콘이다
- 위험도: 중 — 배포 전에 반드시 채워야 한다
- 후속: 원본(1024 PNG 또는 SVG)을 받으면 `.ico`(16~256) · `.icns` · Linux PNG를 만들고 rc · `Info.plist` · `.desktop` · 창 아이콘에 연결한다. T18 착수 전에 확인한다
- 해결: [2026-10-02] T18 — 원본 구성을 1024로 다시 그렸다 (`tools/app-icon/make_app_icon.py`, 사용자 선택). 진짜 고해상도 원본을 찾으면 같은 스크립트 자리를 바꾼다

### [2026-09-27] 검증누락 — Linux 실제 화면(X11 · Wayland)에서 폰트 메트릭을 재지 않았다
- 위치: .github/workflows/font-metrics.yml (Linux는 offscreen만)
- 해결: [2026-09-28] T08 — font-metrics에 Linux xcb(Xvfb) 행을 추가했다. xcb 값이 offscreen과 모두 같다 (실행 36329540869). Wayland는 재지 않았다 — 러너에 Wayland 컴포지터가 없다
- 설명: Windows · macOS는 실제 플랫폼을 쟀지만 Linux 러너에는 디스플레이가 없어 offscreen만 쟀다. Windows에서는 offscreen의 영문 · 숫자 폭이 실제보다 최대 12.5% 넓었다.
- 위험도: 낮음 — Linux offscreen 값이 macOS 실제(픽셀 지정)와 99개 모두 같아 큰 차이는 없을 것으로 보이지만 확인하지 않았다
- 후속: T08 화면 확인 때 Linux(WSLg 등) 실제 플랫폼에서 측정 도구를 한 번 돌린다
