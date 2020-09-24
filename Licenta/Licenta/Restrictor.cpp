//
//  Restrictor.cpp
//  Licenta
//
//  Created by Razvan on 5/9/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "Restrictor.hpp"


// computes the blocking time and adds blocked apps to the list
std::vector<std::string> Restrictor::restrictApps(std::vector<Application> apps_) {
    std::vector<std::string> apps_blocked_;
    for (const auto &app: apps_) {
        time_t current_time = time(nullptr);
        if ( current_time - app.getAllowedForDuration() * 60 >= app.getLastUpdatedTime() ) {
            std::cout << "Blocking app " << app.getName() << " because " << app.getAllowedForDuration() <<
                    " minutes have passed\n";
            apps_blocked_.push_back(app.getName());
        }
    }
    return apps_blocked_;
}

Restrictor::~Restrictor() {
    system("pfctl -d");
}

void Restrictor::disableWebsitesRestriction() {
    system("pfctl -d");
}

void Restrictor::restrictWebsites(std::vector<Website> websites_) {
    std::ofstream config_file("/etc/licentapf.config", std::ios::trunc);
    std::string config_content = "scrub-anchor \"com.apple/*\"\n" \
                              "nat-anchor \"com.apple/*\"\n" \
                              "rdr-anchor \"com.apple/*\"\n" \
                              "dummynet-anchor \"com.apple/*\"\n" \
                              "anchor \"com.apple/*\"\n" \
                              "load anchor \"com.apple\" from \"/etc/pf.anchors/com.apple\"\n\n";
    std::string table_ = "table <blocked> const {";
    std::string block_ = "block out proto tcp from any to <blocked> port {80, 443}\n";
    std::stringstream ss;
    ss << config_content << table_;;
    for (int i = 0; i < websites_.size(); ++i) {
        ss << websites_[i].getLink();
        if (i != websites_.size() - 1)
            ss << ", ";
    }
    
    ss << "}\n" << block_;
    
    config_file << ss.rdbuf();
    config_file.close();
}

void Restrictor::enablePf() {
    system("pfctl -ef /etc/licentapf.config");
}

void Restrictor::disablePf() {
    system("pfctl -d");
}
