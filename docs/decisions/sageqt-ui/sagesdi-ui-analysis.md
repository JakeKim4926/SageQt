# T07 분석 — `sagesdi-ui/SKILL.md` → SageQt 대응

- 원본: `D:/Projects/SageSDI/.claude/skills/sagesdi-ui/SKILL.md` (916줄, 전부 읽음)
- 판정 기준: SageQt `coding-design/references/style.md` · `ui-composition.md` · `model-view.md`, `coding-rules/references/values-and-platform.md`, `docs/decisions/MIGRATION_PLAN.md` *폰트 · 텍스트 메트릭 (T02)*
- 「근거가 삭제된 화면」 판정에 쓴 사실은 SageSDI 현재 코드에서 확인했다 (아래 *0. 현재 SageSDI에 있는 화면*). 스킬 본문 외 값은 모두 「코드 대조」로 표시했다
- 판정 표기: **유지** / **변경** / **폐기**. 「SageQt 대응」 열의 `SageDesignDefine.h`는 `ui/style/SageDesignDefine.h`

---

## 0. 현재 SageSDI에 있는 화면 (근거 판정용, 코드 확인)

| 구분 | 있는 것 | 확인 위치 |
|---|---|---|
| 다이얼로그 | `SageLoginDlg` · `SagePasswordChangeDlg` · `SageMessageBoxDlg` (+ 기반 `SageFramelessDialog` · `SageDialogSizer`). **3종** | `D:/Projects/SageSDI/SageSDI/app/ui/dialogs/` |
| 패널 | Header · Sidebar · Workspace · WorkflowInput · WorkflowResult · WorkflowHistory · ResultTable | `app/ui/panels/` |
| 화면에서 쓰는 커스텀 컨트롤 | `CSageBadge`(Header) · `CSageSelectionBar` · `CSageSearchBox` · `CSageSummaryBar` · `CSageTableTotalBar` · `CSageSectionLabel`(ResultTable) · `CSageFilterPillBar` · `CSageEmptyState`(History) · `CSageStatusCard` · `CSageSectionLabel`(Input) · `CSageEdit` · `CSageInlineError`(로그인 · 비밀번호 변경) | `panels/*.h`, `dialogs/*.h` grep |
| 화면에서 안 쓰는 컨트롤 | `CSageListBox`(목록 선택 다이얼로그용 — 그 다이얼로그 없음) · `CSageFilterComboBox` · `CSageComboBox` · 버튼 아이콘 `MOVE_UP`/`MOVE_DOWN`/`ADD` | 위 grep에서 사용처 0 |
| 없는 화면 | 스킬이 근거로 드는 **목록 선택 다이얼로그**, **데이터 관리 표**(D7-6, 목업 3-6 「항목 추가 / 저장」 두 카드), **다이얼로그 7종**(현재 3종), 「패널·View 16곳」의 `DrawEditBorder`(현재 `SageWorkflowInputPanel` 2곳) | 위 목록 |

---

## 1. 절별 판정

### 1.1 목적 (8-22)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 10 | ERP형 데스크톱 업무 도구로 설계 | **유지** | 제품 성격은 같다 | 불필요 (방향) |
| 12-20 | 적용 범위: MFC 화면 구조, 사이드메뉴 · 탭, 문서 생성, PDF/HWP 표지 검수, 결과 테이블 · 상세 로그, 색 · 폰트 · 여백 · 상태, 접근성 | **변경** | 「MFC 화면 구조」→ Qt 화면 구성. PDF/HWP 검수 화면은 SageSDI에 현재 없음(핸들러 0개, MIGRATION_PLAN 23줄) — 범위 문구로만 남음 | `sageqt-ui` 스킬 적용 범위 |
| 22 | 마케팅 · 랜딩 · 장식적 SaaS 대시보드처럼 디자인하지 않는다 | **유지** | 목표 2(디자인 자유도)와 충돌하지 않는 톤 규칙 | 불필요 (방향) |

### 1.2 핵심 방향 (26-35)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 30 | 업무 탐색이 명확 | **유지** | 플랫폼 무관 | 불필요 |
| 31 | 파일 선택 → 실행 → 결과 확인 → 저장 흐름이 빠르게 반복 | **유지** | 플랫폼 무관 | 패널 구성(`ui-composition.md`) |
| 32 | 결과 테이블이 가장 먼저 읽힘 | **유지** | 플랫폼 무관 | layout (stretch 배분) |
| 33 | 상태 변화 예측 가능 | **유지** | | 불필요 |
| 34 | 장식보다 업무 밀도 · 정돈감 | **유지** | | 불필요 |
| 35 | 밝은 ERP 톤 | **유지** | 팔레트(1.7)로 실현 | QPalette |

### 1.3 제품 사용 흐름 (39-50)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 43-48 | 1 업무 선택 · 2 입력 파일 · 3 출력 폴더/옵션 · 4 미리보기 · 생성 · 검수 · 5 결과 확인 · 6 저장 | **유지** | 업무 흐름 | 패널 구성 |
| 50 | 흐름을 설명문 없이 구조만으로 드러낸다 | **유지** | | layout |

### 1.4 기본 레이아웃 (54-77)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 56 | 좌측 고정 사이드바 + 우측 작업 영역 | **유지** | 구조 | layout (`SageMainWindow` 최상위 `QHBoxLayout`) |
| 58-65 | Top/Header · Sidebar · Workspace Header · Tabs · Task Body · Result Area | **유지** | `ui-composition.md` 화면 클래스 구성과 일치 | 패널: Header · Sidebar · Workspace(+탭) · Input · Result · History |
| 69-73 | 사이드바 그룹: 업무 그룹(의뢰처/업무 성격) + 기타 > 비밀번호 변경 | **유지** | 비밀번호 변경 화면은 SageSDI에 있음 · SageQt 이관 대상(T10) | 핸들러 등록부가 사이드바 분류 답함 |
| 75 | 베이스에는 `샘플 > 샘플 업무` 하나 | **유지** | 핸들러 1개(샘플) 사실과 일치 | 핸들러 |
| 77 | 탭은 선택 업무 안 보조 단계, 사이드바 대체 안 함 | **유지** | | QTabBar + QStackedWidget |

### 1.5 탭 구성 (81-91)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 83 | 탭 구성은 `ISageWorkflowHandler::GetTab`이 답한다 | **변경** | 의도 유지, 이름은 SageQt 핸들러 API로 | 워크플로 핸들러(`core/workflow/`) |
| 85-87 | 기본: 입력 · 결과 · 실행 기록 | **유지** | | 패널 3개 |
| 89 | 탭은 선택 업무 안 보조 단계 | **유지** | | |
| 91 | 탭 전환 시 입력 · 결과 · 상세 · 내보내기 영역 가시성이 함께 바뀜 | **변경** | SageQt는 `setVisible` 행렬 금지, 탭 = 패널 교체 | `QStackedWidget` 현재 패널 교체 (`ui-composition.md` 72-81) |

### 1.6 업무 화면 패턴 (95-115)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 97-104 | 문서 생성 화면: 입력 파일 · 저장 위치 · 미리보기 · 생성 실행 · 결과 표 · 상세 | **유지** | 구성 요소 규칙 | 패널 |
| 106-113 | 검수 화면: 다중 파일 · 파일 목록/경로 · 검수 실행 · 결과 표 · 상세 로그 · CSV 저장 | **유지** | 현재 검수 업무는 없음(핸들러 0) — 향후 업무용 규칙이라 유지 | 패널 |
| 115 | 검수 업무에서는 출력 폴더 · 미리보기를 기본 노출하지 않음 | **유지** | 업무 분기는 핸들러가 답함 | 핸들러 「출력 폴더가 필요한지」 |

### 1.7 색상 시스템 (119-194)

#### SURFACE / TEXT (123-151)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 121 | 팔레트는 고정, 설계 판단은 역할 지키기 | **유지** | 목표 3(일관성) | `SageDesignDefine.h` 색 상수 + QPalette 역할 매핑 |
| 127 | 앱 배경 `#F8F6F1` | **유지** | 색값 | QPalette `Window` 후보 · `SageDesignDefine.h` |
| 128 | 패널 `#FFFFFF` | **유지** | | QPalette `Base` 후보 · `SageDesignDefine.h` |
| 129 | 표 헤더 · 툴바 · 빈 상태 아이콘 박스 `#F2EEE7` | **유지** | | SageStyle(`CE_HeaderSection`) · `SageDesignDefine.h` |
| 130 | 사이드바 `#241F1A` | **유지** | | 사이드바 위젯/delegate가 읽는 `SageDesignDefine.h` 값 (QPalette 전역 역할 아님) |
| 131 | 테두리 `#DCD6CD` | **유지** | | SageStyle (프레임 그리기) |
| 132 | 표 그리드선 `#EDE8E0` | **유지** | | delegate / SageStyle |
| 133 | 본문 텍스트 `#2F2A24` | **유지** | | QPalette `WindowText` · `Text` · `ButtonText` |
| 134 | 보조 텍스트 `#7A7064` | **유지** | | `SageDesignDefine.h` (QPalette `PlaceholderText`와는 별개 — 1.14 참조) |
| 135 | 표 헤더 텍스트 · 폼 라벨 `#6E655B` | **유지** | | SageStyle(헤더) · 라벨 variant |
| 136 | 버튼 중성 테두리 `#C9BFB1` | **유지** | | SageStyle (버튼 Secondary) |
| 137 | 삭제 버튼 테두리 `#E0BDB6` | **유지** | | SageStyle (버튼 Danger) |
| 138 | 빈 값 표시 `#B4ABA0` | **유지** | | delegate · QPalette `PlaceholderText` 후보 |
| 139 | 표 · 목록 선택 행 `#F1E3CD` | **유지** | | QPalette `Highlight` 후보 · delegate |
| 140 | 인라인 오류 텍스트 `#9C4433` | **유지** | | 인라인 메시지 위젯 variant |
| 141 | 인라인 경고 텍스트 `#8A6A32` | **유지** | | 같음 |
| 142 | 인라인 경고 박스 `#FBF5EE` / `#EBDCC6` | **유지** | | 같음 |
| 143 | 강조 면(합계 박스 · 필터 pill) `#F7F2EA` | **유지** | | 필 바 · 합계 박스 위젯 |
| 145-147 | 표 하단 합계 밴드 = `#F2EEE7`(LIST_HEADER), 카드 안 합계 박스 = `#F7F2EA`(ACCENT_SURFACE). 같은 「합계」라도 색이 다름 | **변경** | 규칙 자체는 유지할 가치. 단 근거인 **목업 3-2 카드 안 합계 박스**는 현재 SageSDI 화면에 없음(카드 합계 박스 컨트롤 없음, 표 합계 밴드 `CSageTableTotalBar`만 있음) → **근거가 삭제된 화면**(3-2 쪽). SageQt 기준으로는 「표 합계 밴드 = 헤더 위계」만 현재 필요 | 합계 밴드 위젯이 `SageDesignDefine.h` 값 사용 |
| 149-151 | 인라인 메시지 글자는 배지 색보다 어둡게. `ERROR #B85C4A` · `WARNING #B88746`는 아이콘 · 에디트 테두리에만, 12px 글자엔 어두운 짝 | **변경** | 의도 유지. 「12px」은 캡션 크기 — T02에 따라 픽셀 크기 지정으로 옮김 | 인라인 메시지 위젯 · SageStyle(에디트 오류 테두리) |

#### ACCENT (153-161)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 157 | 주요 액션 카멜 `#9A6B3F` | **유지** | | SageStyle 버튼 Primary · QPalette `Accent`(Qt 6.6+) 후보 |
| 158 | 카멜 press `#76502A` | **유지** | | SageStyle (`State_Sunken`) |
| 159 | 성공 세이지 `#5F7F5F` | **유지** | | `SageDesignDefine.h` |
| 160 | 경고 앰버 브라운 `#B88746` | **유지** | | 같음 |
| 161 | 오류 뮤트 레드 `#B85C4A` | **유지** | | 같음 |

#### 사용량 규칙 (163-194)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 165 | 갈색은 강조에만, 넓은 면 채우지 않음 | **유지** | | 불필요 (설계 규칙) |
| 167-171 | 채워진 카멜/브라운 면은 셋뿐: Primary 버튼(카드마다 1개), 사이드바 선택(`#3A3129` + 좌측 3px 카멜 바), 표 선택 행 | **변경** | 셋 목록은 유지. 「카드마다 1개」의 근거는 삭제된 화면(목업 3-6, 594줄) — 1.12 *버튼 위계 배분* 참조 | variant 규칙 · delegate |
| 173 | 표 헤더를 짙은 갈색으로 채우지 않음 · 금액 컬럼 배경색 없음 | **유지** | | SageStyle 헤더 · delegate |
| 174 | 버튼 4개가 동시에 갈색이면 강조 상실 | **유지** | | 설계 규칙 |
| 176 | 화이트 · 아이보리를 넓게, 흰 면적 유지 | **유지** | | QPalette |
| 178-184 | `#6E655B`(표 안 고정 텍스트 · 폼 라벨 · 합계 밴드 「합계」 · Ghost 버튼) vs `#7A7064`(요약 바 라벨 · 단위, 합계 밴드 「24건」, 선택 바 문구, 빈 상태 설명, 패널 헤더 건수) — 역할이 다르면 한 줄 안에서 함께 씀 | **유지** | 역할 매핑 | `SageDesignDefine.h` 두 상수 + 위젯별 사용 |
| 186-187 | 요약 바 라벨은 `#7A7064`, 합계 밴드 「합계」는 `#6E655B`. 새 요소는 목업 실측 | **유지** | 요약 바 · 합계 밴드 모두 SageSDI에 있음 | 같음 |
| 189-191 | 두 색 채널 차이 약 12 — 색으로 위계 못 만듦, 굵기(합계 밴드: 라벨 Bold · 보조 수치 SemiBold)나 크기로 | **유지** | | 폰트 굵기 (QFont::Weight) |
| 193-194 | 이력: D7-1 2단계에서 목업 3-1 합계 행 확인 → 역할 구분으로 바꿈 | **폐기** | 이력 메모. 규칙 본문(178-191)에 반영됨. 목업 3-1(결과 표)은 현재 화면에 있음 — 근거는 살아 있음 | 불필요 |

