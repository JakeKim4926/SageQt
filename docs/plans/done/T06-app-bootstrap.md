# T06 — 앱 조립 (`main.cpp`)

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI `CSageSDIApp::InitInstance` · `ExitInstance`가 하던 앱 시작 · 종료 흐름을 `main.cpp`(조립 지점)로 옮긴다. 이 주제가 끝나면 앱이 식별 정보를 갖고, 시작할 때 DB를 준비하고, 서비스를 조립해 창에 넘기고, 종료할 때 백그라운드 작업을 기다린다.

## 시작 전에
1. 선행 주제: 없음 (T05 완료 — 조립할 객체: `SageDbConfig::buildDefaultConfig` → `SageSchemaInitializer::prepare` → `SageUserRepository(config)` · `SagePbkdf2PasswordHasher()` → `SageUserService(repository, hasher)` → `ensureDefaultAdmin`(초기 비밀번호를 `std::optional`로 돌려준다) · `SageAuthSession`)
2. 스킬 로드: `git-workflow`, `coding-design`, `coding-rules`
   - 읽을 reference: `coding-design/references/threads-and-db.md` (전역 상태 · 종료 대기 · 스키마 준비 시점) · `cmake-targets.md`
3. 결정 — 없음. 조직 이름 · 앱 이름은 T04에서 정한 앱 식별 정보를 따른다 (`MIGRATION_PLAN.md` 결정 기록)
4. 재확인할 사실
   - DB 경로는 `SageDbConfig::buildDefaultConfig`가 `QStandardPaths::AppDataLocation` + `sageqt.db`로 만든다 (T04). 조직 · 앱 이름(`Sage` · `SageQt`, 결정 기록)을 **먼저** 설정해야 이 경로가 맞다. 스키마 준비는 `SageSchemaInitializer::prepare`

## SageSDI에서 옮길 것
원본: `D:/Projects/SageSDI/SageSDI/SageSDI.cpp`

`InitInstance` 순서 (`:62-116`)
| 줄 | SageSDI | SageQt |
|---|---|---|
| 65 | `AfxOleInit()` | **옮기지 않음** — Qt가 필요할 때 COM을 초기화한다 |
| 74-75 | `SetRegistryKey` · `LoadStdProfileSettings(4)` | **옮기지 않음** — 레지스트리를 읽고 쓰는 코드가 0곳 (마법사 잔재) |
| 77-85 | `CSingleDocTemplate` 등록 | **옮기지 않음** — Doc/View 없음, `SageMainWindow` 하나 |
| 93-95 | `sageDBMgr.Initialize` 실패 → `AfxMessageBox(오류)` → `return FALSE` (앱 종료) | 스키마 준비 실패 → 오류 표시 → 0이 아닌 종료 코드. **오류 표시는 T09에서 메시지 상자로 연결**, 이 주제에서는 종료 코드와 로그 |
| 98-106 | 초기 관리자 비밀번호가 있으면 `SAGE_UI_INITIAL_ADMIN_PW_FORMAT`으로 한 번 안내 후 지움 | 초기 비밀번호를 받아 둔다. **안내 표시는 T09에서 연결** |
| 110-115 | `ProcessShellCommand` → 프레임 생성 → `ShowWindow(SW_SHOW)` | 서비스를 주입해 `SageMainWindow`를 만들고 표시 |

`ExitInstance` (`:119-125`): `sageDBMgr.Finalize()` → SageQt에서는 이벤트 루프가 끝난 뒤 **백그라운드 작업 완료를 기다리고** 서비스를 파괴한다 (`threads-and-db.md`)

## 옮기지 않는 것
위 표의 "옮기지 않음" 항목, 그리고:
| 대상 | 이유 |
|---|---|
| 숨겨진 메뉴의 단축키 테이블 (`SageSDI.rc`의 `IDR_MAINFRAME ACCELERATORS`: Ctrl+N · Ctrl+O · Ctrl+S 등) | SageSDI에서 의도하지 않은 동작(문서 열기 · 저장 창)을 일으키는 마법사 잔재 |
| 정보 창 `IDD_ABOUTBOX` · `OnAppAbout` | 메뉴가 숨겨져 열 방법이 없다 |

