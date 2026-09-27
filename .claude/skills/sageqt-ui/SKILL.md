---
name: sageqt-ui
description: >
  SageQt(Qt 6 Widgets)의 UI 규격 skill. 디자인 값(색 · 여백 · 크기 · 폰트), 레이아웃 정책(고정 픽셀 · 최소값 · 레이아웃),
  폰트 지정 방식(픽셀 크기 · 패밀리 + 굵기), SageStyle이 재정의할 범위, 위젯 변형(Q_PROPERTY enum), 커스텀 위젯 · delegate 목록,
  화면별 규격을 정한다. SageSDI의 sagesdi-ui 규칙과 T02 폰트 측정을 근거로 만들었다.
  IMPORTANT: 화면 · 위젯 · 다이얼로그 · delegate · SageStyle · SageDesignDefine.h를 만들거나 고칠 때 반드시 이 skill을 먼저 호출한다.
  트리거 조건: "UI", "화면", "디자인", "색", "색상", "여백", "간격", "크기", "높이", "폭", "폰트", "글꼴", "SageStyle", "스타일",
  "팔레트", "QPalette", "SageDesignDefine", "버튼", "입력칸", "탭", "표", "헤더", "사이드바", "배지", "상태 카드", "다이얼로그",
  "메시지 상자", "delegate", "variant", "변형", "위젯", "패널", "레이아웃", "스크린샷" 등이 언급되면 이 skill을 트리거한다.
  CRITICAL: 구조 규칙(무엇을 어디에 · SageStyle 구조 · QSS 금지)은 coding-design/references/style.md, 값 위치 규칙은
  coding-rules/references/values-and-platform.md가 정한다. 이 skill은 그 위에서 규격(값 · 정책 · 목록)만 정한다.
---

# SageQt UI 규격

## 이 skill이 정하는 것 · 정하지 않는 것

| 무엇 | 어디 |
|---|---|
| 모양은 `SageStyle`이 그린다, QSS 금지, 화면은 그리지 않는다, 변형은 `Q_PROPERTY` | `coding-design/references/style.md` |
| 디자인 값은 `ui/style/SageDesignDefine.h` 한 곳 | `coding-rules/references/values-and-platform.md` |
| 표는 model · proxy · `QHeaderView` · delegate | `coding-design/references/model-view.md` |
| 패널 구조 · 레이아웃 · 좌표 금지 | `coding-design/references/ui-composition.md` |
| **디자인 값 목록, 레이아웃 정책, 폰트 지정, `SageStyle` 재정의 범위, 변형 목록, 화면별 규격** | **이 skill** |

규칙을 이 skill에 복사하지 않는다. 위 문서가 바뀌면 이 skill의 해당 절을 다시 본다.

## 근거

- SageSDI 디자인 규칙 `sagesdi-ui` (915줄) 분석 — 유지 133 · 변경 111 · 폐기 34 (`docs/decisions/sageqt-ui/sagesdi-ui-analysis.md`)
- SageSDI `SageDefine.h` 상수 477개 분류 (`docs/decisions/sageqt-ui/constants-classification.md`)
- SageSDI `app/ui/drawing/` 25종 분류 (`docs/decisions/sageqt-ui/controls-classification.md`)
- T02 폰트 · 텍스트 메트릭 3-OS 측정 (`docs/decisions/MIGRATION_PLAN.md` *폰트 · 텍스트 메트릭*)
- 결정 기록 (2026-09-27, T07): 레이아웃 정책 · 변형 방식 · 값 출처 · 미사용 경로 · 규격 없는 상태 · 값 없는 규격

---

## 값 출처 원칙

1. **SageSDI 코드가 기준이다.** `sagesdi-ui` 문서와 코드가 다르면 코드(실제 동작)를 따른다. 확인된 충돌은 아래와 같이 정했다
   | 항목 | 문서 | 코드 | SageQt |
   |---|---|---|---|
   | 입력칸 텍스트 위 여백 | 7 | 9 | 9 |
   | 제목 · 섹션 제목 서체 | SemiBold / Bold (문서 안에서도 충돌) | `SAGE_TITLE_FONT_FACE = "Pretendard SemiBold"` | SemiBold |
   | 아이콘 선 굵기 | 1.5px / 2px | 2px | 2px |
   | Primary 버튼 개수 | 카드당 1개 (근거 화면 삭제) / 화면당 1개 | — | **화면당 1개** |
   | 인라인 오류 자리 | 값이 비면 숨김 / 자리는 항상 비워 둠 | T10 착수 시 `SageLoginDlg` 코드로 확인 | 코드의 동작 |