### 1.8 폰트 (198-237)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 200 | 본문 서체 Pretendard, Gmarket Sans는 로고만 | **유지** | | 앱 폰트(main.cpp 1회) + 로고 위젯 |
| 202-203 | Gmarket Sans는 숫자 폭 불균일 → 본문에서 제외 | **유지** | | |
| 207 | 본문 · 라벨 · 버튼 · 표 = Pretendard (`SAGE_CONTROL_FONT_FACE`) | **변경** | T02: 패밀리 `Pretendard` + 굵기로 지정 | `SageDesignDefine.h` 패밀리 상수 + `QFont::Weight` |
| 208 | 제목 · 섹션 제목 · 요약 수치 = Pretendard SemiBold (`SAGE_TITLE_FONT_FACE`) | **변경** | T02: `"Pretendard SemiBold"` 이름은 Windows offscreen에서 Gmarket Sans TTF Light, macOS에서 .AppleSystemUIFont로 잡힘 → `Pretendard` + `QFont::DemiBold` | 같음 |
| 209 | 로고 = Gmarket Sans TTF Bold (`SAGE_LOGO_FONT_FACE`) | **변경** | T02: 이름 `"Gmarket Sans TTF Bold"`는 macOS · Linux · Win offscreen에서 틀림 → `Gmarket Sans TTF` + `QFont::Bold` | 같음 |
| 211-214 | TTF 동봉 + `AddFontResourceEx(..., FR_PRIVATE, 0)`로 로드. `CreatePointFont`는 없는 서체면 시스템 기본으로 조용히 떨어짐 | **변경** | 동봉은 유지, 로드 수단은 Win32 → Qt | `QFontDatabase::addApplicationFont` (Qt 리소스), 등록 결과 확인 |

#### 타입 스케일 (216-237)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 220 | 화면 제목 19px / Bold, `CreatePointFont` `143`, `SAGE_TITLE_FONT_POINT_SIZE` | **변경** | 위계 · px 값 유지, 포인트 지정은 macOS에서 약 25% 작아짐(T02) → 픽셀 지정 | `SageDesignDefine.h` 픽셀 크기 + `QFont::setPixelSize` |
| 221 | 섹션 제목 15px / Bold, `113`, `SAGE_HEADER_FONT_POINT_SIZE` | **변경** | 같음 | 같음 |
| 222 | 본문 · 라벨 · 버튼 14px, `105`, `SAGE_CONTENT_FONT_POINT_SIZE` · `SAGE_CONTROL_FONT_POINT_SIZE` | **변경** | 같음 | 앱 폰트 |
| 223 | 표 셀 13px, `98`, `SAGE_LIST_FONT_POINT_SIZE` | **변경** | 같음 | 표 view/delegate 폰트 (SageStyle 또는 view 폴리시 — 확인 필요, 7장) |
| 224 | 캡션 · 인라인 메시지 12px, `90`, `SAGE_CAPTION_FONT_POINT_SIZE` (D5a 도입) | **변경** | 같음 | 캡션 폰트 |
| 225 | 요약 수치 17px / SemiBold, `128`, 미도입 — D5c | **변경** | 「미도입」 상태가 현재도 맞는지 **확인 못함**(SummaryBar는 D5c 구현됨, 526줄) | 같음 |
| 227-229 | 이 값은 목업(18/14/13/12/11px)보다 한 단계 큼 — 의도된 이탈(기존 앱 14.7px 대비, `DESIGN_PLAN` R1). 목업 px를 그대로 상수로 옮기지 않음 | **유지** | 결정 이력으로 유효 | `SageDesignDefine.h` 값은 이 표 기준 |
| 231 | 본문 · 컨트롤 폰트가 같은 값(`105`)이지만 상수 둘 | **변경** | SageQt는 앱 폰트 1개를 main.cpp에서 적용 — 둘로 나눌지 결정 필요(7장) | 앱 폰트 |
| 233 | 표 셀 · 캡션은 본문과 분리된 폰트 | **유지** | 역할 분리 | 역할별 폰트 상수 |
| 235 | 과도한 대형 타이포 금지 | **유지** | | |

### 1.9 버튼과 상태 (239-261)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 241-247 | 버튼명은 업무 행동: 미리보기 · 실행 · 저장 · CSV 저장 · 설정 저장 | **유지** | | `SageDefine.h` `SAGE_UI_` 문자열 |
| 249 | 실행 버튼 라벨은 `ISageWorkflowHandler::GetActionButtonLabel` | **변경** | 이름만 SageQt 핸들러 API로 | 핸들러 |
| 251 | 필수 입력 없으면 실행 버튼 비활성 | **유지** | | `setEnabled` (패널) |
| 253 | 처리 중엔 관련 업무 컨트롤만 비활성, 화면 맥락 유지 | **유지** | | 패널이 컨트롤러 signal 받아 처리 |
| 255-261 | 상태는 색만으로 표현 금지, 텍스트 동반: 성공 · 주의 · 실패 · 처리 중 · 대기 중 | **유지** | 접근성 | 문자열 상수 + 배지/상태 카드 |

### 1.10 결과 테이블 (265-377)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 267 | 결과 테이블이 주요 출력 영역 | **유지** | | layout stretch |
| 269-274 | 문서 생성 기본 컬럼: 항목 · 값 · 상태 · 사유 | **유지** | | 핸들러 결과 컬럼 정의 |
| 276-282 | 검수 기본 컬럼: 파일명 · 항목 · 값 · 상태 · 사유 | **유지** | | 같음 |
| 284 | 긴 경로 · 값은 말줄임, 상세/선택으로 확인 가능 | **유지** | | `QTableView::setTextElideMode` (기본 `ElideRight`) + 상세 영역 · 툴팁 |
| 286 | JSON · 상세 로그 · 긴 오류는 표 뒤 상세 영역 | **유지** | | 패널 |

#### 표 렌더링 규격 (288-326)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 290 | `CSageListCtrl` 쓰는 모든 표가 규격 공유 | **변경** | 공유 대상이 model/view + 공용 delegate | `QTableView` + `Sage*Delegate` + SageStyle |
| 294 | 행 높이 34 고정, `CListCtrl`은 못 정하므로 1×34 더미 이미지리스트 | **변경** | 34는 유지, 더미 이미지리스트 트릭은 폐기. 단 13px 텍스트를 34 고정 박스에 넣는 것이므로 텍스트 높이 의존(5장) | `QHeaderView::setDefaultSectionSize` (vertical, `Fixed`) + `SageDesignDefine.h` |
| 295 | 가로 hairline 1px(`#EDE8E0`)만, 세로선 없음 | **변경** | `setShowGrid(false)` + delegate가 하단 1px | delegate |
| 296 | 헤더 배경 `#F2EEE7`, 텍스트 `#6E655B`, 세로 구분선 없음 | **변경** | `CSageHeaderCtrl::OnPaint` → SageStyle | SageStyle (`CE_HeaderSection` · `CE_HeaderLabel`) |
| 297 | 헤더 정렬 항상 가운데 | **변경** | | SageStyle 또는 `QHeaderView::setDefaultAlignment(Qt::AlignCenter)` |
| 298 | 교대 행 유지 (`SAGE_COLOR_LIST_ROW_ALT`) | **변경** | 값이 스킬에 없음(코드 대조: `RGB(250, 248, 244)` = `#FAF8F4`, `SageDefine.h:270`) | `setAlternatingRowColors(true)` + QPalette `AlternateBase` |
| 299 | 셀 정렬 모든 열 가운데 | **변경** | | model `Qt::TextAlignmentRole` = `Qt::AlignCenter` (`model-view.md`) |
| 300 | 금액 컬럼 배경색 없음, Bold는 강조 열에만 | **유지** | | delegate |
| 301 | 강조는 한 열만 색 텍스트 + Bold, 배경칠 안 함 | **변경** | 색은 `SetHighlightColumns(5, 3)`(472줄)처럼 컨트롤 인자였음 → role/열 정의 | model `ForegroundRole` · `FontRole` 또는 delegate 속성 |
| 302 | 선택 행 `#F1E3CD` + 좌측 4px 카멜 바(`SAGE_LIST_SELECTION_ACCENT_WIDTH`). 목록(`CSageListBox`)은 + 텍스트 `#76502A` SemiBold. 사이드바 바는 3px 별 상수 | **변경** | 표 부분은 delegate로. **`CSageListBox` 부분은 근거가 삭제된 화면**(목록 선택 다이얼로그 없음) — 목록이 생기면 다시 정함 | delegate (`State_Selected`) · `SageDesignDefine.h` 4 / 3 |
| 303 | 빈 금액 `—`(`SAGE_UI_AMOUNT_EMPTY_MARK`) + `#B4ABA0` | **변경** | 문자는 `SAGE_UI_` 상수, 색은 role | model `DisplayRole` = `—` + `ForegroundRole`, 또는 delegate |
| 304 | 반복 값 축약 안 함 | **유지** | | model |
| 305 | 빈 행(`-`) 그대로 표시 — 고객사 데이터 포맷 구분 표기 | **유지** | | model |
| 306 | 그룹 구분 표시 안 함 | **유지** | | |
| 307 | 상태 배지 `SetBadgeColumn(n)` — pill(radius 4 · padding 0 8 · 높이 20 · 캡션 폰트) | **변경** | 치수 유지. 「padding 0 8」은 텍스트 폭 + 8×2 → pill 폭은 `QFontMetrics`로 | 배지 delegate + `SageDesignDefine.h` |
| 308 | 행 상태 색 `SetRowStyle(nState, ...)` — 행 배경 · 배지 색, 교대 행보다 우선 | **변경** | | model `BackgroundRole` / 상태 role + delegate |
| 310-312 | 행 높이 화면마다 다르게 안 함. 목업 3-5(실행 기록)는 40, 3-1은 34 → 34. 규격이 목업을 이김. 셀 2줄이 필요하면 열로 뺌(실패 사유 → 「사유」 열, D7-5) | **유지** | 실행 기록 · 결과 표 둘 다 현재 있음. D7-5는 결과 열 추가 결정 — 근거 화면 존재 | vertical header 고정 크기 |
| 314-315 | 컨트롤은 행 상태 의미를 모름. `SetItemData`에 상태 번호, 번호에 색 등록, 판정은 화면 | **변경** | 원칙 유지, 수단 변경 | model 커스텀 role(상태 번호) + delegate의 번호→색 표 |
| 317-320 | 컬럼 폭 배분은 `SageWorkflowResultTable::DistributeColumnWidths`(core) 하나. 고정 열은 정의 폭, stretch 열은 정의 폭 비율. 2026-08-07까지 버그(D7-5) | **변경** | SageQt `model-view.md`: core 열 정의에 픽셀 금지, 열 폭은 `QHeaderView` 모드. **비율 stretch**(stretch 열 둘 이상일 때 정의 폭 비율)는 `QHeaderView::Stretch`가 균등 분배라 그대로 안 됨 — 결정 필요(7장) | `QHeaderView` (`Stretch` · `ResizeToContents` · `Fixed`) |
| 322-323 | `LVS_EX_GRIDLINES` 쓰지 않음 (가로·세로 묶음) | **폐기** | Win32 전용 | `setShowGrid(false)` (1.10 295줄 행) |
| 325-326 | 빈 격자 채우지 않음, 데이터 없으면 `CSageEmptyState` | **변경** | | 빈 상태 커스텀 위젯 + `QStackedWidget`(표 ↔ 빈 상태) |

#### 정렬 — 목업에서 의도적으로 벗어난 지점 (328-345)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 330-331 | 헤더 · 셀 전부 가운데. 목업(숫자 우측 · 텍스트 좌측) 안 따름(사용자 결정) | **유지** | 사용자 결정 | `TextAlignmentRole` · 헤더 정렬 |
| 335 | 헤더: `CSageHeaderCtrl::OnPaint`가 `HDF_*` 무시, 항상 `DT_CENTER` | **변경** | | SageStyle / `setDefaultAlignment` |
| 336 | 셀(일반 열): `SageColumnAlign` 전부 `SAGE_COLUMN_ALIGN_CENTER` → `LVCFMT_CENTER` | **변경** | | core 열 정의의 정렬 의미 → `TextAlignmentRole` |
| 337 | 셀 0번 열: Win32가 좌측 고정 → `SetFirstColumnAlign(SAGE_LIST_FIRST_COLUMN_CENTER)` 필수 | **폐기** | Win32 전용 결함. Qt에는 0번 열 제약 없음 | 불필요 |
| 338 | 합계 바 `CSageTableTotalBar`가 같은 정렬 | **변경** | 합계 바가 표 열과 정렬을 맞추려면 열 경계를 알아야 함 — 구현 방식 결정 필요(7장) | 합계 바 위젯(`QHeaderView::sectionPosition/Size` 추종) 또는 표 마지막 행 — 확인 못함 |
| 340-343 | 이력: 2026-08-08까지 셀은 목업대로. D7에서 표가 넓어지자 전부 가운데. 대가는 금액 자릿수 비교 어려움 | **유지** | 결정 근거(결과 표는 존재). 대가 기록은 유효 | 불필요 |
| 345 | 「목업과 다르다」며 되돌리지 않음 | **유지** | | |

