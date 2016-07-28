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

NSString * const CLIENT_ID = kPlatform == YYPushSDKPlatformOnline ? @"1-20113-576fe88462af94a8f0867fabdb09c2c2-ios" : @"1-2-2d22fedfde75367e4a7fa0a9d2f10ba1-ios";
NSString * const SECRET    = kPlatform == YYPushSDKPlatformOnline ? @"2e4b26434900f6c1bb06f4e478a3f4a3" : @"bc6b9387ef8d306cdf16d6f29d40d3a1";

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    // 初始化推送
    [SHPushSDK startWithClientID:CLIENT_ID secret:SECRET platform:kPlatform];
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
    NSError *err = nil;
    [[SHPushSDK sharedInstance] wchatSetUnreadNumber:application.applicationIconBadgeNumber withTag:0 error:&err];
    if (err) {
        NSLog(@"%s,error:%@", __FUNCTION__, err.localizedDescription);
    }
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

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    // 推送
    self.deviceToken = [SHPushSDK trimDeviceToken:deviceToken];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    NSLog(@"fun:%s, %@", __func__, error.description);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    
}


@end