2. **SageSDI에서 호출 0곳인 경로는 옮기지 않는다** — 버튼 아이콘 `SEARCH` · `ADD` · `MOVE_UP` · `MOVE_DOWN`, 빈 상태의 동작 버튼(`SetAction`), 섹션 힌트(`SetHintText`), 경고 인라인 메시지(enum 값 `SAGE_INLINE_WARNING`), `CSageComboBox` · `CSageListBox` · `CSageOptionCheck`, 숫자 형식 상수 (`widgets.md`)
3. **SageSDI에 규격이 없는 상태는 T08에서 정한다** — hover · focus · disabled (SageSDI는 hover가 없고 포커스 표시는 일부 버튼에만 있다). Fusion은 이것들을 그리므로, T08에서 세 OS 스크린샷을 보고 규격을 정해 이 skill에 적는다. 그 전에는 추측으로 값을 넣지 않는다
4. **SageSDI에 값이 없는 규격은 코드 값을 먼저 찾고, 없으면 T08에서 정한다** — 버튼 · 입력칸 · 카드 반경, 행 상태 색, 「관리자」 배지 색, 사이드바 선택 글자색, 검색 박스 면, 필 바 선택 상태 (`style-scope.md` *미정*)

## 레이아웃 정책 (CRITICAL)

**단위는 96 DPI 기준 논리 픽셀이다.** SageSDI는 DPI 인식 설정이 없는 앱이라 96 DPI로 그리고 Windows가 확대했다 (프로젝트 · 소스 · 리소스에 `dpiAware` 설정 0건, 2026-09-27). Qt 6의 논리 픽셀도 96 DPI 기준이므로 SageSDI 픽셀 값을 그대로 쓴다. 이 대응은 설정 부재에서 끌어낸 추론이다 — T08에서 화면으로 확인한다.

| 대상 | 방식 | 예 |
|---|---|---|
| 높이 · 여백 · 간격 · 반경 · 선 굵기 · 아이콘 | **고정** — `SageDesignDefine.h` 값 그대로 | 버튼 · 입력칸 32, 표 행 34, 헤더 56, 탭 줄 40, 상태 카드 70, 여백 16 |
| **글자가 들어가는 폭** | **최소값 + 레이아웃 · `QFontMetrics`** — 고정 폭으로 쓰지 않는다 | 폼 라벨 64 · 96, 배지 폭, 버튼 폭, 다이얼로그 폭, 상태 카드 액션 칸 80 |
| 표 열 폭 | `QHeaderView` 크기 조정 모드 + 최소 폭 (`model-view.md`) | 결과 표 항목 140 · 값 220(늘어남) · 상태 110 · 사유 320 |
| 긴 경로 · 문장 | `QFontMetrics::elidedText` 또는 view 기본 말줄임 | 상태 카드 저장 경로 |

- **폼 라벨**: 한 폼 안의 라벨 열은 그 폼의 가장 긴 라벨에 맞춰 같은 폭으로 정렬한다 (`QFormLayout` 또는 그리드). 64 · 96은 최소 폭이다. SageSDI의 96은 Windows GDI로 잰 「변경할 비밀번호」 88px에서 나온 값이라 다른 OS에서 맞는다는 보장이 없다
- **고정 높이 안의 글자**: T02 측정에서 픽셀 크기 지정 시 줄 높이 차이는 -1.5 ~ +4.4%(로고 제외)라 한 줄 텍스트는 고정 높이 안에 들어간다. 로고(Gmarket Sans)만 macOS에서 -13.7% — T11에서 확인
- 좌표 · 크기를 코드로 지정하지 않는다 — 고정 높이도 `setFixedHeight`가 아니라 위젯의 `sizeHint` · 레이아웃 제약으로 준다 (`ui-composition.md` 소유 규칙 3)
- 간격 값은 SageSDI 규칙대로 4의 배수만 쓴다

## 폰트 (CRITICAL)

**크기는 픽셀로 지정한다** (`QFont::setPixelSize`). px = SageSDI 0.1pt 값 ÷ 10 × 96 / 72, 반올림. 포인트로 지정하면 macOS(논리 DPI 72)에서 약 25% 작게 그려진다 (T02).

**서체는 패밀리 + 굵기로 찾는다.** GDI식 이름(`"Pretendard SemiBold"` · `"Gmarket Sans TTF Bold"`)은 macOS · Linux · Windows offscreen에서 다른 폰트로 잡힌다 (T02).

| 역할 | 패밀리 | 굵기 | 크기 | SageSDI 상수 |
|---|---|---|---|---|
| 본문 · 컨트롤 · 버튼 | `Pretendard` | Regular | 14px | `SAGE_CONTROL_FONT_POINT_SIZE` · `SAGE_CONTENT_FONT_POINT_SIZE` (105) |
| 화면 제목 | `Pretendard` | SemiBold (`QFont::DemiBold`) | 19px | `SAGE_TITLE_FONT_POINT_SIZE` (143) |
| 섹션 제목 · 헤더 | `Pretendard` | SemiBold | 15px | `SAGE_HEADER_FONT_POINT_SIZE` (113) |
| 표 셀 · 목록 | `Pretendard` | Regular (선택 · 강조는 SemiBold · Bold) | 13px | `SAGE_LIST_FONT_POINT_SIZE` (98) |
| 캡션 · 인라인 메시지 · 배지 | `Pretendard` | Regular | 12px | `SAGE_CAPTION_FONT_POINT_SIZE` (90) |
| 요약 수치 | `Pretendard` | SemiBold | 17px | `SAGE_SUMMARY_FONT_POINT_SIZE` (128) |
| 로고 | `Gmarket Sans TTF` | Bold | 19px | `SAGE_TITLE_FONT_POINT_SIZE` (143) + `SAGE_LOGO_FONT_FACE` |

