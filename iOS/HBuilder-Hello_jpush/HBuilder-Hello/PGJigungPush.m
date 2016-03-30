//
//  PluginTest.m
//  HBuilder-Hello
//
//  Created by Mac Pro on 14-9-3.
//  Copyright (c) 2014年 DCloud. All rights reserved.
//

#import "PDRCore.h"
#import "JPUSHService.h"
#import "PGJigungPush.h"
#import "PDRCommonString.h"

@interface PGJigungPush()
{
    NSString* pUserClientID;
}

@end

@implementation PGJigungPush
@synthesize appKey, appSecret, appID, clientId, ptoken;


- (void)onAppStarted:(NSDictionary*)options
{
    // Override point for customization after application launch.
    if ([[UIDevice currentDevice].systemVersion floatValue] >= 8.0) {
        //可以添加自定义categories
        [JPUSHService registerForRemoteNotificationTypes:(UIUserNotificationTypeBadge |
                                                          UIUserNotificationTypeSound |
                                                          UIUserNotificationTypeAlert)
                                              categories:nil];
    } else {
        //categories 必须为nil
        [JPUSHService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
                                                          UIRemoteNotificationTypeSound |
                                                          UIRemoteNotificationTypeAlert)
                                              categories:nil];
    }

    //启动sdk
    [JPUSHService setupWithOption:options appKey:@"0ee788c4561cae4acb572f40" channel:@"" apsForProduction:NO];
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(jpushDidConnectServer) name:kJPFNetworkDidSetupNotification object:nil];
    [super onAppStarted:options];
}

-(void)jpushDidConnectServer{
    pUserClientID = [JPUSHService registrationID];
}

- (void)onRegRemoteNotificationsError:(NSError *)error {
    [JPUSHService registerDeviceToken:nil];
}

- (void)onRevRemoteNotification:(NSDictionary *)userInfo
{
    [JPUSHService handleRemoteNotification:userInfo];
    [super onRevRemoteNotification:userInfo];
}

- (void)onRevLocationNotification:(UILocalNotification *)userInfo
{
    [JPUSHService showLocalNotificationAtFront:userInfo identifierKey:nil];
    [super onRevLocationNotification:userInfo];
}


- (void)onRevDeviceToken:(NSString *)deviceToken
{
    [JPUSHService registerDeviceToken:[self convertHexStrToData:deviceToken]];
    ptoken = deviceToken;
}

- (NSMutableDictionary*)getClientInfoJSObjcet
{
    NSMutableDictionary* clientInfo = [super getClientInfoJSObjcet];
    
    if (clientInfo != nil) {
        NSString* pAppkey = nil;
        NSURL *pFileUrl = [[NSBundle mainBundle] URLForResource:@"PushConfig" withExtension:@"plist"];
        if (pFileUrl) {
            NSDictionary* pDic = [NSDictionary dictionaryWithContentsOfURL: pFileUrl];
            if (pDic != nil) {
                pAppkey = [pDic  objectForKey:@"APP_KEY"];
            }
        }
        
        [clientInfo setObject:@"" forKey:g_pdr_string_appid];
        [clientInfo setObject:pAppkey ? pAppkey : @"" forKey:g_pdr_string_appkey];
        [clientInfo setObject:pUserClientID ? pUserClientID : @"" forKey:@"clientid"];
        [clientInfo setObject:ptoken ? ptoken : @""  forKey:@"token"];
    }
    return clientInfo;
}

- (NSData *)convertHexStrToData:(NSString *)str {
    if (!str || [str length] == 0) {
        return nil;
    }

    NSMutableData *hexData = [[NSMutableData alloc] initWithCapacity:8];
    NSRange range;
    if ([str length] % 2 == 0) {
        range = NSMakeRange(0, 2);
    } else {
        range = NSMakeRange(0, 1);
    }
    for (NSInteger i = range.location; i < [str length]; i += 2) {
        unsigned int anInt;
        NSString *hexCharStr = [str substringWithRange:range];
        NSScanner *scanner = [[NSScanner alloc] initWithString:hexCharStr];

        [scanner scanHexInt:&anInt];
        NSData *entity = [[NSData alloc] initWithBytes:&anInt length:1];
        [hexData appendData:entity];

        range.location += range.length;
        range.length = 2;
    }

    return hexData;
}

- (void)dealloc
{
    if(appKey)
    {
        [appKey release];
        appKey = nil;
    }
    if(appSecret)
    {
        [appSecret release];
        appSecret = nil;
    }
    
    if(appID)
    {
        [appID release];
        appID = nil;
    }
    if(clientId)
    {
        [clientId release];
        clientId = nil;
    }
    if(ptoken)
    {
        [ptoken release];
        ptoken = nil;
    }
    [super dealloc];
}

@end
