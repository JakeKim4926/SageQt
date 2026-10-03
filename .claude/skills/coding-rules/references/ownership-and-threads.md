# 소유권 · 스레드

`coding-rules`의 상세 규칙이다. 객체를 만들거나(`new`) 연결하거나(`connect`) 백그라운드로 보낼 때 읽는다.

## 소유권 규약 (CRITICAL)

**소유권은 주석이 아니라 타입과 Qt 객체 트리로 드러낸다.**
`delete`를 쓰지 않으므로 해제는 아래 세 경로뿐이다.

| 대상 | 소유 방법 | 해제 |
|---|---|---|
| `QObject` 파생 (위젯 포함) | 생성 시 부모 지정 `new T(parent)` | 부모가 파괴될 때 자동 |
| `QObject`가 아닌 객체 | 값 멤버, 또는 `std::unique_ptr<T>` | 소유자가 파괴될 때 자동 |
| 자기 이벤트 처리 중에 없어져야 하는 `QObject` | `deleteLater()` | 이벤트 루프가 처리 |

### 규약 1 — `new`는 부모가 있는 `QObject`에만 쓴다

```cpp
// 금지
SageUserService* service = new SageUserService(repository);
QLabel* label = new QLabel();
layout->addWidget(label);

// 올바름
m_titleLabel = new QLabel(this);
std::unique_ptr<SageUserService> service = std::make_unique<SageUserService>(repository);
```

부모 없이 만든 위젯을 레이아웃에 넣어 부모를 얻게 하는 방식도 쓰지 않는다. **생성하는 그 줄에서 소유자가 보여야 한다.**

최상위 창과 모달 다이얼로그는 스택 값으로 만든다.

예외: **Qt API가 문서로 소유권을 가져가는 경우**에는 부모 없이 만들어 그 줄에서 넘긴다 — `QApplication::setStyle(new SageStyle)`, `QIcon(new SageIconEngine(glyph))`. `QIconEngine::clone()`은 Qt가 요구하는 복제 함수라 `return new SageIconEngine(...)`으로 새 엔진을 돌려주고, 받은 `QIcon`이 소유한다. 넘긴 뒤의 포인터는 빌려 쓰는 것이다 (규약 2). 이 예외는 소유권 이전이 Qt 문서에 명시된 API에만 쓴다.

레이아웃 · 모델에 넘기는 객체도 이 예외에 든다 (사용자 결정 2026-10-03) — `QBoxLayout::addLayout` · `QLayout::addItem` · `QStandardItemModel::appendRow` · `QStandardItem::appendRow`. 조건은 **만든 줄 바로 다음 줄에서 넘기는 것**이다. 두 줄 사이에 다른 코드를 두지 않는다. 넘긴 뒤 멤버로 들고 있는 포인터(`QSpacerItem* m_filterGap`)는 빌린 것이다 (규약 2).

```cpp
m_filterGap = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
m_bandLayout->addItem(m_filterGap);

QHBoxLayout* actionLayout = new QHBoxLayout();
formLayout->addLayout(actionLayout, 2, 1);
actionLayout->addWidget(m_runButton);
```

설정을 먼저 하고 나중에 넘기거나(`new QHBoxLayout()` → `addWidget` 여러 줄 → `addLayout`), 만든 객체를 함수 밖으로 반환한 뒤 넘기는 형태(`return new QStandardItem(...)`)는 이 예외에 들지 않는다.

```cpp
SageLoginDlg dialog(this);
if (dialog.exec() != QDialog::Accepted) {
    return;
}
```

스택 객체에 부모를 줄 때는 **부모가 먼저 생성되어 있어야 한다.** 나중에 생성된 부모는 먼저 파괴되면서 스택에 있는 자식까지 해제하고, 자식은 스코프를 벗어날 때 한 번 더 해제된다.

### 규약 2 — raw 포인터는 절대 소유하지 않는다

