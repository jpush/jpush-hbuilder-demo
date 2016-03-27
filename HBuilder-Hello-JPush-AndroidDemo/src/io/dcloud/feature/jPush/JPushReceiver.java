package io.dcloud.feature.jPush;

import io.dcloud.common.util.JSONUtil;
import io.dcloud.common.util.PdrUtil;
import io.dcloud.feature.aps.APSFeatureImpl;
import io.dcloud.feature.aps.AbsPushService;
import io.dcloud.feature.aps.PushMessage;

import org.json.JSONObject;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import cn.jpush.android.api.JPushInterface;

public class JPushReceiver extends BroadcastReceiver {
	private static final String TAG = "JPush";

	private static String appid;

	@Override
	public void onReceive(Context context, Intent intent) {
		Bundle bundle = intent.getExtras();

		if (JPushInterface.ACTION_MESSAGE_RECEIVED.equals(intent.getAction())) {
			String data = bundle.getString(JPushInterface.EXTRA_EXTRA);

			if (PdrUtil.isEmpty(data)) {

			} else {
				boolean createNotifcation = true;
				JSONObject jsobj = JSONUtil.createJSONObject(data);
				String title = JSONUtil.getString(jsobj, "title");
				if (PdrUtil.isEmpty(title)) {
					createNotifcation = false;
					title = getApplicationName(context);
				}

				String content = JSONUtil.getString(jsobj, "content");
				if (PdrUtil.isEmpty(content)) {
					content = data;
					createNotifcation = false;
				}

				String appid = JSONUtil.getString(jsobj, "appid");
				String payload = JSONUtil.getString(jsobj, "payload");
				if (PdrUtil.isEmpty(payload)) {
					payload = data;
					createNotifcation = false;
				}

				PushMessage _pushMessage = new PushMessage(title, content, appid);
				_pushMessage.mPayload = payload;
				boolean needPush = AbsPushService.getAutoNotification(context,
						appid, bundle.getString(JPushInterface.EXTRA_NOTIFICATION_ID));
				if (needPush && createNotifcation) {
					_pushMessage.setNotificationID();
					APSFeatureImpl.sendCreateNotificationBroadcast(context, appid,
						_pushMessage);
				} else if (!APSFeatureImpl.execScript(context, "receive",
						_pushMessage.toJSON())) {
					APSFeatureImpl.addNeedExecReceiveMessage(context, _pushMessage);
				}
				APSFeatureImpl.addPushMessage(context, appid, _pushMessage);
			}
		} else if (JPushInterface.ACTION_CONNECTION_CHANGE.equals(intent.getAction())) {
			String clientid = JPushInterface.getRegistrationID(context);
			SharedPreferences _sp = context.getSharedPreferences(
				AbsPushService.CLIENTID + JPushService.ID, Context.MODE_PRIVATE);
			Editor ed = _sp.edit();
			ed.putString(AbsPushService.PUSH_CLIENT_ID_NAME, clientid);
			ed.commit();
		}
	}

	public String getApplicationName(Context context) {
		PackageManager packageManager = null;
		ApplicationInfo applicationInfo = null;
		try {
			packageManager = context.getApplicationContext().getPackageManager();
			applicationInfo = packageManager.getApplicationInfo(
				context.getPackageName(), 0);
		} catch (PackageManager.NameNotFoundException e) {
			applicationInfo = null;
		}
		String applicationName = (String) packageManager.getApplicationLabel(
			applicationInfo);
		return applicationName;
	}

}
