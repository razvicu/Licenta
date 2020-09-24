//
//  EndpointSecurityF.cpp
//  Licenta
//
//  Created by Razvan on 5/11/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#include "EndpointSecurityMonitor.h"

EndpointSecurityMonitor::EndpointSecurityMonitor(std::shared_ptr<Communicator> comm) : es_client_(nullptr),
            comm_(comm) {
                initializeEsClient();
}

EndpointSecurityMonitor::~EndpointSecurityMonitor() {
    uninitializeEsClient();
}

void EndpointSecurityMonitor::uninitializeEsClient() {
    if (es_client_) {
        es_return_t res = ES_RETURN_SUCCESS;
        res = es_unsubscribe_all(es_client_);
        if (res != ES_RETURN_SUCCESS) {
            std::cout << "Endpoint Security client unsubscribe failed";
        }
        res = es_delete_client(es_client_);
        if (res != ES_RETURN_SUCCESS) {
            std::cout << "Endpoint Security client delete failed";
        }
    }
}

void EndpointSecurityMonitor::initializeEsClient() {
    es_new_client_result_t result = es_new_client(&es_client_, [&](es_client_t *client, const es_message_t *message) {
        apps_ = comm_.get()->getBlockedApplications();
        es_process_t* process = nullptr;
        switch(message->event_type) {
            case ES_EVENT_TYPE_AUTH_EXEC:
                process = message->event.exec.target;
                std::cout << "AUTH event\n";
                break;

            default:
                break;
        }

        es_string_token_t path = process->executable->path;
        const char *path_char = path.data;
    
        for (const auto &app : apps_)
            if (strstr(path_char, app.c_str()) != NULL) {// && strstr(path_char, "Applications")) {
               std::cout << "Process is " << path_char << "\n";
               es_auth_result_t result = ES_AUTH_RESULT_DENY;
               es_respond_auth_result(client, message, result, false);
           }
        es_auth_result_t result = ES_AUTH_RESULT_ALLOW;
        es_respond_auth_result(client, message, result, false);
    });
    if (result != ES_NEW_CLIENT_RESULT_SUCCESS) {
       std::cout << "Error: es_new_client() failed with " << result << "\n";
        return;
    }
    
    subscribeToEventsOfInterest();
    
}

void EndpointSecurityMonitor::subscribeToEventsOfInterest() {
    // if the es_client_ was not properly initialized, abort
    if (!es_client_)
        return;
    
    es_event_type_t events[] = {
        ES_EVENT_TYPE_AUTH_EXEC,
    };
    
    es_return_t subscribed = es_subscribe(es_client_, events, sizeof(events)/sizeof(events[0]));
    
    if (subscribed != ES_RETURN_SUCCESS) {
        std::cout << "Error: es_subscribe() failed\n";
        return;
    }
    std::cout << "Success: es_subscribe() executed ok\n";
}