#### 반복 값을 축약하지 않는다 (347-372)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 349-351 | 연속 같은 값도 그대로. 행 표시가 앞 행에 의존하면 안 됨 | **유지** | proxy 필터 · 정렬과 맞는 원칙 | model / proxy |
| 353-355 | 축약 비용: 필터 · 정렬 · 복사 | **유지** | `QSortFilterProxyModel` 구조에서 더 강하게 성립 | |
| 357-358 | 그룹 경계 표시 안 함 — 정렬 · 필터로 답함 | **유지** | | |
| 360-365 | 이력: D4c `〃` · 그룹 시작 SemiBold · `SetGroupColumn` 제거 | **폐기** | 사라진 구현의 이력. 규칙은 349-358에 있음 | 불필요 |
| 367-368 | 빈 금액 `—`는 행 하나로 결정 → 프리젠터가 넣음. 기준: 그 값을 정하는 데 다른 행이 필요한가 | **변경** | 프리젠터 → model 변환 | model `data()` |
| 370-372 | `DrawFirstColumn`처럼 커스텀드로우에서 폰트·색 바꾸면 자기가 복원. `nHighlightCount = 0`이면 새어 나감 | **폐기** | GDI DC 상태 누수 문제. Qt는 `QPainter::save/restore` 관례로 대체 — 규칙 필요성 낮음 | delegate `paint`에서 `painter->save()/restore()` (일반 관례) |

#### 결과 요약 (374-377)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 376 | 표 상단 `CSageSummaryBar`로 결과를 수치로 먼저(총 건수 · 합계 · 예외 건수) | **변경** | 의도 유지, 커스텀 위젯(Qt에 없음) | 요약 바 커스텀 위젯 |
| 377 | 하단 상태바는 진행 · 경로 전용, 「완료」 한 단어로 결과 알리지 않음 | **유지** | | `QStatusBar` |

### 1.11 접근성 (381-412)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 383 | 텍스트 · 배경 대비 충분 | **유지** | | QPalette |
| 385-392 | 키보드 순서: 사이드바 → 탭 → 입력 파일 → 실행 → 결과 표 → 상세 | **변경** | 의도 유지. 패널 단위라 창 전체 순서는 `setTabOrder`를 패널 간에 걸 수 없음(다른 부모) — 레이아웃 순서/`setTabOrder` 조합 결정 필요 | layout 추가 순서 + `QWidget::setTabOrder` (확인 못함: 패널 경계 처리) |
| 394 | 사이드바 항목 · 탭 · 버튼 · 표 행에 포커스/선택 상태 명확 | **변경** | SageSDI 스킬에 **포커스 모양 규격이 없음** | SageStyle (`PE_FrameFocusRect`, `State_HasFocus`) — 값은 7장 질문 |

#### 아이콘 (396-412)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 400 | 아이콘 세트 규격 16px · 선 굵기 1.5px, 버튼 32×32 정사각 | **변경** | **776-777줄(「세트 전체 2px」)과 모순** — 7장. 32×32는 1.14와 같음 | `SageDesignDefine.h` + 아이콘 그리기 조각(`ui/style/`) |
| 401 | 툴팁 필수 | **유지** | | `setToolTip` |
| 402 | 세트에 정의된 것만, `SageButtonIcon` 값이 곧 세트 | **변경** | SageQt enum으로 | `Q_ENUM` 아이콘 enum (`Q_PROPERTY` 후보) |
| 404-408 | 이력: 6종 → D7-6이 목업 3-6 `↑ ↓` 넣어 8종(`MOVE_UP` · `MOVE_DOWN`), 실제는 `CALCULATE` · `CLOSE` 있고 「더보기」 없음 | **폐기** | **근거가 삭제된 화면**(목업 3-6 데이터 관리, D7-6). 코드 대조: 현재 enum은 `NONE · SEARCH · RESET · ADD · CLOSE · MOVE_UP · MOVE_DOWN`, 화면 사용은 SEARCH·RESET·CLOSE 쪽만(ADD · MOVE_* 사용처 0 — grep). 규칙(「헤더를 가리킨다」 · 「목업에 있는지 먼저 확인」)은 유지할 가치 있음 → SageQt에서는 **실제 쓰는 아이콘만** 옮김 | `Q_ENUM` 아이콘 목록 |
| 410-412 | 유니코드 글리프를 아이콘으로 안 씀(`↺` · `…`), 드로잉 또는 리소스 | **유지** | T02 결과(서체별 차이)와도 일치 | 그리기 조각 또는 `QIcon`(SVG 리소스) — 방식 결정 필요 |

### 1.12 MFC 공통 컨트롤 규격 (416-643)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 418-419 | 기본 컨트롤을 화면마다 스타일링하지 않음, `app/ui/drawing/` 공통 컨트롤로 감쌈 | **변경** | SageQt는 **기본 위젯 서브클래싱 금지** — 모양은 SageStyle. 감싸는 건 Qt에 없는 요소만 | SageStyle + (없는 것만) 커스텀 위젯 |
| 423-432 | 새 화면은 선언 · 생성 · 변형 지정뿐, 그리기 코드 화면에 없음 (`CSageButton` + `SetVariant(SAGE_BUTTON_PRIMARY)`) | **변경** | 의도 = `style.md` 5-8줄. 수단: `QPushButton` + variant `Q_PROPERTY` | `Q_PROPERTY` variant (SageStyle이 읽음) |
| 436-441 | 메시지 리플렉션으로 컨트롤이 자기 그리기, 부모 `OnDrawItem`/`OnCtlColor`에 분기 없음 | **변경** | 원칙 유지 (화면 클래스에 `paintEvent`/`setPalette` 금지) | SageStyle |
| 443-444 | `CSageFilterComboBox`가 `CBS_OWNERDRAWFIXED` + `DrawItem` | **폐기** | MFC 구현 예시. 게다가 코드 대조상 이 컨트롤 화면 사용처 0 | 불필요 |
| 446-458 | 금지: 부모가 ID 나열로 스타일 분기 / 올바름: `CSageButton : CButton` + `SetVariant` + `DrawItem` | **변경** | 금지 쪽 유지. 「올바름」 예시는 SageQt에서 **위반**(QPushButton 서브클래싱) | `QPushButton` + variant `Q_PROPERTY`는 어디에? — 7장(기본 위젯에 property를 붙이는 방식) |
| 460-461 | 상태는 `itemState`(`ODS_SELECTED`/`ODS_DISABLED`), `CDC::FromHandle` | **폐기** | MFC/GDI 전용 | SageStyle은 `QStyleOption::state` |
| 465-478 | 컨트롤은 도메인 개념 모름, 렌더링 속성만(`SetHighlightColumns(5, 3)`), 업무 판단은 View/핸들러 | **유지** | 원칙 그대로 | delegate · 위젯 속성 / 핸들러 |
| 482-488 | 세 계층: View/Panel → CSage* 컨트롤 → SageUiStyle | **변경** | = `style.md` 11-15줄 계층 (Panel → Sage* 위젯/Delegate → SageStyle + 디자인 값) | SageStyle |
| 490-491 | `SageUiStyle`은 보조 — 둘 이상이 같은 코드일 때만, 화면이 직접 호출 금지 | **유지** | = `style.md` 31줄 | `ui/style/` 그리기 조각 |
| 493-499 | 현재 공유 조각은 `DrawComboArrow` 하나 | **폐기** | 콤보 화살표는 SageStyle(`PE_IndicatorArrowDown`)이 그림. 커스텀 콤보 없음 | SageStyle |
| 501-511 | 추측으로 채우지 않음. 후보(배경 7곳 · 테두리 1곳 · 가운데 텍스트 6곳)가 중복이 아닌 이유. 같은 모양이 아니라 같은 코드여야 옮김 | **유지** | 원칙 유지(구체 수치는 MFC 코드 기준이라 참고용) | 불필요 |

#### 공통 컨트롤 목록 (513-536)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 517 | `CSageHeaderCtrl` 헤더 배경 · 구분선 · 정렬 (`OnPaint`) | **변경** | Qt에 있음 → 서브클래싱 금지 | SageStyle (`QHeaderView`) |
| 518 | `CSageTabCtrl` 탭 배경 · 선택 인디케이터 | **변경** | 같음 | SageStyle (`QTabBar`, `CE_TabBarTabShape`) |
| 519 | `CSageComboBox` 드롭다운 화살표 | **변경** | 같음. 코드 대조상 화면 사용처 0 | SageStyle (`QComboBox`) |
| 520 | `CSageFilterComboBox` 오너드로우 콤보(항목 가운데 정렬) | **폐기** | 화면 사용처 0(코드 대조). 필요하면 delegate | (필요 시) 콤보 view의 delegate |
| 521 | `CSageButton` Primary/Secondary/Ghost/Danger, disabled · pressed, 아이콘(단독 · 텍스트 동반) · 툴팁 | **변경** | 기본 위젯 + variant | SageStyle + variant `Q_PROPERTY` + `QPushButton::setIcon` |
| 522 | `CSageSectionLabel` 섹션 제목 라벨 | **변경** | QLabel로 됨 | `QLabel` + 역할 variant (섹션 제목 폰트) |
| 523 | `CSageListCtrl` 결과 테이블 행 · 강조 컬럼 색 | **변경** | | `QTableView` + model + delegate |
| 524 | `CSageListBox` 목록 상자 — 선택 강조 면 + 카멜 SemiBold, 교대 행, 하단 hairline, NC 1px 테두리 (목록 선택 다이얼로그) | **폐기** | **근거가 삭제된 화면**(목록 선택 다이얼로그 없음, 사용처 0). 목록이 필요해지면 `QListView` + delegate로 새로 정함 | 불필요(현재) |
| 525 | `CSageSidebarTree` 사이드바 트리 표시 | **변경** | | `QTreeView` + delegate (사이드바 전용 색) |
| 526 | `CSageSummaryBar` 요약 밴드 (`라벨 + 강조 수치 + 단위` N개) | **변경** | Qt에 없음 → 커스텀 위젯 허용. 또는 `QLabel` 조합 — 결정 필요 | 커스텀 위젯 |
| 527 | `CSageFilterPillBar` 필터 pill 묶음(`전체 12` · `성공 11` …), 자기가 그리고 클릭 받음 | **변경** | Qt에 없음 → 커스텀 위젯 (style.md가 「필 바」를 예로 듦) | 커스텀 위젯 |
| 528 | `CSageSearchBox` 입력 + 🔍 테두리 하나(우측 32px 칸 · 구분선 · 아이보리). 결과 표용 좌측 기준 칸 미구현 | **변경** | `QLineEdit::addAction(icon, TrailingPosition)`으로 대부분 됨 — 구분선 · 아이보리 칸은 SageStyle 또는 커스텀. D7-6 표기는 도입 이력(결과 표에서 현재 사용) | `QLineEdit` + action, 모양은 SageStyle |
| 529 | `CSageBadge` pill 배지(`관리자` 등), 면 · 테두리 · 글자색 인자 | **변경** | Qt에 없음 → 커스텀 위젯(style.md 예). 헤더에서 사용 중 | 커스텀 위젯 + 변형 `Q_PROPERTY` |
| 530 | `CSageEmptyState` 표 영역 중앙 안내문 + 1차 액션(클릭은 부모로) | **변경** | 커스텀 위젯 또는 QLabel+QPushButton 조합 | 위젯 + 의미 있는 signal |
| 531 | `CSageStatusCard` 대기/처리중/완료/실패 한 자리(후속 액션 부모로) | **변경** | Qt에 없음 → 커스텀 위젯(style.md 예 「상태 카드」) | 커스텀 위젯 + 상태 `Q_PROPERTY` |
| 532 | `CSageInlineError` 인라인 오류 · 경고 1줄, 값이 비면 숨김 | **변경** | 숨기면 자리 이동 — 743줄(「자리 항상 비워 둠」)과 조율 필요 | `QLabel` + variant(오류/경고) — `QSizePolicy::setRetainSizeWhenHidden` |
| 533 | `CSageSelectionBar` `N건 중 M건 선택됨` + 액션 묶음 | **변경** | | 커스텀 위젯 또는 QLabel + 버튼 layout |
| 534 | `CSageEdit` 테두리 1px + 오류 상태, 다이얼로그만 구현 | **변경** | Qt에 있음 → 서브클래싱 금지 | SageStyle (`PE_PanelLineEdit`/`PE_FrameLineEdit`) + 오류 variant `Q_PROPERTY` |
| 536 | 클래스당 파일, `app/ui/drawing/` | **변경** | SageQt 폴더 배치(`coding-design`) 따름 | `ui/widgets/` 등 — coding-design 기준(확인 못함: 폴더명) |

