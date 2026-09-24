---
name: coding-design
description: >
  SageQt(C++20/Qt 6 Widgets/SQLite)에서 무엇을 어디에 만들지 결정할 때 반드시 확인하는 구조 설계 skill.
  파일 배치, 계층 분리, 의존 방향, CMake 타깃, 클래스 분할, 확장점 설계, 화면 구성, 스레드 구조를 다룬다.
  IMPORTANT: Claude가 새 파일이나 새 클래스를 만들거나 구조를 변경하는 모든 경우에 반드시 이 skill을 먼저 호출해야 한다. 사용자가 명시적으로 요청하지 않아도 자동으로 트리거된다.
  트리거 조건: 새 파일 생성, 새 클래스 생성, 파일 이동, 폴더 구조 변경, 계층 분리, 리팩토링, 워크플로 추가, 화면 추가, 클래스 분할, CMakeLists.txt 수정.
  "폴더 구조", "파일 배치", "파일 위치", "어디에 만들", "어디에 추가", "어디에 두", "구조 변경", "구조 개선", "리팩토링", "계층", "의존", "분리", "분할", "새 클래스", "새 파일", "만들어줘", "추가해줘", "CMake", "CMakeLists", "타깃", "target_link_libraries", "워크플로 추가", "화면 추가", "패널", "위젯" 등이 언급되면 이 skill을 트리거한다.
  CRITICAL: 파일을 생성하기 전에 이 skill로 배치를 먼저 확정한다. 배치가 정해진 뒤 코드를 작성할 때는 coding-rules를 함께 따른다.
---

# SageQt 구조 설계 규칙

이 skill은 **무엇을 어디에 만들지**를 다룬다.
배치가 정해진 뒤 **그 자리에 코드를 어떻게 쓸지**는 `coding-rules`를 따른다.

## 이 규칙이 지키는 목표

프로젝트 목표는 `CLAUDE.md`의 *프로젝트 목표*에, 배경과 결정 이력은 `docs/decisions/MIGRATION_PLAN.md`에 있다.
이 문서의 구조 규칙은 세 목표를 구조 수준에서 지키기 위한 것이다.

- **세 OS에서 같은 코드** — 계층마다 CMake 타깃을 두고, OS 차이는 `CMakePresets.json`에만 둔다
- **디자인 자유도** — 모양은 `SageStyle` 한 곳이 그리고, 화면은 그리는 방법을 모른다
- **품질 = 일관성 · 유지보수** — 업무 하나를 추가할 때 고칠 곳이 핸들러 파일과 등록부뿐이고, 계층 규칙은 빌드가 강제한다

규칙끼리 부딪히거나 문서에 없는 판단이 필요하면 이 목표에 비추어 정하고, 정한 내용은 이 문서에 반영한다.

---

## 프로젝트 전제

| 항목 | 사실 |
|------|------|
| UI 프레임워크 | Qt 6 Widgets |
| 스타일 | 커스텀 `QStyle` (Fusion 기반 `QProxyStyle`) + `QPalette` — QSS 없음 |
| 빌드 | CMake — 계층마다 타깃 하나 |
| 대상 OS | Windows · macOS · Linux — **코드는 하나, 세팅만 다르다** |
| 저장소 | SQLite — QtSql `QSQLITE` 드라이버 |
| 비동기 | `QtConcurrent::run` + `QFutureWatcher` |
| 웹 UI / QML / 플러그인 / 다국어 | **없음** |

이 프로젝트에 존재하지 않는 개념(MFC Doc/View, Win32 메시지, QML, 도킹 프레임워크)을 전제로 한 구조 판단은 하지 않는다.

---

## 폴더 구조 (엄격 적용)

**CRITICAL: 신규 파일은 반드시 아래 구조에 맞는 위치에 배치한다. 임의 위치에 생성하지 않는다.**

