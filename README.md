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

<!--![01](https://raw.githubusercontent.com/Yasashi/Yasashi.github.io/master/images/resource/blog01/01.png)-->
<img src="https://raw.githubusercontent.com/Yasashi/Yasashi.github.io/master/images/resource/blog01/01.png" width="700px"></img>

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


### 详细 API 请参考：

[JPush Hbuilder demo API](https://github.com/jpush/jpush-hbuilder-demo/blob/dev/API/API.md)


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
