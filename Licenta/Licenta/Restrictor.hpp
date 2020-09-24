//
//  Restrictor.hpp
//  Licenta
//
//  Created by Razvan on 5/9/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef Restrictor_hpp
#define Restrictor_hpp

#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <mutex>
#include "Application.hpp"
#include "Website.hpp"

class Restrictor {
public:
    ~Restrictor();
    std::vector<std::string> restrictApps(std::vector<Application>);
    void disableWebsitesRestriction();
    void restrictWebsites(std::vector<Website>);
    void enablePf();
    void disablePf();
};


#endif /* Restrictor_hpp */
