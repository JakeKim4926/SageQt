# T05 — 인증 · 사용자

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 사용자 · 로그인 · 비밀번호 변경 · 초기 관리자 생성을 옮긴다. core는 경계 인터페이스로 저장소를 쓰고, 세션은 전역이 아니라 주입받는 객체가 된다.

## 시작 전에
1. 선행 주제: 없음 (T04 완료 — Repository는 `SageDbConnection`으로 작업마다 연결을 연다. SQL은 Repository `.cpp` 안 상수)
2. 스킬 로드: `git-workflow`, `coding-design`, `coding-rules`
   - 읽을 reference: `coding-design/references/cmake-targets.md` (경계 인터페이스) · `threads-and-db.md`, `coding-rules/references/api-shape.md` (null 계약 · `std::optional`) · `values-and-platform.md`
3. 결정 대기 — 사용자에게 확정받는다
   - **비밀번호 해시 방식** (SageSDI는 솔트 없는 SHA-256 — 쓰지 않는다)
     | 선택지 | 장점 | 대가 |
     |---|---|---|
     | PBKDF2-HMAC-SHA256 (`QPasswordDigestor`) + 사용자별 솔트 | Qt 제공, 외부 의존 없음, 세 OS 동일 | `Qt::Network` 모듈에 있다 → core가 아니라 infra에서 계산해야 한다 (core는 `Qt::Core`만 링크) |
     | argon2id (libsodium 등 외부 라이브러리) | 현재 권장되는 가장 강한 방식 | 세 OS에서 외부 라이브러리를 빌드 · 배포해야 한다 |
     권장 — PBKDF2 (`QPasswordDigestor`, 반복 횟수는 착수 시점의 OWASP 권고치로). 해시 계산은 core의 경계 인터페이스(예: 비밀번호 해시기)로 정의하고 infra가 구현한다
   - **비밀번호 정책**: SageSDI는 4~15자다. 그대로 옮길지, 최소 길이를 올릴지. 권장 — 이관은 동작을 그대로 옮기고, 정책 강화는 별도 결정으로
4. 재확인할 사실
   - `SageUserService.cpp`의 검증 메시지 전체와 `Login` 흐름 (아래 표는 2026-09-23 기준 일부)

## SageSDI에서 옮길 것
원본 루트: `D:/Projects/SageSDI/SageSDI/`

**DTO** — `app/core/auth/SageUserDto.h`: `nUserId` · `strLoginId` · `strPwHash` · `nRole` · `nMustChangePw`. 역할 `USER_ROLE_USER = 0` · `USER_ROLE_ADMIN = 1` → `enum class`

**서비스** — `app/core/auth/SageUserService.h`
| 메서드 | 비고 |
|---|---|
| `Login(loginId, password, outDto, bSuccess, strError)` | 성공 여부와 오류를 분리해 돌려준다 |
| `AddUser(dto, plainPassword, nNewUserId, strError)` | |
| `ChangePassword(nUserId, newPassword, strError)` | 저장소에서 `must_change_pw`를 0으로 만든다 |
| `LoadAll(arrUsers, strError)` · `RemoveUser(nUserId, strError)` | |
| `ValidateLoginId(loginId, strError)` · `ValidatePassword(password, strError)` | |

**검증 규칙** — `SageDefine.h:430-433`
- 아이디 2~30자 (`SAGE_USER_LOGIN_ID_MIN_LEN = 2`, `MAX_LEN = 30`)
- 비밀번호 4~15자 (`SAGE_USER_PW_MIN_LEN = 4`, `MAX_LEN = 15`)
- 메시지 예: `"아이디를 입력하세요."`, `"아이디는 %d자 이상이어야 합니다."` → `QString::arg`로

**저장소 SQL** — `app/infra/db/SageUserRepository.cpp`
| 줄 | 동작 |
|---|---|
| 28 | `INSERT INTO SageUser ...` |
| 85 · 137 | `SELECT user_id, login_id, pw_hash, role, must_change_pw ...` (조건은 원문 확인) |
| 179 | `DELETE FROM SageUser WHERE user_id = ?;` |
| 221 | `UPDATE SageUser SET pw_hash = ?, must_change_pw = 0 ...` |
| 265 | `UPDATE SageUser SET role = ? ...` |
| 311 | `SELECT COUNT(*) FROM SageUser WHERE login_id = ?;` |

**기본 관리자** — `app/infra/db/SqlInitializer.cpp:20-45, 170-215`, `SageDefine.h:472-477`
- 아이디 `admin`이 없을 때만 만든다
- 비밀번호: 길이 14 (`SAGE_INITIAL_PW_LENGTH`), 문자 집합 `ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789` (헷갈리는 `I` · `O` · `l` · `0` · `1` 제외)
- `must_change_pw = 1`로 저장, 만든 비밀번호는 호출자에게 돌려줘 **한 번만** 보여준다 (표시는 T09)

**세션** — `app/core/auth/SageAuthSession.h`: `IsLoggedIn` · `IsAdmin` · `GetCurrentUser` · `SetLogin(dto)` · `Logout`

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `SageUserService::HashPassword` (솔트 없는 SHA-256, `BCrypt*`) | 약한 방식 + Windows API. 결정한 방식으로 대체 |
| `BCryptGenRandom` | `QRandomGenerator::system()`으로 대체 |
| `SageAuthSession` 싱글턴 (`#define sageAuth`) | 전역 금지. `main.cpp`가 소유하고 주입한다 |
| core가 infra를 직접 include (`SageUserService.h:5`) | SageSDI의 계층 위반. 경계 인터페이스로 바꾼다 |

