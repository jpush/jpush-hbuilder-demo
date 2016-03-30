//
//  PDR_Application.h
//  Pandora
//
//  Created by Mac Pro on 12-12-22.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "PDRCoreAppFrame.h"
#import "PDRCoreApp.h"

@protocol PDRCoreAppWindowDelegate<NSObject>
@optional
/**
 @brief 是否终止关闭appframe只有调用plus.ui.close接口关闭才会触发改事件
 @param appFrame 将要关闭的appframe
 @return BOOL TRUE 关闭 FALSE 不关闭
 */
- (BOOL)shouldCloseAppFrame:(PDRCoreAppFrame*)appframe;
/**
 @brief 是否显示appframe 只有调用plus.ui.show接口显示才会触发改事件
 @param appFrame 将要关闭的appframe
 @return BOOL TRUE 关闭 FALSE 不关闭
 */
- (BOOL)shouldShowAppFrame:(PDRCoreAppFrame*)appframe;
@end

@interface PDRCoreAppWindow : UIView
/**
 @brief PDRCoreAppWindowDelegate
 */
@property(nonatomic, assign)id<PDRCoreAppWindowDelegate> delegate;
/**
 @brief 注册appframe
 @param appFrame
 @return BOOL TRUE 成功 重复注册同一窗口为失败
 */
- (BOOL)registerFrame:(PDRCoreAppFrame*)appFrame;
/**
 @brief 从window中删除appframe
 @param appFrame
 @return BOOL TRUE 成功 重复注册同一窗口为失败
 */
- (BOOL)unRegisterFrame:(PDRCoreAppFrame*)appFrame;
/**
 @brief 根据指定的ID获取appframe
 @param uuid
 @return PDRCoreAppFrame*
 */
- (PDRCoreAppFrame*)getFrameByID:(NSString*)uuid;
/**
 @brief 根据指定的name获取appframe
 @param name
 @return PDRCoreAppFrame*
 */
- (PDRCoreAppFrame*)getFrameByName:(NSString*)name;
/**
 @brief 关闭指定的appframe该接口不会发出shouldCloseAppFrame:
 而是直接关闭
 @param name
 @return PDRCoreAppFrame*
 */
- (void)closeFrame:(PDRCoreAppFrame*)appFrame;
/**
 @brief 显示指定的appframe 该接口不会发出shouldShowAppFrame:
     而是直接显示
 @param name
 @return PDRCoreAppFrame*
 */
- (void)showFrame:(PDRCoreAppFrame*)farme;
/**
 @brief 获取所有frame
 @return NSArray*
 */
- (NSArray*)allFrames;
@end