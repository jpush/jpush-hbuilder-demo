//
//  PDR_Manager_defs.h
//  Pandora
//
//  Created by Mac Pro_C on 12-12-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Pandora_PDR_Tool_System_h
#define Pandora_PDR_Tool_System_h
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>

#define IOS_DEV_GROUP_IPAD          0x00000001
#define IOS_DEV_GROUP_IPOD          0x00000002
#define IOS_DEV_GROUP_IPHONE        0x00000004
#define IOS_DEV_GROUP_IPHONE5       0x00000008

#define IOS_DEV_SCALE_NORMAL        0x00010000
#define IOS_DEV_SCALE_DOUBLE        0x00020000

#pragma mark -
#pragma mark Devices Name

#define PDR_DEVNAME_IPHONE1G                0x00000001
#define PDR_DEVNAME_IPHONE3G                0x00000002
#define PDR_DEVNAME_IPHONE3GS               0x00000004
#define PDR_DEVNAME_IPHONE4                 0x00000008
#define PDR_DEVNAME_VERIZONIPHONE4          0x00000010
#define PDR_DEVNAME_IPHOEN4S                0x00000020
#define PDR_DEVNAME_IPHONE5                 0x00000040
#define PDR_DEVNAME_IPODTOUCH1G             0x00000080
#define PDR_DEVNAME_IPODTOUCH2G             0x00000100
#define PDR_DEVNAME_IPODTOUCH3G             0x00000200
#define PDR_DEVNAME_IPODTOUCH4G             0x00000400
#define PDR_DEVNAME_IPAD                    0x00000800
#define PDR_DEVNAME_IPAD2WIFI               0x00001000
#define PDR_DEVNAME_IPAD2GSM                0x00002000
#define PDR_DEVNAME_IPAD2CDMA               0x00004000
#define PDR_DEVNAME_IPADMINI                0x00008000


#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     ========================================================================
     * @Summary: 获取系统的屏幕尺寸
     *
     * @Parameters:
     *       
     *
     * @Returns: 返回当前设备的宽高
     *
     * @Remark: 获取的屏幕尺寸为手机正向时宽高
     *
     * @Changelog:
     *
     ========================================================================
     */
    CGRect PDR_Tool_GetSystemScreenRect();

    /*
     ========================================================================
     * @Summary: 获取系统的屏幕尺寸
     *
     * @Parameters:
     *
     *
     * @Returns: 返回当前设备的宽高
     *
     * @Remark: 返回设备指定方向的宽高
     *
     * @Changelog:
     *
     ========================================================================
     */
    CGRect PDR_Tool_GetScreenRectByOrientation(UIInterfaceOrientation eOrientation);
    
   // int     DHA_Tool_GetDeviceModle();
    
   // int     DHA_Tool_GetDevName();

    CGFloat PDR_Tool_String2ScreenValue(NSString* pStringValue, BOOL bIsHorizontal);
    
    UIColor* PDR_Tool_String2UIColor(NSString* pColorString);
    
#ifdef __cplusplus
}
#endif

#endif
