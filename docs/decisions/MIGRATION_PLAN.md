# SageSDI → SageQt 이관 — 배경 · 결정 기록

> **지금 할 일은 이 문서가 아니라 `sageqt-plan` 스킬에 있다** (`.claude/skills/sageqt-plan/SKILL.md`의 *진행 중인 주제*).
> 이 문서는 왜 이렇게 하는지 — 배경 · 조사로 확정한 사실 · 결정 · 전역 리스크 · 완료 이력 — 를 기록한다.

## Context

SageSDI는 Windows 전용 MFC 앱이다. 목표는 macOS에서 쓰는 것이고, Windows · Linux까지 **같은 코드**로 지원한다.
프로젝트 목표 세 가지는 `CLAUDE.md`의 *프로젝트 목표*에 있다 — 세 OS에서 코드 동일, 디자인 자유도, 품질 = 일관성.
모든 주제는 그 목표로 되돌아가 검증한다.

---

## 조사로 확정한 사실

주제별 세부 사실(원본 경로 · 상수 값 · 동작 순서)은 각 주제 파일(`sageqt-plan/references/Txx-*.md`)에 있다. 여기에는 전체 판단에 쓰이는 사실만 둔다.

### SageSDI 구조 (2026-09-22 ~ 23)

| 항목 | 사실 | 확인 위치 |
|---|---|---|
| 규모 | 직접 작성 코드 12,348줄 (SQLite 소스 제외). 비UI 계층(core · infra · common) 2,687줄, ui 8,805줄, 루트 856줄 | `D:/Projects/SageSDI/SageSDI/app/` |
| 실제 업무 | 핸들러 0개 — 배선 확인용 샘플 1개뿐 | `core/workflow/handlers/`, `docs/RELEASE_NOTES.md` v1.0 (2026-09-01) |
| 배포 | 배포본 없음 → 옛 DB · 설정과 호환할 필요 없음 | SageSDI `docs/DEBT_LOG.md` |
| Windows 전용 API | 비UI 계층에서는 5개 파일 620줄에 집중. 이 중 `SageAppSettingsService` 전체와 `SageFileUtils` · `SageDialogHelper`의 대부분은 호출 0곳 (아래) | `SageDialogHelper` · `SageFileUtils` · `SageAppSettingsService` · `SageUserService` · `SqlInitializer` |
| 리소스 (`.rc`) | **UTF-16LE 파일이다.** 업무 화면은 전부 코드로 만든다. 리소스에는 VS 마법사가 만든 정보 창 `IDD_ABOUTBOX` 1개(열 방법 없음), 숨겨진 메뉴, **숨겨졌지만 동작하는 단축키 테이블**, 쓰이지 않는 툴바 비트맵, 버전 정보 `2.0.0.1`(회사 · 제품명 `TODO`), 문자열 테이블 5개, TTF 폰트 6개가 있다 | `SageSDI.rc` (iconv로 변환해 확인, 2026-09-24) |
| MFC Feature Pack | 미사용 (`CMFC*` 2건은 VS가 생성한 검색 핸들러 코드) | `ui/frame/SageSDIDoc.cpp:97` |
| 프레임리스 | 메인 창은 네이티브 타이틀바. 프레임리스는 다이얼로그 3개뿐 — 고정 크기, 상단 40px 드래그, 닫기 버튼 | `ui/dialogs/SageFramelessDialog.cpp`, `SageDefine.h:112` |
| 계층 위반 | README는 `ui → core ← infra`라고 하지만 core가 infra를 include한다. core에 Win32 `::MulDiv`와 픽셀 폭 계산이 있다 | `core/auth/SageUserService.h:5`, `core/workflow/SageWorkflowResultTable.cpp:49` |
| 전역 상태 | DB · 세션이 매크로 싱글턴 | `#define sageDBMgr`, `#define sageAuth` |
| 결과 표 | 패널이 행 · 보이는 행 · 검색어 · 필터 · 체크 상태를 직접 보관 | `ui/panels/SageResultTablePanel.h` |
| SQLite 사용 | prepare · bind · step · finalize · `busy_timeout` · `last_insert_rowid` 등 기본 API뿐 | `infra/db/` |
| 비밀번호 | 솔트 없는 단일 SHA-256 | `core/auth/SageUserService.cpp` `HashPassword` |

### MFC가 암묵적으로 하던 일 (2026-09-24)

이관 계획에 반영했다. 각 행의 처리는 해당 주제 파일에 있다.

