document.addEventListener('plusready', function () {
  var _BARCODE = 'Push' // 插件名称
  var B = window.plus.bridge

  var JPushPlugin = {
    receiveMessage: {},
    openNotification: {},
    receiveNotification: {},

    callNative: function (fname, args, successCallback) {
      var callbackId = this.getCallbackId(successCallback, this.errorCallback)
      if (args != null) {
        args.unshift(callbackId)
      } else {
        var args = [callbackId]
      }
      return B.exec(_BARCODE, fname, args)
    },
    getCallbackId: function (successCallback) {
      var success = typeof successCallback !== 'function' ? null : function (args) {
        successCallback(args)
      }
      return B.callbackId(success, this.errorCallback)
    },
    errorCallback: function (errorMsg) {
      console.log('Javascript callback error: ' + errorMsg)
    },
    fireDocumentEvent: function (ename, jsonData) {
      var event = document.createEvent('HTMLEvents')
      event.initEvent(ename, true, true)
      event.eventType = 'message'

      jsonData = JSON.stringify(jsonData)
      var data = JSON.parse(jsonData)
      event.arguments = data
      document.dispatchEvent(event)
    },
    // Common method
    getRegistrationID: function (successCallback) {
      this.callNative('getRegistrationID', null, successCallback)
    },
    setTags: function (tags) {
      this.callNative('setTags', [tags], null)
    },
    setAlias: function (alias) {
      this.callNative('setAlias', [alias], null)
    },
    setTagsWithAlias: function (tags, alias) {
      if (alias == null) {
        this.setTags(tags)
        return
      }
      if (tags == null) {
        this.setAlias(alias)
        return
      }
      var arrayTagWithAlias = [tags]
      arrayTagWithAlias.unshift(alias)
      this.callNative('setTagsWithAlias', arrayTagWithAlias, null)
    },
    stopPush: function () {
      this.callNative('stopPush', null, null)
    },
    resumePush: function () {
      this.callNative('resumePush', null, null)
    },
    isPushStopped: function (successCallback) {
      this.callNative('isPushStopped', null, successCallback)
    },
    getCacheLaunchNotification: function (successCallback) {
      //点击推送的通知启动应用 JS 还没 ready，无法获取事件，该方法用于获取缓存的通知。
      this.callNative('getCacheLaunchNotification', null, successCallback)
    },

    // Android methods
    init: function () {
      if (plus.os.name == 'Android') {
        this.callNative('init', null, null)
      }
    },
    setDebugMode: function (mode) {
      if (plus.os.name == 'Android') {
        this.callNative('setDebugMode', [mode], null)
      }
    },
    addLocalNotification: function (builderId, content, title, notiID, broadcastTime, extras) {
      if (plus.os.name == 'Android') {
        data = [builderId, content, title, notiID, broadcastTime, extras]
        this.callNative('addLocalNotification', data, null)
      }
    },
    removeLocalNotification: function (notificationId) {
      if (plus.os.name == 'Android') {
        this.callNative('removeLocalNotification', [notificationId], null)
      }
    },
    clearLocalNotifications: function () {
      if (plus.os.name == 'Android') {
        this.callNative('clearLocalNotifications', null, null)
      }
    },
    clearAllNotification: function () {
      if (plus.os.name == 'Android') {
        this.callNative('clearAllNotification', null, null)
      }
    },
    clearNotificationById: function (notificationId) {
      if (plus.os.name == 'Android') {
        this.callNative('clearNotificationById', [notificationId], null)
      }
    },
    setBasicPushNotificationBuilder: function () {
      if (plus.os.name == 'Android') {
        this.callNative('setBasicPushNotification', null, null)
      }
    },
    setCustomPushNotificationBuilder: function () {
      if (plus.os.name == 'Android') {
        this.callNative('setCustomPushNotificationBuilder', null, null)
      }
    },
    setLatestNotificationNum: function (num) {
      if (plus.os.name == 'Android') {
        this.callNative('setLatestNotificationNum', [num], null)
      }
    },
    setPushTime: function (successCallback, weekDays, startHour, endHour) {
      if (plus.os.name == 'Android') {
        this.callNative('setPushTime', [weekDays, startHour, endHour], successCallback)
      }
    },
    setSilenceTime: function (successCallback, startHour, startMinute, endHour, endMinute) {
      if (plus.os.name == 'Android') {
        this.callNative('setSilenceTime', [startHour, startMinute, endHour, endMinute], successCallback)
      }
    },
    requestPermission: function () {
      if (plus.os.name == 'Android') {
        this.callNative('requestPermission', null, null)
      }
    },
    getConnectionState: function (successCallback) {
      if (plus.os.name == 'Android') {
        this.callNative('getConnectionState', null, successCallback)
      }
    },
    onGetRegistrationId: function (rId) {
      if (plus.os.name == 'Android') {
        this.fireDocumentEvent('jpush.onGetRegistrationId', rId)
      }
    },
    getLaunchAppCacheNotification: function (successCallback) {
      if (plus.os.name == 'iOS') {
        this.callNative('getLaunchAppCacheNotification', null, successCallback)
      }
    },
    clearLaunchAppCacheNotification: function () {
      if (plus.os.name == 'Android') {
        this.callNative('clearLaunchAppCacheNotification', null, null)
      }
    },
    receiveMessageInAndroidCallback: function (data) {
      if (plus.os.name == 'Android') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.receiveMessage = jsonObj
        this.fireDocumentEvent('jpush.receiveMessage', this.receiveMessage)
      }
    },
    openNotificationInAndroidCallback: function (data) {
      if (plus.os.name == 'Android') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.openNotification = jsonObj
        this.fireDocumentEvent('jpush.openNotification', this.openNotification)
      }
    },
    openNotificationIniOSCallback: function (data) {
      if (plus.os.name == 'iOS') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.openNotification = jsonObj
        this.fireDocumentEvent('jpush.openNotification', this.openNotification)
      }
    },
    receiveNotificationInAndroidCallback: function (data) {
      if (plus.os.name == 'Android') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.receiveNotification = jsonObj
        this.fireDocumentEvent('jpush.receiveNotification', this.receiveNotification)
      }
    },
    receiveNotificationIniOSCallback: function (data) {
      if (plus.os.name == 'iOS') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.receiveNotification = jsonObj

        this.fireDocumentEvent('jpush.receiveNotification', this.receiveNotification)
      }
    },
    receiveMessageIniOSCallback: function (data) {
      if (plus.os.name == 'iOS') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.receiveMessage = jsonObj
        this.fireDocumentEvent('jpush.receiveMessage', this.receiveMessage)
      }
    },
    receiveNotificationLaunceAppIniOSCallback: function (data) {
      if (plus.os.name == 'iOS') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.receiveMessage = jsonObj
        this.fireDocumentEvent('jpush.receiveNotificationLaunchApp', this.receiveMessage)
      }
    },
    receiveNotificationBackgroundIniOSCallback: function (data) {
      if (plus.os.name == 'iOS') {
        data = JSON.stringify(data)
        var jsonObj = JSON.parse(data)
        this.receiveMessage = jsonObj
        this.fireDocumentEvent('jpush.receiveNotificationBackground', this.receiveMessage)
      }
    },
    // iOS methods
    setBadge: function (value) {
      if (plus.os.name == 'iOS') {
        try {
          this.callNative('setBadge', [value], null)
        } catch (exception) {
          console.log(exception)
        }
      }
    },
    resetBadge: function () {
      if (plus.os.name == 'iOS') {
        try {
          this.callNative('resetBadge', [], null)
        } catch (exception) {
          console.log(exception)
        }
      }
    },
    setApplicationIconBadgeNumber: function (badge) {
      if (plus.os.name == 'iOS') {
        this.callNative('setApplicationIconBadgeNumber', [badge], null)
      }
    },
    getApplicationIconBadgeNumber: function (callback) {
      if (plus.os.name == 'iOS') {
        this.callNative('getApplicationIconBadgeNumber', [], callback)
      }
    },
    startLogPageView: function (pageName) {
      if (plus.os.name == 'iOS') {
        this.callNative('startLogPageView', [pageName], null)
      }
    },
    stopLogPageView: function (pageName) {
      if (plus.os.name == 'iOS') {
        this.callNative('stopLogPageView', [pageName], null)
      }
    },
    beginLogPageView: function (pageName, duration) {
      if (plus.os.name == 'iOS') {
        this.callNative('beginLogPageView', [pageName, duration], null)
      }
    },
    setLogOFF: function () {
      if (plus.os.name == 'iOS') {
        this.callNative('setLogOFF', [], null)
      }
    },
    setCrashLogON: function () {
      if (plus.os.name == 'iOS') {
        this.callNative('crashLogON', [], null)
      }
    },
    setLocation: function (latitude, longitude) {
      if (plus.os.name == 'iOS') {
        this.callNative('setLocation', [latitude, longitude], null)
      }
    },
    addLocalNotificationIniOS: function (delayTime, content, badge, notificationID, extras) {
      if (plus.os.name == 'iOS') {
        var data = [delayTime, content, badge, notificationID, extras]
        this.call_native('setLocalNotification', data, null)
      }
    },
    deleteLocalNotificationWithIdentifierKeyIniOS: function (identifierKey) {
      if (plus.os.name == 'iOS') {
        var data = [identifierKey]
        this.call_native('deleteLocalNotificationWithIdentifierKey', data, null)
      }
    },
    clearAllLocalNotificationsIniOS: function () {
      if (plus.os.name == 'iOS') {
        this.call_native('clearAllLocalNotifications', [], null)
      }
    }
  }

  JPushPlugin.init()
  window.plus.Push = JPushPlugin
}, true)
