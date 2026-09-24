# T18 — 패키징 · CI 산출물

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
CI가 세 OS용 배포 결과물을 만든다 — Qt 런타임 · 플러그인(특히 `qsqlite`)을 포함해, Qt가 설치되지 않은 PC에서도 실행되는 형태로.

## 시작 전에
1. 선행 주제: T17
2. 스킬 로드: `coding-design` (빌드 정의), `git-workflow`
   - 읽을 reference: `coding-design/references/cmake-targets.md`
3. 결정 — 착수 시 사용자와 확정
   - 결과물 형식: Windows (폴더 zip · 설치 파일), macOS (`.dmg`), Linux (AppImage 등)
4. 재확인할 사실
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
- [ ] 배포 스크립트 (CMake install + Qt 배포 스크립트)
- [ ] Linux 배포 도구
- [ ] CI에서 세 OS 결과물 생성 · 업로드
- [ ] Qt가 없는 환경에서 실행 확인

## 완료 기준
- CI가 세 OS 결과물을 산출물로 남긴다
- Qt가 없는 환경에서 세 결과물이 실행되고 로그인 · 샘플 업무가 동작한다

## 범위 밖
- 서명 · 공증 — T19

## 확인한 사실
(진행 중 기록)
