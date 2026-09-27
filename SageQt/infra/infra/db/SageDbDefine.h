#pragma once

#include <QString>

inline constexpr int SAGE_DB_BUSY_TIMEOUT_MS = 5000;

inline const QString SAGE_DB_FILE_NAME = QStringLiteral("sageqt.db");
inline const QString SAGE_DB_DRIVER = QStringLiteral("QSQLITE");
inline const QString SAGE_DB_BUSY_TIMEOUT_OPTION = QStringLiteral("QSQLITE_BUSY_TIMEOUT=%1");
inline const QString SAGE_DB_CONNECTION_NAME_PREFIX = QStringLiteral("sage-db-");

inline const QString SAGE_DB_ERROR_DIRECTORY_NOT_FOUND = QStringLiteral("DB 폴더 경로를 찾을 수 없습니다. DBPath=%1");
inline const QString SAGE_DB_ERROR_DIRECTORY_IS_FILE =
    QStringLiteral("동일한 이름의 파일이 존재하여 폴더를 만들 수 없습니다. Path=%1");
inline const QString SAGE_DB_ERROR_CREATE_DIRECTORY = QStringLiteral("폴더 생성 실패. Path=%1");
inline const QString SAGE_DB_ERROR_OPEN = QStringLiteral("SQLite DB 열기 실패. DBPath=%1, Error=%2");
inline const QString SAGE_DB_ERROR_EXECUTE = QStringLiteral("SQLite SQL 실행 실패. Error=%1");
