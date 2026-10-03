# 리뷰 점검 항목

`code-review-expert`의 Step 3 · Step 4다. 리뷰할 때마다 읽고, 변경 성격에 해당하는 항목을 점검한다.

## Step 3. 규칙 위반 체크

### A. coding-rules 체크 (모든 C++ 변경)

- `auto` 사용 여부
- `NULL` / C 스타일 캐스트 사용 여부
- `std::string` / `std::wstring` 사용 여부
- **주석이 추가되었는지**
- 함수 길이 과다 여부 (200줄 초과 금지)
- 매직 넘버 / 매직 스트링 → `SageDefine.h`
- 색 · 여백 리터럴 → `SageDesignDefine.h`
- 네이밍 형식 — camelCase 함수, `m_` 멤버, `out` 출력 매개변수, `Sage` 클래스 접두사, **헝가리안 잔재** (`str`, `n`, `b`, `p`, `m_wnd`)
- `bool` 멤버가 `m_is` / `m_has` / `m_can`으로 시작하는가
- slot 이름이 `on` + 발신원 + 사건인가
- 위젯 멤버 접미사가 담은 클래스의 종류와 맞는가 (패널 → `Panel`, 막대 → `Bar`)
- `get` 접두사 사용 여부
- `connect` 대상 함수가 모두 `slots` 구역에 선언되어 있는가
- 모든 값 타입 멤버에 선언부 기본값이 있는가 (스칼라 · enum · 포인터는 `= 값`, 클래스 타입은 `{}`, 참조 멤버만 제외)
- 각 파일이 쓰는 타입의 헤더를 직접 include하는가 · include 그룹 순서
- null 계약 — 참조 / `find*` + 포인터 / `std::optional` / `create*` + `unique_ptr`가 이름과 일치하는가
- 실패 가능한 함수가 `bool + QString& outError` 규약을 따르는지
- 비즈니스 전역 상태 · 싱글턴 추가 여부
- `SIGNAL()` / `SLOT()` 문자열 매크로, `connectSlotsByName` 사용 여부
- `QObject` 파생 클래스에 `Q_OBJECT`가 빠지지 않았는가
- `Q_ENUM`이 `Q_OBJECT` / `Q_GADGET` 클래스 안의 enum에만 쓰였는가
- const가 아닌 Qt 컨테이너를 range-for로 돌 때 `std::as_const`를 썼는가
- `explicit` / `override` / `const` 누락
- 컴파일 경고를 끄는 `#pragma` · `-Wno-*` · `/wd`가 추가되지 않았는가
- 포맷 — 제어문 중괄호, 포인터 기호 위치, 들여쓰기

### B. 소유권 체크 (CRITICAL)

**`new`, `connect`, 스레드 경계가 등장하는 모든 변경에서 반드시 확인한다.**

- `delete`가 등장하지 않는가
- 모든 `new`가 **생성 시점에** 부모를 받는 `QObject`인가 — 부모 없이 만들어 레이아웃에 넣는 패턴, `new QHBoxLayout()` · `new QSpacerItem` · `new QStandardItem`을 만든 뒤 다른 줄에서 넘기는 패턴 포함
- 테스트 코드에서도 `QObject`를 `std::unique_ptr`로 들고 있지 않은가
- `QObject`가 아닌 소유 객체가 값 또는 `std::unique_ptr`인가
- `std::shared_ptr`가 쓰이지 않았는가
- `QObject`를 `std::unique_ptr`로 소유하지 않는가 (부모와 이중 해제)
- **람다 `connect`에 수신 객체(3번째 인자)가 있는가** — 가장 자주 빠지는 지점
- 사용자 정의 소멸자가 RAII 래퍼에만 있는가
- 수명이 먼저 끝날 수 있는 `QObject`를 오래 가리키는 곳에 `QPointer`를 썼는가
- 스레드 경계를 넘는 데이터가 포인터가 아니라 값인가

### C. 크로스 플랫폼 체크 (CRITICAL)

**한 OS에서 빌드되고 돌아간다는 사실은 나머지 두 OS에 대해 아무것도 보장하지 않는다.**

- `#ifdef Q_OS_*` / `_WIN32` / `__APPLE__` / `__linux__`가 추가되지 않았는가
- OS 헤더(`<windows.h>` 등)를 include하지 않았는가
- 경로에 `\\`를 쓰거나 문자열 연결로 경로를 만들지 않았는가
- `#include` · 리소스 경로의 대소문자가 실제 파일명과 일치하는가 (Linux는 구분한다)
- 텍스트 폭을 숫자로 가정하거나 위젯 좌표를 고정하지 않았는가
- Qt가 제공하는 기능(JSON, 경로, 날짜, 해시)을 직접 구현하지 않았는가
- 소스 파일이 UTF-8 (BOM 없음)인가

### D. 계층 의존 체크

```
ui  ──→  core  ←──  infra
```

- **`target_link_libraries`가 바뀌었는가** — 바뀌었다면 승인 근거가 있는지 확인한다. 컴파일 에러를 없애려고 링크를 추가한 것이면 Blocker다
- `Qt::Sql`이 `sage_infra`에 PRIVATE로 남아 있고, QtSql 타입이 Repository 공개 헤더에 나타나지 않는가
- 리소스가 실행 파일 타깃에 등록되었는가
- `file(GLOB)`가 쓰이지 않았는가
- IDE 프로젝트 파일(`.sln` · `.vcxproj` 등)이 추가되지 않았는가 — 예외는 `SageQt.slnx` 하나
- **신규 파일이 올바른 계층 타깃의 `CMakeLists.txt`에 등록되었는가** (자주 누락된다)
- 신규 파일이 `coding-design`의 폴더 구조에 맞는 위치에 있는가
- 경계 인터페이스가 `core`에 정의되고 `infra`가 구현하며 `main.cpp`가 조립하는가
- `common`이 도메인 모델을 물지 않는가
- `core` · `infra` 변경에 대응하는 테스트가 추가·수정되었는가 — 새 클래스면 `tests/`의 같은 계층 경로에 테스트 파일이 있는가
- 새 폴더 · `target_link_libraries` 변경이 `coding-design`의 폴더 구조도 · 타깃 표에 반영되었는가 (승인 기록만 있고 표가 그대로면 Major)

