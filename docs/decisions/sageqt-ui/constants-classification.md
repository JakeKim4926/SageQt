# T07 — SageSDI `SageDefine.h` 상수 분류표

원본: `D:/Projects/SageSDI/SageSDI/SageDefine.h` (509줄, `constexpr` 477개). 사용 수 = `SageSDI/` 아래 `*.cpp` · `*.h` 중 `external/`과 `SageDefine.h` 자신을 뺀 파일에서 이름이 단어 단위로 나오는 **파일 수** (Python 정규식 `(?<![A-Za-z0-9_])NAME(?![A-Za-z0-9_])`, 주석 속 등장도 셈). 대상 파일 125개.

이관 여부 확인 대상: SageQt `SageQt/define/SageDefine.h` · `infra/infra/db/SageDbDefine.h` · `infra/infra/auth/SagePasswordHashDefine.h` · `tools/font-probe/SageFontProbeDefine.h`, 그리고 T03 · T05 완료 문서의 "옮기지 않는 것" 결정.

`SageDesignDefine.h` 행에는 주제 번호를 붙이지 않았다 (T08이 파일을 만든다). 사용 위치 주제는 사용 파일로 알 수 있다.

## 1. 상수 표 (477행)

| # | 줄 | 이름 | 값 | 종류 | 사용 | 목적지 | 비고 |
|---|---|---|---|---|---|---|---|
| 1 | 3 | `WM_SAGE_WORKFLOW_COMPLETE` | `WM_APP + 101` | 창 메시지 | 2 — SageWorkspacePanel.cpp, SageWorkflowController.cpp | 옮기지 않음 — 창 메시지 |  |
| 2 | 4 | `WM_SAGE_RESULT_TABLE_CHANGED` | `WM_APP + 102` | 창 메시지 | 4 files | 옮기지 않음 — 창 메시지 |  |
| 3 | 5 | `WM_SAGE_RESULT_SELECTION_CHANGED` | `WM_APP + 103` | 창 메시지 | 4 files | 옮기지 않음 — 창 메시지 |  |
| 4 | 6 | `WM_SAGE_WORKFLOW_RUN_REQUESTED` | `WM_APP + 104` | 창 메시지 | 2 — SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | 옮기지 않음 — 창 메시지 |  |
| 5 | 7 | `WM_SAGE_WORKFLOW_INPUT_RESET` | `WM_APP + 105` | 창 메시지 | 2 — SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | 옮기지 않음 — 창 메시지 |  |
| 6 | 8 | `WM_SAGE_WORKSPACE_TAB_CHANGED` | `WM_APP + 106` | 창 메시지 | 2 — SageWorkspacePanel.cpp, SageSDIView.cpp | 옮기지 않음 — 창 메시지 |  |
| 7 | 9 | `WM_SAGE_WORKSPACE_STATUS` | `WM_APP + 107` | 창 메시지 | 2 — SageWorkspacePanel.cpp, SageSDIView.cpp | 옮기지 않음 — 창 메시지 |  |
| 8 | 10 | `WM_SAGE_WORKSPACE_STATE_CHANGED` | `WM_APP + 108` | 창 메시지 | 2 — SageWorkspacePanel.cpp, SageSDIView.cpp | 옮기지 않음 — 창 메시지 |  |
| 9 | 11 | `WM_SAGE_OPEN_OUTPUT_FOLDER` | `WM_APP + 109` | 창 메시지 | 2 — SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | 옮기지 않음 — 창 메시지 |  |
| 10 | 12 | `WM_SAGE_SIDEBAR_WORKFLOW` | `WM_APP + 110` | 창 메시지 | 2 — SageSidebarPanel.cpp, SageSDIView.cpp | 옮기지 않음 — 창 메시지 |  |
| 11 | 13 | `WM_SAGE_SIDEBAR_ACTION` | `WM_APP + 111` | 창 메시지 | 2 — SageSidebarPanel.cpp, SageSDIView.cpp | 옮기지 않음 — 창 메시지 |  |
| 12 | 14 | `WM_SAGE_COPYGLOBALDATA` | `0x0049` | 창 메시지 | 2 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp | 옮기지 않음 — 창 메시지 | Win32 WM_COPYGLOBALDATA(0x0049)를 ChangeWindowMessageFilterEx로 허용 — 드래그 앤 드롭 UIPI 우회. 이름은 WM_SAGE_지만 앱 메시지 아님 |
| 13 | 15 | `SAGE_KEY_SELECT_ALL` | `'A'` | 기타 | 1 — SageEdit.cpp | 옮기지 않음 — 기타: SageEdit의 Ctrl+A 직접 처리(WM_KEYDOWN·VK_CONTROL). QLineEdit 기본 단축키(macOS Cmd+A)로 대체 | UINT 'A' (가상 키 코드) |
| 14 | 16 | `ID_SAGE_INPUT_EDIT` | `41001` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 15 | 17 | `ID_SAGE_OUTPUT_EDIT` | `41002` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 16 | 18 | `ID_SAGE_SELECT_INPUT` | `41003` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 17 | 19 | `ID_SAGE_SELECT_OUTPUT` | `41004` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 18 | 20 | `ID_SAGE_GENERATE_WORKFLOW` | `41006` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 19 | 21 | `ID_SAGE_RESULT_LIST` | `41007` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 20 | 22 | `ID_SAGE_STATUS_CARD` | `41008` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 21 | 23 | `ID_SAGE_DETAIL_LIST` | `41009` | 컨트롤 ID | 1 — SageWorkflowHistoryPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 22 | 24 | `ID_SAGE_RESULT_FILTER_EDIT` | `41011` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 23 | 25 | `ID_SAGE_RESULT_SEARCH_BTN` | `41012` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 24 | 26 | `ID_SAGE_TASK_TABS` | `41013` | 컨트롤 ID | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 25 | 27 | `ID_SAGE_PROGRESS_TIMER` | `41014` | 타이머 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 타이머 ID | SetTimer ID. 컨트롤 ID(41xxx) 대역을 같이 씀. Qt는 QTimer 객체 |
| 26 | 28 | `ID_SAGE_SIDEBAR_TREE` | `41015` | 컨트롤 ID | 1 — SageSidebarPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 27 | 29 | `ID_SAGE_SELECT_ALL` | `41016` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 28 | 30 | `ID_SAGE_INPUT_SECTION` | `41017` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 29 | 31 | `ID_SAGE_RESULT_SECTION` | `41019` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 30 | 32 | `ID_SAGE_DETAIL_EMPTY` | `41020` | 컨트롤 ID | 1 — SageWorkflowHistoryPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 31 | 33 | `ID_SAGE_DETAIL_FILTER` | `41094` | 컨트롤 ID | 1 — SageWorkflowHistoryPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 32 | 34 | `ID_SAGE_SIDEBAR_PANEL` | `41096` | 컨트롤 ID | 1 — SageSDIView.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 33 | 35 | `ID_SAGE_HEADER_PANEL` | `41097` | 컨트롤 ID | 1 — SageSDIView.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 34 | 36 | `ID_SAGE_ROLE_BADGE` | `41098` | 컨트롤 ID | 1 — SageHeaderPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 35 | 37 | `ID_SAGE_SUMMARY_BADGE` | `41099` | 컨트롤 ID | 1 — SageSummaryBar.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 36 | 38 | `ID_SAGE_RESULT_RESET_BTN` | `41024` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 37 | 39 | `ID_SAGE_INPUT_RESET_BTN` | `41025` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 38 | 40 | `ID_SAGE_RESULT_FILTER_CRITERIA` | `41027` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 39 | 41 | `ID_SAGE_RESULT_FILTER_BOX` | `41028` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 40 | 43 | `SAGE_SIDEBAR_ACTION_NONE` | `0` | 업무 | 1 — SageSidebarPanel.cpp | SageDefine.h (T11) | DWORD_PTR 트리 항목 데이터. Qt에선 enum class + item data role 후보 / 타입 DWORD_PTR |
| 41 | 44 | `SAGE_SIDEBAR_ACTION_CHANGE_PASSWORD` | `10001` | 업무 | 1 — SageSidebarPanel.cpp | SageDefine.h (T11) | DWORD_PTR 트리 항목 데이터(10001). Qt에선 enum class 후보 / 타입 DWORD_PTR |
| 42 | 45 | `SAGE_PROCESS_TIMEOUT_MS` | `600000` | 업무 | 1 — SageFileUtils.cpp | 옮기지 않음 — 기타: SageFileUtils::RunProcessAndWait(호출 0곳, MIGRATION_PLAN 죽은 코드)에서만 사용 · WaitForSingleObject | DWORD (10분) / 타입 DWORD |
| 43 | 47 | `SAGE_MARGIN` | `16` | 여백·크기 | 4 files | SageDesignDefine.h |  |
| 44 | 48 | `SAGE_BUTTON_WIDTH` | `120` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 45 | 49 | `SAGE_BUTTON_HEIGHT` | `32` | 여백·크기 | 8 files | SageDesignDefine.h |  |
| 46 | 50 | `SAGE_EDIT_HEIGHT` | `32` | 여백·크기 | 7 files | SageDesignDefine.h | SAGE_BUTTON_HEIGHT와 같은 값 32 |
| 47 | 51 | `SAGE_ROW_GAP` | `10` | 여백·크기 | 4 files | SageDesignDefine.h |  |
| 48 | 52 | `SAGE_PROGRESS_TEXT_WIDTH` | `54` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 49 | 53 | `SAGE_PROGRESS_TIMER_MS` | `300` | 업무 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T14) | 시간 기반 가짜 진행률 (MIGRATION_PLAN) |
| 50 | 54 | `SAGE_PROGRESS_STEP` | `3` | 업무 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T14) |  |
| 51 | 55 | `SAGE_PROGRESS_RUNNING_MAX` | `95` | 업무 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T14) |  |
| 52 | 56 | `SAGE_PROGRESS_COMPLETE` | `100` | 업무 | 1 — SageStatusCard.cpp | SageDefine.h (T14) |  |
| 53 | 57 | `SAGE_LIST_NO_ITEM` | `-1` | 업무 | 3 — SageWorkflowResultTable.cpp, SageFilterPillBar.cpp, SageListCtrl.cpp | SageDefine.h (T15, T16) | -1 없음 표시값. WorkflowResultTable 사용처는 DistributeColumnWidths(T03 미이관). FilterPillBar=T16 |
| 54 | 58 | `SAGE_LIST_ROW_HEIGHT` | `34` | 여백·크기 | 1 — SageListCtrl.cpp | SageDesignDefine.h |  |
| 55 | 59 | `SAGE_LIST_HEADER_HEIGHT` | `36` | 여백·크기 | 1 — SageHeaderCtrl.cpp | SageDesignDefine.h |  |
| 56 | 60 | `SAGE_LIST_ROW_SPACER_WIDTH` | `1` | 여백·크기 | 1 — SageListCtrl.cpp | 옮기지 않음 — 기타: 행 높이를 맞추려는 CImageList(LVSIL_SMALL) 요령 — Win32 전용 |  |
| 57 | 61 | `SAGE_LIST_CHECK_IMAGE_WIDTH` | `20` | 여백·크기 | 1 — SageListCtrl.cpp | 옮기지 않음 — 기타: 체크 상태 CImageList 이미지 폭 — Win32 전용 (Qt delegate가 직접 그리면 필요 여부 T15 확인) |  |
| 58 | 62 | `SAGE_LIST_CHECK_BOX_SIZE` | `14` | 여백·크기 | 2 — SageListCtrl.cpp, SageOptionCheck.cpp | SageDesignDefine.h |  |
| 59 | 63 | `SAGE_LIST_CHECK_STATE_COUNT` | `2` | 업무 | 1 — SageListCtrl.cpp | 옮기지 않음 — 기타: 체크 상태 CImageList 이미지 개수 — Win32 전용 |  |
| 60 | 64 | `SAGE_LIST_CHECK_STATE_CHECKED` | `1` | 업무 | 1 — SageListCtrl.cpp | 옮기지 않음 — 기타: 체크 상태 CImageList 인덱스 — Win32 전용 |  |
| 61 | 65 | `SAGE_LIST_CHECK_MARK_THICKNESS` | `2` | 여백·크기 | 1 — SageUiStyle.cpp | SageDesignDefine.h | 사용처 UiStyle (이름은 LIST) |
| 62 | 66 | `SAGE_LIST_CHECK_ACCENT_GAP` | `2` | 여백·크기 | 1 — SageListCtrl.cpp | SageDesignDefine.h |  |
| 63 | 67 | `SAGE_COLOR_IMAGE_MASK` | `RGB(255, 0, 255)` | 색 | 1 — SageListCtrl.cpp | 옮기지 않음 — 기타: CImageList 마스크 색(마젠타) — Win32 전용, 화면에 보이는 색 아님 |  |
| 64 | 68 | `SAGE_LIST_GRID_THICKNESS` | `1` | 여백·크기 | 2 — SageListBox.cpp, SageListCtrl.cpp | SageDesignDefine.h |  |
| 65 | 69 | `SAGE_LIST_BOX_ROW_HEIGHT` | `32` | 여백·크기 | 1 — SageListBox.cpp | 옮기지 않음 — 기타: 쓰는 CSageListBox 클래스 자체가 외부 사용 0곳 |  |
| 66 | 70 | `SAGE_LIST_BOX_TEXT_PAD_X` | `12` | 여백·크기 | 1 — SageListBox.cpp | 옮기지 않음 — 기타: 쓰는 CSageListBox 클래스 자체가 외부 사용 0곳 |  |
| 67 | 71 | `SAGE_LIST_CELL_RIGHT_PAD` | `6` | 여백·크기 | 2 — SageListCtrl.cpp, SageTableTotalBar.cpp | SageDesignDefine.h |  |
| 68 | 72 | `SAGE_LIST_CELL_LEFT_PAD` | `6` | 여백·크기 | 1 — SageTableTotalBar.cpp | SageDesignDefine.h |  |
| 69 | 73 | `SAGE_LIST_NO_BADGE_COLUMN` | `-1` | 업무 | 1 — SageListCtrl.cpp | SageDefine.h (T15) | -1 없음 표시값 (SAGE_LIST_NO_ITEM과 같은 값) |
| 70 | 74 | `SAGE_LIST_BADGE_HEIGHT` | `20` | 여백·크기 | 1 — SageListCtrl.cpp | SageDesignDefine.h | SAGE_BADGE_HEIGHT와 같은 값 20 |
| 71 | 75 | `SAGE_LIST_BADGE_PAD_X` | `8` | 여백·크기 | 1 — SageListCtrl.cpp | SageDesignDefine.h | SAGE_BADGE_PAD_X와 같은 값 8 |
| 72 | 76 | `SAGE_LIST_BADGE_RADIUS` | `4` | 여백·크기 | 1 — SageListCtrl.cpp | SageDesignDefine.h | SAGE_BADGE_RADIUS와 같은 값 4 |
| 73 | 77 | `SAGE_BORDER_THICKNESS` | `1` | 여백·크기 | 18 files | SageDesignDefine.h |  |
| 74 | 78 | `SAGE_INLINE_MSG_HEIGHT` | `24` | 여백·크기 | 2 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp | SageDesignDefine.h |  |
| 75 | 79 | `SAGE_INLINE_MSG_ICON_SIZE` | `14` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 76 | 80 | `SAGE_INLINE_MSG_ICON_GAP` | `8` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 77 | 81 | `SAGE_INLINE_MSG_BOX_PAD_X` | `10` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 78 | 82 | `SAGE_INLINE_MSG_BOX_RADIUS` | `4` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 79 | 83 | `SAGE_INLINE_ICON_RADIUS` | `5` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 80 | 84 | `SAGE_INLINE_ICON_STEM_TOP` | `4` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 81 | 85 | `SAGE_INLINE_ICON_STEM_BOTTOM` | `8` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 82 | 86 | `SAGE_INLINE_ICON_DOT_TOP` | `10` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 83 | 87 | `SAGE_INLINE_ICON_DOT_SIZE` | `2` | 여백·크기 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 84 | 88 | `SAGE_EMPTY_ICON_BOX_SIZE` | `44` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 85 | 89 | `SAGE_EMPTY_ICON_BOX_RADIUS` | `8` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 86 | 90 | `SAGE_EMPTY_ICON_SIZE` | `22` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 87 | 91 | `SAGE_EMPTY_ICON_INSET_X` | `3` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 88 | 92 | `SAGE_EMPTY_ICON_INSET_Y` | `4` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 89 | 93 | `SAGE_EMPTY_ICON_HEADER_OFFSET` | `5` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 90 | 94 | `SAGE_EMPTY_ICON_DIVIDER_OFFSET` | `6` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 91 | 95 | `SAGE_EMPTY_BLOCK_GAP` | `12` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 92 | 96 | `SAGE_EMPTY_TITLE_HEIGHT` | `22` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 93 | 97 | `SAGE_EMPTY_DESC_MAX_WIDTH` | `420` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 94 | 98 | `SAGE_EMPTY_ACTION_WIDTH` | `110` | 여백·크기 | 1 — SageEmptyState.cpp | SageDesignDefine.h |  |
| 95 | 99 | `SAGE_ICON_SIZE` | `15` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 96 | 100 | `SAGE_ICON_ADD_SIZE` | `14` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 97 | 101 | `SAGE_ICON_STROKE` | `2` | 여백·크기 | 3 — SageButton.cpp, SageMessageBody.cpp, SageUiStyle.cpp | SageDesignDefine.h |  |
| 98 | 102 | `SAGE_ICON_ADD_SPAN` | `10` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 99 | 103 | `SAGE_ICON_TEXT_GAP` | `6` | 여백·크기 | 3 — SageButton.cpp, SageOptionCheck.cpp, SageSelectionBar.cpp | SageDesignDefine.h |  |
| 100 | 104 | `SAGE_ICON_SEARCH_RADIUS` | `5` | 여백·크기 | 1 — SageUiStyle.cpp | SageDesignDefine.h |  |
| 101 | 105 | `SAGE_ICON_SEARCH_HANDLE` | `4` | 여백·크기 | 1 — SageUiStyle.cpp | SageDesignDefine.h |  |
| 102 | 106 | `SAGE_ICON_RESET_RADIUS` | `6` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 103 | 107 | `SAGE_ICON_RESET_ARROW` | `3` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 104 | 108 | `SAGE_ICON_CLOSE_SPAN` | `10` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 105 | 109 | `SAGE_ICON_ARROW_HALF_WIDTH` | `4` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 106 | 110 | `SAGE_ICON_ARROW_HALF_HEIGHT` | `2` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 107 | 112 | `SAGE_DLG_CAPTION_HEIGHT` | `40` | 여백·크기 | 2 — SageFramelessDialog.cpp, SageDialogCaptionBar.cpp | SageDesignDefine.h |  |
| 108 | 113 | `SAGE_DLG_CAPTION_PAD` | `16` | 여백·크기 | 1 — SageDialogCaptionBar.cpp | SageDesignDefine.h |  |
| 109 | 114 | `SAGE_DLG_CAPTION_BTN_SIZE` | `28` | 여백·크기 | 1 — SageDialogCaptionBar.cpp | SageDesignDefine.h |  |
| 110 | 115 | `SAGE_DLG_CAPTION_BTN_PAD` | `8` | 여백·크기 | 1 — SageDialogCaptionBar.cpp | SageDesignDefine.h |  |
| 111 | 116 | `ID_SAGE_DLG_CLOSE` | `41200` | 컨트롤 ID | 2 — SageFramelessDialog.cpp, SageDialogCaptionBar.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 112 | 117 | `SAGE_UI_TIP_CLOSE` | `L"닫기"` | UI 문자열 | 1 — SageDialogCaptionBar.cpp | SageDefine.h (T09) | 캡션 닫기 버튼 툴팁 |
| 113 | 119 | `SAGE_MSGBOX_WIDTH` | `360` | 여백·크기 | 1 — SageMessageBoxDlg.cpp | SageDesignDefine.h |  |
| 114 | 120 | `SAGE_MSGBOX_TEMPLATE_CX` | `230` | 여백·크기 | 1 — SageMessageBoxDlg.cpp | 옮기지 않음 — 기타: DLGTEMPLATE 크기(대화상자 단위) — T09가 제외 (MFC 전용) |  |
| 115 | 121 | `SAGE_MSGBOX_TEMPLATE_CY` | `100` | 여백·크기 | 1 — SageMessageBoxDlg.cpp | 옮기지 않음 — 기타: DLGTEMPLATE 크기(대화상자 단위) — T09가 제외 (MFC 전용) |  |
| 116 | 122 | `SAGE_MSGBOX_MAX_TEXT_HEIGHT` | `200` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 117 | 123 | `SAGE_MSGBOX_ICON_SIZE` | `22` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 118 | 124 | `SAGE_MSGBOX_ICON_RADIUS` | `10` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 119 | 125 | `SAGE_MSGBOX_ICON_TEXT_GAP` | `12` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 120 | 126 | `SAGE_MSGBOX_ICON_DOT_SIZE` | `2` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 121 | 127 | `SAGE_MSGBOX_ALERT_STEM_TOP` | `6` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 122 | 128 | `SAGE_MSGBOX_ALERT_STEM_BOTTOM` | `13` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 123 | 129 | `SAGE_MSGBOX_ALERT_DOT_TOP` | `15` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 124 | 130 | `SAGE_MSGBOX_INFO_DOT_TOP` | `5` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 125 | 131 | `SAGE_MSGBOX_INFO_STEM_TOP` | `9` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 126 | 132 | `SAGE_MSGBOX_INFO_STEM_BOTTOM` | `16` | 여백·크기 | 1 — SageMessageBody.cpp | SageDesignDefine.h |  |
| 127 | 133 | `SAGE_UI_MSGBOX_TITLE_INFO` | `L"알림"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) |  |
| 128 | 134 | `SAGE_UI_MSGBOX_TITLE_WARNING` | `L"경고"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) |  |
| 129 | 135 | `SAGE_UI_MSGBOX_TITLE_ERROR` | `L"오류"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) |  |
| 130 | 136 | `SAGE_UI_MSGBOX_TITLE_CONFIRM` | `L"확인"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) | SAGE_UI_MSGBOX_OK와 같은 값 "확인" |
| 131 | 137 | `SAGE_UI_MSGBOX_OK` | `L"확인"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) | SAGE_UI_MSGBOX_TITLE_CONFIRM과 같은 값 "확인" |
| 132 | 138 | `SAGE_UI_MSGBOX_YES` | `L"예"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) |  |
| 133 | 139 | `SAGE_UI_MSGBOX_NO` | `L"아니오"` | UI 문자열 | 1 — SageMessageBoxDlg.cpp | SageDefine.h (T09) |  |
| 134 | 140 | `SAGE_RESULT_MIN_HEIGHT` | `160` | 여백·크기 | 3 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | SageDesignDefine.h |  |
| 135 | 141 | `SAGE_COMBO_FIELD_INSET` | `6` | 여백·크기 | 2 — SageFilterComboBox.cpp, SageSearchBox.cpp | SageDesignDefine.h |  |
| 136 | 142 | `SAGE_COMBO_FIT_MAX_PASS` | `3` | 업무 | 1 — SageComboBox.cpp | 옮기지 않음 — 기타: 쓰는 CSageComboBox 클래스 자체가 외부 사용 0곳 · COMBOBOXINFO 높이 맞춤 반복 횟수(Win32) |  |
| 137 | 143 | `SAGE_EDIT_TEXT_TOP_PAD` | `9` | 여백·크기 | 3 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp, SageWorkflowInputPanel.cpp | SageDesignDefine.h | EM_SETRECT 서식 사각형 안쪽 여백 → Qt는 textMargins/스타일로 |
| 138 | 144 | `SAGE_EDIT_TEXT_LEFT_PAD` | `10` | 여백·크기 | 2 — SageSearchBox.cpp, SageWorkflowInputPanel.cpp | SageDesignDefine.h | EM_SETRECT 서식 사각형 안쪽 여백 → Qt는 textMargins/스타일로 |
| 139 | 145 | `SAGE_SEARCH_ICON_CELL_WIDTH` | `32` | 여백·크기 | 2 — SageSearchBox.cpp, SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 140 | 146 | `SAGE_SEARCH_CRITERIA_CELL_WIDTH` | `92` | 여백·크기 | 2 — SageSearchBox.cpp, SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 141 | 147 | `SAGE_EDIT_FORMAT_MAX_WIDTH` | `32767` | 기타 | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 기타: EM_SETRECT 서식 사각형 오른쪽 끝(32767) — Win32 전용 |  |
| 142 | 148 | `SAGE_BUTTON_VERT_ADJUST` | `2` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h | GDI 배치 보정값 — Qt에서 필요 여부 측정 후 판단 |
| 143 | 149 | `SAGE_BUTTON_TEXT_TOP_OFFSET` | `0` | 여백·크기 | 1 — SageButton.cpp | SageDesignDefine.h | GDI 텍스트 위치 보정값(0) — Qt에서 필요 여부 측정 후 판단 |
| 144 | 150 | `SAGE_TITLE_FONT_POINT_SIZE` | `143` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(14.3pt). font-probe ROLES title·logo에 같은 값 (측정 도구 전용) |
| 145 | 151 | `SAGE_CONTROL_FONT_POINT_SIZE` | `105` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(10.5pt). SAGE_CONTENT_FONT_POINT_SIZE와 같은 값. font-probe ROLES에 같은 값 (측정 도구 전용) |
| 146 | 152 | `SAGE_CONTENT_FONT_POINT_SIZE` | `105` | 폰트 | 3 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp, SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(10.5pt). SAGE_CONTROL_FONT_POINT_SIZE와 같은 값. font-probe ROLES에 같은 값 (측정 도구 전용) |
| 147 | 153 | `SAGE_HEADER_FONT_POINT_SIZE` | `113` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(11.3pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| 148 | 154 | `SAGE_LIST_FONT_POINT_SIZE` | `98` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(9.8pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| 149 | 155 | `SAGE_CAPTION_FONT_POINT_SIZE` | `90` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(9.0pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| 150 | 156 | `SAGE_SUMMARY_FONT_POINT_SIZE` | `128` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | 0.1pt 단위(12.8pt). font-probe ROLES에 같은 값 (측정 도구 전용) |
| 151 | 157 | `SAGE_SIDEBAR_WIDTH` | `220` | 여백·크기 | 1 — SageSDIView.cpp | SageDesignDefine.h |  |
| 152 | 158 | `SAGE_TAB_HEIGHT` | `40` | 여백·크기 | 2 — SageTabCtrl.cpp, SageWorkspacePanel.cpp | SageDesignDefine.h |  |
| 153 | 159 | `SAGE_TAB_INDICATOR_HEIGHT` | `2` | 여백·크기 | 1 — SageTabCtrl.cpp | SageDesignDefine.h |  |
| 154 | 160 | `SAGE_ACTION_GAP` | `8` | 여백·크기 | 2 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 155 | 161 | `SAGE_BADGE_HEIGHT` | `20` | 여백·크기 | 1 — SageBadge.cpp | SageDesignDefine.h | SAGE_LIST_BADGE_HEIGHT와 같은 값 20 |
| 156 | 162 | `SAGE_BADGE_PAD_X` | `8` | 여백·크기 | 2 — SageBadge.cpp, SageSummaryBar.cpp | SageDesignDefine.h | SAGE_LIST_BADGE_PAD_X와 같은 값 8 |
| 157 | 163 | `SAGE_BADGE_RADIUS` | `4` | 여백·크기 | 1 — SageSummaryBar.cpp | SageDesignDefine.h | SAGE_LIST_BADGE_RADIUS와 같은 값 4. 사용처는 SummaryBar뿐 (SageBadge는 안 씀) |
| 158 | 164 | `SAGE_HEADER_GAP` | `12` | 여백·크기 | 1 — SageHeaderPanel.cpp | SageDesignDefine.h |  |
| 159 | 165 | `SAGE_HEADER_TITLE_GAP` | `10` | 여백·크기 | 1 — SageHeaderPanel.cpp | SageDesignDefine.h |  |
| 160 | 166 | `SAGE_HEADER_CATEGORY_WIDTH` | `80` | 여백·크기 | 1 — SageHeaderPanel.cpp | SageDesignDefine.h |  |
| 161 | 167 | `SAGE_SIDEBAR_PAD_X` | `20` | 여백·크기 | 2 — SageSidebarTree.cpp, SageSidebarPanel.cpp | SageDesignDefine.h |  |
| 162 | 168 | `SAGE_SIDEBAR_CATEGORY_CHAR_EXTRA` | `1` | 여백·크기 | 1 — SageSidebarTree.cpp | SageDesignDefine.h | SetTextCharacterExtra 자간(px) → QFont::setLetterSpacing |
| 163 | 169 | `SAGE_SIDEBAR_TREE_TOP_PAD` | `16` | 여백·크기 | 1 — SageSidebarPanel.cpp | SageDesignDefine.h |  |
| 164 | 170 | `SAGE_SIDEBAR_ITEM_HEIGHT` | `34` | 여백·크기 | 1 — SageSidebarPanel.cpp | SageDesignDefine.h |  |
| 165 | 171 | `SAGE_SELECTION_ACCENT_WIDTH` | `3` | 여백·크기 | 1 — SageSidebarTree.cpp | SageDesignDefine.h | SAGE_LIST_SELECTION_ACCENT_WIDTH(4)와 값이 다름(3) |
| 166 | 172 | `SAGE_LIST_SELECTION_ACCENT_WIDTH` | `4` | 여백·크기 | 2 — SageListBox.cpp, SageListCtrl.cpp | SageDesignDefine.h | SAGE_SELECTION_ACCENT_WIDTH(3)와 값이 다름(4) |
| 167 | 173 | `SAGE_HEADER_HEIGHT` | `56` | 여백·크기 | 2 — SageSidebarPanel.cpp, SageSDIView.cpp | SageDesignDefine.h |  |
| 168 | 174 | `SAGE_CONTENT_PAD_X` | `24` | 여백·크기 | 2 — SageHeaderPanel.cpp, SageWorkspacePanel.cpp | SageDesignDefine.h |  |
| 169 | 175 | `SAGE_CONTENT_PAD_Y` | `20` | 여백·크기 | 1 — SageWorkspacePanel.cpp | SageDesignDefine.h |  |
| 170 | 176 | `SAGE_CARD_HEADER_HEIGHT` | `38` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 171 | 177 | `SAGE_CARD_PADDING` | `16` | 여백·크기 | 3 — SageSectionLabel.cpp, SageStatusCard.cpp, SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 172 | 178 | `SAGE_CARD_ROW_GAP` | `12` | 여백·크기 | 3 — SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp, SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 173 | 179 | `SAGE_CARD_GAP` | `16` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 174 | 180 | `SAGE_CARD_ACTION_BUTTON_HEIGHT` | `34` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 175 | 181 | `SAGE_FORM_LABEL_WIDTH` | `64` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h | SAGE_LOGIN_DLG_LABEL_WIDTH와 같은 값 64 |
| 176 | 182 | `SAGE_STATUS_CARD_HEIGHT` | `70` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 177 | 183 | `SAGE_STATUS_CARD_DOT_SIZE` | `8` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 178 | 184 | `SAGE_STATUS_CARD_DOT_GAP` | `8` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 179 | 185 | `SAGE_STATUS_CARD_ICON_SIZE` | `18` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 180 | 186 | `SAGE_STATUS_CARD_ICON_GAP` | `12` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 181 | 187 | `SAGE_STATUS_CARD_ICON_RADIUS` | `7` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 182 | 188 | `SAGE_STATUS_CARD_ICON_THICKNESS` | `2` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 183 | 189 | `SAGE_STATUS_CARD_CHECK_START_X` | `-3` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 184 | 190 | `SAGE_STATUS_CARD_CHECK_START_Y` | `0` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 185 | 191 | `SAGE_STATUS_CARD_CHECK_MID_X` | `-1` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 186 | 192 | `SAGE_STATUS_CARD_CHECK_MID_Y` | `3` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 187 | 193 | `SAGE_STATUS_CARD_CHECK_END_X` | `3` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 188 | 194 | `SAGE_STATUS_CARD_CHECK_END_Y` | `-2` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 189 | 195 | `SAGE_STATUS_CARD_ALERT_STEM_TOP` | `-4` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 190 | 196 | `SAGE_STATUS_CARD_ALERT_STEM_BOTTOM` | `1` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 191 | 197 | `SAGE_STATUS_CARD_ALERT_DOT_TOP` | `3` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 192 | 198 | `SAGE_STATUS_CARD_ALERT_DOT_SIZE` | `2` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 193 | 199 | `SAGE_STATUS_CARD_PROGRESS_HEIGHT` | `6` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 194 | 200 | `SAGE_STATUS_CARD_TITLE_LINE_HEIGHT` | `20` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 195 | 201 | `SAGE_STATUS_CARD_DETAIL_LINE_HEIGHT` | `16` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 196 | 202 | `SAGE_STATUS_CARD_ACTION_WIDTH` | `80` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h | SAGE_STATUS_CARD_ACTION_AREA_WIDTH 계산에도 쓰임 |
| 197 | 203 | `SAGE_STATUS_CARD_ACTION_GAP` | `12` | 여백·크기 | 0 | SageDesignDefine.h | 외부 파일 사용 0곳이지만 SageDefine.h 안에서 SAGE_STATUS_CARD_ACTION_AREA_WIDTH 계산에 쓰임 → 함께 옮김 |
| 198 | 204 | `SAGE_STATUS_CARD_ACTION_AREA_WIDTH` | `SAGE_STATUS_CARD_ACTION_WIDTH + SAGE_STATUS_CARD_ACTION_GAP` | 여백·크기 | 1 — SageStatusCard.cpp | SageDesignDefine.h | = 80 + 12 = 92 (두 상수로 계산) |
| 199 | 206 | `SAGE_EDIT_BORDER_WIDTH` | `1` | 여백·크기 | 3 — SageSearchBox.cpp, SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | SageDesignDefine.h | SAGE_BORDER_THICKNESS와 같은 값 1 |
| 200 | 207 | `SAGE_RESULT_HEADER_HEIGHT` | `26` | 여백·크기 | 3 — SageResultTablePanel.cpp, SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | SageDesignDefine.h |  |
| 201 | 208 | `SAGE_HISTORY_TIME_WIDTH` | `124` | 여백·크기 | 1 — SageWorkflowHistoryPanel.cpp | SageDesignDefine.h | DistributeColumnWidths용 열 폭 — T16에서 QHeaderView로 대체 시 필요 여부 확인 |
| 202 | 209 | `SAGE_HISTORY_RESULT_WIDTH` | `88` | 여백·크기 | 1 — SageWorkflowHistoryPanel.cpp | SageDesignDefine.h | 위와 같음 (T16 확인) |
| 203 | 210 | `SAGE_HISTORY_INPUT_WIDTH` | `296` | 여백·크기 | 1 — SageWorkflowHistoryPanel.cpp | SageDesignDefine.h | 위와 같음 (T16 확인) |
| 204 | 211 | `SAGE_HISTORY_OUTPUT_WIDTH` | `360` | 여백·크기 | 1 — SageWorkflowHistoryPanel.cpp | SageDesignDefine.h | 위와 같음 (T16 확인) |
| 205 | 212 | `SAGE_HISTORY_REASON_WIDTH` | `208` | 여백·크기 | 1 — SageWorkflowHistoryPanel.cpp | SageDesignDefine.h | 위와 같음 (T16 확인) |
| 206 | 213 | `SAGE_HISTORY_STATE_SUCCESS` | `0` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 207 | 214 | `SAGE_HISTORY_STATE_FAILED` | `1` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 208 | 215 | `SAGE_HISTORY_FILTER_SUCCESS` | `1` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 209 | 216 | `SAGE_HISTORY_FILTER_FAILED` | `2` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 210 | 217 | `SAGE_PILL_HEIGHT` | `28` | 여백·크기 | 2 — SageFilterPillBar.cpp, SageWorkflowHistoryPanel.cpp | SageDesignDefine.h |  |
| 211 | 218 | `SAGE_PILL_PAD_X` | `12` | 여백·크기 | 1 — SageFilterPillBar.cpp | SageDesignDefine.h |  |
| 212 | 219 | `SAGE_PILL_GAP` | `8` | 여백·크기 | 1 — SageFilterPillBar.cpp | SageDesignDefine.h |  |
| 213 | 220 | `SAGE_PILL_RADIUS` | `14` | 여백·크기 | 1 — SageFilterPillBar.cpp | SageDesignDefine.h |  |
| 214 | 221 | `SAGE_RESULT_FIELD_WIDTH` | `140` | 여백·크기 | 1 — SageWorkflowResultTable.cpp | SageDesignDefine.h | WorkflowResultTable 열 폭 → DistributeColumnWidths(T03 미이관, T15에서 QHeaderView로 대체). 픽셀 폭 필요 여부 T15 확인 |
| 215 | 222 | `SAGE_RESULT_STATUS_WIDTH` | `110` | 여백·크기 | 1 — SageWorkflowResultTable.cpp | SageDesignDefine.h | 위와 같음 (T15 확인) |
| 216 | 223 | `SAGE_RESULT_REASON_WIDTH` | `320` | 여백·크기 | 1 — SageWorkflowResultTable.cpp | SageDesignDefine.h | 위와 같음 (T15 확인) |
| 217 | 224 | `SAGE_RESULT_MIN_VALUE_WIDTH` | `220` | 여백·크기 | 1 — SageWorkflowResultTable.cpp | SageDesignDefine.h | 위와 같음 (T15 확인). stretch 열의 최소 폭 |
| 218 | 225 | `SAGE_RESULT_FILTER_WIDTH` | `150` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 219 | 226 | `SAGE_RESULT_FILTER_BOX_PAD` | `4` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 220 | 227 | `SAGE_RESULT_FILTER_TOP_LIFT` | `8` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 221 | 228 | `SAGE_RESULT_CRITERIA_DROP_ROWS` | `8` | 업무 | 1 — SageResultTablePanel.cpp | SageDefine.h (T15) | 드롭다운에 보일 행 수(픽셀 아님) → QComboBox::setMaxVisibleItems 대응 |
| 222 | 229 | `SAGE_RESULT_FILTER_MAX_LENGTH` | `20` | 업무 | 1 — SageResultTablePanel.cpp | SageDefine.h (T15) | 검색어 최대 글자 수 → QLineEdit::setMaxLength |
| 223 | 230 | `SAGE_RESULT_RESET_WIDTH` | `84` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 224 | 231 | `SAGE_SUMMARY_BAR_HEIGHT` | `32` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 225 | 232 | `SAGE_SUMMARY_ITEM_GAP` | `16` | 여백·크기 | 1 — SageSummaryBar.cpp | SageDesignDefine.h |  |
| 226 | 233 | `SAGE_SUMMARY_TEXT_GAP` | `6` | 여백·크기 | 1 — SageSummaryBar.cpp | SageDesignDefine.h |  |
| 227 | 234 | `SAGE_SUMMARY_DIVIDER_HEIGHT` | `16` | 여백·크기 | 1 — SageSummaryBar.cpp | SageDesignDefine.h |  |
| 228 | 235 | `SAGE_TOTAL_BAR_HEIGHT` | `40` | 여백·크기 | 1 — SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 229 | 236 | `SAGE_SELECTION_BAR_GAP` | `12` | 여백·크기 | 1 — SageSelectionBar.cpp | SageDesignDefine.h |  |
| 230 | 237 | `SAGE_SELECTION_CHECK_GLYPH_WIDTH` | `20` | 여백·크기 | 1 — SageSelectionBar.cpp | SageDesignDefine.h |  |
| 231 | 238 | `SAGE_SELECTION_CLEAR_PAD` | `12` | 여백·크기 | 1 — SageSelectionBar.cpp | SageDesignDefine.h |  |
| 232 | 239 | `SAGE_OPTION_CHECK_PADDING` | `8` | 여백·크기 | 1 — SageOptionCheck.cpp | SageDesignDefine.h |  |
| 233 | 240 | `SAGE_INPUT_RESET_WIDTH` | `72` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h |  |
| 234 | 241 | `SAGE_TAB_INDEX_INPUT` | `0` | 업무 | 3 — SageSampleWorkflowHandler.cpp, SageWorkspacePanel.cpp, SageWorkspacePanel.h | 이관됨 (T03) `SageWorkflowTabKind::Input` | int → enum class |
| 235 | 242 | `SAGE_TAB_INDEX_DOCUMENT_RESULT` | `1` | 업무 | 2 — SageSampleWorkflowHandler.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SageWorkflowTabKind::DocumentResult` | int → enum class |
| 236 | 243 | `SAGE_TAB_INDEX_DOCUMENT_HISTORY` | `2` | 업무 | 2 — SageSampleWorkflowHandler.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SageWorkflowTabKind::DocumentHistory` | int → enum class |
| 237 | 244 | `SAGE_PRIVATE_FONT_COUNT` | `6` | 업무 | 2 — SageSDI.cpp, SageSDI.h | 옮기지 않음 — 기타: AddFontMemResourceEx 핸들 배열 크기 — T08은 쓰이는 폰트만 등록 (Win32 전용) |  |
| 238 | 245 | `SAGE_TITLE_FONT_FACE` | `L"Pretendard SemiBold"` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | GDI식 이름("패밀리 + 굵기"). Qt는 패밀리 "Pretendard" + QFont::DemiBold (T02). font-probe LEGACY_FACES에 같은 값 (측정 도구 전용) |
| 239 | 246 | `SAGE_CONTROL_FONT_FACE` | `L"Pretendard"` | 폰트 | 4 files | SageDesignDefine.h | GDI 패밀리 이름. font-probe SAGE_FONT_PROBE_FAMILY_PRETENDARD("Pretendard") · LEGACY_FACES에 같은 값 (측정 도구 전용) |
| 240 | 247 | `SAGE_LOGO_FONT_FACE` | `L"Gmarket Sans TTF Bold"` | 폰트 | 1 — SageUiResources.cpp | SageDesignDefine.h | GDI식 이름. Qt는 "Gmarket Sans TTF" + QFont::Bold (T02). font-probe LEGACY_FACES에 같은 값 (측정 도구 전용) |
| 241 | 248 | `SAGE_COLOR_APP_BACKGROUND` | `RGB(248, 246, 241)` | 색 | 12 files | SageDesignDefine.h |  |
| 242 | 249 | `SAGE_COLOR_PANEL` | `RGB(255, 255, 255)` | 색 | 23 files | SageDesignDefine.h | SAGE_COLOR_BUTTON_TEXT · SAGE_COLOR_SIDEBAR_SELECTED_TEXT와 같은 값 RGB(255, 255, 255) |
| 243 | 250 | `SAGE_COLOR_SIDEBAR` | `RGB(36, 31, 26)` | 색 | 4 files | SageDesignDefine.h |  |
| 244 | 251 | `SAGE_COLOR_SIDEBAR_TEXT` | `RGB(205, 196, 185)` | 색 | 3 — SageSidebarTree.cpp, SageUiResources.cpp, SageSidebarPanel.cpp | SageDesignDefine.h |  |
| 245 | 252 | `SAGE_COLOR_SIDEBAR_SELECTED` | `RGB(58, 49, 41)` | 색 | 1 — SageSidebarTree.cpp | SageDesignDefine.h |  |
| 246 | 253 | `SAGE_COLOR_SIDEBAR_CATEGORY` | `RGB(130, 120, 108)` | 색 | 2 — SageSidebarTree.cpp, SageUiResources.cpp | SageDesignDefine.h |  |
| 247 | 254 | `SAGE_COLOR_SIDEBAR_DIVIDER` | `RGB(51, 44, 37)` | 색 | 2 — SageSidebarPanel.cpp, SageSDIView.cpp | SageDesignDefine.h |  |
| 248 | 255 | `SAGE_COLOR_BORDER` | `RGB(220, 214, 205)` | 색 | 15 files | SageDesignDefine.h |  |
| 249 | 256 | `SAGE_COLOR_TEXT` | `RGB(47, 42, 36)` | 색 | 23 files | SageDesignDefine.h |  |
| 250 | 257 | `SAGE_COLOR_SECONDARY_TEXT` | `RGB(122, 112, 100)` | 색 | 11 files | SageDesignDefine.h |  |
| 251 | 258 | `SAGE_COLOR_TEXT_MUTED` | `RGB(110, 101, 91)` | 색 | 7 files | SageDesignDefine.h |  |
| 252 | 259 | `SAGE_COLOR_PRIMARY` | `RGB(154, 107, 63)` | 색 | 17 files | SageDesignDefine.h |  |
| 253 | 260 | `SAGE_COLOR_PRIMARY_PRESS` | `RGB(118, 80, 42)` | 색 | 2 — SageButton.cpp, SageListBox.cpp | SageDesignDefine.h |  |
| 254 | 261 | `SAGE_COLOR_BUTTON_TEXT` | `RGB(255, 255, 255)` | 색 | 1 — SageButton.cpp | SageDesignDefine.h | SAGE_COLOR_PANEL · SAGE_COLOR_SIDEBAR_SELECTED_TEXT와 같은 값 RGB(255, 255, 255) |
| 255 | 262 | `SAGE_COLOR_BUTTON_BORDER` | `RGB(201, 191, 177)` | 색 | 4 files | SageDesignDefine.h |  |
| 256 | 263 | `SAGE_COLOR_FOCUS_RING_PRIMARY` | `RGB(240, 228, 213)` | 색 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 257 | 264 | `SAGE_COLOR_FOCUS_RING_NEUTRAL` | `RGB(239, 235, 227)` | 색 | 1 — SageButton.cpp | SageDesignDefine.h |  |
| 258 | 265 | `SAGE_FOCUS_RING_WIDTH` | `2` | 여백·크기 | 2 — SageMessageBoxDlg.cpp, SageButton.cpp | SageDesignDefine.h |  |
| 259 | 266 | `SAGE_COLOR_DANGER_BORDER` | `RGB(224, 189, 182)` | 색 | 2 — SageButton.cpp, SageStatusCard.cpp | SageDesignDefine.h |  |
| 260 | 267 | `SAGE_COLOR_SUCCESS` | `RGB(95, 127, 95)` | 색 | 2 — SageStatusCard.cpp, SageUiResources.cpp | SageDesignDefine.h |  |
| 261 | 268 | `SAGE_COLOR_WARNING` | `RGB(184, 135, 70)` | 색 | 4 files | SageDesignDefine.h |  |
| 262 | 269 | `SAGE_COLOR_ERROR` | `RGB(184, 92, 74)` | 색 | 6 files | SageDesignDefine.h |  |
| 263 | 270 | `SAGE_COLOR_LIST_ROW_ALT` | `RGB(250, 248, 244)` | 색 | 2 — SageListBox.cpp, SageListCtrl.cpp | SageDesignDefine.h |  |
| 264 | 271 | `SAGE_COLOR_LIST_HEADER` | `RGB(242, 238, 231)` | 색 | 11 files | SageDesignDefine.h |  |
| 265 | 272 | `SAGE_COLOR_LIST_GRID` | `RGB(237, 232, 224)` | 색 | 4 files | SageDesignDefine.h |  |
| 266 | 273 | `SAGE_COLOR_LIST_ROW_SELECTED` | `RGB(241, 227, 205)` | 색 | 2 — SageListBox.cpp, SageListCtrl.cpp | SageDesignDefine.h |  |
| 267 | 274 | `SAGE_COLOR_TEXT_PLACEHOLDER` | `RGB(180, 171, 160)` | 색 | 5 files | SageDesignDefine.h |  |
| 268 | 275 | `SAGE_COLOR_INLINE_ERROR_TEXT` | `RGB(156, 68, 51)` | 색 | 3 — SageInlineError.cpp, SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp | SageDesignDefine.h |  |
| 269 | 276 | `SAGE_COLOR_INLINE_WARN_TEXT` | `RGB(138, 106, 50)` | 색 | 1 — SageInlineError.cpp | SageDesignDefine.h |  |
| 270 | 277 | `SAGE_COLOR_INLINE_WARN_BG` | `RGB(251, 245, 238)` | 색 | 2 — SageInlineError.cpp, SageResultTablePanel.cpp | SageDesignDefine.h | SAGE_COLOR_STATUS_BG_WARNING과 같은 값 RGB(251, 245, 238) |
| 271 | 278 | `SAGE_COLOR_INLINE_WARN_BORDER` | `RGB(235, 220, 198)` | 색 | 2 — SageInlineError.cpp, SageResultTablePanel.cpp | SageDesignDefine.h |  |
| 272 | 279 | `SAGE_COLOR_ACCENT_SURFACE` | `RGB(247, 242, 234)` | 색 | 2 — SageFilterPillBar.cpp, SageUiResources.cpp | SageDesignDefine.h |  |
| 273 | 280 | `SAGE_COLOR_STATUS_BG_SUCCESS` | `RGB(234, 244, 234)` | 색 | 1 — SageUiResources.cpp | SageDesignDefine.h |  |
| 274 | 281 | `SAGE_COLOR_STATUS_BG_WARNING` | `RGB(251, 245, 238)` | 색 | 1 — SageUiResources.cpp | SageDesignDefine.h | SAGE_COLOR_INLINE_WARN_BG와 같은 값 RGB(251, 245, 238) |
| 275 | 282 | `SAGE_COLOR_STATUS_BG_ERROR` | `RGB(248, 235, 233)` | 색 | 2 — SageUiResources.cpp, SageWorkflowHistoryPanel.cpp | SageDesignDefine.h |  |
| 276 | 283 | `SAGE_COLOR_STATUS_CARD_BG_SUCCESS` | `RGB(241, 245, 240)` | 색 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 277 | 284 | `SAGE_COLOR_STATUS_CARD_BORDER_SUCCESS` | `RGB(213, 224, 211)` | 색 | 1 — SageStatusCard.cpp | SageDesignDefine.h |  |
| 278 | 285 | `SAGE_COLOR_STATUS_CARD_TEXT_SUCCESS` | `RGB(65, 96, 63)` | 색 | 2 — SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp | SageDesignDefine.h |  |
| 279 | 286 | `SAGE_COLOR_STATUS_CARD_BG_ERROR` | `RGB(253, 246, 244)` | 색 | 2 — SageStatusCard.cpp, SageWorkflowHistoryPanel.cpp | SageDesignDefine.h |  |
| 280 | 287 | `SAGE_COLOR_BADGE_BG_SUCCESS` | `RGB(238, 244, 238)` | 색 | 1 — SageWorkflowHistoryPanel.cpp | SageDesignDefine.h |  |
| 281 | 288 | `SAGE_COLOR_SIDEBAR_SELECTED_TEXT` | `RGB(255, 255, 255)` | 색 | 1 — SageSidebarTree.cpp | SageDesignDefine.h | SAGE_COLOR_PANEL · SAGE_COLOR_BUTTON_TEXT와 같은 값 RGB(255, 255, 255) |
| 282 | 289 | `SAGE_COLOR_LIST_HEADER_BORDER` | `RGB(228, 223, 215)` | 색 | 3 — SageBadge.cpp, SageSearchBox.cpp, SageHeaderPanel.cpp | SageDesignDefine.h |  |
| 283 | 291 | `SAGE_WORKFLOW_NONE` | `0` | 업무 | 0 | 옮기지 않음 — 호출 0곳 |  |
| 284 | 292 | `SAGE_WORKFLOW_SAMPLE` | `1` | 업무 | 3 — SageSampleWorkflowHandler.cpp, SageSidebarPanel.cpp, SageSDIView.cpp | 이관됨 (T03) `SageWorkflowType::Sample` | int → enum class |
| 285 | 293 | `SAGE_WORKFLOW_DELIVERY` | `2` | 업무 | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 기타: 핸들러 없는 SageTaechang 잔재 — T03 결정으로 제외 (WorkspacePanel 초기값으로만 쓰임) |  |
| 286 | 295 | `SAGE_FILTER_CRITERIA_NONE` | `-1` | 업무 | 3 — SageSampleWorkflowHandler.cpp, SageResultTablePanel.cpp, SageWorkspacePanel.h | SageDefine.h (T15) | -1 없음 표시값. SageQt 샘플 핸들러는 filterCriteria() {} 반환이라 T03엔 불필요했음. Qt에선 std::optional 후보 |
| 287 | 296 | `SAGE_TASK_LOAD` | `1` | 업무 | 2 — SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SageTaskType::Load` | int → enum class |
| 288 | 297 | `SAGE_TASK_GENERATE` | `2` | 업무 | 2 — SageWorkflowInputPanel.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SageTaskType::Generate` | int → enum class |
| 289 | 299 | `SAGE_UI_APP_TITLE` | `L"SageSDI"` | UI 문자열 | 1 — SageSidebarPanel.cpp | SageDefine.h (T11) | 사이드바 로고 문구 "SageSDI". SageQt에 SAGE_APPLICATION_NAME · SAGE_UI_MAIN_WINDOW_TITLE = "SageQt"가 있음 — 재사용 여부 T11에서 결정 |
| 290 | 300 | `SAGE_UI_RECEIVABLES_NAME` | `L"미수금 내역서"` | UI 문자열 | 1 — SageHeaderPanel.cpp | SageDefine.h (T12) | "미수금" = SageTaechang 잔재로 보임. 헤더 제목 초기값이고 SetWindowTextW(pszTitle)로 곧 교체됨 (SageHeaderPanel.cpp:30, 54) |
| 291 | 301 | `SAGE_UI_EMPTY_STATE_HINT` | `L"파일을 선택하여 업무를 시작하세요"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) |  |
| 292 | 302 | `SAGE_UI_INPUT_BUTTON` | `L"파일 선택"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) |  |
| 293 | 303 | `SAGE_UI_OUTPUT_BUTTON` | `L"폴더 선택"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) |  |
| 294 | 304 | `SAGE_UI_RECEIVABLES_GENERATE_BUTTON` | `L"내역서 생성"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) | "내역서" = SageTaechang 잔재로 보임. 버튼 초기값이고 SetWindowTextW(pszLabel)로 교체됨 (SageWorkflowInputPanel.cpp:77, 273) |
| 295 | 305 | `SAGE_UI_SELECT_ALL_BUTTON` | `L"전체 선택"` | UI 문자열 | 1 — SageSelectionBar.cpp | SageDefine.h (T15) |  |
| 296 | 306 | `SAGE_UI_SELECTION_CLEAR_BUTTON` | `L"선택 해제"` | UI 문자열 | 1 — SageSelectionBar.cpp | SageDefine.h (T15) |  |
| 297 | 307 | `SAGE_UI_SELECTION_TOTAL_FORMAT` | `L"%d건 중"` | UI 문자열 | 1 — SageSelectionBar.cpp | SageDefine.h (T15) | "%d" printf 형식 → QString::arg |
| 298 | 308 | `SAGE_UI_SELECTION_SELECTED_FORMAT` | `L"%d건"` | UI 문자열 | 1 — SageSelectionBar.cpp | SageDefine.h (T15) | "%d" printf 형식 → QString::arg |
| 299 | 309 | `SAGE_UI_SELECTION_SUFFIX` | `L"선택됨"` | UI 문자열 | 1 — SageSelectionBar.cpp | SageDefine.h (T15) |  |
| 300 | 310 | `SAGE_UI_READY` | `L"대기 중"` | UI 문자열 | 2 — SageWorkspacePanel.cpp, SageSDIView.cpp | SageDefine.h (T16) | T16 지시서: 상태 표시줄 시작 문구 |
| 301 | 311 | `SAGE_UI_RUNNING` | `L"처리 중"` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T16) |  |
| 302 | 312 | `SAGE_UI_DROP_RECEIVED` | `L"파일 드롭 수신"` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T16) |  |
| 303 | 313 | `SAGE_UI_DROP_PATH_SEPARATOR` | `L"\r\n"` | 업무 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T13) | 드롭 경로를 "\r\n"으로 이었다 Tokenize로 자름. Qt는 QMimeData::urls() 목록이라 필요 없을 수 있음 |
| 304 | 314 | `SAGE_UI_PROGRESS_FORMAT` | `L"%d%%"` | UI 문자열 | 1 — SageStatusCard.cpp | SageDefine.h (T14) | "%d%%" printf 형식 → QString::arg |
| 305 | 315 | `SAGE_UI_ROW_NUM_FORMAT` | `L"%lu"` | 업무 | 2 — SageResultTablePanel.cpp, SageWorkspacePanel.cpp | SageDefine.h (T15, T14) | "%lu" printf 형식 → QString::number. 체크한 행 번호를 rowNums payload 문자열로 직렬화 |
| 306 | 316 | `SAGE_UI_ROW_NUM_SEPARATOR` | `L","` | 업무 | 2 — SageResultTablePanel.cpp, SageWorkspacePanel.cpp | SageDefine.h (T15, T14) | SAGE_UI_AMOUNT_GROUP_SEPARATOR와 같은 값 ","이지만 뜻이 다름 |
| 307 | 317 | `SAGE_UI_COMPLETED` | `L"완료"` | UI 문자열 | 2 — SageWorkflowResultPresenter.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SAGE_UI_COMPLETED` | T16 상태 표시줄도 같은 상수를 쓴다 (T16 지시서) |
| 308 | 318 | `SAGE_UI_FAILED` | `L"실패"` | UI 문자열 | 2 — SageWorkflowResultPresenter.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SAGE_UI_FAILED` | T16 상태 표시줄도 같은 상수를 쓴다 (T16 지시서) |
| 309 | 319 | `SAGE_UI_STATUS_CARD_IDLE` | `L"대기 중 — 입력 파일을 선택한 뒤 생성하세요"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T14) | "—" 앞뒤 문구가 SAGE_UI_READY("대기 중")와 겹침 |
| 310 | 320 | `SAGE_UI_STATUS_CARD_RUNNING` | `L"처리 중 — 엑셀 데이터를 읽는 중입니다"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T14) | "—" 앞 문구가 SAGE_UI_RUNNING("처리 중")과 겹침 |
| 311 | 321 | `SAGE_UI_STATUS_CARD_COMPLETED_FORMAT` | `L"%s이 완료되었습니다 · %d건"` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T14) | %s·%d printf 형식 → QString::arg. "%s이" 조사 고정 |
| 312 | 322 | `SAGE_UI_STATUS_CARD_FAILED_FORMAT` | `L"%s에 실패했습니다"` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T14) | %s printf 형식 → QString::arg. "%s에" 조사 고정 |
| 313 | 323 | `SAGE_UI_STATUS_CARD_LOAD_COMPLETED_FORMAT` | `L"불러오기가 완료되었습니다 · %d건"` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T14) | %d printf 형식 → QString::arg |
| 314 | 324 | `SAGE_UI_STATUS_CARD_LOAD_FAILED` | `L"불러오기에 실패했습니다"` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T14) |  |
| 315 | 325 | `SAGE_UI_OUTPUT_PATH_MISSING` | `L"저장한 파일을 찾을 수 없습니다. 옮겨졌거나 삭제되었습니다."` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T14) |  |
| 316 | 326 | `SAGE_UI_STATUS_CARD_OPEN_FOLDER` | `L"폴더 열기"` | UI 문자열 | 1 — SageStatusCard.cpp | SageDefine.h (T14) |  |
| 317 | 327 | `SAGE_UI_EXPLORER_VERB_OPEN` | `L"open"` | 업무 | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 기타: ShellExecuteW 동사 — Windows 전용. QDesktopServices 등으로 대체 (T14) |  |
| 318 | 328 | `SAGE_UI_EXPLORER_COMMAND` | `L"explorer.exe"` | 업무 | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 기타: explorer.exe — Windows 전용 (T14에서 OS별 "폴더 열기" 방식 결정) |  |
| 319 | 329 | `SAGE_UI_EXPLORER_SELECT_FORMAT` | `L"/select,\"%s\""` | 업무 | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 기타: explorer.exe /select 인자 — Windows 전용 (T14) |  |
| 320 | 330 | `SAGE_UI_SELECT_OUTPUT_TITLE` | `L"저장 폴더 선택"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) |  |
| 321 | 331 | `SAGE_UI_INPUT_REQUIRED` | `L"파일을 선택하세요."` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T13) |  |
| 322 | 332 | `SAGE_UI_OUTPUT_REQUIRED` | `L"저장 위치 폴더를 지정하세요."` | UI 문자열 | 1 — SageWorkspacePanel.cpp | SageDefine.h (T13) |  |
| 323 | 333 | `SAGE_UI_WORKFLOW_EXCEPTION` | `L"작업 처리 중 예기치 못한 오류가 발생했습니다."` | UI 문자열 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 324 | 334 | `SAGE_UI_WORKFLOW_ALREADY_RUNNING` | `L"이미 처리 중입니다."` | UI 문자열 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 325 | 335 | `SAGE_UI_WORKFLOW_NOT_FOUND` | `L"등록된 업무가 없습니다."` | UI 문자열 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 326 | 336 | `SAGE_UI_WORKFLOW_START_FAILED` | `L"작업을 시작할 수 없습니다."` | UI 문자열 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 327 | 337 | `SAGE_ERROR_CODE_WORKFLOW_EXCEPTION` | `L"SNX_SAGE_WORKFLOW_001"` | 업무 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 328 | 338 | `SAGE_ERROR_CODE_WORKFLOW_NOT_FOUND` | `L"SNX_SAGE_WORKFLOW_002"` | 업무 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 329 | 339 | `SAGE_UI_EXCEL_FILTER` | `L"Excel Files (*.xls;*.xlsx)\|*.xls;*.xlsx\|All Files (*.*)\|*.*\|\|"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 기타: MFC CFileDialog 필터 형식("\|…\|\|"). T13에서 Qt 형식("… (*.xls *.xlsx);;…")으로 새로 정의 필요 |  |
| 330 | 340 | `SAGE_UI_EXCEL_DEFAULT_EXT` | `L"xls"` | 업무 | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 기타: CFileDialog 기본 확장자 인자 — 열기 대화상자라 Qt에서 쓸 곳 없음(판단) |  |
| 331 | 341 | `SAGE_UI_AMOUNT_EMPTY_MARK` | `L"—"` | UI 문자열 | 2 — SageListCtrl.cpp, SageWorkflowHistoryPanel.cpp | SageDefine.h (T15, T16) | em dash "—". 금액뿐 아니라 빈 칸 표시 전반에 씀 (HistoryPanel 경로·사유) |
| 332 | 342 | `SAGE_UI_RESULT_RESULT_LABEL` | `L"Result"` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_RESULT_LABEL` |  |
| 333 | 343 | `SAGE_UI_RESULT_TOTAL_LABEL` | `L"Total"` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_TOTAL_LABEL` |  |
| 334 | 344 | `SAGE_RESULT_STATUS_SUMMARY` | `L"summary"` | 업무 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_RESULT_STATUS_SUMMARY` |  |
| 335 | 345 | `SAGE_RESULT_STATUS_OUTPUT` | `L"output"` | 업무 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_RESULT_STATUS_OUTPUT` |  |
| 336 | 346 | `SAGE_RESULT_STATUS_SUCCESS` | `L"success"` | 업무 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_RESULT_STATUS_SUCCESS` |  |
| 337 | 347 | `SAGE_RESULT_STATUS_FAILED` | `L"failed"` | 업무 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_RESULT_STATUS_FAILED` |  |
| 338 | 348 | `SAGE_RESULT_STATUS_ERROR` | `L"error"` | 업무 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_RESULT_STATUS_ERROR` |  |
| 339 | 349 | `SAGE_UI_RESULT_PASSED_PREFIX` | `L"Passed "` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_PASSED_PREFIX` |  |
| 340 | 350 | `SAGE_UI_RESULT_FAILED_SUFFIX` | `L", Failed "` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_FAILED_SUFFIX` |  |
| 341 | 351 | `SAGE_UI_RESULT_FIELD` | `L"항목"` | UI 문자열 | 1 — SageWorkflowResultTable.cpp | 이관됨 (T03) `SAGE_UI_RESULT_FIELD` |  |
| 342 | 352 | `SAGE_UI_RESULT_VALUE` | `L"값"` | UI 문자열 | 2 — SageWorkflowResultTable.cpp, SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_RESULT_VALUE` |  |
| 343 | 353 | `SAGE_UI_RESULT_STATUS` | `L"상태"` | UI 문자열 | 2 — SageWorkflowResultPresenter.cpp, SageWorkflowResultTable.cpp | 이관됨 (T03) `SAGE_UI_RESULT_STATUS` |  |
| 344 | 354 | `SAGE_UI_RESULT_REASON` | `L"사유"` | UI 문자열 | 1 — SageWorkflowResultTable.cpp | 이관됨 (T03) `SAGE_UI_RESULT_REASON` |  |
| 345 | 355 | `SAGE_UI_RESULT_FILE` | `L"File"` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_FILE` |  |
| 346 | 356 | `SAGE_UI_RESULT_FOLDER` | `L"Folder"` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_FOLDER` |  |
| 347 | 357 | `SAGE_UI_RESULT_ERROR` | `L"Error"` | UI 문자열 | 1 — SageWorkflowResultPresenter.cpp | 이관됨 (T03) `SAGE_UI_RESULT_ERROR` |  |
| 348 | 358 | `SAGE_UI_SUMMARY_BADGE_FORMAT` | `L"%s %s%s"` | UI 문자열 | 1 — SageSummaryBar.cpp | SageDefine.h (T15) | "%s %s%s" printf 형식 → QString::arg |
| 349 | 359 | `SAGE_UI_SUMMARY_AMOUNT_FORMAT` | `L"%I64d"` | UI 문자열 | 1 — SageWorkflowResultTable.cpp | 옮기지 않음 — 기타: FormatAmountNumber(호출 0곳, T03 제외)에서만 사용 | "%I64d"는 MSVC 전용 형식 |
| 350 | 360 | `SAGE_UI_AMOUNT_GROUP_SEPARATOR` | `L","` | UI 문자열 | 1 — SageWorkflowResultTable.cpp | 옮기지 않음 — 기타: FormatAmountNumber(호출 0곳, T03 제외)에서만 사용 |  |
| 351 | 361 | `SAGE_UI_AMOUNT_NEGATIVE_MARK` | `L"-"` | UI 문자열 | 1 — SageWorkflowResultTable.cpp | 옮기지 않음 — 기타: FormatAmountNumber(호출 0곳, T03 제외)에서만 사용 |  |
| 352 | 362 | `SAGE_AMOUNT_GROUP_DIGITS` | `3` | 업무 | 1 — SageWorkflowResultTable.cpp | 옮기지 않음 — 기타: FormatAmountNumber(호출 0곳, T03 제외)에서만 사용 | SAGE_THOUSAND_SEPARATOR_STEP과 같은 값·뜻 3 |
| 353 | 363 | `SAGE_UI_RESULT_RESET_BTN` | `L"초기화"` | UI 문자열 | 1 — SageResultTablePanel.cpp | SageDefine.h (T15) | SAGE_UI_INPUT_RESET_BTN과 같은 값 "초기화" |
| 354 | 364 | `SAGE_UI_RESULT_FILTER_PLACEHOLDER` | `L"검색어 입력"` | UI 문자열 | 1 — SageResultTablePanel.cpp | SageDefine.h (T15) |  |
| 355 | 365 | `SAGE_UI_INPUT_RESET_BTN` | `L"초기화"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) | SAGE_UI_RESULT_RESET_BTN과 같은 값 "초기화" |
| 356 | 366 | `SAGE_JSON_KEY_FILES` | `L"files"` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 357 | 367 | `SAGE_JSON_KEY_STATUS` | `L"status"` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | 이관됨 (T03) `SAGE_JSON_KEY_STATUS` | SageSDI 사용처는 HistoryPanel → T16이 재사용 |
| 358 | 368 | `SAGE_JSON_VALUE_SUCCESS` | `L"success"` | 업무 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | SageQt SAGE_RESULT_STATUS_SUCCESS · SAGE_JSON_KEY_SUCCESS와 같은 값 "success"(뜻은 파일별 status 값). CompareNoCase로 비교 |
| 359 | 369 | `SAGE_JSON_KEY_FILE_PATH` | `L"filePath"` | 업무 | 2 — SageWorkflowHistoryPanel.cpp, SageWorkspacePanel.cpp | SageDefine.h (T14, T16) |  |
| 360 | 370 | `SAGE_JSON_KEY_OUTPUT_FOLDER` | `L"outputFolder"` | 업무 | 3 — SageSampleWorkflowHandler.cpp, SageWorkflowHistoryPanel.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SAGE_JSON_KEY_OUTPUT_FOLDER` | T14 · T16도 씀 (WorkspacePanel · HistoryPanel) |
| 361 | 371 | `SAGE_JSON_KEY_MESSAGE` | `L"message"` | 업무 | 2 — SageWorkflowHistoryPanel.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SAGE_JSON_KEY_MESSAGE` | T14 · T16도 씀 (WorkspacePanel · HistoryPanel) |
| 362 | 372 | `SAGE_JSON_KEY_CODE` | `L"code"` | 업무 | 2 — SageWorkflowHistoryPanel.cpp, SageWorkspacePanel.cpp | 이관됨 (T03) `SAGE_JSON_KEY_CODE` | T14 · T16도 씀 (WorkspacePanel · HistoryPanel) |
| 363 | 373 | `SAGE_JSON_KEY_ROW_NUMS` | `L"rowNums"` | 업무 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) |  |
| 364 | 374 | `SAGE_UI_TAB_INPUT` | `L"입력"` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_TAB_INPUT` |  |
| 365 | 375 | `SAGE_UI_TAB_RESULT` | `L"결과"` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_TAB_RESULT` |  |
| 366 | 376 | `SAGE_UI_TAB_HISTORY` | `L"실행 기록"` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_TAB_HISTORY` |  |
| 367 | 377 | `SAGE_UI_SAMPLE_NAME` | `L"샘플 업무"` | UI 문자열 | 2 — SageSampleWorkflowHandler.cpp, SageSidebarPanel.cpp | 이관됨 (T03) `SAGE_UI_SAMPLE_NAME` | SageSDI는 SidebarPanel에서도 씀 → T11이 재사용 |
| 368 | 378 | `SAGE_UI_SAMPLE_ACTION_BUTTON` | `L"실행"` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_SAMPLE_ACTION_BUTTON` |  |
| 369 | 379 | `SAGE_UI_SAMPLE_INPUT_DIALOG_TITLE` | `L"샘플 입력 파일 선택"` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_SAMPLE_INPUT_DIALOG_TITLE` |  |
| 370 | 380 | `SAGE_UI_SAMPLE_COMPLETED` | `L"샘플 업무가 완료되었습니다."` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_SAMPLE_COMPLETED` |  |
| 371 | 381 | `SAGE_UI_SAMPLE_STATUS_DONE` | `L"완료"` | UI 문자열 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_UI_SAMPLE_STATUS_DONE` |  |
| 372 | 382 | `SAGE_REQUEST_SAMPLE_RUN` | `L"mfc-sample-run"` | 업무 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_REQUEST_SAMPLE_RUN` | 값이 바뀜: SageQt는 "sample-run" (mfc- 접두 제거) |
| 373 | 383 | `SAGE_SAMPLE_PAYLOAD_FORMAT` | `L"{\"status\":\"%s\",\"fileName\":\"%s\",\"outputFolder\":\"%s\",\"totalFiles\":1,\"passedFiles\":1,\"failedFiles\":0}"` | 업무 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_JSON_KEY_FILE_NAME · SAGE_JSON_KEY_TOTAL_FILES · SAGE_JSON_KEY_PASSED_FILES · SAGE_JSON_KEY_FAILED_FILES + SAGE_SAMPLE_TOTAL_FILES/PASSED_FILES/FAILED_FILES` | 원문 여러 줄(383~) 이어 붙인 값 / 형식 문자열 대신 QJsonObject 키 + 숫자 상수로 분해. 숫자 1·1·0이 문자열에 하드코딩돼 있었음 |
| 374 | 386 | `SAGE_JSON_KEY_INPUT_PATH` | `L"inputPath"` | 업무 | 1 — SageSampleWorkflowHandler.cpp | 이관됨 (T03) `SAGE_JSON_KEY_INPUT_PATH` |  |
| 375 | 387 | `SAGE_PATH_SEPARATOR` | `L'\\'` | 업무 | 1 — SageSampleWorkflowHandler.cpp | 옮기지 않음 — 기타: L'\\'로 경로 자르기 — 크로스 플랫폼 위반, T03 결정으로 QFileInfo 대체 | 타입 wchar_t |
| 376 | 388 | `SAGE_UI_SECTION_INPUT` | `L"입력 파일"` | UI 문자열 | 2 — SageSampleWorkflowHandler.cpp, SageWorkflowInputPanel.cpp | 이관됨 (T03) `SAGE_UI_SECTION_INPUT` | SageSDI는 WorkflowInputPanel에서도 씀 → T13이 재사용 |
| 377 | 389 | `SAGE_UI_SECTION_OUTPUT` | `L"저장 위치"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) |  |
| 378 | 390 | `SAGE_UI_INPUT_CARD_TITLE` | `L"입력 · 저장 위치"` | UI 문자열 | 1 — SageWorkflowInputPanel.cpp | SageDefine.h (T13) |  |
| 379 | 391 | `SAGE_UI_SECTION_RESULT` | `L"처리 결과"` | UI 문자열 | 2 — SageResultTablePanel.cpp, SageWorkflowResultPanel.cpp | SageDefine.h (T15) |  |
| 380 | 392 | `SAGE_UI_HISTORY_SUCCESS` | `L"성공"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 381 | 393 | `SAGE_UI_HISTORY_FAILED` | `L"실패"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | SAGE_UI_FAILED와 같은 값 "실패" |
| 382 | 394 | `SAGE_UI_HISTORY_TIME_FORMAT` | `L"%m-%d %H:%M:%S"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | strftime 형식 → QDateTime::toString("MM-dd HH:mm:ss") |
| 383 | 395 | `SAGE_UI_HISTORY_COL_TIME` | `L"실행 시각"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 384 | 396 | `SAGE_UI_HISTORY_COL_RESULT` | `L"결과"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | SAGE_UI_TAB_RESULT와 같은 값 "결과" |
| 385 | 397 | `SAGE_UI_HISTORY_COL_INPUT` | `L"입력 파일"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | SAGE_UI_SECTION_INPUT과 같은 값 "입력 파일" |
| 386 | 398 | `SAGE_UI_HISTORY_COL_OUTPUT` | `L"저장 경로"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 387 | 399 | `SAGE_UI_HISTORY_COL_REASON` | `L"사유"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | SAGE_UI_RESULT_REASON과 같은 값 "사유" |
| 388 | 400 | `SAGE_UI_HISTORY_NO_OUTPUT` | `L"미리보기 (저장 없음)"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 389 | 401 | `SAGE_UI_HISTORY_FILTER_ALL` | `L"전체 %d"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | %d printf 형식 → QString::arg |
| 390 | 402 | `SAGE_UI_HISTORY_FILTER_SUCCESS` | `L"성공 %d"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | %d printf 형식 → QString::arg |
| 391 | 403 | `SAGE_UI_HISTORY_FILTER_FAILED` | `L"실패 %d"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) | %d printf 형식 → QString::arg |
| 392 | 404 | `SAGE_UI_HISTORY_EMPTY_TITLE` | `L"아직 실행 기록이 없습니다"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 393 | 405 | `SAGE_UI_HISTORY_EMPTY_DESC` | `L"문서를 생성하면 여기에 쌓입니다"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 394 | 406 | `SAGE_UI_HISTORY_FILTER_EMPTY_TITLE` | `L"조건에 맞는 기록이 없습니다"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 395 | 407 | `SAGE_UI_HISTORY_FILTER_EMPTY_DESC` | `L"다른 항목을 선택해 보세요"` | UI 문자열 | 1 — SageWorkflowHistoryPanel.cpp | SageDefine.h (T16) |  |
| 396 | 408 | `SAGE_REQUEST_UNKNOWN` | `L"mfc-unknown"` | 업무 | 1 — SageWorkflowController.cpp | SageDefine.h (T14) | "mfc-" 접두. SageQt는 sample 요청 ID에서 mfc- 를 뺐음 (T03) → 값 재검토 |
| 397 | 410 | `ID_SAGE_LOGIN_BTN` | `41021` | 컨트롤 ID | 1 — SageHeaderPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 398 | 411 | `ID_SAGE_LOGOUT_BTN` | `41022` | 컨트롤 ID | 1 — SageHeaderPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 399 | 412 | `ID_SAGE_USER_LABEL` | `41023` | 컨트롤 ID | 1 — SageHeaderPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 400 | 413 | `ID_SAGE_LOGIN_ID_EDIT` | `41100` | 컨트롤 ID | 1 — SageLoginDlg.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 401 | 414 | `ID_SAGE_LOGIN_PW_EDIT` | `41101` | 컨트롤 ID | 1 — SageLoginDlg.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 402 | 415 | `ID_SAGE_PW_CURRENT_EDIT` | `41102` | 컨트롤 ID | 1 — SagePasswordChangeDlg.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 403 | 416 | `ID_SAGE_PW_NEW_EDIT` | `41103` | 컨트롤 ID | 1 — SagePasswordChangeDlg.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 404 | 417 | `ID_SAGE_PW_CONFIRM_EDIT` | `41104` | 컨트롤 ID | 1 — SagePasswordChangeDlg.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 405 | 419 | `SAGE_LOGIN_BTN_WIDTH` | `68` | 여백·크기 | 1 — SageHeaderPanel.cpp | SageDesignDefine.h |  |
| 406 | 420 | `SAGE_USER_LABEL_WIDTH` | `150` | 여백·크기 | 1 — SageHeaderPanel.cpp | SageDesignDefine.h |  |
| 407 | 421 | `SAGE_LOGIN_DLG_WIDTH` | `320` | 여백·크기 | 1 — SageLoginDlg.cpp | SageDesignDefine.h |  |
| 408 | 422 | `SAGE_PASSWORD_DLG_WIDTH` | `360` | 여백·크기 | 1 — SagePasswordChangeDlg.cpp | SageDesignDefine.h |  |
| 409 | 423 | `SAGE_LOGIN_DLG_LABEL_WIDTH` | `64` | 여백·크기 | 1 — SageLoginDlg.cpp | SageDesignDefine.h | SAGE_FORM_LABEL_WIDTH와 같은 값 64 |
| 410 | 424 | `SAGE_PASSWORD_DLG_LABEL_WIDTH` | `96` | 여백·크기 | 1 — SagePasswordChangeDlg.cpp | SageDesignDefine.h |  |
| 411 | 425 | `SAGE_LOGIN_DLG_BTN_WIDTH` | `96` | 여백·크기 | 3 — SageLoginDlg.cpp, SageMessageBoxDlg.cpp, SagePasswordChangeDlg.cpp | SageDesignDefine.h | MessageBoxDlg도 씀 (이름은 LOGIN_DLG) |
| 412 | 426 | `SAGE_LOGIN_DLG_TEMPLATE_CX` | `180` | 여백·크기 | 2 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp | 옮기지 않음 — 기타: DLGTEMPLATE 크기(대화상자 단위) — T09가 제외 (MFC 전용) |  |
| 413 | 427 | `SAGE_LOGIN_DLG_TEMPLATE_CY` | `90` | 여백·크기 | 2 — SageLoginDlg.cpp, SagePasswordChangeDlg.cpp | 옮기지 않음 — 기타: DLGTEMPLATE 크기(대화상자 단위) — T09가 제외 (MFC 전용) |  |
| 414 | 428 | `SAGE_LOGIN_DLG_FONT_PT` | `10` | 폰트 | 1 — SageFramelessDialog.cpp | 옮기지 않음 — 기타: DLGTEMPLATE 글꼴 크기(WORD) — T09가 제외 (MFC 전용). 실제 글꼴은 SageUiResources 역할 | 타입 WORD |
| 415 | 430 | `SAGE_USER_LOGIN_ID_MIN_LEN` | `2` | 업무 | 1 — SageUserService.cpp | 옮기지 않음 — 기타: ValidateLoginId ← AddUser(호출 0곳)에서만 사용 — T05 결정으로 제외 |  |
| 416 | 431 | `SAGE_USER_LOGIN_ID_MAX_LEN` | `30` | 업무 | 1 — SageUserService.cpp | 옮기지 않음 — 기타: ValidateLoginId ← AddUser(호출 0곳)에서만 사용 — T05 결정으로 제외 |  |
| 417 | 432 | `SAGE_USER_PW_MIN_LEN` | `4` | 업무 | 1 — SageUserService.cpp | 이관됨 (T05) `SAGE_USER_PW_MIN_LEN` |  |
| 418 | 433 | `SAGE_USER_PW_MAX_LEN` | `15` | 업무 | 2 — SageUserService.cpp, SagePasswordChangeDlg.cpp | 이관됨 (T05) `SAGE_USER_PW_MAX_LEN` | SageSDI는 PasswordChangeDlg에서도 씀(입력 길이 제한) → T10이 재사용 |
| 419 | 435 | `SAGE_UI_LOGIN_BTN` | `L"로그인"` | UI 문자열 | 1 — SageHeaderPanel.cpp | SageDefine.h (T12) | SAGE_UI_LOGIN_DLG_TITLE · SAGE_UI_LOGIN_OK와 같은 값 "로그인" |
| 420 | 436 | `SAGE_UI_LOGOUT_BTN` | `L"로그아웃"` | UI 문자열 | 1 — SageHeaderPanel.cpp | SageDefine.h (T12) |  |
| 421 | 437 | `SAGE_UI_LOGIN_DLG_TITLE` | `L"로그인"` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | SAGE_UI_LOGIN_BTN · SAGE_UI_LOGIN_OK와 같은 값 "로그인" |
| 422 | 438 | `SAGE_UI_LOGIN_ID_LABEL` | `L"아이디"` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) |  |
| 423 | 439 | `SAGE_UI_LOGIN_PW_LABEL` | `L"비밀번호"` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) |  |
| 424 | 440 | `SAGE_UI_LOGIN_OK` | `L"로그인"` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | SAGE_UI_LOGIN_BTN · SAGE_UI_LOGIN_DLG_TITLE와 같은 값 "로그인" |
| 425 | 441 | `SAGE_UI_LOGIN_CANCEL` | `L"취소"` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | SAGE_UI_CHANGE_PW_CANCEL과 같은 값 "취소" |
| 426 | 442 | `SAGE_UI_ROLE_ADMIN` | `L"관리자"` | UI 문자열 | 1 — SageHeaderPanel.cpp | SageDefine.h (T12) |  |
| 427 | 443 | `SAGE_UI_ROLE_USER` | `L"사용자"` | UI 문자열 | 1 — SageHeaderPanel.cpp | SageDefine.h (T12) |  |
| 428 | 444 | `SAGE_UI_LOGIN_FAILED` | `L"아이디 또는 비밀번호가 올바르지 않습니다."` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | T10 지시서가 명시 |
| 429 | 445 | `SAGE_UI_LOGIN_EMPTY_ID` | `L"아이디를 입력하세요."` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | T10 지시서가 명시 |
| 430 | 446 | `SAGE_UI_LOGIN_EMPTY_PW` | `L"비밀번호를 입력하세요."` | UI 문자열 | 1 — SageLoginDlg.cpp | 이관됨 (T05) `SAGE_UI_PW_EMPTY` | 같은 값 "비밀번호를 입력하세요.". SageSDI 사용처는 LoginDlg(T10) — 재사용 여부 T10 확인 |
| 431 | 447 | `SAGE_UI_LOGIN_REQUIRED` | `L"로그인 상태에서만 사용가능합니다."` | UI 문자열 | 1 — SageSidebarPanel.cpp | SageDefine.h (T11) | T11 지시서가 명시 · "사용가능" 띄어쓰기 원문 그대로 |
| 432 | 448 | `SAGE_UI_CHANGE_PW_TITLE` | `L"비밀번호 변경"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) | SAGE_UI_CHANGE_PW_MENU와 같은 값 "비밀번호 변경" |
| 433 | 449 | `SAGE_UI_CHANGE_PW_CURRENT` | `L"현재 비밀번호"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 434 | 450 | `SAGE_UI_CHANGE_PW_NEW` | `L"새 비밀번호"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 435 | 451 | `SAGE_UI_CHANGE_PW_CONFIRM` | `L"새 비밀번호 확인"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 436 | 452 | `SAGE_UI_CHANGE_PW_HINT` | `L"영문 · 숫자 4~15자"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) | 문자열 안 "4~15자" 하드코딩 (SAGE_USER_PW_MIN_LEN/MAX_LEN과 연동 안 됨) |
| 437 | 453 | `SAGE_UI_CHANGE_PW_OK` | `L"변경"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 438 | 454 | `SAGE_UI_CHANGE_PW_CANCEL` | `L"취소"` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) | SAGE_UI_LOGIN_CANCEL과 같은 값 "취소" |
| 439 | 455 | `SAGE_UI_CHANGE_PW_EMPTY_CURRENT` | `L"현재 비밀번호를 입력하세요."` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 440 | 456 | `SAGE_UI_CHANGE_PW_EMPTY_NEW` | `L"변경할 비밀번호를 입력하세요."` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 441 | 457 | `SAGE_UI_CHANGE_PW_EMPTY_CONFIRM` | `L"변경할 비밀번호 확인을 입력하세요."` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 442 | 458 | `SAGE_UI_CHANGE_PW_TOO_SHORT` | `L"비밀번호는 4자 이상이어야 합니다."` | UI 문자열 | 1 — SageUserService.cpp | 이관됨 (T05) `SAGE_UI_CHANGE_PW_TOO_SHORT` | 문자열 안 "4자" 하드코딩 (SAGE_USER_PW_MIN_LEN과 연동 안 됨) |
| 443 | 459 | `SAGE_UI_CHANGE_PW_TOO_LONG` | `L"비밀번호는 15자 이하이어야 합니다."` | UI 문자열 | 1 — SageUserService.cpp | 이관됨 (T05) `SAGE_UI_CHANGE_PW_TOO_LONG` | 문자열 안 "15자" 하드코딩 (SAGE_USER_PW_MAX_LEN과 연동 안 됨) |
| 444 | 460 | `SAGE_UI_CHANGE_PW_INVALID_CHAR` | `L"비밀번호는 영문과 숫자만 사용할 수 있습니다."` | UI 문자열 | 1 — SageUserService.cpp | 이관됨 (T05) `SAGE_UI_CHANGE_PW_INVALID_CHAR` |  |
| 445 | 461 | `SAGE_UI_CHANGE_PW_MISMATCH` | `L"변경할 비밀번호가 서로 다릅니다."` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 446 | 462 | `SAGE_UI_CHANGE_PW_CURRENT_INVALID` | `L"현재 비밀번호가 올바르지 않습니다."` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 447 | 463 | `SAGE_UI_CHANGE_PW_COMPLETED` | `L"비밀번호가 변경되었습니다."` | UI 문자열 | 1 — SagePasswordChangeDlg.cpp | SageDefine.h (T10) |  |
| 448 | 465 | `SAGE_UI_INITIAL_ADMIN_PW_FORMAT` | `L"관리자 계정을 새로 만들었습니다.\n\n아이디: %s\n초기 비밀번호: %s\n\n이 비밀번호는 지금 한 번만 표시됩니다. 적어 두고 첫 로그인에서 변경하세요."` | UI 문자열 | 1 — SageSDI.cpp | SageDefine.h (T09) | 원문 여러 줄(465~) 이어 붙인 값 / 두 줄 문자열 이어 붙임. \n 줄바꿈, %s 두 개 → QString::arg. T09 지시서가 명시 |
| 449 | 468 | `SAGE_UI_MUST_CHANGE_PW_REQUIRED` | `L"초기 비밀번호를 변경해야 로그인할 수 있습니다."` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | T10 지시서가 명시 |
| 450 | 469 | `SAGE_UI_INITIAL_PW_GENERATE_FAILED` | `L"초기 비밀번호를 생성할 수 없습니다."` | UI 문자열 | 1 — SqlInitializer.cpp | 옮기지 않음 — 기타: BCryptGenRandom 실패 메시지 — SageQt(T05)는 QRandomGenerator::system()이라 실패 경로 없음 |  |
| 451 | 470 | `SAGE_UI_MUST_CHANGE_PW_CANCELED` | `L"비밀번호를 변경하지 않아 로그인을 취소했습니다."` | UI 문자열 | 1 — SageLoginDlg.cpp | SageDefine.h (T10) | T10 지시서가 명시 |
| 452 | 472 | `SAGE_DEFAULT_ADMIN_ID` | `L"admin"` | 업무 | 2 — SageSDI.cpp, SqlInitializer.cpp | 이관됨 (T05) `SAGE_DEFAULT_ADMIN_ID` | SageSDI는 SageSDI.cpp(안내 표시)에서도 씀 → T09가 재사용 |
| 453 | 473 | `SAGE_INITIAL_PW_LENGTH` | `14` | 업무 | 1 — SqlInitializer.cpp | 이관됨 (T05) `SAGE_INITIAL_PW_LENGTH` |  |
| 454 | 474 | `SAGE_INITIAL_PW_ALPHABET` | `L"ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789"` | 업무 | 1 — SqlInitializer.cpp | 이관됨 (T05) `SAGE_INITIAL_PW_ALPHABET` | I·O·l·0·1 제외 57자 |
| 455 | 476 | `SAGE_MUST_CHANGE_PW_NONE` | `0` | 업무 | 2 — SageUserDto.h, SagePasswordChangeDlg.cpp | 이관됨 (T05) `SageUserDto::m_isPasswordChangeRequired (bool)` | int 0/1 → bool false |
| 456 | 477 | `SAGE_MUST_CHANGE_PW_REQUIRED` | `1` | 업무 | 2 — SqlInitializer.cpp, SageLoginDlg.cpp | 이관됨 (T05) `SageUserDto::m_isPasswordChangeRequired (bool)` | int 0/1 → bool true. SageSDI는 LoginDlg에서도 확인 → T10이 재사용 |
| 457 | 480 | `ID_EMPTY_STATE_ACTION` | `41029` | 컨트롤 ID | 1 — SageEmptyState.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 458 | 481 | `ID_SAGE_RESULT_TABLE_PANEL` | `41071` | 컨트롤 ID | 1 — SageWorkflowResultPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 459 | 482 | `ID_SAGE_INPUT_TABLE_PANEL` | `41072` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 460 | 483 | `ID_SAGE_RESULT_SUMMARY_BAR` | `41073` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 461 | 484 | `ID_SAGE_RESULT_TOTAL_BAR` | `41074` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 462 | 485 | `ID_SAGE_RESULT_SELECTION_BAR` | `41075` | 컨트롤 ID | 1 — SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 463 | 486 | `ID_SAGE_SELECTION_CHECK` | `41076` | 컨트롤 ID | 1 — SageSelectionBar.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 464 | 487 | `ID_SAGE_SELECTION_CLEAR` | `41077` | 컨트롤 ID | 2 — SageSelectionBar.cpp, SageResultTablePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 465 | 488 | `ID_SAGE_WORKFLOW_INPUT_PANEL` | `41085` | 컨트롤 ID | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 466 | 489 | `ID_SAGE_WORKFLOW_RESULT_PANEL` | `41086` | 컨트롤 ID | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 467 | 490 | `ID_SAGE_WORKFLOW_HISTORY_PANEL` | `41087` | 컨트롤 ID | 1 — SageWorkspacePanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 468 | 491 | `ID_SAGE_WORKSPACE_PANEL` | `41088` | 컨트롤 ID | 1 — SageSDIView.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 469 | 492 | `ID_STATUS_CARD_OPEN_FOLDER` | `41090` | 컨트롤 ID | 1 — SageStatusCard.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 470 | 493 | `ID_SAGE_OPEN_OUTPUT_FOLDER` | `41092` | 컨트롤 ID | 1 — SageWorkflowInputPanel.cpp | 옮기지 않음 — 컨트롤 ID |  |
| 471 | 496 | `SAGE_UI_SIDEBAR_GROUP_SAMPLE` | `L"샘플"` | UI 문자열 | 1 — SageSidebarPanel.cpp | 이관됨 (T03) `SAGE_UI_SAMPLE_CATEGORY` | 같은 값 "샘플"·같은 뜻(사이드바 그룹). SageQt는 핸들러 category로 제공 |
| 472 | 497 | `SAGE_UI_SIDEBAR_GROUP_ETC` | `L"기타"` | UI 문자열 | 1 — SageSidebarPanel.cpp | SageDefine.h (T11) |  |
| 473 | 498 | `SAGE_UI_CHANGE_PW_MENU` | `L"비밀번호 변경"` | UI 문자열 | 1 — SageSidebarPanel.cpp | SageDefine.h (T11) | SAGE_UI_CHANGE_PW_TITLE과 같은 값 "비밀번호 변경" |
| 474 | 502 | `SAGE_UI_NUMBER_FORMAT` | `L"%I64d"` | UI 문자열 | 1 — SageNumberFormat.cpp | 옮기지 않음 — 기타: SageNumberFormat(FormatPrice 등) 함수 4개 모두 호출 0곳 | "%I64d"는 MSVC 전용 형식 |
| 475 | 503 | `SAGE_THOUSAND_SEPARATOR` | `L','` | UI 문자열 | 1 — SageNumberFormat.cpp | 옮기지 않음 — 기타: SageNumberFormat(FormatPrice 등) 함수 4개 모두 호출 0곳 | wchar_t. SAGE_UI_AMOUNT_GROUP_SEPARATOR와 같은 뜻 / 타입 wchar_t |
| 476 | 504 | `SAGE_THOUSAND_SEPARATOR_STEP` | `3` | 업무 | 1 — SageNumberFormat.cpp | 옮기지 않음 — 기타: SageNumberFormat(FormatPrice 등) 함수 4개 모두 호출 0곳 | SAGE_AMOUNT_GROUP_DIGITS와 같은 값·뜻 3 |
| 477 | 508 | `SAGE_CO_COMPANY_EDIT_MIN_WIDTH` | `80` | 여백·크기 | 1 — SageWorkflowInputPanel.cpp | SageDesignDefine.h | 이름의 CO_COMPANY는 "법인 순서 데이터 관리" 잔재(주석 줄 506). 실제 용도는 입력 폼 편집칸 최소 폭 |

