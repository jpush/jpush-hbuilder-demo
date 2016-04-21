/*
 *------------------------------------------------------------------
 *  pandora/feature/map/pg_object.h
 *  Description:
 *      JS Native对象抽象基类实现
 *  DCloud Confidential Proprietary
 *  Copyright (c) Department of Research and Development/Beijing/DCloud.
 *  All Rights Reserved.
 *
 *  Changelog:
 *	number	author	modify date  modify record
 *   0       xty     2012-12-10  创建文件
 *------------------------------------------------------------------
 */

#import <Foundation/Foundation.h>

@interface PGObject : NSObject
@property(nonatomic, readonly)NSString* UUID;
@end

@interface NSObject(PGObject)
-(NSString*)JSObject;
+(NSString*)JSArray:(NSArray*)pois;
-(NSDictionary*)NativeJSON;
+(NSArray*)NativeJSONArray:(NSArray*)object;
- (BOOL)updateObject:(NSArray*)command;
- (NSData*)updateObjectSync:(NSArray*)command;
@end