### E. 창 비대화 체크

`SageMainWindow`는 이 프로젝트에서 가장 커지기 쉬운 파일이다.

- 워크플로별 `if` / `switch` 분기가 창이나 패널에 추가되지 않았는가
- 업무 추가 시 사이드바 코드를 고쳤는가 (등록부에서 만들어져야 한다)
- 새 기능이 창 대신 워크플로 핸들러 / 패널로 갔는가
- **중계 slot**(창이 패널 대신 받아 넘기는 slot)이 생기지 않았는가
- 화면 클래스에 `paintEvent` / `setPalette` / `setFont` / `move` / `resize` / `setGeometry`가 들어가지 않았는가
- `setStyleSheet` 호출이나 `.qss` 파일이 추가되지 않았는가
- 위젯을 가진 화면 클래스의 생성자가 `createWidgets()` → `createLayout()` → `connectSignals()` 형태인가 (위젯이 없으면 빈 함수를 만들지 않는다)
- Qt Designer `.ui` 파일이 추가되지 않았는가
- 패널이 자기가 소유하지 않은 위젯(자식의 자식)의 signal을 연결하거나 그 API를 accessor로 꺼내 직접 부르지 않는가 (`ui-composition.md` 소유 규칙 2)
- 바뀐 화면 클래스마다 완료 기준 A — 이 파일이 열리는 이유가 하나인가. 이번 변경으로 이유가 하나 늘었으면 Major

### F. DB 변경 체크

- SQL 문자열이 Repository 밖으로 새어 나가지 않았는가
- **DB 연결을 연 스레드 밖에서 쓰거나, 스레드 사이에서 공유·보관하지 않는가**
- 연결 이름이 작업마다 고유하고, 연결 스코프가 RAII로 닫히는가
- 스키마 변경 시 마이그레이션 경로가 있는가
- 트랜잭션 경계가 명확한가
- 조회 결과가 없는 경우를 정상 흐름(`std::optional`)으로 처리하는가

### G. 화면 변경 체크

- 표준 위젯의 모양을 `SageStyle` 밖(위젯 · 화면)에서 바꾸지 않았는가
- 색 · 여백 · 폰트 크기가 `SageDesignDefine.h`에서 오는가
- 위젯 변형이 `Q_PROPERTY`로 노출되고, 바뀔 때 `update()`를 호출하는가
- Qt 기본 위젯으로 되는 것을 서브클래싱하지 않았는가
- 표 데이터를 model이 유일하게 보관하고, 필터 · 정렬이 proxy에 있으며, 열 폭이 `QHeaderView` 모드로 정해지는가
- 고정 좌표 대신 레이아웃을 쓰는가
- `design-values.md`의 *쓰는 방식*이 **최소값**인 상수를 `setFixedWidth`나 그리기 계산의 고정 폭으로 쓰지 않았는가
- 같은 그리기 코드(체크 상자 등)가 `SageStyle`과 delegate · 커스텀 위젯에 따로 있지 않은가
- 새 디자인 상수 · 커스텀 위젯이 `design-values.md` · `widgets.md`에 반영되었는가
- 상태 변화(실행 중 / 완료 / 실패)가 예측 가능한가

세부 규격은 `sageqt-ui` 스킬(`design-values.md` · `style-scope.md` · `widgets.md` · `screens.md`)을 따른다. 특히 글자 폭 고정 · 포인트 폰트 · GDI식 서체 이름 · 규격 없는 값의 추측을 확인한다.

---

## Step 4. 구조 품질 리뷰

규칙 위반이 없더라도 아래를 따로 본다.

### 1. 책임 분리
- UI가 로직을 먹고 있지 않은가
- Service가 UI 사정을 과하게 알고 있지 않은가
- model이 그리기를, delegate가 데이터 변환을 하고 있지 않은가

### 2. 확장성
- 워크플로를 하나 더 추가할 때 이 코드가 또 수정되어야 하는가
- 특정 포맷 전용 명칭이 `core`로 스며들고 있지 않은가

### 3. 일관성
- 기존 네이밍 / 폴더 구조 / 계층 방향과 맞는가
- 이미 있는 패턴을 깨고 새 방식이 섞이지 않는가
- 동일 문제를 화면마다 다른 방식으로 풀고 있지 않은가

### 4. 오류 처리
- 실패 시 앱이 죽지 않는가
- 오류 메시지가 사용자에게 의미 있는가
- 복구 가능한 실패와 치명적 실패를 구분했는가
- 예외가 slot · 이벤트 핸들러 · 백그라운드 작업 밖으로 나가지 않는가

### 5. 비동기 / 스레드 안정성
- 오래 걸리는 작업이 UI를 막지 않는가
- 백그라운드 코드가 위젯이나 UI 스레드 객체에 닿지 않는가
- 작업 완료 전 화면 전환 / 앱 종료 상황을 고려했는가 — 종료 시 `main.cpp`가 작업 완료를 기다리는가
- 실행 중 같은 작업을 다시 실행할 수 있는 경로가 열려 있지 않은가
- race condition 가능성이 있는가
