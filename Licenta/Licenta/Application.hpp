//
//  Application.h
//  Licenta
//
//  Created by Razvan on 3/23/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <iostream>
#include "db/dbconn.hpp"
#include "db/statement.hpp"
#include "db/row.hpp"

#define DB_PATH "/private/opt/licenta/lic.db"

class Application {
public:
    Application(std::string, std::string, int, int);
    void save();
    std::string getId() const;
    std::string getName() const;
    int getAllowedForDuration() const;
    void setAllowedForDuration(int);
    void setLastUpdatedTime(int);
    int getLastUpdatedTime() const;
private:
    std::string id;
    std::string name;
    int allowed_for_duration;
    int last_updated_time;
};


#endif /* Header_h */