```
SageQt/                                 ← 저장소 루트
├── CMakeLists.txt                      ← 최상위 (하위 디렉터리 추가만)
├── CMakePresets.json                   ← OS별 세팅 — OS는 코드가 아니라 여기서 갈린다
├── CLAUDE.md
├── docs/
│   ├── DEBT_LOG.md
│   ├── RELEASE_NOTES.md
│   ├── decisions/                      ← PR_LOG.md, MIGRATION_PLAN.md
│   └── plans/done/                     ← 끝난 계획 주제 파일 (이력, sageqt-plan)
├── tests/                              ← Qt Test. 소스 계층 구조를 따라간다
│   ├── core/
│   └── infra/
│
└── SageQt/                             ← 소스 루트
    ├── CMakeLists.txt                  ← 실행 파일 타깃 + 하위 타깃 추가
    ├── main.cpp                        ← 조립 지점 (모든 계층을 아는 유일한 파일)
    │
    ├── define/                         ← sage_define (헤더 전용)
    │   └── SageDefine.h
    │
    ├── common/                         ← sage_common 타깃 폴더
    │   └── common/                     ← include 접두사
    │
    ├── core/                           ← sage_core 타깃 폴더
    │   └── core/
    │       ├── workflow/               ← 핸들러 인터페이스 · 레지스트리 · DTO
    │       │   └── handlers/           ← 업무별 핸들러 (업무 1종 = 파일 1쌍)
    │       └── auth/
    │
    ├── infra/                          ← sage_infra 타깃 폴더
    │   └── infra/
    │       ├── db/                     ← QtSql 연결 스코프 · 스키마 초기화 · Sage*Repository
    │       ├── file/
    │       ├── office/                 ← (문서 처리 도입 시 생성)
    │       └── platform/               ← (승인된 OS 전용 코드만. 승인 시 생성)
    │
    ├── ui/                             ← sage_ui 타깃 폴더
    │   └── ui/
    │       ├── window/                 ← SageMainWindow
    │       ├── panels/                 ← Sage*Panel
    │       ├── dialogs/                ← Sage*Dlg
    │       ├── widgets/                ← 커스텀 위젯 · delegate
    │       ├── models/                 ← item model
    │       ├── style/                  ← SageStyle · 팔레트 · SageDesignDefine.h
    │       └── workflow/               ← SageWorkflowController
    │
    └── resources/                      ← 폰트 · 아이콘 (실행 파일 타깃에 등록)
```

`core/core/`처럼 폴더가 겹치는 이유는 `references/cmake-targets.md`에 있다.

---

## 계층 의존 방향 (위반 금지)

```
ui  ──→  core  ←──  infra
 │         │          │
 └────→ common ←──────┘

main.cpp — 모든 계층을 조립한다
```

- **`ui`는 `core`를 호출한다.** `infra`를 모른다.
- **`core`는 아무것도 호출하지 않는다.** `infra`가 필요하면 `core`가 정의한 경계 인터페이스를 통해 주입받는다.
- **`infra`는 `core`의 경계 인터페이스를 구현한다.**
- `common`은 어떤 계층에도 의존하지 않는다.
- **`main.cpp`만** 모든 계층을 안다. `infra` 구현체를 만들어 `core`에 주입하고, 그 결과를 `ui`에 넘긴다.

이 방향을 CMake 타깃이 강제하는 방식과 경계 인터페이스 규칙은 `references/cmake-targets.md`에 있다.

---

## 상황별 요약

