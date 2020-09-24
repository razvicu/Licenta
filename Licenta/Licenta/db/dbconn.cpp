//
//  dbconn.cpp
//  Licenta
//
//  Created by Razvan on 4/3/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "dbconn.hpp"

SQLite3::SQLite3(const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), &connection);
    if (rc) {
        std::cout << "Error creating the db: " << sqlite3_errmsg(connection) << "\n";
        return;
    }
    std::cout << "Db created succesfully!\n";
}

SQLite3::~SQLite3() {
    close();
}

Statement SQLite3::prepare(const std::string & query) {
    return Statement(connection, query);
}

void SQLite3::execute(const std::string & statement) {
    int rc = sqlite3_exec(connection, statement.c_str(), nullptr, nullptr, nullptr);
    if (rc) {
        std::cout << "Error executing statement!\n";
        return;
    }
    std::cout << "Statement executed succesfully!\n";
}

void SQLite3::close() {
    int rc = sqlite3_close(connection);
    connection = nullptr;
    if (rc) {
        std::cout << "Error closing DB\n";
        return;
    }
    std::cout << "DB closed succesfuly!\n";
}

bool SQLite3::isOpen() {
    return (connection != nullptr);
}

sqlite3* SQLite3::getConnection() {
    return connection;
}

void SQLite3::createApplicationsTable(std::shared_ptr<SQLite3> db) {
    const std::string statement_ = "CREATE TABLE IF NOT EXISTS applications (id STRING PRIMARY KEY, \
                                    name TEXT NOT NULL, allowed_time INT, last_updated_time INT);";
    Statement stmt(db.get()->getConnection(), statement_);
    stmt.execute(nullptr);
}

void SQLite3::createWebsitesTable(std::shared_ptr<SQLite3> db) {
    const std::string statement_ = "CREATE TABLE IF NOT EXISTS websites (id INTEGER PRIMARY KEY AUTOINCREMENT, \
                                    website TEXT NOT NULL);";
    Statement stmt(db.get()->getConnection(), statement_);
    stmt.execute(nullptr);
}

void SQLite3::dropWebsitesTable(std::shared_ptr<SQLite3> db) {
    const std::string statement_ = "DROP table websites";
    Statement stmt(db.get()->getConnection(), statement_);
    stmt.execute(nullptr);
}

void SQLite3::dropApplicationsTable(std::shared_ptr<SQLite3> db) {
    const std::string statement_ = "DROP table applications";
    Statement stmt(db.get()->getConnection(), statement_);
    stmt.execute(nullptr);
}
