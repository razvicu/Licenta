//
//  AppDelegate.m
//  Licenta
//
//  Created by Razvan on 1/2/20.
//  Copyright © 2020 Razvan. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

NSStatusItem *appStatusItem;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSStatusBar *appStatusBar = [NSStatusBar systemStatusBar];
    appStatusItem = [appStatusBar statusItemWithLength:NSVariableStatusItemLength];
    [appStatusItem.button setImage:[NSImage imageNamed:@"logo.png"]];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (void)startIt:(NSTimer *)timer {
    return;
}



@end
