# 값 · 크로스 플랫폼

`coding-rules`의 상세 규칙이다. 숫자 · 문자열 · 색 · 경로를 쓰거나, 파일 · 프로세스 · OS 기능이 필요할 때 읽는다.

## 하드코딩 금지 (CRITICAL)

**코드 어디에도 값을 직접 박아 넣지 않는다. 이 규칙은 예외 없이 적용된다.**

### 숫자 리터럴 (매직 넘버)
```cpp
// 금지
if (selectedCount > 12) { ... }

// 올바름 — SageDefine.h
constexpr int SAGE_RESULT_PAGE_MAX_ROWS = 12;
```

**0 · 1 · 2 · -1도 예외가 아니다** (사용자 결정 2026-10-03). 앱 코드(`SageQt/`)의 모든 숫자 리터럴은 이름 붙은 상수다.
- **같은 뜻은 공통 상수 하나**를 쓴다. 기하 값은 `SageDesignDefine.h`, 나머지는 `SageDefine.h`에 둔다
  - 기하: `SAGE_NO_MARGIN`(여백 · 간격 0), `SAGE_CENTER_DIVISOR`(가운데 맞춤 2), `SAGE_RECT_EDGE_OFFSET`(`QRect`의 `right() + 1` 보정 1)
  - 그 밖: `SAGE_INDEX_NONE`(없음 -1), `SAGE_FIRST_INDEX`(첫 번째 0)
  - 같은 뜻의 상수가 이미 있으면 새로 만들지 않는다 (`SAGE_FILTER_CRITERIA_NONE`처럼 도메인 이름이 붙은 것은 그 도메인에서 쓴다)
- **화면마다 뜻이 다른 값은 그 자리의 이름**을 준다
  - 그리드 행 · 열 번호 → 그 화면 클래스 안의 `enum class` (`SageLoginDlgGridRow::Password`)
  - 표의 열 → model의 열 enum (`SageResultColumn::Check`)
- 공통 상수 이름은 이 문서에 추가한 뒤 쓴다. 쓰는 곳마다 새 이름을 만들지 않는다

### 문자열 리터럴 (매직 스트링)
```cpp
// 금지
QString extension = QStringLiteral(".xlsx");
m_statusLabel->setText(QStringLiteral("파일을 찾을 수 없습니다."));

// 올바름 — SageDefine.h
inline const QString SAGE_FILE_EXT_XLSX = QStringLiteral(".xlsx");
inline const QString SAGE_UI_FILE_NOT_FOUND = QStringLiteral("파일을 찾을 수 없습니다.");
```

**UI 표시 문자열은 `SageDefine.h`에 `SAGE_UI_` 접두사 상수로 선언한다.**
이 프로젝트는 다국어를 지원하지 않으므로 `tr()`과 번역 파일을 쓰지 않는다.
- `SAGE_UI_` 접두사는 **화면에 보이는 문자열에만** 붙인다. 직렬화 구분자처럼 화면에 나오지 않는 문자열은 붙이지 않는다
- **문구 안에 다른 상수의 값을 다시 쓰지 않는다** (사용자 결정 2026-10-03). `"4~15자"`가 아니라 `"%1~%2자"`로 두고 `arg(SAGE_USER_PW_MIN_LEN).arg(SAGE_USER_PW_MAX_LEN)`으로 채운다. 값을 바꿀 때 문구가 따라온다

### 디자인 값 (색 · 여백 · 폰트 크기)
위젯·화면 코드에 색 리터럴(`QColor(...)`, `"#RRGGBB"`)과 여백·크기 숫자를 쓰지 않는다.
디자인 값은 **`ui/style/SageDesignDefine.h` 한 곳**에 둔다. `SageStyle`, delegate, 커스텀 위젯이 모두 이 값을 쓴다.
세부 목록은 `sageqt-ui/references/design-values.md`에 있다.

### 파일 경로 / 폴더명
```cpp
// 금지
QString dataPath = QStringLiteral("C:/SageQt/data/");

// 올바름 — QStandardPaths / QCoreApplication::applicationDirPath / 앱 설정 서비스
```

### 식별자 집합은 `enum class`
```cpp
// 금지
if (workflowType == 1) { ... }

// 올바름 — SageDefine.h
enum class SageWorkflowType { Sample = 1 };
if (workflowType == SageWorkflowType::Sample) { ... }
```

정해진 값 집합이 화면에도 보이면 **enum과 표시 문구를 나눈다** (사용자 결정 2026-10-03). 로직은 enum을 비교하고, 문구는 표시하는 자리에서 `SAGE_UI_` 상수로 바꾼다. 예: 결과 상태 → `SageResultStatus` enum + `SAGE_UI_RESULT_STATUS_*`.

