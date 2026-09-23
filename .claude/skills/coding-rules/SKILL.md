---
name: coding-rules
description: >
  SageQt(C++20/Qt 6 Widgets/SQLite, Windows·macOS·Linux)에서 코드를 어떻게 쓸지 규정하는 코딩 규칙 skill.
  네이밍, 타입, 반환 규약, 소유권, 스레드, 하드코딩 금지, 크로스 플랫폼 금지 사항, 주석 금지, 포맷을 다룬다.
  IMPORTANT: Claude가 스스로 코드를 작성하거나 수정하는 모든 경우에 반드시 이 skill을 먼저 호출해야 한다. 사용자가 명시적으로 요청하지 않아도 자동으로 트리거된다.
  트리거 조건: 함수/클래스/변수 추가, 기존 코드 수정, 버그 수정, 상수 선언, 반환 타입 결정, 객체 생성과 소유권 결정, signal/slot 연결, 백그라운드 작업, PR 전 코드 검토.
  "코드 작성", "함수 추가", "변수 선언", "버그 수정", "수정해줘", "고쳐줘", "C++", "Qt", "위젯", "다이얼로그", "signal", "slot", "connect", "스레드", "QtConcurrent", "소유권", "메모리", "누수", "SQLite", "네이밍", "변수명", "함수명", "주석", "상수", "매직넘버", "하드코딩", "반환", "빌드", "컴파일", "크로스 플랫폼", "맥", "리눅스" 등이 언급되면 이 skill을 트리거한다.
  CRITICAL: 코드를 작성하기 전에 이 skill을 반드시 먼저 확인하고, 규칙을 위반하는 코드는 절대 작성하지 않는다.
  새 파일이나 새 클래스를 만들거나 구조를 바꾸는 작업이면 coding-design을 함께 확인한다.
---

# SageQt 코딩 규칙

이 skill은 **정해진 자리에 코드를 어떻게 쓸지**를 다룬다.
무엇을 어디에 만들지(폴더 배치, 계층 분리, CMake 타깃, 클래스 분할)는 **`coding-design`**을 따른다.

## 이 규칙이 지키는 목표

프로젝트 목표는 `CLAUDE.md`의 *프로젝트 목표*에, 배경과 결정 이력은 `docs/decisions/MIGRATION_PLAN.md`에 있다.
이 문서의 규칙은 그중 두 가지를 코드 수준에서 지키기 위한 것이다.

- **세 OS에서 같은 코드** — OS 차이는 CMake 세팅에만 둔다
- **품질 = 일관성** — 누가 어느 파일을 열어도 같은 규칙으로 쓰여 있다

규칙끼리 부딪히거나 문서에 없는 판단이 필요하면 이 목표에 비추어 정하고, 정한 내용은 이 문서에 반영한다.

---

## 프로젝트 전제

| 항목 | 사실 |
|------|------|
| 언어 | C++20 |
| UI 프레임워크 | Qt 6 Widgets |
| 스타일 | 커스텀 `QStyle` (Fusion 기반 `QProxyStyle`) + `QPalette` — QSS 없음 |
| 빌드 | CMake + `CMakePresets.json` |
| 대상 OS | Windows · macOS · Linux — **코드는 하나, 세팅만 다르다** |
| 저장소 | SQLite — QtSql `QSQLITE` 드라이버 |
| 비동기 | `QtConcurrent::run` + `QFutureWatcher` |
| 문자열 | `QString` |
| 공통 상수 | `SageDefine.h` |
| 문서 처리 | 미정 — 도입 시 `infra/office/` |
| 웹 UI / QML / 플러그인 / 다국어 | **없음** |

이 프로젝트에 존재하지 않는 개념(MFC, Win32 메시지, QML, 번역, 플러그인)을 전제로 한 판단은 하지 않는다.

## 목적

- 세 OS에서 **같은 코드**가 빌드되고 올바르게 동작한다
- 소유권이 **타입과 Qt 객체 트리**로 드러나 누수와 이중 해제가 구조적으로 생기지 않는다
- 주석 없이도 이름만으로 의도가 드러난다
- 누가 어느 파일을 열어도 같은 규칙으로 쓰여 있다

---

## 상황별 요약

### 코드를 작성할 때
1. 파일·클래스를 새로 만드는 작업이면 **`coding-design`으로 배치부터 확정**한다
2. 이름을 짓는다 → `references/naming.md`
3. 객체를 만들거나(`new`) 연결하거나(`connect`) 백그라운드로 보낸다 → `references/ownership-and-threads.md`
4. 함수 시그니처 · 반환 · signal/slot · 클래스 선언 · include를 쓴다 → `references/api-shape.md`
5. 숫자 · 문자열 · 색 · 경로를 쓰거나, 파일 · 프로세스 · OS 기능이 필요하다 → `references/values-and-platform.md`
6. 끝나면 *금지 사항*과 *최종 판단 기준*으로 점검한다. 포맷은 `references/format-and-tools.md`

### 코드를 수정할 때
1. 요청과 직접 이어지는 줄만 바꾼다. 인접 코드의 "개선"과 재포맷은 하지 않는다 (CLAUDE.md 3)
2. 바꾸는 줄은 이 규칙대로 쓴다. 같은 파일의 다른 위반은 고치지 않고 보고한다
3. 시그니처 · 이름 · 반환 타입이 바뀌면 모든 호출부와 대응 테스트를 함께 고친다
4. 이름 · 타입이 null 계약과 어긋나면 호출부가 아니라 이름과 타입을 고친다 → `references/api-shape.md`
5. 수정으로 쓰이지 않게 된 것은 *코드를 삭제할 때* 1번대로 지운다