### 파일 · 클래스를 추가할 때
1. *배치 결정 순서*로 계층과 폴더를 정한다
2. 그 계층 타깃의 `CMakeLists.txt` 소스 목록에 등록한다 → `references/cmake-targets.md`
3. core가 infra 기능을 써야 하면 core에 경계 인터페이스를 둔다 → `references/cmake-targets.md`
4. 화면(창 · 패널 · 다이얼로그)이면 → `references/ui-composition.md`
5. 표를 보여주면 → `references/model-view.md`
6. 모양을 바꾸거나 커스텀 위젯을 그리면 → `references/style.md`
7. 백그라운드 작업 · DB 접근 · 앱 공통 객체가 필요하면 → `references/threads-and-db.md`
8. core · infra 클래스면 `tests/`의 같은 계층 경로에 테스트를 추가한다
9. 업무(워크플로)를 추가하면 핸들러 파일 1쌍 · core 소스 목록 · 업무 식별자 · 등록부 1곳만 고친다. 그 밖에 고칠 곳이 생기면 확장점이 없다는 신호다 → `references/ui-composition.md` 완료 기준 D

### 구조를 수정할 때
1. `target_link_libraries`를 바꿔야 할 것 같으면 **멈추고 승인을 받는다** — 배치가 잘못됐다는 신호다
2. 한 클래스의 책임이 늘어나면 완료 기준 A~E로 점검한다 → `references/ui-composition.md`
3. 패널을 나눌 때는 위젯 · 레이아웃 · 연결을 한 번에 옮긴다 → `references/ui-composition.md`
4. core · infra 클래스의 동작이 바뀌면 대응 테스트를 고친다

### 파일을 옮길 때
`references/cmake-targets.md`의 *파일 이동 시* 체크리스트를 따른다.

### 파일 · 클래스를 삭제할 때
1. 그 계층 타깃의 `CMakeLists.txt` 소스 목록에서 지운다
2. `tests/`의 대응 테스트를 지운다
3. 그 파일을 include하던 곳이 남지 않았는지 확인한다
4. 워크플로 핸들러라면 등록부에서 지운다. 사이드바는 등록부에서 만들어지므로 따로 고치지 않는다
5. 경계 인터페이스의 마지막 구현을 지우면, 인터페이스와 `main.cpp`의 조립 코드도 함께 지운다
6. 코드 안의 참조(상수 · `connect` 등) 정리는 `coding-rules`의 *코드를 삭제할 때*를 따른다

---

## 배치 원칙

1. **`core/`** — `Qt::Core`만 쓴다. 업무 규칙, 워크플로 흐름, DTO, 경계 인터페이스만 배치한다.
2. **`infra/`** — SQLite, 파일 시스템, 외부 프로세스, (도입 시) 문서 처리. 외부 의존을 여기서 끝낸다.
3. **`ui/`** — 위젯, 창, 다이얼로그, model, delegate, 스타일. SQL 실행, 파일 파싱, 문서 생성 금지.
4. **`common/`** — 도메인을 모르는 순수 유틸리티만. Qt가 이미 제공하는 기능은 여기에 만들지 않는다.
5. **`define/`** — `SageDefine.h` 하나. 모든 계층이 공유하는 상수와 `enum class`.
6. **신규 파일 생성 시** — 위 구조에서 속할 폴더를 먼저 확정한 후 생성한다.

### 배치 결정 순서

새 코드를 어디에 둘지 판단할 때 아래 순서로 묻는다.

1. `QWidget` · `QDialog` · item model · delegate를 상속하거나 화면을 다루는가 → `ui/`
2. SQLite / 파일 시스템 / 외부 프로세스 / 외부 라이브러리를 직접 부르는가 → `infra/`
3. 업무 규칙이나 워크플로 흐름인가 → `core/`
4. 위 어디에도 속하지 않고 도메인을 모르는 순수 유틸리티인가 → `common/`

2번과 3번이 동시에 해당하면 **둘로 쪼갠다.** 흐름은 `core`, 외부 호출은 `infra`로 나누고
`core`가 경계 인터페이스를 정의한다.

---

## 계층별 책임

### Window / Panel / Dialog (`ui/`)
역할: 사용자 입력 수집, 상태 표시, 위젯 배치, signal 연결

직접 해서는 안 되는 것: SQL 실행, 파일 파싱, 문서 생성, 장시간 동기 작업

