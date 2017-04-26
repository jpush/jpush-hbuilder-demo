(function(w) {
	// 空函数
	function shield() {
		return false;
	}

	document.addEventListener('touchstart', shield, false);//取消浏览器的所有事件，使得active的样式在手机上正常生效
	document.oncontextmenu = shield;//屏蔽选择函数

	// H5 plus事件处理
	var ws = null,as = 'pop-in';
	function plusReady() {
		ws = plus.webview.currentWebview();
		// Android处理返回键
		plus.key.addEventListener('backbutton', function() {
			back();
		}, false);
		compatibleAdjust();
	}
	if(w.plus) {
		plusReady();
	} else {
		document.addEventListener('plusready', plusReady, false);
	}

	// DOMContentLoaded事件处理
	var domready = false;
	document.addEventListener('DOMContentLoaded', function() {
		domready = true;
		gInit();
		document.body.onselectstart = shield;
		compatibleAdjust();
	}, false);

	// 处理返回事件
	w.back = function(hide) {
		if(w.plus) {
			ws || (ws = plus.webview.currentWebview());
			if(hide || ws.preate) {
				ws.hide('auto');
			} else {
				ws.close('auto');
			}
		} else if(history.length > 1) {
			history.back();
		} else {
			w.close();
		}
	};

	// 处理点击事件
	var openw = null, waiting = null;
	/**
	 * 打开新窗口
	 * @param {URIString} id : 要打开页面url
	 * @param {boolean} wa : 是否显示等待框
	 * @param {boolean} ns : 是否不自动显示
	 */
	w.clicked = function(id, wa, ns) {
		if(openw) {//避免多次打开同一个页面
			return null;
		}
		if(w.plus) {
			wa && (waiting = plus.nativeUI.showWaiting());
			var pre = '';//'http://192.168.1.178:8080/h5/';
			openw = plus.webview.create(pre+id, id, 
				{
					scrollIndicator:'none',
					scalable:false
				});
			ns || openw.addEventListener('loaded', function() {//页面加载完成后才显示
	//		setTimeout(function() {//延后显示可避免低端机上动画时白屏
				openw.show(as);
				closeWaiting();
	//		},200);
			}, false);
			openw.addEventListener('close', function() {//页面关闭后可再次打开
				openw = null;
			}, false);
			return openw;
		} else {
			w.open(id);
		}
		return null;
	};
	w.openDoc = function(t, c) {
		var d = plus.webview.getWebviewById('document');
		if(d) {
			d.evalJS('updateDoc("' + t + '","' + c + '")');
		}else{
			d = plus.webview.create('/plus/doc.html', 'document', 
				{
					zindex:9999,
					popGesture:'hide'
				}, {
					preate:true
				});
			d.addEventListener('loaded', function() {
				d.evalJS('updateDoc("' + t + '","' + c + '")');
			}, false);
		}
	}

	/**
	 * 关闭等待框
	 */
	w.closeWaiting = function() {
		waiting && waiting.close();
		waiting = null;
	}

	// 兼容性样式调整
	var adjust = false;
	function compatibleAdjust() {
		if(adjust || !w.plus || !domready) {
			return;
		}	// iOS平台使用滚动的div
		if('iOS' == plus.os.name) {
			var t = document.getElementById("dcontent");
			t && (t.className = "sdcontent");
			t = document.getElementById("content");
			t && (t.className = "scontent");
			//iOS8横竖屏切换div不更新滚动问题
			var lasto = window.orientation;
			window.addEventListener("orientationchange", function() {
				var nowo = window.orientation;
				if(lasto != nowo && (90 == nowo || -90 == nowo)) {
					dcontent && (0 == dcontent.scrollTop) && (dcontent.scrollTop = 1);
					content && (0 == content.scrollTop) && (content.scrollTop = 1);
				}
				lasto = nowo;
			}, false);
		}
		adjust = true;
	};
	w.compatibleConfirm = function() {
		plus.nativeUI.confirm(
			'本OS原生层面不提供该控件，需使用mui框架实现类似效果。请点击“确定”下载Hello mui示例',
			function(e) {
				if(0==e.index) {
					plus.runtime.openURL("http://www.dcloud.io/hellomui/");
				}
			}, "", ["确定","取消"]);
	}

	// 通用元素对象
	var _dout_ = null,_dcontent_ = null;
	w.gInit = function() {
		_dout_ = document.getElementById("output");
		_dcontent_ = document.getElementById("dcontent");
	};

	// 清空输出内容
	w.outClean = function() {
		_dout_.innerText = "";
		_dout_.scrollTop = 0;//在iOS8存在不滚动的现象
	};

	// 输出内容
	w.outSet = function(s) {
		_dout_.innerText = (s + "\n");
		(0 == _dout_.scrollTop) && (_dout_.scrollTop = 1);//在iOS8存在不滚动的现象
	};

	// 输出行内容
	w.outLine = function(s) {
		_dout_.innerText += (s + "\n");
		(0 == _dout_.scrollTop) && (_dout_.scrollTop = 1);//在iOS8存在不滚动的现象
	};

	// 格式化时长字符串，格式为"HH:MM:SS"
	w.timeToStr = function(ts) {
		if(isNaN(ts)) {
			return "--:--:--";
		}
		var h = parseInt(ts / 3600);
		var m = parseInt((ts % 3600) / 60);
		var s = parseInt(ts % 60); 
		return (ultZeroize(h) + ":" + ultZeroize(m) + ":" + ultZeroize(s));
	};

	// 格式化日期时间字符串，格式为"YYYY-MM-DD HH:MM:SS"
	w.dateToStr = function(d) {
		return (d.getFullYear() + "-" + ultZeroize(d.getMonth() + 1) + "-"
			+ ultZeroize(d.getDate()) + " " + ultZeroize(d.getHours()) + ":" 
			+ ultZeroize(d.getMinutes()) + ":" + ultZeroize(d.getSeconds()));
	};

	/**
	 * zeroize value with length(default is 2).
	 * @param {Object} v
	 * @param {Number} l
	 * @return {String} 
	 */
	w.ultZeroize = function(v, l) {
		var z = "";
		l = l || 2;
		v = String(v);
		for(var i = 0; i < l-v.length; i++) {
			z += "0";
		}
		return z + v;
	};
})(window);