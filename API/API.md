#JPushPlugin API
![JPush Dev logo](http://community.jpush.cn/uploads/default/original/1X/a1dbd54304178079e65cdc36810fdf528fdebe24.png)

#通用API说明

##获取 RegistrationID API

### API - getRegistrationID

RegistrationID 定义

集成了 JPush SDK 的应用程序在第一次成功注册到 JPush 服务器时，JPush 服务器会给客户端返回一个唯一的该设备的标识 - RegistrationID。JPush SDK 会以广播的形式发送 RegistrationID 到应用程序。

应用程序可以把此 RegistrationID 保存以自己的应用服务器上，然后就可以根据 RegistrationID 来向设备推送消息或者通知。

#### 接口定义

	window.plus.Push.getRegistrationID = function(callback)

##### 参数说明
无
#### 返回值

调用此 API 来取得应用程序对应的 RegistrationID。 只有当应用程序成功注册到 JPush 的服务器时才返回对应的值，否则返回空字符串。

#### 调用示例

 	window.plus.Push.getRegistrationID(onGetRegistradionID);
	var onGetRegistradionID = function(data) {
		try{
			console.log("JPushPlugin:registrationID is "+data)		}
		catch(exception){
			console.log(exception);
		}
	}

##别名与标签 API

### API - setTagsWithAlias,setTags,setAlias

提供几个相关 API 用来设置别名（alias）与标签（tags）。

这几个 API 可以在 App 里任何地方调用。

别名 alias

为安装了应用程序的用户，取个别名来标识。以后给该用户 Push 消息时，就可以用此别名来指定。

每个用户只能指定一个别名。

同一个应用程序内，对不同的用户，建议取不同的别名。这样，尽可能根据别名来唯一确定用户。

系统不限定一个别名只能指定一个用户。如果一个别名被指定到了多个用户，当给指定这个别名发消息时，服务器端API会同时给这多个用户发送消息。

举例：在一个用户要登录的游戏中，可能设置别名为 userid。游戏运营时，发现该用户 3 天没有玩游戏了，则根据 userid 调用服务器端API发通知到客户端提醒用户。

标签 tag

为安装了应用程序的用户，打上标签。其目的主要是方便开发者根据标签，来批量下发 Push 消息。

可为每个用户打多个标签。

不同应用程序、不同的用户，可以打同样的标签。

举例： game, old_page, women

#### 接口定义

	window.plus.Push.setTagsWithAlias = function(tags,alias)
	window.plus.Push.setTags = function(tags)
	window.plus.Push.setAlias = function(alias)

####使用平台
android iOS


#### 参数说明
* tags
	* 参数类型为数组	
	* nil 此次调用不设置此值
	* 空集合表示取消之前的设置 
	* 每次调用至少设置一个 tag，覆盖之前的设置，不是新增
	* 有效的标签组成：字母（区分大小写）、数字、下划线、汉字
	* 限制：每个 tag 命名长度限制为 40 字节，最多支持设置 100 个 tag，但总长度不得超过1K字节。（判断长度需采用UTF-8编码）
	* 单个设备最多支持设置 100 个 tag。App 全局 tag 数量无限制。
* alias 
	* 参数类型为字符串
	* nil 此次调用不设置此值
	* 空字符串 （""）表示取消之前的设置
	* 有效的别名组成：字母（区分大小写）、数字、下划线、汉字。
	* 限制：alias 命名长度限制为 40 字节。（判断长度需采用UTF-8编码）
	
#### 返回值说明

函数本身无返回值，但需要注册`jpush.setTagsWithAlias	`事件来监听设置结果
	
	document.addEventListener("jpush.setTagsWithAlias", onTagsWithAlias, false);
    var onTagsWithAlias = function(event){
       try{
           console.log("onTagsWithAlias");    
           var result="result code:"+event.resultCode+" ";
           result+="tags:"+event.tags+" ";
           result+="alias:"+event.alias+" ";
           $("#tagAliasResult").html(result);
       }
       catch(exception){
           console.log(exception)
       }
   }

####错误码定义



|Code|描述                   		       |详细解释 |
|----|:----------------------------------------|:--------|
|6001|无效的设置，tag/alias 不应参数都为 null  |   	 |
|6002|设置超时			       	       |建议重试|
|6003|alias 字符串不合法	               |有效的别名、标签组成：字母（区分大小写）、数字、下划线、汉字。|
|6004|alias超长。			       |最多 40个字节	中文 UTF-8 是 3 个字节|
|6005|某一个 tag 字符串不合法		       |有效的别名、标签组成：字母（区分大小写）、数字、下划线、汉字。|
|6006|某一个 tag 超长			       |一个 tag 最多 40个字节	中文 UTF-8 是 3 个字节|
|6007|tags 数量超出限制。最多 100个	       |这是一台设备的限制。一个应用全局的标签数量无限制。|
|6008|tag/alias 超出总长度限制	       	       |总长度最多 1K 字节|
|6011|10s内设置tag或alias大于3次	       |短时间内操作过于频繁|


## 获取通知内容

### 获取点击通知内容

#### event - jpush.openNotification

点击通知进入应用程序时会出发该事件

#####代码示例

- 在你需要接收通知的的js文件中加入:
	           
		document.addEventListener("jpush.openNotification", onOpenNotification, false);

- onOpenNotification需要这样写：
		
		
                    var alertContent
                    if(device.platform == "Android"){
                        alertContent=window.plus.Push.openNotification.alert;
                    }else{
                        alertContent   = event.aps.alert;
                    }
                    alert("open Notificaiton:"+alertContent);

ps：点击通知后传递的json object 保存在window.plus.Push.openNotification，直接访问即可，字段示例，根据实际推送情况，可能略有差别，请注意
	
+ android

		{"alert":"ding",
		"extras":{
			     "cn.jpush.android.MSG_ID":"1691785879",
			     "app":"com.thi.pushtest",
			     "cn.jpush.android.ALERT":"ding",
			     "cn.jpush.android.EXTRA":{},
			     "cn.jpush.android.PUSH_ID":"1691785879",
			     "cn.jpush.android.NOTIFICATION_ID":1691785879,
			     "cn.jpush.android.NOTIFICATION_TYPE":"0"}}
		
+ iOS 

		{
		"aps":{
			  "badge":1,
			  "sound":"default",
			  "alert":"今天去哪儿",
			  "content-available" = 1//未设置则没有,用于iOS7以上remote notification
			  },
		"_j_msgid":154604475,
		"key1":"value1"//未设置extra则没有相应键值
		}

### 获取通知内容

#### event - jpush.receiveNotification

app处于前台收到通知会触发该事件

#####代码示例

- 在你需要接收通知的的js文件中加入:
	           
		document.addEventListener("jpush.receiveNotification", onReceiveNotification, false);

- onReceiveNotification需要这样写：
		
		
                    var alertContent
                    if(device.platform == "Android"){
                        alertContent=window.plus.Push.receiveNotification.alert;
                    }else{
                        alertContent   = event.aps.alert;
                    }
                    alert("open Notificaiton:"+alertContent);

ps：点击通知后传递的json object 保存在window.plus.Push.receiveNotification，直接访问即可，字段示例，根据实际推送情况，可能略有差别，请注意
	
+ android

		{"alert":"ding",
		"extras":{
			     "cn.jpush.android.MSG_ID":"1691785879",
			     "app":"com.thi.pushtest",
			     "cn.jpush.android.ALERT":"ding",
			     "cn.jpush.android.EXTRA":{},
			     "cn.jpush.android.PUSH_ID":"1691785879",
			     "cn.jpush.android.NOTIFICATION_ID":1691785879,
			     "cn.jpush.android.NOTIFICATION_TYPE":"0"}}
		
+ iOS 

		{
		"aps":{
			  "badge":1,
			  "sound":"default",
			  "alert":"今天去哪儿",
			  "content-available" = 1//未设置则没有,用于iOS7以上remote notification
			  },
		"_j_msgid":154604475,
		"key1":"value1"//未设置extra则没有相应键值
		}


### 获取自定义消息推送内容


####event - jpush.receiveMessage

收到应用内消息时触发这个事件

`推荐使用事件的方式传递，但同时保留了receiveMessageIniOSCallback的回调函数，兼容以前的代码`


#####代码示例

- 在你需要接收通知的的js文件中加入:
	           
		document.addEventListener("jpush.receiveMessage", onReceiveMessage, false);

- onReceiveMessage需要这样写：
		
		
            var onReceiveMessage = function(event){
                try{
                    var message
                    if(device.platform == "Android"){
                  		 message = window.plus.Push.receiveMessage.message;
                    }else{
                         message   = event.content;
                    }          
                     $("#messageResult").html(message);
                     
                }
                catch(exception){
                    console.log("JPushPlugin:onReceiveMessage-->"+exception);
                }
            }

ps：点击通知后传递的json object 保存在window.plus.Push.receiveMessage，直接访问即可，字段示例，根据实际推送情况，可能略有差别，请注意

+ android

		{"message":"今天去哪儿",
		"extras"{
				"cn.jpush.android.MSG_ID":"154378013",
				"cn.jpush.android.CONTENT_TYPE":"",
				"cn.jpush.android.EXTRA":{"key":"不添没有"}
				}
		}
+ iOS

		 {
		 "content":"今天去哪儿",
		 "extras":
		          {
		 		  "key":"不填写没有"
		 		  }
		 }
	



## 停止与恢复推送服务 API	
### API - init

调用此API,用来开启
JPush SDK 提供的推送服务。

开发者App可以通过调用停止推送服务API来停止极光推送服务。当又需要使用极光推送服务时，则必须要调用恢复推送服务 API。

```
本功能是一个完全本地的状态操作。也就是说：停止推送服务的状态不会保存到服务器上。

如果停止推送服务后，开发者App被重新安装，或者被清除数据，

JPush SDK 会恢复正常的默认行为。（因为保存在本地的状态数据被清除掉了）。 
本功能其行为类似于网络中断的效果，即：推送服务停止期间推送的消息，

恢复推送服务后，如果推送的消息还在保留的时长范围内，则客户端是会收到离线消息。
```

#### 接口定义

	window.plus.Push.init()

### API - stopPush
+ 在android平台 

	+ 开发者App可以通过调用停止推送服务API来停止极光推送服务。当又需要使用极光推送服务时，则必须要调用恢复推送服务 API。
	
	
	+ 调用了本 API 后，JPush 推送服务完全被停止。具体表现为：
		
		+ JPush Service 不在后台运行
		+ 收不到推送消息
		+ 不能通过 JPushInterface.init 恢复，需要调用resumePush恢复
		+ 极光推送所有的其他 API 调用都无效

+ iOS平台

	+ 不推荐调用，因为这个API只是让你的DeviceToken失效，在设置－通知 中您的应用程序没有任何变化
    + 推荐：设置一个UI界面， 提醒用户在在设置－通知关闭推送服务

### 接口定义 
    
    window.plus.Push.stopPush()
	

#### API - resumePush


恢复推送服务。调用了此 API 后

+ 在android平台 

	+ 极光推送完全恢复正常工作，

+ iOS平台
	
	+ 重新去APNS注册



##### 接口定义

	window.plus.Push.resumePush()

#### API - isPushStopped

+ 在android平台 

	+ 用来检查 Push Service 是否已经被停止

+ iOS平台
	
	+ 平台检查推送服务是否注册


##### 接口定义

	    window.plus.Push.isPushStopped(callback)


##### 参数说明

+ callback 回调函数，用来通知JPush的推送服务是否开启

####代码示例
	    window.plus.Push.resumePush(callback)
		var onCallback = function(data) {
		    if(data>0){
		    	//开启
		    }else{
		    	//关闭
		    }
		}


## adnroid API简介


### 获取集成日志

#### API - setDebugMode

用于开启调试模式，可以查看集成JPush过程中的log，如果集成失败，可方便定位问题所在

##### 接口定义

	window.plus.Push.setDebugMode (mode)

##### 参数说明

- mode的值

	- true  显示集成日志
	- false 不显示集成日志


###  接收消息和点击通知事件
#### API - receiveMessageInAndroidCallback

用于android收到应用内消息的回调函数(请注意和通知的区别)，该函数不需要主动调用

##### 接口定义

	window.plus.Push.receiveMessageInAndroidCallback = function(data)

##### 参数说明
- data 接收到的js字符串，包含的key:value请进入该函数体查看

##### 代码示例

#### API - openNotificationInAndroidCallback

当点击android手机的通知栏进入应用程序时,会调用这个函数，这个函数不需要主动调用，是作为回调函数来用的


##### 接口定义

	window.plus.Push.openNotificationInAndroidCallback = function(data)

##### 参数说明
- data js字符串

##### 代码示例

###  统计分析 API

#### API - onResume / onPause
这是一个 android local api，不是js的api，请注意
本 API 用于“用户使用时长”，“活跃用户”，“用户打开次数”的统计，并上报到服务器，在 Portal 上展示给开发者。



####接口定义

		public static void onResume(final Activity activity)
		public static void onPause(final Activity activity)
####参数说明

 ＋ Activity activity 当前所在的Activity。
####调用说明

应在所有的 Activity 的 onResume / onPause 方法里调用。

####代码示例

	@Override
	protected void onResume() {
	    super.onResume();
	    JPushInterface.onResume(this);
	}
	@Override
	protected void onPause() {
	    super.onPause();
	    JPushInterface.onPause(this);
	}

#### API - setStatisticsOpen(boolean)

用于在 js 中控制是否打开应用的统计分析功能，但如果已经添加了上面的 onResume/onPause 方法，
就不能再通过这个方法来控制统计分析功能了。

#### 接口定义

	window.plus.Push.setStatisticsOpen(boolean)

#### 参数说明
- boolean
	-true : 打开统计分析功能
	-false: 关闭统计分析功能

#### API - reportNotificationOpened

用于上报用户的通知栏被打开，或者用于上报用户自定义消息被展示等客户端需要统计的事件。


##### 接口定义

	window.plus.Push.reportNotificationOpened(msgID)

##### 参数说明
- msgID
	-收到的通知或者自定义消息的id 	


###  清除通知 API

#### API - clearAllNotification

推送通知到客户端时，由 JPush SDK 展现通知到通知栏上。

此 API 提供清除通知的功能，包括：清除所有 JPush 展现的通知（不包括非 JPush SDK 展现的）


##### 接口定义

	window.plus.Push.clearAllNotification = function()

###  设置允许推送时间 API
###  设置通知静默时间 API
###  通知栏样式定制 API

#### API - setBasicPushNotificationBuilder,setCustomPushNotificationBuilder

当用户需要定制默认的通知栏样式时，则可调用此方法。
极光 Push SDK 提供了 2 个用于定制通知栏样式的构建类：

- setBasicPushNotificationBuilder
	- Basic 用于定制 Android Notification 里的 defaults / flags / icon 等基础样式（行为）
- setCustomPushNotificationBuilder
	- 继承 Basic 进一步让开发者定制 Notification Layout

如果不调用此方法定制，则极光Push SDK 默认的通知栏样式是：Android标准的通知栏提示。

##### 接口定义

	window.plus.Push.setBasicPushNotificationBuilder = function()
	window.plus.Push.setCustomPushNotificationBuilder = function()


###  设置保留最近通知条数 API

#### API - setLatestNotificationNum

通过极光推送，推送了很多通知到客户端时，如果用户不去处理，就会有很多保留在那里。

新版本 SDK (v1.3.0) 增加此功能，限制保留的通知条数。默认为保留最近 5 条通知。

开发者可通过调用此 API 来定义为不同的数量。

##### 接口定义

	window.plus.Push.setLatestNotificationNum(num)

##### 参数说明

- num 保存的条数


###  本地通知API
#### API - addLocalNotification,removeLocalNotification,clearLocalNotifications


本地通知API不依赖于网络，无网条件下依旧可以触发

本地通知与网络推送的通知是相互独立的，不受保留最近通知条数上限的限制

本地通知的定时时间是自发送时算起的，不受中间关机等操作的影响


三个接口的功能分别为：添加一个本地通知，删除一个本地通知，删除所有的本地通知

#####接口定义

	window.plus.Push.addLocalNotification = function(builderId,
											    content,
												title,
												notificaitonID,
												broadcastTime,
												extras)
	window.plus.Push.removeLocalNotification = function(notificationID)
	window.plus.Push.clearLocalNotifications = function()

##### 参数说明

- builderId 设置本地通知样式
- content 设置本地通知的content
- title 设置本地通知的title
- notificaitonID 设置本地通知的ID
- broadcastTime 设置本地通知触发时间，为距离当前时间的数值，单位是毫秒
- extras 设置额外的数据信息extras为json字符串


## iOS API

### 设置Badge
#### API - setBadge, resetBadge

 JPush 封装 badge 功能，允许应用上传 badge 值至 JPush 服务器，由 JPush 后台帮助管理每个用户所对应的推送 badge 值，简化了设置推送 badge 的操作。
（本接口不会直接改变应用本地的角标值. 要修改本地 badege 值，使用 setApplicationIconBadgeNumber）

实际应用中，开发者可以直接对 badge 值做增减操作，无需自己维护用户与 badge 值之间的对应关系。
##### 接口定义

	window.plus.Push.setBadge(value)
	window.plus.Push.reSetBadge()

`resetBadge相当于setBadge(0)`
##### 参数说明
value 取值范围：[0,99999]
##### 返回值
无，控制台会有 log 打印设置结果
#####代码示例

	window.plus.Push.setBadge(5);
	window.plus.Push.reSetBadge();

#### API - setApplicationIconBadgeNumber

本接口直接改变应用本地的角标值.
设置 iOS 的角标，当设置 badge ＝ 0 时为清除角标

##### 接口定义

	window.plus.Push.setApplicationIconBadgeNumber(badge)

##### 参数说明

- badge 整形,例如0，1，2
- 当 badge 为 0 时，角标被清除

#####代码示例

	window.plus.Push.setApplicationIconBadgeNumber(0);


#### API - getApplicationIconBadgeNumber

获取 iOS 的角标值

##### 接口定义

	window.plus.Push.getApplicationIconBadgeNumber(callback)

##### 参数说明

- callback 回调函数

#####代码示例
```			    	
window.plus.Push.getApplicationIconBadgeNumber(function(data){
     console.log(data);               
 });

```

### 本地通知
#### API - addLocalNotificationForIOS

API 用于注册本地通知

最多支持64个

##### 接口定义

	window.plus.Push.addLocalNotificationForIOS(delayTime, content, badge, notificationID, extras)

##### 参数说明

- delayTime 本地推送延迟多长时间后显示，数值类型或纯数字的字符型均可
- content 本地推送需要显示的内容
- badge 角标的数字。如果不需要改变角标传-1。数值类型或纯数字的字符型均可
- notificationID 本地推送标识符,字符串。
- extras 自定义参数，可以用来标识推送和增加附加信息。字典类型。

#####代码示例

	window.plus.Push.addLocalNotificationForIOS(6*60*60, "本地推送内容", 1, "notiId", {"key":"value"});

#### API - deleteLocalNotificationWithIdentifierKeyInIOS

API 删除本地推送定义

##### 接口定义

	window.plus.Push.deleteLocalNotificationWithIdentifierKeyInIOS(identifierKey)

##### 参数说明

- identifierKey 本地推送标识符

#####代码示例

	window.plus.Push.deleteLocalNotificationWithIdentifierKeyInIOS("identifier");

#### API - clearAllLocalNotifications	

API 清除所有本地推送对象

##### 接口定义

	window.plus.Push.clearAllLocalNotifications()

#####代码示例

    window.plus.Push.clearAllLocalNotifications();

### 日志等级设置
#### API - setDebugModeFromIos
API 用于开启 Debug 模式，显示更多的日志信息

建议调试时开启这个选项，不调试的时候注释这句代码，这个函数 setLogOFF 是相反的一对
##### 接口定义

	window.plus.Push.setDebugModeFromIos()

#####代码示例

	window.plus.Push.setDebugModeFromIos();

#### API - setLogOFF

API 用来关闭日志信息（除了必要的错误信息）

不需要任何调试信息的时候，调用此 API（发布时建议调用此 API，用来屏蔽日志信息，节省性能消耗)

