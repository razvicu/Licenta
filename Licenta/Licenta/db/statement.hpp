//
//  statement.hpp
//  Licenta
//
//  Created by Razvan on 4/3/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef statement_hpp
#define statement_hpp

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <functional>
#include "row.hpp"

class Statement {
public:
    Statement(sqlite3 *, const std::string &);
    ~Statement();
    void setInt(int, int);
    void setString(int, const std::string&);
    void execute(std::function<void(Row &row)> rowCb);
    
private:
    sqlite3_stmt *stmt;
};

#endif /* statement_hpp */
