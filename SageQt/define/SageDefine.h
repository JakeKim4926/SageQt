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

inline const QString SAGE_UI_MAIN_WINDOW_TITLE = QStringLiteral("SageQt");

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
