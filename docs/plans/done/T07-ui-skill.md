# T07 — `sageqt-ui` 스킬 작성

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 디자인 규칙(`sagesdi-ui`)과 T02 측정 결과를 바탕으로 SageQt의 UI 스킬을 만든다. 이 스킬이 디자인 값 목록(`SageDesignDefine.h`), `SageStyle`이 재정의할 범위, 위젯 변형, 레이아웃 정책을 정한다. T08 이후의 모든 화면 주제가 이 스킬을 따른다.

## 시작 전에
1. 선행 주제: 없음 (T02 완료 — 레이아웃 정책의 근거는 `MIGRATION_PLAN.md` *폰트 · 텍스트 메트릭*과 `docs/plans/done/T02-font-metrics.md`)
2. 스킬 로드: `sageqt-plan`(스킬 구조 원칙), `coding-design` · `coding-rules`
   - 읽을 reference: `coding-design/references/style.md` · `model-view.md` · `ui-composition.md`, `coding-rules/references/values-and-platform.md` (디자인 값 위치)
3. 결정 — 분석 결과를 보고 사용자와 확정
   - 레이아웃 정책: 고정 픽셀 · 폰트 기반 치수 · 혼합 중 무엇으로 할지 (T02 수치 근거)
4. 재확인할 사실
   - T02 결과표 (`MIGRATION_PLAN.md` 확정 사실)

## SageSDI에서 옮길 것
- **디자인 규칙 원본**: `D:/Projects/SageSDI/.claude/skills/sagesdi-ui/SKILL.md` (915줄) — 레이아웃 · 색상 팔레트 · 커스텀 컨트롤 규격
- **상수 원본**: `D:/Projects/SageSDI/SageSDI/SageDefine.h` — 상수 477개, 509줄 (2026-09-23 기준). 이 중 색 상수 42개 (`SAGE_COLOR_*`)
- **폰트 역할과 크기**: `docs/plans/done/T02-font-metrics.md`의 표 (0.1pt 단위 주의). 3 OS 측정 결과는 `MIGRATION_PLAN.md` *폰트 · 텍스트 메트릭*
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
- [x] `sagesdi-ui` 분석 — 유지 · 변경 · 폐기 표
- [x] 상수 477개 분류표
- [x] 커스텀 컨트롤 25종 분류표
- [x] 레이아웃 정책 결정 (T02 근거, 사용자 확정)
- [x] `.claude/skills/sageqt-ui/` 작성 — 디자인 값 목록, `SageStyle` 재정의 범위, 위젯 변형(`Q_PROPERTY` enum) 목록, 화면별 규격
- [x] 다른 스킬의 "UI 스킬" 참조를 실제 경로로 갱신, 계획 재점검

## 완료 기준
- `sageqt-ui` SKILL.md가 300줄 이하이고 references로 나뉘어 있다
- 상수 477개가 모두 분류표에 있다 (행 수 477)
- 커스텀 컨트롤 25종이 모두 분류표에 있다
- 다른 스킬에 "UI 스킬이 정한다" 같은 미정 표현이 0개다

## 범위 밖
- `SageDesignDefine.h` · `SageStyle` 코드 — T08

## 확인한 사실
- 규모가 지시서와 같다: `sagesdi-ui` 915줄, `SageDefine.h` `constexpr` 477개(색 42), `drawing/` 25종 (2026-09-27, SageSDI `2a6c179`)
- 분석은 에이전트 세 개로 나눠 초안을 만들고, 표본을 원본과 대조해 확정했다 (값 · 줄 번호 · 사용 수). 분류표는 `docs/decisions/sageqt-ui/`
  - 상수: `SageDesignDefine.h` 212 · `SageDefine.h`(해당 주제) 117 · 이관됨 52 · 옮기지 않음 96
  - 컨트롤: 기본 위젯 + SageStyle 9 · 커스텀 위젯 9 · delegate 2 · 불필요 3 · 공용 자원 2
  - `sagesdi-ui`: 유지 133 · 변경 111 · 폐기 34, 근거가 삭제된 화면 14건
- SageSDI에는 DPI 인식 설정이 없다 (`dpiAware` 등 0건) → 96 DPI 논리 픽셀로 본다 (추론 — T08에서 화면 확인)
- SageSDI 문서의 px 폰트 크기와 코드의 0.1pt 값이 pt × 96 / 72로 정확히 맞는다 (105 → 14px, 143 → 19px, 98 → 13px 등) — 픽셀 지정 결정의 근거
- 문서 · 코드 충돌: `SAGE_EDIT_TEXT_TOP_PAD = 9`(문서 7), `SAGE_ICON_STROKE = 2`(문서 1.5 · 2), `SAGE_BUTTON_TEXT_TOP_OFFSET = 0`(문서 2). SageSDI 문서의 `SAGE_ICON_BUTTON_SIZE` · `SAGE_LABEL_EDIT_GAP`은 코드에 없다
- 표 선택 행의 첫 열 글자는 `SAGE_COLOR_TEXT` 그대로다 (`SageListCtrl.cpp:331`)
- 스킬에 인용한 `SAGE_` 이름을 디자인 값 목록 · 원본과 스크립트로 대조했다 — 잘못 인용한 1개(`SAGE_ICON_BUTTON_SIZE`)를 고쳤다

## 결과
- 작업 브랜치 CI 통과 후 `develop`에 squash merge
- `.claude/skills/sageqt-ui/` — SKILL.md 130줄 + references 4개 (`design-values.md`는 원본에서 스크립트로 생성: 색 41 · 폰트 10 · 여백 · 크기 161, 쓰는 방식 열 포함)
- 결정 6건 (`MIGRATION_PLAN.md` 결정 기록): 레이아웃 정책 · 변형은 얇은 서브클래스 · 값 출처는 코드 · 미사용 경로 제외 · 규격 없는 상태와 값은 T08
- 다른 스킬의 미정 표현 0개 (`style.md` · `values-and-platform.md` · `checklists.md`), `style.md`에 얇은 서브클래스 예외. T08 · T11 · T13 지시 반영
- 교훈
  - 에이전트가 만든 표는 표본을 원본과 대조해야 믿을 수 있다 — 상수표는 맞았지만, 에이전트의 "GDI 보정값 4개"는 비고로 확인되는 것이 2개뿐이었다
  - 값을 손으로 옮기지 않고 원본에서 생성하면 옮겨 적기 오류가 없다 — 대신 생성 규칙(쓰는 방식 분류)을 명시한다
