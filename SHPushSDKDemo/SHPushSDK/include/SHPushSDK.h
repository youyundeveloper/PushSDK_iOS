//
//  SHPushSDK.h
//  SHPushSDK
//
//  Created by Frederic on 15/12/24.
//  Copyright © 2015年 Sina ShiHui. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "WChatCommon.h"

@protocol SHPushSDKDelegate;

@interface SHPushSDK : NSObject

/**
 *  是否验证成功
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
                 platform:(WChatPlatform)platform;

/*! @method
 *  获取设备token。
 *
 *  @note 需要在UIApplicationDelegate的回调
 *   -(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
 *  中调用trimDeviceToken。
 */
+ (NSString *)trimDeviceToken:(NSData *)deviceToken;

/**
 *  客户端回到前台, 开启服务器消息notice下发, 关闭推送 (进入前台调用)
 *  @note  需要在UIApplicationDelegate的回调
 *  - (void)applicationDidBecomeActive:(UIApplication *)application; 中调用。
 *  @param application
 */
+ (void)applicationDidBecomeActive:(UIApplication *)application;

/*! @method
 *  客户端退到后台, 关闭服务器消息notice下发, 开启推送 (进入后台调用),设置下次push时的未读数。
 *
 *  @note 需要在UIApplicationDelegate的回调
 *  - (void)applicationDidEnterBackground:(UIApplication *)application;
 *  中调用。
 */
+ (void)applicationDidEnterBackground:(UIApplication *)application;

/*! @method
 *  重新连接服务器。
 *
 *  @note 需要在UIApplicationDelegate的回调
 *  - (void)applicationWillEnterForeground:(UIApplication *)application;
 *  中调用。
 */
+ (void)applicationWillEnterForeground:(UIApplication *)application;

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
- (void)deviceRegisterPush:(NSString *)pushToken pushStartTime:(NSInteger)startTime endTime:(NSInteger)endTime completionHandler:(void (^)(BOOL isRegister, NSError* requestError))handler;

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

/*! @method
 *  黑名单添加用户.
 *
 *  @param userId  聊天对方uid(非当前登陆用户)
 *  @param handler 回调block (是否操作成功, 如果错误则返回错误信息)
 */
- (void)blacklistAddUser:(NSString *)userId completionHandler:(void (^)(BOOL isAdd, NSError* requestError))handler;

/*! @method
 *  黑名单删除用户.
 *
 *  @param userId  聊天对方uid(非当前登陆用户)
 *  @param handler 回调block (是否操作成功, 如果错误则返回错误信息)
 */
- (void)blacklistDelUser:(NSString *)userId completionHandler:(void (^)(BOOL isDel, NSError* requestError))handler;

/*! @method
 *  当前用户黑名单
 *
 *  @param handler 回调block (黑名单数据, 如果错误则返回错误信息)
 */
- (void)blacklistWithCompletionHandler:(void (^)(NSArray *blacklist, NSError* requestError))handler;

#pragma mark - 群组 - 管理相关接口

/**
 *  创建群组
 *
 *  @param handler 回调block (创建成功的群组id, 如果错误则返回错误信息)
 */
- (void)createGroupWithCompletionHandler:(void(^)(NSString *groupId, NSError* requestError))handler;

/**
 *  群组加人
 *
 *  @param groupId 群组id
 *  @param userIds 用户id数组
 *  @param handler 回调block (是否操作成功, 如果错误则返回错误信息)
 */
- (void)group:(NSString *)groupId addUser:(NSArray *)userIds completionHandler:(void (^)(BOOL isAdd, NSError* requestError))handler;

/**
 *  群组踢人
 *
 *  @param groupId 群组id
 *  @param userIds 用户id数组
 *  @param handler 回调block (是否操作成功, 如果错误则返回错误信息)
 */
- (void)group:(NSString *)groupId delUser:(NSArray *)userIds completionHandler:(void (^)(BOOL isDel, NSError* requestError))handler;

/**
 *  退出群组
 *
 *  @param groupId 群组id
 *  @param handler 回调block (是否操作成功, 如果错误则返回错误信息)
 */
- (void)exitGroup:(NSString *)groupId CompletionHandler:(void(^)(BOOL isExit, NSError* requestError))handler;

