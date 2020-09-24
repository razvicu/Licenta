//
//  AppsKiller.h
//  Licenta
//
//  Created by Razvan on 6/2/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#ifndef AppsKiller_h
#define AppsKiller_h

#import <Cocoa/Cocoa.h>
#include <cstring>
#include "Communicator.hpp"
#include "Logger.hpp"


class AppsKiller {
public:
    AppsKiller(std::shared_ptr<Communicator> com) : comm_(com) {}
    void start() {
        while(true) {
            apps_ = comm_.get()->getBlockedApplications();
            @autoreleasepool {
                NSMutableArray *nsstrings = [NSMutableArray arrayWithCapacity:10];
                std::for_each(apps_.begin(), apps_.end(), ^(std::string str) {
                    NSString* nsstr = [NSString stringWithUTF8String:str.c_str()];
                    [nsstrings addObject:nsstr];
                });
                NSPredicate *predicate = [NSPredicate predicateWithFormat:@"(self.executableURL.lastPathComponent IN %@)", nsstrings];
                NSArray *running_apps = [[[NSWorkspace sharedWorkspace] runningApplications] filteredArrayUsingPredicate: predicate];
                for (const auto &app_ : apps_) {
                    for (NSRunningApplication *app in running_apps) {
                        if ([[NSString stringWithUTF8String:app_.c_str()] isEqualToString:[app.executableURL lastPathComponent]] ) {
                            std::string app_name = [[app.executableURL absoluteString] UTF8String];
                            std::cout << "Terminating app " << app_name << "\n";
                            // Twitter (and maybe others that are not found yet) takes a long time to terminate
                            // (even 10secs) so we approach it with force so it closes instantly.
                            if ([app terminate] == FALSE || strstr(app_name.c_str(), "Twitter")) {
                                bool res_f = [app forceTerminate];
                                LOG_DBG("Force terminate: %d", res_f);
                            }
                        }
                    }
                }
            }
            sleep(1);
        }
    }
private:
    std::vector<std::string> apps_;
    std::shared_ptr<Communicator> comm_;
};

#endif /* AppsKiller_hpp */
