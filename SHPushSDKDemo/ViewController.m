//
//  ViewController.m
//  SHPushSDK
//
//  Created by Frederic on 15/12/25.
//  Copyright © 2015年 Sina ShiHui. All rights reserved.
//

#import "ViewController.h"
#import "AppDelegate.h"

#import "SHPushSDK.h"


NSString * const yyTableViewCellIdentifer = @"yyTableViewCellIdentifer";

@interface ViewController ()<SHPushSDKDelegate>

@property (nonatomic, strong) UITextField *toUIDTextField;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [[SHPushSDK sharedInstance] setDelegate:self];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - UITableView
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 50.0;
}
- (NSInteger )numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 2;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:yyTableViewCellIdentifer];
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:yyTableViewCellIdentifer];
    }
    if (indexPath.row == 0) {
        // 设置推送时间
        [cell.textLabel setText:@"设置推送时间:8~24"];
    } else if (indexPath.row == 1) {
        // 取消推送
        [cell.textLabel setText:@"取消推送"];
    }
    return cell;
}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    if (indexPath.row == 0) {
        if ([SHPushSDK sharedInstance].isAuth) {
            [self settingPushTime];
        }
    } else if (indexPath.row == 1) {
        if ([SHPushSDK sharedInstance].isAuth) {
            [self unRegisterPush];
        }
    }
}

- (void)settingPushTime {
    AppDelegate *sharedDelegate = [UIApplication sharedApplication].delegate;
    [[SHPushSDK sharedInstance] deviceRegisterPush:sharedDelegate.deviceToken pushStartTime:8 endTime:24 completionHandler:^(BOOL isRegister, NSError *requestError) {
        if (isRegister) {
            [[SHPushSDK sharedInstance] deviceInfoWithCompletionHandler:^(NSDictionary *deviceInfo, NSError *requestError) {
                NSLog(@"register notification info :%@", deviceInfo);
                UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"推送" message:@"注册推送成功" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
                [alertView show];
            }];
        }
    }];
}

- (void)unRegisterPush {
    [[SHPushSDK sharedInstance]deviceUnRegisterPush:^(BOOL isUnRegister, NSError *requestError) {
        if (isUnRegister) {
            UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"推送" message:@"取消推送成功" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alertView show];
        }
    }];
}

#pragma mark -SHPushSDKDelegate

- (void)onwchatAuth:(SHPushSDK *)instance userinfo:(NSDictionary *)userinfo withError:(NSError *)error {
    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        // update UI
        NSString *userID = [userinfo objectForKey:@"id"];
        if (userID) {
            [weakSelf setTitle:userID];
        } else {
            UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"authError" message:error.localizedDescription delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
            [alert show];
        }
    });
    
}

- (void)onRecvMsg:(SHPushSDK *)instance
    withMessageId:(NSString *)messageId
          fromUid:(NSString *)fromUid
            toUid:(NSString *)toUid
             time:(NSInteger)timevalue
          content:(NSData *)content
          extBody:(NSData *)extContent
        withError:(NSError *)error {
    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        // update UI
        NSLog(@"onRecvMsg:messageId:%@, fromUid: %@, toUid: %@,time: %d, content:%@, extContent:%@",messageId, fromUid, toUid, (int)timevalue, [[NSString alloc] initWithData:content encoding:NSUTF8StringEncoding], [[NSString alloc] initWithData:extContent encoding:NSUTF8StringEncoding]);
            // 文本消息
        UIAlertView *noticeTextAlertView = [[UIAlertView alloc]initWithTitle:@"notice" message:[NSString stringWithFormat:@"%@, from user: %@", [[NSString alloc] initWithData:content encoding:NSUTF8StringEncoding], fromUid] delegate:weakSelf cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        [noticeTextAlertView show];
    });
}

@end
