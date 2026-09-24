# T01 — 정적 분석 · CI 경고 에러화

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
코딩 규칙을 사람의 기억이 아니라 CI가 검사하게 한다. 이 주제가 끝나면 포맷 · 네이밍 · Qt 관례 · 플랫폼 분기 · 컴파일 경고 위반이 있는 PR은 CI에서 실패한다.

## 시작 전에
1. 선행 주제: 없음
2. 스킬 로드: `git-workflow`, `coding-rules`, `coding-design`, `debt-log-guard`
   - 읽을 reference: `coding-rules/references/format-and-tools.md` (포맷 규칙 · 컴파일 경고 · 정적 분석 표), `coding-rules/references/naming.md` (네이밍 표), `coding-design/references/cmake-targets.md`
3. 결정 대기 — 사용자에게 확정받는다
   - `docs/DEBT_LOG.md` 신설과 아래 항목 기록 승인 (`debt-log-guard`는 기록 전 확인을 요구한다)
     ```
     ### [2026-09-23] 임시구현 — CI가 정식 릴리스가 아닌 aqtinstall 커밋을 쓴다
     - 위치: .github/workflows/build.yml (AQT_SOURCE)
     - 설명: Qt 6.11의 새 Windows 저장소 구조를 aqtinstall 3.3.0이 지원하지 않아 main 커밋 076e165(PR #1000 포함)로 고정했다. 정식 릴리스가 아니다.
     - 위험도: 중 — 고정 커밋이라 갑자기 깨지진 않지만, 릴리스 전 코드라 검증 범위가 좁다
     - 후속: aqtinstall 3.4.0 이상이 릴리스되면 aqtsource를 지우고 aqtversion으로 되돌린다
     ```
4. 재확인할 사실 — 착수 시점에 다시 실측한다
   - 로컬 clang-format · clang-tidy 버전 (2026-09-23 기준 VS 번들 22.1.3, `C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/`)
   - ubuntu-24.04 러너에서 설치할 수 있는 clang-format · clang-tidy · clazy 버전과, clazy가 Qt 6.11 헤더를 처리할 수 있는지
   - aqtinstall 3.4.0 이상이 릴리스됐는지 (됐으면 DEBT 항목 대신 바로 되돌린다)

## SageSDI에서 옮길 것
해당 없음. 이 주제는 SageQt의 도구 설정만 다룬다.

## 옮기지 않는 것
해당 없음.

## 함정
- **clang-format 주 버전을 로컬과 CI에서 같게 고정한다.** 주 버전이 다르면 같은 설정으로도 결과가 달라 CI만 실패한다. ubuntu apt의 기본 버전은 로컬(22)과 다를 수 있다 — 설치 방법을 착수 시 확인한다
- clang-tidy는 `compile_commands.json`이 필요하다 → CI configure에 `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`. `.gitignore`는 이미 이 파일을 무시한다
- clang-tidy · clazy는 **Linux job 하나에서만** 돌린다. 코드가 세 OS에서 같으므로 한 번이면 충분하고, MSVC 플래그가 든 `compile_commands.json`은 clang-tidy가 잘못 해석할 수 있다
- moc 생성 파일(`*_autogen`)은 검사 대상에서 뺀다 — 헤더 필터와 소스 목록으로 제한한다
- 네이밍: 인터페이스는 `ISage`, 나머지 클래스는 `Sage` 접두사다. clang-tidy의 추상 클래스 접두사 옵션으로 표현할 수 있는지 착수 시 옵션 이름을 확인한다. 표현할 수 없는 규칙은 규칙 문서(`format-and-tools.md`)에 "도구로 검사하지 않음"을 명시한다
- clazy의 `non-pod-global-static`은 끈다 (`format-and-tools.md` 규칙)
- 컴파일 경고 에러화는 CI configure에 `-DCMAKE_COMPILE_WARNING_AS_ERROR=ON`으로 준다. CMake 3.24 이상에서 동작한다 (러너는 3.31 이상). 로컬 최소 버전 3.22에서는 이 변수를 무시하므로 로컬 흐름은 그대로다
- 설정 파일(YAML · `.clang-format` · `.clang-tidy`)에도 주석을 쓰지 않는다. action 버전은 step 이름에 적는다 (기존 `build.yml` 방식)
- 외부 action은 커밋 해시로 고정한다

공통 도구 함정은 `sageqt-plan` SKILL.md의 *공통 함정*을 따른다.

## 작업
PR 1개: `chore/static-analysis`
- [ ] `.clang-format` — `format-and-tools.md`의 포맷 6개 규칙을 옮긴다
- [ ] 기존 소스(`SageQt/**`)에 clang-format을 돌려 변경이 0줄인지 확인한다. 변경이 생기면 포맷 커밋을 분리한다
- [ ] `.clang-tidy` — 네이밍 표, `auto` · `NULL` · C 스타일 캐스트 금지, `explicit` · `override` · `const`
- [ ] `build.yml`에 정적 분석 job 추가 (ubuntu-24.04): Qt 설치(기존 고정 action · aqtinstall 그대로) → configure(`linux-x64-debug` + compile commands) → clang-format 검사 → clang-tidy → clazy → 플랫폼 분기 검사
- [ ] 플랫폼 분기 검사: 소스에 `Q_OS_` · `_WIN32` · `__APPLE__` · `__linux__` · `<windows.h>`가 있으면 실패
- [ ] 빌드 job 6개의 configure에 경고 에러화 추가
- [ ] **음성 테스트**: 같은 PR에서 위반을 일부러 넣은 커밋 4개(포맷 · 네이밍 · 플랫폼 분기 · 컴파일 경고)로 해당 job이 실패하는지 확인하고, 각각 되돌린다. 실행 링크를 PR 본문에 남긴다
- [ ] (승인 시) `docs/DEBT_LOG.md` 신설 + aqtinstall 항목 — 별도 `docs:` 커밋
- [ ] 도구로 검사할 수 없는 규칙이 있으면 `format-and-tools.md`에 명시 — 별도 `docs:` 커밋, `sageqt-plan` 재점검

## 완료 기준
- `develop`에서 빌드 job 6개와 정적 분석 job이 모두 통과한다
- 음성 테스트 4개가 각각 해당 job을 실패시켰다 (실행 링크 4개)
- 현재 소스에 clang-format을 돌린 결과 변경 0줄
- 로컬과 CI의 clang-format 주 버전이 같다

## 범위 밖
- 로컬 Windows용 clazy 설치 — CI에서 한 번 돌리면 충분하다
- pre-commit 훅 — 요청되지 않았다

## 확인한 사실
(진행 중 기록)
