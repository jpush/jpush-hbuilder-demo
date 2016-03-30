//
//  PluginTest.h
//  HBuilder-Hello
//
//  Created by Mac Pro on 14-9-3.
//  Copyright (c) 2014年 DCloud. All rights reserved.
//

#include "PGPlugin.h"
#include "PGMethod.h"
#include "PGPush.h"
#import <Foundation/Foundation.h>



@interface PGJigungPush : PGPush{
    
}

@property (retain, nonatomic) NSString *appKey;
@property (retain, nonatomic) NSString *appSecret;
@property (retain, nonatomic) NSString *appID;
@property (retain, nonatomic) NSString *clientId;
@property (retain, nonatomic) NSString *ptoken;

- (NSMutableDictionary*)getClientInfoJSObjcet;
- (void)onRegRemoteNotificationsError:(NSError *)error;
- (void)onRevDeviceToken:(NSString *)deviceToken;
- (void)onRevRemoteNotification:(NSDictionary *)userInfo;
- (void)onRevLocationNotification:(UILocalNotification *)userInfo;



@end
