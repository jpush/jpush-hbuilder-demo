/*
 *------------------------------------------------------------------
 *  pandora/tools/PDRToolSystemEx.h.h
 *  Description:
 *      获取设备信息头文件
 *  DCloud Confidential Proprietary
 *  Copyright (c) Department of Research and Development/Beijing/DCloud.
 *  All Rights Reserved.
 *
 *  Changelog:
 *	number	author	modify date modify record
 *   0       xty     2013-1-10 创建文件
 *------------------------------------------------------------------
 */
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#define PT_IsAtLeastiOSVersion(X) ([[[UIDevice currentDevice] systemVersion] compare:X options:NSNumericSearch] != NSOrderedAscending)

#define PT_IsIPad() ([[UIDevice currentDevice] respondsToSelector:@selector(userInterfaceIdiom)] && ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad))

/*
 **@获取系统时间
 */
@interface PTDate : NSObject

+(PTDate*)date;

@property(nonatomic, readonly)NSInteger year;
@property(nonatomic, readonly)SInt8 month;
@property(nonatomic, readonly)SInt8 day;
@property(nonatomic, readonly)SInt8 hour;
@property(nonatomic, readonly)SInt8 minute;
@property(nonatomic, readonly)double sencond;
@property(nonatomic, readonly)double milliseconds;
+(NSDate*)dateWithYear:(NSInteger)year month:(NSInteger)month day:(NSInteger)day;
+(NSDate*)dateWithHour:(NSInteger)hour minute:(NSInteger)minute sencond:(NSInteger)sencond;
@end

/*
 **@采集网络的相关信息，域名为plus.device
 */
typedef NS_ENUM(NSInteger, PTNetType) {
    PTNetTypeUnknow = 0,
    PTNetTypeNone,     // none
    PTNetTypeEthernet, // none
    PTNetTypeWIFI,   // wifi
    PTNetTypeCell2G, // 2G
    PTNetTypeCell3G, // 3G
    PTNetTypeCell4G, // 4G
    PTNetTypeWWAN    // 2g/3g
};

@interface PTNetInfo : NSObject
+(PTNetInfo*)info;
@property(nonatomic, readonly)PTNetType netType;
@end

/*
 **@采集手机硬件的相关信息，域名为plus.device
 */
@interface PTDeviceInfo : NSObject

//国际移动设备身份码
@property(nonatomic, retain)NSString *IMEI;
//国际移动用户识别码
@property(nonatomic, retain)NSString *IMSI;
//设备型号
@property(nonatomic, retain)NSString *model;
//生产厂商
@property(nonatomic, retain)NSString *vendor;
@property(nonatomic, retain)NSString *UUID;
+(PTDeviceInfo*)deviceInfo;
+ (NSString*)openUUID;
+ (NSString*)uniqueAppInstanceIdentifier;
@end

typedef NS_ENUM(NSInteger, PTSystemVersion) {
    PTSystemVersion5Series = 0,
    PTSystemVersion6Series,
    PTSystemVersion7Series,
    PTSystemVersion8Series,
    PTSystemVersion9Series,
    PTSystemVersionUnknown
};

typedef NS_ENUM(NSInteger, PTDeviceType) {
    PTDeviceTypeiPhoneSimulator,
    PTDeviceTypeiPhone3G,
    PTDeviceTypeiPhone3GS,
    PTDeviceTypeiPhone4,
    PTDeviceTypeiPhone4s,
    PTDeviceTypeiPhone5,
    PTDeviceTypeiPhone5c,
    PTDeviceTypeiPhone5s,
    PTDeviceTypeiPhone6,
    PTDeviceTypeiPhone6p,
    PTDeviceTypeiPhone6s,
    PTDeviceTypeiPhone6sp,
    PTDeviceTypeiPod3G,
    PTDeviceTypeiPod4G,
    PTDeviceTypeiPod5G,
    PTDeviceTypeiPadAir2,
    PTDeviceTypeiPadAir,
    PTDeviceTypeNewiPad,
    PTDeviceTypeiPad3,
    PTDeviceTypeiPad2,
    PTDeviceTypeiPad1,
    PTDeviceTypeiPadMini3,
    PTDeviceTypeiPadMini2,
    PTDeviceTypeiPadMini1,
    PTDeviceTypeiUnknown
};

/*
 **@采集手机操作系统的相关信息，域名为plus.os
 */
