//
//  row.cpp
//  Licenta
//
//  Created by Razvan on 4/3/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "row.hpp"

Row::Row() {}

Row::Row(sqlite3_stmt *stmt) {
    this->stmt = stmt;
}

void Row::setStatement(sqlite3_stmt *stmt_) {
    this->stmt = stmt_;
}

int Row::getInt(int idx) {
    return sqlite3_column_int(stmt, idx);
}

std::string Row::getString(int idx) {
    return std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, idx)));
}


