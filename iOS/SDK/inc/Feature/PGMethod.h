//
//  PGMethod.h
//  Pandora
//
//  Created by Pro_C Mac on 12-12-24.
//
//

#ifndef _PANDORA_PGMETHOD_H_
#define _PANDORA_PGMETHOD_H_
#import <Foundation/Foundation.h>

/** JavaScript 调用参数
 */
@interface PGMethod : NSObject

+ (PGMethod*)commandFromJson:(NSArray*)pJsonEntry;
- (void) legacyArguments:(NSMutableArray**)legacyArguments andDict:(NSMutableDictionary**)legacyDict ;

@property (nonatomic, retain)NSString*   htmlID;
@property (nonatomic, retain)NSString*   featureName;
@property (nonatomic, retain)NSString*   functionName;
@property (nonatomic, retain)NSString*   callBackID;
@property (nonatomic, retain)NSString*   sid;
/// @brief JavaScirpt调用参数数组
@property (nonatomic, retain)NSArray*    arguments;
-(id)getArgumentAtIndex:(NSUInteger)index;
@end

#endif
