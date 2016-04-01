var JPushPlugin = function(){
};

JPushPlugin.prototype.call_native = function(name, args, callback) {
	var b = window.plus.bridge;
	return window.plus.bridge.exec("PGJigungPush",name,[b.callbackId(null,null),1]);
}

JPushPlugin.prototype.getRegistrationID = function(){
		var b = window.plus.bridge;

	 window.plus.bridge.exec("PGJigungPush","getRegistrationID",[b.callbackId(null,null),1]);
}

