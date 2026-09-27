# PR 작업 로그

PR을 생성하거나 머지할 때마다 아래 형식으로 기록한다. 형식은 `git-workflow` 스킬을 따른다.

```markdown
## [yyyy-mm-dd] 브랜치명
- **목적**: 무엇을 위한 작업인지
- **변경 내용**: 주요 작업 요약
- **PR 링크**: (있으면)
- **결과**: merged / closed / pending
```

---

## [2026-09-27] chore/ci-guards
- **목적**: PR 없이 머지할 때도 머지 전에 CI로 검증하고, 주석 금지 규칙을 도구로 검사한다
- **변경 내용**: `build.yml` · `font-metrics.yml`의 push 트리거를 모든 브랜치(`'**'`)로. 정적 분석 job에 주석 grep(`//` · `/*`, `://` 제외) 단계. `format-and-tools.md` 도구 표 갱신. 위반 검출 테스트: 주석 커밋이 "Check comments" 단계만 실패시킴 (실행 36318817652), 되돌린 뒤 36319310012 통과
- **PR 링크**: 없음 — `develop`에 직접 squash merge
- **결과**: merged (develop, 2026-09-27)

## [2026-09-27] feature/db-infra
- **목적**: SageSDI의 SQLite 연결 · 스키마 준비를 QtSql로 옮긴다 (sageqt-plan T04)
- **변경 내용**: `sage_infra` — `SageDbConfig`(기본 경로 = 앱 데이터 폴더 `sageqt.db`, busy timeout 5000ms) · `SageDbConnection`(고유 연결 이름 · 폴더 생성 · `PRAGMA foreign_keys` · RAII 정리) · `SageSchemaInitializer`(`SageUser`). 테스트 3개. 앱 식별 정보 · 레거시 마이그레이션 생략 결정 기록. `.clang-format` `FixNamespaceComments: false`와 T03 테스트의 namespace 주석 제거. T04 완료 처리
- **PR 링크**: 없음 — `develop`에 직접 squash merge
- **결과**: merged (develop, 2026-09-27)

## [2026-09-27] feature/workflow-core
- **목적**: SageSDI 워크플로 모델을 `sage_core`로 옮기고 Qt Test 기반을 만든다 (sageqt-plan T03)
- **변경 내용**: `sage_core` — 핸들러 인터페이스(`QJsonObject` · `QList` · `enum class`, 사이드바 라벨 · 분류 추가) · 탭 · 결과 표 타입 · 결과 행 · 응답 · 결과 변환 · 등록부 · 샘플 핸들러(요청 ID `sample-run`). `tests/` 기반과 테스트 4개(17건), 테스트 프리셋 6개, CI `ctest` 단계. 규칙 3건(인터페이스 기본 소멸자 · 테스트 추가 위치 · 테스트 데이터 리터럴). T03 완료 처리
- **PR 링크**: 없음 — 사용자 결정으로 `develop`에 직접 squash merge
- **결과**: merged (develop, 2026-09-27)

## [2026-09-27] chore/font-metrics-probe
- **목적**: SageSDI 번들 폰트가 세 OS의 Qt에서 어떤 이름 · 크기로 그려지는지 실측해 T07 레이아웃 정책의 근거를 만든다 (sageqt-plan T02)
- **변경 내용**: `coding-design`에 `tools/` · `SAGE_BUILD_TOOLS` 규칙. `SageQt/resources/`에 폰트 6개 + OFL 라이선스 2개. 측정 도구 `tools/font-probe`(역할 11종 × 문자열 9개, 포인트 · 픽셀 크기). `font-metrics.yml`로 Windows · macOS 실제 플랫폼 + 3 OS offscreen 측정. 결과 — GDI식 폰트 이름은 환경마다 다른 폰트로 잡힘, macOS는 72 DPI라 포인트 크기로 약 25% 작고 픽셀 크기로는 ±4% 안. T02 완료 처리
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/10
- **결과**: merged (develop, 2026-09-27)

## [2026-09-27] chore/static-analysis
- **목적**: 코딩 규칙을 CI가 검사하게 한다 (sageqt-plan T01)
- **변경 내용**: `.clang-format`(포맷 규칙 6개) · `.clang-tidy`(네이밍 · `NULL` · C 캐스트 · `explicit` · `override` · 멤버 함수 `const`) 추가. CI `static-analysis` job — clang-format 22.1.3, clang-tidy 22, clazy 1.17.1(LLVM 22 소스 빌드), 플랫폼 분기 · `auto` grep. 빌드 job 6개 경고 에러화. 위반 검출 테스트 5개로 각 검사가 실패하는 것을 확인. `format-and-tools.md` 도구 표 수정, `DEBT_LOG.md` 신설(2건), T01 완료 처리
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/9
- **결과**: merged (develop, 2026-09-27)

