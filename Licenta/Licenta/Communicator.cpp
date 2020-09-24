//
//  communicator.cpp
//  Licenta
//
//  Created by Razvan on 2/13/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "Communicator.hpp"

void Communicator::populateAppsFromDb(Row& row) {
    std::cout << row.getInt(0) << " " << row.getString(1) << " " << row.getInt(2) << " " << row.getInt(3) << "\n";
    apps_.push_back(Application{std::to_string(row.getInt(0)), row.getString(1), row.getInt(2), row.getInt(3)});
}

void Communicator::populateWebsitesFromDb(Row& row) {
    std::cout << row.getInt(0) << " " << row.getString(1) << "\n";
    websites_.push_back(Website(row.getString(1)));
}

void Communicator::restrictWebsites() {
    r.restrictWebsites(websites_);
}

void Communicator::onWebsiteListReceived(std::string const& name, sio::message::ptr const& data,
                                         bool isAck, sio::message::list &ack_resp) {
    std::shared_ptr<SQLite3> db = std::make_shared<SQLite3>(DB_PATH);

    if (data->get_flag() == sio::message::flag::flag_string && data->get_string() == "\"[]\"") {
        LOG_DBG("%s", "something went wrong with data sent from the server");
        SQLite3::dropWebsitesTable(db);
        websites_.clear();
    } else if (data->get_flag() == sio::message::flag::flag_array && data->get_vector().size() > 0 ) {
        // we clear the website data stored
        SQLite3::dropWebsitesTable(db);
        SQLite3::createWebsitesTable(db);
        websites_.clear();
        std::vector<std::shared_ptr<sio::message>> websites = data->get_vector();
        
        if (websites.size() == 1 && websites[0]->get_map()["str"]->get_string() == "empty") {
            LOG_DBG("%s", "received empty websites array from the server");
            restrictWebsites();
            return;
        }
        
        Statement *sites_stmt = new Statement(db.get()->getConnection(), "SELECT * FROM websites;");
        using std::placeholders::_1;
        sites_stmt->execute(std::bind(&Communicator::populateWebsitesFromDb, this, _1));
            
        // here we save to db if we dont have the website
        for (auto website_json : websites) {
            std::string website_link_ = website_json->get_map()["str"]->get_string();

            LOG_DBG("received website with link %s", website_link_.c_str());
            
            auto website_ = std::find_if(websites_.begin(), websites_.end(), [=](const Website& website) {
                return website_link_ == website.getLink(); });
            
            if (website_ == websites_.end()) {
                Website website{website_link_};
                website.save();
                websites_.push_back(website);
            }
        }
    }
    restrictWebsites();

}

void Communicator::onAppListReceived(std::string const& name, sio::message::ptr const& data,
                       bool isAck, sio::message::list &ack_resp) {
    std::shared_ptr<SQLite3> db = std::make_shared<SQLite3>(DB_PATH);
    
    // if there is a server error we handle it here
    if (data->get_flag() == sio::message::flag::flag_string && data->get_string() == "\"[]\"") {
        LOG_DBG("%s", "something went wrong with data sent from the server");
        SQLite3::dropApplicationsTable(db);
        apps_.clear();
        apps_blocked_.clear();
    } else if (data->get_flag() == sio::message::flag::flag_array && data->get_vector().size() > 0 ) {
        // we clear the application data stored
        SQLite3::dropApplicationsTable(db);
        SQLite3::createApplicationsTable(db);
        apps_.clear();
        apps_blocked_.clear();
        std::vector<std::shared_ptr<sio::message>> apps = data->get_vector();
        
        if (apps.size() == 1 && apps[0]->get_map()["_id"]->get_string() == "-1") {
            LOG_DBG("%s", "received empty apps array from the server");
            return;
        }
        
        Statement *apps_stmt = new Statement(db.get()->getConnection(), "SELECT * FROM applications;");
        apps_stmt->execute(std::bind(&Communicator::populateAppsFromDb, this, std::placeholders::_1));
    
        // here we save to db if we dont have the app or update
        for (auto app_json : apps) {
            std::string app_id_ = app_json->get_map()["_id"]->get_string();
            std::string app_name_ = app_json->get_map()["name"]->get_string();
            int app_time_allowed_ = app_json->get_map()["timeAllowed"]->get_int();
            int app_time_updated_ = app_json->get_map()["timeUpdated"]->get_int();

            LOG_DBG("received app with id: %s, name: %s, time allowed: %d, time updated: %d",
                    app_id_.c_str(), app_name_.c_str(), app_time_allowed_, app_time_updated_);
            auto app_ = std::find_if(apps_.begin(), apps_.end(), [=](const Application& app) {
                                        return app_id_ == app.getId(); });
            // if the app does not exist in the list, we add it
            if (app_ == apps_.end()) {
                Application app{app_id_, app_name_, app_time_allowed_, app_time_updated_};
                app.save();
                apps_.push_back(app);
            // if the app exists in the list we update the time
            } else {
                app_->setAllowedForDuration(app_time_allowed_);
                app_->setLastUpdatedTime(app_time_updated_);
                app_->save();
            }
        }
    } else {
        std::cout << "rcv unknown data";
    }
}

void Communicator::getRestrictionList() {
    websocket.connect("wss://localhost:3098");
    
    std::shared_ptr<SQLite3> db = std::make_shared<SQLite3>(DB_PATH);
    SQLite3::createApplicationsTable(db);
    SQLite3::createWebsitesTable(db);
    
    Statement apps_stmt{db.get()->getConnection(), "SELECT * FROM applications;"};
    Statement sites_stmt{db.get()->getConnection(), "SELECT * FROM websites;"};
    apps_stmt.execute(std::bind(&Communicator::populateAppsFromDb, this, std::placeholders::_1));
    sites_stmt.execute(std::bind(&Communicator::populateWebsitesFromDb, this, std::placeholders::_1));

    restrictWebsites();
    
    int attempts = 0;
    bool initialConnOpened = websocket.opened();
    bool nextConnOpened = initialConnOpened;
            
    while(true) {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        using std::placeholders::_4;
        websocket.socket()->on("appsEmit", std::bind(&Communicator::onAppListReceived, this, _1, _2, _3, _4));
        websocket.socket()->on("websitesEmit", std::bind(&Communicator::onWebsiteListReceived, this, _1, _2, _3, _4));
        
        if (websocket.opened()) {
            r.enablePf();
            nextConnOpened = true;
            attempts = 0;
        }
        
        if (nextConnOpened == true && attempts >= 5 * 60 && apps_.size()) {
            apps_.clear();
            r.disableWebsitesRestriction();
            LOG_DBG("%s", "Deleting stored data because we lost server connection for more than 5 minutes");
            attempts = 0;
        }
        
        if (nextConnOpened == false && attempts >= 10 && apps_.size()) {
            apps_.clear();
            r.disableWebsitesRestriction();
            LOG_DBG("%s", "Deleting stored data because we did not successfully comunicate with the server");
            attempts = 0;
        }
        
        if ((nextConnOpened || !initialConnOpened) && !websocket.opened())
            attempts++;
        
        apps_blocked_.setVector(r.restrictApps(apps_));
        sleep(1);
    }
}

std::vector<Application>& Communicator::getApplicationsRef() {
    return apps_;
}

std::vector<std::string> Communicator::getBlockedApplications() {
    return apps_blocked_.get();
}



