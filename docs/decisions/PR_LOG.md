# PR 작업 로그

PR을 생성하거나 머지할 때마다 아래 형식으로 기록한다. 형식은 `git-workflow` 스킬을 따른다.

```markdown
## [yyyy-mm-dd] 브랜치명
- **목적**: 무엇을 위한 작업인지
- **변경 내용**: 주요 작업 요약
- **PR 링크**: (있으면)
- **결과**: merged / closed / pending
```

---

## [2026-09-23] docs/step0-plan
- **목적**: 이관 Step 0 착수 전 계획 상세화와 결정 반영 (PR 0-0)
- **변경 내용**: `MIGRATION_PLAN.md`에 Step 0 작업 순서 · 완료 기준 · 범위 밖 기록, 결정 3건(계층 타깃은 소스가 생길 때 생성 / 경고 수준 상향 · CI에서만 에러 / `SageQt.slnx` 유지) 반영. 컴파일 경고 규칙과 `.slnx` 예외를 스킬과 리뷰 점검 항목에 추가
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/3
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] docs/skills-residue-cleanup
- **목적**: 스킬에 남은 SageSDI 사례 · 예시를 정리해 스킬을 SageQt의 영구 규칙으로만 유지
- **변경 내용**: `coding-design` 참조 파일의 SageSDI 사례 · 비교 제거. `git-workflow` · `sageqt-plan` · `debt-log-guard` 예시를 SageQt 기준으로 교체. SageQt 규칙과 모순되던 예시 2건(PCH 경로, UTF-8 컴파일러 옵션) 교체. 규칙 변경 없음
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/2
- **결과**: merged (develop, 2026-09-23)

## [2026-09-23] docs/sageqt-skills
- **목적**: SageSDI(MFC) 이관을 시작하기 전에 코드 규칙 · 프로젝트 목표 · 이관 계획을 확정 (Step -1)
- **변경 내용**: `CLAUDE.md`에 프로젝트 목표 추가. 스킬 6종 추가 — `coding-rules` · `coding-design` · `code-review-expert`는 Qt 6 기준으로 재작성, `git-workflow` · `sageqt-plan` · `debt-log-guard`는 이식, SKILL.md 300줄 이하 + `references/` 분리. `MIGRATION_PLAN.md` 추가 및 Step -1 완료 반영
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/1
- **결과**: merged (develop, 2026-09-23)
