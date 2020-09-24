//
//  Website.cpp
//  Licenta
//
//  Created by Razvan on 4/15/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "Website.hpp"

void Website::save() {
    SQLite3 db(DB_PATH);
    const std::string statement_ = "INSERT INTO websites (website) VALUES (?) ";
    Statement stmt(db.getConnection(), statement_);
    stmt.setString(1, link);
    stmt.execute(nullptr);
}

std::string Website::getLink() const {
    return this->link;
}

Website::Website(std::string website) {
    this->link = website;
}
