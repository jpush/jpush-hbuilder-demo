# JPush For HBuilder API

- [通用 API](#通用-api)
  - [获取 RegistrationID](#获取-registrationid)
  - [设置别名与标签](#设置别名与标签)
  - [获取点击通知内容](#获取点击通知内容)
  - [获取收到通知内容](#获取收到通知内容)
  - [获取自定义消息内容](#获取自定义消息内容)
  - [停止与恢复推送服务](#停止与恢复推送服务)
- [Android API](#android-api)
  - [获取集成日志](#获取集成日志)
  - [接收消息和点击通知事件](#接收消息和点击通知事件)
  - [清除通知](#清除通知)
  - [设置允许推送时间](#设置允许推送时间)
  - [设置通知静默时间](#设置通知静默时间)
  - [通知栏样式定制](#通知栏样式定制)
  - [设置保留最近通知条数](#设置保留最近通知条数)
  - [本地通知](#本地通知)
  - [获取点击通知内容缓存](#获取点击通知内容缓存)
  - [清除点击通知内容缓存](#清除点击通知内容缓存)
- [iOS API](#ios-api)
  - [设置 Badge](#设置-badge)
  - [本地通知](#本地通知)
  - [日志等级设置](#日志等级设置)
  - [页面统计](#页面统计)
  - [地理位置上报](#地理位置上报)

## 通用 API

### 获取 RegistrationID

#### API - getRegistrationID

RegistrationID 定义:集成了 JPush SDK 的应用程序在第一次成功注册到 JPush 服务器时，JPush 服务器会给客户端返回一个唯一的该设备的标识 - RegistrationID。

JPush SDK 会以广播的形式发送 RegistrationID 到应用程序。
应用程序可以把此 RegistrationID 保存以自己的应用服务器上，然后就可以根据 RegistrationID 来向设备推送消息或者通知。

##### 接口定义

```js
window.plus.Push.getRegistrationID(callback)
```

##### 参数说明

- callback: 获取 RegistrationID 的成功回调方法。

##### 返回值

调用此 API 来取得应用程序对应的 RegistrationID，只有当应用程序成功注册到 JPush 的服务器时才返回对应的值，否则返回空字符串。

##### 调用示例

```js
var onGetRegistrationID = function(data) {
  try {
    console.log("JPushPlugin:registrationID is " + data);
  } catch(exception) {
    console.log(exception);
  }
}
window.plus.Push.getRegistrationID(onGetRegistrationID);
```

#### EVENT - jpush.onGetRegistrationId

在第一次注册到 JPush 服务器时会触发该事件，并返回当前设备的唯一标识 - Registration id。

##### 代码示例

```js
document.addEventListener('jpush.onGetRegistrationId', (rId) => {
  console.log('registration id: ' + rId)
}, false)
```

### 设置别名与标签

#### API - setTagsWithAlias, setTags, setAlias

提供几个相关 API 用来设置别名（alias）与 标签（tags）。

这几个 API 可以在 App 里任何地方调用。

**别名 - alias**:

为安装了应用程序的用户，取个别名来标识。以后给该用户推送消息时，就可以用此别名来指定。

每个用户只能指定一个别名。

同一个应用程序内，对不同的用户，建议取不同的别名。这样，尽可能根据别名来唯一确定用户。

系统不限定一个别名只能指定一个用户。如果一个别名被指定到了多个用户，当给指定这个别名发消息时，服务器端 API 会同时给这多个用户发送消息。

举例：在一个用户要登录的游戏中，可能设置别名为 userId。游戏运营时，发现该用户 3 天没有玩游戏了，则根据 userId 调用服务器端 API 发通知到客户端提醒用户。

**标签 - tag**:

为安装了应用程序的用户，打上标签。其目的主要是方便开发者根据标签，来批量下发推送消息。

可为每个用户打多个标签。

不同应用程序、不同的用户，可以打同样的标签。

举例： game, old_page, women

##### 接口定义

  window.plus.Push.setTagsWithAlias(tags, alias)
  window.plus.Push.setTags(tags)
  window.plus.Push.setAlias(alias)

##### 参数说明

- tags
  - 参数类型为数组。
  - nil 此次调用不设置此值。
  - 空集合表示取消之前的设置。
  - 每次调用至少设置一个 tag，覆盖之前的设置，不是新增。
  - 有效的标签组成：字母（区分大小写）、数字、下划线、汉字。
  - 限制：每个 tag 命名长度限制为 40 字节，最多支持设置 100 个 tag，但总长度不得超过1K字节（判断长度需采用 UTF-8 编码）。
  - 单个设备最多支持设置 100 个 tag，App 全局 tag 数量无限制。

- alias
  - 参数类型为字符串。
  - nil 此次调用不设置此值。
  - 空字符串 （""）表示取消之前的设置。
  - 有效的别名组成：字母（区分大小写）、数字、下划线、汉字。
  - 限制：alias 命名长度限制为 40 字节（判断长度需采用 UTF-8 编码）。

##### 返回值说明

函数本身无返回值，但需要注册 `jpush.setTagsWithAlias` 事件来监听设置结果。

```js
var onTagsWithAlias = function(event) {
  try {
    console.log("onTagsWithAlias");
    var result = "result code:" + event.resultCode + " ";
    result += "tags:" + event.tags + " ";
    result += "alias:" + event.alias + " ";
    $("#tagAliasResult").html(result);
  } catch(exception) {
    console.log(exception);
  }
}

document.addEventListener("jpush.setTagsWithAlias", onTagsWithAlias, false);
```

### 获取点击通知内容

#### event - jpush.openNotification

点击通知进入应用程序时会出发该事件 (在这个回调函数中直接使用 alert 可能会出现卡死情况，如果有需要使用 alert 的需求，可以使用 mui.alert 代替)。

##### 代码示例

在你需要接收通知的的 js 文件中加入:

```js
var onOpenNotification = function(event) {
  var alertContent;
  if(plus.os.name == "Android") {
      alertContent = window.plus.Push.openNotification.alert;
  } else {
      alertContent = event.aps.alert;
  }
  alert("Open notification:" + alertContent);
}

document.addEventListener("jpush.openNotification", onOpenNotification, false);
```

ps：点击通知后传递的 json 对象保存在 window.plus.Push.openNotification，直接访问即可。

字段示例，根据实际推送情况，可能略有差别，请注意。

- Android:

```json
{
  "title": "title",
  "alert": "ding",
  "extras": {
    "yourKey": "yourValue",
    "cn.jpush.android.MSG_ID":"1691785879",
    "app":"com.thi.pushtest",
    "cn.jpush.android.ALERT":"ding",
    "cn.jpush.android.EXTRA":{},
    "cn.jpush.android.PUSH_ID":"1691785879",
    "cn.jpush.android.NOTIFICATION_ID":1691785879,
    "cn.jpush.android.NOTIFICATION_TYPE":"0"
  }
}
```

- iOS:

```json
{
  "aps":{
    "badge":1,
    "sound":"default",
    "alert":"今天去哪儿",
    "content-available" = 1 // 未设置则没有（用于 iOS 7 以上 remote notification）。
    },
  "_j_msgid":154604475,
  "key1":"value1" // 未设置 extra 则没有相应键值。
}
```

### 获取收到通知内容

#### event - jpush.receiveNotification

App 处于前台收到通知会触发该事件 (在这个回调函数中直接使用 alert 可能会出现卡死情况，如果有需要使用 alert 的需求，可以使用 mui.alert 代替)。

##### 代码示例

在你需要接收通知的的 js 文件中加入:

```js
var onReceiveNotification = function(event) {
  var alertContent;
  if(plus.os.name == "Android") {
      alertContent=window.plus.Push.receiveNotification.alert;
  } else {
      alertContent = event.aps.alert;
  }
  alert("Open notification:" + alertContent);
}

document.addEventListener("jpush.receiveNotification", onReceiveNotification, false);
```

ps：点击通知后传递的 JSON 对象保存在 `window.plus.Push.receiveNotification`，直接访问即可。

字段示例，根据实际推送情况，可能略有差别，请注意。

- Android:

```json
{
  "title": "title",
  "alert": "alert",
  "extras": {
    "yourKey": "yourValue",
      "cn.jpush.android.MSG_ID":"1691785879",
      "app":"com.thi.pushtest",
      "cn.jpush.android.ALERT":"ding",
      "cn.jpush.android.EXTRA":{},
      "cn.jpush.android.PUSH_ID":"1691785879",
      "cn.jpush.android.NOTIFICATION_ID":1691785879,
      "cn.jpush.android.NOTIFICATION_TYPE":"0"
  }
}
​```json

- iOS:

​```json
{
  "aps":{
    "badge":1,
      "sound":"default",
      "alert":"今天去哪儿",
      "content-available" = 1 // 未设置则没有（用于 iOS 7 以上 remote notification）。
    },
  "_j_msgid":154604475,
  "key1":"value1" // 未设置 extra，则没有相应键值。
}
```

### 获取自定义消息内容

#### event - jpush.receiveMessage

收到自定义消息时触发这个事件 (在这个回调函数中直接使用 alert 可能会出现卡死情况，如果有需要使用 alert 的需求，可以使用 mui.alert 代替)。

推荐使用事件的方式传递，但同时保留了 `receiveMessageIniOSCallback` 的回调函数，兼容以前的代码。

##### 代码示例

在你需要接收通知的的 js 文件中加入:

```js
var onReceiveMessage = function(event) {
    try {
      var message;
      if(plus.os.name == "Android") {
        message = window.plus.Push.receiveMessage.message;
      } else {
        message = event.content;
      }
    } catch(exception) {
      console.log("JPushPlugin:onReceiveMessage-->" + exception);
    }
}

document.addEventListener("jpush.receiveMessage", onReceiveMessage, false);
```

ps：点击通知后传递的 JSON 对象保存在 `window.plus.Push.receiveMessage`，直接访问即可。

字段示例，根据实际推送情况，可能略有差别，请注意。

- Android:

```json
{
  "message":"今天去哪儿",
  "extras"{
    "cn.jpush.android.MSG_ID":"154378013",
    "cn.jpush.android.CONTENT_TYPE":"",
    "cn.jpush.android.EXTRA":{"key":"不添加没有"}
  }
}
```

- iOS:

```json
{
  "content":"今天去哪儿",
  "extras":{"key":"不添加没有"}
}
```

### 停止与恢复推送服务

#### API - init

调用此 API，用来开启 JPush SDK 提供的推送服务。

开发者 App 可以通过调用停止推送服务 API 来停止极光推送服务。当又需要使用极光推送服务时，则必须要调用恢复推送服务 API。

本功能是一个完全本地的状态操作。也就是说：停止推送服务的状态不会保存到服务器上。

如果停止推送服务后，开发者 App 被重新安装，或者被清除数据，JPush SDK 会恢复正常的默认行为（因为保存在本地的状态数据被清除掉了）。

本功能其行为类似于网络中断的效果，即：推送服务停止期间推送的消息，恢复推送服务后，如果推送的消息还在保留的时长范围内，则客户端是会收到离线消息。

##### 接口定义

window.plus.Push.init()

#### API - stopPush

- Android
  - 开发者 App 可以通过调用停止推送服务 API 来停止极光推送服务。当又需要使用极光推送服务时，则必须要调用恢复推送服务 API。
  - 调用了本 API 后，JPush 的行为如下：
    - JPush Service 不在后台运行；
    - 收不到推送消息；
    - 不能通过 init 方法恢复，需要调用 resumePush 恢复；
    - 极光推送所有的其他 API 调用都无效。

- iOS
  - 不推荐调用，因为这个 API 只是让你的 DeviceToken 失效，在「设置－通知」中您的应用程序没有任何变化。
  - 推荐：设置一个 UI 界面，提醒用户在「设置－通知」中关闭推送服务。

##### 接口定义

window.plus.Push.stopPush()

#### API - resumePush

恢复推送服务。调用了此 API 后的行为：

- Android
  - 极光推送完全恢复正常工作。

- iOS
  - 重新去 APNS 注册。

##### 接口定义

window.plus.Push.resumePush()

#### API - isPushStopped

- Android
  - 用来检查 Push Service 是否已经被停止。

- iOS
  - 平台检查推送服务是否注册。

##### 接口定义

window.plus.Push.isPushStopped(callback)

##### 参数说明

- callback: 回调函数，用来通知 JPush 的推送服务是否开启。

##### 代码示例

```js
var onCallback = function(data) {
    if(data > 0) {
      // 开启
    } else {
      // 关闭
    }
}

window.plus.Push.resumePush(callback);
```

## Android API

### 获取集成日志

#### API - setDebugMode

用于开启调试模式，可以查看集成 JPush 过程中的 log，如果集成失败，可方便定位问题所在。

##### 接口定义

window.plus.Push.setDebugMode(mode)

##### 参数说明

- mode:
  - true: 显示集成日志。
  - false: 不显示集成日志。

#### API - getConnectionState

获取推送连接状态。

##### 代码示例

```js
plus.Push.getConnectionState((state) => {
  if (state) {
    // 连接状态
  } else {
    // 断开状态
  }
})
```

#### API - reportNotificationOpened

用于上报用户的通知栏被打开，或者用于上报用户自定义消息被展示等客户端需要统计的事件。

##### 接口定义

window.plus.Push.reportNotificationOpened(msgID)

##### 参数说明

- msgID：收到的通知或者自定义消息的 id。

### 清除通知

#### API - clearAllNotification

推送通知到客户端时，由 JPush SDK 展现通知到通知栏上。

此 API 提供清除通知的功能，包括：清除所有 JPush 展现的通知（不包括非 JPush SDK 展现的通知）。

##### 接口定义

window.plus.Push.clearAllNotification()

##### 代码示例

window.plus.Push.clearAllNotification();

### 设置允许推送时间

#### API - setPushTime

默认情况下用户在任何时间都允许推送。即任何时候有推送下来，客户端都会收到，并展示。

开发者可以调用此 API 来设置允许推送的时间。

如果不在该时间段内收到消息，当前的行为是：推送到的通知会被扔掉。

##### 接口定义

window.plus.Push.setPushTime(weekDays, startHour, endHour)

##### 参数说明

- weekDays: 数组，0表示星期天，1表示星期一，以此类推（7 天制，数组里面的值范围为 0 到 6）。
  数组的值为 null，则任何时间都可以收到消息和通知，数组的 size 为 0，则表示任何时间都收不到消息和通知。
- startHour: 整形，允许推送的开始时间（24小时制：startHour的范围为 0 到 23）。
- endHour: 整形，允许推送的结束时间（24小时制：endHour的范围为0到23）。

##### 代码示例

```js
// 表示允许推送的时间为周一到周三的 10 点至 20 点
window.plus.Push.setPushTime([0, 1, 2], 10, 20);
```

### 设置通知静默时间

#### API - setSilenceTime

默认情况下用户在收到推送通知时，客户端可能会有震动，响铃等提示。
但用户在睡觉、开会等时间点希望为 "免打扰" 模式，也是静音时段的概念。

开发者可以调用此 API 来设置静音时段。如果在该时间段内收到消息，则：不会有铃声和震动。

##### 接口定义

window.plus.Push.setSilenceTime(startHour, startMinute, endHour, endMinute)

##### 参数定义

- startHour: 静音时段的开始时间 - 小时（24小时制，范围：0~23）。
- startMinute: 静音时段的开始时间 - 分钟（范围：0~59）。
- endHour: 静音时段的结束时间 - 小时（24小时制，范围：0~23）。
- endMinute: 静音时段的结束时间 - 分钟（范围：0~59）。

##### 代码示例

```js
// 设置通知静默时间为晚上 11 点至早上 8 点
window.plus.Push.setSilenceTime(23, 0, 8, 0);
```

### 通知栏样式定制

极光 Push SDK 提供了 2 个用于定制通知栏样式的构建类。

如果不调用这两个方法定制，则 JPush SDK 默认的通知栏样式是 Android 标准的通知栏样式。

通过修改 ../src/io/dcloud/feature/jPush/JPushService.java 中的代码进行定制。

#### API - setBasicPushNotificationBuilder

Basic 用于定制 Android Notification 里的 defaults / flags / icon 等基础样式（行为）。

##### 接口定义

window.plus.Push.setBasicPushNotificationBuilder()

#### API - setCustomPushNotificationBuilder

继承 Basic 进一步让开发者定制 Notification Layout。

##### 接口定义

window.plus.Push.setCustomPushNotificationBuilder()

### 设置保留最近通知条数

#### API - setLatestNotificationNum

通过极光推送，推送了很多通知到客户端时，如果用户不去处理，就会有很多保留在那里。

新版本 SDK (v1.3.0) 增加此功能，限制保留的通知条数。默认为保留最近 5 条通知。

开发者可通过调用此 API 来定义为不同的数量。

##### 接口定义

window.plus.Push.setLatestNotificationNum(num)

##### 参数说明

- num: 保存的条数。

### 本地通知

#### API - addLocalNotification, removeLocalNotification, clearLocalNotifications

本地通知API不依赖于网络，无网条件下依旧可以触发。

本地通知与网络推送的通知是相互独立的，不受保留最近通知条数上限的限制。

本地通知的定时时间是自发送时算起的，不受中间关机等操作的影响。

三个接口的功能分别为：添加一个本地通知，删除一个本地通知，删除所有的本地通知。

##### 接口定义

window.plus.Push.addLocalNotification(builderId, content, title, notificationID, broadcastTime, extras)

window.plus.Push.removeLocalNotification(notificationID)

window.plus.Push.clearLocalNotifications()

##### 参数说明

- builderId: 设置本地通知样式。
- content: 设置本地通知的content。
- title: 设置本地通知的title。
- notificationID: 设置本地通知的 ID。
- broadcastTime: 设置本地通知触发时间，为距离当前时间的数值，单位是毫秒。
- extras: 设置额外的数据信息 extras 为 json 字符串。

### 获取点击通知内容缓存

#### API - getLaunchAppCacheNotification

点击推送启动应用的时候原生会将该 notification 缓存起来，该方法用于获取缓存 notification

退出程序后收到推送通知，此时点击通知事件下发，应用若不能及时注册点击通知事件，可调用此方法获取点击内容。

##### 接口定义

```js
window.plus.Push.getLaunchAppCacheNotification(callback)
```

##### 参数说明

- callback: 获取缓存 notification 的成功回调方法。

##### 返回值

调用此 API 来取得应用程序缓存的notification内容,返回内容结构为[openNotification](#获取点击通知内容)

调用一次此方法后缓存清除，直到下一次点击推送后重新缓存notification。

##### 调用示例

```js
var onGetLaunchAppCacheNotification = function(data) {
  if (data.alert) {
  		alert("cache:"+data.alert)
	}
}
window.plus.Push.getRegistrationID(onGetLaunchAppCacheNotification);
```
### 清除点击通知内容缓存

#### API - clearLaunchAppCacheNotification

推送通知到客户端时，点击通知时会缓存通知内容，可以通过[getLaunchAppCacheNotification](#获取点击通知内容缓存)获取

此 API 提供清除点击通知缓存的功能。

##### 接口定义

window.plus.Push.clearLaunchAppCacheNotification()

##### 代码示例

window.plus.Push.clearLaunchAppCacheNotification();

## iOS API

### 设置 Badge

#### API - setBadge, resetBadge

JPush 封装 badge 功能，允许应用上传 badge 值至 JPush 服务器，由 JPush 后台帮助管理每个用户所对应的推送 badge 值，
简化了设置推送 badge 的操作（本接口不会直接改变应用本地的角标值. 要修改本地 badge 值，使用 `setApplicationIconBadgeNumber`）。

实际应用中，开发者可以直接对 badge 值做增减操作，无需自己维护用户与 badge 值之间的对应关系。

##### 接口定义

window.plus.Push.setBadge(value)
window.plus.Push.resetBadge()

resetBadge 相当于 setBadge(0)。

##### 参数说明

- value: 取值范围[0, 99999]。

####代码示例

```js
window.plus.Push.setBadge(5);

window.plus.Push.resetBadge();
```

#### API - setApplicationIconBadgeNumber

直接改变应用本地的角标值，当设置 badge ＝ 0 时为清除角标。

##### 接口定义

window.plus.Push.setApplicationIconBadgeNumber(badge)

##### 参数说明

- badge: 整形，当 badge 为 0 时，角标被清除。

##### 代码示例

```js
window.plus.Push.setApplicationIconBadgeNumber(0);
```

#### API - getApplicationIconBadgeNumber

获取 iOS 的角标值。

##### 接口定义

window.plus.Push.getApplicationIconBadgeNumber(callback)

##### 参数说明

- callback: 回调函数。

####代码示例

```js
window.plus.Push.getApplicationIconBadgeNumber(function(data) {
  console.log(data);
});
```

### 本地通知

#### API - addLocalNotificationIniOS

用于注册本地通知(最多支持64个)。

##### 接口定义

window.plus.Push.addLocalNotificationIniOS(delayTime, content, badge, notificationID, extras)

##### 参数说明

- delayTime: 本地推送延迟多长时间后显示，数值类型或纯数字的字符型均可。
- content: 本地推送需要显示的内容。
- badge: 角标的数字。如果不需要改变角标传-1。数值类型或纯数字的字符型均可。
- notificationID: 本地推送标识符（字符串）。
- extras: 自定义参数，可以用来标识推送和增加附加信息（字典类型）。

##### 代码示例

```js
window.plus.Push.addLocalNotificationIniOS(6*60*60, "本地推送内容", 1, "1", {"key":"value"});
```

#### API - deleteLocalNotificationWithIdentifierKeyInIOS

删除本地推送定义。

##### 接口定义

window.plus.Push.deleteLocalNotificationWithIdentifierKeyInIOS(identifierKey)

##### 参数说明

- identifierKey: 本地推送标识符。

##### 代码示例

```js
window.plus.Push.deleteLocalNotificationWithIdentifierKeyInIOS("identifier");
```

#### API - clearAllLocalNotifications

清除所有本地推送对象。

##### 接口定义

window.plus.Push.clearAllLocalNotifications()

##### 代码示例

```js
window.plus.Push.clearAllLocalNotifications();
```

### 日志等级设置

#### API - setDebugModeFromIos

用于开启 Debug 模式，显示更多的日志信息。

建议调试时开启这个选项，不调试的时候注释这句代码，这个函数 `setLogOFF` 是相反的一对。

##### 接口定义

window.plus.Push.setDebugModeFromIos()

##### 代码示例

```js
window.plus.Push.setDebugModeFromIos();
```

#### API - setLogOFF

用来关闭日志信息（除了必要的错误信息）。

不需要任何调试信息的时候，调用此 API（发布时建议调用此 API，用来屏蔽日志信息，节省性能消耗)。

##### 接口定义

window.plus.Push.setLogOFF()

##### 代码示例

```js
window.plus.Push.setLogOFF();
```

#### API - setCrashLogON

用于统计用户应用崩溃日志。

如果需要统计 Log 信息，调用该接口。当你需要自己收集错误信息时，切记不要调用该接口。

##### 接口定义

window.plus.Push.setCrashLogON()

##### 代码示例

```js
window.plus.Push.setCrashLogON();
```

### 页面统计

#### API - startLogPageView, stopLogPageView, beginLogPageView

用于“用户指定页面使用时长”的统计，并上报到服务器，在 Portal 上展示给开发者。

页面统计集成正确，才能够获取正确的页面访问路径、访问深度（PV）的数据。

##### 接口定义

window.plus.Push.startLogPageView(pageName)

window.plus.Push.stopLogPageView(pageName)

window.plus.Push.beginLogPageView(pageName, duration)

##### 参数说明

- pageName: 需要统计页面自定义名称。
- duration: 自定义的页面时间。

##### 调用说明

应在所有需要统计的页面的 `viewWillAppear` 和 `viewWillDisappear` 中加入 `startLogPageView` 和 `stopLogPageView` 来统计当前页面的停留时间。

或者直接使用 `beginLogPageView` 来自定义加入页面和时间信息。

##### 代码示例

```js
window.plus.Push.beginLogPageView("newPage", 5);

window.plus.Push.startLogPageView("onePage");

window.plus.Push.stopLogPageView("onePage");
```

### 地理位置上报

#### API - setLocation

用于统计用户地理信息。

##### 接口定义

window.plus.Push.setLocation(latitude, longitude)

##### 参数说明

- latitude: 地理位置纬度，数值类型或纯数字的字符型均可。
- longitude: 地理位置精度，数值类型或纯数字的字符型均可。

##### 代码示例

```js
window.plus.Push.setLocation(39.26, 115.25);
```


