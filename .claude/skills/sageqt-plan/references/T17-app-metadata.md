# T17 — 버전 · 아이콘 · 폰트 라이선스

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
세 OS에서 앱이 올바른 이름 · 버전 · 아이콘을 갖고, 번들 폰트의 라이선스 고지가 갖춰진다.

## 시작 전에
1. 선행 주제: T16
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
- [ ] 버전 단일 출처
- [ ] Windows 버전 리소스 · 아이콘
- [ ] macOS `Info.plist` · `.icns` · 번들 식별자
- [ ] Linux 아이콘 · `.desktop`
- [ ] 폰트 라이선스 고지 파일

## 완료 기준
- 세 OS 빌드 결과물에서 버전이 같은 값으로 보인다 (Windows 파일 속성 · macOS 정보 가져오기 · Linux `.desktop`)
- 번들 폰트마다 라이선스 고지가 있다

## 범위 밖
- 설치 파일 · 서명 — T18 · T19

## 확인한 사실
(진행 중 기록)
