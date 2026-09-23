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

## [2026-09-23] docs/sageqt-skills
- **목적**: SageSDI(MFC) 이관을 시작하기 전에 코드 규칙 · 프로젝트 목표 · 이관 계획을 확정 (Step -1)
- **변경 내용**: `CLAUDE.md`에 프로젝트 목표 추가. 스킬 6종 추가 — `coding-rules` · `coding-design` · `code-review-expert`는 Qt 6 기준으로 재작성, `git-workflow` · `sageqt-plan` · `debt-log-guard`는 이식, SKILL.md 300줄 이하 + `references/` 분리. `MIGRATION_PLAN.md` 추가 및 Step -1 완료 반영
- **PR 링크**: https://github.com/JakeKim4926/SageQt/pull/1
- **결과**: merged (develop, 2026-09-23)
