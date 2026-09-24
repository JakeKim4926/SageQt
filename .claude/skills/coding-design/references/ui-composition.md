# 화면 구성

`coding-design`의 상세 규칙이다. 창 · 패널 · 다이얼로그를 만들거나 나눌 때, 업무(워크플로)를 추가할 때 읽는다.

## MainWindow 비대화 방지 (CRITICAL)

`SageMainWindow`는 이 프로젝트에서 가장 커지기 쉬운 파일이다.
기능이 워크플로 단위로 늘어나는데 그 분기가 전부 창으로 모이면 창 하나가 프로젝트 전체를 삼킨다.

### 금지

**워크플로 타입을 조건으로 하는 `if` / `switch`를 창과 패널에 추가하지 않는다.**

```cpp
// 금지 — 워크플로가 늘어날 때마다 창을 수정하게 된다
if (workflowType == SageWorkflowType::Sample) {
    ...
}
```

같은 조건 분기가 이미 다른 함수에 있다면, 그것은 확장점이 없다는 신호다.

### 대신

워크플로 고유 동작은 **워크플로 핸들러**(`core/workflow/`)에 둔다.
화면은 현재 워크플로 핸들러에게 물어보는 방식으로 동작한다.

핸들러가 답해야 하는 것:
- 사이드바 표시 라벨과 분류
- 탭 구성 / 표시 라벨
- 출력 폴더가 필요한지
- 행 선택이 필요한지
- 결과 컬럼 정의
- 응답을 결과 행으로 변환하는 방법

**사이드바 트리는 등록부의 핸들러 목록으로 만든다.** 업무를 추가할 때 사이드바 코드를 고치지 않는다.

**판단 기준: 워크플로를 하나 추가할 때 아래 *완료 기준 D*의 목록 밖을 고치지 않아도 되는가.**
창·패널·사이드바나 결과 컬럼·응답 파싱·탭 구성을 함께 고쳐야 한다면 확장점 설계가 잘못된 것이다.

(등록부 수정까지 없애려면 자동 등록 구조가 필요한데, C++에서는 정적 초기화 순서와
링커의 미참조 오브젝트 제거 때문에 함정이 많다. 등록부 1곳은 허용한다.)

---

## ui 계층 구성 (CRITICAL)

**금지 규칙만으로는 창이 커지는 것을 막지 못한다.** "워크플로 분기를 넣지 마라"를 지켜도
위젯 소유 · signal 연결 · 레이아웃 · 실행 조정 · 상태 보관이 전부 합법적으로 창에 쌓인다.
그래서 **무엇이 독립 클래스여야 하는지**를 아래에 규정한다.

### 화면 클래스 구성

```
SageMainWindow                          조립과 최상위 배치만
  ├ SageSidebarPanel                    업무 탐색
  ├ SageHeaderPanel                     제목 · 상태 · 인증 표시
  └ SageWorkspacePanel                  탭 + QStackedWidget
       ├ SageWorkflowInputPanel         입력 · 저장 위치 · 실행 · 전체선택
       │    └ SageResultTablePanel      입력 표 + 필터
       ├ SageWorkflowResultPanel
       │    └ SageResultTablePanel      결과 표 + 필터
       └ SageWorkflowHistoryPanel       실행 기록

  업무 전용 패널은 여기에 형제로 추가한다.

SageWorkflowController                  실행 상태 전이 · 백그라운드 작업 수명
```

새 화면 영역이 필요하면 **창에 넣지 않고 패널을 추가한다.**

### 탭은 패널 단위로 나눈다 (CRITICAL)

**탭 전환은 `QStackedWidget`의 현재 패널을 바꾸는 것이다.**
한 위젯이 여러 탭의 위젯을 함께 들고 `setVisible` 행렬로 켜고 끄지 않는다.

가시성과 배치가 서로 다른 조건으로 갈리면 "보이는데 배치되지 않은 위젯"이 만들어진다.
패널이 탭 단위이면 **패널이 숨을 때 자식 전부가 함께 숨고, 보일 때 자기 레이아웃이 적용된다.**

**한 위젯이 두 탭에서 다른 역할을 겸하게 두지 않는다.** 겸하면 소유자를 정할 수 없다.
역할이 둘이면 **인스턴스를 둘로 만든다** (예: 입력 표와 결과 표는 같은 `SageResultTablePanel`의 **두 인스턴스**다).

**`QDockWidget`은 쓰지 않는다.** 필요한 것은 자유 배치가 아니라 반복 업무의 예측 가능한 화면이다.
도킹 상태 저장·플로팅은 요청된 적 없는 기능이다.

### 소유 규칙 (위반 금지)

1. **업무 위젯은 그 영역 패널이 소유한다** (Qt 부모 = 패널). 창은 최상위 패널만 소유한다
2. **위젯의 signal 연결은 그 위젯을 소유한 패널이 한다.** 창에 **중계 slot**(패널 대신 받아 넘기는 slot)을 만들지 않는다. 패널은 밖으로 **의미 있는 signal**만 낸다 (`runRequested(inputPath)`이지 `runButtonClicked()`가 아니다)
3. **레이아웃은 패널이 자기 `QLayout`으로 정한다.** 부모는 패널을 자기 레이아웃에 넣기만 한다. **자식 위젯의 좌표·크기를 코드로 지정하지 않는다** (`move` / `resize` / `setGeometry` 금지). 최소·최대 크기 제약은 허용하되 값은 디자인 값에서 가져온다
4. **실행 상태와 화면 상태는 컨트롤러 또는 패널이 보관한다.** 창 멤버로 두지 않는다

