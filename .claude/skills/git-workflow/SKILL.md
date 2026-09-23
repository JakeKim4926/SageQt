---
name: git-workflow
description: >
  SageQt 프로젝트의 Git 워크플로우 skill. 브랜치 생성, 커밋, PR 작성, 머지 등 모든 Git 작업 시 반드시 이 skill을 참조한다.
  트리거 조건: "브랜치", "커밋", "PR", "머지", "기능 추가", "버그 수정", "리팩토링", "git", "push", "작업 시작", "릴리스", "배포" 등이 언급되면 이 skill을 트리거한다.
  CRITICAL: 새 작업을 시작하기 전에 반드시 이 skill을 확인한다. 브랜치는 작업 단위별로 소규모로 생성하고, 커밋은 하나의 변경 목적당 하나만 생성한다. 이 규칙은 예외 없이 적용된다.
---

# Git Workflow 규칙

## 목적

이 규칙은 SageQt 프로젝트의 Git 운영 기준을 정의한다.
다음 목표를 가진다.

- 배포 브랜치 안정성 유지
- 통합 개발 흐름 명확화
- 커밋/PR 히스토리 품질 유지
- 릴리스 단위 산출물 관리
- 변경 목적과 범위를 추적 가능하게 유지

---

## 기본 원칙

- `main`과 `develop`에는 직접 커밋하지 않는다
- 모든 작업은 작업 브랜치에서 수행한다
- 모든 PR의 기본 목적지는 `develop`이다
- 버전 릴리스 시점에만 `develop → main` PR을 생성한다
- 머지 후에는 작업 브랜치를 삭제한다
- PR 생성 및 머지 이력은 `docs/decisions/PR_LOG.md`에 기록한다

## 브랜치 단위 원칙 (엄격 적용)

**브랜치는 반드시 하나의 작업 단위만 담는다.**

- 하나의 브랜치 = 하나의 기능 / 하나의 버그 수정 / 하나의 리팩토링
- 여러 작업을 하나의 브랜치에 묶지 않는다
- 릴리스 전체를 하나의 브랜치로 처리하지 않는다
- 작업 범위가 커지면 더 작은 단위로 나눠 순차적으로 진행한다

잘못된 예:
- `feature/new-workflow` 에 조회, 파일 내보내기, 정렬 수정, UI 변경을 모두 포함

올바른 예:
- `feature/result-filter-criteria-select` → `fix/result-table-sort-order` → ...

## 커밋 단위 원칙 (엄격 적용)

**커밋은 반드시 하나의 변경 목적당 하나만 생성한다.**

- 하나의 커밋 = 하나의 목적 (기능 추가 / 버그 수정 / 설정 변경 중 하나)
- 여러 파일이 바뀌어도 같은 목적이면 하나의 커밋
- 다른 목적의 변경은 반드시 별도 커밋으로 분리
- 빌드 오류 수정, 인코딩 설정, include 경로 수정은 각각 별도 커밋

잘못된 예:
- 기능 구현 + 버그 수정 + 설정 변경을 하나의 커밋에 묶음

올바른 예:
- `feat: 결과 표 검색 기준 선택 추가`
- `fix: Linux에서 include 경로 대소문자 불일치 수정`
- `chore: .gitattributes로 줄 끝 규칙 고정`

---

## 브랜치 전략

| 브랜치 | 용도 | 예시 |
|---|---|---|
| `main` | 배포 가능한 안정 버전 | — |
| `develop` | 통합 개발 브랜치 | — |
| `feature/*` | 기능 개발 | `feature/result-filter-criteria-select` |
| `fix/*` | 버그 수정 | `fix/enable-clipboard-shortcuts` |
| `refactor/*` | 구조 개선 | `refactor/view-panel-split` |
| `docs/*` | 문서 작업 | `docs/release-notes-v1.1` |

---

## 브랜치 흐름

```text
feature/* │ fix/* │ refactor/* │ docs/*
          ↓
       develop
          ↓
        main
```