#### `CSageEdit`은 화면에 따라 갈린다 (538-558)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 540-549 | 다이얼로그 7종은 적용, 패널·View 16곳은 보류(부모가 `InflateRect(1,1)`로 바깥 1px) | **폐기** | **근거가 삭제된 화면**(다이얼로그 7종 → 현재 3종, 패널·View 16곳 → 현재 `DrawEditBorder` 2곳). 게다가 SageQt에서는 두 방식이 모두 없음(테두리 = SageStyle 하나) | SageStyle |
| 551-553 | `WS_BORDER` 제거 + `WM_NCCALCSIZE` 1px 확보 + `WM_NCPAINT`, `SetWindowTheme(hwnd, L"", L"")` | **폐기** | Win32 전용 | 불필요 |
| 555 | 덧칠 방식은 깨짐 — 그릴 주체를 하나로 | **유지** | 원칙 = SageStyle 단일 | SageStyle |
| 557-558 | 화면이 `SetFont()` 나눠주는 구조(`ApplyControlFonts`) → 컨트롤이 자기 폰트 알게, 공용 폰트 저장소 필요 | **변경** | SageQt: 앱 폰트 1회 적용, 화면 `setFont` 금지. 역할별 폰트(표 셀 · 캡션 · 제목)를 누가 적용하는지는 결정 필요(7장) | 앱 폰트 + SageStyle/variant |

#### 버튼 변형 (560-571)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 564 | PRIMARY: 배경 카멜 · 테두리 카멜 · 텍스트 흰색 · Bold · 생성/실행/저장/확인 | **유지** | 값 유지 | variant `Q_PROPERTY` + SageStyle |
| 565 | SECONDARY: 패널 흰색 · 중성 `#C9BFB1` · 본문색 · Regular · 파일 선택/폴더 선택/취소/수정 | **유지** | | 같음 |
| 566 | GHOST: 투명 · 없음 · `#6E655B` · Regular · 초기화/선택 해제 | **유지** | | 같음 |
| 567 | DANGER: 패널 흰색 · `#E0BDB6` · 오류색 · Bold · 삭제 계열 전용 | **유지** | | 같음 |
| 569-571 | Secondary 테두리는 중성색(카멜 아님) | **유지** | | 같음 |

#### 버튼 위계 배분 (573-596)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 575 | 한 카드에 Primary 하나, 카드 없으면 화면에 하나 | **변경** | 「카드 단위」의 근거는 **삭제된 화면**(목업 3-6: 목록 카드 「항목 추가」 + 편집 카드 「저장」, 590-593줄). 현재 SageSDI 화면은 카드 2개가 Primary를 가진 곳이 없음(확인: 데이터 관리 화면 없음). 「화면당 1개」(766 · 897줄)와 모순 — SageQt에서 어느 쪽을 쓸지 결정 필요(7장) | 설계 규칙 |
| 577-581 | 완료 동작 1개 = Primary, 보조 = Secondary, 되돌림 = Ghost, 삭제만 Danger, 읽기 화면은 Primary 없어도 됨 | **유지** | | variant |
| 583-585 | 카드가 단위인 이유 (컨텍스트) | **변경** | 575와 같은 근거(삭제된 화면) | |
| 587-588 | 파괴적 동작이 추가와 같은 무게 금지(「항목 추가 / 하위 항목 추가 / 수정 / 삭제」 묶음) | **유지** | 예시는 삭제된 화면이지만 원칙은 일반적 | variant |
| 590-593 | 이력: 목업 3-6 두 카드 → 카드 단위로 완화 | **폐기** | **근거가 삭제된 화면** (목업 3-6) | 불필요 |
| 595-596 | Text/Link형 버튼 안 씀. Ghost는 테두리 없는 사각 버튼, 클릭 영역 · 높이는 다른 변형과 같음 | **유지** | | SageStyle (`QPushButton::setFlat` 대신 variant) |

#### 새 UI 요소는 처음부터 공통 컨트롤로 (598-611)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 600-601 | 없던 종류는 한 곳만 쓰여도 컨트롤로 | **유지** | = `style.md` 33줄 | 커스텀 위젯 |
| 603-605 | 버튼 그리기 코드가 View 1곳 · 다이얼로그 7곳에 358줄 복제 | **폐기** | SageSDI 이력(다이얼로그 7종도 현재 3종) | 불필요 |
| 607-611 | 지금 필요한 변형 하나만, 두 번째 화면에서 variant 추가. speculative generality 아님 | **유지** | CLAUDE.md 2와 일치 | |

#### 승격 절차 (613-621)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 615-618 | 1 기존 컨트롤 확인 → 2 선언·생성·변형 → 3 없으면 최소 컨트롤 추가 + 리플렉션 → 4 차이는 variant/property | **변경** | 3의 「리플렉션」 → SageStyle 재정의 우선, Qt에 없을 때만 커스텀 위젯 | SageStyle → `Q_PROPERTY` → 커스텀 위젯 순 |
| 620-621 | 부모 `OnDrawItem`/`OnCtlColor`에 새 분기 금지 — 그러고 싶으면 승격 신호 | **변경** | = `style.md` 38-39줄 판단 기준 | SageStyle |

#### 직접 클릭을 받는 컨트롤의 함정 (623-633)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 625-629 | `SS_NOTIFY` 정적은 창 ID · 명령 ID 다르게(`STN_CLICKED` = `BN_CLICKED` = 0). `CSageSearchBox` 버그(D7-6) | **폐기** | Win32 전용 | 불필요 (signal/slot) |
| 631-633 | `CHeaderCtrl`의 `fmt` 바꾸지 않음(셀 정렬까지 바뀜). 데이터 관리 표 이름 컬럼 원인(D7-6) | **폐기** | Win32 전용 + **근거가 삭제된 화면**(데이터 관리 표) | 불필요 (Qt 헤더 정렬과 셀 정렬은 독립) |

#### 에딧 안 글자를 세로 가운데로 두는 법 (635-643)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 639 | `ES_MULTILINE`: `EM_SETRECT`로 `top += SAGE_EDIT_TEXT_TOP_PAD` | **폐기** | Win32. `QLineEdit`은 세로 가운데가 기본 | 불필요 |
| 640 | 단일 행: `EM_SETRECT` 안 먹음 → 높이를 글줄 높이에 맞추고 부모 안 가운데 | **폐기** | Win32 | 불필요 |
| 642-643 | 글줄 높이 추측 금지, `TEXTMETRIC::tmHeight` 실측 (20 어림 → 두 번 고침, D7-6) | **변경** | 「추측 금지 · 실측」은 유지(CLAUDE.md No guessing) | `QFontMetrics::height()` |

### 1.13 MFC 화면 패널 규격 (647-696)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 649-650 | 패널 규격 = 화면 영역이 자기를 소유, 전문은 `coding-design` | **유지** | SageQt `ui-composition.md`가 대체 | `ui-composition.md` |
| 654-664 | 패널 트리: CSageSDIView → Sidebar · Header · Workspace(Input · Result · History), 업무 전용 패널은 형제 | **변경** | = `ui-composition.md` 54-67 (`SageMainWindow` · `QStackedWidget`) | 패널 구성 |
| 666-668 | 탭 하나 = 패널 하나, `SageResultTablePanel` 두 인스턴스 | **유지** | = `ui-composition.md` 72-81 | QStackedWidget |
| 670-671 | 패널은 `CWnd` 파생 자식 윈도우, 자기 컨트롤·메시지맵·레이아웃, 위임 스텁 없음, `C` 접두 없음 | **변경** | `QWidget` 파생 + 자기 `QLayout` + `createWidgets/createLayout/connectSignals` | layout |
| 673-676 | 패널도 과중 가능, 완료 기준 A~E 적용 | **유지** | = `ui-composition.md` 147-178 | |
| 680-684 | 공용 패널에 업무 분기 없음, 핸들러가 답함. 판단 기준: 워크플로 추가 시 패널을 고치는가 | **유지** | = `ui-composition.md` 12-39 | 핸들러 |
| 688-690 | 컨트롤러가 상태 전이 · 워커 수명, 컨트롤 API 호출 안 함, 결과는 패널 HWND로 `PostMessage` | **변경** | `PostMessage` → signal | `SageWorkflowController` signal → 패널 slot |
| 694-696 | 새 화면: 1 패널 먼저, 2 View에 컨트롤 · `OnSize` 좌표 추가 금지, 3 공통 컨트롤 사용 | **변경** | `OnSize` 좌표 → `move/resize/setGeometry` 금지 | layout |

### 1.14 현재 구현된 설계 결정 (700-879)

#### 폰트 (704-710)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 705-708 | 크기는 타입 스케일 표 하나만 본다. 옛 `g_fontControl` 9.8pt · 표 셀 9pt · 캡션 8.3pt는 낡은 값(D1 이후 105 · 98 · 90) | **유지** | 「값은 한 곳」 = `SageDesignDefine.h` 한 곳. 낡은 값 서술 자체는 폐기 대상 | `SageDesignDefine.h` |
| 709 | 폰트는 `SageUiResources` 공용 저장소, 화면이 서체명 · 크기 직접 안 씀 | **변경** | | 앱 폰트(main.cpp) + `SageDesignDefine.h` |
| 710 | 새 컨트롤: 표는 표 셀 폰트, 상태 · 설명은 캡션 폰트, 그 외 본문 | **유지** | 역할 규칙 | 역할별 폰트 — 적용 주체 결정 필요(7장) |

#### 에디트 박스 (712-726)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 713 | 스타일 `ES_MULTILINE \| ES_READONLY` (WS_BORDER 없음) | **폐기** | Win32. 입력 경로 표시는 `QLineEdit::setReadOnly(true)` | 불필요 |
| 714-715 | 패널 · View 기존: 부모가 `DrawEditBorder()`로 `SAGE_COLOR_BORDER` 1px, 신규는 복제 금지 | **폐기** | MFC 구현 이력 | SageStyle이 `#DCD6CD` 1px |
| 716 | 다이얼로그는 이미 `CSageEdit` (D5a) | **폐기** | 구현 상태 기록 | |
| 717-719 | 오류 상태 `SetState(SAGE_EDIT_ERROR)` → 테두리 `ERROR`. 인라인 오류 줄은 폼 하단 한 곳이라 어느 입력이 틀렸는지는 테두리가 알림 | **변경** | 의도 유지 | 에디트 오류 variant `Q_PROPERTY` → SageStyle이 `#B85C4A` 테두리 |
| 719 | `WS_BORDER`를 다시 넣지 않음 | **폐기** | Win32 | |
| 720 | 텍스트 수직: `EM_SETRECT` top+7px (`SAGE_EDIT_TEXT_TOP_PAD = 7`) | **폐기** | Win32. 코드 대조: 현재 `SageDefine.h:143`은 `9` — 스킬과 불일치 | 불필요 |
| 721 | 높이 `SAGE_EDIT_HEIGHT = 32` (버튼 높이와 동일) | **변경** | 32 유지. 고정 높이 vs 14px 폰트 + 여백 — 5장 | `SageDesignDefine.h` + SageStyle `sizeFromContents` 또는 `setFixedHeight`(값은 디자인 값) |
| 722 | 레이블-에디트 간격 `SAGE_LABEL_EDIT_GAP = 4` (좌측 테두리 가림 방지) | **변경** | 4 유지. 「테두리 가림 방지」 이유는 GDI 바깥 테두리 때문 — SageQt에서는 이유 소멸, 간격 값만 | layout spacing (`SageDesignDefine.h`) |
| 723-726 | `WS_BORDER` 재추가 금지는 View 한정, 다이얼로그는 사용, 이유는 `DEBT_LOG` | **폐기** | Win32 + SageSDI `DEBT_LOG` 참조 | 불필요 |

#### 다이얼로그 (728-763)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 730-731 | 창 높이 상수 금지, `LayoutControls()` 바닥 → `SageDialogSizer::SizeToClient` | **변경** | 의도 유지, 수단은 layout `sizeHint` | `QLayout::setSizeConstraint(SetFixedSize)` 또는 `adjustSize()` |
| 733-736 | 높이 상수 수동 동기화 금지 (D5a 7종 `+30` 사례) | **유지** | 원칙. 사례는 7종 시절 이력 | layout |
| 738 | 폭은 상수 — 콘텐츠가 정하는 값 아님 | **변경** | 폭 상수는 텍스트 폭 가정과 충돌할 수 있음(5장) | `SageDesignDefine.h` 최소 폭 + layout |
| 740-741 | 예외: 늘어나는 자식 있는 창(목록 선택 다이얼로그)은 고정 높이 | **폐기** | **근거가 삭제된 화면**(목록 선택 다이얼로그 없음) | 불필요(현재) |
| 743 | 인라인 오류 자리는 항상 비워 둠(버튼 안 움직이게) | **유지** | | `setRetainSizeWhenHidden(true)` 또는 빈 텍스트 유지 |
| 745-746 | 본문은 흰 면(`PANEL`), 캡션 밴드만 `#F2EEE7` (목업 3-9 `background:#fff`) | **변경** | 색 규칙 유지. 캡션 밴드는 프레임리스 다이얼로그 전제 — SageQt에서 프레임리스를 유지하는지 확인 못함(7장) | QPalette(다이얼로그 `Window` = 흰색?) · 캡션 밴드 위젯 |
| 747-748 | `CSageLabel` · `CSageInlineError`는 자기 배경을 칠하므로 다이얼로그에선 `SetBackgroundRole(SAGE_BG_PANEL)` | **폐기** | GDI 배경 칠 문제. Qt 라벨은 기본 투명(autoFillBackground false) | 불필요 |
| 750-751 | 모달 알림은 `ShowSageMessageBox`, `AfxMessageBox` 신규 금지 (D9). `SageFramelessDialog` 파생 | **변경** | 의도(앱 모양 통일) 유지 | `QMessageBox`를 SageStyle로 그릴지, 전용 다이얼로그인지 결정 필요(7장) |
| 755 | 진입점 `int ShowSageMessageBox(LPCWSTR, UINT nType = MB_OK, CWnd* pParent = NULL)` — `AfxMessageBox` 호환 | **폐기** | Win32 시그니처 | Qt API로 새로 |
| 756 | 아이콘 3종(정보 `PRIMARY` · 경고 `WARNING` · 오류 `ERROR`), 22px 원 + 2px 획. 「?」 안 만듦, `MB_ICONQUESTION` → 경고 | **변경** | 3종 · 22 · 2 유지 | 그리기 조각 + `SageDesignDefine.h` |
| 757 | 버튼: 확인 = Primary / 예 = Danger · 아니오 = Secondary. `MB_YESNO`는 전부 삭제 확인 | **변경** | 「`MB_YESNO`는 전부 삭제 확인」은 SageSDI 사용처 사실 — SageQt 확인 대상 화면이 현재 무엇인지 확인 못함 | variant |
| 758 | 취소: Esc · 캡션 X = `IDCANCEL`, `MB_YESNO`에선 `IDNO`. Enter는 기본 포커스 쪽 | **변경** | 동작 의도 유지 | `QDialog::reject` / default button |
| 759 | 다이얼로그 위에서는 `pParent = this`, 패널은 기본값 | **변경** | Qt에선 부모 지정이 모달 위치 · 소유 결정 | 부모 `QWidget*` |
| 760 | 예외: `SageSDI.cpp` 앱 초기화 실패 1곳은 `AfxMessageBox` (`InitInstance`) | **변경** | main.cpp 시점의 실패 알림 방식 — T06에서 이미 정했는지 확인 못함 | main.cpp |
| 762-763 | 메시지 본문은 본문 크기 · `SAGE_COLOR_TEXT`, 인라인 오류색 `#9C4433` 금지(12px 인라인 전용) | **유지** | | |

