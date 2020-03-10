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
#include <stdlib.h>
#include <unistd.h>

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

        char to_send_msg[] = "poll";
        ssize_t nums = send(c, &to_send_msg, 4, 0);
        if (nums == 0)
            std::cout << "Error sending data\n";
        ssize_t numb = recv(c, &buffer, sizeof(buffer), 0);
        while (numb == 0) {
            nums = send(c, &to_send_msg, 4, 0);
            if (nums == 0)
                std::cout << "Error sending data\n";
            std::cout << "Did not receive data, retrying...\n";
            numb = recv(c, &buffer, sizeof(buffer), 0);
            sleep(5);
        }
        buffer[numb] = '\0';
        
        char *pch = strtok(buffer, "\n");
        
        std::vector<std::string> apps_(apps);
        apps.clear();
        
        while(pch) {
            printf("Value received is %s\n", pch);
            apps.push_back(pch);
            pch = strtok(NULL, "\n");
        }
        
        if (apps.size() == 1 && apps[0] == "none")
            apps = apps_;
        
        shutdown(c, 0);
        sleep(10);
    }
    
}



