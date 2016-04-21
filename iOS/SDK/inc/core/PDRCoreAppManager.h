//
//  PDR_Application.h
//  Pandora
//
//  Created by Mac Pro on 12-12-22.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "PDRCoreApp.h"

@class PDRCoreAppCongfigParse;
@protocol PDRCoreAppWindowDelegate;

NS_ASSUME_NONNULL_BEGIN
/// 应用管理模块
@interface PDRCoreAppManager : NSObject

/// 当前激活的应用
@property (nonatomic, readonly)PDRCoreApp *activeApp;

///查询应用
- (PDRCoreApp*__nullable)getAppByID:(NSString*)appid;
- (PDRCoreApp*)getMainApp;

///重启指定应用
- (void)restart:(PDRCoreApp*)coreApp;
- (void)restartWithAppid:(NSString*)appId;
/// 关闭指定的应用
- (void)end:(PDRCoreApp*)coreApp;
- (void)end:(PDRCoreApp*)coreApp animated:(BOOL)animated;
/// 关闭指定的应用
- (void)endWithAppid:(NSString*)appId;
- (void)endWithAppid:(NSString*)appId animated:(BOOL)animated;
/**
 @brief 创建app
 @param location app目录
 @param args 启动参数
 @return int
 */
- (PDRCoreApp*)openAppAtLocation:(NSString*)location
                   withIndexPath:(NSString*)indexPath
                        withArgs:(NSString*__nullable)args
                    withDelegate:(id<PDRCoreAppWindowDelegate>__nullable)delegate;
@end
NS_ASSUME_NONNULL_END