raw 포인터(`T*`)는 항상 **빌려 쓰는 것**이다.
멤버 `QPushButton* m_runButton`의 소유자는 포인터가 아니라 Qt 부모다.

- `std::shared_ptr` 금지. 소유자가 둘 필요가 생기면 설계를 다시 본다
- **`QObject`를 `std::unique_ptr`로 소유하지 않는다.** 부모와 `unique_ptr`가 같은 객체를 각각 해제한다 (`Qt::WA_DeleteOnClose`를 켠 창도 같다). `QObject`의 소유자는 언제나 Qt 부모 또는 스택이다
- 수명이 먼저 끝날 수 있는 `QObject`를 오래 가리켜야 하면 `QPointer<T>`를 쓴다

### 규약 3 — 소멸자를 직접 쓰지 않는다 (Rule of Zero)

정리는 멤버(`std::unique_ptr`, 값 멤버, Qt 부모)가 한다. 소멸자가 필요해 보이면 소유 구조가 잘못된 신호다.

예외는 **정리 순서를 지켜야 하는 외부 자원을 감싸는 RAII 클래스**뿐이다.
예: DB 연결 스코프 — 연결 객체를 먼저 비운 뒤 `QSqlDatabase::removeDatabase`를 호출해야 한다.
이 클래스만 소멸자를 갖고, 복사를 `= delete`로 막는다.

인터페이스(`ISage*`)의 `virtual ~ISage*() = default;`는 정리 코드가 없어 이 규약의 대상이 아니다. `std::unique_ptr<ISage*>`로 구현체를 지울 때 필요하므로 모든 인터페이스에 둔다.

### 규약 4 — 람다 연결에는 반드시 수신 객체를 넘긴다

```cpp
// 금지 — this가 먼저 파괴되면 크래시
connect(m_watcher, &QFutureWatcher<SageWorkflowResult>::finished, [this]() { applyResult(); });

// 올바름 — this가 파괴되면 연결도 자동 해제
connect(m_watcher, &QFutureWatcher<SageWorkflowResult>::finished, this, &SageWorkflowController::onWorkflowFinished);
```

멤버 함수 연결을 우선한다. 람다는 인자를 가공해야 할 때만 쓰고, 그때도 3번째 인자로 수신 객체를 넘긴다.

### 소유권 체크리스트

코드 작성 후 반드시 확인한다.

- [ ] `delete`가 없는가
- [ ] 모든 `new`가 생성 시점에 부모를 받는 `QObject`인가
- [ ] `QObject`가 아닌 소유 객체는 값 또는 `std::unique_ptr`인가
- [ ] `QObject`를 `std::unique_ptr`로 들고 있지 않은가
- [ ] 람다 `connect`에 수신 객체가 있는가
- [ ] 사용자 정의 소멸자가 RAII 래퍼에만 있는가
- [ ] 스레드 경계를 넘는 데이터가 포인터가 아니라 값인가

---

## 스레드
- UI 스레드에서 DB 조회, 파일 처리, 문서 생성 등 무거운 작업 금지
- 백그라운드 코드에서 `QWidget`과 UI 스레드 소속 `QObject`에 접근 금지 — **값을 받고 값을 돌려준다**. 앱 수명 동안 사는 서비스 · 등록부의 참조만 예외다 (`coding-design/references/threads-and-db.md`)
- UI 스레드를 막는 대기 금지: `QFuture::waitForFinished()`, `Qt::BlockingQueuedConnection` (교착 위험)
- `QThread` 서브클래싱 금지 — 백그라운드 작업은 `QtConcurrent::run`으로 통일한다
- 백그라운드 작업은 예외를 밖으로 던지지 않는다 — 실패는 결과 값에 담는다
- **DB 연결(`QSqlDatabase`)은 그것을 연 스레드에서만 쓴다** (Qt 공식 제약) — 연결을 여닫는 구조는 `coding-design/references/threads-and-db.md`

무엇을 UI 스레드에 두고 무엇을 백그라운드로 보낼지는 `coding-design/references/threads-and-db.md`를 따른다.
