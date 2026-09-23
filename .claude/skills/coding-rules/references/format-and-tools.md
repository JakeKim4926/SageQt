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

## 정적 분석

이 문서의 규칙은 사람의 기억이 아니라 도구가 검사한다.

| 도구 | 검사 대상 |
|---|---|
| `clang-format` | *포맷* 전부 |
| `clang-tidy` | 네이밍 형식, `auto` · `NULL` · C 캐스트 금지, `explicit` · `override` · `const` |
| `clazy` | Qt 전용 — detach, `Q_OBJECT` 누락, connect 오용 등 |

clazy의 `non-pod-global-static`은 끈다. `SageDefine.h`의 `inline const QString` 상수가 대상인데, 실행 파일 하나에 들어가는 상수라 시작 비용이 무시할 수준이다.