#### 버튼 (765-772)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 766 | 변형은 *버튼 변형* 표, **화면당 Primary 1개** | **변경** | 575줄(카드당 1개)과 모순 — 7장 | variant |
| 767 | 높이 `SAGE_BUTTON_HEIGHT = 32`, 아이콘 버튼 32×32 | **변경** | 값 유지, 고정 높이는 텍스트 높이 의존(5장) | `SageDesignDefine.h` + SageStyle `sizeFromContents(CT_PushButton)` |
| 767-768 | 카드 안 액션 행만 34 (`SAGE_CARD_ACTION_BUTTON_HEIGHT`). 「위 *크기와 좌표* 참조」 | **유지** | 입력 카드(`SageWorkflowInputPanel`)에서 사용 중(코드 대조). 참조 방향은 「아래」가 맞음(849줄) | `SageDesignDefine.h` + variant 또는 크기 속성 |
| 769-770 | 기존 구현은 부모 `OnDrawItem` ID 나열, 신규는 `CSageButton` + `SetVariant()` | **폐기** | MFC 이력 | variant `Q_PROPERTY` |
| 771-772 | `SAGE_BUTTON_TEXT_TOP_OFFSET`은 ascent 보정, Gmarket 기준 `2`, Pretendard 전환 시 재실측 | **폐기** | GDI 텍스트 위치 보정. Qt는 `drawItemText` + 정렬로 세로 가운데. 코드 대조: 현재 값 `0`(`SageDefine.h:149`) — 스킬과 불일치 | 불필요 (단 3 OS에서 세로 위치 확인은 필요) |

#### 아이콘 (774-781)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 776 | 그린다, 유니코드 글리프 안 씀(D6, 진단 9) | **유지** | | 그리기 조각 / SVG |
| 777 | 굵기 세트 전체 2px. 목업 실측 1.2~1.5px지만 GDI 1px 선은 약함. 예외 없음 | **변경** | 400줄(1.5px)과 모순. 「GDI 1px 선이 약하다」는 GDI 근거 — `QPainter` 안티앨리어싱에서는 1.5px 가능. 값 재결정 필요(7장) | `SageDesignDefine.h` + `QPen` |
| 778 | 길이 · 두께 홀짝 맞춤(`FillSolidRect` `[x, x+w)`, 십자 span 10 / 두께 2) | **변경** | GDI 정수 채우기 근거. `QPainter`(실수 좌표 · 안티앨리어싱)에서는 다른 규칙 필요. 10 / 2 값 자체는 유지 가능 | 그리기 조각 |
| 779 | 아이콘 단독 버튼 `SAGE_ICON_BUTTON_SIZE`(32) 정사각 + 툴팁 필수 (`CSageButton::SetTooltip`) | **유지** | | `SageDesignDefine.h` + `setToolTip` |
| 780 | 아이콘 + 텍스트 `SAGE_ICON_TEXT_GAP`(6), 묶음 가운데 | **변경** | 6 유지. 묶음 폭 = 아이콘 + 6 + 텍스트 폭(`QFontMetrics`) | SageStyle `CE_PushButtonLabel` |
| 781 | 화면이 이상하면 목업 수치와 먼저 대조 | **유지** | | |

#### 앱 셸 — 헤더와 탭 줄은 흰 면 (783-806)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 785-786 | 흰 면은 위쪽 두 줄(헤더 · 탭), 콘텐츠는 아이보리. 콘텐츠 안 요소(요약 바 · 선택 바)에 흰 박스 금지 | **유지** | | QPalette + 패널 배경 역할 |
| 790 | 헤더(제목 · 인증) 높이 56, `#FFFFFF`, 하단 1px `#DCD6CD` | **변경** | 값 유지. 56 고정 안에 19px 제목 + 32 버튼 — 5장 | `SageDesignDefine.h` + 헤더 패널 최소/고정 높이 |
| 791 | 탭 줄 높이 40, `#FFFFFF`, 하단 1px `#DCD6CD` | **변경** | 같음 | SageStyle (`QTabBar` `sizeFromContents(CT_TabBarTab)`) |
| 792 | 콘텐츠 영역 나머지, `#F8F6F1` | **유지** | | QPalette `Window` |

#### 탭 (794-806)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 795 | `CSageTabCtrl` 서브클래스 `OnPaint`(시스템 테마 구분선 제거) | **폐기** | MFC. Fusion은 테마 선 없음 | SageStyle |
| 796 | 줄 전체 흰 면, 선택 · 비선택 배경 같음 | **유지** | | SageStyle |
| 797 | 선택 탭: Bold + `SAGE_COLOR_TEXT` + 하단 `SAGE_TAB_INDICATOR_HEIGHT = 2`px 카멜 라인 | **변경** | 값 유지. **Bold/Regular 전환 시 탭 텍스트 폭이 바뀜** — 탭 폭 흔들림(5장) | SageStyle (`State_Selected`) |
| 798 | 비선택: 같은 흰 배경 + 보조 텍스트 색, 배경으로 구분 안 함 | **유지** | | SageStyle |
| 799 | 생성/실행 버튼은 입력 탭에서만 (`IsActionTabVisible()` = `IsInputTabSelected()`) | **변경** | 탭 = 패널이면 입력 패널에만 버튼이 있으므로 자동 성립 | 패널 구성(불필요 판단식) |
| 801-806 | 이력: 탭 줄 아이보리 오기 → 목업 3-1 `#FFFFFF` · `font-weight:700` · `box-shadow:inset 0 -2px 0 #9A6B3F`. 스킬은 목업 요약본, 목업을 함께 연다 | **유지** | 이력 부분은 폐기해도 되나 「목업을 함께 연다」 교훈은 유효. 목업 3-1은 살아 있는 화면 | 불필요 |

#### 사이드바 (808-815)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 809 | 배경 `SAGE_COLOR_SIDEBAR = RGB(36, 31, 26)` | **유지** | = `#241F1A` | `SageDesignDefine.h` · 사이드바 view 배경 |
| 810 | 텍스트 `SAGE_COLOR_SIDEBAR_TEXT = RGB(205, 196, 185)` | **유지** | = `#CDC4B9` (환산) | delegate |
| 811-812 | 선택 `SAGE_COLOR_SIDEBAR_SELECTED = RGB(58, 49, 41)` + 좌측 3px 카멜 바 (`SAGE_SELECTION_ACCENT_WIDTH` — 표 선택 행과 같은 상수 공유). 전체를 밝은 카멜로 채우지 않음 | **변경** | 값 유지. 「표와 같은 상수 공유」는 302줄 · 코드(`SAGE_SELECTION_ACCENT_WIDTH = 3`, `SAGE_LIST_SELECTION_ACCENT_WIDTH = 4`, `SageDefine.h:171-172`)와 **모순 — 낡은 문장** | delegate + `SageDesignDefine.h` 3 |
| 813 | 상태바 위까지 full-bleed, 하단 틈 없음 | **유지** | | layout (margin 0) |
| 814 | `SetWindowTheme` + `NM_CUSTOMDRAW`로 파란 선택색 제거 | **폐기** | Win32. Fusion + delegate | delegate |
| 815 | `TVS_NOSCROLL` 스크롤바 비표시 | **변경** | 의도(스크롤바 숨김) 유지 | `setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff)` — 항목이 넘칠 때 동작 확인 못함 |

#### 진행바 / 상태 표시 — `CSageStatusCard` (817-838)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 819-820 | 한 컨트롤이 대기 · 처리 중 · 완료 · 실패, 높이 70 고정(가장 높은 처리 중에 맞춤, 나머지 세로 가운데) | **변경** | 의도 유지. 70 고정은 줄 높이 합(코드 대조: 제목 줄 20 · 상세 줄 16 · 진행바 6) 전제 — 5장 | 커스텀 위젯 + 상태 `Q_PROPERTY` + `sizeHint` |
| 824 | 대기: 흰 면 / `#DCD6CD`, 회색 dot `#B4ABA0` + 안내 한 줄, 진행바 없음 | **유지** | | 커스텀 위젯 |
| 825 | 처리 중: 흰 면 / `#DCD6CD`, dot `#B88746` + 문구 · 우측 `%` · 6px 진행바(트랙 `#EDE8E0` / 채움 카멜) | **유지** | 진행바는 `QProgressBar` + SageStyle도 가능 | 커스텀 위젯 또는 `QProgressBar`(SageStyle) |
| 826 | 완료: `#F1F5F0` / `#D5E0D3`, 체크 18px `#5F7F5F` + 제목 `#41603F` + 저장 경로 `#6E655B` + 「폴더 열기」 | **유지** | | 커스텀 위젯 |
| 827 | 실패: `#FDF6F4` / `#E0BDB6`, 느낌표 18px `#B85C4A` + 제목 `#9C4433` + 사유 | **유지** | | 커스텀 위젯 |
| 829-830 | 대기 · 실패는 목업에 없음. 실패 색은 목업 3-5 실패 행(`#FDF6F4`). 배지용 `#F8EBE9`(`STATUS_BG_ERROR`)는 pill 전용 | **유지** | 3-5(실행 기록) 존재 | `SageDesignDefine.h` |
| 831-833 | 후속 버튼은 완료 + 저장 경로 있을 때만, 「폴더 열기」 하나, Secondary. 「결과 보기」 걷어냄(D7-4 3단계) | **유지** | D7-4는 입력 패널 상태 카드 — 현재 존재 | 패널 + `QDesktopServices::openUrl` |
| 834-835 | 처리 중 문구 고정, %는 타이머 흉내. 세분화하려면 통지부터 (`DEBT_LOG`) | **변경** | SageSDI `DEBT_LOG` 참조. SageQt에서는 T14(진행률) 결정 대상 (MIGRATION_PLAN 46줄) | 컨트롤러 진행 signal — T14 |
| 836 | 헤더 우측 상태 텍스트(`m_wndHeaderStatus`) 함께 갱신 | **변경** | 컨트롤러 signal을 헤더 패널도 받음 | signal |
| 837 | 없는 기능 안내 안 함(「예상 소요 · Esc 취소」 뺌) | **유지** | | |
| 838 | 워크플로 바꾸거나 초기화하면 대기로, 워크플로별 저장 · 복원 안 함 | **유지** | | 패널 |

#### 빈 화면 (840-844)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 841 | 파일 미선택 / 결과 없음: `CSageEmptyState` 안내문 + 1차 액션을 표 영역 중앙 | **변경** | | 빈 상태 위젯 + layout 가운데 정렬 |
| 842-843 | 안내문은 「무엇이 없다」 + 「무엇을 하면 되는지」 (「등록된 항목이 없습니다 / 대상을 선택한 뒤 항목을 추가하세요」) | **유지** | 예문의 「항목 추가」는 데이터 관리 화면 쪽 표현으로 보이나 확인 못함 | `SAGE_UI_` 문자열 |
| 844 | 빈 격자 채우지 않음 | **유지** | | |

#### 워크플로우 전환 (846-847)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 847 | 다른 업무 선택 시 입력 경로 · 저장 위치 · 상태 텍스트 · 결과 표 자동 초기화 | **유지** | | 패널 slot |

