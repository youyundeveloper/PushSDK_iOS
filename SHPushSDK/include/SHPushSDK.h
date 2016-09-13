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
#define SHPushSDKVersion                @"1.0.3"

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
 *  初始化SDK，设置授权信息(默认使用游云提供的UDID)
 *
 *  @param client        游云app帐号ID
 *  @param secret        游云app帐号密钥
 *  @param launchOptions 启动参数
 *  @param platform 平台
 *
 *  @note 必须在UIApplicationDelegate的回调中
 *   - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions; 调用loadEngineWithPush。
 */
+ (void)startWithClientID:(NSString*)client
                   secret:(NSString*)secret
            launchOptions:(NSDictionary *)launchOptions
                 platform:(YYPushSDKPatform)platform;

/**
 *  初始化SDK，设置授权信息
 *
 *  @param client        游云app帐号ID
 *  @param secret        游云app帐号密钥
 *  @param udid          设备标识ID
 *  @param launchOptions 应用启动参数
 *  @param platform      平台
 */
+ (void)startWithClientID:(NSString*)client
                   secret:(NSString*)secret
                     udid:(NSString *)udid
            launchOptions:(NSDictionary *)launchOptions
                 platform:(YYPushSDKPatform)platform;
/*! @method
 *  获取设备token。
 *
 *  @note 需要在UIApplicationDelegate的回调
 *   -(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
 *  中调用trimDeviceToken。
 */
+ (NSString *)trimDeviceToken:(NSData *)deviceToken;
/**
 *  @method
 *  Called when your app has received a remote notification.
 *  app运行时收到推送, 用来统计
 *
 *  @param userInfo app收到的苹果推送信息
 */
+ (void)didReceiveRemoteNotification:(NSDictionary *)userInfo;

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
/**
 *  获取游云提供的UDID
 *
 *  @return UDID字符串
 */
+ (NSString *)getUDID;

#pragma mark - 消息未读数设置
/**
 *  @brief 设置消息的未读数量(短链接)
 *
 *  @param number  用户还剩的消息未读数
 *  @param handler 回调
 */
- (void)wchatSetMsgUnreadNumber:(NSInteger)number
                     completion:(void (^)(BOOL success, NSError *err))handler;
/**
 *  @brief 设置消息减少的未读数量(短链接)
 *
 *  @param number  要减少的未读数
 *  @param handler 回调
 */
- (void)wchatReduceMsgUnreadNumber:(NSInteger)number
                        completion:(void (^)(BOOL success, NSError *err))handler;

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

#pragma mark - 连接状态

/**
 *  @brief 连接状态回调
 *
 *  @param instance 实例
 *  @param state    连接状态
 */
- (void)onConnectState:(SHPushSDK *)instance
                 state:(int)state;

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


@end
