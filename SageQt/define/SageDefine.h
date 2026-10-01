#pragma once

#include <QString>

enum class SageWorkflowType
{
    Sample = 1
};

enum class SageTaskType
{
    Load = 1,
    Generate = 2
};

enum class SageWorkflowTabKind
{
    Input = 0,
    DocumentResult = 1,
    DocumentHistory = 2
};

enum class SageUserRole
{
    User = 0,
    Admin = 1
};

inline const QString SAGE_ORGANIZATION_NAME = QStringLiteral("Sage");
inline const QString SAGE_APPLICATION_NAME = QStringLiteral("SageQt");
inline constexpr char SAGE_LOG_CATEGORY_APP[] = "sage.app";
inline constexpr char SAGE_LOG_CATEGORY_UI[] = "sage.ui";

inline const QString SAGE_UI_MAIN_WINDOW_TITLE = QStringLiteral("SageQt");
inline const QString SAGE_UI_APP_TITLE = QStringLiteral("SageQt");
inline const QString SAGE_UI_SIDEBAR_GROUP_ETC = QStringLiteral("기타");
inline const QString SAGE_UI_CHANGE_PW_MENU = QStringLiteral("비밀번호 변경");
inline const QString SAGE_UI_LOGIN_BTN = QStringLiteral("로그인");
inline const QString SAGE_UI_LOGOUT_BTN = QStringLiteral("로그아웃");
inline const QString SAGE_UI_ROLE_ADMIN = QStringLiteral("관리자");
inline const QString SAGE_UI_ROLE_USER = QStringLiteral("사용자");
inline const QString SAGE_UI_LOGIN_DLG_TITLE = QStringLiteral("로그인");
inline const QString SAGE_UI_LOGIN_ID_LABEL = QStringLiteral("아이디");
inline const QString SAGE_UI_LOGIN_PW_LABEL = QStringLiteral("비밀번호");
inline const QString SAGE_UI_LOGIN_OK = QStringLiteral("로그인");
inline const QString SAGE_UI_LOGIN_CANCEL = QStringLiteral("취소");
inline const QString SAGE_UI_LOGIN_FAILED = QStringLiteral("아이디 또는 비밀번호가 올바르지 않습니다.");
inline const QString SAGE_UI_LOGIN_EMPTY_ID = QStringLiteral("아이디를 입력하세요.");
inline const QString SAGE_UI_LOGIN_EMPTY_PW = QStringLiteral("비밀번호를 입력하세요.");
inline const QString SAGE_UI_MUST_CHANGE_PW_REQUIRED = QStringLiteral("초기 비밀번호를 변경해야 로그인할 수 있습니다.");
inline const QString SAGE_UI_MUST_CHANGE_PW_CANCELED =
    QStringLiteral("비밀번호를 변경하지 않아 로그인을 취소했습니다.");
inline const QString SAGE_UI_CHANGE_PW_TITLE = QStringLiteral("비밀번호 변경");
inline const QString SAGE_UI_CHANGE_PW_CURRENT = QStringLiteral("현재 비밀번호");
inline const QString SAGE_UI_CHANGE_PW_NEW = QStringLiteral("새 비밀번호");
inline const QString SAGE_UI_CHANGE_PW_CONFIRM = QStringLiteral("새 비밀번호 확인");
inline const QString SAGE_UI_CHANGE_PW_HINT = QStringLiteral("영문 · 숫자 4~15자");
inline const QString SAGE_UI_CHANGE_PW_OK = QStringLiteral("변경");
inline const QString SAGE_UI_CHANGE_PW_CANCEL = QStringLiteral("취소");
inline const QString SAGE_UI_CHANGE_PW_EMPTY_CURRENT = QStringLiteral("현재 비밀번호를 입력하세요.");
inline const QString SAGE_UI_CHANGE_PW_EMPTY_NEW = QStringLiteral("변경할 비밀번호를 입력하세요.");
inline const QString SAGE_UI_CHANGE_PW_EMPTY_CONFIRM = QStringLiteral("변경할 비밀번호 확인을 입력하세요.");
inline const QString SAGE_UI_CHANGE_PW_MISMATCH = QStringLiteral("변경할 비밀번호가 서로 다릅니다.");
inline const QString SAGE_UI_CHANGE_PW_CURRENT_INVALID = QStringLiteral("현재 비밀번호가 올바르지 않습니다.");
inline const QString SAGE_UI_CHANGE_PW_COMPLETED = QStringLiteral("비밀번호가 변경되었습니다.");
inline const QString SAGE_UI_LOGIN_REQUIRED = QStringLiteral("로그인 상태에서만 사용가능합니다.");

