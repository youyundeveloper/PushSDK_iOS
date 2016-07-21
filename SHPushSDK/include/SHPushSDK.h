//
//  SHPushSDK.h
//  SHPushSDK
//
//  Created by Frederic on 15/12/24.
//  Copyright © 2015年 Sina ShiHui. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//sdk版本
#define SHPushSDKVersion                @"1.0.0"

typedef NS_ENUM(NSInteger, YYPushSDKPatform) {
    YYPushSDKPlatformOnline              = 1,    // 生产环境
    YYPushSDKPlatformDevelop             = 2,    // 开发环境
};

@protocol SHPushSDKDelegate;

@interface SHPushSDK : NSObject

/**
 *  是否授权验证成功
 */
@property (nonatomic, assign, readonly) BOOL isAuth;

@property (nonatomic, weak) id<SHPushSDKDelegate> delegate;


#pragma mark method

/**
 *  推送单例
 *
 *  @return 注册单例
 */
+ (SHPushSDK *)sharedInstance;

/**
 *  @brief 释放单例
 */
+ (void)purgeSharedInstance;

/**
 *  @method
 *  添加推送
 *
 *  @param client   客户端ID
 *  @param secret   客户端key(secret)
 *  @param platform 平台
 *
 *  @note 必须在UIApplicationDelegate的回调中
 *   - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions; 调用loadEngineWithPush。
 */
+ (void)startWithClientID:(NSString*)client
                   secret:(NSString*)secret
                 platform:(YYPushSDKPatform)platform;

/*! @method
 *  获取设备token。
 *
 *  @note 需要在UIApplicationDelegate的回调
 *   -(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
 *  中调用trimDeviceToken。
 */
+ (NSString *)trimDeviceToken:(NSData *)deviceToken;

#pragma mark - 服务相关接口

/*! @method
 *  当前设备注册推送. push时段，需要登录成功后才能有效注册push.
 *
 *  @param pushToken ios注册的推送token
 *  @param startTime push时段开始时间(0~24),默认0,  如: 开始时间为9,  结束时间为20, push时段从当天9 点到 当天  20点.
 *  @param endTime   push时段结束时间(0~24),默认24, 如: 开始时间为20, 结束时间为9,  push时段从当天20点到 第二天 9点.
 *  @param handler   回调block (是否操作成功, 如果错误则返回错误信息)
 *
 */
- (void)deviceRegisterPush:(NSString *)pushToken
             pushStartTime:(NSInteger)startTime
                   endTime:(NSInteger)endTime
         completionHandler:(void (^)(BOOL isRegister, NSError* requestError))handler;

/*! @method
 *  取消push服务.
 *
 *  @param handler 回调block (设备信息注册信息, 如果错误则返回错误信息)
 */
- (void)deviceUnRegisterPush:(void (^)(BOOL isUnRegister, NSError* requestError))handler;

/*! @method
 *  获取设备信息.
 *
 *  @param handler 回调block (设备信息注册信息, 如果错误则返回错误信息)
 */
- (void)deviceInfoWithCompletionHandler:(void (^)(NSDictionary *deviceInfo, NSError* requestError))handler;
/*! @method
 * 退出登录
 *
 * @see registerWMSDK:
 * @see onLogoutWithError:
 */
- (void)Logout;

#pragma mark - 消息未读数设置
/**
 *  @brief 设置消息未读数
 *
 *  @param number 未读数数量
 *  @param tag    消息标示, 用于回调
 *  @param errPtr 错误句柄
 *
 *  @return 是否发送设置, YES是, NO否
 */
- (BOOL)wchatSetUnreadNumber:(NSInteger)number
                     withTag:(NSInteger)tag
                       error:(NSError **)errPtr;

/**
 *  @brief 设置减少消息未读数 - number
 *
 *  @param number 减掉的消息未读数
 *  @param tag    消息标示, 用于回调
 *  @param errPtr 错误句柄
 *
 *  @return 是否发送设置, YES是, NO否
 */