## 2. 요약

### 목적지별

| 목적지 | 개수 |
|---|---|
| SageDesignDefine.h | 212 |
| SageDefine.h (T09) | 9 |
| SageDefine.h (T10) | 22 |
| SageDefine.h (T11) | 6 |
| SageDefine.h (T12) | 5 |
| SageDefine.h (T13) | 11 |
| SageDefine.h (T14) | 21 |
| SageDefine.h (T14, T16) | 1 |
| SageDefine.h (T15) | 13 |
| SageDefine.h (T15, T14) | 2 |
| SageDefine.h (T15, T16) | 2 |
| SageDefine.h (T16) | 25 |
| 이관됨 (T03) | 41 |
| 이관됨 (T05) | 11 |
| 옮기지 않음 — 기타 | 34 |
| 옮기지 않음 — 창 메시지 | 12 |
| 옮기지 않음 — 컨트롤 ID | 48 |
| 옮기지 않음 — 타이머 ID | 1 |
| 옮기지 않음 — 호출 0곳 | 1 |
| **합계** | **477** |

### 목적지 묶음

| 묶음 | 개수 |
|---|---|
| SageDesignDefine.h | 212 |
| SageDefine.h (Txx) | 117 |
| 이관됨 | 52 |
| 옮기지 않음 — 컨트롤 ID | 48 |
| 옮기지 않음 — 창 메시지 | 12 |
| 옮기지 않음 — 타이머 ID | 1 |
| 옮기지 않음 — 호출 0곳 | 1 |
| 옮기지 않음 — 기타 | 34 |
| **합계** | **477** |