inline const QString SAGE_UI_TIP_CLOSE = QStringLiteral("닫기");
inline const QString SAGE_UI_MSGBOX_TITLE_INFO = QStringLiteral("알림");
inline const QString SAGE_UI_MSGBOX_TITLE_WARNING = QStringLiteral("경고");
inline const QString SAGE_UI_MSGBOX_TITLE_ERROR = QStringLiteral("오류");
inline const QString SAGE_UI_MSGBOX_OK = QStringLiteral("확인");
inline const QString SAGE_LOG_WINDOW_MOVE_UNSUPPORTED =
    QStringLiteral("창 끌어서 이동을 시작할 수 없습니다. Platform=%1");

inline const QString SAGE_UI_COMPLETED = QStringLiteral("완료");
inline const QString SAGE_UI_FAILED = QStringLiteral("실패");

inline const QString SAGE_UI_RESULT_RESULT_LABEL = QStringLiteral("Result");
inline const QString SAGE_UI_RESULT_TOTAL_LABEL = QStringLiteral("Total");
inline const QString SAGE_UI_RESULT_PASSED_PREFIX = QStringLiteral("Passed ");
inline const QString SAGE_UI_RESULT_FAILED_SUFFIX = QStringLiteral(", Failed ");
inline const QString SAGE_UI_RESULT_FIELD = QStringLiteral("항목");
inline const QString SAGE_UI_RESULT_VALUE = QStringLiteral("값");
inline const QString SAGE_UI_RESULT_STATUS = QStringLiteral("상태");
inline const QString SAGE_UI_RESULT_REASON = QStringLiteral("사유");
inline const QString SAGE_UI_RESULT_FILE = QStringLiteral("File");
inline const QString SAGE_UI_RESULT_FOLDER = QStringLiteral("Folder");
inline const QString SAGE_UI_RESULT_ERROR = QStringLiteral("Error");

inline const QString SAGE_RESULT_STATUS_SUMMARY = QStringLiteral("summary");
inline const QString SAGE_RESULT_STATUS_OUTPUT = QStringLiteral("output");
inline const QString SAGE_RESULT_STATUS_SUCCESS = QStringLiteral("success");
inline const QString SAGE_RESULT_STATUS_FAILED = QStringLiteral("failed");
inline const QString SAGE_RESULT_STATUS_ERROR = QStringLiteral("error");

inline const QString SAGE_JSON_KEY_TYPE = QStringLiteral("type");
inline const QString SAGE_JSON_KEY_REQUEST_ID = QStringLiteral("requestId");
inline const QString SAGE_JSON_KEY_SUCCESS = QStringLiteral("success");
inline const QString SAGE_JSON_KEY_PAYLOAD = QStringLiteral("payload");
inline const QString SAGE_JSON_KEY_ERROR = QStringLiteral("error");
inline const QString SAGE_JSON_KEY_CODE = QStringLiteral("code");
inline const QString SAGE_JSON_KEY_MESSAGE = QStringLiteral("message");
inline const QString SAGE_JSON_KEY_STATUS = QStringLiteral("status");
inline const QString SAGE_JSON_KEY_FILE_NAME = QStringLiteral("fileName");
inline const QString SAGE_JSON_KEY_OUTPUT_FOLDER = QStringLiteral("outputFolder");
inline const QString SAGE_JSON_KEY_TOTAL_FILES = QStringLiteral("totalFiles");
inline const QString SAGE_JSON_KEY_PASSED_FILES = QStringLiteral("passedFiles");
inline const QString SAGE_JSON_KEY_FAILED_FILES = QStringLiteral("failedFiles");
inline const QString SAGE_JSON_KEY_INPUT_PATH = QStringLiteral("inputPath");
inline const QString SAGE_JSON_KEY_ROW_NUMS = QStringLiteral("rowNums");
inline const QString SAGE_UI_ROW_NUM_SEPARATOR = QStringLiteral(",");
inline const QString SAGE_JSON_KEY_FILE_PATH = QStringLiteral("filePath");
inline const QString SAGE_JSON_TYPE_RESPONSE = QStringLiteral("response");

