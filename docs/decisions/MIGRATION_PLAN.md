# SageSDI → SageQt 이관 계획

> 미검증 커밋: 없음 (Step -1은 문서 작업이며 아직 커밋 전)

## Context

SageSDI는 Windows 전용 MFC 앱이다. 목표는 macOS에서 쓰는 것이고, Windows · Linux까지 **같은 코드**로 지원한다.
프로젝트 목표 세 가지는 `CLAUDE.md`의 *프로젝트 목표*에 있다 — 세 OS에서 코드 동일, 디자인 자유도, 품질 = 일관성.
이 문서의 모든 Step은 그 목표로 되돌아가 검증한다.

### 조사로 확정한 사실 (2026-09-22 ~ 23)

| 항목 | 사실 | 확인 위치 |
|---|---|---|
| SageSDI 규모 | 직접 작성 코드 12,348줄 (SQLite 소스 제외). 비UI 계층(core · infra · common) 2,687줄, ui 8,805줄, 루트 856줄 | `D:/Projects/SageSDI/SageSDI/app/` |
| 실제 업무 | 핸들러 0개 — 배선 확인용 샘플 1개뿐 | `core/workflow/handlers/`, `docs/RELEASE_NOTES.md` v1.0 (2026-09-01) |
| Windows 전용 API | 비UI 계층에서는 5개 파일 620줄에 집중 | `SageDialogHelper` · `SageFileUtils` · `SageAppSettingsService` · `SageUserService` · `SqlInitializer` |
| 다이얼로그 템플릿 | `.rc`에 DIALOGEX 0개 — UI는 전부 코드로 생성 | `SageSDI.rc` |
| MFC Feature Pack | 미사용 (`CMFC*` 2건은 VS가 생성한 검색 핸들러 코드) | `ui/frame/SageSDIDoc.cpp:97` |
| 프레임리스 | 메인 창은 네이티브 타이틀바. 프레임리스는 다이얼로그 3개뿐 — 고정 크기, 상단 40px 드래그, 닫기 버튼 | `ui/dialogs/SageFramelessDialog.cpp`, `SageDefine.h:112` |
| 계층 위반 | README는 `ui → core ← infra`라고 하지만 core가 infra를 include한다. core에 Win32 `::MulDiv`와 픽셀 폭 계산이 있다 | `core/auth/SageUserService.h:5`, `core/workflow/SageWorkflowResultTable.cpp:49` |
| 전역 상태 | DB · 세션이 매크로 싱글턴 | `#define sageDBMgr`, `#define sageAuth` |
| 결과 표 | 패널이 행 · 보이는 행 · 검색어 · 필터 · 체크 상태를 직접 보관 | `ui/panels/SageResultTablePanel.h` |
| SQLite 사용 | prepare · bind · step · finalize · `busy_timeout` · `last_insert_rowid` 등 기본 API뿐 | `infra/db/` |
| 비밀번호 | 솔트 없는 단일 SHA-256 | `core/auth/SageUserService.cpp` `HashPassword` |
| 로컬 Qt | `D:/Qt/6.11.2/msvc2022_64`. MSVC `-utf-8`은 Qt가 자동으로 붙인다. `qsqlite.dll` 설치됨 | `lib/cmake/Qt6/Qt6Targets.cmake:64`, `plugins/sqldrivers/` |
| SageQt 현재 코드 | VS 템플릿 그대로 — C++17, `.ui` 파일, Qt5 호환 `qt.cmake`. 새 규칙과 어긋난다 | `SageQt/` |

### 확정한 결정

| 날짜 | 결정 | 근거 |
|---|---|---|
| 2026-09-22 | 단일 Qt 코드베이스로 가고 MFC는 은퇴한다 | 업무가 0개라 지금이 가장 싸다. 두 벌이면 업무를 추가할 때마다 두 번 만든다 |
| 2026-09-22 | Excel 자동화는 이 계획의 범위 밖 | 사용자가 다른 방식으로 시도하기로 함 |
| 2026-09-23 | Qt Widgets (QML 아님) | 앱의 중심이 결과 표이고, `QTableView`가 QML `TableView`보다 성숙하다 |
| 2026-09-23 | 네이밍은 Qt 스타일 — camelCase 함수, `m_` 유지, 헝가리안 · `get` 제거 | Qt 오버라이드가 camelCase를 강제하므로, 맞추지 않으면 한 클래스에 두 스타일이 섞인다 |
| 2026-09-23 | 계층마다 CMake 타깃, 타깃마다 include 루트를 격리 (`core/core/`) | 계층 위반이 컴파일 에러가 된다. SageSDI의 실제 위반 두 건이 필요성을 보여줬다 |
| 2026-09-23 | 스타일은 `SageStyle`(Fusion 기반 `QProxyStyle`) + `QPalette`, QSS 금지 | 디자인 값을 C++ 한 곳에 둘 수 있다. 플랫폼 모양을 벗어나는 디자인에는 QStyle이 권장된다 (KDAB, Qt 포럼) |
| 2026-09-23 | DB는 QtSql `QSQLITE` | SageSDI의 사용 범위를 전부 대체하고, 코딩 규칙에서 C API 예외가 사라진다 |
| 2026-09-23 | 백그라운드 작업은 `QtConcurrent::run` + `QFutureWatcher` | 한 번 돌고 끝나는 작업에 Qt 공식 문서가 권장하는 조합 |
| 2026-09-23 | 스킬은 SKILL.md 300줄 이하 + `references/`, 상황별 요약(작성 · 수정 · 삭제) | 코드를 건드릴 때마다 읽히는 분량을 줄이고, 삭제 규칙의 빈틈을 메운다 |

---

## 진행 상황

