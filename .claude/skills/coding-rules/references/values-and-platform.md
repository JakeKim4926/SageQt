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

### 디자인 값 (색 · 여백 · 폰트 크기)
위젯·화면 코드에 색 리터럴(`QColor(...)`, `"#RRGGBB"`)과 여백·크기 숫자를 쓰지 않는다.
디자인 값은 **`ui/style/SageDesignDefine.h` 한 곳**에 둔다. `SageStyle`, delegate, 커스텀 위젯이 모두 이 값을 쓴다.
세부 목록은 UI 스킬(`sageqt-ui`)이 정한다.

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

비밀번호 해시는 단순 해시(`QCryptographicHash` 포함)로 만들지 않는다. 방식은 로직 이관 단계에서 결정해 이 문서에 반영한다.

### Qt가 제공하는 기능을 직접 구현하지 않는다
JSON 파싱, 경로 조작, 날짜·숫자 형식, 인코딩 변환, 해시를 문자열 처리로 직접 만들지 않는다. 위 표의 Qt 클래스를 쓴다.
