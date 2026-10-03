# PR 작업 로그

PR을 생성하거나 머지할 때마다 아래 형식으로 기록한다. 형식은 `git-workflow` 스킬을 따른다.

```markdown
## [yyyy-mm-dd] 브랜치명
- **목적**: 무엇을 위한 작업인지
- **변경 내용**: 주요 작업 요약
- **PR 링크**: (있으면)
- **규칙 점검**: Blocker n · Major n · Minor n (남긴 항목은 DEBT 번호) / 코드 변경 없음
- **결과**: merged / closed / pending
```

---

## [2026-10-03] docs/skill-compliance-gate
- **목적**: 전체 코드 점검에서 나온 규칙 위반의 원인 차단 — 규칙은 모두 위반 코드보다 먼저 있었는데, 주제가 일부 reference만 읽게 했고 다 쓴 뒤 대조하는 단계가 없었다
- **변경 내용**: (1) coding-rules 작성 후 점검표 · reference 다섯 파일 항상 읽기. (2) develop 반영 전 code-review-expert 규칙 점검(Blocker · Major 0), 주제 종료 시 책임 재측정 · 스킬 동기화. (3) 규칙 해석 16건 사용자 결정 반영(숫자 리터럴 · 소유권 이전 · add* · 오버라이드 접근 수준 · 고정 높이 · 예외 3종 · 값 집합 enum · 업무 상수 블록 · 테스트 색 리터럴 · 문구 속 숫자 · 멤버 기본값 · 테스트 링크 · 테스트 로그 · qdrawutil). (4) 승인된 코드와 어긋난 문서 동기화(infra/auth, Qt::Network, 디자인 값 9개, 위젯 2종 · 라벨 변형 2개, 구조도 3곳)
- **PR 링크**: 없음
- **규칙 점검**: 코드 변경 없음
- **결과**: merged (develop, 2026-10-03)

## [2026-10-03] fix/label-text-color
- **목적**: 글자가 SageSDI보다 진하고 거칠게 보이던 문제 (사용자 확인 화면에서 발견)
- **변경 내용**: (1) `SageStyle::setTextColor`가 `Text` 역할에도 색을 넣는다 — 카드 안 폼 라벨이 본문 색으로 그려지던 버그(세 OS), 테스트 추가. (2) Windows 글자 엔진 GDI — 프리셋 `SAGE_QPA_PLATFORM_ARGUMENTS`, `QT_QPA_PLATFORM`이 비어 있을 때만 적용(사용자 결정). SageSDI 화면과 픽셀 대조로 확인(폼 라벨 15.81 → 0.15). sageqt-ui 폰트 규칙 · DEBT_LOG
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-03)

## [2026-10-02] chore/ci-artifacts
- **목적**: CI 결과물 · Qt 없는 환경 실행 확인 (sageqt-plan T18, 세 번째 PR) · T18 완료 처리
- **변경 내용**: build job(release)에 install + cpack + 산출물 업로드, `verify-package` job(새 러너에서 압축 풀고 실행 · DB 생성 확인), Linux 검증 러너에 시스템 라이브러리. T18 결과 · DEBT_LOG
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-02)

## [2026-10-02] chore/deploy-script
- **목적**: 배포 스크립트 (sageqt-plan T18, 두 번째 PR)
- **변경 내용**: `SageQt/packaging/CMakeLists.txt`(install + `qt_generate_deploy_app_script` NO_TRANSLATIONS + 라이선스 · .desktop · 아이콘 설치 + CPack ZIP/DragNDrop/TGZ + SHA256), 프리셋 `SAGE_PACKAGE_PLATFORM`. 로컬 Windows에서 zip 생성 · Qt 없이 실행 · qsqlite 로드 확인
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-02)