inline const QString SAGE_UI_TAB_INPUT = QStringLiteral("입력");
inline const QString SAGE_UI_TAB_RESULT = QStringLiteral("결과");
inline const QString SAGE_UI_SECTION_RESULT = QStringLiteral("처리 결과");
inline const QString SAGE_UI_AMOUNT_EMPTY_MARK = QStringLiteral("—");
inline const QString SAGE_UI_RESULT_RESET_BTN = QStringLiteral("초기화");
inline const QString SAGE_UI_INPUT_RESET_BTN = QStringLiteral("초기화");
inline const QString SAGE_UI_EMPTY_STATE_HINT = QStringLiteral("파일을 선택하여 업무를 시작하세요");
inline const QString SAGE_UI_RESULT_FILTER_PLACEHOLDER = QStringLiteral("검색어 입력");
inline const QString SAGE_UI_SELECT_ALL_BUTTON = QStringLiteral("전체 선택");
inline const QString SAGE_UI_SELECTION_CLEAR_BUTTON = QStringLiteral("선택 해제");
inline const QString SAGE_UI_SELECTION_TOTAL_FORMAT = QStringLiteral("%1건 중");
inline const QString SAGE_UI_SELECTION_SELECTED_FORMAT = QStringLiteral("%1건");
inline const QString SAGE_UI_SELECTION_SUFFIX = QStringLiteral("선택됨");
inline const QString SAGE_UI_SUMMARY_BADGE_FORMAT = QStringLiteral("%1 %2%3");
inline constexpr int SAGE_FILTER_CRITERIA_NONE = -1;
inline constexpr int SAGE_RESULT_CRITERIA_DROP_ROWS = 8;
inline constexpr int SAGE_RESULT_FILTER_MAX_LENGTH = 20;
inline const QString SAGE_UI_TAB_HISTORY = QStringLiteral("실행 기록");
inline const QString SAGE_UI_HISTORY_SUCCESS = QStringLiteral("성공");
inline const QString SAGE_UI_HISTORY_FAILED = QStringLiteral("실패");
inline const QString SAGE_UI_HISTORY_TIME_FORMAT = QStringLiteral("MM-dd HH:mm:ss");
inline const QString SAGE_UI_HISTORY_COL_TIME = QStringLiteral("실행 시각");
inline const QString SAGE_UI_HISTORY_COL_RESULT = QStringLiteral("결과");
inline const QString SAGE_UI_HISTORY_COL_INPUT = QStringLiteral("입력 파일");
inline const QString SAGE_UI_HISTORY_COL_OUTPUT = QStringLiteral("저장 경로");
inline const QString SAGE_UI_HISTORY_COL_REASON = QStringLiteral("사유");
inline const QString SAGE_UI_HISTORY_NO_OUTPUT = QStringLiteral("미리보기 (저장 없음)");
inline const QString SAGE_UI_HISTORY_FILTER_ALL = QStringLiteral("전체 %1");
inline const QString SAGE_UI_HISTORY_FILTER_SUCCESS = QStringLiteral("성공 %1");
inline const QString SAGE_UI_HISTORY_FILTER_FAILED = QStringLiteral("실패 %1");
inline const QString SAGE_UI_HISTORY_EMPTY_TITLE = QStringLiteral("아직 실행 기록이 없습니다");
inline const QString SAGE_UI_HISTORY_EMPTY_DESC = QStringLiteral("문서를 생성하면 여기에 쌓입니다");
inline const QString SAGE_UI_HISTORY_FILTER_EMPTY_TITLE = QStringLiteral("조건에 맞는 기록이 없습니다");
inline const QString SAGE_UI_HISTORY_FILTER_EMPTY_DESC = QStringLiteral("다른 항목을 선택해 보세요");
inline const QString SAGE_JSON_KEY_FILES = QStringLiteral("files");
inline const QString SAGE_JSON_VALUE_SUCCESS = QStringLiteral("success");
inline const QString SAGE_UI_SECTION_INPUT = QStringLiteral("입력 파일");
inline const QString SAGE_UI_SECTION_OUTPUT = QStringLiteral("저장 위치");
inline const QString SAGE_UI_INPUT_CARD_TITLE = QStringLiteral("입력 · 저장 위치");
inline const QString SAGE_UI_INPUT_BUTTON = QStringLiteral("파일 선택");
inline const QString SAGE_UI_OUTPUT_BUTTON = QStringLiteral("폴더 선택");
inline const QString SAGE_UI_SELECT_OUTPUT_TITLE = QStringLiteral("저장 폴더 선택");

