# 기술부채 로그 (DEBT_LOG)

이번 작업 범위 밖이라 남겨둔 위험 요소를 기록한다. 즉시 해결이 아니라 추적이 목적이다.
해결한 항목은 `## 해결됨` 섹션으로 옮긴다.

## 열린 항목

### [2026-09-27] 검증누락 — Linux 실제 화면(X11 · Wayland)에서 폰트 메트릭을 재지 않았다
- 위치: .github/workflows/font-metrics.yml (Linux는 offscreen만)
- 설명: Windows · macOS는 실제 플랫폼을 쟀지만 Linux 러너에는 디스플레이가 없어 offscreen만 쟀다. Windows에서는 offscreen의 영문 · 숫자 폭이 실제보다 최대 12.5% 넓었다.
- 위험도: 낮음 — Linux offscreen 값이 macOS 실제(픽셀 지정)와 99개 모두 같아 큰 차이는 없을 것으로 보이지만 확인하지 않았다
- 후속: T08 화면 확인 때 Linux(WSLg 등) 실제 플랫폼에서 측정 도구를 한 번 돌린다

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