## [2026-10-02] chore/app-icon
- **목적**: 앱 아이콘 (sageqt-plan T18, 첫 번째 PR — T17에서 넘긴 것)
- **변경 내용**: `tools/app-icon/make_app_icon.py`(원본 구성을 1024로 다시 그림), `resources/icons`(1024 원본 · ico · icns · 256 PNG), Windows rc · macOS Info.plist · Linux .desktop · 창 아이콘 연결, CI 아이콘 검사, DEBT_LOG 아이콘 해결. 사용자 결정 2건(결과물 형식 · 아이콘 B)
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-02)

## [2026-10-02] chore/app-metadata
- **목적**: 버전 · 앱 식별 정보 · 폰트 라이선스 (sageqt-plan T17)
- **변경 내용**: `project(VERSION 1.0.0)` + 식별 정보 변수 단일 출처, `SageAppInfo.h` 생성, `packaging/windows/SageQt.rc.in` · `packaging/linux/SageQt.desktop.in`, macOS `MACOSX_BUNDLE_*`, `licenses/` 복사, CI 메타데이터 확인 단계, `coding-design` 폴더 구조에 `packaging/`. 아이콘은 원본 대기(DEBT_LOG). 사용자 결정 4건
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-02)

## [2026-10-02] feature/status-bar
- **목적**: 상태 표시줄 · 상태 메시지 (sageqt-plan T16, 두 번째 PR) · T16 완료 처리
- **변경 내용**: 작업 영역 `statusChanged`(대기 중 · 처리 중 · 파일 드롭 수신 · 완료 · 실패), 메인 창 상태 표시줄(라벨 · 왼쪽 여백 24 · 높이 24 · 보조 글자색 · 손잡이 없음), `SageStyle` 상태 표시줄 면 · 위 구분선. 사용자 결정 4건. T16 결과 · DEBT_LOG 2건 · 스크린샷
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-02)

## [2026-10-02] feature/history-panel
- **목적**: 실행 기록 탭 (sageqt-plan T16, 첫 번째 PR)
- **변경 내용**: core `SageWorkflowHistory`(응답 → 기록 행), `SageHistoryModel` · `SageHistoryFilterProxyModel`, `SageWorkflowHistoryPanel`(필 바 · 표 · 빈 상태), `SageFilterPillBar` · `SageEmptyState`, 공통 표 위젯 `SageTableView`(결과 표에서 뽑아냄), delegate 실패 행 면 · 배지, 표 role `SageTableRole`로 모음, 캡처 스크립트 실행 기록 탭 단계. 사용자 결정 3건(표시기 안 옮김 · 시각 고정 형식 · 기록 표도 결과 표 규칙). 스크린샷 `docs/screenshots/T16/`
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-02)

## [2026-10-01] feature/input-table
- **목적**: 입력 표 · 작업 영역 연결 (sageqt-plan T15, 네 번째 PR) · T15 완료 처리
- **변경 내용**: 입력 패널의 입력 표(같은 표 패널 클래스) · 빈 상태 안내 · Ghost 입력 초기화, 작업 영역 표 고르기 · 보이기 조건 · 실행 버튼 조건 · 생성 요청 행 번호와 검증 · 결과 반영 · 요약/합계 갱신 · 업무별 검색어/기준/체크 보존, Ghost 면 = 배경을 칠하는 조상 면, `SageLabel` Hint, 테스트 핸들러 확장 · 작업 영역 테스트 6건. T15 결과 · DEBT_LOG 2건 · 스크린샷
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-01)

## [2026-10-01] feature/result-table-controls
- **목적**: 결과 표 패널 안 컨트롤 (sageqt-plan T15, 세 번째 PR)
- **변경 내용**: `SageSearchBox`(기준 콤보 · 검색어 · 돋보기), `SageSelectionBar` + `SageSelectionCountLabel`, `SageSummaryBar`, `SageTableTotalBar`, 표 패널 띠 · 검색 영역 · 합계 막대 · `filterChanged` · `selectionChanged`, proxy가 필터 기준 보관, `SageStyle` Ghost · 아이콘 버튼 · 체크 상자 · 기준 콤보. 사용자 결정 2건(선택 해제 면 · 전체 선택 체크 상자). 테스트 18 + model 13
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-01)