- (BOOL)wchatMinusUnreadNumber:(NSInteger)number
                       withTag:(NSInteger)tag
                         error:(NSError **)errPtr;

@end

@protocol SHPushSDKDelegate <NSObject>

@optional

/**
 *  @brief 认证回调, 必须实现
 *
 *  @param instance 实例
 *  @param userinfo 认证成功, 返回用户信息
 *  @param error    认证失败, 返回错误信息
 */
- (void)onwchatAuth:(SHPushSDK *)instance
           userinfo:(NSDictionary *)userinfo
          withError:(NSError *)error;
/**
 *  @brief 连接成功回调
 *
 *  @param instance 实例
 */
- (void)onConnected: (SHPushSDK *)instance;

/**
 *  @brief 连接断开回调
 *
 *  @param instance 实例
 *  @param error    如连接出错断开, 则返回错误消息
 */
- (void)onDisconnect:(SHPushSDK *)instance withError:(NSError *)error;

/**
 *  @brief 向服务器发送断开连接的消息回调
 *
 *  @param instance 实例
 *  @param error    如果设置失败, 则返回错误信息
 */
- (void)onClose:(SHPushSDK *)instance withError:(NSError *)error;

/**
 *  @brief 退出登陆回调
 *
 *  @param instance 实例
 *  @param error    如登陆出错, 则返回错误消息
 */
- (void)onLogout:(SHPushSDK *)instance withError:(NSError *)error;

/**
 *  @brief 超时回调
 *
 *  @param instance 实例
 *  @param tag      消息标示
 *  @param error    如操作超时, 则返回错误消息
 */
- (void)onTimeout:(SHPushSDK *)instance
          withTag:(NSInteger)tag
        withError:(NSError *)error;

#pragma mark - 前后台切换
/**
 *  @brief 客户端退到后台, 关闭服务器消息notice下发, 开启推送回调
 *
 *  @param instance 实例
 *  @param error    如果设置失败, 则返回错误信息
 */
- (void)onPreClose:(SHPushSDK *)instance withError:(NSError *)error;

/**
 *  @brief 客户端回到前台, 开启服务器消息notice下发, 关闭推送
 *
 *  @param instance 实例
 *  @param error    如果设置失败, 则返回错误信息
 */
- (void)onKeepAlive:(SHPushSDK *)instance
          withError:(NSError *)error;

#pragma mark - 连接状态

/**
 *  @brief 连接状态回调
 *
 *  @param instance 实例
 *  @param state    连接状态
 */
- (void)onConnectState:(SHPushSDK *)instance
                 state:(int)state;

#pragma mark - 未读数设置回调
/**
 *  未读数设置回调
 *
 *  @param instance   实例
 *  @param callbackId 消息标示
 */
- (void)onUnreadNoticeCallback:(SHPushSDK*)instance
               withCallbackId:(NSInteger)callbackId;

#pragma mark - 接收文本
/**
 *  @brief 接收文本消息回调
 *
 *  @param instance   实例
 *  @param messageId  消息id
 *  @param fromUid    发消息人Uid
 *  @param toUid      收消息人Uid
 *  @param timevalue  消息时间
 *  @param content    消息内容
 *  @param extContent 消息扩展内容
 *  @param error      如收消息出错, 则返回错误信息
 */
- (void)onRecvMsg:(SHPushSDK *)instance
    withMessageId:(NSString *)messageId
          fromUid:(NSString *)fromUid
            toUid:(NSString *)toUid
             time:(NSInteger)timevalue
          content:(NSData *)content
          extBody:(NSData *)extContent
        withError:(NSError *)error;

#pragma mark - 获取个人 & 群组消息未读数
/**
 *  @brief 获取消息未读数
 *
 *  @param user  用户消息未读数, 字典格式 @{ @"用户id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 }, @"用户id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 } }
 *  @param group 群组消息未读数, 字典格式 @{ @"群组id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 }, @"群组id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 } }
 */
- (void)onRecvUnreadNumber:(SHPushSDK *)instance
                  withUser:(NSDictionary *)user
                 withGroup:(NSDictionary *)group;


@end
