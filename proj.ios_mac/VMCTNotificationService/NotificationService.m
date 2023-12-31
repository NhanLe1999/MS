//
//  NotificationService.m
//  VMCTNotificationService
//
//  Created by Apple on 10/17/20.
//

#import "NotificationService.h"

@implementation NotificationService

- (void)didReceiveNotificationRequest:(UNNotificationRequest *)request withContentHandler:(void (^)(UNNotificationContent * _Nonnull))contentHandler { 
    
    self.mediaUrlKey = @"myMediaUrlKey";
    self.mediaTypeKey = @"myMediaTypeKey";
    
    [super didReceiveNotificationRequest:request withContentHandler:contentHandler];
}

@end