| 항목 | 사실 | 주제 |
|---|---|---|
| 앱 시작 순서 | DB 준비 → 실패 시 종료 → 첫 실행이면 초기 관리자 비밀번호 1회 안내 → 창 표시 | T06 · T09 |
| DB · 설정 위치 | 실행 파일 옆 폴더 (`GetModuleFileName` 기준, `data/estimate.db`) → macOS `.app` 안 · Windows `Program Files`에서 쓸 수 없다 | T04 |
| 숨은 단축키 | 메뉴는 `SetMenu(NULL)`로 숨겼지만 `CFrameWnd`가 `IDR_MAINFRAME` 단축키를 로드한다. Ctrl+O · Ctrl+S가 MFC 문서 열기 · 저장 창을 띄운다 (의도하지 않은 동작) | T06 (옮기지 않음) |
| 키보드 처리 | 다이얼로그 Enter · Tab, `SageEdit`의 Ctrl+A, 툴팁 중계, 검색창 Enter를 직접 구현 — Qt가 기본으로 한다 | T10 · T15 |
| 파일 드롭 | View · 프레임 · 입력 패널 · 결과 표 패널이 드롭을 받고, View가 모아 입력 경로로 넘긴다 | T13 |
| 진행률 | 실제 진행이 아니라 300ms마다 +3%, 최대 95%, 완료 시 100%인 시간 기반 표시 | T14 |
| 폰트 이름 | `"Pretendard SemiBold"` · `"Gmarket Sans TTF Bold"` 같은 GDI식 패밀리 이름 → 다른 OS · 폰트 엔진에서 다르게 잡힐 수 있다. 크기는 0.1pt 단위 | T02 |
| JSON 처리 | 문자열 검색식이라 중첩(`payload` · `error`)을 무시한다 → `QJsonDocument`로 옮길 때 값이 빌 수 있다 | T03 |
| `PRAGMA foreign_keys` | 연결 단위 설정 → 작업마다 연결을 여는 구조에서는 매번 실행해야 한다 | T04 |
| 실행 기록 | 메모리에만 보관 (앱을 끄면 사라짐) | T16 |
| 죽은 코드 | `SageAppSettingsService`(pdftotext 설정) 호출 0곳. 호출 0곳 함수 8개: `RunProcessAndWait` · `QuoteArgument` · `BuildTempJsonPath` · `CombinePath` · `FolderExists` · `GetAppMainWindow` · `FormatAmountNumber` · `JsonSplitStringArray`. `ShowIFileOpenDialog` 외부 호출 0곳. 레지스트리 · MRU(`SetRegistryKey` · `LoadStdProfileSettings`) 사용 0곳. `SAGE_WORKFLOW_DELIVERY = 2`는 핸들러 없음 | 해당 주제에서 옮기지 않음 |

### 로컬 · CI 환경