#### 크기와 좌표 (849-873)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 850 | 크기 상수는 96dpi 기준 논리 픽셀(컨트롤 높이 32, 라벨 폭 64, 행 높이 34) | **변경** | Qt 논리 픽셀은 macOS에서도 같은 크기(장치 픽셀 비율은 Qt가 처리). 텍스트는 T02대로 픽셀 크기 지정 시 ±4% | `SageDesignDefine.h` (논리 px) |
| 851 | 간격은 4의 배수만 | **유지** | | `SageDesignDefine.h` 값 규칙 |
| 852-854 | 컨트롤 높이 32 기본, 예외는 카드 안 액션 행 34 하나(목업 3-4 실측). 목업이 30이어도 32 (D7-4 3단계). 높이 셋이면 잡음 | **유지** | 입력 카드 존재(코드 대조) | `SageDesignDefine.h` |
| 855 | 폼 라벨 폭 64 단일, 긴 라벨 폼만 96(비밀번호 변경 — 「변경할 비밀번호」 88px). 본문 · 다이얼로그 구분 없음 | **변경** | **고정 텍스트 폭 가정**. 88px은 Windows GDI 실측. SageQt 규칙(「텍스트 폭을 숫자로 가정하지 않는다」)과 충돌 → 라벨 열 폭은 `QFormLayout`/`QGridLayout`이 라벨 `sizeHint`로 정함 | layout (`QFormLayout`) |
| 858-862 | 이력: 80(목업 CSS `width:80px`) 도입 → 라벨이 40px 떨어져 보임, 두 라벨 다 52px 실측 → 64로 | **폐기** | 이력. 교훈(목업 CSS 폭을 그대로 옮기지 않음)은 layout 방식에서 자동 해소 | 불필요 |
| 863-866 | 라벨 폭 실측: PowerShell `AddFontResourceEx(FR_PRIVATE)` + `CreateFontW(lfHeight=-14)` + `GetTextExtentPoint32W`. GDI+ 금지, `GetTextFaceW`로 face 확인 | **변경** | 「실측 · face 확인」 원칙 유지, 도구는 T02 `tools/font-probe`(Qt) | `QFontMetrics` · `QFontInfo::family()` |
| 867-868 | 모든 좌표는 `LayoutChildControls`/`LayoutControls` 안, 생성 시점 rect 쓰는 컨트롤 금지 | **변경** | SageQt는 좌표 계산 자체 금지 | QLayout |
| 869 | 요약 바 · 선택 바 폭은 표 폭에서 파생, 고정 폭 금지 | **변경** | | 같은 `QVBoxLayout`에 넣어 폭 공유 |
| 870-871 | 컬럼 폭은 `SageWorkflowColumn` 비율 배분(`MulDiv`), 컬럼 증감 시 정의 폭 합 재조정 | **변경** | core 픽셀 금지(`model-view.md` 22줄), Win32 `MulDiv` core 위반(MIGRATION_PLAN 29줄) | `QHeaderView` 모드 — 비율 stretch는 7장 |
| 872-873 | 좌표 계산에 숫자 직접 금지, DPI 배율 대응 시 상수 지점에 스케일 변환 | **변경** | 하드코딩 금지는 유지, DPI는 Qt가 처리 | `SageDesignDefine.h` |

#### UI 텍스트 언어 (875-878)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 876 | 레이블 · 버튼 · 컬럼 헤더 한국어 통일 | **유지** | `values-and-platform.md` 30줄(다국어 없음)과 일치 | `SAGE_UI_` 상수 |
| 877 | Input→파일, Output→폴더, Status→상태, Reason→사유 | **유지** | | 같음 |
| 878 | 영문 레이블 신규 금지 | **유지** | | 같음 |

### 1.15 디자인 검토 체크리스트 (882-904)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 886-893 | 사이드바로 업무 찾기 · 현재 업무/단계 · 영역 구분 · 생성/검수 구분 · 표 스캔 · 상태 정의 · ERP 톤 · 효율 우선 | **유지** | 플랫폼 무관 | `sageqt-ui` 체크리스트 |
| 897 | 한 화면에 Primary 버튼 하나인가? | **변경** | 575줄(카드 단위)과 모순 — 7장 | |
| 898 | 갈색 채운 면이 카드당 Primary 1개 · 사이드바 선택 · 선택 행뿐인가? | **변경** | 같은 모순 | |
| 899-904 | 파괴적 동작 무게 · 세로 격자/금액 배경 없음 · 요약 바 수치 · 빈 상태 · 창 크기 변화 시 요약/선택 바 정렬 · 아이콘 툴팁/글리프 | **유지** | 903은 layout이면 자동 성립하지만 점검 항목으로 유효 | 체크리스트 |

### 1.16 참조 (908-915)

| 줄 | 규칙 / 규격 | 판정 | 이유 | SageQt에서의 대응 |
|---|---|---|---|---|
| 910-911 | 단계 · 명세 · 리스크는 `docs/decisions/DESIGN_PLAN.md`(SageSDI) | **폐기** | SageSDI 문서. SageQt 계획은 `sageqt-plan` | 불필요 |
| 913-915 | 원본 디자인: Claude Design 프로젝트 `d4a524f6-c20a-4550-879c-772ca9fbb521` (`SageSDI 디자인 개선안.dc.html` — 목업 9세트 + 다이얼로그 6종). UI 수정 시 목업 기준 | **유지** | 목업은 디자인 원본으로 여전히 유효(단 삭제된 화면 목업 포함) | `sageqt-ui`가 참조 |

---

## 2. 색 팔레트 (원문 값 그대로)

| 이름(상수) | 값 | 역할 | 줄 |
|---|---|---|---|
| `SAGE_COLOR_APP_BACKGROUND` | `#F8F6F1` | 앱 배경 · 콘텐츠 영역 | 127, 792 |
| `SAGE_COLOR_PANEL` | `#FFFFFF` | 패널 · 헤더 · 탭 줄 · 다이얼로그 본문 · 버튼 Secondary/Danger 면 · 상태 카드 대기/처리 중 면 | 128, 790-791, 745 |
| `SAGE_COLOR_LIST_HEADER` | `#F2EEE7` | 표 헤더 · 툴바 · 빈 상태 아이콘 박스 · 표 하단 합계 밴드 · 다이얼로그 캡션 밴드 | 129, 145, 745 |
| `SAGE_COLOR_SIDEBAR` | `#241F1A` = `RGB(36, 31, 26)` | 사이드바 배경 | 130, 809 |
| `SAGE_COLOR_SIDEBAR_TEXT` | `RGB(205, 196, 185)` (= `#CDC4B9`, 환산) | 사이드바 텍스트 | 810 |
| `SAGE_COLOR_SIDEBAR_SELECTED` | `RGB(58, 49, 41)` = `#3A3129` | 사이드바 선택 항목 | 170, 811 |
| `SAGE_COLOR_BORDER` | `#DCD6CD` | 테두리 · 헤더/탭 하단 1px · 에디트 테두리 · 상태 카드 대기/처리 중 테두리 | 131, 714, 790-791, 824-825 |
| `SAGE_COLOR_LIST_GRID` | `#EDE8E0` | 표 가로 hairline · 진행바 트랙 | 132, 295, 825 |
| `SAGE_COLOR_TEXT` | `#2F2A24` | 본문 텍스트 · 선택 탭 · 메시지 본문 · Secondary 버튼 글자 | 133, 797, 762 |
| `SAGE_COLOR_SECONDARY_TEXT` | `#7A7064` | 보조 텍스트(요약 바 라벨 · 단위, 합계 밴드 건수, 선택 바, 빈 상태 설명, 패널 헤더 건수) · 비선택 탭 | 134, 184, 798 |
| `SAGE_COLOR_TEXT_MUTED` | `#6E655B` | 표 헤더 텍스트 · 폼 라벨 · 합계 밴드 「합계」 · Ghost 버튼 · 상태 카드 저장 경로 | 135, 183, 566, 826 |
| `SAGE_COLOR_BUTTON_BORDER` | `#C9BFB1` | Secondary 버튼 테두리 | 136, 565 |
| `SAGE_COLOR_DANGER_BORDER` | `#E0BDB6` | Danger 버튼 테두리 · 상태 카드 실패 테두리 | 137, 567, 827 |
| `SAGE_COLOR_TEXT_PLACEHOLDER` | `#B4ABA0` | 빈 값(`—`) · 상태 카드 대기 dot | 138, 303, 824 |
| `SAGE_COLOR_LIST_ROW_SELECTED` | `#F1E3CD` | 표 · 목록 선택 행 | 139, 302 |
| `SAGE_COLOR_INLINE_ERROR_TEXT` | `#9C4433` | 인라인 오류 텍스트(12px 전용) · 상태 카드 실패 제목 | 140, 762, 827 |
| `SAGE_COLOR_INLINE_WARN_TEXT` | `#8A6A32` | 인라인 경고 텍스트 | 141 |
| `SAGE_COLOR_INLINE_WARN_BG` | `#FBF5EE` | 인라인 경고 박스 면 | 142 |
| `SAGE_COLOR_INLINE_WARN_BORDER` | `#EBDCC6` | 인라인 경고 박스 테두리 | 142 |
| `SAGE_COLOR_ACCENT_SURFACE` | `#F7F2EA` | 강조 면(카드 안 합계 박스 · 필터 pill) | 143, 146 |
| `SAGE_COLOR_PRIMARY` | `#9A6B3F` | 주요 액션 카멜 · Primary 버튼 면/테두리 · 선택 바(표 4px · 사이드바 3px) · 탭 인디케이터 · 진행바 채움 | 157, 564, 797, 825 |
| `SAGE_COLOR_PRIMARY_PRESS` | `#76502A` | 카멜 press · 목록 선택 행 텍스트 | 158, 302 |
| `SAGE_COLOR_SUCCESS` | `#5F7F5F` | 성공 · 완료 체크 아이콘 | 159, 826 |
| `SAGE_COLOR_WARNING` | `#B88746` | 경고 · 아이콘 · 에디트 테두리(경고) · 처리 중 dot | 160, 150, 825 |
| `SAGE_COLOR_ERROR` | `#B85C4A` | 오류 · 아이콘 · 에디트 오류 테두리 · Danger 버튼 글자 · 실패 느낌표 | 161, 150, 567, 717, 827 |
| (상수명 없음, 흰색) | `#FFFFFF` / 「흰색」 | Primary 버튼 텍스트 | 564 |
| `SAGE_COLOR_LIST_ROW_ALT` | **스킬에 값 없음** (코드 대조: `RGB(250, 248, 244)` = `#FAF8F4`, `SageDefine.h:270`) | 표 교대 행 | 298 |
| `SAGE_COLOR_STATUS_BG_ERROR` (`STATUS_BG_ERROR`) | `#F8EBE9` | 오류 배지(pill) 면 전용 | 830 |
| (상태 카드 완료 면) | `#F1F5F0` | 상태 카드 완료 면 | 826 |
| (상태 카드 완료 테두리) | `#D5E0D3` | 상태 카드 완료 테두리 | 826 |
| (상태 카드 완료 제목) | `#41603F` | 상태 카드 완료 제목 | 826 |
| (상태 카드 실패 면 · 목업 3-5 실패 행) | `#FDF6F4` | 상태 카드 실패 면 · 실행 기록 실패 행 | 827, 829 |

- 스킬에 이름만 있고 값이 없는 것: `SAGE_COLOR_LIST_ROW_ALT`(298). 행 상태 색(`SetRowStyle`, 308)의 성공/실패 행 배경 · 배지 색 값은 **스킬에 없음** (829줄에 실패 행 `#FDF6F4`만 언급)
- 개수: 원문에 값이 적힌 색 **31개**(흰색 텍스트 포함, `LIST_ROW_ALT` 제외) + 값 없는 이름 1개

## 3. 레이아웃 · 여백 규격 (원문 값 그대로)