@interface PTDeviceOSInfo : NSObject

//操作系统语言
@property(nonatomic, retain)NSString *language;
//操作系统版本号
@property(nonatomic, retain)NSString *version;
//操作系统名称
@property(nonatomic, retain)NSString *name;
//操作系统提供商
@property(nonatomic, retain)NSString *vendor;

+ (NSString*)getPreferredLanguage;
+ (PTDeviceOSInfo*)osInfo;
+ (PTSystemVersion)systemVersion;
+ (PTDeviceType)deviceType;
+ (BOOL)is7Series;
+ (BOOL)is6Series;
+ (BOOL)is5Series;
@end

/*
 **@采集手机自身屏幕的相关分辨率等信息，域名为plus.screen
 */
@interface PTDeviceScreenInfo : NSObject

//屏幕高度
@property(nonatomic, assign)CGFloat resolutionHeight;
//屏幕宽度
@property(nonatomic, assign)CGFloat resolutionWidth;
//X方向上的密度
@property(nonatomic, assign)CGFloat dpiX;
//Y方向上的密度
@property(nonatomic, assign)CGFloat dpiY;
@property(nonatomic, assign)CGFloat scale;
+(PTDeviceScreenInfo*)screenInfo;

@end

/*
 **@采集手机自身屏幕的相关分辨率等信息，域名为plus.screen
 */
@interface PTDeviceDisplayInfo : NSObject

//应用可用区域
@property (nonatomic, assign)CGRect displayRect;
//应用可用高度
@property(nonatomic, assign)CGFloat resolutionHeight;
//应用可用宽度
@property(nonatomic, assign)CGFloat resolutionWidth;

- (CGRect)displayRect;

+(PTDeviceDisplayInfo*)displayInfo;
+(PTDeviceDisplayInfo*)displayInfoWith:(UIInterfaceOrientation)orientation;
@end


@interface PTDevice : NSObject
{
    PTDeviceInfo *_deviceInfo;
    PTDeviceOSInfo *_osInfo;
    PTDeviceScreenInfo *_screenInfo;
    PTDeviceDisplayInfo *_displayInfo;
    PTNetInfo *_netInfo;
}

+(PTDevice*)sharedDevice;
-(void)update;
@property(nonatomic, retain)PTDeviceInfo *deviceInfo;
@property(nonatomic, retain)PTDeviceOSInfo *osInfo;
@property(nonatomic, retain)PTDeviceScreenInfo *screenInfo;
@property(nonatomic, retain)PTDeviceDisplayInfo *displayInfo;
@property(nonatomic, retain)PTNetInfo *netInfo;

@end

@interface NSString(Measure)
- (BOOL)isAlphaNumeric;
- (int)getMeasure:(CGFloat*)aOutValue withStaff:(CGFloat)aStaff;
@end

@interface UIColor(longColor)
-(NSString*)CSSColor:(BOOL)hasAlpha;
+(UIColor*)colorWithLong:(long)colorValue;
+(UIColor*)colorWithCSS:(NSString*)cssColor;
@end

@interface CAMediaTimingFunction(Util)
+(CAMediaTimingFunction*)curveEnum2Obj:(UIViewAnimationCurve)curve;
@end

//导航图标旋转接口
@interface UIImage(Util)
- (UIImage *)adjustOrientation;
- (UIImage*)adjustOrientationToup;
- (UIImage*)imageRotatedByDegrees:(CGFloat)degrees
                    supportRetina:(BOOL)support
                            scale:(CGFloat)scale;
+ (UIImage*)screenshot:(UIView*)view clipRect:(CGRect)shotRect;
@end

@interface NSString (WBRequest)
- (NSString *)URLEncodedStringEx;
- (NSString *)URLEncodedStringWithCFStringEncodingEx:(CFStringEncoding)encoding;
@end

@interface PTTool : NSObject
+ (BOOL)setSkipBackupAttribute:(BOOL)skip toItemAtURL:(NSURL*)URL;
@end

@interface NSData (AES)
- (NSData *)AESEncryptWithKey:(NSString *)key;
- (NSData *)AESDecryptWithKey:(NSString *)key;
@end

#ifdef __cplusplus
extern "C" {
#endif
int PT_Parse_GetMeasurement( NSObject* aMeasure, CGFloat aStaff, CGFloat * aOutMeasureValue );
#ifdef __cplusplus
}
#endif