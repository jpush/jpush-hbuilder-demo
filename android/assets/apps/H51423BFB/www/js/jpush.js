document.addEventListener("plusready", function() {
    var _BARCODE = 'JPush',
		B = window.plus.bridge;
    
    var JPushPlugin = {
    	receiveMessage : {},
    	openNotification : {},
    	receiveNotification : {},
		callNative : function(fname, args, successCallback) {
			if (successCallback != null) {
				var callbackId = this.getCallbackId(successCallback, 
						this.errorCallback);
				if (args != null) {
					args.unshift(callbackId);
				} else {
					var args = [callbackId];
				}
			}
			return B.exec(_BARCODE, fname, args);
		},
		getCallbackId : function(successCallback) {
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			};
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};
			callbackID = B.callbackId(success, fail);
			return callbackID;
		},
		errorCallback : function(errorMsg) {
			console.log("Javascript callback error: " + errorMsg);
		},
		fireDocumentEvent : function(ename, jsonData) {
			var event = document.createEvent('HTMLEvents');
			event.initEvent(ename, true, true);
			event.eventType = 'message';
			jsonData = JSON.stringify(jsonData);
			var data = JSON.parse(jsonData);
			event.arguments = data;
			document.dispatchEvent(event);
		},
		init : function() {
			this.callNative("init", null, null);
		},
		setDebugMode : function(mode) {
			this.callNative("setDebugMode", [mode], null);
		},
		stopPush : function() {
			this.callNative("stopPush", null, null);
		},
		resumePush : function() {
			this.callNative("resumePush", null, null);
		},
		isPushStopped : function(successCallback) {
			this.callNative("isPushStopped", null, successCallback);
		},
		getRegistrationId : function(successCallback) {
			this.callNative("getRegistrationId", null, successCallback);
		},
		addLocalNotification : function(builderId, content, title,
				notificaitonID, broadcastTime, extras) {
			data = [builderId, content, title, notificaitonID, broadcastTime, extras];
			this.callNative("addLocalNotification", data, null);
		},
		removeLocalNotification : function(notificationId) {
			this.callNative("removeLocalNotification", [notificationId], null);
		},
		clearLocalNotifications : function() {
			this.callNative("clearLocalNotifications", null, null);
		},
		clearAllNotification : function() {
			this.callNative("clearAllNotification", null, null);
		},
		clearNotificationById : function(notificationId) {
			this.callNative("clearNotificationById", [notificationId], null);
		},
		setTags : function(tags) {
			this.callNative("setTags", tags, null);
		},
		setAlias : function(alias) {
			this.callNative("setAlias", [alias], null);
		},
		setAliasAndTags : function(alias, tags) {
			if(alias == null) {
				this.setTags(tags);
				return;
			}
			if(tags == null) {
				this.setAlias(alias);
				return;
			}
			var arrayTagWithAlias = [tags];
			arrayTagWithAlias.unshift(alias);
			this.callNative("setAliasAndTags", arrayTagWithAlias, null);
		},
		setBasicPushNotificationBuilder : function() {
			this.callNative("setBasicPushNotification", null, null);
		},
		setCustomPushNotificationBuilder : function() {
			this.callNative("setCustomPushNotificationBuilder", null, null);
		},
		setLatestNotificationNum : function(num) {
			this.callNative("setLatestNotificationNum", [num], null);
		},
		setPushTime : function(successCallback, weekDays, startHour, endHour) {
			this.callNative("setPushTime", [weekDays, startHour, endHour],
					successCallback);
		},
		setSilenceTime : function(successCallback, startHour, startMinute,
				endHour, endMinute) {
			this.callNative("setSilenceTime",
					[startHour, startMinute, endHour, endMinute], successCallback);
		},
		requetPermission : function() {
			this.callNative("requestPermission", null, null);
		},
		receiveMessageInAndroidCallback : function(data) {
			data = JSON.stringify(data);
			var jsonObj = JSON.parse(data);
			this.receiveMessage = jsonObj;
			this.fireDocumentEvent("jpush.receiveMessage", null);	
		},
		openNotificationInAndroidCallback : function(data) {
			data = JSON.stringify(data);
			var jsonObj = JSON.parse(data);
			this.openNotification = jsonObj;
			this.fireDocumentEvent("jpush.openNotification", null);
		},
		receiveNotificationInAndroidCallback : function(data) {
			data = JSON.stringify(data);
			var jsonObj = JSON.parse(data);
			this.receiveNotification = jsonObj;
			this.fireDocumentEvent("jpush.receiveNotification", null);
		}
    };
    
    JPushPlugin.init();
    window.plus.JPush = JPushPlugin;

}, true);