/**
 *  获取群组成员
 *
 *  @param groupId 群组id
 *  @param handler 回调block (群组成员数据, 如果错误则返回错误信息)
 */
- (void)getGroupUsers:(NSString *)groupId completionHandler:(void (^)(NSArray *users, NSError* requestError))handler;

/**
 *  获取当前用户的群组
 *
 *  @param handler 回调block (用户的群组数据, 如果错误则返回错误信息)
 */
- (void)getUserGroupsWithCompletionHandler:(void (^)(NSArray *groups, NSError* requestError))handler;

#pragma mark - 发送通知消息接口
/**
 *  @brief 发送通知消息
 *
 *  @param tuid    收消息人Uid
 *  @param type    通知类型
 *  @param content 通知内容
 *  @param errPtr  错误句柄
 *
 *  @return 消息是否正常发送, YES是, NO否
 */
- (BOOL)wchatSendNoticeMessage:(NSString *)tuid withType:(NSInteger)type withContent:(NSString *)content error:(NSError **)errPtr;

#pragma mark - 发送文本消息接口
/**
 *  @brief 发送文本消息
 *
 *  @param tuid       收消息人Uid
 *  @param content    消息内容
 *  @param extContent 扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param type       消息类型
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 消息是否正常发送, YES是, NO否
 */
- (BOOL)wchatSendMsg:(NSString *)tuid body:(NSData *)content extBody:(NSData *)extContent withTag:(NSInteger)tag withType:(WChatFileType)type withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *  @brief 发送群组文本消息
 *
 *  @param gid        收消息群组Gid
 *  @param content    消息内容
 *  @param extContent 扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param type       消息类型
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 消息是否正常发送, YES是, NO否
 */
- (BOOL)wchatSendGroupMsg:(NSString *)gid body:(NSData *)content extBody:(NSData *)extContent withTag:(NSInteger)tag withType:(WChatFileType)type withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

#pragma mark - 发送语音消息接口

/**
 *  @brief 发送音频消息
 *
 *  @param tuid       收消息人Uid
 *  @param spanId     语音唯一标示
 *  @param sequenceNo 语音分片编号, 如 1, 2, 3, ... -1, -1 表示结束
 *  @param content    语音消息内容
 *  @param ext        扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 消息是否正常发送, YES是, NO否
 */
- (BOOL)wchatSendVoice:(NSString *)tuid spanId:(NSString *)spanId sequenceNo:(NSInteger)sequenceNo content:(NSData *)content ext:(NSData *)ext withTag:(NSInteger)tag  withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *  @brief 发送群组音频消息
 *
 *  @param gid        收消息群组Gid
 *  @param spanId     语音唯一标示
 *  @param sequenceNo 语音分片编号, 如 1, 2, 3, ... -1, -1 表示结束
 *  @param content    语音消息内容
 *  @param ext        扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 消息是否正常发送, YES是, NO否
 */
- (BOOL)wchatSendGroupVoice:(NSString *)gid spanId:(NSString *)spanId sequenceNo:(NSInteger)sequenceNo content:(NSData *)content ext:(NSData *)ext withTag:(NSInteger)tag  withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;


/**
 *  @brief 获取语音唯一标示
 *
 *  @param tuid 收消息人Uid
 *
 *  @return 语音消息唯一标示
 */
- (NSString *)getVoiceSpanId:(NSString *)tuid;

#pragma mark - 发送文件消息接口
/**
 *  @brief 发送文件给个人
 *
 *  @param tuid       收消息人Uid
 *  @param filepath   文件路径
 *  @param extContent 扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param fileType   文件类型
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 文件id
 */
- (NSString *)wchatSendFile:(NSString *)tuid path:(NSString *)filepath extBody:(NSData *)extContent withTag:(NSInteger)tag filetype:(WChatFileType)fileType withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *  @brief 发送文件给群组
 *
 *  @param gid        收消息群组Gid
 *  @param filepath   文件路径
 *  @param extContent 扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param fileType   文件类型
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 文件id
 */
- (NSString *)wchatSendGroupFile:(NSString *)gid path:(NSString *)filepath extBody:(NSData *)extContent withTag:(NSInteger)tag filetype:(WChatFileType)fileType withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *  @brief 发送文件给个人, 带缩略图
 *
 *  @param tuid       收消息人Uid
 *  @param filepath   文件路径
 *  @param nailpath   缩略图路径
 *  @param extContent 扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param fileType   文件类型
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 文件id
 */