## 함정
- **초기 비밀번호 생성의 편향**: SageSDI는 임의 바이트를 문자 집합 길이(57)로 나눈 나머지로 고른다 → 약간 치우친다. `QRandomGenerator::system()->bounded(57)`은 치우침이 없다. 동작(길이 · 문자 집합)은 같고 품질만 좋아진다
- 해시 방식이 SageSDI와 다르므로 SageSDI DB와 호환되지 않는다 — 배포본이 없어 문제는 없다. T04의 레거시 마이그레이션 생략 결정과 일관된다
- 저장 형식에 알고리즘 · 반복 횟수 · 솔트를 함께 담아 나중에 방식을 바꿀 수 있게 한다 (예: `pbkdf2-sha256$반복$솔트$해시`)
- 로그인 · 사용자 조회는 DB 작업이므로 UI 스레드에서 하지 않는다 (`ownership-and-threads.md`). 호출 흐름은 T10에서 `QtConcurrent`로 연결한다. 이 주제에서는 서비스가 스레드와 무관하게 동작하게 만든다
- 경계 인터페이스는 core에, 구현은 infra에 둔다. 해시기를 infra로 보내면 core의 `Login`은 해시기 인터페이스를 주입받는다

## 작업
PR 1개: `feature/auth` (해시기가 크면 분리)
- [x] core: 사용자 DTO · 역할 enum · 사용자 저장소 경계 인터페이스 · (필요 시) 해시기 경계 인터페이스 · 사용자 서비스 · 세션
- [x] infra: 사용자 저장소 구현 (위 SQL) · 해시기 구현 · 기본 관리자 생성
- [x] 쓰이는 문자열 · 길이 상수를 `SageDefine.h`로
- [x] 테스트: 검증 경계값(아이디 1·2·30·31자, 비밀번호 3·4·15·16자), 로그인 성공 · 실패, 비밀번호 변경 후 `must_change_pw = 0`, 기본 관리자는 한 번만 생성, 초기 비밀번호 길이 · 문자 집합, 같은 비밀번호도 솔트 때문에 해시가 다름

## 완료 기준
- 3-OS CI에서 빌드와 테스트가 모두 통과한다
- core에 infra include가 0개다 (CMake가 막는다 — 빌드 성공이 곧 확인)
- 비밀번호 검증 테스트가 SageSDI 규칙과 같은 결과를 낸다 — 경계값 4개(3 · 4 · 15 · 16자) · 빈 값 · 영문 · 숫자 외 문자 (아이디 검증은 호출 0곳이라 옮기지 않는다 — 결정 기록)

## 범위 밖
- 로그인 · 비밀번호 변경 화면 — T10
- 초기 비밀번호 표시 — T09
- 사용자 관리(추가 · 목록 · 삭제 · 역할 변경) — SageSDI에서 호출 0곳이라 옮기지 않는다 (결정 기록)

## 확인한 사실
- 결정: PBKDF2-HMAC-SHA256 600,000회 · 솔트 16바이트 · `pbkdf2-sha256$반복$솔트$해시`, 정책 4~15자 영문 · 숫자, 쓰이는 것만 이관 (`MIGRATION_PLAN.md` 결정 기록)
- `QPasswordDigestor`는 Qt Network 모듈 (`find_package(Qt6 COMPONENTS Network)`) → `sage_infra`가 `Qt::Network` PRIVATE 링크. OWASP 권고 PBKDF2-HMAC-SHA256 600,000회 (2026-09-27 확인)
- SageSDI `ValidatePassword`에는 길이 외에 **영문 대소문자 · 숫자만 허용** 검사가 있다 (`SAGE_UI_CHANGE_PW_INVALID_CHAR`). 검증은 `private`이고 `AddUser` · `ChangePassword`에서만 쓰인다 — `Login`은 검증하지 않는다
- SageSDI 호출 지점: `Login`(로그인 창) · `ChangePassword`(`SagePasswordChangeDlg.cpp:221`) · 초기 관리자 생성(`SqlInitializer.cpp:170-215`). `AddUser` · `LoadAll` · `RemoveUser` · `UpdateRole`은 호출 0곳 → 사용자 관리 화면이 없다 (T12 · T10 반영)
- SageSDI `ChangePassword`는 현재 비밀번호를 확인하지 않는다 — 다이얼로그 쪽 확인 여부는 T10 재확인 항목
- SageSDI 해시 비교는 대소문자 무시(`CompareNoCase`, hex) — SageQt는 PBKDF2 바이트 비교
- 초기 관리자 생성은 SageSDI에서 infra(`SqlInitializer`)였지만 "없으면 만든다"는 업무 규칙이라 core `SageUserService::ensureDefaultAdmin`에 두었다. 해시기 · 저장소는 경계 인터페이스로 주입
- 해시기 반복 횟수를 생성자로 받는다 — 테스트는 1,000회. 검증은 저장된 반복 횟수를 쓰므로 나중에 횟수를 올려도 기존 해시가 검증된다 (테스트로 고정)

## 결과
- 작업 브랜치 CI 통과 후 `develop`에 squash merge (새 반영 절차)
- core `auth/` 7개 파일, infra `auth/` 3개 · `db/SageUserRepository`, 테스트 5개(`tests/core/auth/` 2 · `tests/infra/auth/` 2 · `tests/infra/db/` 1). 로컬 테스트 12개 모두 통과
- 교훈
  - 원본 코드의 호출 지점을 먼저 세면 옮길 범위가 줄어든다 — 계획에 있던 사용자 관리 기능 4개와 아이디 검증이 호출 0곳이었다
