# PR · 머지 · 릴리스

`git-workflow`의 상세 규칙이다. PR을 만들거나, 머지하거나, 릴리스할 때 읽는다.

## develop 반영 절차 (기본)

PR 없이 반영한다. CI가 작업 브랜치 push에서도 돌기 때문에 머지 전에 세 OS 검증을 받을 수 있다.

1. 작업 브랜치를 push한다 (`git push -u origin <브랜치명>`)
2. **그 커밋 해시로** CI 실행을 찾아 모든 job이 `success`인지 확인한다 (`sageqt-plan` 공통 함정: 최근 실행이 아니라 해시로)
3. 로컬에서 `develop`을 최신으로 받고 squash merge한다
   ```bash
   git checkout develop
   git pull origin develop
   git merge --squash <브랜치명>
   git commit            # 아래 squash merge 메시지 형식
   git push origin develop
   ```
4. `develop`의 CI도 통과하는지 확인한다
5. *머지 후 정리*대로 브랜치를 지우고, *PR 작업 로그*를 남긴다 (로그 커밋은 3번 전에 작업 브랜치에 넣는다)

UI 변경의 스크린샷은 `docs/screenshots/<주제 ID>/`에 커밋하고, 수동 확인 표는 주제 파일의 *결과* 절에 적는다.

---

## PR 규칙 (사용자가 PR을 요청한 경우)

### PR 목적지
PR은 반드시 `develop` 기준으로 생성한다.
기본 브랜치가 `main`인 저장소에서의 실수를 막기 위해 `--base develop`을 명시한다.

```bash
gh pr create --base develop --title "..." --body "..."
```

### PR 필수 항목
PR 본문에는 아래 항목을 반드시 기재한다.

```markdown
## 작업 목적
(이 PR이 왜 필요한지)

## 변경 범위
(어떤 파일/계층이 변경되었는지)

## 스크린샷
(UI 변경이 있는 경우 필수)

## 참고 사항
(리뷰어가 알아야 할 내용, 있으면 기재)
```

### 필수 조건
- `작업 목적` 필수
- `변경 범위` 필수
- UI 변경 시 `스크린샷` 필수

필수 항목이 누락된 PR은 머지하지 않는다.

---

## 머지 전 체크리스트

- [ ] 작업 브랜치 CI의 모든 job이 통과했는가 (커밋 해시로 확인)
- [ ] PR_LOG에 목적과 변경 범위가 기재되어 있는가
- [ ] UI 변경이 있다면 스크린샷이 `docs/screenshots/<주제 ID>/`에 있는가
- [ ] 커밋 메시지가 컨벤션을 따르는가

---

## 머지 전략

### 기본 전략
- 작업 브랜치에서 `develop`으로 머지할 때는 **squash merge**를 기본으로 사용한다
- 여러 개의 작업 커밋을 `develop`에 깔끔한 단위로 남긴다
- 불필요한 merge commit은 만들지 않는다
- squash merge 커밋 제목이 `develop`에 남는 최종 제목이다
- 브랜치 안의 개별 커밋 제목은 squash merge 본문 후보가 되므로 의미 있는 문장으로 유지한다

### squash merge 메시지 형식
기본적으로 아래 형식을 따른다.

```text
타입: PR 한 줄 요약 (#번호)

* 타입: 하위 변경 요약 1
* 타입: 하위 변경 요약 2
```

예시:

```text
fix: 결과 표 UI 수정 2건 (#12)

* fix: 늘어나는 열을 QHeaderView::Stretch로 변경
* style: 선택 막대 버튼 정렬 통일
```

원칙:
- squash merge 본문은 PR 내부의 핵심 작업 단위를 bullet로 요약한다
- bullet은 커밋 제목처럼 읽히도록 `타입: 요약` 형식을 유지한다
- 의미 없는 자동 생성 문구보다 변경 목적이 드러나는 bullet을 우선한다

### 예외
- 커밋 히스토리 자체가 의미 있는 경우에만 별도 판단한다

---

## 릴리스 머지 (`develop → main`)

버전 릴리스 시점의 `develop → main` 머지는 아래 형식을 사용한다.

```text
Release: develop -> main (주요 변경 요약)
```

예시:

```text
Release: develop -> main (로그인 · 결과 표 · 실행 기록 화면 이관)
```

- 요약에는 이번 릴리스에 포함된 주요 변경을 쉼표로 나열한다
- 릴리스 머지는 squash하지 않는다 (`develop` 히스토리를 보존한다)

---

## 머지 후 정리

머지 완료 후 작업 브랜치는 즉시 삭제한다.

```bash
git checkout develop
git branch -d <브랜치명>
git push origin --delete <브랜치명>
```

역할이 끝난 브랜치를 장기간 남겨두지 않는다.

---

## develop / main 보호 원칙

### develop
- 작업 커밋을 직접 push하지 않는다 — squash merge 커밋만 push한다
- 반영 전 작업 브랜치의 CI가 모두 통과해야 한다 (*develop 반영 절차*)
- 통합 개발 기준 브랜치로 사용

### main
- 직접 push 금지
- 배포 가능한 안정 버전만 반영
- `develop → main` PR을 통해서만 반영

---

## 문서 브랜치 운영 원칙

`docs/*` 브랜치도 동일한 *develop 반영 절차*를 따른다.

- 목적 없이 머지하지 않는다
- `develop`으로 반영한다
- 구조 문서 변경 시 관련 문서와 함께 정리한다
- 필요하면 PR 로그와 연결한다

---

## PR 작업 로그

`develop`에 반영할 때마다 (PR이 있든 없든) `docs/decisions/PR_LOG.md`에 아래 형식으로 기록한다.

```markdown
## [yyyy-mm-dd] 브랜치명
- **목적**: 무엇을 위한 작업인지
- **변경 내용**: 주요 작업 요약
- **PR 링크**: (있으면. 없으면 "없음")
- **결과**: merged / closed / pending
```

이 기록은 변경 이력 추적과 의사결정 회고를 위한 공식 로그로 본다.
PR_LOG 기록은 작업 커밋과 분리해 `docs:` 타입 커밋으로 남긴다.

---

## 릴리스 태그

버전 릴리스 시점에 `main`에 태그를 남긴다.

### 형식
```text
v<major>.<minor>
```

### 예시
- `v0.9`
- `v1.0`
- `v1.1`

태그와 함께 `docs/RELEASE_NOTES.md`에 해당 버전 변경 내역을 기록한다.