### 종류별

| 종류 | 개수 |
|---|---|
| 색 | 42 |
| 여백·크기 | 169 |
| 폰트 | 11 |
| UI 문자열 | 126 |
| 업무 | 66 |
| 컨트롤 ID | 48 |
| 창 메시지 | 12 |
| 타이머 ID | 1 |
| 기타 | 2 |
| **합계** | **477** |

행 수 = 477

## 3. 목적지가 확실하지 않은 상수

규칙대로 목적지를 적었지만 판단이 들어간 것들이다. 해당 주제를 시작할 때 다시 확인한다.

| 이름 | 적은 목적지 | 확실하지 않은 이유 |
|---|---|---|
| `SAGE_STATUS_CARD_ACTION_GAP` | SageDesignDefine.h | 외부 파일 사용은 0곳이라 규칙상 "호출 0곳"이다. 하지만 `SageDefine.h` 안에서 `SAGE_STATUS_CARD_ACTION_AREA_WIDTH`(사용 1곳)를 계산하는 데 쓰여서 함께 옮기는 것으로 적었다 |
| `SAGE_TITLE_FONT_FACE` · `SAGE_CONTROL_FONT_FACE` · `SAGE_LOGO_FONT_FACE` | SageDesignDefine.h | 요청 규칙에는 폰트 **크기**만 SageDesignDefine.h로 가게 되어 있고, 폰트 이름은 따로 정해져 있지 않다. 또 GDI식 이름("Pretendard SemiBold")은 Qt에서 패밀리 + 굵기로 바뀐다 (T02). font-probe(`SAGE_FONT_PROBE_LEGACY_FACES` · `FAMILY_*`)에 같은 값이 있지만 측정 도구 전용이라 "이관됨"으로 보지 않았다 |
| 폰트 크기 7개 (`SAGE_*_FONT_POINT_SIZE`) | SageDesignDefine.h | font-probe `SAGE_FONT_PROBE_ROLES`에 같은 값이 있지만 측정 도구 전용이라 "이관됨"으로 보지 않았다 |
| `SAGE_UI_LOGIN_EMPTY_PW` | 이관됨 (T05) `SAGE_UI_PW_EMPTY` | 값("비밀번호를 입력하세요.")과 뜻(빈 비밀번호)은 같다. 다만 SageSDI 사용처는 로그인 창(T10)이고, T10 지시서는 옛 이름으로 적혀 있다. T10이 `SAGE_UI_PW_EMPTY`를 그대로 쓸지 확인해야 한다 |
| `SAGE_UI_SIDEBAR_GROUP_SAMPLE` | 이관됨 (T03) `SAGE_UI_SAMPLE_CATEGORY` | 값 "샘플"과 쓰임(사이드바 그룹 이름)이 같다. 다만 SageQt에서는 핸들러가 category로 주는 값이고, SageSDI는 사이드바가 직접 가지고 있었다. T11 지시서는 옛 이름으로 적혀 있다 |
| `SAGE_MUST_CHANGE_PW_NONE` · `_REQUIRED` | 이관됨 (T05) `m_isPasswordChangeRequired` | 이름이 같은 상수는 없다. `SageUserDto`의 bool 필드로 바뀐 것을 같은 개념으로 보았다 |
| `SAGE_SAMPLE_PAYLOAD_FORMAT` | 이관됨 (T03) | 형식 문자열은 없어지고, JSON 키 상수와 `SAGE_SAMPLE_*_FILES` 숫자 상수로 나뉘어 옮겨졌다 |
| `SAGE_UI_APP_TITLE` ("SageSDI") | SageDefine.h (T11) | SageQt에 `SAGE_APPLICATION_NAME` · `SAGE_UI_MAIN_WINDOW_TITLE` = "SageQt"가 있다. 사이드바 로고 문구로 이것을 다시 쓸지, 따로 상수를 둘지 T11에서 정한다 |
| `SAGE_UI_RECEIVABLES_NAME` · `SAGE_UI_RECEIVABLES_GENERATE_BUTTON` | SageDefine.h (T12) · (T13) | "미수금 내역서"는 SageTaechang에서 남은 것으로 보인다. 둘 다 컨트롤을 만들 때 넣는 초기값이고 곧 `SetWindowTextW`로 바뀐다. Qt에서는 빈 값으로 시작하면 필요 없을 수도 있다 |
| `SAGE_UI_DROP_PATH_SEPARATOR` ("\r\n") | SageDefine.h (T13) | 드롭된 경로들을 한 문자열로 이었다가 다시 자르는 MFC 방식이다. Qt는 `QMimeData::urls()`로 목록을 받으므로 필요 없을 수 있다 |
| `SAGE_UI_EXCEL_DEFAULT_EXT` | 옮기지 않음 — 기타 | 열기 대화상자의 기본 확장자 인자다. Qt 열기 대화상자에서는 쓸 곳이 없다고 **판단**했다 (측정한 것 아님) |
| `SAGE_UI_EXCEL_FILTER` | 옮기지 않음 — 기타 | 요청에 적힌 예시(MFC 필터 형식)를 따랐다. 필터 문구 자체는 T13에서 Qt 형식으로 다시 필요하다 |
| `SAGE_UI_INITIAL_PW_GENERATE_FAILED` | 옮기지 않음 — 기타 | `BCryptGenRandom`이 실패할 때만 쓰인다. SageQt T05의 `generateInitialPassword`에는 실패하는 경로가 없어서 옮기지 않은 것으로 보았다. 옮기지 않는다는 결정 기록은 T05 문서에 없다 |
| `SAGE_LIST_CHECK_IMAGE_WIDTH` (20) | 옮기지 않음 — 기타 | CImageList의 이미지 폭이다. 다만 체크 열의 실제 폭 역할도 하므로, T15 delegate가 같은 폭을 쓰려면 디자인 값으로 다시 필요할 수 있다 |
| `SAGE_LIST_BOX_ROW_HEIGHT` · `SAGE_LIST_BOX_TEXT_PAD_X` · `SAGE_COMBO_FIT_MAX_PASS` | 옮기지 않음 — 기타 | 사용 수는 1이지만, 그것을 쓰는 클래스 `CSageListBox` · `CSageComboBox`를 include하거나 쓰는 곳이 0곳이다 (grep으로 확인). 규칙에 있는 "호출 0곳"(사용 수 0)과는 다른 경우다 |
| `SAGE_PROCESS_TIMEOUT_MS` | 옮기지 않음 — 기타 | 사용 수는 1이지만, 쓰는 곳이 `RunProcessAndWait` 한 곳이고 이 함수는 호출 0곳이다 (MIGRATION_PLAN 죽은 코드 목록) |
| `SAGE_UI_SUMMARY_AMOUNT_FORMAT` · `SAGE_UI_AMOUNT_GROUP_SEPARATOR` · `SAGE_UI_AMOUNT_NEGATIVE_MARK` · `SAGE_AMOUNT_GROUP_DIGITS` | 옮기지 않음 — 기타 | 쓰는 곳이 `FormatAmountNumber` 한 곳이고 이 함수는 호출 0곳이다 (T03 결정) |
| `SAGE_UI_NUMBER_FORMAT` · `SAGE_THOUSAND_SEPARATOR` · `SAGE_THOUSAND_SEPARATOR_STEP` | 옮기지 않음 — 기타 | `SageNumberFormat`의 함수 4개(`FormatPrice` · `RemovePriceSeparators` · `PriceTextToInt` · `FormatPriceEditText`)를 grep한 결과 외부 호출이 0곳이다. 이 결정은 MIGRATION_PLAN에 적혀 있지 않다 |
| `SAGE_RESULT_FIELD_WIDTH` · `_STATUS_WIDTH` · `_REASON_WIDTH` · `_MIN_VALUE_WIDTH`, `SAGE_HISTORY_*_WIDTH` 5개 | SageDesignDefine.h | 픽셀 열 폭 분배(`DistributeColumnWidths`)에 쓰이는 값이다. T03에서 이 함수를 옮기지 않고 `QHeaderView`로 바꾸기로 했으므로, T15 · T16에서 픽셀 폭이 여전히 필요한지 확인해야 한다 |
| `SAGE_BUTTON_VERT_ADJUST` · `SAGE_BUTTON_TEXT_TOP_OFFSET` · `SAGE_EDIT_TEXT_TOP_PAD` · `SAGE_EDIT_TEXT_LEFT_PAD` | SageDesignDefine.h | GDI · `EM_SETRECT`에 맞춘 보정값이다. Qt에서도 필요한지는 화면에서 재 봐야 안다 |
| `SAGE_RESULT_CRITERIA_DROP_ROWS` | SageDefine.h (T15), 종류 업무 | 드롭다운에 보이는 행 **개수**라서 픽셀 값이 아니다. 그래서 디자인 값이 아니라 업무 값으로 분류했다 |
| `SAGE_JSON_VALUE_SUCCESS` ("success") | SageDefine.h (T16) | SageQt의 `SAGE_RESULT_STATUS_SUCCESS` · `SAGE_JSON_KEY_SUCCESS`와 값은 같다. 하지만 뜻(파일별 status 값)이 달라서 "이관됨"으로 보지 않았다 |
| `SAGE_SIDEBAR_ACTION_NONE` · `_CHANGE_PASSWORD` | SageDefine.h (T11) | Win32 트리 항목 데이터(`DWORD_PTR`)다. Qt에서는 enum class와 item data role로 바꿀 후보다 |
| `SAGE_UI_ROW_NUM_FORMAT` · `SAGE_UI_ROW_NUM_SEPARATOR` | SageDefine.h (T15, T14) | 이름은 `SAGE_UI_`로 시작하지만 화면 문자열이 아니다. 체크한 행 번호를 `rowNums` payload로 만드는 데 쓰여서 종류를 업무로 적었다 |
| `SAGE_UI_EXPLORER_*` 3개 | 옮기지 않음 — 기타 | 이름은 `SAGE_UI_`지만 화면 문자열이 아니고 Windows 전용이다 (`ShellExecuteW` · `explorer.exe /select`). "폴더 열기"를 OS마다 어떻게 할지는 T14에서 정한다 |

### 확인 못함
- 없음. 모든 행에서 값 · 사용 수 · 목적지를 정했다

### 셈 방식 참고
- 사용 수는 **파일 수**다. `.h`와 `.cpp`에 둘 다 나오면 2로 센다 (예: `SAGE_PRIVATE_FONT_COUNT` = `SageSDI.cpp` + `SageSDI.h`)
- 주석이나 문자열 안에 이름이 나와도 센다 (단어 단위 정규식)
