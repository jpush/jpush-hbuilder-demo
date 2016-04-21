//
//  AppDelegate.m
//  Pandora
//
//  Created by Mac Pro_C on 12-12-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"
#import "PDRCore.h"
#import "PDRCommonString.h"

@implementation AppDelegate

@synthesize window = _window;

#pragma mark -
#pragma mark app lifecycle
/*
 * @Summary:程序启动时收到push消息
 */
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // 设置当前SDK运行模式
    BOOL ret = [PDRCore initEngineWihtOptions:launchOptions
                                  withRunMode:PDRCoreRunModeNormal];
    return ret;
}

- (void)application:(UIApplication *)application performActionForShortcutItem:(UIApplicationShortcutItem *)shortcutItem
  completionHandler:(void(^)(BOOL succeeded))completionHandler{
    [PDRCore handleSysEvent:PDRCoreSysEventPeekQuickAction withObject:shortcutItem];
    completionHandler(true);
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)applicationWillResignActive:(UIApplication *)application
{
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [PDRCore handleSysEvent:PDRCoreSysEventEnterBackground withObject:nil];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [PDRCore handleSysEvent:PDRCoreSysEventEnterForeGround withObject:nil];
}


- (void)applicationWillTerminate:(UIApplication *)application
{
    [PDRCore destoryEngine];
}

#pragma mark -
#pragma mark URL

- (BOOL)application:(UIApplication *)application
            openURL:(NSURL *)url
  sourceApplication:(NSString *)sourceApplication
         annotation:(id)annotation {
    [self application:application handleOpenURL:url];
    return YES;
}

/*
 * @Summary:程序被第三方调用，传入参数启动
 *
 */
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    [PDRCore handleSysEvent:PDRCoreSysEventOpenURL withObject:url];
    return YES;
}

#ifdef PDR_PLUS_GETUI
#pragma mark -
#pragma mark APNS
/*
 * @Summary:远程push注册成功收到DeviceToken回调
 *
 */
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    [PDRCore handleSysEvent:PDRCoreSysEventRevDeviceToken withObject:deviceToken];
}

/*
 * @Summary: 远程push注册失败
 */
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    [PDRCore handleSysEvent:PDRCoreSysEventRegRemoteNotificationsError withObject:error];
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    [PDRCore handleSysEvent:PDRCoreSysEventRevRemoteNotification withObject:userInfo];
}

-(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler{
    [PDRCore handleSysEvent:PDRCoreSysEventRevRemoteNotification withObject:userInfo];
    completionHandler(UIBackgroundFetchResultNewData);
}

/*
 * @Summary:程序收到本地消息
 */
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    [PDRCore handleSysEvent:PDRCoreSysEventRevLocalNotification withObject:notification];
}
#endif

- (void)dealloc
{
    [super dealloc];
}

@end
