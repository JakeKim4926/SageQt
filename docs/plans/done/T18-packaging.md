# T18 — 패키징 · CI 산출물

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
CI가 세 OS용 배포 결과물을 만든다 — Qt 런타임 · 플러그인(특히 `qsqlite`)을 포함해, Qt가 설치되지 않은 PC에서도 실행되는 형태로.

## 시작 전에
1. 선행 주제: 없음 (T17 완료 — 버전 · 식별 정보 · 라이선스 복사가 있다, 아이콘은 대기)
2. 스킬 로드: `coding-design` (빌드 정의), `git-workflow`
   - 읽을 reference: `coding-design/references/cmake-targets.md`
3. 결정 — 착수 시 사용자와 확정
   - 결과물 형식: Windows (폴더 zip · 설치 파일), macOS (`.dmg`), Linux (AppImage 등)
4. **T17에서 넘긴 것** — 앱 아이콘은 사용자가 고해상도 원본을 주기로 했다 (`DEBT_LOG.md`). 받았는지 확인하고, 받았으면 `.ico` · `.icns` · Linux PNG를 먼저 연결한다. 배포 틀은 `SageQt/packaging/`(windows `SageQt.rc.in` · linux `SageQt.desktop.in`)에 둔다. 라이선스는 빌드 결과물의 `licenses/`(macOS 번들 `Contents/Resources/licenses`)에 이미 복사된다
5. 재확인할 사실
   - Qt 6.11의 `qt_generate_deploy_app_script`가 세 OS에서 무엇을 해 주는지 (Qt 문서)
   - Linux 배포 도구는 Qt가 제공하지 않는다 — 착수 시 선택지 조사

## SageSDI에서 옮길 것
- SageSDI의 빌드 후 단계(`SageSDI.vcxproj` `<PostBuildEvent>`)는 삭제된 `templates/` · `tools/` 폴더를 복사한다 — **옮기지 않는다** (SageSDI `docs/DEBT_LOG.md`의 잔재 항목)

## 옮기지 않는 것
위 빌드 후 복사.

## 함정
- **`qsqlite` 플러그인이 빠지면 DB가 열리지 않는다.** 배포 결과물에 SQL 드라이버 플러그인이 들어갔는지 확인한다
- DB · 설정은 `QStandardPaths` 경로에 쓰므로 설치 폴더가 읽기 전용이어도 된다 (T04 · T06) — 설치 폴더에 쓰는 코드가 없는지 다시 확인한다
- 결과물을 Qt가 없는 깨끗한 환경에서 실행해 본다 (CI의 새 러너 또는 가상 환경)
- 번들 폰트 · 라이선스 파일이 결과물에 들어갔는지 확인한다

## 작업
PR 1~2개: `chore/deploy-script`, `chore/ci-artifacts`
- [x] 배포 스크립트 (CMake install + Qt 배포 스크립트)
- [x] Linux 배포 도구 (Qt 자체 복사 + tar.gz — 외부 도구 없음)
- [x] CI에서 세 OS 결과물 생성 · 업로드
- [x] Qt가 없는 환경에서 실행 확인 (실행 · DB 생성까지 — 로그인 · 샘플 업무는 아래 결과)

## 완료 기준
- CI가 세 OS 결과물을 산출물로 남긴다
- Qt가 없는 환경에서 세 결과물이 실행되고 로그인 · 샘플 업무가 동작한다

## 범위 밖
- 서명 · 공증 — T19

