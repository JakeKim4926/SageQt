# 타입 · 클래스 · 반환 규약

`coding-rules`의 상세 규칙이다. 함수 시그니처 · 반환 타입 · signal/slot · 클래스 선언 · include를 쓸 때 읽는다.

## 타입 규칙

### `bool`
`bool`만 쓴다.

### 문자열
- `QString`을 쓴다
- 형식 문자열은 `QString::arg`로 만든다. `sprintf` 계열을 쓰지 않는다

### 컨테이너
- 기본은 Qt 컨테이너다: `QList`, `QStringList`, `QHash`, `QMap`, `QSet`
- 복사할 수 없는 원소(`std::unique_ptr` 등)만 `std::vector`에 담는다
- 한 API 안에서 Qt 컨테이너와 표준 컨테이너를 섞지 않는다
- const가 아닌 Qt 컨테이너를 range-for로 돌 때는 `std::as_const`로 감싼다 — Qt 컨테이너는 암시적 공유라 불필요한 복사(detach)가 일어난다

```cpp
for (const SageResultRow& row : std::as_const(m_rows)) {
    ...
}
```

### enum
`enum class`만 쓴다.

메타 객체 등록(`Q_ENUM`)이 필요한 enum — 위젯 property, `QVariant` 저장 — 은 그것을 쓰는 `Q_OBJECT` / `Q_GADGET` 클래스 **안에** 선언한다.
`SageDefine.h`의 공용 enum은 등록하지 않는다. 클래스 밖 enum은 `Q_NAMESPACE` + `Q_ENUM_NS`와 moc 처리가 필요한데, `sage_define`은 moc이 돌지 않는 헤더 전용 타깃이다.

---

## 클래스 작성 규칙

- 인자 하나로 호출할 수 있는 생성자는 `explicit`
- 오버라이드에는 `override`를 쓰고 `virtual`을 반복하지 않는다
- 상태를 바꾸지 않는 멤버 함수는 `const`
- 모든 멤버는 선언부 기본값으로 초기화한다 (포인터는 `nullptr`)
- `QObject`를 상속하는 **모든** 클래스에 `Q_OBJECT`를 쓴다 — signal이 없어도 쓴다. 빠지면 `qobject_cast`와 `metaObject()`가 잘못 동작한다 (Qt 공식 권고)

### 선언 순서

```cpp
class SageSamplePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SageSamplePanel(QWidget* parent = nullptr);

    QString inputPath() const;
    void setRunning(bool isRunning);

signals:
    void runRequested(const QString& inputPath);

private slots:
    void onRunButtonClicked();

private:
    void createWidgets();
    void createLayout();
    void connectSignals();

private:
    QLineEdit* m_inputEdit = nullptr;
    QPushButton* m_runButton = nullptr;
    bool m_isRunning = false;
};
```

`public` (생성자 → 접근자·설정자 → 동작) → `signals` → `public slots` → `protected` (오버라이드) → `private slots` → `private` 함수 → `private` 멤버 변수.
멤버 변수는 별도의 `private:` 구역에 둔다.

---

## signal / slot 규칙

- `connect`는 함수 포인터 문법만 쓴다. `SIGNAL()` / `SLOT()` 문자열 매크로 금지 (컴파일 타임 검사 불가)
- 이름 기반 자동 연결(`on_<객체>_<signal>` + `connectSlotsByName`) 금지
- 멤버 함수 slot을 우선하고, 람다는 `ownership-and-threads.md`의 *소유권 규약 4*를 따른다
- slot으로 쓰는 함수는 `slots` 구역에 선언한다
- signal 인자는 값 또는 `const T&`다. 포인터를 signal로 넘기지 않는다
- 패널은 내부 위젯의 signal을 그대로 밖에 노출하지 않고 의미 있는 signal로 다시 낸다 (`coding-design/references/ui-composition.md`의 *소유 규칙*)

---

## 반환 규약

### 실패 가능한 함수
`bool`을 반환하고 실패 이유는 `QString& outError`로 전달한다.

```cpp
bool loadWorkflowRows(const QString& inputPath, QList<SageResultRow>& outRows, QString& outError);
```

### 단순 상태 변경 / UI 갱신 함수
`void`를 사용한다.

```cpp
void updateExportButtonState();
void setRunning(bool isRunning);
```

### 예외 사용 원칙
복구 불가능한 초기화 실패, 내부 계약 위반 등 치명적 오류에만 제한적으로 사용한다.
복구 가능한 일반 실패는 `bool + outError`를 쓴다.
**예외가 slot · 이벤트 핸들러 · 백그라운드 작업 밖으로 나가게 두지 않는다.** Qt 이벤트 루프는 예외를 전파하지 않는다.

### null 계약 (CRITICAL)

**null 가능성은 반환 타입이 결정하고, 이름이 그것을 드러낸다.**

| 상황 | 반환 | 이름 | 호출부 |
|---|---|---|---|
| 항상 존재하는 객체 | `T&` / `const T&` | 명사 (`userService()`) | 검사하지 않는다 |
| 없을 수 있는 객체 (비소유) | `T*` | `find*` (`findHandler()`) | **반드시** `nullptr` 검사 |
| 없을 수 있는 값 | `std::optional<T>` | 조회 동사 | `has_value()` 검사 |
| 소유권 이전 | `std::unique_ptr<T>` | `create*` | 호출자가 소유 |

```cpp
SageUserService& userService();
ISageWorkflowHandler* findHandler(SageWorkflowType type);

ISageWorkflowHandler* handler = findHandler(type);
if (handler == nullptr) {
    return;
}
```

**이름·타입과 호출부가 어긋나면 호출부가 아니라 이름과 타입을 고친다.**
`find*`인데 항상 유효하면 참조를 반환하는 명사 접근자로 바꾼다.

출력 매개변수에 포인터를 넘기지 않는다 (`T*& out` 금지). 없을 수 있는 조회 결과는 `std::optional`로 채운다.

```cpp
bool selectByLoginId(const QString& loginId, std::optional<SageUserDto>& outUser, QString& outError);
```

### 매개변수

| 종류 | 형태 |
|---|---|
| 입력, 복사가 싼 타입 (`int`, `bool`, enum) | 값 |
| 입력, 그 외 | `const T&` |
| 출력 | `T&` + `out` 접두사 |
| 비소유 필수 | `T&` |
| 비소유 생략 가능 | `T*` (기본값 `nullptr`) |
| Qt 부모 | `QWidget* parent` / `QObject* parent` |

### 프레임워크 / 외부 라이브러리 시그니처

아래는 외부 계약이므로 위 규약에 맞추려고 바꾸지 않는다.

- Qt 가상 함수 오버라이드 (`paintEvent`, `resizeEvent`, `data`, `drawControl` 등)

---

## include 규칙

- 헤더는 `#pragma once`
- 프로젝트 헤더는 계층 이름부터 쓴다: `#include "core/workflow/SageWorkflowRegistry.h"`
- 공통 상수는 `#include "SageDefine.h"`
- Qt 헤더는 클래스명만 쓴다: `#include <QPushButton>` (`<QtWidgets/QPushButton>` 쓰지 않음)
- 순서: 대응 헤더 → 프로젝트 헤더 → Qt → 표준 라이브러리. 그룹 사이는 빈 줄
- 각 파일은 쓰는 헤더를 직접 include한다. 미리 컴파일된 헤더(pch)에 기대지 않는다
- 헤더에서 포인터·참조로만 쓰는 타입은 전방 선언한다