- (NSString *)wchatSendFileWithThumbnail:(NSString *)tuid path:(NSString *)filepath nailpath:(NSString *)nailpath extBody:(NSData *)extContent withTag:(NSInteger)tag filetype:(WChatFileType)fileType withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *	@brief 发送文件给群组, 带缩略图
 *
 *  @param gid        收消息群组Gid
 *  @param filepath   文件路径
 *  @param nailpath   缩略图路径
 *  @param extContent 扩展消息内容
 *  @param tag        消息标示, 用于回调
 *  @param fileType   文件类型
 *  @param timeout    调用超时时间
 *  @param errPtr     错误句柄
 *
 *  @return 文件id
 */
- (NSString *)wchatSendGroupFileWithThumbnail:(NSString *)gid path:(NSString *)filepath nailpath:(NSString *)nailpath extBody:(NSData *)extContent withTag:(NSInteger)tag filetype:(WChatFileType)fileType withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;


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
- (BOOL)wchatSetUnreadNumber:(NSInteger)number withTag:(NSInteger)tag error:(NSError **)errPtr;

/**
 *  @brief 设置消息未读数 - number
 *
 *  @param number 减掉的消息未读数
 *  @param tag    消息标示, 用于回调
 *  @param errPtr 错误句柄
 *
 *  @return 是否发送设置, YES是, NO否
 */
- (BOOL)wchatMinusUnreadNumber:(NSInteger)number withTag:(NSInteger)tag  error:(NSError **)errPtr;

#pragma mark - 获取文件
/**
 *  @brief 根据文件id获取文件, 分片获取
 *
 *  @param fid     文件id
 *  @param length  文件长度
 *  @param size    分片长度
 *  @param tag     消息标示, 用于回调
 *  @param index   分片索引
 *  @param timeout 调用超时时间
 *  @param errPtr  错误句柄
 *
 *  @return 是否开始获取, YES是, NO否
 */
- (BOOL)wchatGetFile:(NSString *)fid filelength:(UInt64)length pieceSize:(UInt32)size withTag:(NSInteger)tag index:(UInt32)index withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;

/**
 *  @brief 根据文件id获取文件
 *
 *  @param fid     文件id
 *  @param length  文件长度
 *  @param tag     消息标示, 用于回调
 *  @param timeout 调用超时时间
 *  @param errPtr  错误句柄
 *
 *  @return 是否开始获取, YES是, NO否
 */
- (BOOL)wchatGetFile:(NSString *)fid filelength:(UInt64)length withTag:(NSInteger)tag withTimeout:(NSTimeInterval)timeout error:(NSError **)errPtr;


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
- (void)onwchatAuth:(SHPushSDK *)instance userinfo:(NSDictionary *)userinfo withError:(NSError *)error;
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
- (void)onTimeout:(SHPushSDK *)instance withTag:(NSInteger)tag withError:(NSError *)error;

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
- (void)onKeepAlive:(SHPushSDK *)instance withError:(NSError *)error;

#pragma mark - 连接状态

/**
 *  @brief 连接状态回调
 *
 *  @param instance 实例
 *  @param state    连接状态
 */
- (void)onConnectState:(SHPushSDK *)instance state:(int)state;

#pragma mark - 发送消息回调
/**
 *  @brief 消息已送达到服务器, 但服务器还未下发相应, sdk预先返回, 随后服务器会下发相应, 以及时间戳.
 *  可理解为发送消息成功, 前端可根据此状态, 预先显示消息发送成功, 后台处理服务器下发.
 *
 *  @param instance 实例
 *  @param tag      消息标示
 */
- (void)onSendPreBack:(SHPushSDK *)instance withTag:(NSInteger)tag;

/**
 *  @brief 发送文本消息回调
 *
 *  @param instance  实例
 *  @param tag       消息标示
 *  @param time      消息时间
 *  @param messageId 消息id
 *  @param error     如发送出错, 则返回错误消息
 */
- (void)onSendMsg:(SHPushSDK *)instance withTag:(NSInteger)tag withTime:(NSInteger)time withMessageId:(NSString *)messageId withError:(NSError *)error;

