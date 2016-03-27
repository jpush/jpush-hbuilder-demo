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

		// 鎺ユ敹鍒版帹閫佷笅鏉ョ殑閫氱煡
		if (JPushInterface.ACTION_MESSAGE_RECEIVED.equals(intent.getAction())) {
			String data = bundle.getString(JPushInterface.EXTRA_EXTRA);
			if (PdrUtil.isEmpty(data)) {// payload鏁版嵁涓虹┖鏃讹紝璁や负鏄秷鎭腑蹇冪偣鍑昏繘鍏ョ殑锛岄渶瑕佽Е鍙慶lick浜嬩欢

			} else {// 鑾峰緱鐨刾ayload涓嶄负绌烘椂瑙﹀彂receive閫昏緫
				boolean createNotifcation = true;// title銆乧ontent銆乸ayload涓夎�呴兘瀛樺湪鐨勬椂鍊欐墠鍒涘缓娑堟伅閫氱煡

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
				// appid瑕佽�冭檻寮�鏈鸿嚜鍚姩鐨勬椂鍊欙紝绋嬪簭娌℃湁鍒濆鍖栫殑鎯呭喌锛屼负绌虹殑鏃跺�欑幇鍦ㄧ敤榛樿鐨凱DR浣滀负
				String appid = JSONUtil.getString(jsobj, "appid");
				String payload = JSONUtil.getString(jsobj, "payload");
				if (PdrUtil.isEmpty(payload)) {
					payload = data;
					createNotifcation = false;
				}

				PushMessage _pushMessage = new PushMessage(title, content,
						appid);
				_pushMessage.mPayload = payload;

				boolean needPush = AbsPushService.getAutoNotification(context,
						appid,
						bundle.getString(JPushInterface.EXTRA_NOTIFICATION_ID));
				if (needPush && createNotifcation) {
					_pushMessage.setNotificationID();
					APSFeatureImpl.sendCreateNotificationBroadcast(context,
							appid, _pushMessage);
				} else if (!APSFeatureImpl.execScript(context, "receive",
						_pushMessage.toJSON())) {// 娣诲姞receive鎵ц闃熷垪
					APSFeatureImpl.addNeedExecReceiveMessage(context,
							_pushMessage);
				}
				APSFeatureImpl.addPushMessage(context, appid, _pushMessage);
			}
		} else if (JPushInterface.ACTION_CONNECTION_CHANGE.equals(intent
				.getAction())) {
			String clientid = JPushInterface.getRegistrationID(context);
			SharedPreferences _sp = context.getSharedPreferences(
					AbsPushService.CLIENTID + JPushService.ID,
					Context.MODE_PRIVATE);
			Editor ed = _sp.edit();
			ed.putString(AbsPushService.PUSH_CLIENT_ID_NAME, clientid);
			ed.commit();
		}
	}

	public String getApplicationName(Context context) {
		PackageManager packageManager = null;
		ApplicationInfo applicationInfo = null;
		try {
			packageManager = context.getApplicationContext()
					.getPackageManager();
			applicationInfo = packageManager.getApplicationInfo(
					context.getPackageName(), 0);
		} catch (PackageManager.NameNotFoundException e) {
			applicationInfo = null;
		}
		String applicationName = (String) packageManager
				.getApplicationLabel(applicationInfo);
		return applicationName;
	}

}