### 코드를 삭제할 때
1. 이번 삭제로 쓰이지 않게 된 것 — 상수(`SageDefine.h` · `SageDesignDefine.h`), include, 전방 선언, `connect`, slot — 을 함께 지운다
2. 원래부터 쓰이지 않던 코드는 지우지 않고 보고만 한다 (CLAUDE.md 3)
3. 삭제한 코드를 쓰던 곳이 남지 않았는지 빌드로 확인한다
4. 파일 · 클래스 단위 삭제라면 `coding-design`의 *파일 · 클래스를 삭제할 때*를 함께 따른다

---

## 금지 사항

모든 작업에 적용한다.

### 언어
- `auto` 키워드 사용 금지 (구조화된 바인딩 포함)
- 널 포인터는 `nullptr`로 표기한다 (`NULL`, `0` 금지)
- **`delete` 키워드 사용 금지** — 해제는 소유권 규약의 세 경로로만 일어난다 (`references/ownership-and-threads.md`)
- C 스타일 캐스트 금지 → `static_cast` / `qobject_cast`
- `std::string` / `std::wstring` 금지 → `QString`

### 함수
- 함수 길이 200줄 초과 금지
- 권장 함수 길이 60~100줄 이내 유지
- **매직 넘버 / 매직 스트링 / 하드코딩 값 금지** → `references/values-and-platform.md`
- 비즈니스 로직 전역 변수 금지 (전역 인스턴스 규칙은 `coding-design/references/threads-and-db.md`)

### 주석
- **주석 작성 금지**
- 코드에 어떠한 주석도 작성하지 않는다
- 설명이 필요하다면 이름(클래스명, 함수명, 변수명)으로 의도를 드러낸다
- 기존 코드에 있는 주석도 수정 시 함께 제거한다

### 영역별 핵심 — 어기면 되돌리기 어려운 것

세부 규칙과 예시는 오른쪽 파일에 있다. 이 표는 그 파일을 읽지 않은 상태에서도 지켜야 하는 최소선이다.

| 영역 | 핵심 규칙 | 상세 |
|---|---|---|
| 크로스 플랫폼 | `#ifdef Q_OS_*` · OS API · `\\` 경로 금지. Qt 대안이 없으면 멈추고 승인을 받는다 | `references/values-and-platform.md` |
| 소유권 | `new`는 생성 시점에 부모를 받는 `QObject`에만. `QObject`를 `std::unique_ptr`로 소유하지 않는다. `std::shared_ptr` 금지. 람다 `connect`에는 수신 객체 | `references/ownership-and-threads.md` |
| 스레드 | UI 스레드에서 무거운 작업 금지. 백그라운드는 `QtConcurrent::run`, 값만 주고받는다. DB 연결은 연 스레드에서만 | `references/ownership-and-threads.md` |
| 하드코딩 | 숫자 · 문자열 → `SageDefine.h`, 색 · 여백 · 폰트 크기 → `SageDesignDefine.h`, 식별자 집합 → `enum class` | `references/values-and-platform.md` |
| 네이밍 | 클래스 `Sage` 접두사, 함수 camelCase, 멤버 `m_`. 헝가리안 · `get` 접두사 금지 | `references/naming.md` |
| 반환 | 실패 = `bool + outError`, 없을 수 있음 = `find*` + 포인터 / `std::optional`, 항상 존재 = 참조 | `references/api-shape.md` |
| Qt 관례 | 모든 `QObject` 파생에 `Q_OBJECT`. `connect`는 함수 포인터 문법만. QSS 금지 | `references/api-shape.md` · `coding-design/references/style.md` |

---

## 상세 규칙 파일

| 파일 | 읽을 때 | 담긴 것 |
|---|---|---|
| `references/naming.md` | 이름을 지을 때 | 이름 형식 표 · 클래스 역할 접미사 · 위젯 멤버 접미사 |
| `references/ownership-and-threads.md` | 객체를 만들거나 연결하거나 백그라운드로 보낼 때 | 소유권 규약 1~4 · 소유권 체크리스트 · 스레드 금지 사항 |
| `references/api-shape.md` | 함수 · 클래스를 선언할 때 | 타입 · 클래스 작성 · 선언 순서 · signal/slot · 반환 규약 · null 계약 · 매개변수 · include |
| `references/values-and-platform.md` | 값을 쓰거나 OS 기능이 필요할 때 | 하드코딩 금지 · 상수 위치 · 크로스 플랫폼 · Qt 대체 표 |
| `references/format-and-tools.md` | 포맷이 애매할 때, 도구 설정을 만질 때 | 포맷 · 컴파일 경고 · 정적 분석 |

---

## 최종 판단 기준

문서에 없는 세부 판단이 필요할 때는 아래 기준을 따른다.

1. UI 응답성을 해치지 않는가
2. 세 OS에서 같은 코드로 동작하는가
3. 소유권이 `delete` 없이 타입과 Qt 객체 트리로 특정되는가
4. null 가능성이 반환 타입과 이름(`find*`)에 일치하는가
5. 주석 없이 이름만으로 역할이 드러나는가
6. 실패 처리가 `bool + outError` 규약을 따르는가
7. 값이 코드에 직접 박혀 있지 않은가

새 파일·클래스를 만들거나 구조를 바꾸는 작업이면 **`coding-design`**을 함께 확인한다.
