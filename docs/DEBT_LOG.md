# 기술부채 로그 (DEBT_LOG)

이번 작업 범위 밖이라 남겨둔 위험 요소를 기록한다. 즉시 해결이 아니라 추적이 목적이다.
해결한 항목은 `## 해결됨` 섹션으로 옮긴다.

## 열린 항목

### [2026-09-29] 검증누락 — 실제 OS 파일 끌어 놓기와 파일 · 폴더 선택 창을 사람이 확인하지 않았다
- 위치: SageQt/ui/ui/window/SageFileDropFilter.cpp, SageQt/ui/ui/panels/SageWorkflowInputPanel.cpp
- 설명: 드롭은 Qt 드래그 이벤트를 위젯에 보내는 테스트로만 확인했다 (탐색기 · Finder · 파일 관리자에서 실제로 끌어 놓지 않았다). `QFileDialog` 파일 · 폴더 선택 창은 CI에서 띄워 조작할 수 없어 확인하지 않았다 (macOS 시트 여부 포함)
- 위험도: 중 — 입력 경로를 넣는 두 경로라 동작하지 않으면 업무를 시작할 수 없다
- 후속: 세 OS 실제 화면에서 창의 세 곳에 파일을 끌어 놓고, 파일 · 폴더 선택 창을 한 번씩 띄운다 (Mac mini 준비 뒤 macOS)

### [2026-09-28] 임시구현 — design-values.md 생성 스크립트와 입력 분류표가 저장소 밖(임시 폴더)에 있다
- 위치: .claude/skills/sageqt-ui/references/design-values.md (생성물), 생성 스크립트 gen_design_values.py · t07-constants.md (T07 세션의 임시 폴더)
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

### [2026-09-27] 검증누락 — Linux 실제 화면(X11 · Wayland)에서 폰트 메트릭을 재지 않았다
- 위치: .github/workflows/font-metrics.yml (Linux는 offscreen만)
- 해결: [2026-09-28] T08 — font-metrics에 Linux xcb(Xvfb) 행을 추가했다. xcb 값이 offscreen과 모두 같다 (실행 36329540869). Wayland는 재지 않았다 — 러너에 Wayland 컴포지터가 없다
- 설명: Windows · macOS는 실제 플랫폼을 쟀지만 Linux 러너에는 디스플레이가 없어 offscreen만 쟀다. Windows에서는 offscreen의 영문 · 숫자 폭이 실제보다 최대 12.5% 넓었다.
- 위험도: 낮음 — Linux offscreen 값이 macOS 실제(픽셀 지정)와 99개 모두 같아 큰 차이는 없을 것으로 보이지만 확인하지 않았다
- 후속: T08 화면 확인 때 Linux(WSLg 등) 실제 플랫폼에서 측정 도구를 한 번 돌린다
