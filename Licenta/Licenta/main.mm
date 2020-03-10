//
//  main.cpp
//  Licenta
//
//  Created by Razvan on 2/15/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#import <EndpointSecurity/EndpointSecurity.h>
#import <bsm/libbsm.h>
#import "communicator.hpp"
#import <Cocoa/Cocoa.h>


es_client_t* endpointClient = nullptr;
std::vector<std::string> apps;


int main() {
    es_new_client_result_t result = es_new_client(&endpointClient, [](es_client_t *client, const es_message_t *message)
    {
        es_process_t* process = NULL;
        switch(message->event_type) {
            case ES_EVENT_TYPE_AUTH_EXEC:
                process = message->event.exec.target;
                std::cout << "AUTH event\n";
                break;
            case ES_EVENT_TYPE_NOTIFY_EXEC:
                process = message->event.exec.target;
                std::cout << "NOTIFY event\n";
                break;
            case ES_EVENT_TYPE_NOTIFY_GETATTRLIST: {
                process = message->event.exec.target;
                std::cout << "FSGETPATH event\n";
                for (const auto &app_ : apps)
                    for ( NSRunningApplication* app in NSWorkspace.sharedWorkspace.runningApplications )
                    {
                        if ( [[NSString stringWithUTF8String:app_.c_str()] isEqualToString:[app.executableURL lastPathComponent]] )
                        {
                            std::cout << "Terminating app " << [app.executableURL.absoluteString UTF8String] << "\n";
                            
                            [app terminate];
                        }
                    }
                return;
                break;
            }

            default:
                break;
        }

        es_string_token_t path = process->executable->path;
        const char *path_char = path.data;
        for (const auto &app : apps)
           if (strstr(path_char, app.c_str()) != NULL && strstr(path_char, "Applications") != NULL) {
               std::cout << "Process is " << path_char << "\n";
               es_auth_result_t result = ES_AUTH_RESULT_DENY;
               es_respond_auth_result(client, message, result, false);
        }
        es_auth_result_t result = ES_AUTH_RESULT_ALLOW;
        es_respond_auth_result(client, message, result, false);

    });
    std::cout << "Before es_subscribe() call\n";

    if (ES_NEW_CLIENT_RESULT_SUCCESS != result) {
       std::cout << "Error: es_new_client() failed with " << result << "\n";
    }
    es_event_type_t events[] = {
        ES_EVENT_TYPE_AUTH_EXEC,
        ES_EVENT_TYPE_NOTIFY_EXEC,
        ES_EVENT_TYPE_NOTIFY_GETATTRLIST

    };
    if (ES_RETURN_SUCCESS != es_subscribe(endpointClient, events, sizeof(events)/sizeof(events[0]))) {
       std::cout << "Error: es_subscribe() failed\n";
    }
    std::thread t(getApplicationsList, std::ref(apps));
    t.detach();
    NSRunLoop *loop = [NSRunLoop mainRunLoop];
    [loop run];
}
