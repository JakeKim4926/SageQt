# 네이밍 규칙

`coding-rules`의 상세 규칙이다. 클래스 · 함수 · 변수 · 상수 · 파일의 이름을 지을 때 읽는다.

## 네이밍 규칙

### 접두사는 `Sage` (CRITICAL)

**모든 클래스의 접두사는 `Sage`를 쓴다.** 의뢰처 이름을 접두사로 쓰지 않는다.
의뢰처를 구분해야 하면 접두사가 아니라 **폴더**로 나눈다 (`coding-design` 참조).
프로젝트·실행 파일 이름인 `SageQt`는 그대로 쓴다.

### 이름 형식

**타입 접두사(헝가리안: `str`, `n`, `b`, `p`, `m_wnd`)는 쓰지 않는다. 타입은 선언이 말하고, 이름은 역할을 말한다.**

| 대상 | 형식 | 예 |
|---|---|---|
| 클래스 | `Sage` + PascalCase | `SageUserService` |
| 인터페이스 | `ISage` + PascalCase | `ISageWorkflowHandler` |
| 함수 | camelCase, 동사 + 목적어 | `loadWorkflowRows()` |
| 접근자 / 설정자 | 명사 / `set` + 명사 | `title()` / `setTitle()` |
| `bool` 접근자 | `is` / `has` / `can` + 상태 | `isRunning()` |
| 멤버 변수 | `m_` + camelCase | `m_inputPath` |
| `bool` 멤버 변수 | `m_` + `is` / `has` / `can` + 상태 | `m_isRunning` |
| 매개변수 / 지역 변수 | camelCase | `inputPath`, `isValid` |
| 출력 매개변수 | `out` + PascalCase | `outRows`, `outError` |
| 상수 | `SAGE_` + UPPER_SNAKE | `SAGE_DLG_CAPTION_HEIGHT` |
| enum 타입 | `Sage` + PascalCase, `enum class` | `SageButtonVariant` |
| enum 값 | PascalCase | `SageButtonVariant::Primary` |
| signal | 과거형 또는 상태 변화 | `workflowCompleted`, `loginSucceeded` |
| slot | `on` + 발신원 + 사건 | `onRunButtonClicked()` |
| 테스트 클래스 | `Sage` + 대상 + `Test` | `SageUserServiceTest` |
| 테스트 함수 | camelCase, 기대 동작 서술 | `loginFailsWithWrongPassword()` |
| 파일 | 클래스명과 동일 | `SageUserService.h` / `.cpp` |

`get` 접두사는 쓰지 않는다 (Qt 관례). null 계약은 `api-shape.md`의 *null 계약*에 따라 반환 타입과 `find*`가 표현한다.

enum 값에 타입명을 반복하지 않는다 — `SageButtonVariant::Primary`이지 `SageButtonVariant::ButtonPrimary`가 아니다.

약어는 PascalCase로 통일한다 (`Sql`, `Db`, `Pdf`, `Json`). `SQLInitializer` 같은 전대문자 표기는 쓰지 않는다.

금지 예: `MyDoc`, `Manager1`, `TempClass`, `Helper`, `Utils`, `Mgr`

### 클래스 역할 접미사

| 대상 | 규칙 | 예 |
|------|------|-----|
| 메인 창 | `SageMainWindow` | — |
| 화면 영역 (`QWidget` 파생) | `Sage` + 역할 + `Panel` | `SageWorkspacePanel` |
| `QDialog` 파생 | `Sage` + 역할 + `Dlg` | `SageLoginDlg` |
| 커스텀 위젯 | `Sage` + 역할 | `SageFilterPillBar` |
| item model | `Sage` + 대상 + `Model` | `SageResultTableModel` |
| item delegate | `Sage` + 대상 + `Delegate` | `SageResultTableDelegate` |
| Service / Repository / Presenter / Controller | `Sage` + 역할 | `SageUserRepository` |
| DTO | `Sage` + 대상 + `Dto` | `SageUserDto` |

### 위젯 멤버는 역할 + 위젯 종류

```cpp
QPushButton* m_loginButton = nullptr;
QLineEdit* m_idEdit = nullptr;
QTableView* m_resultTable = nullptr;
```

| 위젯 | 접미사 |
|---|---|
| `QPushButton` / `QToolButton` | `Button` |
| `QLineEdit` / `QTextEdit` | `Edit` |
| `QLabel` | `Label` |
| `QComboBox` | `Combo` |
| `QCheckBox` / `QRadioButton` | `Check` / `Radio` |
| `QTableView` / `QTreeView` / `QListView` | `Table` / `Tree` / `List` |
| `QTabBar` / `QStackedWidget` | `Tabs` / `Stack` |
| 패널 · 커스텀 위젯 | 클래스의 역할 접미사 (`Panel`, `Bar` 등) |
