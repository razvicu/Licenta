//
//  dbconn.hpp
//  Licenta
//
//  Created by Razvan on 4/3/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef dbconn_hpp
#define dbconn_hpp

#include <iostream>
#include <sqlite3.h>
#include <string>

#include "statement.hpp"

#define DB_PATH "/private/opt/licenta/lic.db"


class SQLite3 {
public:
    SQLite3(const std::string&);
    ~SQLite3();
    void connect();
    void open(const std::string&);
    Statement prepare(const std::string&);
    void execute(const std::string&);
    void close();
    void query();
    bool isOpen();
    sqlite3* getConnection();
    static void createApplicationsTable(std::shared_ptr<SQLite3>);
    static void createWebsitesTable(std::shared_ptr<SQLite3>);
    static void dropWebsitesTable(std::shared_ptr<SQLite3>);
    static void dropApplicationsTable(std::shared_ptr<SQLite3>);
private:
    sqlite3* connection;
};

#endif /* dbconn_hpp */
