# iOS远程推送
本文主要介绍了使用 游云 Push 时，何时会收到远程推送、如何使用远程推送、如何获取远程推送的内容。
# 何时会收到远程推送
## 1. 游云 SDK 运行状态
游云 SDK 根据 iOS App 运行的特性，主要有以下三种运行状态：

-  前台状态 如字面意思，App 前台可见时 SDK 处于前台状态。此时 App 使用游云的 `socket` 来收发消息。

-  后台活动状态 当 App 进入后台三分钟之内，SDK 处于后台活跃状态。此时 App 使用游云的 `socket` 接收消息。此时 SDK 不会弹出本地通知，如果您需要可以自己弹出本地通知提示。

-  后台暂停状态 当 App 进入后台两分钟之后或被杀进程或被冻结，SDK 将处于后台暂停状态。此时游云的 `socket` 会断开，融云 Server 会通过 `APNs` 将消息以远程推送的形式下发到客户端。 此状态下如果有人给该用户发送消息，游云的服务器会根据 `deviceToken` 和推送证书将消息发送到苹果推送服务器，苹果服务器会将该消息推送到客户端。

## 2. 消息远程推送的流程
当 SDK 处于后台暂停状态时，并且已经上传了 `deviceToken` ，这时候如果有人给该用户发送消息，融云的服务器会根据 deviceToken 和推送证书将消息发送到苹果推送服务器（`APNs`）。 苹果推送服务器会根据 `deviceToken` 查找相应的设备，并根据推送证书中的 `BundleID` 和 App 打包时使用的 `Provisioning Profile` 查找 App，从而确定唯一的设备上的唯一 App，并进行远程推送。

## 3. 开发环境与生产环境
苹果推送服务器区分开发环境（`Development`）和生产环境（`Production`），两个环境的服务器不同，可以使用相同的 P12 证书。

有几点需要注意的。

 - `deviceToken` 是唯一标识客户端的凭证，所以必须上传应用服务器才能使用远程推送。
 - 模拟器收不到远程推送。
 - 越狱的设备 `APNs` 服务不能保证，所以不一定能收到远程推送。
 - `APNs` 使用 `BundleID` 区分 App，使用通配符 `BundleID` 的 App 将无法使用远程推送。

# 如何使用远程推送
1. 开启 Push Notification
 您可以新建一个 `AppID`，或者在您原有的 `AppID` 上增加 `Push Notification` 的 Service。 需要注意的是，您 App 的 `BundleID` 不能使用通配符，否则将无法使用远程推送服务。
- 新建AppID，进入[App IDs](https://developer.apple.com/account/ios/identifier/bundle)页面，选择右上角`+`按钮，添加。
![新建AppID](http://img.blog.csdn.net/20160428172841965)
开启Push Notification。点击`Continue`。
![开启Push Service](http://img.blog.csdn.net/20160428172754871)
- 设置已经存在的AppID开启Push Notification。按下图选中，点击`Done `
![开启Push](http://img.blog.csdn.net/20160503164353278)

2. 生成证书
- 打开Mac中`钥匙串访问`软件，从证书颁发机构请求证书...
![请求证书](http://img.blog.csdn.net/20160429114657705)
- 输入邮箱，名称，选择存储到磁盘，选择`继续`，会生成一个`.certSigningRequest`文件，继续，存储到磁盘。
![输入信息](http://img.blog.csdn.net/20160429114917193)

3. 登录 [Apple Developer](https://developer.apple.com/account)，选择`Certificates,Identifiers & Profiles`。
![选择Certificates](http://img.blog.csdn.net/20160428165857709)
- 选择 `Certificates`，选择`All`，选择下图中的新建。
![添加证书](http://img.blog.csdn.net/20160429113250874)
- 选择 `Production` 栏下的 `Apple Push Notification service SSL (Sandbox & Production)` 勾选后，点击下一步。
![选中Push](http://img.blog.csdn.net/20160429113728505)
- 选择 `continue`，选择`App ID`，选择`continue`。
![选择App ID](http://img.blog.csdn.net/20160429141213263)
- 进入导入证书页面，点击`Choose File...`，选择步骤1中保存的`.certSigningRequest`文件。
![导入证书](http://img.blog.csdn.net/20160429114344701)
- 点击 `Continue`生成证书。点击`Download`下载到本地，双击安装。此证书便可以测试开发环境和生产环境。
![下载到本地](http://img.blog.csdn.net/20160429115521279)

- 双击打开证书，证书打开时会启动钥匙串访问工具。 在钥匙串访问工具中，你的证书会显示在 “证书” 中，注意选择左下角的 “证书” 和左上角 “登录”。 选中对应的证书，右键选择导出。保存 P12 文件时，可以为其设置密码，也可以不设置密码。
 ![本地证书](http://img.blog.csdn.net/20160429141233795)
4. 验证刚刚的设置
进入[证书页面](https://developer.apple.com/account/ios/certificate)，选择`开启Push Notification`的`App ID`，在`Distribution`列下的Push Notification 显示绿色的`Enabled`表示设置成功。如下图：
![设置成功](http://img.blog.csdn.net/20160503165046857)
 
5. 开发者平台上传证书

6. 代码调用推送


### 参考
1. [Configuring Push Notifications](https://developer.apple.com/library/ios/documentation/IDEs/Conceptual/AppDistributionGuide/AddingCapabilities/AddingCapabilities.html#//apple_ref/doc/uid/TP40012582-CH26-SW11)
2. [APNs Provider API](https://developer.apple.com/library/ios/documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/APNsProviderAPI.html)