- 굵기를 쓰는 곳: Primary · Danger 버튼 Bold, 선택 탭 Bold, 강조 열 Bold, 합계 밴드 라벨 Bold, 목록 선택 SemiBold
- **등록하는 폰트 파일은 쓰이는 4개다** — `PretendardRegular` · `PretendardSemiBold` · `PretendardBold` · `GmarketSansTTFBold`. `GmarketSansTTFLight` · `Medium`은 저장소에 있지만(T02) 쓰는 곳이 0곳이라 등록하지 않는다
- 앱 기본 폰트는 본문(14px Regular)이다. 역할 폰트를 누가 적용하는지는 `style-scope.md` *폰트 적용*

## 위젯 변형 (variant)

`coding-design/references/style.md`의 규칙("변형은 `Q_ENUM` enum 타입의 `Q_PROPERTY`, `SageStyle`이 읽어 그린다")을 따른다. Qt 기본 위젯에 변형 속성을 붙이려면 **변형 속성만 가진 얇은 서브클래스**를 만든다 — `paintEvent`를 두지 않고, 그리기는 계속 `SageStyle`이 한다 (결정 2026-09-27, `style.md`에 반영). 목록은 `widgets.md`.

---

## 상황별 요약

### 화면 · 위젯을 만들 때
1. 기본 위젯으로 되는지, 커스텀 위젯 · delegate가 필요한지 `widgets.md`에서 찾는다
2. 값은 `design-values.md`에서 찾는다. 없으면 만들지 말고 멈춘다 — 이 skill에 먼저 추가한다
3. 폭 · 높이는 *레이아웃 정책*대로 — 글자 폭은 고정하지 않는다
4. 폰트는 *폰트* 표의 역할로 지정한다
5. 화면별 규격은 `screens.md`
6. UI를 바꿨으면 세 OS 스크린샷을 `docs/screenshots/<주제 ID>/`에 남긴다 (`git-workflow`)

### SageStyle을 고칠 때
`style-scope.md`의 재정의 범위 · 팔레트 역할 · 미정 항목을 먼저 본다. 새 상태 규격을 정하면 이 skill의 *미정*에서 지우고 값을 적는다.

### 규격을 바꾸거나 지울 때
1. `design-values.md` · `screens.md`의 값을 고친다 — 코드보다 먼저
2. 그 값을 쓰는 `SageDesignDefine.h` 상수와 위젯 · 스타일을 함께 고친다 (`coding-rules` *코드를 수정할 때*)
3. 값을 지우면 `SageDesignDefine.h`에서도 지운다 (`coding-rules` *코드를 삭제할 때*)
4. 규칙(skill)을 고쳤으므로 `sageqt-plan` 절차 5로 진행 중인 주제를 다시 본다

---

## 상세 규격 파일

| 파일 | 읽을 때 | 담긴 것 |
|---|---|---|
| `references/design-values.md` | 값이 필요할 때, `SageDesignDefine.h`를 만들거나 고칠 때 | 색 41 · 폰트 10 · 여백 · 크기 161 — 이름 · 원문 값 · px |
| `references/style-scope.md` | `SageStyle` · 팔레트 · 앱 폰트를 만들거나 고칠 때 | 재정의할 위젯 · 요소, 팔레트 역할 대응, 폰트 적용, 미정 항목 |
| `references/widgets.md` | 위젯 · delegate를 만들 때 | SageSDI 컨트롤 25종의 SageQt 대응, 변형 enum 목록, 커스텀 위젯 · delegate, 이관 시 바뀌는 동작 |
| `references/screens.md` | 화면 · 다이얼로그를 만들 때 | 창 · 사이드바 · 헤더 · 탭 · 입력 · 상태 카드 · 표 · 다이얼로그 규격 |

## 하지 말 것

- 이 skill에 없는 값을 코드에 넣지 않는다 — 먼저 이 skill에 추가하고 근거를 적는다
- 글자가 들어가는 폭을 고정 픽셀로 두지 않는다
- 폰트를 포인트로 지정하거나 GDI식 이름으로 찾지 않는다
- 규격이 없는 상태(hover · focus · disabled)를 추측으로 채우지 않는다 — T08에서 화면을 보고 정한다