## [2026-10-01] feature/result-table-panel
- **목적**: 결과 표 패널 · 그리기 · 결과 탭 연결 (sageqt-plan T15, 두 번째 PR)
- **변경 내용**: `SageResultTablePanel`(제목 + `QTableView`), `SageResultTableDelegate`(교대 행 · 선택 면 + 막대 · 가로선 · 체크 상자 · 강조 열 · 빈 값 흐림 · 마우스를 올린 행 `#F8F1E6`), `SageStyle` 헤더, 열 폭(SageSDI 규칙 — 좁으면 가로 스크롤, `model-view.md` 예외), 업무를 고를 때 열 설정 · 실행 결과로 행 채우기. 캡처 스크립트 결과 탭 단계. 사용자 결정 3건(hover 색 · 가로 스크롤 · 헤더/스크롤바 그대로). 스크린샷 `docs/screenshots/T15/`
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-10-01)

## [2026-09-29] feature/result-table-model
- **목적**: 결과 표 model · 필터 proxy (sageqt-plan T15, 첫 번째 PR)
- **변경 내용**: `SageResultTableModel`(핸들러 열 · 행, 첫 열 가운데, 첫 열 체크), `SageResultFilterProxyModel`(검색어 부분 일치 · 대소문자 무시, 필터 변경 시 체크 해제, 보이는 행 기준 체크 수 · 행 번호 · 복원 · 전체 선택). 테스트 12건. T15 결정 3건 · 확인한 사실 기록
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-29)

## [2026-09-29] feature/status-card
- **목적**: 상태 카드 · 시간 기반 진행 표시 · 폴더 열기 (sageqt-plan T14, 두 번째 PR)
- **변경 내용**: `SageStatusCard`(대기 · 처리 중 · 완료 · 실패, 진행 막대 `QProgressBar` + `SageStyle`), 입력 패널 진행 타이머(300ms · 3% · 95%), 작업 영역 결과 문구 · 사유 · 저장 경로, 「폴더 열기」(파일이면 든 폴더 — 사용자 결정), 경로 말줄임(파일 이름 유지). 테스트(카드 상태 · 픽셀 · 진행 · 결과 · 폴더 열기). CLAUDE.md에 C 드라이브 사용 금지 규칙. 스크린샷 `docs/screenshots/T14/`. T14 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-29)

## [2026-09-29] feature/workflow-controller
- **목적**: 업무 실행 흐름 — 실행 버튼 → 백그라운드 실행 → 결과 (sageqt-plan T14, 첫 번째 PR)
- **변경 내용**: core `SageWorkflowRunner`(payload 선택 키 규칙, 핸들러 없음 · 예외 → 오류 응답), ui `SageWorkflowController`(QtConcurrent + QFutureWatcher, 실행 중 재시작 거절, 결과 상태 보존/복원), 입력 카드 실행 버튼, 작업 영역 실행 흐름(입력 · 저장 폴더 검증, 결과 탭 전환, 생성 완료 안내, 실행 중 드롭 무시, 입력 표 업무 자동 불러오기). `SAGE_REQUEST_UNKNOWN` 값 변경 · `START_FAILED` 미이관. 테스트(payload · 오류 응답 · 컨트롤러 · 작업 영역 실행). 작업 파일 위치를 D 드라이브로 옮김
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-29)

## [2026-09-29] feature/input-panel
- **목적**: 입력 패널(입력 카드 · 파일/폴더 선택)과 창 어디든 파일 드롭 (sageqt-plan T13, 두 번째 PR)
- **변경 내용**: `SageWorkflowInputPanel`(카드 · 경로 칸 · 파일/폴더 창), 핸들러 `inputFileFilter()`(사용자 결정), `SageFileDropFilter`, 업무별 입력 경로 · 저장 폴더 보존, `SageStyle` 읽기 전용 입력칸 · 섹션 제목 · 탭 hover(사용자 결정), 콘텐츠 여백 수정. 테스트(입력 패널 · 경로 보존 · 드롭 세 곳 · 스타일). 규격을 SageSDI 코드로 확정. 스크린샷 `docs/screenshots/T13/`. T13 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-29)

