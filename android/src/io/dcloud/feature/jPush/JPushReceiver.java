package io.dcloud.feature.jPush;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import cn.jpush.android.api.JPushInterface;

public class JPushReceiver extends BroadcastReceiver {
	private static final String TAG = "JPush";

    private static final List<String> IGNORED_EXTRAS_KEYS =
            Arrays.asList(
                    "cn.jpush.android.TITLE",
                    "cn.jpush.android.MESSAGE",
                    "cn.jpush.android.APPKEY",
                    "cn.jpush.android.NOTIFICATION_CONTENT_TITLE"
            );

	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (JPushInterface.ACTION_NOTIFICATION_RECEIVED.equals(action)) {
			handleNotificationReceived(context, intent);
		} else if (JPushInterface.ACTION_MESSAGE_RECEIVED.equals(action)) {
			handleMessageReceived(intent);
		} else if (JPushInterface.ACTION_NOTIFICATION_OPENED.equals(action)) {
			handleNotificationOpened(context, intent);
		}
	}
	
	private void handleNotificationReceived(Context context, Intent intent) {
        Intent launch = context.getPackageManager().getLaunchIntentForPackage(
        		context.getPackageName());
        launch.addCategory(Intent.CATEGORY_LAUNCHER);
        launch.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);

        String alert = intent.getStringExtra(JPushInterface.EXTRA_ALERT);
        JPushService.notificationAlert = alert;

        Map<String, Object> extras = getNotificationExtras(intent);
        JPushService.notificationExtras = extras;

        JPushService.transmitReceive(alert, extras);
	}
	
	private void handleNotificationOpened(Context context, Intent intent) {
		String alert = intent.getStringExtra(JPushInterface.EXTRA_ALERT);
		JPushService.openNotificationAlert = alert;
        Map<String, Object> extras = getNotificationExtras(intent);
        JPushService.openNotificationExtras = extras;

        JPushService.transmitOpen(alert, extras);

        Intent launch = context.getPackageManager().getLaunchIntentForPackage(
            context.getPackageName());
        launch.addCategory(Intent.CATEGORY_LAUNCHER);
        launch.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
        context.startActivity(launch);
	}

	private void handleMessageReceived(Intent intent) {
		String msg = intent.getStringExtra(JPushInterface.EXTRA_MESSAGE);  
		Map<String, Object> extras = getNotificationExtras(intent);
		JPushService.transmitPush(msg, extras);
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
	
	private Map<String, Object> getNotificationExtras(Intent intent) {
        Map<String, Object> extrasMap = new HashMap<String, Object>();
        for (String key : intent.getExtras().keySet()) {
            if (!IGNORED_EXTRAS_KEYS.contains(key)) {
                if (key.equals(JPushInterface.EXTRA_NOTIFICATION_ID)) {
                    extrasMap.put(key, intent.getIntExtra(key, 0));
                } else {
                    extrasMap.put(key, intent.getStringExtra(key));
                }
            }
        }
        return extrasMap;
	}

}
