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

//启动配置
NSString *const kJPushConfig_appkey     = @"APP_KEY";
NSString *const kJPushConfig_channel    = @"CHANNEL";
NSString *const kJPushConfig_production = @"PRODUCTION";

//以下为js中可监听到的事件
NSString *const kJPushReceiveMessage    = @"jpush.receiveMessage";     //收到自定义消息
NSString *const kJPushReceiveAPNS       = @"jpush.receiveNotification";//前台收到推送消息或点击推送消息唤醒app
NSString *const kJPushLaunchWithAPNS    = @"jpush.openNotification"; //点击推送消息启动app

@interface JPushPlugin : PGPush

- (void)onRegRemoteNotificationsError:(NSError *)error;
- (void)onRevDeviceToken:(NSString *)deviceToken;
- (void)onRevRemoteNotification:(NSDictionary *)userInfo;
- (void)onRevLocationNotification:(UILocalNotification *)userInfo;

//以下为js中可调用接口
//设置标签、别名
-(void)setTagsWithAlias:(PGMethod*)command;
-(void)setTags:(PGMethod*)command;
-(void)setAlias:(PGMethod*)command;

//获取 RegistrationID
-(void)getRegistrationID:(PGMethod*)command;

//页面统计
-(void)startLogPageView:(PGMethod*)command;
-(void)stopLogPageView:(PGMethod*)command;
-(void)beginLogPageView:(PGMethod*)command;

//设置角标到服务器,服务器下一次发消息时,会设置成这个值
//本接口不会改变应用本地的角标值.
-(void)setBadge:(PGMethod*)command;
//相当于 [setBadge:0]
-(void)resetBadge:(PGMethod*)command;

//应用本地的角标值设置/获取
-(void)setApplicationIconBadgeNumber:(PGMethod*)command;
-(void)getApplicationIconBadgeNumber:(PGMethod*)command;

//停止与恢复推送
-(void)stopPush:(PGMethod*)command;
-(void)resumePush:(PGMethod*)command;
-(void)isPushStopped:(PGMethod*)command;

//开关日志
-(void)setDebugMode:(PGMethod*)command;
-(void)setLogOFF:(PGMethod*)command;
-(void)crashLogON:(PGMethod*)command;

//本地推送
-(void)setLocalNotification:(PGMethod*)command;
-(void)deleteLocalNotificationWithIdentifierKey:(PGMethod*)command;
-(void)clearAllLocalNotifications:(PGMethod*)command;

//地理位置上报 [latitude,longitude]
-(void)setLocation:(PGMethod*)command;

@end
