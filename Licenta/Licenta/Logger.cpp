//
//  Logger.cpp
//  Licenta
//
//  Created by Razvan on 5/11/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "Logger.hpp"

Logger::Logger() {
}

Logger::~Logger() {
    fout.close();
}

std::mutex Logger::mtx;
std::ofstream Logger::fout("/private/opt/licenta/log/daemon.log");

void Logger::log(const char *func, const char *file, long line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buff[4096] = {};
    char buff_[32] = {};
    
    std::lock_guard<std::mutex> lck(mtx);
    time_t current_time = time(nullptr);
    std::tm * ptm = std::localtime(&current_time);
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buff_, 32, "%d.%m.%Y %H:%M:%S", ptm);
    fout << "[" << buff_ << " " << file << "::" << func << " " << line << "] ";
    vsprintf(buff, fmt, args);
    va_end(args);
    fout << buff << "\n";
    fout.flush();
}
