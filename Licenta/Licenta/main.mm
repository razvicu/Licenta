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
#include <set>

#import "Communicator.hpp"
#import "EndpointSecurityMonitor.h"
#include "Logger.hpp"
#include "AppDelegate.h"
#import "AppsKiller.h"

int main(int argc, const char *argv[]) {
    std::shared_ptr<Communicator> comm = std::make_shared<Communicator>();
    EndpointSecurityMonitor esm(comm);
    AppsKiller killer(comm);
    
    system("echo 0 > /private/opt/licenta/fl");
        
    std::thread communicatorThread(&Communicator::getRestrictionList, comm.get());
    std::thread appsKillerThread(&AppsKiller::start, &killer);
    communicatorThread.detach();
    appsKillerThread.detach();
    
    @autoreleasepool {
        AppDelegate *delegate = [[AppDelegate alloc] init];
         
        //start a timer so that the process does not exit
        NSDate *now = [[NSDate alloc] init];
        NSTimer *timer = [[NSTimer alloc] initWithFireDate:now
        interval:.01
        target:delegate
        selector:@selector(startIt:)
        userInfo:nil
        repeats:YES];
         
        NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
        [runLoop addTimer:timer forMode:NSDefaultRunLoopMode];
        
        NSApplication *application = [NSApplication sharedApplication];
        [application setDelegate:delegate];
        [NSApp run];
    }
    
    return 0;
}
