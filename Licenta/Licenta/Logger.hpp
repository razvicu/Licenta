//
//  Logger.hpp
//  Licenta
//
//  Created by Razvan on 5/11/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <fstream>
#include <list>

#define LOG_DBG(msg, ...) Logger::log(__FUNCTION__, __FILE_NAME__, __LINE__, msg, __VA_ARGS__)

class Logger {
public:
    Logger();
    ~Logger();
    static void log(const char *, const char *, long, const char *, ...);
private:
    static std::ofstream fout;
    static std::mutex mtx;
};

#endif /* Logger_hpp */