## [2026-09-29] feature/workspace-tabs
- **목적**: 작업 영역 탭과 업무별 선택 탭 보존 (sageqt-plan T13, 첫 번째 PR)
- **변경 내용**: `SageWorkspacePanel`(핸들러 탭 → `QStackedWidget` 패널 교체, 업무별 선택 탭 저장 · 복원, 없는 업무는 입력 탭), 입력 · 결과 · 실행 기록 패널 자리, `SageStyle` 탭 그리기(같은 폭 · 좌우 16 · 인디케이터 — 사용자 결정), `SageSurface` `Header` → `Panel`, 창 연결. 테스트(탭 · 상태 · 그린 값). 스크린샷 스크립트의 로그인 버튼 위치에 탭 줄 반영
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-29)

## [2026-09-28] feature/login-dialog
- **목적**: SageSDI 로그인 · 비밀번호 변경 다이얼로그와 강제 변경 흐름을 옮기고 헤더 · 사이드바 요청에 연결 (sageqt-plan T10)
- **변경 내용**: `SageLoginDlg` · `SagePasswordChangeDlg`(서비스 호출은 QtConcurrent), `SageLineEdit` · `SageInlineMessage` · `FormLabel`, 입력칸 스타일(포커스 테두리 주 색 — 사용자 결정), 창 연결, 폰트 패밀리 상수 초기화 순서 버그 수정과 규칙, 조립 수준 로그인 흐름 테스트(실제 DB · PBKDF2), 스크린샷 워크플로에 로그인 창 확인. 규격을 SageSDI 코드로 확정. 스크린샷 `docs/screenshots/T10/`. T10 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-28)

## [2026-09-28] feature/header
- **목적**: SageSDI 헤더(업무 제목 · 분류 · 인증 표시 · 로그인/로그아웃)를 옮긴다 (sageqt-plan T12)
- **변경 내용**: `SageHeaderPanel` · `SageBadge`, `SageLabel` · `SageSurface` 변형 추가, 앱 팔레트 `Mid`, 사이드바 옆 세로 구분선, `SageAuthSession` → `QObject`(`authStateChanged`). 테스트(인증 표시 · 로그아웃 · 그린 값 · 창 배치). 헤더 규격을 SageSDI 코드로 확정(배지 알약 · 같은 색, 제목 19px). 스크린샷 `docs/screenshots/T12/`. T12 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-28)

## [2026-09-28] feature/sidebar
- **목적**: SageSDI 사이드바를 옮기고 메인 창의 첫 패널 배치를 시작 (sageqt-plan T11)
- **변경 내용**: `SageSidebarPanel` · `SageSidebarModel` · `SageSidebarDelegate` · `SageSurface` · `SageLabel`, `SageStyle` polish · 구분선, 등록부 `handlers()` · `registerHandler`, `SageMainWindow` 배치(초기 1280 × 800), `sage_ui` → `sage_core` 링크. 테스트(트리 구성 · 선택 처리 · 그린 값). 사이드바 규격을 SageSDI 코드로 확정, 스크린샷 `docs/screenshots/T11/`. T11 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-28)

## [2026-09-28] feature/frameless-dialog
- **목적**: 프레임리스 다이얼로그 기반과 메시지 상자, 시작 시 안내 연결 (sageqt-plan T09)
- **변경 내용**: `SageFramelessDlg` · `SageMessageBoxDlg` · `SageDialogCaptionBar` · `SageButton` · `SageIconEngine`, `SageStyle` 버튼 · 테두리 · 표준 아이콘, `main.cpp` 오류 · 초기 비밀번호 안내. 테스트(키 동작 · 크기 · 그린 색). `screenshots.yml` + `tools/dialog-capture`(세 OS 끌어서 이동 · Enter 확인). SageSDI 코드 대조로 `sageqt-ui` 규격 정정(확인형 제외 · Primary Regular · 캡션 14px SemiBold), 사용자 결정(버튼 hover · 포커스 없음, 본문 말줄임 없음), Linux 끌기 DEBT. T09 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-28)