inline const QString SAGE_UI_SAMPLE_NAME = QStringLiteral("샘플 업무");
inline const QString SAGE_UI_SAMPLE_CATEGORY = QStringLiteral("샘플");
inline const QString SAGE_UI_SAMPLE_ACTION_BUTTON = QStringLiteral("실행");
inline const QString SAGE_UI_SAMPLE_INPUT_DIALOG_TITLE = QStringLiteral("샘플 입력 파일 선택");
inline const QString SAGE_UI_SAMPLE_INPUT_FILTER = QStringLiteral("모든 파일 (*)");
inline const QString SAGE_UI_SAMPLE_COMPLETED = QStringLiteral("샘플 업무가 완료되었습니다.");
inline const QString SAGE_UI_SAMPLE_STATUS_DONE = QStringLiteral("완료");
inline const QString SAGE_REQUEST_SAMPLE_RUN = QStringLiteral("sample-run");
inline const QString SAGE_REQUEST_UNKNOWN = QStringLiteral("sageqt-unknown");
inline const QString SAGE_ERROR_CODE_WORKFLOW_EXCEPTION = QStringLiteral("SNX_SAGE_WORKFLOW_001");
inline const QString SAGE_ERROR_CODE_WORKFLOW_NOT_FOUND = QStringLiteral("SNX_SAGE_WORKFLOW_002");
inline const QString SAGE_UI_WORKFLOW_EXCEPTION = QStringLiteral("작업 처리 중 예기치 못한 오류가 발생했습니다.");
inline const QString SAGE_UI_WORKFLOW_ALREADY_RUNNING = QStringLiteral("이미 처리 중입니다.");
inline const QString SAGE_UI_WORKFLOW_NOT_FOUND = QStringLiteral("등록된 업무가 없습니다.");
inline const QString SAGE_UI_INPUT_REQUIRED = QStringLiteral("파일을 선택하세요.");
inline const QString SAGE_UI_OUTPUT_REQUIRED = QStringLiteral("저장 위치 폴더를 지정하세요.");
inline const QString SAGE_UI_PROGRESS_FORMAT = QStringLiteral("%1%");
inline const QString SAGE_UI_STATUS_CARD_IDLE = QStringLiteral("대기 중 — 입력 파일을 선택한 뒤 생성하세요");
inline const QString SAGE_UI_STATUS_CARD_RUNNING = QStringLiteral("처리 중 — 엑셀 데이터를 읽는 중입니다");
inline const QString SAGE_UI_STATUS_CARD_COMPLETED_FORMAT = QStringLiteral("%1이 완료되었습니다 · %2건");
inline const QString SAGE_UI_STATUS_CARD_FAILED_FORMAT = QStringLiteral("%1에 실패했습니다");
inline const QString SAGE_UI_STATUS_CARD_LOAD_COMPLETED_FORMAT = QStringLiteral("불러오기가 완료되었습니다 · %1건");
inline const QString SAGE_UI_STATUS_CARD_LOAD_FAILED = QStringLiteral("불러오기에 실패했습니다");
inline const QString SAGE_UI_OUTPUT_PATH_MISSING =
    QStringLiteral("저장한 파일을 찾을 수 없습니다. 옮겨졌거나 삭제되었습니다.");
inline const QString SAGE_UI_STATUS_CARD_OPEN_FOLDER = QStringLiteral("폴더 열기");

inline constexpr int SAGE_PROGRESS_TIMER_MS = 300;
inline constexpr int SAGE_PROGRESS_STEP = 3;
inline constexpr int SAGE_PROGRESS_RUNNING_MAX = 95;
inline constexpr int SAGE_PROGRESS_COMPLETE = 100;
inline constexpr int SAGE_SAMPLE_TOTAL_FILES = 1;
inline constexpr int SAGE_SAMPLE_PASSED_FILES = 1;
inline constexpr int SAGE_SAMPLE_FAILED_FILES = 0;

inline constexpr int SAGE_USER_PW_MIN_LEN = 4;
inline constexpr int SAGE_USER_PW_MAX_LEN = 15;
inline const QString SAGE_USER_PW_ALLOWED_CHARACTERS =
    QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
inline const QString SAGE_UI_PW_EMPTY = QStringLiteral("비밀번호를 입력하세요.");
inline const QString SAGE_UI_CHANGE_PW_TOO_SHORT = QStringLiteral("비밀번호는 4자 이상이어야 합니다.");
inline const QString SAGE_UI_CHANGE_PW_TOO_LONG = QStringLiteral("비밀번호는 15자 이하이어야 합니다.");
inline const QString SAGE_UI_CHANGE_PW_INVALID_CHAR = QStringLiteral("비밀번호는 영문과 숫자만 사용할 수 있습니다.");

inline const QString SAGE_DEFAULT_ADMIN_ID = QStringLiteral("admin");
inline const QString SAGE_UI_INITIAL_ADMIN_PW_FORMAT =
    QStringLiteral("관리자 계정을 새로 만들었습니다.\n\n아이디: %1\n초기 비밀번호: %2\n\n"
                   "이 비밀번호는 지금 한 번만 표시됩니다. 적어 두고 첫 로그인에서 변경하세요.");
inline constexpr int SAGE_INITIAL_PW_LENGTH = 14;
inline const QString SAGE_INITIAL_PW_ALPHABET =
    QStringLiteral("ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789");
