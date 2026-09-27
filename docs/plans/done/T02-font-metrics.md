# T02 — 폰트 · 텍스트 메트릭 3-OS 측정

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
SageSDI의 번들 폰트가 세 OS의 Qt에서 **어떤 이름으로 잡히는지**, 같은 한글 문자열이 **몇 픽셀로 그려지는지**를 실측한다. 이 수치가 T07(`sageqt-ui`)의 레이아웃 정책 — 고정 픽셀을 쓸지, 폰트에서 치수를 끌어낼지 — 의 근거가 된다.

## 시작 전에
1. 선행 주제: 없음 (T01 완료 — 정적 분석이 걸린 CI 위에서 작업한다)
2. 스킬 로드: `git-workflow`, `coding-rules`, `coding-design`
   - 읽을 reference: `coding-design/references/cmake-targets.md`, `coding-rules/references/values-and-platform.md`, `coding-rules/references/naming.md`
3. 결정 — 착수 시 사용자와 확정
   - 측정 도구 위치: 권장 `tools/font-probe/` (앱이 아닌 측정 도구라 `SageQt/` 소스 루트에 두지 않는다). **`coding-design`의 폴더 구조에 `tools/`가 없으므로 규칙 추가가 필요하다** → 규칙 커밋 분리, `sageqt-plan` 재점검
   - 측정 도구를 평소 빌드에 포함할지: 권장 — CI 측정 job에서만 빌드
   - Gmarket Sans의 배포 라이선스 확인 전까지 저장소에 넣을지 (아래 *함정*)
4. 재확인할 사실
   - Qt 6.11의 Windows 기본 폰트 엔진 (DirectWrite인지 GDI인지 — Qt 문서로 확인)
   - Gmarket Sans Light · Medium이 SageSDI에서 굵기 지정으로 쓰이는지 (`CreateFont` · `lfWeight` 검색)

## SageSDI에서 옮길 것
**폰트 파일** (`D:/Projects/SageSDI/SageSDI/resources/`)
| 파일 | 크기(바이트) |
|---|---|
| `PretendardRegular.ttf` | 2,725,828 |
| `PretendardSemiBold.ttf` | 2,671,468 |
| `PretendardBold.ttf` | 2,661,752 |
| `GmarketSansTTFBold.ttf` | 2,511,976 |
| `GmarketSansTTFLight.ttf` | 2,413,604 |
| `GmarketSansTTFMedium.ttf` | 2,417,020 |
| `PretendardLicense.txt` | 4,513 |

**폰트 이름** (`SageDefine.h:245-247`) — Windows GDI 방식 패밀리 이름이다
- `SAGE_TITLE_FONT_FACE = L"Pretendard SemiBold"`
- `SAGE_CONTROL_FONT_FACE = L"Pretendard"`
- `SAGE_LOGO_FONT_FACE = L"Gmarket Sans TTF Bold"`

**폰트 크기** (`SageDefine.h:150-156`) — **0.1포인트 단위**다 (`CreatePointFont` 인자). 143은 14.3pt
| 상수 | 값 | 포인트 |
|---|---|---|
| `SAGE_TITLE_FONT_POINT_SIZE` | 143 | 14.3 |
| `SAGE_CONTROL_FONT_POINT_SIZE` | 105 | 10.5 |
| `SAGE_CONTENT_FONT_POINT_SIZE` | 105 | 10.5 |
| `SAGE_HEADER_FONT_POINT_SIZE` | 113 | 11.3 |
| `SAGE_LIST_FONT_POINT_SIZE` | 98 | 9.8 |
| `SAGE_CAPTION_FONT_POINT_SIZE` | 90 | 9.0 |
| `SAGE_SUMMARY_FONT_POINT_SIZE` | 128 | 12.8 |

**로딩 방식** — `SageSDI.cpp:134-154`: `.rc`의 `TTF` 리소스 6개를 `LoadResource` → `AddFontMemResourceEx`로 모두 로드한다

