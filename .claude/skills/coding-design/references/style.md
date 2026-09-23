# 스타일

`coding-design`의 상세 규칙이다. 모양을 바꾸거나 커스텀 위젯 · delegate를 그릴 때 읽는다. 세부 규격(색 목록, 위젯별 치수)은 UI 스킬(`sageqt-ui`)이 정한다.

## 화면은 그리는 방법을 몰라야 한다

**위젯이 자기를 그린다.** 화면 클래스(창 · 패널 · 다이얼로그)에는 배치와 업무 흐름만 둔다.
목표는 화면 클래스가 그리기·스타일 코드를 한 줄도 갖지 않는 것이다 — 선언 · 생성 · 변형 지정으로 끝난다.

```
Window / Panel / Dialog     배치, 업무 흐름, signal 연결
      ↓
Sage* 위젯 / Delegate        상태(variant) 보유, 자기 그리기
      ↓
SageStyle + 디자인 값        표준 위젯의 모양 · 색 · 여백 · 폰트
```

**아래가 화면 클래스에 있으면 위젯이나 스타일로 내려야 한다.**

- `paintEvent` 오버라이드
- 색·폰트를 직접 지정하는 코드 (`setPalette`, `setFont`에 값 지정)
- 자식 위젯 종류를 가르는 스타일 분기

## 스타일 규칙 (CRITICAL)

- **스타일은 앱 전역 `SageStyle` 하나가 담당한다.** `SageStyle`은 Fusion을 바탕으로 한 `QProxyStyle`이고, `QPalette` · 앱 폰트와 함께 `main.cpp`에서 한 번 적용한다. Fusion은 플랫폼에 묶이지 않아 세 OS에서 같은 모양을 낸다
- **Qt 스타일시트(QSS)를 쓰지 않는다.** `setStyleSheet()` 호출과 `.qss` 파일 금지. QSS가 걸린 위젯은 `setPalette` / `setFont`가 듣지 않고, `QProxyStyle`과 섞이지 않으며, 디자인 값을 C++와 공유할 수 없다
- 표준 위젯의 모양(버튼 · 입력칸 · 콤보 · 체크 · 탭 · 헤더 · 스크롤바)은 `SageStyle`이 그린다. 위젯과 화면에서 `setPalette` / `setFont`로 개별 지정하지 않는다
- 위젯 변형(variant)은 `Q_ENUM` enum 타입의 `Q_PROPERTY`로 노출하고, `SageStyle`이 그 값을 읽어 그린다. 변형을 바꾸면 위젯이 `update()`를 호출한다 (크기가 바뀌면 `updateGeometry()`도)
- 색 · 여백 · 폰트 크기의 출처는 `coding-rules/references/values-and-platform.md`의 *디자인 값*을 따른다. `SageStyle`, delegate, 커스텀 위젯의 `paintEvent`가 모두 같은 값을 쓴다
- 여러 위젯이 공유하는 그리기 조각은 `ui/style/`에 두고, 위젯과 `SageStyle`만 호출한다. 화면 클래스는 호출하지 않는다
- **Qt 기본 위젯으로 되는 것은 서브클래싱하지 않는다.** 모양은 `SageStyle`이 바꾼다. 커스텀 위젯은 기본 위젯에 없는 요소(필 바, 배지, 상태 카드 등)에만 만든다
- 기본 위젯에 없는 새 종류의 UI 요소가 필요하면 한 곳에서만 쓰이더라도 위젯으로 만든다. "지금은 여기서만 쓰니까"가 반복되면 그리기 코드가 화면마다 복제된다
- **UI는 코드로 만든다. Qt Designer `.ui` 파일을 쓰지 않는다** — 위젯 생성 · 배치 방식을 하나로 유지하고, `ui-composition.md`의 *화면 클래스 작성 형태*를 모든 화면에 똑같이 적용하기 위해서다

세부 규격(색 목록, 위젯별 치수, `SageStyle`이 재정의할 항목)은 UI 스킬(`sageqt-ui`)이 정한다.

**판단 기준: 위젯을 하나 추가할 때 화면 클래스의 그리기나 스타일 코드를 함께 고쳐야 하는가.**
고쳐야 한다면 확장점이 없다는 신호다.
