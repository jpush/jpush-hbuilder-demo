//
//  PDR_Application.h
//  Pandora
//
//  Created by Mac Pro on 12-12-22.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#import <Foundation/Foundation.h>

#import "PDRCoreDefs.h"
#import "PDRCoreSettings.h"
#import "PDRCoreAppWindow.h"

@class PDRCoreAppInfo;
@class PDRCoreAppWindow;
@class PDRCoreAppFrame;

extern NSString *const PDRCoreAppDidStartedFailedKey;
/// H5+应用
@interface PDRCoreApp : NSObject
/// 应用运行目录
@property (nonatomic, copy)NSString *workRootPath;
/// 安装包位置目录
@property (nonatomic, copy)NSString *executableRootPath;
/// 应用信息
@property (nonatomic, readonly)PDRCoreAppInfo *appInfo;
/// 应用根窗口
@property (nonatomic, readonly)PDRCoreAppWindow *appWindow;
/// 应用首页面
@property (nonatomic, readonly)PDRCoreAppFrame *mainFrame;
/// 创建App使用的参数
@property (nonatomic, retain)NSString *arguments;
@property (nonatomic, readonly) BOOL isStreamApp;
/// 激活H5+应用
- (void)active;
/// H5+应用转入后台
- (void)deActive;

@end

