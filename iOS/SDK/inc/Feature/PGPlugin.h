
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define PDRPluginHandleOpenURLNotification	@"PDRPluginHandleOpenURLNotification"

typedef enum {
    PDRCommandStatusNoResult = 0,
	PDRCommandStatusOK,
	PDRCommandStatusError
} PDRCommandStatus;

@class PDRCoreApp;
@class PDRCoreAppFrame;

/** Native执行结果
 */
@interface PDRPluginResult : NSObject {
    
}

@property (nonatomic, strong, readonly) NSNumber* status;
@property (nonatomic, strong, readonly) id message;
@property (nonatomic, assign) BOOL keepCallback;

-(PDRPluginResult*) init;
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal;
/**
 @brief 返回JS字符串
 @param statusOrdinal 结果码
 @param theMessage 字符串结果
 @return PDRPluginResult*
 */
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal messageAsString: (NSString*) theMessage;
/**
 @brief 返回JS数组
 @param statusOrdinal 结果码
 @param theMessage 字符串结果
 @return PDRPluginResult*
 */
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal messageAsArray: (NSArray*) theMessage;
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal messageAsInt: (int) theMessage;
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal messageAsDouble: (double) theMessage;
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal messageAsDictionary: (NSDictionary*) theMessage;
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal messageToErrorObject: (int) errorCode;
/**
 @brief 返回错误对象
 @param statusOrdinal 结果码
 @param errorCode 错误码
 @param message 错误描述
 @return PDRPluginResult*
 */
+(PDRPluginResult*) resultWithStatus: (PDRCommandStatus) statusOrdinal
                messageToErrorObject: (int) errorCode
                         withMessage:(NSString*)message;
+(PDRPluginResult*) resultWithInnerError:(int) errorCode
                             withMessage:(NSString*)message;
/**
 @brief 返回JSON格式的结果
 @return NSString*
 */
-(NSString*) toJSONString;

@end

enum {
    PGPluginErrorInner = -100,
    PGPluginErrorUnknown = -99,
    PGPluginErrorAuthDenied = -10,
    PGPluginErrorNoInstall = -8,
    PGPluginErrorConfig = -7,
    PGPluginErrorNet = -6,
    PGPluginErrorIO = -5,
    PGPluginErrorFileNotFound = -4,
    PGPluginErrorNotSupport = -3,
    PGPluginErrorUserCancel = -2,
    PGPluginErrorInvalidArgument = -1,
    PGPluginOK = 0,
    PGPluginErrorFileExist,
    PGPluginErrorFileCreateFail,
    PGPluginErrorZipFail,
    PGPluginErrorUnZipFail,
    PGPluginErrorNotAllowWrite,
    PGPluginErrorNotAllowRead,
    PGPluginErrorBusy,
    PGPluginErrorNotPermission,
    PGPluginErrorNext
};

@protocol PGPlugin <NSObject>
- (void) onAppStarted:(NSDictionary*)options;
//事件通知类
- (void) onDestroy;
- (void) onAppUpgradesNoClose;
- (void) onNeedLayout;
- (void) onAppFrameWillClose:(PDRCoreAppFrame*)theAppframe;
- (void) onAppTerminate;
- (void) onMemoryWarning;
- (void) onAppEnterBackground;
- (void) onAppEnterForeground;
// 需要启动aps消息才会接受到
- (void) onRegRemoteNotificationsError:(NSError *)error;
- (void) onRevDeviceToken:(NSString *)deviceToken;
- (void) onRevRemoteNotification:(NSDictionary *)userInfo;
- (void) onRevLocationNotification:(UILocalNotification *)userInfo;
@end

typedef NS_ENUM(NSInteger, PGPluginAuthorizeStatus) {
    PGPluginAuthorizeStatusNotDetermined,
    PGPluginAuthorizeStatusDenied,
    PGPluginAuthorizeStatusRestriction,
    PGPluginAuthorizeStatusAuthorized
};

@protocol PGPluginAuthorize <NSObject>
- (PGPluginAuthorizeStatus)authorizeStatus;
@end

/** PDR插件基类
 扩展插件都应该从该类继承
 */
@interface PGPlugin : NSObject<PGPluginAuthorize>

/// 插件运行的窗口对象 <br/>参考: `PDRCoreAppFrame`
@property (nonatomic, assign) PDRCoreAppFrame* JSFrameContext;
/// 插件运行的应用对象 <br/>参考: `PDRCoreApp`
@property (nonatomic, assign) PDRCoreApp* appContext;

/// 插件错误码参考地址
@property (nonatomic, copy) NSString *errorURL;
@property (nonatomic, copy) NSString *sdkErrorURL;
/// 插件名字
@property (nonatomic, copy) NSString *name;
/// 插件描述
@property (nonatomic, copy) NSString *content;

