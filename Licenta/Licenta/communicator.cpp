//
//  communicator.c
//  Licenta
//
//  Created by Razvan on 2/13/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "communicator.hpp"

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <unistd.h>

#include "Application.hpp"
#include "nlohmann/json.hpp"

std::vector<Application> apps_;

void getApplicationsList(std::vector<std::string> &apps) {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(3099);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    char buffer[2048];
    
    signal(SIGPIPE, SIG_IGN);
    
    while(true) {
        int c = socket(AF_INET, SOCK_STREAM, 0); // TCP sockets have to be recreated
        if (c < 0) {
            printf("Error creating socket\n");
        }
        while (connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
            printf("Error connecting\n");
            c = socket(AF_INET, SOCK_STREAM, 0); // TCP sockets have to be recreated
            if (c < 0) {
                printf("Error creating socket\n");
            }
            sleep(5);
        }

        const char request_poll[] = "poll";
        ssize_t nums = send(c, &request_poll, 4, 0);
        if (nums == 0)
            std::cout << "Error sending data\n";
        ssize_t numb = recv(c, &buffer, sizeof(buffer), 0);
        while (numb == 0) {
            nums = send(c, &request_poll, 4, 0);
            if (nums == 0)
                std::cout << "Error sending data\n";
            std::cout << "Did not receive data, retrying...\n";
            numb = recv(c, &buffer, sizeof(buffer), 0);
            sleep(5);
        }
        buffer[numb] = '\0';
            
        nlohmann::json apps_json = nlohmann::json::parse(buffer);
                        
        for (const auto &app_json : apps_json) {
            auto app_ = std::find_if(apps_.begin(), apps_.end(), [app_json](const Application& app) {
                                        return app_json["_id"] == app.getId(); });
            if (app_ == apps_.end())
                apps_.emplace_back(app_json["_id"], app_json["name"], app_json["time"]);
        }
        
        for (const auto &app: apps_) {
            time_t current_time = time(nullptr);
            if ( current_time - app.getAllowedForDuration() * 60 >= app.getStartCount() ) {
                std::cout << "Blocking app " << app.getName() << " because " << app.getAllowedForDuration() <<
                        " minutes have passed\n";
                if (std::find(apps.begin(), apps.end(), app.getName()) == apps.end())
                    apps.push_back(app.getName());
            }
        }
        
        for (const auto a: apps_)
            std::cout << a;
        
        std::cout << "\n";
        
        for (const auto a : apps)
            std::cout << a << " ";
        
        shutdown(c, 0);
        sleep(10);
    }
    
}



