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
inline const QString SAGE_JSON_TYPE_RESPONSE = QStringLiteral("response");

inline const QString SAGE_UI_TAB_INPUT = QStringLiteral("입력");
inline const QString SAGE_UI_TAB_RESULT = QStringLiteral("결과");
inline const QString SAGE_UI_TAB_HISTORY = QStringLiteral("실행 기록");
inline const QString SAGE_UI_SECTION_INPUT = QStringLiteral("입력 파일");

inline const QString SAGE_UI_SAMPLE_NAME = QStringLiteral("샘플 업무");
inline const QString SAGE_UI_SAMPLE_CATEGORY = QStringLiteral("샘플");
inline const QString SAGE_UI_SAMPLE_ACTION_BUTTON = QStringLiteral("실행");
inline const QString SAGE_UI_SAMPLE_INPUT_DIALOG_TITLE = QStringLiteral("샘플 입력 파일 선택");
inline const QString SAGE_UI_SAMPLE_COMPLETED = QStringLiteral("샘플 업무가 완료되었습니다.");
inline const QString SAGE_UI_SAMPLE_STATUS_DONE = QStringLiteral("완료");
inline const QString SAGE_REQUEST_SAMPLE_RUN = QStringLiteral("sample-run");
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
