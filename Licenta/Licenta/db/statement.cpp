//
//  statement.cpp
//  Licenta
//
//  Created by Razvan on 4/3/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "statement.hpp"

Statement::Statement(sqlite3* connection, const std::string &statement) {
    int rc = sqlite3_prepare(connection, statement.c_str(), -1, &stmt, 0);
    if (rc) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(connection) << "\n";
        return;
    }
    std::cout << "Statement prepared successfully\n";
}

Statement::~Statement() {
    sqlite3_finalize(stmt);
}

void Statement::setInt(int index, int value) {
    sqlite3_bind_int(stmt, index, value);
}

void Statement::setString(int index, const std::string& value) {
    sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
}

void Statement::execute(std::function<void(Row &row)> rowCb) {
    int res = 0;
    Row row;
    do {
        res = sqlite3_step(stmt);
        switch (res) {
            case SQLITE_DONE:
                std::cout << "Finished reading!\n";
                return;
            case SQLITE_ROW:
                row.setStatement(stmt);
                rowCb(row);
                break;
            default:
                sqlite3_reset(stmt);
                std::cout << "E bai! Error encountered: " << sqlite3_db_handle(stmt) << "\n";
        }
    } while (res == SQLITE_ROW);
    sqlite3_reset(stmt);
}