## 함정
- **식별 정보는 `QStandardPaths`를 쓰기 전에 설정한다.** 순서가 바뀌면 DB가 엉뚱한 폴더에 만들어진다
- **이 주제부터 T09 전까지는 초기 관리자 비밀번호가 화면에 표시되지 않는다.** 이 기간에 만든 개발용 DB는 관리자 비밀번호를 알 수 없으므로, T09 · T10을 확인하기 전에 개발용 DB를 지우고 다시 만든다. 비밀번호를 로그로 출력하지 않는다
- 스키마 준비는 **이벤트 루프 시작 전**, 메인 스레드에서 한다 (`threads-and-db.md`). 이것이 "UI 스레드에서 DB 작업 금지"의 유일한 예외다 — 아직 화면이 없다
- 종료 대기는 `QThreadPool::globalInstance()->waitForDone()` 같은 방식으로, 서비스 파괴 **전에**
- 스택 객체 생성 순서: 서비스가 창보다 먼저 만들어져야 창이 먼저 파괴된다 (`ownership-and-threads.md` 규약 1)

## 작업
PR 1개: `feature/app-bootstrap`
- [x] 식별 정보 설정
- [x] 연결 설정(경로) → 저장소 → 서비스 → 세션 조립, `SageMainWindow`에 주입할 수 있는 형태로
- [x] 시작 시 스키마 준비 + 기본 관리자 생성, 실패 시 종료 코드
- [x] 종료 시 백그라운드 작업 대기
- [x] Windows 로컬: 실행 → DB 파일이 `QStandardPaths` 경로에 생기는지 확인

## 완료 기준
- 3-OS CI에서 빌드와 테스트가 모두 통과한다
- Windows 로컬에서 앱 실행 후 DB 파일이 사용자 데이터 폴더에 생긴다 (경로 기록)
- 전역 · 싱글턴 객체가 0개다
- DB 준비 실패를 흉내 냈을 때(쓰기 불가 경로) 0이 아닌 코드로 종료한다

## 범위 밖
- 오류 · 초기 비밀번호의 화면 표시 — T09
- 폰트 · 스타일 적용 — T08

## 확인한 사실
- SageSDI `SageSDI.cpp`의 줄 번호가 이 파일과 같다 (`:65 · 74-75 · 77-85 · 93-95 · 98-106 · 110-115 · 119-125`)
- 결정: 로그 규칙(계층별 `QLoggingCategory` + `qC*`, 로그 파일 없음, 비밀번호 금지 — `coding-rules/references/api-shape.md`). 창 생성자는 이번에 바꾸지 않는다 — 서비스를 처음 쓰는 주제(T10 · T11)에서 인자를 추가한다. 실패 확인은 코드 옵션 없이 수동으로 흉내 낸다
- Windows 로컬 실행: DB가 `C:\Users\USER\AppData\Roaming\Sage\SageQt\sageqt.db`(16,384바이트)에 생긴다. 창을 닫으면 종료 코드 0
- **첫 실행은 초기 관리자 해시(PBKDF2 600,000회)를 창 표시 전에 메인 스레드에서 계산한다** — 창이 뜨기까지 Debug 7.05초 / Release 1.52초, 두 번째 실행부터 Debug 0.41초 / Release 0.31초 (로컬 Windows 10). 첫 실행 1회뿐이고 SageSDI도 DB 준비를 창 전에 했으므로 그대로 둔다. 로그인 한 번도 같은 해시를 계산하므로 T10은 반드시 UI 스레드 밖에서 부른다
- 실패 흉내: `%APPDATA%\Sage\SageQt` 자리에 같은 이름의 파일을 두고 실행 → 스스로 종료, 종료 코드 1. 되돌린 뒤 DB 정상
- `WIN32_EXECUTABLE` 앱이라 `qCCritical` 출력은 콘솔에 보이지 않는다 (Qt 기본 처리기 → 디버거 출력). 사용자에게 보이는 오류 표시는 T09
- 전역 · 싱글턴 객체 0개 (`grep`). `Q_STATIC_LOGGING_CATEGORY`는 로그 규칙이 허용하는 파일 범위 함수다

## 결과
- 작업 브랜치 CI 통과 후 `develop`에 squash merge
- `main.cpp`: 식별 정보 → DB 설정 · 스키마 준비 → 저장소 · 해시기 · 서비스 → 초기 관리자 확인 → 세션 → 창 → 이벤트 루프 → `QThreadPool` 대기. `SageDefine.h`에 식별 정보 · 로그 카테고리 이름. 로그 규칙 추가
- 교훈
  - 보안 설정(해시 600,000회)은 시작 시간 같은 다른 동작에도 비용을 준다 — 실행해서 재야 보인다
  - 여러 단계를 한 번에 돌릴 때 앞 단계가 실패하면 멈추게 묶는다 — 마무리 스크립트가 실패했는데 파일 이동 커밋만 push됐다
