//
//  PluginTest.m
//  HBuilder-Hello
//
//  Created by Mac Pro on 14-9-3.
//  Copyright (c) 2014年 DCloud. All rights reserved.
//

#import "PDRCore.h"
#import "JPUSHService.h"
#import "JPushPlugin.h"
#import "PDRCommonString.h"
#import "PDRCoreAppFrame.h"
#import <AdSupport/AdSupport.h>

#ifdef NSFoundationVersionNumber_iOS_9_x_Max
#import <UserNotifications/UserNotifications.h>
#endif

//启动配置
NSString *const kJPushConfig_appkey     = @"APP_KEY";
NSString *const kJPushConfig_channel    = @"CHANNEL";
NSString *const kJPushConfig_production = @"PRODUCTION";
NSString *const kJPushConfig_idfa = @"IDFA";

//以下为js中可监听到的事件
NSString *const kJPushReceiveMessage    = @"plus.Push.receiveMessageIniOSCallback";         //收到自定义消息
NSString *const kJPushReceiveAPNS       = @"plus.Push.receiveNotificationIniOSCallback";    //前台收到推送消息
NSString *const kJPushOpenNotification  = @"plus.Push.openNotificationIniOSCallback";       //前台点击推送消息
NSString *const kJPushReceiveLaunch     = @"plus.Push.receiveNotificationLaunceAppIniOSCallback";       //点击推送消息启动或唤醒app

NSString *const kJPushReceiveBackground = @"plus.Push.receiveNotificationBackgroundIniOSCallback"; //后台收到推送
NSString *const kJPushOnRegistrationId = @"plus.Push.onGetRegistrationId";

@interface JPushPlugin()

@property(nonatomic, strong)PGMethod *aliasAndTagsCommand;
@property(nonatomic, strong)NSDictionary *cacheLaunchNotification;
@end

@implementation JPushPlugin

#pragma mark - HBuilder listener

- (void)onAppStarted:(NSDictionary*)options
{
    _cacheLaunchNotification = options;
    [JPUSHService registrationIDCompletionHandler:^(int resCode, NSString *registrationID) {
      NSString *rid = registrationID?:@"";
      [self fireEvent: kJPushOnRegistrationId args: rid];
    }];
  
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(NSEC_PER_USEC)), dispatch_get_main_queue(), ^{
      JPUSHRegisterEntity * entity = [[JPUSHRegisterEntity alloc] init];
      entity.types = JPAuthorizationOptionAlert|JPAuthorizationOptionBadge|JPAuthorizationOptionSound;
      if ([[UIDevice currentDevice].systemVersion floatValue] >= 8.0) {
      }
      
      [JPUSHService registerForRemoteNotificationConfig:entity delegate:self];
    });
  
    NSString *path = [[NSBundle mainBundle]pathForResource:@"PushConfig" ofType:@"plist"];
    NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:path];

    NSString *advertisingId = nil;
    if([dict[kJPushConfig_idfa] boolValue]){
        advertisingId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    }
    [JPUSHService setupWithOption:options
                           appKey:dict[kJPushConfig_appkey]
                          channel:dict[kJPushConfig_channel]
                 apsForProduction:[dict[kJPushConfig_production] boolValue]
            advertisingIdentifier:advertisingId];

    [JPUSHService setDebugMode];
    
    [super onAppStarted:options];

    if (options) {
        NSDictionary *userInfo = [options valueForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
        if ([userInfo count]) {
            [self fireEvent:kJPushReceiveLaunch args:userInfo];
        }
    }

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(networkDidReceiveMessage:) name:kJPFNetworkDidReceiveMessageNotification object:nil];
}

- (void)onRegRemoteNotificationsError:(NSError *)error {
    [JPUSHService registerDeviceToken:nil];
}