## [2026-09-28] feature/style-foundation
- **목적**: 앱 전체의 모양을 한 곳에서 그리는 기반 — `SageStyle` · 팔레트 · 폰트 등록 · 역할 폰트 (sageqt-plan T08)
- **변경 내용**: `ui/style/` (`SageDesignDefine.h` · `SageStyleDefine.h` · `SageStyle` · `SageFontRegistry` · `SageFontCatalog`), 폰트 qrc(실행 파일 타깃), `main.cpp` 적용, `tests/ui/style/` 2개. `screenshots.yml` 추가, `font-metrics.yml`에 Linux xcb 추가(Linux 폰트 DEBT 해결). 규칙 3건(`setStyle` 예외 · `tests/ui/` · 값은 쓰는 주제에서), 규격 없는 상태 판정을 T09 · T10 · T11 · T13 · T15로 넘김. 스크린샷 `docs/screenshots/T08/`. T08 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-28)

## [2026-09-28] docs/sageqt-ui-skill
- **목적**: SageQt UI 규격 스킬을 만든다 — 디자인 값 · 레이아웃 정책 · 폰트 · 변형 · SageStyle 범위 · 화면별 규격 (sageqt-plan T07)
- **변경 내용**: `.claude/skills/sageqt-ui/`(SKILL.md + design-values · style-scope · widgets · screens), `docs/decisions/sageqt-ui/` 분류표 3개(상수 477 · 컨트롤 25 · sagesdi-ui 분석). `style.md` 얇은 변형 서브클래스 예외, 다른 스킬의 미정 표현 제거, 결정 3건, T08 · T11 · T13 반영. T07 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-28)

## [2026-09-27] feature/app-bootstrap
- **목적**: SageSDI `InitInstance` · `ExitInstance`의 시작 · 종료 흐름을 `main.cpp`로 옮긴다 (sageqt-plan T06)
- **변경 내용**: `main.cpp` 조립(식별 정보 · 스키마 준비 · 서비스 · 초기 관리자 · 세션 · 종료 대기), 실패 시 `sage.app` 로그 + `EXIT_FAILURE`. 로그 규칙(`coding-rules`) · 결정 기록. 로컬 실행 확인 — DB 위치, 첫 실행 1.52초(Release), 실패 흉내 종료 코드 1. T06 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-27)

## [2026-09-27] feature/auth
- **목적**: SageSDI의 로그인 · 비밀번호 변경 · 초기 관리자 생성 · 세션을 옮긴다 (sageqt-plan T05)
- **변경 내용**: core `auth/` — `SageUserDto` · `SageUserRole` · `ISageUserRepository` · `ISagePasswordHasher` · `SageUserService` · `SageAuthSession`. infra — `SagePbkdf2PasswordHasher`(PBKDF2-HMAC-SHA256 600,000회, `Qt::Network` PRIVATE) · `SageUserRepository`(SageSDI 원문 SQL 4개). 테스트 5개. 결정 3건(해시 · 정책 · 범위), T10 · T12 반영, T05 완료 처리
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-27)

## [2026-09-27] docs/workflow-direct-merge
- **목적**: PR 없이 CI 통과 후 `develop`에 반영하는 실제 방식을 `git-workflow` 규칙으로 만든다
- **변경 내용**: `git-workflow` SKILL.md · `pr-and-release.md`에 develop 반영 절차 · 체크리스트 · 보호 원칙. UI 스크린샷 위치 `docs/screenshots/<주제 ID>/`(coding-design · T08 · T10). 리뷰 스킬 문구. 결정 기록 1건. DEBT 1건(Windows CI Qt 새 설치 간헐 실패)
- **PR 링크**: 없음
- **결과**: merged (develop, 2026-09-27)

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
