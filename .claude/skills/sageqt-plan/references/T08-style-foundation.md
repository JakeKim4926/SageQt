# T08 — `SageStyle` · 디자인 값 · 폰트 등록

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
앱 전체의 모양을 한 곳에서 그리는 기반을 만든다. 이 주제가 끝나면 `SageDesignDefine.h`에 디자인 값이 있고, Fusion 기반 `SageStyle`과 `QPalette`, 번들 폰트가 `main.cpp`에서 한 번 적용된다.

## 시작 전에
1. 선행 주제: T06 (조립 지점), T07 (`sageqt-ui` 스킬)
2. 스킬 로드: `sageqt-ui`, `coding-design`, `coding-rules`, `git-workflow`
   - 읽을 reference: `coding-design/references/style.md` · `cmake-targets.md` (리소스는 실행 파일 타깃), `coding-rules/references/values-and-platform.md`, `sageqt-ui`의 디자인 값 · `SageStyle` 범위 reference
3. 결정 — `sageqt-ui`가 정한 것을 따른다. 스킬에 없는 판단이 필요하면 스킬을 먼저 고친다
4. 재확인할 사실
   - T02의 OS별 폰트 패밀리 · 스타일 이름 (폰트를 이름으로 찾을 때 쓴다)
   - Mac mini 준비 여부 — 이 주제부터 macOS 화면 판정이 필요하다

## SageSDI에서 옮길 것
- 디자인 값 · 폰트 역할 · 크기: `sageqt-ui` 스킬이 정리한 목록 (원본은 T07 파일 참조)
- 폰트 파일: T02에서 저장소에 넣은 것 (`SageQt/resources/`)
- 메인 창 크기: SageSDI는 **지정하지 않는다** — `CMainFrame::PreCreateWindow`가 창 제목만 바꾸고, 최소 크기(`OnGetMinMaxInfo`)도 없다 (2026-09-23 확인). SageQt의 초기 · 최소 크기는 `sageqt-ui` 규격을 따르고, 규격에 없으면 사용자에게 확인한다

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `SageUiResources.cpp`의 전역 GDI 폰트 객체 (`g_fontControl` 등) | 전역 금지 · GDI. 앱 폰트와 `SageStyle`이 대체 |
| `AddFontMemResourceEx`로 6개 폰트를 모두 로드 | 쓰이는 폰트만 등록한다 (T02 · T07 결론) |
| `OnCtlColor` · `CBrush` 기반 색 지정 | `SageStyle` · `QPalette` |

## 함정
- `QApplication::setStyle`은 스타일 객체의 소유권을 가져간다. 위젯을 만들기 **전에** 적용한다
- Fusion은 `QStyleFactory::create("Fusion")`로 만든다. `SageStyle`(`QProxyStyle`)의 기반으로 넘긴다
- 폰트는 리소스(qrc)로 **실행 파일 타깃**에 등록한다 (정적 라이브러리에 넣으면 링커가 등록 코드를 지울 수 있다). 등록은 `QFontDatabase::addApplicationFont`, 돌려받은 패밀리 이름으로 찾는다 — SageSDI의 GDI식 이름을 하드코딩하지 않는다 (T02)
- 0.1pt 단위 크기를 포인트로 변환해 쓴다 (`setPointSizeF`)
- QSS · `setStyleSheet` 금지. 위젯 · 화면에서 `setPalette` · `setFont` 개별 지정 금지 (`style.md`)
- 시작 시 안내(DB 오류 · 초기 비밀번호)가 T09부터 이 스타일로 그려지도록, 스타일 적용은 스키마 준비보다 **먼저** 한다

## 작업
PR 1개: `feature/style-foundation`
- [ ] `ui/style/SageDesignDefine.h` — `sageqt-ui`의 디자인 값
- [ ] `ui/style/`의 `SageStyle` (Fusion 기반 `QProxyStyle`) — `sageqt-ui`가 정한 재정의 범위 중 T09 이전에 필요한 것부터
- [ ] 팔레트 생성, 앱 폰트
- [ ] 폰트 리소스 등록 (실행 파일 타깃)
- [ ] `main.cpp`에서 스타일 · 팔레트 · 폰트 적용 (스키마 준비보다 먼저)
- [ ] Windows · Linux(가능하면 WSLg) · macOS 화면 확인, 스크린샷을 PR에 첨부

## 완료 기준
- 3-OS CI 통과
- 디자인 값이 `SageDesignDefine.h` 밖에 0개다 (색 리터럴 · 여백 숫자 검색)
- 등록된 폰트 패밀리가 세 OS에서 기대한 이름으로 잡힌다 (로그 또는 테스트)
- Windows · macOS 스크린샷이 PR에 있다

## 범위 밖
- 개별 위젯 · 화면 — T09 이후

## 확인한 사실
(진행 중 기록)
