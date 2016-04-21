(function(w){
// 空函数
function shield(){
	return false;
}
//取消浏览器的所有事件，使得active的样式在手机上正常生效
document.addEventListener('touchstart',shield,false);
document.oncontextmenu=shield;
// DOMContentLoaded事件处理
document.addEventListener('DOMContentLoaded',function(){
	document.body.onselectstart=shield;
	prettyPrint();
},false);
})(window);