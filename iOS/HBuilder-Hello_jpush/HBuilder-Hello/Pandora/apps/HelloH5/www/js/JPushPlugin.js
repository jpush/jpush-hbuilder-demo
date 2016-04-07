var JPushPlugin = function(){
};

if(!window.plugins) {
	window.plugins = {};
}

if(!window.plugins.jPushPlugin){
	window.plugins.jPushPlugin = new JPushPlugin();
}

JPushPlugin.prototype.call_native = function(name, args, callback) {
	var b = window.plus.bridge;
	var error_callback = function(msg){
		console.log("Javascript Callback Error: " + msg);
	}
	return window.plus.bridge.exec("JPushPlugin",name,[b.callbackId(callback,error_callback),args]);
}

//public plugin function

JPushPlugin.prototype.setTagsWithAlias = function(tags, alias, callback) {
	try {
  		if(tags == null) {
  			this.setAlias(alias);
      		return;
  		}
  		if(alias == null) {
  			this.setTags(tags);
			return;
		}
		var arrayTagWithAlias = [tags];
		arrayTagWithAlias.unshift(alias);
		this.call_native("setTagsWithAlias", arrayTagWithAlias, callback);
	} catch(exception) {
    	console.log(exception);
	}
}

JPushPlugin.prototype.setTags = function(tags) {
	try {
		this.call_native("setTags", tags, null);
	} catch(exception) {
		console.log(exception);
	}
}

JPushPlugin.prototype.setAlias = function(alias) {
	try {
		this.call_native("setAlias", [alias], null);
	} catch(exception) {
		console.log(exception);
	}
}

JPushPlugin.prototype.getRegistrationID = function(callback) {
	try {
		this.call_native("getRegistrationID", [], callback);
	} catch(exception) {
		console.log(exception);
	}
}

JPushPlugin.prototype.setBadge = function(value) {
	try {
		this.call_native("setBadge", [value], null);
	} catch(exception) {
		console.log(exception);
	}
}

JPushPlugin.prototype.resetBadge = function() {
	try {
		this.call_native("resetBadge", [], null);
	} catch(exception) {
		console.log(exception);
	}
}

JPushPlugin.prototype.setApplicationIconBadgeNumber = function(badge) {
	this.call_native("setApplicationIconBadgeNumber", [badge], null);
}

JPushPlugin.prototype.getApplicationIconBadgeNumber = function(callback) {
	this.call_native("getApplicationIconBadgeNumber", [], callback);
}

JPushPlugin.prototype.startLogPageView = function(pageName) {
	this.call_native("startLogPageView", [pageName], null);
}

JPushPlugin.prototype.stopLogPageView = function(pageName) {
	this.call_native("stopLogPageView", [pageName], null);
}

JPushPlugin.prototype.beginLogPageView = function(pageName, duration) {
	this.call_native("beginLogPageView", [pageName, duration], null);
}

JPushPlugin.prototype.setDebugMode = function() {
    	this.call_native("setDebugMode", [], null);
}

JPushPlugin.prototype.setLogOFF = function() {
    	this.call_native("setLogOFF", [], null);
}

JPushPlugin.prototype.setCrashLogON = function() {
	this.call_native("crashLogON", [], null);
}

JPushPlugin.prototype.addLocalNotification = function(delayTime, content,
	badge, notificationID, extras) {
	var data = [delayTime, content, badge, notificationID, extras];
	this.call_native("setLocalNotification", data, null);
}

JPushPlugin.prototype.deleteLocalNotificationWithIdentifierKeyIn = function(
	identifierKey) {
	var data = [identifierKey];
	this.call_native("deleteLocalNotificationWithIdentifierKey", data, null);
}
 
JPushPlugin.prototype.clearAllLocalNotifications = function(){
	this.call_native("clearAllLocalNotifications", [], null);
}

JPushPlugin.prototype.setLocation = function(latitude, longitude){
	this.call_native("setLocation", [latitude, longitude], null);
}

JPushPlugin.prototype.receiveMessage = function(data) {
	try {
		console.log("JPushPlugin:receiveMessage--data:" + data);
		var bToObj = JSON.parse(data);
		var content = bToObj.content;
    		console.log(content);
	} catch(exception) {
		console.log("JPushPlugin:receiveMessage" + exception);
	}
}

JPushPlugin.prototype.stopPush = function() {
	this.call_native("stopPush", [], null);
}

JPushPlugin.prototype.resumePush = function() {
	this.call_native("resumePush", [], null);
}

JPushPlugin.prototype.isPushStopped = function(callback) {
	this.call_native("isPushStopped", [], callback);
}
