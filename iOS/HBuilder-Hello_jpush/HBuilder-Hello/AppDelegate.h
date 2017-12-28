//
//  AppDelegate.h
//  Pandora
//
//  Created by Mac Pro_C on 12-12-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "JPUSHService.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate, JPUSHRegisterDelegate>
@property (strong, nonatomic) UIWindow *window;
@end
