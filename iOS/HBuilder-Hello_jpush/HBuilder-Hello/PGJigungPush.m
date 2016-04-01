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

@end

@implementation PGJigungPush

- (void)onAppStarted:(NSDictionary*)options
{
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
    [JPUSHService setupWithOption:options appKey:@"abcacdf406411fa656ee11c3" channel:@"" apsForProduction:NO];
    [JPUSHService setDebugMode];
    [super onAppStarted:options];
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

#pragma mark - jpush methods

//-(void)setTagsWithAlias:(PGMethod*)command;
//-(void)setTags:(PGMethod*)command;
//-(void)setAlias:(PGMethod*)command;

-(void)getRegistrationID:(PGMethod*)command{
    if (command) {
        NSString *rid = [JPUSHService registrationID];
        [self handleResultWithValue:rid andCommand:command];
    }
}

-(void)handleResultWithValue:(id)value andCommand:(PGMethod*)command{

    PDRPluginResult *result = nil;
    PDRCommandStatus status = PDRCommandStatusOK;

    if ([value isKindOfClass:[NSString class]]) {
        value  = [value stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        result = [PDRPluginResult resultWithStatus:status messageAsString:value];
    }else if ([value isKindOfClass:[NSArray class]]) {
        result = [PDRPluginResult resultWithStatus:status messageAsArray:value];
    }else if ([value isKindOfClass:[NSDictionary class]]){
        result = [PDRPluginResult resultWithStatus:status messageAsDictionary:value];
    }else if ([value isKindOfClass:[NSNull class]]){
        result = [PDRPluginResult resultWithStatus:status];
    }else if ([value isKindOfClass:[NSNumber class]]){
        CFNumberType numberType = CFNumberGetType((CFNumberRef)value);
        if (numberType == kCFNumberIntType) {
            result = [PDRPluginResult resultWithStatus:status messageAsInt:[value intValue]];
        } else  {
            result = [PDRPluginResult resultWithStatus:status messageAsDouble:[value doubleValue]];
        }
    }else{
        NSString *error = [NSString stringWithFormat:@"unrecognized type: %@",NSStringFromClass([value class])];
        NSLog(@"%@",error);
        result = [PDRPluginResult resultWithStatus:PDRCommandStatusError messageAsString:error];
    }

    [self toCallback:command.arguments[0] withReslut:[result toJSONString]];
}

//-(void)startLogPageView:(PGMethod*)command;
//-(void)stopLogPageView:(PGMethod*)command;
//-(void)beginLogPageView:(PGMethod*)command;
//
//
//
//-(void)setBadge:(PGMethod*)command;
//
//-(void)resetBadge:(PGMethod*)command;
//
//
//-(void)setApplicationIconBadgeNumber:(PGMethod*)command;
//-(void)getApplicationIconBadgeNumber:(PGMethod*)command;
//
//
//-(void)stopPush:(PGMethod*)command;
//-(void)resumePush:(PGMethod*)command;
//-(void)isPushStopped:(PGMethod*)command;
//
//
//-(void)setDebugModeFromIos:(PGMethod*)command;
//-(void)setLogOFF:(PGMethod*)command;
//-(void)crashLogON:(PGMethod*)command;
//
//
//-(void)setLocalNotification:(PGMethod*)command;
//-(void)deleteLocalNotificationWithIdentifierKey:(PGMethod*)command;
//-(void)clearAllLocalNotifications:(PGMethod*)command;
//
//
//-(void)setLocation:(PGMethod*)command;


@end
