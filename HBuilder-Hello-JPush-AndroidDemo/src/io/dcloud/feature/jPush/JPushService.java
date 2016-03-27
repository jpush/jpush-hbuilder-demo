package io.dcloud.feature.jPush;

import io.dcloud.common.DHInterface.IWebview;
import io.dcloud.common.adapter.util.AndroidResources;
import io.dcloud.feature.aps.AbsPushService;

import org.json.JSONArray;
import org.json.JSONException;

import android.content.Context;
import android.os.Bundle;
import cn.jpush.android.api.JPushInterface;

public class JPushService extends AbsPushService {
	public static final String ID = "JPush";

	@Override
	public void onStart(Context pContext, Bundle arg1, String[] arg2) {
		JPushInterface.setDebugMode(false);
		JPushInterface.init(pContext);
	}

	@Override
	public String getClientInfo(Context context) {
		
		if(clientid == null)
		{
			clientid = JPushInterface.getRegistrationID(context);
		}
		
		if(appkey == null)
		{
			appkey = AndroidResources.getMetaValue("JPUSH_APPKEY");
		}
		appid = "";
		appsecret = "";
		saveClientId(context);
		
		return super.getClientInfo(context);
	}
	
	@Override
	public boolean setAutoNotification(IWebview pWebViewImpl,
			JSONArray pJsArgs, String _appId) throws JSONException {
		boolean needPush = super.setAutoNotification(pWebViewImpl, pJsArgs,
				_appId);
		if (needPush) {
			if(JPushInterface.isPushStopped(pWebViewImpl.getContext()))
				JPushInterface.resumePush(pWebViewImpl.getContext());
		} else {
			JPushInterface.stopPush(pWebViewImpl.getContext());

		}
		return needPush;
	}

}
