# T17 — 버전 · 아이콘 · 폰트 라이선스

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
세 OS에서 앱이 올바른 이름 · 버전 · 아이콘을 갖고, 번들 폰트의 라이선스 고지가 갖춰진다.

## 시작 전에
1. 선행 주제: 없음 (T16 완료 — UI 이관이 끝났다)
2. 스킬 로드: `coding-design` (빌드 정의 — OS별 분기는 패키징 설정에만), `git-workflow`
   - 읽을 reference: `coding-design/references/cmake-targets.md`
3. 결정 대기 — 사용자에게 확정받는다
   - **제품 버전**: SageSDI는 `2.0.0.1` (`VERSIONINFO`). SageQt를 이어서 매길지, 새로 시작할지
   - 회사명 · 제품명 · 저작권 문구 · macOS 번들 식별자는 T04에서 정한 앱 식별 정보를 따른다 (`MIGRATION_PLAN.md` 결정 기록)
4. 재확인할 사실
   - Gmarket Sans의 배포 라이선스 조건과 고지 문구 (SageSDI에 라이선스 파일이 없다)
   - T02 · T08에서 실제로 저장소에 넣은 폰트 목록

## SageSDI에서 옮길 것
- **버전 정보** — `SageSDI.rc`(UTF-16LE)의 `VS_VERSION_INFO`: `FILEVERSION 2,0,0,1` · `PRODUCTVERSION 2,0,0,1` · `FileDescription "SageSDI"` · `OriginalFilename "SageSDI.exe"`, 회사 · 제품 · 저작권은 `TODO`
- **아이콘** — `SageSDI/res/SageSDI.ico` (4,286바이트). `SageSDIDoc.ico`는 문서 아이콘이라 쓰지 않는다
- **폰트 라이선스** — `SageSDI/resources/PretendardLicense.txt` (OFL)

## 옮기지 않는 것
| 대상 | 이유 |
|---|---|
| `SageSDIDoc.ico` · `Toolbar.bmp` | 문서 · 툴바가 없다 |
| `TODO` 문구 그대로 | 결정한 값으로 채운다 |

## 함정
- `.rc`는 UTF-16LE — `iconv -f UTF-16LE -t UTF-8`로 읽는다
- 버전은 한 곳(CMake `project(VERSION)`)에서 정하고, Windows 버전 리소스 · macOS `Info.plist` · 앱의 `setApplicationVersion`이 모두 그 값을 쓴다 — 세 곳에 따로 적지 않는다
- 아이콘은 OS마다 형식이 다르다: Windows `.ico`, macOS `.icns`, Linux `.png` + `.desktop`. 원본 `.ico`의 해상도가 macOS에 충분한지 확인한다 (부족하면 사용자에게 원본 이미지 요청)
- OS별 설정은 CMake의 패키징 · 배포 영역에서만 분기한다

## 작업
PR 1개: `chore/app-metadata`
- [x] 버전 단일 출처
- [x] Windows 버전 리소스 (아이콘은 원본을 받은 뒤)
- [x] macOS `Info.plist` · 번들 식별자 (`.icns`는 원본을 받은 뒤)
- [x] Linux `.desktop` (아이콘은 원본을 받은 뒤)
- [x] 폰트 라이선스 고지 파일

## 완료 기준
- 세 OS 빌드 결과물에서 버전이 같은 값으로 보인다 (Windows 파일 속성 · macOS 정보 가져오기 · Linux `.desktop`)
- 번들 폰트마다 라이선스 고지가 있다

## 범위 밖
- 설치 파일 · 서명 — T18 · T19

