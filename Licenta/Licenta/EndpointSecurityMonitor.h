//
//  EndpointSecurityF.hpp
//  Licenta
//
//  Created by Razvan on 5/11/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef EndpointSecurityMonitor_hpp
#define EndpointSecurityMonitor_hpp

#include <iostream>
#include <string>
#include <vector>
#import <EndpointSecurity/EndpointSecurity.h>
#import <bsm/libbsm.h>

#include "Communicator.hpp"
#include "Logger.hpp"

class EndpointSecurityMonitor {
public:
    EndpointSecurityMonitor(std::shared_ptr<Communicator>);
    ~EndpointSecurityMonitor();
private:
    void initializeEsClient();
    void uninitializeEsClient();
    void subscribeToEventsOfInterest();
    
    es_client_t *es_client_;
    std::vector<std::string> apps_;
    std::shared_ptr<Communicator> comm_;
};

#endif /* EndpointSecurityF_hpp */
