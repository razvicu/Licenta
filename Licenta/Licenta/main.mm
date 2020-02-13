//
//  main.m
//  Licenta
//
//  Created by Razvan on 1/2/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <EndpointSecurity/EndpointSecurity.h>
#import <bsm/libbsm.h>

es_client_t* endpointClient = nil;

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
    }
    es_new_client_result_t result = es_new_client(&endpointClient, ^(es_client_t *client, const es_message_t *message)
    {
        es_process_t* process = NULL;
        switch(message->event_type) {
            case ES_EVENT_TYPE_AUTH_EXEC:
                process = message->event.exec.target;
                NSLog(@"AUTH event\n");
                break;
            case ES_EVENT_TYPE_NOTIFY_EXEC:
                process = message->event.exec.target;
                NSLog(@"NOTIFY event\n");
                break;
           
            case ES_EVENT_TYPE_NOTIFY_FORK:
                process = message->event.fork.child;
                break;
            
            case ES_EVENT_TYPE_NOTIFY_EXIT:
                process = message->process;
                break;
                
            default:
                break;
        }
        
        es_string_token_t path = process->executable->path;
        const char *path_char = path.data;
        const char *google_chrome = "Google Chrome";
        if (strstr(path_char, google_chrome) != NULL) {
            NSLog(@"Process is %s\n", path_char);
            es_auth_result_t result = ES_AUTH_RESULT_DENY;
            es_respond_auth_result(client, message, result, false);
        }
        else {
            es_auth_result_t result = ES_AUTH_RESULT_ALLOW;
            es_respond_auth_result(client, message, result, false);
        }
        
    });
    NSLog(@"Before es_subscribe() call");

    if (ES_NEW_CLIENT_RESULT_SUCCESS != result) {
        NSLog(@"Error: es_new_client() failed with %d", result);
    }
    es_event_type_t events[] = {
        ES_EVENT_TYPE_AUTH_EXEC,
        ES_EVENT_TYPE_NOTIFY_EXEC,
        ES_EVENT_TYPE_NOTIFY_FORK,
        ES_EVENT_TYPE_NOTIFY_EXIT
    };
    if (ES_RETURN_SUCCESS != es_subscribe(endpointClient, events, sizeof(events)/sizeof(events[0]))) {
        NSLog(@"Error: es_subscribe() failed");
    }
    return NSApplicationMain(argc, argv);
}
