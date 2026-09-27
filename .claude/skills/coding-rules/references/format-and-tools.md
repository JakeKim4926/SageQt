# 포맷 · 정적 분석

`coding-rules`의 상세 규칙이다. 포맷이 애매할 때, clang-format · clang-tidy · clazy 설정을 만질 때 읽는다.

## 포맷

`clang-format` 설정은 이 규칙을 따른다.

- 들여쓰기는 스페이스 4칸. 탭 금지
- 한 줄 최대 120자
- 클래스·함수의 여는 중괄호는 새 줄, 제어문(`if` / `for` / `while` / `switch`)은 같은 줄
- **제어문 본문은 한 줄이어도 중괄호를 쓴다**
- 포인터·참조 기호는 타입에 붙인다: `QWidget* parent`, `const QString& path`
- 생성자 초기화 리스트는 한 줄에 하나, 쉼표를 앞에 둔다

```cpp
SageUserService::SageUserService(ISageUserRepository& repository, const SageDbConfig& config)
    : m_repository(repository)
    , m_config(config)
{
}
```

---

## 컴파일 경고

| 컴파일러 | 경고 수준 |
|---|---|
| MSVC | `/W4` |
| GCC · Clang | `-Wall -Wextra -Wpedantic` |

- 경고 옵션은 소스 루트 `SageQt/CMakeLists.txt`에서 컴파일러 생성식(`$<CXX_COMPILER_ID:MSVC>`)으로 지정한다. 컴파일러에 따른 분기이며, 금지된 OS 분기(`if(WIN32)` 등)가 아니다
- 로컬에서는 경고로 보이고, **CI에서는 에러로 처리한다** — 경고가 남은 코드는 머지되지 않는다
- 경고를 끄는 `#pragma`나 `-Wno-*` · `/wd` 옵션을 코드와 CMake에 추가하지 않는다. 필요해 보이면 멈추고 승인을 받는다

---

## 정적 분석

이 문서의 규칙은 사람의 기억이 아니라 도구가 검사한다.

| 도구 | 검사 대상 |
|---|---|
| 컴파일러 | *컴파일 경고* (CI에서는 에러) |
| `clang-format` | *포맷* 전부 |
| `clang-tidy` | 네이밍 형식(대소문자 · 접두사), `NULL` · C 캐스트 금지, `explicit` · `override` · 멤버 함수 `const` |
| `clazy` | Qt 전용 — detach, `Q_OBJECT` 누락, connect 오용 등 (level1 + `missing-qobject-macro` · `old-style-connect`) |
| CI grep | `auto` 키워드, 플랫폼 분기(`Q_OS_` · `_WIN32` · `__APPLE__` · `__linux__` · `<windows.h>`) — clang-tidy에는 `auto`를 금지하는 검사가 없다 |

clazy의 `non-pod-global-static`은 끈다. `SageDefine.h`의 `inline const QString` 상수가 대상인데, 실행 파일 하나에 들어가는 상수라 시작 비용이 무시할 수준이다.

### 도구로 검사하지 않는 규칙

아래 네이밍 규칙은 clang-tidy로 표현할 수 없다. 리뷰(`code-review-expert`)가 검사한다.

- `bool` 멤버 · 접근자의 `is` / `has` / `can`
- 출력 매개변수의 `out` 접두사
- `get` 접두사 금지
- signal 이름(과거형 · 상태 변화)과 slot 이름(`on` + 발신원 + 사건)
- 클래스 역할 접미사 · 위젯 멤버 접미사
- 약어 PascalCase (`Sql`이지 `SQL`이 아니다)