### Model / Delegate (`ui/models/`, `ui/widgets/`)
역할: `core`의 데이터를 표시 형태로 바꾼다. **model은 데이터 변환만, 그리기는 delegate가 한다.**

### Service (`core/`)
역할: 업무 흐름 조합, 검증, 실행 단위 관리

### Repository (`infra/db/`)
역할: QtSql로 쿼리 실행, 행 매핑. **SQL 문자열과 QtSql 타입은 Repository 밖으로 나가지 않는다.**
연결은 주입받은 연결 설정으로 작업 안에서 열고 닫는다 (`references/threads-and-db.md`).

### Exporter / Converter (`infra/office/`, 도입 시)
역할: 문서 포맷 세부 처리, 외부 라이브러리 의존 캡슐화

---

## 파일 / 모듈 작성 원칙

- 하나의 클래스는 하나의 주된 책임만 가진다
- 파일 하나에 과도하게 많은 클래스를 넣지 않는다
- `Helper`, `Utils`, `Manager`, `Mgr` 같은 모호한 이름은 쓰지 않는다
- 계층이 서로의 세부 구현을 직접 물지 않도록 한다
- 구현체가 하나뿐인 인터페이스는 만들지 않는다 (경계 인터페이스는 예외 — `references/cmake-targets.md`)

---

## 상세 규칙 파일

| 파일 | 읽을 때 | 담긴 것 |
|---|---|---|
| `references/cmake-targets.md` | 파일을 추가 · 이동 · 삭제할 때, `CMakeLists.txt`를 만질 때, core가 infra 기능을 쓸 때 | include 루트를 가두는 이유 · 타깃 표 · 경계 인터페이스 · 신규 파일 절차 · 이동 체크리스트 · 빌드 정의 |
| `references/ui-composition.md` | 창 · 패널 · 다이얼로그를 만들거나 나눌 때, 업무를 추가할 때 | 창 비대화 방지 · 핸들러가 답하는 것 · 화면 클래스 구성 · 탭 · 소유 규칙 · 작성 형태 · 완료 기준 A~E |
| `references/style.md` | 모양을 바꾸거나 커스텀 위젯 · delegate를 그릴 때 | 화면이 그리기를 모르게 하는 구조 · `SageStyle` 스타일 규칙 |
| `references/model-view.md` | 표를 보여줄 때 | model · proxy · header · delegate 책임 분담 |
| `references/threads-and-db.md` | 백그라운드 작업 · DB 접근 · 앱 공통 객체가 필요할 때 | 스레드 역할 · 결과 전달 구조 · DB 연결 · 전역 상태 |

---

## 구조 판단 기준

문서에 없는 세부 판단이 필요할 때는 아래 기준을 따른다.

1. `core`에 `Qt::Gui` / `Qt::Widgets` 의존이 새지 않는가
2. `ui`가 `infra`를 모르는가
3. 워크플로를 하나 추가할 때 새 파일 + 등록부 1곳으로 끝나는가
4. `SageMainWindow`를 수정하지 않고 기능을 추가할 수 있는가
5. 지금 만드는 추상화에 실제 사례가 둘 이상 있는가 (경계 인터페이스는 예외)
6. 클래스 하나가 하나의 책임만 갖는가
7. **위젯 · 연결 · 레이아웃이 같은 클래스에 함께 있는가**
8. **화면 영역을 하나 추가할 때 창을 고쳐야 하는가** (레이아웃 1줄 외에 고칠 것이 있으면 배치가 잘못된 것이다)
9. 세 OS에서 같은 코드로 동작하는가
10. `target_link_libraries`를 바꾸지 않고 해결되는가
11. 표 데이터가 model 한 곳에만 있는가
12. 모양을 바꾸는 코드가 `SageStyle`과 `SageDesignDefine.h` 밖에 없는가

배치를 확정한 뒤 코드를 작성할 때는 **`coding-rules`**(네이밍, 타입, 반환 규약, 소유권 규약, 하드코딩 금지)를 함께 따른다.
