//
//  PGPush.h
//  Pandora
//
//  Created by Pro_C Mac on 13-3-12.
//
//

#import "PGPlugin.h"
#import "PGMethod.h"

@interface PGPush : PGPlugin
@property (assign, nonatomic) BOOL handleOfflineMsg;
- (void)clear:(PGMethod*)pMethod;
- (void)registerForRemoteNotificationTypes;
- (void)addEventListener:(PGMethod*)pMethod;
- (void)createMessage:(PGMethod*)pMethod;
- ( NSData* )getClientInfo:(PGMethod*)pMethod;
- (NSMutableDictionary*)getClientInfoJSObjcet;

- (void) onAppStarted:(NSDictionary*)options;
- (void) onRevRemoteNotification:(NSDictionary *)userInfo isReceive:(BOOL)isReceive;
- (void) onRevRemoteNotification:(NSDictionary *)userInfo;
- (void) onRevLocationNotification:(UILocalNotification *)userInfo;
- (void) onAppEnterBackground;
- (void) onAppEnterForeground;
@end
