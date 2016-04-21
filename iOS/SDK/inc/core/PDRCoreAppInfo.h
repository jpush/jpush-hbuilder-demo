//
//  PDR_Application.h
//  Pandora
//
//  Created by Mac Pro on 12-12-22.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, PDRCoreAppPopGesture) {
    PDRCoreAppPopGestureNone = 0,
    PDRCoreAppPopGestureClose,
    PDRCoreAppPopGestureHide,
    PDRCoreAppPopGestureSkip
};

@interface PDRCoreAppUserAgentInfo : NSObject
@property(nonatomic, copy)NSString *value;
@property(nonatomic, assign)BOOL concatenate;
@end

@interface PDRCoreAppAuthorInfo : NSObject
@property(nonatomic, copy)NSString *email;
@property(nonatomic, copy)NSString *href;
@property(nonatomic, copy)NSString *content;
@end

@interface PDRCoreAppLicenseInfo : NSObject
@property(nonatomic, copy)NSString *href;
@property(nonatomic, copy)NSString *content;
@end

@interface PDRCoreAppCongfigParse : NSObject {
@private
    // 应用权限目录
    NSMutableArray *_permission;
}
@property(nonatomic, retain)NSArray *permission;
// 应用的主页面
@property(nonatomic, copy)NSString *appID;
@property(nonatomic, assign)BOOL copyToReadWriteDir;
@property(nonatomic, copy)NSString *name;
@property(nonatomic, copy)NSString *version;
@property(nonatomic, assign)BOOL fullScreen;
@property(nonatomic, assign)BOOL allowsInlineMediaPlayback;
@property(nonatomic, assign)BOOL showSplashscreen;
@property(nonatomic, assign)BOOL autoClose;
@property(nonatomic, assign)BOOL splashShowWaiting;
@property(nonatomic, assign)NSUInteger delay;
@property(nonatomic, copy)NSString *contentSrc;
@property(nonatomic, copy)NSString *description;
@property(nonatomic, retain)PDRCoreAppAuthorInfo *author;
@property(nonatomic, retain)PDRCoreAppLicenseInfo *license;
@property(nonatomic, retain)PDRCoreAppUserAgentInfo *userAgent;
@property(nonatomic, retain)NSString *adaptationSrc;
@property(nonatomic, copy)NSString *errorHtmlPath;
@property(nonatomic, assign)PDRCoreAppPopGesture popGesture;
@property(nonatomic, assign)BOOL isStreamCompetent;


@property(nonatomic, copy)NSString *configPath;   //配置文件全路径

- (int) load;
- (int) loadWithConfig:(NSString*)fullPath;
- (BOOL)lessThanPermission:(NSArray*)permission moreModes:(NSArray**)output;
@end

@interface PDRCoreAppInfo : PDRCoreAppCongfigParse {
    @private
}
@property(nonatomic, copy)NSString *indexPage;
@property(nonatomic, copy)NSString *wwwPath;
@property(nonatomic, copy)NSString *documentPath;
@property(nonatomic, copy)NSString *tmpPath;
@property(nonatomic, copy)NSString *logPath;
@property(nonatomic, copy)NSString *dataPath;
@property(nonatomic, retain)NSString *adaptationJS;

- (void)changeExecutableRootPath:(NSString *)executableRootPath;
- (void)setPathsWithExecutableRootPath:(NSString *)executableRootPath
                              workPath:(NSString *)workRootPath
                              docuPath:(NSString *)documentPath;
- (void)loadAdaptationJS;
- (void)webviewDefaultSetting;

- (void)registerForKVO;
- (void)unregisterFromKVO;
@end