## 확인한 사실
- 사용자 결정 (2026-10-02): (1) 제품 버전 **1.0.0** 새로 시작 (SageSDI `2.0.0.1`을 잇지 않는다). (2) 아이콘은 **사용자가 고해상도 원본(1024 PNG 또는 SVG)을 주면** 만든다 — SageSDI `SageSDI.ico`는 32×32 32bpp 한 장(4,286바이트)뿐이라 macOS · Linux에 부족하다. 받기 전까지 아이콘 작업은 미룬다. (3) 폰트 라이선스는 **실행 파일 옆 `licenses/`**(macOS는 번들 `Contents/Resources/licenses`). (4) 배포 틀 폴더 `SageQt/packaging/`을 새로 둔다 (`coding-design` 폴더 구조에 추가)
- 단일 출처: 최상위 `CMakeLists.txt`의 `project(SageQt VERSION 1.0.0)`과 앱 식별 정보 변수(`SAGE_ORGANIZATION_NAME` · `SAGE_APPLICATION_NAME` · `SAGE_COMPANY_NAME` · `SAGE_PRODUCT_NAME` · `SAGE_COPYRIGHT` · `SAGE_BUNDLE_IDENTIFIER`, 값은 T04 결정). `configure_file`로 `SageAppInfo.h`(조직 · 앱 이름 · 버전 — `SageDefine.h`에서 옮김) · Windows `SageQt.rc` · Linux `SageQt.desktop`을 만들고, macOS는 `MACOSX_BUNDLE_*` 속성으로 Qt 기본 `Info.plist`에 넣는다. 앱은 `QCoreApplication::setApplicationVersion`
- Windows 버전 리소스: `#pragma code_page(65001)`(저작권 ©), 언어 0x412(한국어)/1200(유니코드). 로컬 빌드에서 파일 속성 확인: CompanyName `Sage` · FileDescription `SageQt` · FileVersion/ProductVersion `1.0.0` · LegalCopyright `Copyright © 2026 Sage` · OriginalFilename `SageQt.exe`. FILEVERSION 네 번째 자리는 0
- Linux `.desktop`에는 표준 버전 키가 없다(`Version`은 명세 버전) → `X-AppVersion`. 설치 위치 · 아이콘 줄은 T18 · 아이콘 받은 뒤
- 폰트: 앱에 넣는 것은 Pretendard Regular · SemiBold · Bold, Gmarket Sans Bold 4개(`SAGE_FONT_RESOURCES`). 저장소의 Gmarket Light · Medium은 `tools/font-probe`만 쓴다. 라이선스: `PretendardLicense.txt`(OFL, SageSDI에서), `GmarketSansLicense.txt`(OFL, T02에서 폰트 name ID 13에서 꺼냄) — 두 파일이 각 패밀리의 모든 굵기를 덮는다
- CI: build job마다 "Check app metadata" 단계 — `CMakeLists.txt`의 버전과 Windows 파일 속성 · macOS `CFBundleShortVersionString`(+ 번들 ID) · Linux `X-AppVersion`을 비교하고 `licenses/` 두 파일을 확인한다
- 로컬 MSVC: 헤더가 바뀌어 전체를 다시 빌드할 때 테스트마다 들어 있는 폰트 리소스(qrc, 수십만 줄)를 동시에 컴파일하다 `C1060`(컴파일러 힙 부족)이 났다 — 이어서 빌드하면 끝난다

## 결과
- PR 없이 `chore/app-metadata` 한 브랜치로 `develop`에 squash merge
- 완료 기준 확인: 세 OS 빌드 결과물에서 버전이 같다 — CI 실행 36978520423의 "Check app metadata"(Windows 파일 속성 · macOS `CFBundleShortVersionString` · Linux `X-AppVersion`이 모두 `1.0.0`, macOS 번들 ID `com.sage.sageqt`) / 번들 폰트마다 라이선스 고지 — Pretendard · Gmarket Sans 라이선스가 `licenses/`에 들어간다 (같은 단계에서 확인)
- **남긴 것**: 앱 아이콘 — 사용자가 고해상도 원본을 주기로 했다. T18 *시작 전에*와 `DEBT_LOG.md`에 넘겼다
- 사용자 결정 4건: 1.0.0 새로 시작 · 아이콘은 원본을 받은 뒤 · 라이선스는 실행 파일 옆 `licenses/` · 배포 틀 폴더 `SageQt/packaging/`
- 교훈
  - 같은 브랜치에 연달아 푸시하면 앞 CI 실행이 취소된다 — 문서 커밋은 CI가 끝난 뒤 올리거나 한 번에 올린다
  - 버전 · 식별 정보를 CMake 한 곳에 두고 `configure_file`로 퍼뜨리면 코드 상수 · 리소스 · plist가 어긋날 수 없다