**측정할 문자열** (UI에 실제로 쓰이는 것에서 고른다, `SageDefine.h`)
- `"샘플 업무"` (`SAGE_UI_SAMPLE_NAME`), `"실행 기록"` (`SAGE_UI_TAB_HISTORY`), `"입력 파일"` (`SAGE_UI_SECTION_INPUT`)
- `"검색어 입력"` (`SAGE_UI_RESULT_FILTER_PLACEHOLDER`), `"초기화"` (`SAGE_UI_RESULT_RESET_BTN`)
- `"샘플 업무가 완료되었습니다."` (`SAGE_UI_SAMPLE_COMPLETED`)
- 비교용: `"Result"` · `"1,234,567"` · 영문 대소문자 한 줄

## 옮기지 않는 것
- `.rc` 리소스로 폰트를 넣는 방식 — Qt에서는 리소스(qrc)를 실행 파일 타깃에 등록한다 (T08). 이 주제에서는 측정 도구가 파일 경로로 직접 읽어도 된다

## 함정
- **폰트 패밀리 이름이 OS · 폰트 엔진마다 다를 수 있다.** GDI는 레거시 이름(`"Pretendard SemiBold"`)을, CoreText · fontconfig · DirectWrite는 타이포그래픽 이름(`"Pretendard"` + 스타일 `SemiBold`)을 쓸 수 있다. `QFontDatabase::addApplicationFont`가 돌려주는 패밀리 이름과 `QFontDatabase::styles()`를 OS별로 기록한다
- **포인트 크기를 0.1pt 단위에서 변환해 쓴다.** 143을 그대로 `setPointSize(143)`에 넣지 않는다
- **offscreen 플랫폼의 측정값이 실제 화면과 다를 수 있다.** CI 러너에는 화면이 없어 `QT_QPA_PLATFORM=offscreen`이 필요하다. Windows 로컬에서는 실제 플랫폼과 offscreen을 둘 다 재서 차이를 기록한다. macOS 실제 화면 측정은 Mac mini가 생긴 뒤(T08)
- **Gmarket Sans는 라이선스 파일이 SageSDI에 없다.** Pretendard는 OFL 파일이 있다. Gmarket Sans의 배포 조건을 확인하기 전에는 저장소에 넣지 않거나, 넣더라도 T17에서 고지를 정리한다
- 폰트 파일은 바이너리다 — `.gitattributes`(`* text=auto eol=lf`)가 내용으로 판별하므로 줄 끝 변환은 일어나지 않는다. 추가 후 `git ls-files --eol`로 `-text`인지 확인한다

## 작업
PR 1개: `chore/font-metrics-probe` (규칙 추가가 필요하면 `docs:` 커밋 분리)
- [x] `coding-design` 폴더 구조에 측정 도구 위치 추가 (결정에 따라)
- [x] 측정 도구: 폰트를 등록하고, 등록된 패밀리 · 스타일 이름과 위 문자열의 폭 · 높이 · 기준선 · DPI를 JSON으로 출력
- [x] CI 측정 job: 3 OS에서 실행 → JSON을 산출물로 업로드
- [x] Windows 로컬: 실제 플랫폼과 offscreen 결과 비교
- [x] 결과표 작성 → 이 파일의 *확인한 사실*과 `MIGRATION_PLAN.md`의 확정 사실에 기록
- [x] T07이 쓸 레이아웃 정책 근거 정리 (예: 같은 문자열의 OS 간 폭 차이가 몇 %인지)

## 완료 기준
- 3 OS의 측정 JSON이 CI 산출물로 남아 있다
- OS별 등록 패밀리 이름 표와, 문자열별 폭 차이(%) 표가 `MIGRATION_PLAN.md`에 있다
- Windows에서 실제 플랫폼과 offscreen의 차이가 기록되어 있다

## 범위 밖
- macOS 실제 화면 측정 — Mac mini가 필요하다 (T08)
- 레이아웃 정책 결정 자체 — T07에서 이 결과를 근거로 정한다

