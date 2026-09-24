# T19 — 서명 · 공증

> 이 파일은 구현 지시서다. 위에서부터 순서대로 읽고 따른다.

## 목표
배포 결과물이 사용자 PC에서 보안 경고 없이 실행된다 — Windows Authenticode 서명, macOS 서명 · 공증.

## 시작 전에
1. 선행 주제: T18
2. 스킬 로드: `git-workflow`
3. 결정 · 준비 — 사용자에게 확인
   - Apple Developer Program 가입 (연 $99), Windows 코드서명 인증서
   - 인증서 · 비밀번호를 CI 비밀값(GitHub Secrets)으로 등록하는 것은 **사용자가 직접** 한다 (자격 증명을 다루지 않는다)
4. 재확인할 사실
   - 착수 시점의 Apple 공증 도구(`notarytool`)와 요구사항 (hardened runtime 등)

## SageSDI에서 옮길 것
해당 없음 — SageSDI는 서명하지 않았다.

## 옮기지 않는 것
해당 없음.

## 함정
- 자격 증명(인증서 · 비밀번호 · Apple ID)을 코드나 로그에 남기지 않는다. CI는 비밀값으로만 받는다
- macOS: 서명 후 번들을 수정하면 서명이 깨진다 — 배포 도구 실행 → 서명 → 공증 → staple 순서
- 공증 결과는 실제 Mac(Mac mini)에서 Gatekeeper 경고 없이 열리는지로 확인한다
- 외부 action은 커밋 해시로 고정한다

## 작업
PR 1~2개: `chore/windows-signing`, `chore/macos-notarization`
- [ ] Windows 서명 (CI)
- [ ] macOS 서명 · 공증 · staple (CI)
- [ ] 실제 PC에서 경고 없이 실행되는지 확인

## 완료 기준
- Windows 결과물에 유효한 서명이 있다 (파일 속성 · `signtool verify`)
- macOS 결과물이 공증되어 Mac mini에서 Gatekeeper 경고 없이 열린다

## 범위 밖
- 자동 업데이트 — 요청되지 않았다

## 확인한 사실
(진행 중 기록)