- (PGPlugin*) initWithWebView:(PDRCoreAppFrame*)theWebView withAppContxt:(PDRCoreApp*)app;
- (UIViewController*) rootViewController;
- (void)presentViewController:(UIViewController *)viewControllerToPresent animated: (BOOL)flag completion:(void (^)(void))completion;
- (void)dismissViewControllerAnimated:(BOOL)flag completion:(void (^)(void))completion;
- (void) enableRevAps;
- (void) disableRevAps;
- (void) handleOpenURL:(NSNotification*)notification;
- (void) onAppFrameWillClose:(PDRCoreAppFrame*)theAppframe;
- (void) onAppClose;
- (NSString*)errorMsgWithCode:(int)errorCode;
//同步执行时放回int值
- (NSData*) resultWithInt:(int)value;
- (NSData*) resultWithString:(NSString*)value;
- (NSData*) resultWithBool:(BOOL)value;
- (NSData*) resultWithDouble:(double)value;
- (NSData*) resultWithJSON:(NSDictionary*)dict;
- (NSData*) resultWithArray:(NSArray*)array;
- (NSData*) resultWithNull;
- (NSData*) resultWithNaN;
/**
 @brief 同步调用JavaScript回调函数  参考:`toCallback:withReslut:`
 */
-(void) toSyncCallback: (NSString*) callbackId withReslut:(NSString*)message;
/**
 @brief 异步调用JavaScript回调函数
 @param callbackId 回调ID
 @param message JSON格式结果 参考:`toJSONString`
 @return void
 */
-(void) toCallback: (NSString*) callbackId withReslut:(NSString*)message;
///调用JS层错误回调
-(void) toErrorCallback: (NSString*) callbackId withCode:(int)errorCode;
-(void) toErrorCallback: (NSString*) callbackId withCode:(int)errorCode  withMessage:(NSString*)message;
-(void) toErrorCallback: (NSString*) callbackId withCode:(int)errorCode  withMessage:(NSString*)message keepCallback:(BOOL)keepCallback;
-(void) toErrorCallback: (NSString*) callbackId withInnerCode:(int)errorCode withMessage:(NSString*)message;
-(void) toErrorCallback: (NSString*) callbackId withInnerCode:(int)errorCode withMessage:(NSString*)message keepCallback:(BOOL)keepCallback;

///将OC NSError转化为H5+Error
-(void) toErrorCallback: (NSString*) callbackId
            withNSError:(NSError*)error;
///封装插件使用的SDK产生的错误信息
///code：-100
///message：[self.name self.contexnt:errorCode]message，self.sdkErrorURL
-(void) toErrorCallback: (NSString*) callbackId
           withSDKError:(int)errorCode
            withMessage:(NSString*)message;
-(void) toErrorCallback: (NSString*) callbackId
         withMoudleName:(NSString*)moudleName
               withCode:(int)errorCode
            withMessage:(NSString*)message withURL:(NSString*)url;

-(void) toSucessCallback: (NSString*) callbackId withInt:(int)intValue;
-(void) toSucessCallback: (NSString*) callbackId withInt:(int)errorCode keepCallback:(BOOL)keepCallback;
-(void) toSucessCallback: (NSString*) callbackId withDouble:(double)doubleValue;
-(void) toSucessCallback: (NSString*) callbackId withDouble:(double)doubleValue keepCallback:(BOOL)keepCallback;
-(void) toSucessCallback: (NSString*) callbackId withString:(NSString*)stringValue;
-(void) toSucessCallback: (NSString*) callbackId withString:(NSString*)stringValue keepCallback:(BOOL)keepCallback;
-(void) toSucessCallback: (NSString*) callbackId withJSON:(NSDictionary*)jsonValue;
-(void) toSucessCallback: (NSString*) callbackId withJSON:(NSDictionary*)jsonValue keepCallback:(BOOL)keepCallback;
-(void) toSucessCallback: (NSString*) callbackId withArray:(NSArray*)arrayValue;
-(void) toSucessCallback: (NSString*) callbackId withArray:(NSArray*)arrayValue keepCallback:(BOOL)keepCallback;
- (NSString*) writeJavascript:(NSString*)javascript;
- (NSString*) asyncWriteJavascript:(NSString*)javascript;
- (PGPluginAuthorizeStatus)authorizeStatus;
@end

@interface PGPluginParamHelper : NSObject
+(BOOL)getBoolValue:(id)jsValue defalut:(BOOL)defalutValue;
+(int)getIntValue:(id)jsValue defalut:(int)defalutValue;
+(int)getIntValueInDict:(NSDictionary*)jsValue
                 forKey:(NSString*)key
                defalut:(int)defalutValue;
+(float)getFloatValue:(id)jsValue defalut:(float)defalutValue;
+(float)getFloatValueInDict:(NSDictionary*)jsValue
                     forKey:(NSString*)key;
+(float)getFloatValueInDict:(NSDictionary*)jsValue
                     forKey:(NSString*)key defalut:(float)defalutValue;
///String
+(BOOL)isEmptyString:(NSString*)jsValue;
+(NSString*)getStringValue:(id)jsValue;
+(NSString*)getStringValue:(id)jsValue defalut:(NSString*)defalutValue;
+(NSString*)getStringValueInDict:(NSDictionary*)jsValue
                          forKey:(NSString*)key;
+(NSString*)getStringValueInDict:(NSDictionary*)jsValue
                     forKey:(NSString*)key defalut:(NSString*)defalutValue;
+(NSDictionary*)getJSONValue:(id)jsValue defalut:(NSDictionary*)defalutValue;
+(NSDictionary*)getJSONValueInDict:(NSDictionary*)jsValue
                            forKey:(NSString*)key;

+(NSString*)testString:(NSString*)jsValue
               inRange:(NSArray*)ranges
               defalut:(NSString*)defalutValue;
@end

