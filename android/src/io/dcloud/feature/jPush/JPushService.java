package io.dcloud.feature.jPush;

import io.dcloud.common.DHInterface.IWebview;
import io.dcloud.common.adapter.util.AndroidResources;
import io.dcloud.common.util.JSUtil;
import io.dcloud.feature.aps.AbsPushService;
import io.dcloud.feature.aps.PushMessage;

import java.util.HashSet;

import org.json.JSONArray;
import org.json.JSONException;

import android.content.Context;
import android.os.Bundle;
import cn.jpush.android.api.JPushInterface;
import cn.jpush.android.data.JPushLocalNotification;

public class JPushService extends AbsPushService {
	public static final String ID = "JPush";

	@Override
	public void onStart(Context pContext, Bundle arg1, String[] arg2) {
		JPushInterface.init(pContext);
		JPushInterface.setDebugMode(false);
	}
	
	@Override
	public String getClientInfo(Context context) {
		if(clientid == null) {
			clientid = JPushInterface.getRegistrationID(context);
		}
		if(appkey == null) {
			appkey = AndroidResources.getMetaValue("JPUSH_APPKEY");
		}
		appid = "";
		appsecret = "";
		saveClientId(context);
		return super.getClientInfo(context);
	}
	
	@Override
	public boolean setAutoNotification(IWebview webview, JSONArray pJsArgs,
			String _appId) throws JSONException {
		boolean needPush = super.setAutoNotification(webview, pJsArgs, _appId);
		if (needPush) {
			if(JPushInterface.isPushStopped(webview.getContext())) {
				JPushInterface.resumePush(webview.getContext());
			}
		} else {
			JPushInterface.stopPush(webview.getContext());
		}
		return needPush;
	}
	
	/**
	 * 创建本地通知
	 */
	@Override
	public String createMessage(IWebview webview, JSONArray pJsArgs,
			final String appName, final Context context) throws JSONException {
		PushMessage pushMsg = new PushMessage(webview, pJsArgs.getString(0)); 
		JPushLocalNotification ln = new JPushLocalNotification();
		ln.setContent(pushMsg.mContent);
		ln.setTitle(pushMsg.mTitle);
		JPushInterface.addLocalNotification(webview.getContext(), ln);
        return JSUtil.wrapJsVar(pushMsg.mUUID);
	}
	
	public void setTags(IWebview webview, JSONArray data) {
		try {
			HashSet<String> tags = new HashSet<String>();
			for (int i = 0, len = data.length(); i < len; i++) {
					tags.add(data.getString(i));
			}
			JPushInterface.setTags(webview.getContext(), tags, null);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setAlias(IWebview webview, JSONArray data) {
		try {
			String alias = data.getString(0);
			JPushInterface.setAlias(webview.getContext(), alias, null);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setAliasAndTags(IWebview webview, JSONArray data) {
		
	}
	
}