/**
 *  @brief 发送文件回调
 *
 *  @param instance  实例
 *  @param tag       消息标示
 *  @param time      消息时间
 *  @param messageId 消息id
 *  @param error     如发送出错, 则返回错误消息
 */
- (void)onSendFile:(SHPushSDK *)instance withTag:(NSInteger)tag withTime:(NSInteger)time withMessageId:(NSString *)messageId withError:(NSError *)error;

#pragma mark - 未读数设置回调
/**
 *  未读数设置回调
 *
 *  @param instance   实例
 *  @param callbackId 消息标示
 */
-(void)onUnreadNoticeCallback:(SHPushSDK*)instance withCallbackId:(NSInteger)callbackId;

#pragma mark - 接收文本, 语音, 文件, notice, 订阅消息回调
/**
 *  @brief 接收文本消息回调
 *
 *  @param instance   实例
 *  @param messageId  消息id
 *  @param fromUid    发消息人Uid
 *  @param toUid      收消息人Uid
 *  @param type       消息类型
 *  @param timevalue  消息时间
 *  @param content    消息内容
 *  @param extContent 消息扩展内容
 *  @param error      如收消息出错, 则返回错误信息
 */
- (void)onRecvMsg:(SHPushSDK *)instance
    withMessageId:(NSString *)messageId
          fromUid:(NSString *)fromUid
            toUid:(NSString *)toUid
         filetype:(WChatFileType)type
             time:(NSInteger)timevalue
          content:(NSData *)content
          extBody:(NSData *)extContent
        withError:(NSError *)error;

/**
 *  @brief 接收群组文本消息回调
 *
 *  @param instance   实例
 *  @param messageId  消息id
 *  @param gid        群id
 *  @param fromUid    发消息人Uid
 *  @param type       消息类型
 *  @param timevalue  消息时间
 *  @param content    消息内容
 *  @param extContent 消息扩展内容
 *  @param error      如收消息出错, 则返回错误信息
 */
- (void)onRecvGroupMsg:(SHPushSDK *)instance withMessageId:(NSString *)messageId withGroupId:(NSString *)gid fromUid:(NSString *)fromUid filetype:(WChatFileType)type time:(NSInteger)timevalue content:(NSData *)content extBody:(NSData *)extContent withError:(NSError *)error;

/**
 *  @brief 接收语音消息回调
 *
 *  @param instance   实例
 *  @param messageId  消息id
 *  @param fromUid    发消息人Uid
 *  @param toUid      收消息人Uid
 *  @param spanId     语音唯一标识
 *  @param sequenceNo 语音分片编号, 如 1, 2, 3, ... -1, -1 表示结束
 *  @param timevalue  消息时间
 *  @param content    消息内容
 *  @param extContent 消息扩展内容
 *  @param error      如收消息出错, 则返回错误信息
 */
- (void)onRecvVoice:(SHPushSDK *)instance withMessageId:(NSString *)messageId fromUid:(NSString *)fromUid toUid:(NSString *)toUid spanId:(NSString *)spanId sequenceNo:(NSInteger)sequenceNo time:(NSInteger)timevalue content:(NSData *)content extBody:(NSData *)extContent withError:(NSError *)error;

/**
 *  @brief 接收群组语音消息回调
 *
 *  @param instance   实例
 *  @param messageId  消息id
 *  @param gid        群id
 *  @param fromUid    发消息人Uid
 *  @param spanId     语音唯一标识
 *  @param sequenceNo 语音分片编号, 如 1, 2, 3, ... -1, -1 表示结束
 *  @param timevalue  消息时间
 *  @param content    消息内容
 *  @param extContent 消息扩展内容
 *  @param error      如收消息出错, 则返回错误信息
 */
- (void)onRecvGroupVoice:(SHPushSDK *)instance withMessageId:(NSString *)messageId withGroupId:(NSString *)gid fromUid:(NSString *)fromUid spanId:(NSString *)spanId sequenceNo:(NSInteger)sequenceNo time:(NSInteger)timevalue content:(NSData *)content extBody:(NSData *)extContent withError:(NSError *)error;

