//
//  AppDelegate.m
//  SHPushDemo
//
//  Created by Frederic on 15/12/25.
//  Copyright © 2015年 Sina ShiHui. All rights reserved.
//

#import "AppDelegate.h"

#import "SHPushSDK.h"

NSInteger const kPlatform = YYPushSDKPlatformDevelop;

NSString * const CLIENT_ID = kPlatform == YYPushSDKPlatformOnline ? @"1-20525-4ab3a7c3ddb665945d0074f51e979ef0-ios" : @"1-20142-2e563db99a8ca41df48973b0c43ea50a-ios";
NSString * const SECRET    = kPlatform == YYPushSDKPlatformOnline ? @"6f3efde9fb49a76ff6bfb257f74f4d5b" : @"ace518dab1fde58eacb126df6521d34c";

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    // 初始化推送，使用游云提供的UDID
    [SHPushSDK startWithClientID:CLIENT_ID secret:SECRET launchOptions:launchOptions platform:kPlatform];
    // 如果不使用游云提供的UDID，请调用以下方法
//    [SHPushSDK startWithClientID:CLIENT_ID secret:SECRET udid:@"yourselfUDID" launchOptions:launchOptions platform:kPlatform];
    
    NSDictionary* pushNotificationKey = [launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
    if (pushNotificationKey) {
        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"pushLauchApp" message:pushNotificationKey.description delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [alert show];
    }
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
    
    [[SHPushSDK sharedInstance] wchatSetMsgUnreadNumber:0 completion:^(BOOL success, NSError *err) {
        if (err) {
            NSLog(@"%s,error:%@", __FUNCTION__, err.localizedDescription);
        }
    }];
    
//    [[SHPushSDK sharedInstance] wchatReduceMsgUnreadNumber:1 completion:^(BOOL success, NSError *err) {
//        if (err) {
//            NSLog(@"%s,error:%@", __FUNCTION__, err.localizedDescription);
//        }
//    }];
    
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings {
    
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    // 推送
    self.deviceToken = [SHPushSDK trimDeviceToken:deviceToken];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    NSLog(@"fun:%s, %@", __func__, error.description);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    // Called when your app has received a remote notification.
    [SHPushSDK didReceiveRemoteNotification:userInfo];
    
    UILocalNotification *localNotification = [[UILocalNotification alloc]init];
    localNotification.alertTitle = @"LocalNotification";
    localNotification.alertBody = @"本地通知";
    localNotification.userInfo = userInfo;
    [application scheduleLocalNotification:localNotification];
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification {
    NSLog(@"收到本地通知: %@", notification);
}

@end