## [2026-09-24] docs/plan-restructure
- **목적**: 개발 계획을 구현 가능한 주제 단위로 나누고, 진행 중인 주제의 지시서만 읽도록 계획 관리 구조를 바꿈
- **변경 내용**: `sageqt-plan`을 절차 + 진행 중인 주제 목록(T01~T19)으로 재작성, 주제별 구현 지시서 `references/Txx-*.md` 19개 작성(SageSDI 원본 사실을 값 · 경로 · 줄 번호까지 기록), 끝난 주제는 `docs/plans/done/`으로 옮기는 절차. `MIGRATION_PLAN.md`를 배경 · 결정 기록 문서로 재구성하고 `.rc`(UTF-16LE) 사실 정정. CLAUDE.md와 스킬 3곳이 새 구조를 가리키도록 갱신
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/8
- **결과**: merged (develop, 2026-09-24)

## [2026-09-23] chore/ci-3os-build
- **목적**: "코드는 하나"를 CI가 커밋마다 세 OS에서 검증하게 함 (PR 0-4)
- **변경 내용**: GitHub Actions 매트릭스 6개(windows-2025 · macos-15 · ubuntu-24.04 × Debug · Release), Qt 6.11.2 설치, 외부 action 커밋 해시 고정. Windows 설치 실패 원인(Qt 6.11 Windows 저장소 구조 변경, aqtinstall 3.3.0 미지원)을 찾아 aqtinstall main 커밋 `076e165`로 고정. 6개 모두 통과, 경고 0
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/7
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] chore/cmake-presets-3os
- **목적**: OS별 차이를 CMake 프리셋에 두기 위해 세 OS 프리셋을 갖춤 (PR 0-3)
- **변경 내용**: `macos-arm64` · `linux-x64` 기본 프리셋 추가(호스트 OS 조건), 프리셋 이름을 `<OS>-<아키텍처>-<구성>`으로 통일, 예시 파일 갱신. Windows 두 프리셋 빌드 경고 0, 예시 파일 동작 확인
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/6
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] refactor/cmake-layer-targets
- **목적**: VS 템플릿 구조를 SageQt 계층 타깃 구조로 교체 (PR 0-2)
- **변경 내용**: 템플릿 파일 4개 제거. CMake 3.22 · C++20 · Qt 6.11 정책 · 경고 수준 상향. `sage_define` · `sage_ui`(`SageMainWindow`) · 실행 파일, 타깃별 include 루트 격리, `main.cpp` 조립 지점. Windows Debug · Release 경고 0, 실행 확인, 격리 음성 테스트(링크 안 한 헤더 include 시 C1083) 확인
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/5
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] chore/gitattributes
- **목적**: 줄 끝 규칙을 PC 설정이 아니라 저장소가 정하도록 고정 (PR 0-1, 리스크 #9)
- **변경 내용**: `.gitattributes`에 `* text=auto eol=lf` 추가. 저장소 안은 이미 LF라 내용 변경 없음, 적용 후 추적 파일 33개 모두 `i/lf w/lf`
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/4
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] docs/step0-plan
- **목적**: 이관 Step 0 착수 전 계획 상세화와 결정 반영 (PR 0-0)
- **변경 내용**: `MIGRATION_PLAN.md`에 Step 0 작업 순서 · 완료 기준 · 범위 밖 기록, 결정 3건(계층 타깃은 소스가 생길 때 생성 / 경고 수준 상향 · CI에서만 에러 / `SageQt.slnx` 유지) 반영. 컴파일 경고 규칙과 `.slnx` 예외를 스킬과 리뷰 점검 항목에 추가
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/3
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] docs/skills-residue-cleanup
- **목적**: 스킬에 남은 SageSDI 사례 · 예시를 정리해 스킬을 SageQt의 영구 규칙으로만 유지
- **변경 내용**: `coding-design` 참조 파일의 SageSDI 사례 · 비교 제거. `git-workflow` · `sageqt-plan` · `debt-log-guard` 예시를 SageQt 기준으로 교체. SageQt 규칙과 모순되던 예시 2건(PCH 경로, UTF-8 컴파일러 옵션) 교체. 규칙 변경 없음
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/2
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] docs/sageqt-skills
- **목적**: SageSDI(MFC) 이관을 시작하기 전에 코드 규칙 · 프로젝트 목표 · 이관 계획을 확정 (Step -1)
- **변경 내용**: `CLAUDE.md`에 프로젝트 목표 추가. 스킬 6종 추가 — `coding-rules` · `coding-design` · `code-review-expert`는 Qt 6 기준으로 재작성, `git-workflow` · `sageqt-plan` · `debt-log-guard`는 이식, SKILL.md 300줄 이하 + `references/` 분리. `MIGRATION_PLAN.md` 추가 및 Step -1 완료 반영
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/1
- **결과**: merged (develop, 2026-09-23)