/**
 *  @brief 接收文件消息回调
 *
 *  @param instance      实例
 *  @param messageId     消息id
 *  @param fromUid       发消息人Uid
 *  @param toUid         收消息人Uid
 *  @param type          消息类型
 *  @param timevalue     消息时间
 *  @param fileid        文件id
 *  @param thumbnailData 缩略图二进制数据
 *  @param extContent    消息扩展内容
 *  @param length        文件长度
 *  @param size          文件分片大小
 *  @param error         如收文件出错, 则返回错误信息
 */
- (void)onRecvFile:(SHPushSDK *)instance withMessageId:(NSString *)messageId fromUid:(NSString *)fromUid toUid:(NSString *)toUid filetype:(WChatFileType)type time:(NSInteger)timevalue fileId:(NSString *)fileid thumbnail:(NSData *)thumbnailData extBody:(NSData *)extContent filelength:(UInt64)length pieceSize:(UInt32)size withError:(NSError *)error;

/**
 *  @brief 接收群组文件消息回调
 *
 *  @param instance      实例
 *  @param messageId     消息id
 *  @param gid           群id
 *  @param fromUid       发消息人Uid
 *  @param type          消息类型
 *  @param timevalue     消息时间
 *  @param fileid        文件id
 *  @param thumbnailData 缩略图二进制数据
 *  @param extContent    消息扩展内容
 *  @param length        文件长度
 *  @param size          文件分片大小
 *  @param error         如收文件出错, 则返回错误信息
 */
- (void)onRecvGroupFile:(SHPushSDK *)instance withMessageId:(NSString *)messageId withGroupId:(NSString *)gid fromUid:(NSString *)fromUid filetype:(WChatFileType)type time:(NSInteger)timevalue fileId:(NSString *)fileid thumbnail:(NSData *)thumbnailData extBody:(NSData *)extContent filelength:(UInt64)length pieceSize:(UInt32)size withError:(NSError *)error;

/**
 *  @brief 系统下发的notice消息, 踢人回调
 *
 *  @param instance 实例
 *  @param fuid     发消息人Uid
 *  @param type     Notice类型
 *  @param content  消息内容
 */
- (void)onRecvNoticeMessage:(SHPushSDK *)instance fromUid:(NSString *)fuid withType:(WChatNoticeType)type withContent:(NSString *)content;

/**
 *  @brief 订阅消息回调
 *
 *  @param instance   实例
 *  @param messageId  消息id
 *  @param fromUid    发消息人Uid
 *  @param toUid      收消息人Uid
 *  @param type       消息类型
 *  @param timevalue  消息时间
 *  @param content    消息内容
 *  @param extContent 消息扩展内容
 *  @param error      如收消息出错, 则返回错误信息
 */
-(void)onRecvSubscribeMsg:(SHPushSDK*)instance withMessageId:(NSString*)messageId fromUid:(NSString *)fromUid toUid:(NSString*)toUid filetype:(WChatFileType)type time:(NSInteger)timevalue content:(NSData *)content extBody:(NSData*)extContent withError:(NSError*)error;

#pragma mark - 获取文件 & 文件进度
/**
 *  @brief 获取文件数据回调
 *
 *  @param instance 实例
 *  @param fileid   文件id
 *  @param tag      消息标示
 *  @param error    如获取文件出错, 则返回错误信息
 */
- (void)onGetFile:(SHPushSDK *)instance fileId:(NSString *)fileid withTag:(NSInteger)tag withError:(NSError *)error;

/**
 *  @brief 发送和接收文件进度的回调
 *
 *  @param instance 实例
 *  @param tag      消息标示
 *  @param index    文件分片索引
 *  @param limit    文件分片总数
 *  @param error    如获取进度出错, 则返回错误信息
 */
- (void)onFileProgress:(SHPushSDK *)instance withTag:(NSInteger)tag withIndex:(UInt32)index withLimit:(UInt32)limit withError:(NSError *)error;

#pragma mark - 获取个人 & 群组消息未读数

/**
 *  @brief 获取消息未读数
 *
 *  @param user  用户消息未读数, 字典格式 @{ @"用户id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 }, @"用户id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 } }
 *  @param group 群组消息未读数, 字典格式 @{ @"群组id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 }, @"群组id": @{ @"num": NSNumber 未读数, @"time": NSNumber 消息时间 } }
 */
- (void)onRecvUnreadNumber:(SHPushSDK *)instance withUser:(NSDictionary *)user withGroup:(NSDictionary *)group;


@end