### 테스트 데이터
테스트의 입력 데이터(JSON · 경로 · 문자열)는 리터럴로 쓴다. 기대값은 테스트의 목적에 따른다.
- 값 자체를 원본과 대조하는 테스트(예: 핸들러 라벨이 SageSDI와 같은가)는 **리터럴** — 상수를 쓰면 상수가 틀려도 통과한다
- 순서 · 구조를 보는 테스트(예: 결과 행의 순서)는 **앱 상수** — 값 대조는 위 테스트가 맡는다
- **화면 픽셀 색 · 크기를 확인하는 테스트는 값 대조다** → 리터럴 (`QColor(220, 214, 205)`). `SAGE_COLOR_*`를 기대값으로 쓰지 않는다 (사용자 결정 2026-10-03)
- 숫자 상수 규칙(0 · 1 · 2 · -1 포함)은 테스트의 입력 · 값 대조 기대값에 적용하지 않는다. 단, **앱에 enum이 있는 값**(표의 열 번호 등)은 테스트도 그 enum을 쓴다 (`index(0, 4)`가 아니라 `SageHistoryColumn`)

### 상수 위치 결정 규칙

| 성격 | 위치 |
|------|------|
| 앱 공통 상수, 버퍼 크기, 타임아웃 | `SageDefine.h` |
| UI 표시 문자열 | `SageDefine.h` (`SAGE_UI_` 접두사) |
| 워크플로 / 태스크 타입 | `SageDefine.h` (`enum class`) |
| 도메인이 명확한 전용 상수 | 그 모듈 옆 `Sage*Define.h` |
| SQL 문자열 | 해당 Repository 내부 상수 |
| 경로 / 환경 정보 | 런타임 경로 API |
| 색 · 여백 · 폰트 크기 | `ui/style/SageDesignDefine.h` |

`SageDefine.h`가 과도하게 비대해지면 도메인 전용 `Sage*Define.h`로 분리한다.
도메인별 블록 구분에 주석을 쓰지 않는다. 접두사(`SAGE_UI_`, `SAGE_WORKFLOW_`)로 그룹을 드러낸다.

**상수를 다른 헤더의 동적 초기화 상수(`QString` · `QColor` · `QList` 등)로 초기화하지 않는다.** 전역 상수의 초기화 순서는 파일 사이에서 정해져 있지 않아, 빈 값을 복사할 수 있다 (T10에서 `SAGE_FONT_FILES`가 빈 패밀리 이름을 받아 폰트 등록이 실패했다). 다른 상수가 가져다 쓰는 값은 컴파일 시간에 정해지는 타입(`constexpr` 정수 · `QStringView`)으로 둔다.

### 하드코딩 의심 패턴 체크리스트

- [ ] 숫자 리터럴이 코드 안에 직접 있는가 → `SageDefine.h`
- [ ] 문자열 리터럴이 로직·화면 코드 안에 있는가 → `SageDefine.h`
- [ ] 색·여백 값이 위젯 코드에 있는가 → `SageDesignDefine.h`
- [ ] 경로 문자열이 코드 안에 있는가 → 런타임 경로 API
- [ ] 타입을 정수 리터럴로 비교하는가 → `enum class`
- [ ] SQL 문자열이 흩어져 있는가 → Repository 내부 상수로 집약

---

## 크로스 플랫폼 (CRITICAL)

**코드는 세 OS에서 동일해야 한다. OS별 차이는 CMake 세팅에만 둔다.**

- `#ifdef Q_OS_*` / `_WIN32` / `__APPLE__` / `__linux__` 금지
- OS API 직접 호출 금지 — `<windows.h>`, Cocoa, POSIX 헤더를 include하지 않는다
- 코드 안의 경로 구분자는 항상 `/`다. `\\` 금지. 화면 표시가 필요할 때만 `QDir::toNativeSeparators`
- 경로를 문자열 연결로 만들지 않는다 → `QDir::filePath` / `QFileInfo`
- `#include` 경로와 리소스 경로는 실제 파일명과 **대소문자까지** 일치시킨다 (Linux는 구분한다)
- 텍스트 폭을 숫자로 가정하지 않는다. 폰트 메트릭은 OS마다 다르다 → `QFontMetrics` 또는 레이아웃
- 소스 파일은 **UTF-8 (BOM 없음)** 으로 저장한다

Qt 대안이 없어 보이면 **작업을 멈추고 승인을 받는다.**
승인된 경우에만 `infra/platform/`에 격리한다 (`coding-design` 참조).

| 필요한 것 | 쓰는 것 |
|---|---|
| 실행 파일 위치 | `QCoreApplication::applicationDirPath()` |
| 설정·데이터 저장 위치 | `QStandardPaths` |
| 설정 읽기·쓰기 | `QSettings` |
| 외부 프로세스 실행 | `QProcess` |
| 파일 선택 창 | `QFileDialog` |
| 기본 앱으로 열기 | `QDesktopServices::openUrl` |
| 난수 | `QRandomGenerator::system()` |
| 무결성 해시 | `QCryptographicHash` |
| JSON | `QJsonDocument` / `QJsonObject` |
| 날짜·숫자 표시 형식 | `QLocale` |
| DB | `QSqlDatabase` / `QSqlQuery` (`QSQLITE`) |

비밀번호 해시는 단순 해시(`QCryptographicHash` 포함)로 만들지 않는다. 방식은 인증 이관 주제(`sageqt-plan`)에서 결정해 이 문서에 반영한다.

### Qt가 제공하는 기능을 직접 구현하지 않는다
JSON 파싱, 경로 조작, 날짜·숫자 형식, 인코딩 변환, 해시를 문자열 처리로 직접 만들지 않는다. 위 표의 Qt 클래스를 쓴다.
