//
//  communicator.h
//  Licenta
//
//  Created by Razvan on 2/13/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef communicator_hpp
#define communicator_hpp

#include <fstream>
#include <vector>
#include <string>
#include <sio_client.h>

#include "db/dbconn.hpp"
#include "Restrictor.hpp"
#include "Logger.hpp"
#include "Application.hpp"
#include "Website.hpp"
#include "ThreadSafeVector.hpp"


#define DB_PATH "/private/opt/licenta/lic.db"


class Communicator {
public:
    Communicator() {}
    void getRestrictionList();
    std::vector<Application>& getApplicationsRef();
    std::vector<std::string> getBlockedApplications();
    void populateAppsFromDb(Row &);
    void populateWebsitesFromDb(Row &);
private:
    void onAppListReceived(std::string const&, sio::message::ptr const&, bool, sio::message::list &);
    void onWebsiteListReceived(std::string const&, sio::message::ptr const&, bool, sio::message::list &);
    void restrictWebsites();
    
    Restrictor r;
    sio::client websocket;
    std::vector<Application> apps_;
    std::vector<Website> websites_;
    ThreadSafeVector<std::string> apps_blocked_;
};


#endif /* communicator_h */
