# T04 — DB 기반 (QtSql)

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 SQLite 연결 · 스키마 준비를 QtSql로 옮긴다. 이 주제가 끝나면 `sage_infra`가 연결 설정을 주입받아 작업마다 연결을 열고 닫으며, 스키마가 세 OS의 사용자 데이터 폴더에 만들어진다.

## 시작 전에
1. 선행 주제: T03
2. 스킬 로드: `git-workflow`, `coding-design`, `coding-rules`
   - 읽을 reference: `coding-design/references/cmake-targets.md` · `threads-and-db.md`, `coding-rules/references/ownership-and-threads.md` (RAII 예외) · `values-and-platform.md`
3. 결정 대기 — 사용자에게 확정받는다
   - **DB 파일 이름**: SageSDI는 `data/estimate.db` (`estimate`는 SageTaechang 견적 업무에서 온 이름). 권장 — 앱 이름에 맞춘 새 이름. 배포본이 없어 옛 이름과 호환할 필요가 없다
   - **레거시 마이그레이션 생략**: SageSDI의 `must_change_pw` 컬럼 추가(`ALTER TABLE`)는 그 컬럼이 없던 SageSDI DB를 위한 것이다. SageSDI 배포본이 없으므로(SageSDI `docs/DEBT_LOG.md`) 권장 — 처음부터 컬럼을 포함한 스키마로 만들고 마이그레이션 코드는 옮기지 않는다
4. 재확인할 사실
   - QtSql SQLite 드라이버 접속 옵션 `QSQLITE_BUSY_TIMEOUT` (Qt 문서 SQL Database Drivers)
   - 로컬 `D:/Qt/6.11.2/msvc2022_64/plugins/sqldrivers/qsqlite.dll` 존재 (2026-09-23 확인)

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/app/infra/db/`

**연결 설정** — `SqlContext.cpp:6-8, 68-70`
- busy timeout `5000` ms (`SQL_CONTEXT_BUSY_TIMEOUT_MS`) → QtSql 접속 옵션 `QSQLITE_BUSY_TIMEOUT=5000`
- 연결 직후 `PRAGMA foreign_keys = ON;`

**스키마** — `SqlInitializer.cpp:147-168`, 원문 그대로
```sql
CREATE TABLE IF NOT EXISTS SageUser (
    user_id   INTEGER PRIMARY KEY AUTOINCREMENT,
    login_id  TEXT NOT NULL UNIQUE,
    pw_hash   TEXT NOT NULL,
    role      INTEGER NOT NULL DEFAULT 0,
    must_change_pw INTEGER NOT NULL DEFAULT 0,
    CHECK (role >= 0),
    CHECK (must_change_pw IN (0, 1))
);
```
- 스키마 준비 순서: 연결 → 테이블 생성 → (레거시 컬럼 마이그레이션) → 기본 관리자 생성. 기본 관리자 생성은 비밀번호 해시가 필요하므로 **T05로 넘긴다**

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| DB 경로를 실행 파일 폴더에서 계산 (`SqlContext::GetDefaultDbPath` · `GetExeDirectory`, `GetModuleFileName` 사용) | macOS `.app` 번들 안에 쓰면 서명이 깨지고 `/Applications`에서는 쓰기 권한이 없다. Windows `Program Files`도 쓰기 불가. `QStandardPaths`로 대체 (`values-and-platform.md`) |
| `sqlite3_*` C API, `external/sqlite` 소스 | QtSql로 대체 (결정 기록) |
| `SageDBMgr` 싱글턴 (`#define sageDBMgr`) | 전역 금지. `main.cpp`가 조립한다 (T06) |
| 레거시 마이그레이션 (`MigrateSageUserColumns` · `HasColumn`) | 결정 대기 항목 — 승인 시 옮기지 않는다 |

## 함정
- **`PRAGMA foreign_keys`는 연결 단위 설정이다.** 작업마다 연결을 여는 구조(`threads-and-db.md`)에서는 **연결을 열 때마다** 실행해야 한다. 한 번만 실행하면 다음 연결에서는 꺼져 있다
- **DB 연결은 연 스레드에서만 쓴다.** `QtConcurrent` 작업마다 고유한 연결 이름으로 열고 닫는다
- `QSqlDatabase::removeDatabase`는 그 연결을 쓰는 `QSqlDatabase` · `QSqlQuery` 객체가 모두 사라진 뒤에 불러야 한다. 순서가 틀리면 경고와 자원 누수가 생긴다 — RAII 연결 스코프가 이 순서를 보장한다 (소멸자를 갖는 유일한 예외, `ownership-and-threads.md` 규약 3)
- `QStandardPaths`의 앱 데이터 경로는 `QCoreApplication`의 조직 · 앱 이름에 따라 정해진다. 테스트에서는 연결 설정으로 **임시 폴더 경로를 직접 주입**해서 실제 사용자 폴더를 건드리지 않는다
- `Qt::Sql`은 `sage_infra`가 PRIVATE로 링크한다 — 공개 헤더에 QtSql 타입이 나오면 안 된다
- SQLite 버전은 Qt에 번들된 것이다. 특정 SQLite 기능(JSON1 · FTS 등)은 SageSDI에서 쓰지 않았다 (2026-09-23 확인)

## 작업
PR 1개: `feature/db-infra`
- [ ] `sage_infra` 타깃 생성 (`SageQt/infra/`, `Qt::Sql` PRIVATE)
- [ ] 연결 설정 값 타입 (경로 · 옵션) — `main.cpp`가 주입한다
- [ ] RAII 연결 스코프: 고유 이름으로 열기, busy timeout, `PRAGMA foreign_keys = ON`, 닫을 때 순서 보장
- [ ] 스키마 준비 (테이블 생성, 반복 실행해도 안전)
- [ ] 테스트: 임시 폴더에 스키마 생성 · 두 번 실행해도 성공 · 새 연결마다 `foreign_keys`가 켜져 있음 · 여러 `QtConcurrent` 작업이 각자 연결을 열어 동시에 조회

## 완료 기준
- 3-OS CI에서 빌드와 테스트가 모두 통과한다
- 연결을 열 때마다 `PRAGMA foreign_keys`가 1인 것을 테스트가 확인한다
- 공개 헤더에 `QSql` 타입이 0개다 (`grep`으로 확인)
- DB 파일 경로가 실행 파일 폴더가 아니다

## 범위 밖
- 기본 관리자 생성 · 사용자 Repository — T05
- 앱 시작 시 스키마 준비 호출 — T06

## 확인한 사실
(진행 중 기록)
