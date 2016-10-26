[![JPush Dev logo](http://community.jpush.cn/uploads/default/original/1X/a1dbd54304178079e65cdc36810fdf528fdebe24.png)](http://community.jpush.cn/)

# JPush HBuilder Demo

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/jpush/jpush-hbuilder-demo)
[![release](https://img.shields.io/badge/release-1.0.0-blue.svg)](https://github.com/jpush/jpush-hbuilder-demo/releases)
[![platforms](https://img.shields.io/badge/platforms-iOS%7CAndroid-lightgrey.svg)](https://github.com/jpush/jpush-hbuilder-demo)
[![weibo](https://img.shields.io/badge/weibo-JPush-blue.svg)](http://weibo.com/jpush?refer_flag=1001030101_&is_all=1)

[极光推送官方](https://www.jpush.cn/) 提供的 JPush HBuilder Demo。是基于 HBuilder 提供的 [第三方插件架构](http://ask.dcloud.net.cn/docs/#http://ask.dcloud.net.cn/article/66) 进而开发出的推送插件，并集成到 iOS/Android 工程里的 demo。开发者可以通过我们提供的 [安装方式](#install) 将推送功能集成到自己的项目中，从而在 JS 层实现对推送的控制。

<!--JPush 官方支持的 HBuilder Demo(基于 [DCloud](http://dev.dcloud.net.cn/mui/) HTML5+ 官方 Demo)，支持 iOS, Android。
-->

## 功能特性
+ 发送推送通知。
+ 发送推送自定义消息。
+ 设置推送标签和别名。
+ 设置角标（iOS）。


<h2 id="install">安装</h2>

可以将 Demo 直接导入 Android Studio 或 Xcode 运行，如果想要在自己的项目中集成 JPush，可以参考以下步骤：

### Android 手动安装
 - 将 Demo/android 项目中的 /libs/jpush-android-x.x.x.jar 和 x86 等文件夹下的 libjpushXXX.so 拷贝到对应文件中。
 - 拷贝 /src/io.dcloud.feature.jPush 文件夹。
 - 拷贝 /assets/apps/H51423BFB/js/jpush.js。
 - 修改 /src/io.dcloud/RInformation.java 文件中的包名。
 - 按照 Demo 中的 AndroidManifest.xml ，添加需要的权限和组件，替换包名和 JPush_APPKEY。
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
- 配置 manifest.json ，首先用源码的方式打开工程 /Pandora/ 目录下的 manifest.json ，在"permissions"中添加新的插件名称：

        "permissions": {
            "Push": {
        		"description": "极光推送插件"
        	}
        }

- 配置 feature.plist ，在 Xcode 中打开 ../PandoraApi.bundle/ 目录下的 feature.plist ，为插件添加新的 item：

	![feature.plist](iOS/doc_res/res_01.jpg)

- 将 JPush_Support 文件夹中所有内容在 Xcode 中拖到自己的工程里

- 在 JPush_Support/PushConfig.plist 中配置 APP_KEY 、 PRODUCTION（0开发 / 1发布）、IDFA（是否需要通过广告标识符启动 sdk）

- 打开 xcode，点击工程目录中顶部的 工程，选择(Target -> Build Phases -> Link Binary With Libraries)，添加以下框架：

		CFNetwork.framework
		CoreFoundation.framework
		CoreTelephony.framework
		SystemConfiguration.framework
		CoreGraphics.framework
		Foundation.framework
		UIKit.framework
		AdSupport.framework
		libz.tbd(若存在 libz.dylib 则替换为 libz.tbd)

## API 说明

iOS、Android 详细 API 文档请参阅 [JPush Hbuilder demo API 文档](API.md)。

插件的 API 集中在 jpush.js 文件中,该文件的具体位置如下：

Android:

	[Project]/android/assets/apps/H51423BFB/www/js/jpush.js

iOS:

	[Project]/iOS/HBuilder-Hello_jpush/HBuilder-Hello/Pandora/apps/HelloH5/www/js/jpush.js


## 常见问题

### 1. Android

	eclipse 中工程 import 之后出现："Type CallbackContext cannot be resolved to a type"。
	解决方案：eclipse 中右键单击工程名，
        Build Path -> Config Build Path -> Projects -> 选中工程名称 -> CordovaLib -> 点击 add

### 2. iOS

- 收不到推送:

	请首先按照正确方式再次配置证书、描述文件 
	[iOS 证书设置指南](http://docs.jpush.io/client/ios_tutorials/#ios_1)

- 设置 PushConfig.plist：

	- APP_KEY：应用标识 
	- CHANNEL：渠道标识
	- IsProduction：是否生产环境
	- IsIDFA：是否使用 IDFA 启动 sdk


### 更多

 [JPush 官网文档](http://docs.jpush.io/)
