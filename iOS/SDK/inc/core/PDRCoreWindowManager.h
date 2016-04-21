
//
//  PDRCore.h
//  Pandora
//
//  Created by Mac Pro on 12-12-22.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "PDRCoreDefs.h"

@interface PDRCoreAppDefalutLoadView : UIImageView {
    UIImage *_iPadLandscapeImg;
    UIImage *_iPadPortraitImg;
}
- (void)setLoadingPage;
@end

@class PDRCoreAppWindow;

@interface PDRCoreWindowManager : UIView

- (void)addAppWindow:(PDRCoreAppWindow*)window;
- (void)removeAppWindow:(PDRCoreAppWindow*)window;

- (void)showLoadingPage:(UIView*)view forKey:(NSString*)key;
- (void)endLoadingPageForKey:(NSString*)key;

- (void)startLoadingPage;
- (void)startLoadingPageUserCustomView:(UIView*)customLoadingView;
- (void)endLoadingPage;
- (void)showIndicatorView;
- (void)hiddenIndicatorView;
- (void)showView:(UIView*)view;
- (void)closeView:(UIView*)view;
//- (void)SetDebugRunMode:(BOOL)bDebug;

- (id)handleSysEvent:(PDRCoreSysEvent)evt withObject:(id)object;

@end