## 확인한 사실
- 결과표는 `MIGRATION_PLAN.md` *폰트 · 텍스트 메트릭*에 있다. CI 실행 36300491819 (산출물 5개)
- Qt 6.11 Windows 폰트 엔진: Qt 문서는 `fontengine=gdi`를 "legacy GDI-based font database를 쓴다 (Since Qt 6.8)"라고 설명한다 → 기본값은 DirectWrite로 **추론**. 공개 API로 엔진을 알 수 없어 측정 도구는 기록하지 않는다
- SageSDI가 쓰는 폰트: Pretendard Regular · SemiBold · Bold(`SageUiResources.cpp:52`의 `FW_BOLD` → `SAGE_FONT_LIST_BOLD` 2곳), Gmarket Sans Bold(`SAGE_FONT_LOGO` 1곳). Gmarket Light · Medium은 쓰는 곳 0곳 — 사용자 결정으로 6개 모두 저장소에 넣었다
- Gmarket Sans는 SIL OFL이다 ([공식 페이지](https://corp.gmarket.com/fonts/)). 폰트 name ID 13에 저작권 문구와 OFL 전문이 들어 있어 `GmarketSansLicense.txt`로 꺼냈다
- 폰트 name 테이블: Pretendard SemiBold는 레거시 패밀리 `Pretendard SemiBold`(스타일 Regular) / 타이포그래픽 `Pretendard`(SemiBold). Gmarket은 `Gmarket Sans TTF`(Bold)
- **macOS에서 상대 경로로 `addApplicationFont`를 부르면 실패한다** (offscreen · cocoa 모두). `QDir::absoluteFilePath`로 해결 — T08 폰트 등록은 qrc라 해당하지 않지만, 파일 경로로 폰트를 읽는 코드는 절대 경로를 쓴다
- GitHub macOS 러너에서 `cocoa` 플랫폼이 실행된다 (논리 DPI 72, 물리 DPI 44). Windows 러너에서 `windows` 플랫폼이 실행된다
- Windows 10(로컬)과 Windows Server 2025(CI)의 측정값은 99개 모두 같다
- Windows 실제 플랫폼과 offscreen: 한글 폭은 최대 0.5% 차이, 영문 · 숫자는 최대 12.5% 차이 (offscreen이 넓다)
- macOS 실제 플랫폼은 논리 DPI 72 → 포인트 크기로는 약 25% 작다. 픽셀 크기(포인트 × 96 / 72)로는 Windows와 한글 -2.1 ~ +3.6%, 영문 · 숫자 -3.4 ~ +2.8%
- `QFontMetricsF`가 Windows에서는 정수, macOS · Linux에서는 소수를 돌려준다

## 결과
- PR [#10](https://github.com/JakeKim4926/SageQt/pull/10) `chore/font-metrics-probe`
- `tools/` 규칙과 `SAGE_BUILD_TOOLS` 옵션, 번들 폰트 6개 + OFL 라이선스 2개, 측정 도구 `SageFontProbe`, 5개 환경 측정 워크플로(`font-metrics.yml`)
- 계획보다 넓힌 것: offscreen만으로는 실제 화면을 대표하지 못해 Windows · macOS 실제 플랫폼을 추가했고(사용자 승인 없이 측정 범위 판단 — 보고함), 포인트/픽셀 판단 근거를 위해 픽셀 크기 버전 측정을 추가했다 (사용자 승인)
- T07에 넘기는 근거: (1) 폰트는 패밀리 + 굵기로 지정한다 — GDI식 이름은 환경마다 다른 폰트로 잡힌다 (2) 포인트 크기는 macOS에서 25% 작다 — 픽셀 크기로 지정하면 세 OS 폭 차이가 ±4% 안이다 (3) 영문 · 숫자는 한글보다 OS 간 차이가 크다 (4) Gmarket 로고의 줄 높이는 macOS에서 14% 낮다
- 교훈
  - 요약 스크립트가 일부 항목만 보면 결론이 틀린다 — 로컬 비교를 문자열 2개로만 보고 "거의 같다"고 보고했다가 정정했다. 비교는 전체 항목으로 한다
  - 실패 원인을 플랫폼으로 단정했다가 틀렸다 (실제로는 상대 경로). 가설을 바꾸기 전에 오류 조건을 하나씩 줄인다
  - 대체 환경(offscreen)의 값은 실제 환경과 한 번 비교한 뒤에 쓴다