### 창의 역할은 네 가지뿐이다

- 최상위 패널 생성
- 최상위 레이아웃 배치
- 활성 업무 패널 교체
- 앱 수준 연결 (패널 signal ↔ 컨트롤러)

워크플로 입력값 · 진행률 · 결과 표 · 인증 표시의 **세부 구현은 창 밖에 있어야 한다.**

### 화면 클래스 작성 형태

모든 패널 · 다이얼로그 · 창의 생성자는 **`createWidgets()` → `createLayout()` → `connectSignals()`** 순서로 구성한다.
위젯이 아직 없는 화면 클래스는 빈 함수를 만들지 않는다 (CLAUDE.md 2). 첫 위젯을 추가할 때 이 형태로 만든다.
선언 순서는 `coding-rules/references/api-shape.md`의 *선언 순서*를 따른다.

```cpp
SageSamplePanel::SageSamplePanel(QWidget* parent)
    : QWidget(parent)
{
    createWidgets();
    createLayout();
    connectSignals();
}

void SageSamplePanel::createWidgets()
{
    m_inputEdit = new QLineEdit(this);
    m_runButton = new QPushButton(SAGE_UI_RUN_BUTTON, this);
}

void SageSamplePanel::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_inputEdit);
    layout->addWidget(m_runButton);
}

void SageSamplePanel::connectSignals()
{
    connect(m_runButton, &QPushButton::clicked, this, &SageSamplePanel::onRunButtonClicked);
}

void SageSamplePanel::onRunButtonClicked()
{
    if (m_isRunning) {
        return;
    }
    emit runRequested(m_inputEdit->text());
}
```

**분리는 패널 단위 수직으로 한다.** 한 패널의 위젯 · 레이아웃 · signal 연결을 **한 번에** 만든다.
축을 나눠 만들면 "위젯은 패널이 갖는데 연결은 창에" 같은 중간 상태가 생기고, 그때 만든 중계 slot을 다음 단계에서 다시 지우게 된다.

### 완료 기준은 책임이다 (CRITICAL)

**목표는 줄 수를 줄이는 것이 아니라 한 클래스가 여러 책임을 지지 않게 하는 것이다.**
줄 수·멤버 수를 목표로 삼으면 짐을 패널 하나로 옮겨 창만 얇아지는 통과가 가능하다.
지표는 통과하고 과중화된 클래스는 이름만 바뀐 채 옆으로 옮겨간다.

아래 다섯을 **모든 화면 클래스에 적용한다.** 창만의 기준이 아니다.
새로 만드는 패널도 만든 직후 같은 기준으로 점검한다.

**A. 이 파일이 열리는 이유가 하나인가**
클래스마다 "무엇이 바뀌면 이 파일을 고치는가"를 나열한다. 둘 이상이면 그 수만큼의 책임이 있다.
`SageMainWindow`의 이유는 **화면 조립이 바뀔 때** 하나여야 한다.
워크플로가 늘어서 · 결과 표 컬럼이 바뀌어서 · 업무 계산식이 바뀌어서 열린다면 아직 남아 있다.

**B. 무엇을 알아야 하는가**
창은 **워크플로 종류 · SQL · 그리는 방법 · 좌표를 몰라야 한다.**
측정: `SageWorkflowType` 비교, `paintEvent`, `setPalette` / `setFont`, `move` / `resize` / `setGeometry` 호출이 남아 있으면 지식이 새어 있다.
(`infra` include는 CMake가 막으므로 측정할 필요가 없다.)

**C. 위젯 · 연결 · 레이아웃이 한 클래스에 함께 있는가**
셋 중 하나라도 다른 클래스에 있으면 분리가 덜 된 것이다. 중계 slot이 그 증거다.

**D. 하나 추가할 때 고칠 지점이 몇 곳인가**
- 워크플로 1종 추가 = 아래 4곳뿐 (사이드바 포함 다른 곳은 수정 없음)
  1. `core/workflow/handlers/`에 핸들러 파일 1쌍
  2. `SageQt/core/CMakeLists.txt` 소스 목록에 그 2개 (`file(GLOB)` 금지 — `cmake-targets.md`)
  3. `SageDefine.h`에 업무 식별자 상수 1개
  4. 등록부 1곳
- 화면 영역 1개 추가 = 패널 1개 + 창의 레이아웃 1줄

**E. 중계 slot 0개**
창이 패널 대신 받아 넘기는 slot을 두지 않는다.

**`SageWorkflowController`**
- `QObject`. 실행 상태와 진행 상태 보관, 백그라운드 작업 시작 · 완료 · 오류 조정
- **위젯 API 호출 금지** (`setText` / `setModel` 류)
- 결과와 진행률은 **signal로 내보내고**, 패널이 자기 slot에서 받는다

### 증상 지표 (목표가 아니다)

아래 수치는 넘어야 할 선이 아니라 **A~E를 통과했는데도 값이 크면 아직 못 본 책임이 있다**는
역방향 점검 도구다. 수치를 맞추려고 코드를 옮기지 않는다.

| 지표 | 참고값 |
|---|---|
| 창의 업무 위젯 멤버 | 0개 |
| 창 · 패널의 워크플로 타입 분기 | 0곳 |
| 중계 slot | 0개 |

줄 수 · 연결 수 참고값은 Qt 코드가 생긴 뒤 실측해서 정한다.
