# T07 — `sageqt-ui` 스킬 작성

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 디자인 규칙(`sagesdi-ui`)과 T02 측정 결과를 바탕으로 SageQt의 UI 스킬을 만든다. 이 스킬이 디자인 값 목록(`SageDesignDefine.h`), `SageStyle`이 재정의할 범위, 위젯 변형, 레이아웃 정책을 정한다. T08 이후의 모든 화면 주제가 이 스킬을 따른다.

## 시작 전에
1. 선행 주제: T02 (레이아웃 정책의 근거)
2. 스킬 로드: `sageqt-plan`(스킬 구조 원칙), `coding-design` · `coding-rules`
   - 읽을 reference: `coding-design/references/style.md` · `model-view.md` · `ui-composition.md`, `coding-rules/references/values-and-platform.md` (디자인 값 위치)
3. 결정 — 분석 결과를 보고 사용자와 확정
   - 레이아웃 정책: 고정 픽셀 · 폰트 기반 치수 · 혼합 중 무엇으로 할지 (T02 수치 근거)
4. 재확인할 사실
   - T02 결과표 (`MIGRATION_PLAN.md` 확정 사실)

## SageSDI에서 옮길 것
- **디자인 규칙 원본**: `D:/Projects/SageSDI/.claude/skills/sagesdi-ui/SKILL.md` (915줄) — 레이아웃 · 색상 팔레트 · 커스텀 컨트롤 규격
- **상수 원본**: `D:/Projects/SageSDI/SageSDI/SageDefine.h` — 상수 477개, 509줄 (2026-09-23 기준). 이 중 색 상수 42개 (`SAGE_COLOR_*`)
- **폰트 역할과 크기**: T02 파일의 표 (0.1pt 단위 주의)
- **프레임리스 다이얼로그 규격**: 캡션 높이 40 · 좌우 여백 16 · 버튼 28 · 버튼 여백 8 (`SageDefine.h:112-115`), 로그인 창 폭 320 · 비밀번호 창 폭 360 · 메시지 상자 폭 360 (`:119, 421-422`)
- **결과 표 열 폭**: 항목 140 · 값 최소 220(늘어남) · 상태 110 · 사유 320 (`:221-224`)
- **커스텀 컨트롤 25종**: `D:/Projects/SageSDI/SageSDI/app/ui/drawing/` — 각각 "기본 위젯 + `SageStyle`로 됨 / 커스텀 위젯 필요 / 필요 없음"으로 분류한다 (`style.md`: 기본 위젯으로 되는 것은 서브클래싱하지 않는다)

**상수 분류** — 477개를 하나씩 다음 중 하나로 보낸다
| 분류 | 행선지 |
|---|---|
| 색 · 여백 · 크기 · 폰트 크기 | `SageDesignDefine.h` (이 스킬이 목록을 정한다) |
| UI 문자열 · 업무 상수 | `SageDefine.h` (쓰이는 것만, 해당 주제에서) |
| 컨트롤 ID (`ID_SAGE_*`) · 창 메시지 (`WM_SAGE_*`) · 타이머 ID | **옮기지 않음** — Qt는 signal · 객체 참조를 쓴다 |
| 호출 0곳인 상수 | **옮기지 않음** |

## 옮기지 않는 것
- `sagesdi-ui`의 "규칙이 생긴 이유" 사례 중 **SageSDI · SageTaechang의 삭제된 화면**을 근거로 한 것 (예: 「D7-6」 · 「D7-12」, SageSDI `docs/DEBT_LOG.md` 참조) — SageQt 사용자가 확인할 수 없는 근거다. 규칙 자체가 필요하면 근거를 SageQt 기준으로 다시 쓴다
- GDI 그리기 세부(`CDC` · `CBrush` · `OnCtlColor`) — `SageStyle`과 delegate로 대체된다

## 함정
- **`SageSDI.rc`는 UTF-16LE다.** grep이 아무것도 찾지 못한다. `iconv -f UTF-16LE -t UTF-8`로 변환해서 읽는다
- `SageDefine.h`는 UTF-8 **BOM 포함 · CRLF**다. 문자열을 복사해 올 때 SageQt 규칙(UTF-8 BOM 없음 · LF)으로 저장된다 — 편집기가 BOM을 따라 가져오지 않았는지 확인한다
- SageSDI의 픽셀 값은 Windows GDI 기준이다. SageSDI 프로젝트 파일에 DPI 인식 설정이 명시되어 있지 않다 (`SageSDI.vcxproj`, 2026-09-23). Qt의 논리 픽셀과 1:1로 대응하는지는 T02 · T08에서 화면을 보고 확인한다 — 추측으로 환산하지 않는다
- 스킬 크기 규칙: SKILL.md 300줄 이하, 세부는 `references/` (`sageqt-plan` 스킬 구조 원칙). 규칙을 다른 스킬과 중복하지 않는다
- 이 스킬을 만든 뒤 `coding-design/references/style.md`, `code-review-expert/references/checklists.md`의 G 항목, `coding-rules/references/values-and-platform.md`가 가리키는 "UI 스킬(`sageqt-ui`)"을 실제 경로로 바꾼다 → `sageqt-plan` 재점검

## 작업
PR 1개: `docs/sageqt-ui-skill`
- [ ] `sagesdi-ui` 분석 — 유지 · 변경 · 폐기 표
- [ ] 상수 477개 분류표
- [ ] 커스텀 컨트롤 25종 분류표
- [ ] 레이아웃 정책 결정 (T02 근거, 사용자 확정)
- [ ] `.claude/skills/sageqt-ui/` 작성 — 디자인 값 목록, `SageStyle` 재정의 범위, 위젯 변형(`Q_PROPERTY` enum) 목록, 화면별 규격
- [ ] 다른 스킬의 "UI 스킬" 참조를 실제 경로로 갱신, 계획 재점검

## 완료 기준
- `sageqt-ui` SKILL.md가 300줄 이하이고 references로 나뉘어 있다
- 상수 477개가 모두 분류표에 있다 (행 수 477)
- 커스텀 컨트롤 25종이 모두 분류표에 있다
- 다른 스킬에 "UI 스킬이 정한다" 같은 미정 표현이 0개다

## 범위 밖
- `SageDesignDefine.h` · `SageStyle` 코드 — T08

## 확인한 사실
(진행 중 기록)