### 원칙
- 일반 작업 브랜치는 모두 `develop`으로 PR을 보낸다
- `main`은 직접 수정하지 않는다
- `develop → main`은 버전 릴리스 시점에만 수행한다
- 현재 `release` 브랜치는 운영하지 않는다

---

## 작업 시작 절차

작업 시작 전 반드시 최신 `develop` 기준으로 브랜치를 생성한다.

```bash
git checkout develop
git pull origin develop
git checkout -b <타입>/<작업명>
```

### 예시
- 기능 추가: `feature/result-table-date-column`
- 버그 수정: `fix/result-table-sort-order`
- 구조 개선: `refactor/view-panel-split`
- 문서 작업: `docs/release-notes-v1.1`

---

## 커밋 규칙

### 커밋 단위
- 한 커밋은 하나의 변경 목적만 가진다
- 기능, 리팩토링, 문서 수정, 버그 수정을 한 커밋에 섞지 않는다
- 의미 없는 대량 커밋을 금지한다
- 리뷰 가능한 단위로 자주 커밋한다

### 커밋 타입
- `feat` : 새 기능
- `fix` : 버그 수정
- `style` : 화면 스타일 / 색상 / 정렬 등 표시 변경 (동작 변경 없음)
- `refactor` : 동작 변경 없는 구조 개선
- `docs` : 문서 작업
- `test` : 테스트 추가/수정
- `chore` : 빌드, 설정, 기타 유지보수

`style`과 `refactor` 구분: 화면에 보이는 결과가 바뀌면 `style`, 코드 구조만 바뀌고 결과가 같으면 `refactor`.

### 커밋 메시지 형식
```text
타입: 한 줄 요약
```

### 커밋 메시지 세부 규칙
- 기본은 제목 한 줄만 사용한다
- 제목은 `타입: 변경 요약` 형식을 유지한다
- 제목만으로 변경 목적이 충분하면 body를 붙이지 않는다
- 하나의 목적 아래 하위 변경이 2개 이상이면 빈 줄 뒤 body bullet을 허용한다
- body는 구현 세부사항 나열이 아니라 리뷰 관점의 변경 포인트만 적는다

### body 형식
```text
타입: 한 줄 요약

- 변경 1
- 변경 2
- 참고 사항
```

### 예시
```text
feat: 결과 표 검색 기준 선택 추가
fix: macOS에서 로그인 창이 부모 창 중앙에 뜨지 않는 문제 수정
style: 검색 기준 콤보를 입력 콤보 스타일로 통일
refactor: 실행 기록 패널을 메인 창에서 분리
docs: RELEASE_NOTES v1.1 추가
```

```text
feat: 실행 기록 삭제 기능 추가

- 선택한 기록을 지우는 버튼 추가
- 삭제 전 확인 다이얼로그 표시
- 삭제 후 목록과 요약 막대 갱신
```


## PR · 머지 · 릴리스

PR을 만들거나, 머지하거나, 릴리스할 때는 **`references/pr-and-release.md`를 읽는다.**

| 상황 | 볼 섹션 |
|---|---|
| PR 생성 | PR 규칙 · PR 작업 로그 |
| 머지 | 머지 전 체크리스트 · 머지 전략 · 머지 후 정리 |
| 릴리스 | 릴리스 머지 · 릴리스 태그 |
| 문서 작업 | 문서 브랜치 운영 원칙 |
| `develop` / `main`에 반영 | develop / main 보호 원칙 |

---

## 최종 원칙

이 워크플로우의 핵심은 다음과 같다.

- 작업은 항상 목적이 분명한 브랜치에서 시작한다
- 통합은 `develop`에서 수행한다
- 배포 반영은 `main`에서 관리한다
- PR은 단순 병합 수단이 아니라 변경 목적과 범위를 기록하는 문서다
- 모든 릴리스는 실행 가능한 상태와 기록(RELEASE_NOTES, PR_LOG)을 함께 남긴다
