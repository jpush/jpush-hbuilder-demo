//
//  PDR_Manager_Feature.h
//  Pandora
//
//  Created by Mac Pro_C on 12-12-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, PDRExendPluginType) {
    ///全局插件 该插件native实例将采用单例模式
    PDRExendPluginTypeApp = 0,
    PDRExendPluginTypeFrame = 1,
    /// NView插件 该插件可以使用UI接口进行管理
    PDRExendPluginTypeNView = 2
};

@interface PDRExendPluginInfo : NSObject
@property(nonatomic, copy)NSString *name;
@property(nonatomic, copy)NSString *impClassName;
@property(nonatomic, copy)NSString *javaScript;
@property(nonatomic, assign)PDRExendPluginType type;
+(PDRExendPluginInfo*)infoWithName:(NSString*)name
                      impClassName:(NSString*)impClassName
                              type:(PDRExendPluginType)pluginType
                        javaScript:(NSString*)javasrcipt;
@end

@interface DC5PAppStartParams : NSObject
@property(nonatomic, copy)NSString *version; //app version
@property(nonatomic, copy)NSString *appid;   //app id
@property(nonatomic, copy)NSString *documentPath; //配置的应用文档目录
@property(nonatomic, copy)NSString *rootPath; //应用的运行目录
@property(nonatomic, copy)NSString *arguments; //启动参数
@property(nonatomic, copy)NSString *indexPage; //启动参数
@property(nonatomic, copy)NSString *iconPath; //应用图标
@property(nonatomic, copy)NSString *summary; //应用说明
@property(nonatomic, assign)BOOL streamApp; //应用说明
@property(nonatomic, assign)BOOL debug;
@property(nonatomic, assign)BOOL isSDKApp;
@end

@interface PDRCoreSettings : NSObject
@property(nonatomic, assign)BOOL fullScreen;
@property(nonatomic, assign)UIStatusBarStyle statusBarStyle;
@property(nonatomic, assign)BOOL reserveStatusbarOffset;
@property(nonatomic, copy)NSString *version; //manifest.josn info.plist中的版本号
@property(nonatomic, copy)NSString *innerVersion; //runtime版本号
@property(nonatomic, assign)BOOL debug;      //是否是debug模式
@property(nonatomic, retain)NSArray *apps;   //apps节点
@property(nonatomic, retain)NSString *autoStartdAppid;
@property(nonatomic, retain)NSString *docmentPath;
@property(nonatomic, retain)NSString *downloadPath;
@property(nonatomic, retain)NSString *executableAppsPath;
@property(nonatomic, retain)NSString *workAppsPath;
@property(nonatomic, readonly)NSArray *extendPlugins;
@property(nonatomic, retain)UIColor *statusBarColor;
//加载配置文件
- (void) load;
// info.plist中支持的方向
- (BOOL)configSupportOrientation:(UIInterfaceOrientation)orientation ;
//判断是否支持指定的方向
- (BOOL) supportsOrientation:(UIInterfaceOrientation)orientation;
//判断所有支持的方向
- (NSUInteger)supportedInterfaceOrientations;
//设置支持的方向
- (void) setlockOrientation:(NSUInteger)orientation;
- (void) unlockOrientation;
- (void)setAppid:(NSString*)appid documentPath:(NSString*)doumnetPath;
- (DC5PAppStartParams*)settingWithAppid:(NSString*)appid;
- (void)setupAutoStartdAppid:(NSString *)autoStartdAppid;
- (int)regPluginWithName:(NSString*)pluginName
            impClassName:(NSString*)impClassName
                    type:(PDRExendPluginType)pluginType
              javaScript:(NSString*)javaScript;
@end