- (void)onRevRemoteNotification:(NSDictionary *)userInfo
{
  NSDictionary *userinforet;
  if ([userInfo isKindOfClass:[NSNotification class]]){
    NSNotification *noti = (NSNotification *)userInfo;
    userinforet = (NSDictionary *)noti.object;
  } else if ([userInfo isKindOfClass: [NSDictionary class]]){
    userinforet = userInfo;
  }
  
  if (!userinforet){
    return;
  }
  
  [JPUSHService handleRemoteNotification:userinforet];
  [super onRevRemoteNotification:userInfo];
  switch ([UIApplication sharedApplication].applicationState) {
    case UIApplicationStateActive:
      [self fireEvent:kJPushReceiveAPNS args:userinforet];
      break;
    case UIApplicationStateInactive:
      [self fireEvent:kJPushReceiveLaunch args:userinforet];
      break;
    case UIApplicationStateBackground:
      [self fireEvent:kJPushReceiveBackground args:userinforet];
      break;
    default:
      break;
  }
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

#pragma mark - jpush public methods

-(void)setTagsWithAlias:(PGMethod*)command{
    self.aliasAndTagsCommand = command;
    NSString *alias = command.arguments[1];
    NSArray  *tags  = command.arguments[2];

    NSLog(@"#### setTagsWithAlias alias is %@, tags is %@",alias,tags);

    [JPUSHService setTags:[NSSet setWithArray:tags]
                    alias:alias
         callbackSelector:@selector(tagsWithAliasCallback:tags:alias:)
                   object:self];
}

-(void)setTags:(PGMethod*)command{
    self.aliasAndTagsCommand = command;

    NSArray *tags = command.arguments[1];

    NSLog(@"#### setTags %@",tags);

    [JPUSHService setTags:[NSSet setWithArray:tags]
         callbackSelector:@selector(tagsWithAliasCallback:tags:alias:)
                   object:self];
}

-(void)setAlias:(PGMethod*)command{
    self.aliasAndTagsCommand = command;
    NSLog(@"#### setAlias %@",command.arguments);
    [JPUSHService setAlias:command.arguments[1]
          callbackSelector:@selector(tagsWithAliasCallback:tags:alias:)
                    object:self];
}

-(void)tagsWithAliasCallback:(int)resultCode tags:(NSSet *)tags alias:(NSString *)alias{
    NSDictionary *dict = @{@"resultCode":[NSNumber numberWithInt:resultCode],
                           @"tags"      :tags  == nil ? [NSNull null] : [tags allObjects],
                           @"alias"     :alias == nil ? [NSNull null] : alias
                           };
    NSError  *error;
    NSData   *jsonData   = [NSJSONSerialization dataWithJSONObject:dict options:0 error:&error];
    NSString *jsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];

    [self handleResultWithValue:jsonString command:self.aliasAndTagsCommand];
}

-(void)getRegistrationID:(PGMethod*)command{
    if (command) {
        NSString *rid = [JPUSHService registrationID];
        [self handleResultWithValue:rid command:command];
    }
}

-(void)getLaunchAppCacheNotification:(PGMethod*)command{
    if (command) {
        
        if (_cacheLaunchNotification != nil) {
            
            if ([_cacheLaunchNotification valueForKey:UIApplicationLaunchOptionsRemoteNotificationKey]) {
                NSMutableDictionary *dic = [[NSMutableDictionary alloc] initWithDictionary:[_cacheLaunchNotification valueForKey:UIApplicationLaunchOptionsRemoteNotificationKey]];
                [self handleResultWithValue:dic command:command];
                return;
            }
            
            if ([_cacheLaunchNotification valueForKey:UIApplicationLaunchOptionsLocalNotificationKey]) {
                UILocalNotification *localNotification = [_cacheLaunchNotification valueForKey:UIApplicationLaunchOptionsLocalNotificationKey];
                NSMutableDictionary *localNotificationEvent = @{}.mutableCopy;
                localNotificationEvent[@"content"] = localNotification.alertBody;
                localNotificationEvent[@"badge"] = @(localNotification.applicationIconBadgeNumber);
                localNotificationEvent[@"extras"] = localNotification.userInfo;
                [self handleResultWithValue:localNotificationEvent command:command];
                return;
            }
            
            [self handleResultWithValue:@{} command:command];
        }
    }
}


-(void)handleResultWithValue:(id)value command:(PGMethod*)command{

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

-(void)startLogPageView:(PGMethod*)command{
    [JPUSHService startLogPageView:(NSString*)command.arguments[1]];
}

-(void)stopLogPageView:(PGMethod*)command{
    [JPUSHService stopLogPageView:(NSString*)command.arguments[1]];
}

-(void)beginLogPageView:(PGMethod*)command{
    [JPUSHService stopLogPageView:(NSString*)command.arguments[1]];
}

-(void)setBadge:(PGMethod*)command{
    [JPUSHService setBadge:((NSNumber*)command.arguments[1]).integerValue];
}

-(void)resetBadge:(PGMethod*)command{
    [JPUSHService resetBadge];
}

-(void)setApplicationIconBadgeNumber:(PGMethod*)command {
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:((NSString*)command.arguments[1]).integerValue];
}

-(void)getApplicationIconBadgeNumber:(PGMethod*)command{
    NSInteger badge = [UIApplication sharedApplication].applicationIconBadgeNumber;
    NSNumber *number = [NSNumber numberWithInteger:badge];
    [self handleResultWithValue:number command:command];
}

-(void)stopPush:(PDRPluginResult*)command{
    [[UIApplication sharedApplication]unregisterForRemoteNotifications];
}

-(void)resumePush:(PDRPluginResult*)command{
    JPUSHRegisterEntity * entity = [[JPUSHRegisterEntity alloc] init];
    entity.types = JPAuthorizationOptionAlert|JPAuthorizationOptionBadge|JPAuthorizationOptionSound;
    if ([[UIDevice currentDevice].systemVersion floatValue] >= 8.0) {
    }
    
    [JPUSHService registerForRemoteNotificationConfig:entity delegate:self];
}

-(void)isPushStopped:(PGMethod*)command{
    NSNumber *value;
    if ([[UIApplication sharedApplication] isRegisteredForRemoteNotifications]) {
        value = @(0);
    }else{
        value = @(1);
    }
    [self handleResultWithValue:value command:command];
}

-(void)setDebugMode:(PGMethod*)command{
    [JPUSHService setDebugMode];
}

