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




### iOS 手动安装




###示例

"$JPUSH_PLUGIN_DIR/example"文件夹内找到并拷贝以下文件

		src/example/index.html to www/index.html
		src/example/css/* to www/css
		src/example/js/* to www/js

###关于'PhoneGap build'云服务

该项目基于 cordova 实现，目前无法使用 'PhoneGap build' 云服务进行打包，建议使用本地环境进行打包

## API 说明

iOS、Android [完整 API 详细说明](API/API.md)

插件的 API 集中在 jpush.js 文件中,该文件的具体位置如下：

Android:

	[Project]/assets/www/plugins/cn.jpush.phonegap.JPushPlugin/www

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
