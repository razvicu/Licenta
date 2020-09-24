//
//  Application.cpp
//  Licenta
//
//  Created by Razvan on 3/23/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "Application.hpp"

Application::Application(std::string id, std::string name, int allowed_time, int last_updated_time) {
    this->id = id;
    this->name = name;
    this->allowed_for_duration = allowed_time;
    this->last_updated_time = last_updated_time;
}

void Application::save() {
    SQLite3 db(DB_PATH);
    const std::string statement_ = "REPLACE INTO applications (id, name, allowed_time, last_updated_time) VALUES (?, ?, ?, ?)";
    Statement stmt(db.getConnection(), statement_);
    stmt.setString(1, id);
    stmt.setString(2, name);
    stmt.setInt(3, allowed_for_duration);
    stmt.setInt(4, last_updated_time);
    stmt.execute(nullptr);
}

std::string Application::getId() const {
    return this->id;
}

std::string Application::getName() const {
    return this->name;
}

int Application::getAllowedForDuration() const {
    return this->allowed_for_duration;
}

void Application::setAllowedForDuration(int dur) {
    this->allowed_for_duration = dur;
}

void Application::setLastUpdatedTime(int last_time) {
    this->last_updated_time = last_time;
}

int Application::getLastUpdatedTime() const {
    return this->last_updated_time;
}