-(void)setLogOFF:(PGMethod*)command{
    [JPUSHService setLogOFF];
}

-(void)crashLogON:(PGMethod*)command{
    [JPUSHService crashLogON];
}

-(void)setLocalNotification:(PGMethod*)command{
    NSArray      *arguments = command.arguments;
    NSDate       *date      = arguments[1] == [NSNull null] ? nil : [NSDate dateWithTimeIntervalSinceNow:[((NSString*)arguments[1]) intValue]];
    NSString     *alertBody = arguments[2] == [NSNull null] ? nil : (NSString*)arguments[2];
    int           badge     = arguments[3] == [NSNull null] ? 0   : [(NSString*)arguments[3] intValue];
    NSString     *idKey     = arguments[4] == [NSNull null] ? nil : (NSString*)arguments[4];
    NSDictionary *dict      = arguments[5] == [NSNull null] ? nil : (NSDictionary*)arguments[5];
    [JPUSHService setLocalNotification:date alertBody:alertBody badge:badge alertAction:nil identifierKey:idKey userInfo:dict soundName:nil];
}

-(void)deleteLocalNotificationWithIdentifierKey:(PGMethod*)command{
    [JPUSHService deleteLocalNotificationWithIdentifierKey:(NSString*)command.arguments[1]];
}

-(void)clearAllLocalNotifications:(PGMethod*)command{
    [JPUSHService clearAllLocalNotifications];
}

-(void)setLocation:(PGMethod*)command{
    [JPUSHService setLatitude:[(NSString*)command.arguments[1] doubleValue] longitude:[(NSString*)command.arguments[1] doubleValue]];
}

#pragma mark - jpush privates

-(void)networkDidReceiveMessage:(NSNotification*)notification{
    if (notification) {
        [self fireEvent:kJPushReceiveMessage args:[notification userInfo]];
    }
}

-(void)setAliasAndTagsCommand:(PGMethod *)aliasAndTagsCommand{
    if (!_aliasAndTagsCommand) {
        _aliasAndTagsCommand = [[PGMethod alloc]init];
    }
    _aliasAndTagsCommand.htmlID       = [aliasAndTagsCommand.htmlID copy];
    _aliasAndTagsCommand.featureName  = [aliasAndTagsCommand.featureName copy];
    _aliasAndTagsCommand.functionName = [aliasAndTagsCommand.functionName copy];
    _aliasAndTagsCommand.callBackID   = [aliasAndTagsCommand.callBackID copy];
    _aliasAndTagsCommand.sid          = [aliasAndTagsCommand.sid copy];
    _aliasAndTagsCommand.arguments    = [aliasAndTagsCommand.arguments copy];
}

-(void)fireEvent:(NSString*)event args:(id)args{
    NSString *evalString = nil;
    NSError  *error      = nil;
    NSString *argsString = nil;

    if (args) {
        if ([args isKindOfClass:[NSString class]]) {
          argsString = [NSString stringWithFormat:@"'%@'", args];
        }else{
            NSData   *jsonData   = [NSJSONSerialization dataWithJSONObject:args options:0 error:&error];
            argsString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            if (error) {
                NSLog(@"%@",error);
            }
        }
      evalString = [NSString stringWithFormat:@"%@(%@)", event, argsString];
    }else{
      evalString = [NSString stringWithFormat:@"%@({})", event];
    }
    [self evaluatingJavaScriptFromString:evalString];
}

-(void)evaluatingJavaScriptFromString:(NSString*)string{
  dispatch_async(dispatch_get_main_queue(), ^{
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    NSArray *views = [[[window rootViewController] view] subviews];
    NSArray *frames = [self searchViews:views];
  
      for (PDRCoreAppFrame *appFrame in frames) {
        [appFrame stringByEvaluatingJavaScriptFromString:string];
      }
    });
}

-(NSMutableArray*)searchViews:(NSArray*)views{
    NSMutableArray *frames = [NSMutableArray array];
    for (UIView *temp in views) {
        if ([temp isMemberOfClass:[PDRCoreAppFrame class]]) {
            [frames addObject:temp];
        }
        if ([temp subviews]) {
            NSMutableArray *tempArray = [self searchViews:[temp subviews]];
            for (UIView *tempView in tempArray) {
                if ([tempView isMemberOfClass:[PDRCoreAppFrame class]]) {
                    [frames addObject:tempView];
                }
            }
        }
    }
    return frames;
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

- (void)jpushNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(NSInteger))completionHandler {
  NSDictionary * userInfo = notification.request.content.userInfo;
  if([notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
    [self fireEvent:kJPushReceiveAPNS args:userInfo];
    [JPUSHService handleRemoteNotification:userInfo];
  }
  completionHandler(UNNotificationPresentationOptionAlert);
  
}

- (void)jpushNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler {
  NSDictionary * userInfo = response.notification.request.content.userInfo;
  if([response.notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
    
    [self fireEvent:kJPushOpenNotification args:userInfo];
    [JPUSHService handleRemoteNotification:userInfo];
  }
  completionHandler();
}

@end
