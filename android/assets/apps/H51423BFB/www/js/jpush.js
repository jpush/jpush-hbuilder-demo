document.addEventListener( "plusready",  function() {
    var _BARCODE = 'push';
	var B = window.plus.bridge;
	
	var push = {
		setAliasAndTags : function(alias, tags, successCallback, errorCallback) {
			if(alias == null) {
				
			}
			if(tags == null) {
				
			}
		},
		setTags : function(tags, successCallback, errorCallback) {
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			},
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};
			return B.exec(_BARCODE, "setTags", tags);
		},
		setAlias : function(alias, successCallback, errorCallback) {
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			},
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};

			return B.exec(_BARCODE, "setAlias", alias);
		}
	};
	
	window.plus.push = push;
}, true);