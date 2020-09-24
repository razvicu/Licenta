//
//  Website.hpp
//  Licenta
//
//  Created by Razvan on 4/15/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef Website_hpp
#define Website_hpp

#include "db/dbconn.hpp"
#include "db/statement.hpp"
#include "db/row.hpp"

#define DB_PATH "/private/opt/licenta/lic.db"

class Website {
public:
    Website(std::string);
    void save();
    std::string getLink() const;
private:
    std::string link;
};

#endif /* Website_hpp */