| 규격 | 값 | 적용 위치 | 줄 |
|---|---|---|---|
| 표 행 높이 | 34 고정 (목업 3-5는 40이지만 34) | 모든 `CSageListCtrl` 표 | 294, 310 |
| 표 가로 hairline | 1px | 표 행 하단 | 295 |
| 표 선택 행 좌측 바 | 4px (`SAGE_LIST_SELECTION_ACCENT_WIDTH`) | 표 선택 행 | 302 |
| 사이드바 선택 좌측 바 | 3px (`SAGE_SELECTION_ACCENT_WIDTH`) | 사이드바 선택 항목 | 170, 302, 812 |
| 상태 배지 pill | radius 4 · padding 0 8 · 높이 20 | 배지 열 | 307 |
| 아이콘 크기 · 선 굵기 | 16px · 1.5px (400줄) / 2px (777줄) — 모순 | 아이콘 | 400, 777 |
| 아이콘 단독 버튼 | 32×32 (`SAGE_ICON_BUTTON_SIZE` 32) | 아이콘 버튼 | 400, 767, 779 |
| 아이콘 + 텍스트 간격 | 6 (`SAGE_ICON_TEXT_GAP`) | 아이콘 동반 버튼 | 780 |
| 아이콘 십자 | span 10 / 두께 2 | 추가(+) 아이콘 | 778 |
| 검색 박스 우측 칸 | 32px | `CSageSearchBox` | 528 |
| 에디트 테두리 | 1px | 에디트 | 534, 714 |
| 에디트 텍스트 top 패딩 | 7 (`SAGE_EDIT_TEXT_TOP_PAD`) — 코드는 9 | 멀티라인 에디트 | 639, 720 |
| 에디트 높이 | 32 (`SAGE_EDIT_HEIGHT`) | 입력 에디트 | 721 |
| 레이블-에디트 간격 | 4 (`SAGE_LABEL_EDIT_GAP`) | 폼 행 | 722 |
| 글줄 높이 어림(틀린 값) | 20 | 단일행 에디트 — 쓰지 말 것 | 642 |
| 다이얼로그 인라인 오류 추가 시 일괄 가산(틀린 방식) | `+30` | 다이얼로그 7종 — 쓰지 말 것 | 734 |
| 메시지 박스 아이콘 | 22px 원 + 2px 획 | `ShowSageMessageBox` | 756 |
| 버튼 높이 | 32 (`SAGE_BUTTON_HEIGHT`) | 모든 버튼 | 767 |
| 카드 안 액션 행 버튼 높이 | 34 (`SAGE_CARD_ACTION_BUTTON_HEIGHT`, 목업 3-4 실측) | 카드 액션 행 | 768, 852 |
| 버튼 텍스트 세로 오프셋 | 2 (Gmarket 기준, `SAGE_BUTTON_TEXT_TOP_OFFSET`) — 코드는 0 | 버튼 텍스트 | 771-772 |
| 헤더 높이 · 하단선 | 56 · 1px | 헤더(제목 · 인증) | 790 |
| 탭 줄 높이 · 하단선 | 40 · 1px | 탭 줄 | 791 |
| 탭 인디케이터 | 2px (`SAGE_TAB_INDICATOR_HEIGHT`) | 선택 탭 하단 | 797 |
| 상태 카드 높이 | 70 고정 | `CSageStatusCard` | 819 |
| 진행바 높이 | 6px | 상태 카드 처리 중 | 825 |
| 상태 카드 아이콘 | 18px (체크 · 느낌표) | 완료 · 실패 | 826-827 |
| 크기 단위 | 96dpi 기준 논리 픽셀 | 전역 | 850 |
| 간격 규칙 | 4의 배수만 | 전역 | 851 |
| 컨트롤 기본 높이 | 32 (목업 30이어도 32) | 전역 | 852-853 |
| 폼 라벨 폭 | 64 단일 / 긴 라벨 폼 96 (「변경할 비밀번호」 88px) | 폼 라벨 (본문 · 다이얼로그 공통) | 855 |
| 옛 라벨 폭(폐기된 값) | 80 (실측 라벨 52px, 28px 뜸, 40px 떨어져 보임) | 이력 | 858-861 |
| 측정용 GDI 폰트 높이 | `lfHeight=-14` | 라벨 폭 실측 방법 | 864 |
| 목업 타입 스케일(쓰지 않음) | 18/14/13/12/11px, 기존 앱 14.7px | 이력 | 227-228 |
| 표 빈 격자 예 | 40행 | 금지 예 | 326 |

- 개수: **34항목**
- 스킬에 **없는** 레이아웃 값(코드에는 있음, 참고): `SAGE_LIST_HEADER_HEIGHT = 36`, `SAGE_CARD_HEADER_HEIGHT = 38`, `SAGE_RESULT_HEADER_HEIGHT = 26`, `SAGE_STATUS_CARD_*`(dot 8 · gap 8 · icon gap 12 · 제목 줄 20 · 상세 줄 16 · 액션 폭 80 · 액션 gap 12), 카드 padding · 행 간격(`SAGE_CARD_PADDING` · `SAGE_CARD_ROW_GAP` 값 미확인), 사이드바 폭 · 사이드바 항목 높이 · 패널 여백 · 버튼 좌우 padding · 버튼 최소 폭 · radius(버튼 · 에디트 · 카드) — **스킬에 규격 없음**

## 4. 컨트롤 규격 (variant 후보)

### 4.1 버튼 (`QPushButton` + variant `Q_PROPERTY`)

| 변형 | normal 면 / 테두리 / 글자 / 굵기 | pressed | disabled | hover | focus | 용도 |
|---|---|---|---|---|---|---|
| Primary | 카멜 `#9A6B3F` / 카멜 / 흰색 / Bold | `#76502A`(카멜 press — 면에 쓰는지 명시 없음, 158줄 역할명으로 추정 **확인 못함**) | 스킬에 색 없음 (521줄 「disabled · pressed」 상태 존재만) | 스킬에 없음 | 스킬에 없음 | 생성 · 실행 · 저장 · 확인 |
| Secondary | 흰 `#FFFFFF` / `#C9BFB1` / 본문 `#2F2A24` / Regular | 없음 | 없음 | 없음 | 없음 | 파일 선택 · 폴더 선택 · 취소 · 수정 · 「폴더 열기」 · 메시지 박스 「아니오」 |
| Ghost | 투명 / 없음 / `#6E655B` / Regular | 없음 | 없음 | 없음 | 없음 | 초기화 · 선택 해제 |
| Danger | 흰 / `#E0BDB6` / 오류 `#B85C4A` / Bold | 없음 | 없음 | 없음 | 없음 | 삭제 전용 · 메시지 박스 「예」 |

- 크기: 높이 32, 카드 안 액션 행 34 (크기 variant 또는 속성 후보). 아이콘 단독 32×32 + 툴팁 필수. 아이콘 + 텍스트 간격 6, 묶음 가운데
- 아이콘 속성: `SageButtonIcon` enum (스킬은 목록을 적지 않음. 코드: `NONE · SEARCH · RESET · ADD · CLOSE · MOVE_UP · MOVE_DOWN`)
- Ghost도 클릭 영역 · 높이는 다른 변형과 같음

### 4.2 입력칸 (`QLineEdit` + 상태 `Q_PROPERTY`)

| 상태 | 테두리 | 글자 | 비고 |
|---|---|---|---|
| normal | 1px `#DCD6CD` | 본문 | 높이 32 |
| error | 1px `#B85C4A` (`SAGE_EDIT_ERROR`) | — | 어느 입력이 틀렸는지 테두리가 알림 |
| warning | `#B88746`는 「에디트 테두리에만」(150줄) — 경고 상태 에디트가 있는지는 **확인 못함** | | |
| readonly | 입력 경로 · 저장 위치(`ES_READONLY`) — 배경 색 규격 없음 | | |
| focus / hover / disabled | **스킬에 규격 없음** | | |

### 4.3 표 (`QTableView` + `QHeaderView` + delegate)

| 요소 | 상태 | 규격 |
|---|---|---|
| 헤더 | normal | 면 `#F2EEE7`, 글자 `#6E655B`, 세로 구분선 없음, 항상 가운데 (hover · pressed · 정렬 표시 규격 없음) |
| 행 | normal | 흰 면, 하단 1px `#EDE8E0`, 34 고정, 13px |
| 행 | alternate | `SAGE_COLOR_LIST_ROW_ALT` (값 스킬에 없음) |
| 행 | selected | 면 `#F1E3CD` + 좌측 4px 카멜 바 |
| 행 | 상태(성공/실패 등) | `SetRowStyle` 번호별 배경 · 배지 색, 교대 행보다 우선 (값 없음, 실패 행 `#FDF6F4`만 언급) |
| 셀 | 강조 열 | 한 열만 색 텍스트 + Bold (색 값 명시 없음) |
| 셀 | 빈 금액 | `—` + `#B4ABA0` |
| 셀 | 배지 열 | pill radius 4 · padding 0 8 · 높이 20 · 캡션 폰트 |
| focus | — | 규격 없음 |

### 4.4 목록 (`CSageListBox`) — 근거 화면 삭제
- 선택 면 `#F1E3CD` + 텍스트 `#76502A` SemiBold, 교대 행, 하단 hairline, 1px 테두리 (302, 524)

### 4.5 탭 (`QTabBar`, SageStyle)

| 상태 | 면 | 글자 | 인디케이터 |
|---|---|---|---|
| selected | `#FFFFFF` | Bold + `#2F2A24` | 하단 2px `#9A6B3F` |
| normal | `#FFFFFF` | Regular(명시 없음 — Bold가 아님) + `#7A7064` | 없음 |
| hover / focus / disabled | 규격 없음 | | |
- 줄 높이 40, 줄 하단 1px `#DCD6CD`

### 4.6 사이드바 (`QTreeView` + delegate)

| 상태 | 면 | 글자 | 기타 |
|---|---|---|---|
| normal | `#241F1A` | `RGB(205, 196, 185)` | 스크롤바 비표시 |
| selected | `RGB(58, 49, 41)` | 스킬에 선택 글자색 없음 | 좌측 3px 카멜 바, 밝은 카멜 채움 금지 |
| 그룹 헤더 · hover · focus | 규격 없음 | | |
- 로고: Gmarket Sans TTF Bold (사이드바 상단)

### 4.7 상태 카드 (커스텀 위젯, 상태 `Q_PROPERTY`: Idle / Running / Completed / Failed)
- 높이 70 고정
- 대기: 흰 / `#DCD6CD`, dot `#B4ABA0` + 안내 한 줄
- 처리 중: 흰 / `#DCD6CD`, dot `#B88746` + 문구 + 우측 `%` + 진행바 6px(트랙 `#EDE8E0` / 채움 `#9A6B3F`)
- 완료: `#F1F5F0` / `#D5E0D3`, 체크 18px `#5F7F5F`, 제목 `#41603F`, 경로 `#6E655B`, 「폴더 열기」 Secondary (저장 경로 있을 때만)
- 실패: `#FDF6F4` / `#E0BDB6`, 느낌표 18px `#B85C4A`, 제목 `#9C4433`, 사유

### 4.8 인라인 메시지 (`QLabel` + variant: Error / Warning)
- Error: 글자 `#9C4433`, 12px, 1줄
- Warning: 글자 `#8A6A32`, 박스 `#FBF5EE` / `#EBDCC6`
- 값이 비면 숨김(532) vs 자리는 항상 비워 둠(743)

### 4.9 배지 (커스텀 위젯, 면 · 테두리 · 글자색을 인자로)
- pill, 표 배지 규격: radius 4 · padding 0 8 · 높이 20 · 캡션 폰트
- 오류 배지 면 `#F8EBE9`. 헤더 「관리자」 배지 색은 **스킬에 없음**

### 4.10 요약 바 · 합계 밴드 · 선택 바 · 필 바 · 검색 박스 · 빈 상태 (커스텀 또는 조합)
- 요약 바: `라벨 + 강조 수치 + 단위` N개, 라벨 · 단위 `#7A7064`, 수치 17px SemiBold(미도입 표기), 흰 박스 금지, 폭은 표 폭 파생
- 합계 밴드: 면 `#F2EEE7`, 「합계」 `#6E655B` Bold, 건수 `#7A7064` SemiBold, 표와 같은 정렬(가운데)
- 선택 바: `N건 중 M건 선택됨` `#7A7064` + 액션 묶음, 흰 박스 금지, 폭은 표 폭 파생
- 필 바: pill `#F7F2EA`, `전체 12` · `성공 11` … 선택 상태 모양 **스킬에 없음**
- 검색 박스: 테두리 하나, 우측 32px 칸 · 구분선 · 아이보리(값 없음 — `#F8F6F1`인지 `#F7F2EA`인지 확인 못함)
- 빈 상태: 안내문(`#7A7064`) + 1차 액션 버튼, 아이콘 박스 `#F2EEE7`, 표 영역 중앙
- 섹션 제목 라벨: 15px Bold
- 콤보: 드롭다운 화살표만 규격(형태 값 없음)

- 개수: 컨트롤 종류 **10묶음** (버튼 · 입력칸 · 표 · 목록 · 탭 · 사이드바 · 상태 카드 · 인라인 메시지 · 배지 · 기타 6종 묶음) — 개별로 세면 18종

## 5. 폰트 규칙

| 역할 | 서체(원문) | SageQt 지정(T02) | 크기(원문) | `CreatePointFont` | 상수 |
|---|---|---|---|---|---|
| 본문 · 라벨 · 버튼 · 표 | Pretendard | `Pretendard` + Regular | 14px | `105` | `SAGE_CONTROL_FONT_FACE`, `SAGE_CONTENT_FONT_POINT_SIZE` · `SAGE_CONTROL_FONT_POINT_SIZE` |
| 화면 제목 | Pretendard SemiBold(208) / **Bold**(220) | `Pretendard` + ? | 19px / Bold | `143` | `SAGE_TITLE_FONT_FACE`, `SAGE_TITLE_FONT_POINT_SIZE` |
| 섹션 제목 | Pretendard SemiBold(208) / **Bold**(221) | `Pretendard` + ? | 15px / Bold | `113` | `SAGE_HEADER_FONT_POINT_SIZE` |
| 표 셀 | Pretendard | `Pretendard` + Regular | 13px | `98` | `SAGE_LIST_FONT_POINT_SIZE` |
| 캡션 · 인라인 메시지 | Pretendard | `Pretendard` + Regular | 12px | `90` | `SAGE_CAPTION_FONT_POINT_SIZE` |
| 요약 수치 | Pretendard SemiBold | `Pretendard` + DemiBold | 17px / SemiBold | `128` | 미도입 (D5c) |
| 로고 | Gmarket Sans TTF Bold | `Gmarket Sans TTF` + Bold | 스킬에 없음 (T02 측정: 19px) | — | `SAGE_LOGO_FONT_FACE` |

- 굵기 사용처: Primary · Danger 버튼 Bold, 선택 탭 Bold, 강조 열 Bold, 합계 밴드 라벨 Bold · 보조 수치 SemiBold, 목록 선택 SemiBold
- 기타: 목업 스케일 18/14/13/12/11px은 쓰지 않음(의도된 이탈, 기존 앱 14.7px). 옛 값 9.8pt · 9pt · 8.3pt는 낡은 값
- T02 적용: 모든 크기는 **픽셀 크기**(원문 px 값 그대로)로 — 포인트 지정 시 macOS에서 약 25% 작게 그려짐. 픽셀 지정 시 3 OS 폭 차이 ±4% 안 (한글 -2.1 ~ +3.6%, 영문 · 숫자 -3.4 ~ +2.8%)
- 모순: 208줄은 제목 · 섹션 제목 서체를 SemiBold로, 220-221줄은 Bold로 적음 → 7장