| 항목 | 사실 | 확인 위치 |
|---|---|---|
| 로컬 Qt | `D:/Qt/6.11.2/msvc2022_64`. MSVC `-utf-8`은 Qt가 자동으로 붙인다. `qsqlite.dll` 설치됨 | `lib/cmake/Qt6/Qt6Targets.cmake:64`, `plugins/sqldrivers/` |
| Qt 최소 CMake | 3.22 | `QT_SUPPORTED_MIN_CMAKE_VERSION_FOR_USING_QT` |
| 로컬 도구 | VS 번들 CMake 4.3.1 · Ninja 1.13.2 · clang-format / clang-tidy 22.1.3. clazy 없음 | 2026-09-23 |
| CI의 Qt 설치 | Qt 6.11부터 Windows 저장소가 컴파일러별 하위 폴더로 바뀌었고, aqtinstall 3.3.0(최신 릴리스)은 이를 모른다. main 커밋 `076e165`(PR #1000 포함)로 고정해 해결 | PR #7 |

---

## 결정 기록

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
| 2026-09-23 | 계층 타깃은 소스가 생길 때 만든다 — 기반 단계는 `sage_define` · `sage_ui` · 실행 파일만 | CMake는 소스 없는 정적 라이브러리를 허용하지 않고, 빈 껍데기 클래스는 CLAUDE.md 2에 어긋난다 |
| 2026-09-23 | 컴파일 경고 수준을 올리고(`/W4`, `-Wall -Wextra -Wpedantic`) CI에서만 경고를 에러로 처리 | 로컬 작업 흐름은 유지하면서 경고가 남은 코드의 머지를 막는다 |
| 2026-09-23 | `SageQt.slnx`는 유지 — IDE 파일 금지 규칙의 예외 | 사용자 결정. Visual Studio 진입점이며 빌드 설정은 담지 않는다 |
| 2026-09-24 | 계획을 주제 단위 구현 지시서로 관리한다 — `sageqt-plan` SKILL.md는 진행 중인 주제 목록, 주제마다 `references/Txx-*.md`, 끝난 주제는 `docs/plans/done/`으로 | 필요한 주제 파일 하나만 읽게 하고, SageSDI에서 옮길 사실이 세션이 바뀌어도 손상되지 않게 한다 |
| 2026-09-24 | 이관은 SageSDI의 동작을 그대로 옮긴다. 동작을 바꾸는 개선(실제 진행률 · 비밀번호 정책 강화 등)은 별도 결정으로 | 이관과 개선을 섞으면 동작이 바뀐 원인을 가려낼 수 없다 (각 주제의 결정 대기 항목으로 확정) |
| 2026-09-24 | 인증 UI 순서를 T09 → T11(사이드바) → T12(헤더) → T10(로그인 · 비밀번호 변경 + 연결)으로 한다. 헤더 · 사이드바는 요청 signal만 내고, 로그인이 필요한 확인은 T10에서 한다 | 로그인 창을 여는 곳이 헤더 · 사이드바라서, 그 뒤에 와야 실제 앱에서 로그인 흐름을 확인할 수 있다 |
| 2026-09-24 | 앱 식별 정보(조직 · 앱 · 표시 이름, DB 파일 이름, 번들 식별자, 회사명 · 제품명 · 저작권)는 T04 착수 시 한 번에 정하고, 정한 값을 이 표에 적는다 | 네 주제(T04 · T06 · T11 · T17)에서 따로 정하면 이름이 어긋난다. 조직 · 앱 이름은 사용자 데이터 위치를 정하므로 나중에 바꾸기 어렵다 |

---

## 전역 리스크

주제 하나에만 해당하는 위험은 그 주제 파일의 *함정*에 있다.

| # | 내용 | 영향 | 대응 |
|---|---|---|---|
| 1 | macOS 실기가 없다 | T08 이후 macOS 화면 판정 불가 | T08 전에 Mac mini. 그 전에는 CI macOS 러너로 빌드 · 테스트 |
| 2 | 한글 폰트 메트릭의 OS 간 차이를 모른다 | SageSDI 고정 픽셀 규격을 얼마나 다시 해석할지 불명 | T02에서 측정 |
| 3 | QStyle 작성 난도가 QSS보다 높다 | UI 기반 단계 기간 증가 | T07에서 `SageStyle` 재정의 범위를 먼저 확정 |
| 4 | 프레임리스 다이얼로그의 macOS · Wayland 동작 미검증 | 로그인 창 모양 · 이동 | T09에서 `startSystemMove` 검증 |
| 5 | CI가 정식 릴리스가 아닌 aqtinstall 커밋을 쓴다 | 설치 도구의 검증 범위가 좁다 | `docs/DEBT_LOG.md`에 기록 (T01). aqtinstall 3.4.0 이상이 나오면 되돌린다 |
| 6 | 로컬 WSL은 Ubuntu 20.04 (GCC 9, C++20 부족) | 로컬 Linux 검증 불가 (CI에서는 됨) | Ubuntu 24.04 추가 설치 (승인 필요) |

---

## 완료 이력

### 규칙 · 목표 · 계획 (2026-09-23, [#1](https://github.com/JakeKim4926/SageQt/pull/1) · [#2](https://github.com/JakeKim4926/SageQt/pull/2))

- `CLAUDE.md`에 프로젝트 목표 3가지. 스킬 6종 (Qt 6 기준 재작성 3종 · SageSDI에서 이식 3종), SKILL.md 300줄 이하 + `references/`, `develop` 브랜치
- 교훈
  - **추측 대신 실측이 이전 판단 두 건을 뒤집었다.** "MSVC `/utf-8` 누락"은 틀렸고, "`Q_OBJECT`는 필요할 때만"은 Qt 공식 권고와 반대였다
  - **문서로만 있는 규칙은 지켜지지 않는다.** SageSDI의 계층 규칙 위반 두 건이 그 증거다 — CMake 타깃 격리가 빌드 단계에서 막는다
  - **규칙 재배치는 손으로 다시 쓰지 않는다.** 줄 범위 복사와 스냅샷 대조로 규칙 줄 1,308개의 누락 0을 확인했다

### 기반 — 정적 분석 전까지 (2026-09-23, [#3](https://github.com/JakeKim4926/SageQt/pull/3) ~ [#7](https://github.com/JakeKim4926/SageQt/pull/7))

- `.gitattributes`(추적 파일 33개 모두 LF), VS 템플릿 제거 · 계층 타깃 구조(`sage_define` · `sage_ui` · 실행 파일, C++20 · CMake 3.22), 세 OS 프리셋 6개, GitHub Actions 3 OS × Debug · Release — 6개 job 모두 Qt 6.11.2 · 경고 0
- 남은 기반 작업(정적 분석 · 경고 에러화)은 T01
- 교훈
  - **격리는 음성 테스트로만 증명된다.** 링크하지 않은 계층의 헤더를 include하면 `C1083`으로 실패하는 것을 확인했다. 빌드 성공만으로는 증거가 되지 않는다
  - **검증 도구 자체를 검증한다.** 배치 파일의 경로가 깨져 첫 음성 테스트가 무효였고, CI 조회가 이전 실행을 보고 있었다. 둘 다 다시 확인했다 (`sageqt-plan`의 *공통 함정*에 반영)
  - **".rc에 다이얼로그 0개"라는 기록은 UTF-16 파일을 grep한 결과였다** — 2026-09-24 정정. 파일 인코딩을 먼저 확인한다
