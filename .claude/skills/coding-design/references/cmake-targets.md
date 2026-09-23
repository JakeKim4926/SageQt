# CMake 타깃 · 파일 절차

`coding-design`의 상세 규칙이다. 파일을 추가 · 이동 · 삭제할 때, `CMakeLists.txt`를 만질 때, core가 infra 기능을 써야 할 때 읽는다.

## `core/core/`처럼 폴더가 겹치는 이유

바깥은 **타깃 폴더**(`CMakeLists.txt`가 있는 곳), 안쪽은 **include 접두사**다.
각 타깃은 자기 바깥 폴더만 include 루트로 공개한다.
그래서 `sage_ui`는 링크한 `sage_core`의 헤더(`"core/..."`)는 보지만, 링크하지 않은 `sage_infra`의 헤더(`"infra/..."`)는 **파일을 찾지 못해 컴파일 에러**가 난다.
모든 타깃이 소스 루트 하나를 include 루트로 공유하면 이 보호가 사라진다 — 헤더가 디스크에서 찾아지고, 실행 파일이 모든 타깃을 링크하므로 링크까지 통과한다.

---

## CMake 타깃이 의존 방향을 강제한다 (CRITICAL)

| 타깃 | 폴더 | 링크 |
|---|---|---|
| `sage_define` | `SageQt/define` | `Qt::Core` |
| `sage_common` | `SageQt/common` | `sage_define`, `Qt::Core` |
| `sage_core` | `SageQt/core` | `sage_common`, `sage_define`, `Qt::Core` |
| `sage_infra` | `SageQt/infra` | `sage_core`, `sage_common`, `sage_define`, `Qt::Core`, `Qt::Sql`(PRIVATE) |
| `sage_ui` | `SageQt/ui` | `sage_core`, `sage_common`, `sage_define`, `Qt::Widgets`, `Qt::Concurrent` |
| `SageQt` | `SageQt/` (`main.cpp`) | 위 전부 |
| 테스트 | `tests/` | 대상 타깃 + `Qt::Test` |

- `sage_core`는 `Qt::Gui` / `Qt::Widgets`를 링크하지 않는다 → `core`에서 `<QWidget>`을 include하면 컴파일 에러
- `sage_ui`는 `sage_infra`를 링크하지 않는다 → `ui`에서 `"infra/..."`를 include하면 컴파일 에러
- `Qt::Sql`은 `sage_infra`가 **PRIVATE**로 링크한다 → `QSqlDatabase` · `QSqlQuery`가 `infra` 밖으로 새지 않는다. Repository의 공개 헤더에는 DTO와 경계 인터페이스만 나타난다
- **`target_link_libraries` 변경은 계층 규칙 변경이다.** 컴파일 에러를 없애려고 링크를 추가하지 않는다 — 배치가 잘못됐다는 신호다. 변경이 필요해 보이면 작업을 멈추고 승인을 받는다
- `file(GLOB)` 금지. 소스 파일은 목록에 명시한다 (새 파일이 리뷰에서 드러나도록)

## 계층 경계 인터페이스

구현체가 하나뿐인 인터페이스는 만들지 않는다 — **단, `core`가 `infra` 기능을 쓰기 위한 경계 인터페이스는 예외다.**
`core`는 `infra`를 include할 수 없으므로(CMake가 막는다) 구현체가 하나여도 인터페이스가 있어야 한다.

- 경계 인터페이스는 **`core`에 둔다**: `core/auth/ISageUserRepository.h`
- 구현은 **`infra`에 둔다**: `infra/db/SageUserRepository` (`ISageUserRepository` 구현)
- 조립은 **`main.cpp`에서 한다**

같은 계층 안에서는 여전히 두 번째 구현이 실제로 필요해질 때 인터페이스를 추출한다.

---

## 신규 파일 추가 절차 (CRITICAL — 2단계 모두 수행)

1. **디스크에 파일 생성** — 위 폴더 구조에 맞는 경로에 생성한다.
2. **그 계층 타깃의 `CMakeLists.txt` 소스 목록에 추가한다.**
   `SageQt/core/core/workflow/SageWorkflowRegistry.cpp` → `SageQt/core/CMakeLists.txt`

`core` · `infra` 클래스를 추가하거나 동작을 바꾸면 `tests/`의 같은 계층 경로에 테스트를 추가하거나 고친다.

### 파일 이동 시

- [ ] 이동한 파일을 참조하는 모든 `#include` 경로를 수정했는가
- [ ] 옮기기 전 타깃의 목록에서 지우고, 옮긴 후 타깃의 목록에 추가했는가 (중복 금지)
- [ ] 이동으로 계층 의존 방향이 깨지지 않는가
- [ ] 대응 테스트 파일도 함께 옮겼는가

### 빌드 정의

- 빌드 정의는 `CMakeLists.txt`와 `CMakePresets.json`이 유일한 원천이다
- IDE 프로젝트 파일(`.sln`, `.vcxproj`, `.xcodeproj`, `.pro`)을 만들거나 커밋하지 않는다. 예외는 저장소 루트의 `SageQt.slnx` 하나다 — Visual Studio 진입점으로 유지하며, 빌드 설정을 담지 않는다
- OS별 차이(컴파일러, Qt 경로, 아키텍처)는 `CMakePresets.json`에 둔다
- `CMakeLists.txt`의 `if(WIN32)` / `if(APPLE)` 분기는 패키징·배포 설정에만 허용한다
- 리소스(폰트 · 아이콘)는 **`SageQt` 실행 파일 타깃에 등록한다.** 정적 라이브러리에 넣은 리소스는 링커가 등록 코드를 제거할 수 있다
