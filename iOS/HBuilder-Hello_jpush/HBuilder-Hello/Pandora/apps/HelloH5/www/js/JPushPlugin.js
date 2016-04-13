document.addEventListener( "plusready",  function()
{
	//插件名称
	var plugin_name = "JPushPlugin";
    var jPushPlugin = 
	{   
		
call_native : function(name, args, callback) {
	var b = window.plus.bridge;
	var error_callback = function(msg){
	console.log("Javascript Callback Error: " + msg);	
	}
	return window.plus.bridge.exec("JPushPlugin",name,[b.callbackId(callback,error_callback),args]);	
},

//public plugin function

setTagsWithAlias : function(tags, alias, callback) {
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
},

setTags : function(tags) {
	try {
		this.call_native("setTags", tags, null);
	} catch(exception) {
		console.log(exception);
	}
},

setAlias : function(alias) {
	try {
		this.call_native("setAlias", [alias], null);
	} catch(exception) {
		console.log(exception);
	}
},

getRegistrationID : function(callback) {
	try {
		this.call_native("getRegistrationID", [], callback);
	} catch(exception) {
		console.log(exception);
	}
},

setBadge : function(value) {
	try {
		this.call_native("setBadge", [value], null);
	} catch(exception) {
		console.log(exception);
	}
},

resetBadge : function() {
	try {
		this.call_native("resetBadge", [], null);
	} catch(exception) {
		console.log(exception);
	}
},

setApplicationIconBadgeNumber : function(badge) {
	this.call_native("setApplicationIconBadgeNumber", [badge], null);
},

getApplicationIconBadgeNumber : function(callback) {
	this.call_native("getApplicationIconBadgeNumber", [], callback);
},

startLogPageView : function(pageName) {
	this.call_native("startLogPageView", [pageName], null);
},

stopLogPageView : function(pageName) {
	this.call_native("stopLogPageView", [pageName], null);
},

beginLogPageView : function(pageName, duration) {
	this.call_native("beginLogPageView", [pageName, duration], null);
},

setDebugMode : function() {
    	this.call_native("setDebugMode", [], null);
},

setLogOFF : function() {
    	this.call_native("setLogOFF", [], null);
},

setCrashLogON : function() {
	this.call_native("crashLogON", [], null);
},

addLocalNotification : function(delayTime, content,
	badge, notificationID, extras) {
	var data = [delayTime, content, badge, notificationID, extras];
	this.call_native("setLocalNotification", data, null);
},

deleteLocalNotificationWithIdentifierKeyIn : function(
	identifierKey) {
	var data = [identifierKey];
	this.call_native("deleteLocalNotificationWithIdentifierKey", data, null);
},
 
clearAllLocalNotifications : function(){
	this.call_native("clearAllLocalNotifications", [], null);
},

setLocation : function(latitude, longitude){
	this.call_native("setLocation", [latitude, longitude], null);
},

receiveMessage : function(data) {
	try {
		console.log("JPushPlugin:receiveMessage--data:" + data);
		var bToObj = JSON.parse(data);
		var content = bToObj.content;
    		console.log(content);
	} catch(exception) {
		console.log("JPushPlugin:receiveMessage" + exception);
	}
},

stopPush : function() {
	this.call_native("stopPush", [], null);
},

resumePush : function() {
	this.call_native("resumePush", [], null);
},

isPushStopped : function(callback) {
	this.call_native("isPushStopped", [], callback);
},
	
    };
    
    window.plus.jPushPlugin = jPushPlugin;
}, true );
