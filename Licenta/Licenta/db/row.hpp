//
//  row.hpp
//  Licenta
//
//  Created by Razvan on 4/3/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef row_hpp
#define row_hpp

#include <iostream>
#include <sqlite3.h>
#include <string>

class Row {
public:
    Row();
    Row(sqlite3_stmt*);
    void setStatement(sqlite3_stmt*);
    int getInt(int);
    std::string getString(int);
    
private:
    sqlite3_stmt *stmt;
};

#endif /* row_hpp */