| Step | 내용 | 상태 |
|---|---|---|
| -1 | 규칙(스킬) · 목표 · 계획 문서 | 진행 중 (커밋 전) |
| 0 | 기반 — 템플릿 정리 · CMake 타깃 · 3-OS 프리셋 · CI · 정적 분석 | 대기 |
| 1 | 측정 — 한글 폰트 메트릭 3-OS 비교 | 대기 |
| 2 | 로직 이관 — core · infra + 테스트 | 대기 |
| 3 | UI 기반 — `sageqt-ui` 스킬 · `SageStyle` · 프레임리스 다이얼로그 | 대기 |
| 4 | UI 이관 — 사이드바 → 헤더 → 결과 표 → 실행 기록 | 대기 |
| 5 | 배포 — 서명 · 공증 · 패키징 | 대기 |

### Step 간 의존

```
-1 → 0     규칙이 먼저 정해져야 첫 코드(CMake · 템플릿 정리)가 규칙을 따른다
 0 → 전부  3-OS CI가 있어야 "코드 동일"이 커밋마다 검증된다
 1 → 3     폰트 메트릭 차이의 크기가 레이아웃과 디자인 값 설계를 좌우한다
 2 → 4     UI는 core의 핸들러 · DTO를 소비한다
 3 → 4     SageStyle과 디자인 값이 있어야 화면을 옮길 수 있다
 4 → 5     배포할 앱이 있어야 한다
```

외부 준비물: Mac mini — Step 4 전까지 / Apple Developer Program — Step 5 전까지 / Windows 코드서명 인증서 — Step 5 전까지

---

## 완료된 작업

아직 없음.

---

## 미해결 리스크

| # | 내용 | 영향 | 대응 |
|---|---|---|---|
| 1 | macOS 실기가 없다 | Step 3~4의 화면 판정 불가 | Step 0~2는 CI macOS 러너, Step 4 전에 Mac mini |
| 2 | 한글 폰트 메트릭의 OS 간 차이를 모른다 | SageSDI 고정 픽셀 규격을 얼마나 다시 해석할지 불명 | Step 1에서 측정 |
| 3 | QStyle 작성 난도가 QSS보다 높다 | Step 3 기간 증가 | `sageqt-ui`에서 `SageStyle`이 재정의할 범위를 먼저 확정 |
| 4 | 프레임리스 다이얼로그의 macOS · Wayland 동작 미검증 | 로그인 창 모양 · 이동 | Step 3에서 `startSystemMove` 검증 |
| 5 | 비밀번호 해싱 방식 미정 (현재 솔트 없는 SHA-256) | 보안 | Step 2에서 결정해 `coding-rules`에 반영 |
| 6 | `sagesdi-ui`(915줄) 미분석 | 디자인 값 · `SageStyle` 설계 근거가 없다 | Step 3 착수 전 분석 |
| 7 | `develop` 브랜치가 없다 | `git-workflow`를 따를 수 없다 | Step -1 커밋 전에 생성 (승인 필요) |
| 8 | 정적 분석 도구 설정 전 | 규칙이 사람의 기억에 의존 | Step 0 |
| 9 | 로컬 WSL은 Ubuntu 20.04 (GCC 9, C++20 부족) | 로컬 Linux 검증 불가 | Ubuntu 24.04 추가 설치 (승인 필요) |

---

## Step -1 — 규칙 · 목표 · 계획

- **브랜치**: `docs/sageqt-skills` (`develop` 생성 후 분기)
- **규칙 출처**: SageSDI `.claude/skills/` 원본 7종, Qt 공식 문서 · 커뮤니티 재검증 결과

작업 순서:
- [x] SageSDI 스킬 분석 및 Qt판 작성 — `coding-rules` · `coding-design` · `code-review-expert` 재작성, `git-workflow` · `sageqt-plan` · `debt-log-guard` 이식
- [x] Qt 공식 문서 · 커뮤니티 기준 재검증 및 반영 — `Q_OBJECT` 전면 적용, QStyle, QtSql, Model/View 등 9건
- [x] SKILL.md 300줄 이하로 분리, 상황별 요약 추가
- [x] `CLAUDE.md`에 프로젝트 목표 추가, 이 계획 문서 작성
- [ ] `develop` 브랜치 생성 (승인 필요)
- [ ] 커밋 → `develop`으로 PR

완료 기준:
- 모든 SKILL.md가 300줄 이하 — `wc -l .claude/skills/*/SKILL.md`
- 재구성 전 규칙 줄이 모두 새 파일에 남아 있다 (제목 수준 · 참조 경로 · 중복 정리처럼 의도한 변경만 예외)
- 스킬 안의 파일 참조가 모두 실제 파일을 가리킨다

범위 밖:
- `sageqt-ui` 스킬 — Step 1 측정 결과를 반영해야 하므로 Step 3 직전에 작성한다
- `.clang-format` · `.clang-tidy` · clazy 설정 파일 — CMake · CI와 함께 있어야 검증할 수 있으므로 Step 0에서 한다

---

## Step 0 — 기반 (착수 시 상세화)

- SageQt 템플릿 정리 — `SageQt.ui` · `qt.cmake` 제거, C++20, 폴더 구조 적용
- CMake 타깃 5개(`sage_define` · `sage_common` · `sage_core` · `sage_infra` · `sage_ui`) + 실행 파일
- `CMakePresets.json` — windows-x64 / macos-arm64 / linux-x64
- GitHub Actions 3-OS 매트릭스 + clang-format · clang-tidy · clazy + 플랫폼 분기(`#ifdef Q_OS_`) 검사

---

## 검증 방법

- Step -1: SKILL.md 줄 수 측정, 재구성 전 스냅샷과의 규칙 줄 대조
- Step 0 이후: 3-OS CI가 모두 통과하는 것이 모든 Step의 기본 조건이다
