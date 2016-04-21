//
//  PDRCore.h
//  Pandora Project
//
//  Created by Mac on 12-12-25.
//  Copyright (c) 2012年 DCloud. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>

#import "PDRCoreDefs.h"
#import "PDRCoreSettings.h"

@class PDRCoreWindowManager;
@class PDRCoreAppManager;

/// H5+代理类用于和宿主环境沟通
@protocol PDRCoreDelegate <NSObject>
@optional
/// @brief 获取状态栏颜色
-(UIColor*)getStatusBarBackground;
/// @brief 设置状态栏颜色
-(void)setStatusBarBackground:(UIColor*)newColor;
/// @brief 设置状态栏样式
-(void)setStatusBarStyle:(UIStatusBarStyle)statusBarStyle;
-(UIStatusBarStyle)getStatusBarStyle;
-(void)wantsFullScreen:(BOOL)fullScreen;
-(BOOL)getStatusBarHidden;
@end

/// H5+核心类负责H5+runtime的启动关闭
@interface PDRCore : NSObject

/// @brief Runtime应用管理对象
@property(nonatomic, readonly)PDRCoreAppManager *appManager;
/// @brief Runtime根视图管理对象
@property(nonatomic, readonly)PDRCoreWindowManager *coreWindow;
/// @brief Runtime设置对象,保存设置信息
@property(nonatomic, readonly)PDRCoreSettings *settings;
/// @brief Runtime启动参数,与系统参数保持一致
@property(nonatomic, retain)NSDictionary* launchOptions;
/// @brief Runtime代理类
@property(nonatomic, assign)id<PDRCoreDelegate> coreDeleagete;
/// @brief 设置5+Runtime ViewContoller persentViewController 未设置使用rootViewContoller
@property(nonatomic, assign)UIViewController *persentViewController;
/// 生成错误码
+ (NSError*)errorWithCode:(NSInteger)code withMessage:(NSString*)message;
/// 消息
+ (void)postNotification:(NSString *)aName withPoster:(id)object withArgument:(id)anObject;
+ (void)observerNotification:(NSString *)aName use:(id)observer selector:(SEL)aSelector;
+ (void)removeObserver:(id)observer onNotification:(NSString *)aName;
/// @brief 获取Core单例对象
+ (PDRCore*)Instance;
#pragma mark - life cycle
/// @brief 设置shell启动参数
+ (BOOL)initEngineWihtOptions:(NSDictionary *)launchOptions
                  withRunMode:(PDRCoreRunMode)runMode;
/// @brief 关闭runtime
+ (BOOL)destoryEngine;
/**
 @brief 
 PDRCoreRunModeNormal 才支持
 @see 
 @return int
*/
- (int)start;
/**
 @brief 显示启动图
 PDRCoreRunModeNormal 才支持
 @see start
 @return int
 */
- (int)showLoadingPage;
/// @brief 获取PandoraApi.bundle的路径
- (NSString*)mainBundlePath;
/// @brief 设置应用运行时目录<br/>
/// 当应用 runmode为liberate时将把资源拷贝到该目录<br/>
/// 应用运行时产生的文件在该目录下生成<br/>
- (int)setAppsRunPath:(NSString*)workPath;
/// @brief 设置runtime应用的安装目录<br/>
/// @brief 该地址为安装包中携带的应用资源位置
- (int)setAppsInstallPath:(NSString*)installPath;
- (void)setInnerVersion:(NSString*)innerVersion;
/// @brief 设置runtime文档目录.
- (int)setDocumethPath:(NSString*)documentPath;
/// @brief 设置runtime下载目录
- (int)setDownloadPath:(NSString*)downlaodPath;
/// @brief 设置runtiem启动时自动运行的APP
- (int)setAutoStartAppid:(NSString*)appid;
/// @brief 设置runtime根视图的父亲View
- (int)setContainerView:(UIView*)containerView;
/// @brief 通知runtime处理指定的事件想问下
+ (id)handleSysEvent:(PDRCoreSysEvent)evt withObject:(id)object;
- (id)handleSysEvent:(PDRCoreSysEvent)evt withObject:(id)object;
/**
 @brief 设置指定app的文档目录
 @param appid 要设置的appid
 @param doucmentPath 要设置的路径
 @return int 0 成功
 */
- (int)setApp:(NSString*)appid documentPath:(NSString*)doucmentPath;
/**
 @brief 注册第三方扩展的插件
 @param pluginName 插件名称JS文件中定义的名字
 @param impClassName 插件对应的实现类名
 @param pluginType 插件类型 详情: `PDRExendPluginType`
 @see PDRExendPluginType
 @param javaScript js实现 为javascript文本
 @return int 0 成功
 */
- (int)regPluginWithName:(NSString*)pluginName
             impClassName:(NSString*)impClassName
                    type:(PDRExendPluginType)pluginType
               javaScript:(NSString*)javaScript;
/**
 @brief 注册第三方扩展的插件
 @param pluginName 插件名称JS文件中定义的名字
 @param impClassName 插件对应的实现类名
 @param pluginType 插件类型 详情: `PDRExendPluginType`
 @see PDRExendPluginType
 @param javaScript js实现 为javascript文件 该文件为同步加载
 @return int 0 成功
 */
- (int)regPluginWithName:(NSString*)pluginName
            impClassName:(NSString*)impClassName
                    type:(PDRExendPluginType)pluginType
              javaScriptPath:(NSString*)javaScript;

#pragma deprecated
- (int)startAsWebClient;
- (int)startAsAppClient;
- (int)load __attribute__((deprecated));
- (int)unLoad __attribute__((deprecated));
- (int)end __attribute__((deprecated));
@end