## 6. 텍스트 폭 · 고정 픽셀 의존

| 줄 | 위치 | 가정 | 영향 |
|---|---|---|---|
| 855 | 폼 라벨 폭 64 / 96 | 「변경할 비밀번호」 88px (Windows GDI 실측), 「입력 파일」·「저장 위치」 52px | 라벨 폭 고정. 픽셀 지정으로 ±4%라도 64 · 96 칸 안 여유(64-52=12, 96-88=8)가 OS별로 달라짐. layout이면 불필요 |
| 858-861 | 옛 라벨 폭 80 | 목업 CSS `width:80px` | 폐기된 값 — 고정 폭이 텍스트와 어긋난 실제 사례 |
| 863-866 | 라벨 폭 실측 방법 | GDI `GetTextExtentPoint32W` (`lfHeight=-14`) | 측정이 Windows 기준 |
| 294, 310 | 표 행 높이 34 | 13px 표 셀 텍스트가 34 안에 들어감 | 줄 높이는 macOS 픽셀 지정에서도 -13.7% ~ +4.4%(로고 제외 -1.5 ~ +4.4%). 34 고정은 여유 있으나 2줄 불가(312) |
| 307 | 배지 pill 높이 20, padding 0 8 | 12px 캡션 1줄이 20 안에 | pill 폭 = 텍스트 폭 + 16 → QFontMetrics 필요 |
| 721, 767 | 에디트 · 버튼 높이 32 | 14px 본문이 32 안에 | 고정 높이 |
| 768, 852 | 카드 액션 버튼 34 | 같음 | |
| 790 | 헤더 56 | 19px 제목 · 32 버튼 · 배지 · 사용자 라벨 | 코드 대조: `SAGE_USER_LABEL_WIDTH`로 사용자 라벨 폭 고정(값 미확인) — 스킬에는 없음 |
| 791 | 탭 줄 40 | 14px 탭 텍스트 | 탭 폭은 스킬에 없음 |
| 797 | 선택 탭 Bold | 선택/비선택 굵기가 달라 탭 텍스트 폭이 바뀜 | 탭 폭이 굵기로 흔들리는지 확인 필요 |
| 819-820 | 상태 카드 70 고정 | 제목 줄 + 상세 줄 + 진행바(코드: 20 · 16 · 6) | 줄 높이 OS 차이 |
| 825 | 우측 `%` | 폭 가정 명시 없음 | 코드 대조: 액션 칸 `SAGE_STATUS_CARD_ACTION_WIDTH = 80` 고정 — 「폴더 열기」 버튼 폭 80 고정 |
| 528 | 검색 박스 우측 32px 칸 | 아이콘 칸 | 텍스트 아님 — 영향 작음 |
| 642 | 글줄 높이 20 어림 | 틀린 가정(기록) | `QFontMetrics`로 대체 |
| 771-772 | 버튼 텍스트 오프셋 2 | 서체 ascent 보정 | Qt에서는 불필요 |
| 738 | 다이얼로그 폭 상수 | 콘텐츠가 정하지 않는다고 봄 | 라벨 · 입력 · 버튼 폭 합이 폭 상수를 넘으면 잘림 — 최소 폭으로 바꿀지 |
| 317-320, 870-871 | 컬럼 정의 폭(픽셀) · 비율 배분 | 열 헤더 · 값 폭을 픽셀로 정의 | core 픽셀 금지 — `QHeaderView` 모드로 |
| 780 | 아이콘 + 텍스트 묶음 가운데 | 텍스트 폭 필요 | `QFontMetrics` |
| 284 | 긴 경로 말줄임 | 가용 폭 기준 | `QFontMetrics::elidedText` / view 기본 |
| 228 | 기존 앱 14.7px 대비 | 크기 인상 비교 | T02 픽셀 지정이면 3 OS 유지 |

- 코드 대조(참고, 스킬 밖): 버튼 폭 `SAGE_BUTTON_WIDTH`, `SAGE_INPUT_RESET_WIDTH` 고정 폭 버튼이 입력 패널에 있음 (`SageWorkflowInputPanel.cpp:199, 202`) — 버튼 텍스트 폭 의존

## 7. 근거가 삭제된 화면인 규칙

| 줄 | 규칙 | 근거 | 규칙이 아직 필요한가 |
|---|---|---|---|
| 145-147 | 카드 안 합계 박스 = `#F7F2EA` | 목업 3-2 카드 합계 박스(현재 컨트롤 · 화면 없음) | 표 합계 밴드 쪽만 필요. 카드 합계 박스는 화면이 생길 때 |
| 167-169, 575, 583-585, 590-593 | Primary는 **카드당** 1개 (카드 둘이면 둘) | 목업 3-6 데이터 관리 화면 두 카드(D7-6) — 없음 | 필요 여부 결정 필요. 현재 화면만 보면 「화면당 1개」로 충분 (**변경**) |
| 302 | `CSageListBox` 선택 행 `#76502A` + SemiBold | 목록 선택 다이얼로그 — 없음 | 현재 불필요 |
| 404-408 | 아이콘 8종(`MOVE_UP` · `MOVE_DOWN`) | 목업 3-6 순서 이동(D7-6) — 없음 | 아이콘 enum은 쓰는 것만 |
| 524 | `CSageListBox` 컨트롤 전체 | 목록 선택 다이얼로그 — 없음 | 현재 불필요 |
| 540-549 | `CSageEdit` 다이얼로그 7종 적용 / 패널·View 16곳 보류 | 다이얼로그 7종(현재 3), 16곳(현재 2) | 불필요 — SageStyle 하나 |
| 603-605 | 버튼 그리기 358줄 복제(View 1 · 다이얼로그 7) | 다이얼로그 7종 | 불필요 (원칙은 600-601에 있음) |
| 631-633 | `CHeaderCtrl` `fmt` 금지 | 데이터 관리 표 이름 컬럼(D7-6) — 없음 + Win32 | 불필요 |
| 625-629 | `SS_NOTIFY` ID 분리 | `CSageSearchBox` 버그(D7-6) — 검색 박스는 존재하나 규칙은 Win32 전용 | 불필요 |
| 642-643 | 글줄 높이 20 어림 사례 | D7-6 | 사례는 불필요, 「실측」 원칙은 필요 |
| 733-736 | 다이얼로그 7종 `+30` 사례 | D5a 7종 | 사례 불필요, 원칙 필요 |
| 740-741 | 늘어나는 자식 창은 고정 높이 | 목록 선택 다이얼로그 — 없음 | 현재 불필요 |
| 842-843 | 빈 상태 예문 「등록된 항목이 없습니다 / 대상을 선택한 뒤 항목을 추가하세요」 | 데이터 관리 화면 문구로 보임 — **확인 못함** | 형식 규칙은 필요 |
| 897-898 | 체크리스트 Primary 개수 | 카드 단위 규칙과 같은 근거 | 결정 후 한쪽으로 |

- SageTaechang 언급: 스킬 본문에 **없음**. `DEBT_LOG` 언급: 726, 835 (SageSDI 문서)
- D7-4(상태 카드 · 카드 액션 34 · 라벨 64) · D7-5(사유 열 · 컬럼 비율 · 필 바) · D7-1(합계 밴드 색) · D5a/b/c · D9 · 3-B-5b(관리자 배지)의 근거 화면은 **현재 존재**한다 — 삭제 목록에서 뺐다

## 8. 질문 / 확인 못함

1. **Primary 개수 기준** — 575줄(카드당 1개, 근거는 삭제된 목업 3-6) vs 766 · 897줄(화면당 1개). SageQt `sageqt-ui`에 어느 쪽을 쓸지
2. **아이콘 선 굵기** — 400줄 1.5px vs 777줄 2px(GDI 근거). `QPainter` 안티앨리어싱에서 값을 무엇으로 할지
3. **제목 · 섹션 제목 굵기** — 208줄 Pretendard SemiBold vs 220-221줄 Bold
4. **사이드바/표 선택 바 상수** — 812줄 「같은 상수 공유」는 302줄 · 코드(3 / 4 별 상수)와 모순. 낡은 문장으로 보고 3 / 4 별도로 가도 되는지
5. **스킬 값 vs 코드 값 불일치** — `SAGE_EDIT_TEXT_TOP_PAD` 스킬 7 / 코드 9, `SAGE_BUTTON_TEXT_TOP_OFFSET` 스킬 2 / 코드 0. 둘 다 SageQt에서 폐기 예정이라 영향은 없으나, 스킬 수치를 원본으로 믿을 수 없는 사례 — 다른 값도 코드와 대조할지
6. **값이 없는 규격** — `SAGE_COLOR_LIST_ROW_ALT`(코드 `#FAF8F4`), 행 상태 색(`SetRowStyle` 성공/실패 값), 강조 열 색, 헤더 「관리자」 배지 색, 사이드바 선택 글자색, 검색 박스 「아이보리」 값, 필 바 선택 상태, 로고 크기. 코드에서 옮길지, 목업을 실측할지
7. **없는 상태 규격** — 버튼 hover · focus · disabled 색, 에디트 focus · hover · disabled · readonly, 탭 hover · focus, 표 헤더 hover/정렬 표시, 표 focus, 포커스 표시 전반(394줄은 「명확해야 한다」만). Fusion 기본을 쓸지, 새로 정할지
8. **radius** — 배지 radius 4 외에 버튼 · 에디트 · 카드 · 상태 카드 radius가 스킬에 없음 (확인 못함)
9. **요약 수치 17px** — 225줄 「미도입 — D5c」인데 526줄 `CSageSummaryBar`는 「구현됨 (D5c)」. 요약 수치 폰트가 실제로 쓰이는지 확인 못함
10. **컬럼 폭 비율 stretch** — stretch 열이 둘 이상일 때 정의 폭 비율로 나누는 규칙(317-320)은 `QHeaderView::Stretch`(균등)로 안 됨. 비율을 버릴지, `resizeSection` 계산(패널 · 헤더 서브클래스)을 허용할지 — 후자는 `model-view.md`(패널이 열 폭 계산 금지)와 충돌
11. **합계 밴드 정렬** — `CSageTableTotalBar`가 표 열과 같은 정렬(338)을 하려면 열 경계를 알아야 함. 별도 위젯 + 헤더 추종인지, 표의 마지막 고정 행인지
12. **역할별 폰트 적용 주체** — 앱 폰트는 1개(main.cpp). 표 셀 13px · 캡션 12px · 제목 19px · 섹션 15px를 누가 적용하는가(SageStyle의 `polish`에서 위젯 variant를 보고 `setFont`? 커스텀 위젯 자체?) — `style.md`는 위젯 · 화면의 `setFont` 값 지정 금지만 말함. 본문 · 컨트롤 두 상수(231)를 하나로 합칠지도
13. **기본 위젯 variant 붙이는 방법** — `Q_PROPERTY`는 클래스 선언이 필요하므로 `QPushButton`에 variant를 주려면 서브클래스가 필요해 보임 → 「기본 위젯 서브클래싱 금지」와 충돌. 동적 property(`setProperty`) 허용인지, variant만 가진 얇은 서브클래스 허용인지 (확인 못함 — `style.md` 29 · 32줄 해석 문제)
14. **프레임리스 다이얼로그 · 캡션 밴드** — 745-751줄 캡션 밴드 `#F2EEE7`는 `SageFramelessDialog` 전제. SageQt가 프레임리스를 유지하는지(macOS 네이티브 타이틀바와의 관계) 확인 못함
15. **메시지 박스** — `ShowSageMessageBox` 대체를 `QMessageBox` + SageStyle로 할지 전용 다이얼로그로 할지. `MB_YESNO` = 삭제 확인이라는 사실(757)이 SageQt 화면에도 맞는지, main.cpp 초기화 실패 알림(760)을 T06에서 어떻게 했는지 확인 못함
16. **폼 라벨 폭** — 64 / 96 고정을 버리고 `QFormLayout` 라벨 `sizeHint`로 갈지(텍스트 폭 정책 결정 대상). 「앱의 모든 폼 라벨 폭이 하나」(861)라는 일관성 의도를 layout에서 어떻게 지킬지
17. **고정 높이(32 · 34 · 40 · 56 · 70)** — 고정 값 유지(`setFixedHeight`/`sizeFromContents`)인지, 최소 높이인지
18. **인라인 오류 숨김 vs 자리 유지** — 532줄(값이 비면 숨김)과 743줄(자리 항상 비워 둠)
19. **키보드 순서** — 패널 경계를 넘는 탭 순서(385-392)를 어떻게 보장할지
20. **사이드바 스크롤바 비표시**(815) — 업무가 많아 넘칠 때의 동작 규격 없음
21. **767-768줄 참조 방향** — 「위 *크기와 좌표* 참조」인데 해당 절은 아래(849줄). 문서 오기로 봄
22. **SageQt 폴더명** — `app/ui/drawing/` 대응 폴더(`ui/widgets/`?)는 `coding-design`에서 확인 못함 (이 분석에서 읽지 않은 파일)

---

## 판정 집계

1장 표 행 기준 — **유지 133 · 변경 111 · 폐기 34** (합계 278행)