## 확인한 사실
- 사용자 결정 (2026-10-02): (1) 결과물은 **Windows zip · macOS .dmg · Linux tar.gz** (CPack 기본 도구만, 설치 파일 · AppImage는 필요할 때). (2) 앱 아이콘: 사용자가 알려준 `D:/Projects/SageTaechang/SageTaechang/res/SageTaechang.ico`는 SageSDI 아이콘과 **같은 파일**(MD5 `035bf091…`, 32×32 한 장)이라 고해상도 원본이 없다 → 사용자 지시("어떻게든 고화질로")로 **다시 그렸다**. 원본 구성(크림색 원형 배지 · 점 테두리 · 금색 띠 · S.A.G.E 글자)을 Gmarket Sans Bold로 1024에 그렸고, 원본 위쪽 인물 모양은 32px에서 알아볼 수 없어 넣지 않았다. 후보 A(원본 확대) · B(다시 그림) 중 B 선택 (`docs/screenshots/T18/icon-candidates.png`)
- 아이콘: `tools/app-icon/make_app_icon.py`가 `SageQt/resources/icons/`에 `sageqt-1024.png`(원본) · `sageqt.ico`(16~256 7개) · `sageqt.icns`(16~1024) · `sageqt-256.png`(Linux · 창 아이콘)를 만든다. 연결: Windows rc `IDI_ICON1` · macOS `MACOSX_BUNDLE_ICON_FILE` + 번들 `Resources` · Linux `.desktop` `Icon=sageqt` · 창 아이콘 `QApplication::setWindowIcon`(Qt 리소스 `:/icons/sageqt-256.png`). 로컬 빌드 exe에서 아이콘을 꺼내 확인
- Qt 6.11 배포 스크립트(`Qt6CoreDeploySupport.cmake`): Windows `windeployqt`, macOS `macdeployqt`, 그 밖(Linux)은 Qt 자체 복사(`_qt_internal_generic_deployqt` — 라이브러리 `lib/` · 플러그인 `plugins/`, Technical Preview)
- 배포 스크립트 (`SageQt/packaging/CMakeLists.txt`): `install(TARGETS)`(macOS 번들은 접두사 루트, 나머지는 `bin`) → `qt_generate_deploy_app_script(... NO_TRANSLATIONS)` → 라이선스 `bin/licenses`(macOS는 번들 안), Linux는 `.desktop` → `share/applications`, 아이콘 → `share/icons/hicolor/256x256/apps/sageqt.png`. CPack 생성기는 Windows `ZIP` · macOS `DragNDrop` · Linux `TGZ`, 파일 이름 `SageQt-1.0.0-<플랫폼>`(플랫폼 이름은 `CMakePresets.json`의 `SAGE_PACKAGE_PLATFORM` — OS 차이는 프리셋에), SHA256 파일 함께. 실행 파일 타깃의 빌드 단계(rc · 라이선스 복사 · `.desktop` 생성)는 `add_custom_command(TARGET)`가 같은 디렉터리여야 해서 `SageQt/CMakeLists.txt`에 남겼다
- 로컬 Windows(디버그) 확인: `cmake --install` + `cpack` → `SageQt-1.0.0-windows-x64.zip`. `bin/`에 exe · Qt DLL · VC 런타임 · `qt.conf`, `plugins/`에 `platforms/qwindows` · `sqldrivers/qsqlite`(+ 다른 SQL 드라이버들) · imageformats · styles · tls 등. Qt 번역 파일은 `NO_TRANSLATIONS`로 뺐다(앱은 다국어 없음). PATH에서 Qt를 뺀 채 실행하자 창 제목 `SageQt`로 떴고, 프로세스가 패키지의 `Qt6*.dll` · `plugins/sqldrivers/qsqlited.dll` · `plugins/platforms/qwindowsd.dll`을 불러왔다. 이 PC에는 Qt가 설치돼 있어 "Qt가 없는 환경"은 CI 새 러너로 확인한다 (다음 PR)
- CI (`build.yml`): release 빌드 job마다 `cmake --install` + `cpack` → 산출물 `package-<프리셋>`(결과물 + `.sha256`). `verify-package` job이 **Qt를 설치하지 않은 새 러너**에서 산출물만 받아 풀고(Windows unzip · macOS `hdiutil attach` 후 `.app` 복사 · Linux tar), Qt 명령(`qmake6` · `qtpaths6`)이 없는지 확인한 뒤 실행해 20초 동안 떠 있는지와 `QStandardPaths` 데이터 폴더에 `sageqt.db`가 새로 생기는지(= `qsqlite`가 동작) 본다
- 결과물 크기 (실행 36990400806): Windows zip 54,714,961 · macOS dmg 29,824,727 · Linux tar.gz 약 35.8MB(산출물 압축 35,837,143). 세 OS 모두 새 러너에서 `sageqt.db` 16,384바이트 생성
- **Linux tar.gz는 시스템 라이브러리를 묶지 않는다**: 첫 검증에서 `libOpenGL.so.0: cannot open shared object file`로 실행 실패. Qt의 Linux 복사는 Qt 라이브러리 · 플러그인만 넣는다. 사용자 PC에 필요한 것(Ubuntu 24.04 기준 패키지): `libopengl0 libegl1 libxcb-cursor0 libxkbcommon-x11-0 libxcb-icccm4 libxcb-keysyms1 libxcb-shape0` — 검증 러너에 이것만 설치하고 통과
- macOS dmg는 서명 · 공증 전이라 사용자 PC에서는 Gatekeeper가 막는다 (T19)

## 결과
- PR 없이 세 브랜치로 `develop`에 squash merge: `chore/app-icon`(아이콘 — T17에서 넘긴 것), `chore/deploy-script`(설치 규칙 · Qt 런타임 복사 · CPack), `chore/ci-artifacts`(CI 결과물 · 새 러너 실행 확인)
- 완료 기준 확인
  - CI가 세 OS 결과물을 산출물로 남긴다 — 실행 36990400806의 `package-windows-x64-release` · `package-macos-arm64-release` · `package-linux-x64-release`
  - Qt가 없는 환경에서 세 결과물이 실행된다 — `verify-*` 세 job (20초 실행 + DB 생성). **로그인 · 샘플 업무는 사람이 화면에서 해 보지 않았다** (CI 새 러너에서 창을 조작하지 않음) → `DEBT_LOG.md`
- 사용자 결정 2건: 결과물 zip · dmg · tar.gz · 아이콘은 다시 그린 B
- 교훈
  - "Qt가 없는 환경"은 Qt가 깔린 개발 PC로 흉내 낼 수 없다 — 새 러너가 Linux 시스템 라이브러리 누락을 잡았다
  - DB 파일 생성 여부는 GUI 없이 "SQL 드라이버 플러그인이 들어갔는가"를 확인하는 확실한 신호다