##### 接口定义

	window.plus.Push.setLogOFF()

#####代码示例

	window.plus.Push.setLogOFF();

#### API - setCrashLogON

API 用于统计用户应用崩溃日志

如果需要统计 Log 信息，调用该接口。当你需要自己收集错误信息时，切记不要调用该接口。


##### 接口定义

	window.plus.Push.setCrashLogON()

#####代码示例

	window.plus.Push.setCrashLogON();


### 页面的统计
#### API - startLogPageView, stopLogPageView, beginLogPageView

本 API 用于“用户指定页面使用时长”的统计，并上报到服务器，在 Portal 上展示给开发者。页面统计集成正确，才能够获取正确的页面访问路径、访问深度（PV）的数据。

##### 接口定义
	window.plus.Push.startLogPageView = function(pageName)
	window.plus.Push.stopLogPageView = function(pageName)
	window.plus.Push.beginLogPageView = function(pageName, duration)
#####参数说明
pageName 需要统计页面自定义名称

duration 自定义的页面时间
#####调用说明
应在所有的需要统计得页面得 viewWillAppear 和 viewWillDisappear 加入 startLogPageView 和 stopLogPageView 来统计当前页面的停留时间。

	或者直接使用 beginLogPageView 来自定义加入页面和时间信息。
#####返回值说明
无
#####代码示例

	window.plus.Push.beginLogPageView("newPage", 5);
	window.plus.Push.startLogPageView("onePage");
	window.plus.Push.stopLogPageView("onePage");



### 地理位置上报
#### API - setLocation
API 用于统计用户地理信息

##### 接口定义

window.plus.Push.setLocation(latitude,longitude)

##### 参数说明

- latitude 地理位置纬度，数值类型或纯数字的字符型均可
- longitude 地理位置精度，数值类型或纯数字的字符型均可

#####代码示例

window.plus.Push.setLocation(39.26,115.25);
 
