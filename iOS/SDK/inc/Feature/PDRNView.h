//
//  PDRNView.h
//  Pandora
//
//  Created by Pro_C Mac on 13-4-7.
//
//

#import "PGMethod.h"
#import "PGPlugin.h"

@interface PDRJSContext : PGPlugin

@end

/**
 NView基类所有扩展出的NView插件都应该从该类继承
 */
@interface PDRNView : UIView {
    NSMutableDictionary *_viewOptions;
}

/// @brief JavaScript执行环境
@property (nonatomic, retain)PDRJSContext *JSContext;
/// @brief NView插件类别名称
@property (nonatomic, copy) NSString *identity;
/// @brief NView唯一标识
@property (nonatomic, copy) NSString *ID;
@property (nonatomic, copy) NSString *jsUUID;
@property (nonatomic, copy) NSString *jsCallbackId;
@property (nonatomic, copy) NSString *parent;
@property (nonatomic, strong)UIViewController*  viewController;
@property (nonatomic, retain)NSDictionary* options;
/**
 @brief 使用JS NViewOption创建NView 子类应该重写该方法实现初始化
 @param options NViewOption
 @return id NView对象
 */
- (id)initWithOptions:(NSDictionary*)options;
/**
 @brief 分发event事件
 @param evtName 事件名称
 @return 无
 */
- (void)dispatchEvent:(NSString*)evtName;
/**
 @brief NView从NWindow上移除时回调
 @param
 @return 无
 */
- (void)onRemoveFormSuperView;
- (NSData*)getMettics:( PGMethod*) pMethod;
// 返回当前控件最小尺寸，可以是%，或者PX值，或者Auto
- (NSDictionary*)GetMiniControllerSize:(int)nOri;
- (void)CreateView:(PGMethod*)pMethod;
- (NSString*)getObjectString;

@end

