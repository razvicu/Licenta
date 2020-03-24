//
//  Header.h
//  Licenta
//
//  Created by Razvan on 3/23/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef Header_hpp
#define Header_hpp

#include <iostream>

class Application {
public:
    Application(std::string id, std::string name, int time) {
        this->id = id;
        this->name = name;
        this->allowed_for_duration = time;
        this->start_count = std::time(nullptr);
    }
    std::string getId() const {
        return this->id;
    }
    std::string getName() const {
        return this->name;
    }
    int getAllowedForDuration() const {
        return this->allowed_for_duration;
    }
    time_t getStartCount() const {
        return this->start_count;
    }
    friend std::ostream& operator<<(std::ostream& out, const Application& app) {
        out << "id: " << app.id << " name: " << app.name << " allowed for: " << app.allowed_for_duration <<
            " starting at: " << app.start_count << "\n";
        return out;
    }
private:
    std::string id;
    std::string name;
    int allowed_for_duration;
    time_t start_count;
};


#endif /* Header_h */
