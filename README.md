![JPush Dev logo](http://community.jpush.cn/uploads/default/original/1X/a1dbd54304178079e65cdc36810fdf528fdebe24.png)

## JPush HBuilder Demo ##

JPush 官方支持的 HBuilder Demo，支持 iOS, Android


### 功能特性
+ 发送推送通知
+ 发送推送自定义消息
+ 设置推送标签和别名
+ 设置角标（iOS）


## 安装 ##

### Android 手动安装
如果想要在自己的项目中集成 JPush，可以按照以下步骤：
 - 将 Demo 项目中的 /libs/jpush-android-x.x.x.jar 和 x86 等文件夹下的 libjpushXXX.so 拷贝到对应文件中
 - 拷贝 /src/io.dcloud.feature.jPush 文件夹
 - 拷贝 /assets/apps/H51423BFB/js/jpush.js
 - 修改 /src/io.dcloud/RInformation.java 文件中的包名
 - 按照 Demo 中的 AndroidManifest.xml ，添加需要的权限和组件，替换包名和 JPush_APPKEY
 - 在 /assets/apps/[yourAppName]/www/manifest.json 文件中添加：

        "Push": {
            "description": "消息推送"
        }
 - 在 /assets/data/properties.xml 中添加：

        <feature
            name="Push"
            value="io.dcloud.feature.jPush.JPushService" >
        </feature>


### iOS 手动安装
- 配置 manifest.json ，首先用源码的方式打开工程 /Pandora/ 目录下的 manifest.json ，在 "permissions" 中添加新的插件名称：
	
```json
 "permissions": {
	"Push":{
		"description": "极光推送插件"
	}
},
```

 
- 配置 feature.plist ，在 Xcode 中打开 /PandoraApi.bundle/ 目录下的 feature.plist ，为插件添加新的 item：


- 将 JPush_Support 文件夹中所有内容在 Xcode 中拖到自己的工程里

- 在 JPush_Support/PushConfig.plist 中配置 APP_KEY 和 PRODUCTION （0开发/1发布）

- 添加必要框架，打开 xcode，点击 project，选择(Targets -> Build Phases -> Link Binary With Libraries)

<!--![01](https://raw.githubusercontent.com/jpush/jpush-hbuilder-demo/dev/iOS/HBuilder-Hello_jpush/HBuilder-Hello/img/01.png)-->
<img src="https://raw.githubusercontent.com/jpush/jpush-hbuilder-demo/dev/iOS/HBuilder-Hello_jpush/HBuilder-Hello/img/01.png" width="700px"></img>

## API 说明

iOS、Android [完整 API 详细说明](API/API.md)

插件的 API 集中在 jpush.js 文件中,该文件的具体位置如下：

Android:

	[Project]/android/assets/apps/H51423BFB/www/js/jpush.js

iOS:

	.../jpush-hbuilder-demo/iOS/HBuilder-Hello_jpush/HBuilder-Hello/Pandora/apps/HelloH5/www/js/jpush.js


###API 简介 请参考下面：

#### iOS 和 Android 通用 API 简介

+ 停止与恢复推送服务

		window.plus.Push.init()
		window.plus.Push.stopPush()
		window.plus.Push.resumePush()
		window.plus.Push.isPushStopped(callback)

+ 获取 RegistrationID

		window.plus.Push.getRegistrationID(callback)

+ 别名与标签

		window.plus.Push.setTagsWithAlias(tags, alias)
		window.plus.Push.setTags(tags)
		window.plus.Push.setAlias(alias)

	 	event - jpush.setTagsWithAlias //三个方法都是触发该回调事件

+ 获取点击通知内容

		event - jpush.openNotification

+ 获取通知内容

		event - jpush.receiveNotification

+ 获取自定义消息推送内容

		event - jpush.receiveMessage



#### iOS API简介

+ 获取自定义消息推送内容

		//推荐使用事件的方式传递，但同时保留了 receiveMessageIniOSCallback 的回调函数，兼容以前的代码
		window.plus.Push.receiveMessageIniOSCallback(data)

		event - jpush.receiveMessage

+ 页面的统计

		window.plus.Push.startLogPageView(pageName)
		window.plus.Push.stopLogPageView(pageName)
		window.plus.Push.beginLogPageView(pageName, duration)

+ 设置 Badge

		window.plus.Push.setBadge(value)
		window.plus.Push.resetBadge()
		window.plus.Push.setApplicationIconBadgeNumber(badge)
		window.plus.Push.getApplicationIconBadgeNumber(callback)

+ 本地通知

		window.plus.Push.addLocalNotificationForIOS(delayTime, content,
			badge, notificationID, extras)
		window.plus.Push.deleteLocalNotificationWithIdentifierKeyInIOS()
		window.plus.Push.clearAllLocalNotifications()

+ 日志等级设置

		window.plus.Push.setDebugModeFromIos()
		window.plus.Push.setLogOFF()
		window.plus.Push.setCrashLogON()

+ 地理位置上报

		window.plus.Push.setLocation(latitude, longitude)


#### Android API简介

+ 获取集成日志

		window.plus.Push.setDebugMode(mode)

+ 接收推送消息和点击通知

		//下面这两个 API 是兼容旧有的代码
		window.plus.Push.receiveMessageInAndroidCallback(data)
		window.plus.Push.openNotificationInAndroidCallback(data)

+ 统计分析

		window.plus.Push.setStatisticsOpen(boolean)

	或在 MainActivity 中的 onPause() 和 onResume() 方法中分别调用
	JPushInterface.onPause(this) 和 JPushInterface.onResume(this) 来启用统计分析功能,
	如果使用这种方式启用统计分析功能，则 window.plus.Push.setStatisticsOpen(boolean)
	方法不再有效，建议不要同时使用。

+ 清除通知

		window.plus.Push.clearAllNotification()

+ 通知栏样式定制

		window.plus.Push.setBasicPushNotificationBuilder = function()
		window.plus.Push.setCustomPushNotificationBuilder = function()

+ 设置保留最近通知条数

		window.plus.Push.setLatestNotificationNum(num)

+ 本地通知

		window.plus.Push.addLocalNotification(builderId, content,
				title, notificaitonID, broadcastTime, extras)
		window.plus.Push.removeLocalNotification(notificationID)
		window.plus.Push.clearLocalNotifications()


###常见问题

####1. Android

	eclipse 中 PhoneGap 工程 import 之后出现：`Type CallbackContext cannot be resolved to a type`
	解决方案：eclipse 中右键单击工程名，Build Path -> Config Build Path -> Projects -> 选中工程名称 -> CordovaLib -> 点击 add

####2. iOS 设置 / 修改 APP_KEY

    在 PushConfig.plist 中修改。
	PushConfig.plist 其他值说明：
    	CHANNEL: 渠道标识
    	IsProduction: 是否生产环境（暂未启用）


###更多
 [JPush 官网文档](http://docs.jpush.io/)
