package io.dcloud.feature.jPush;

import io.dcloud.common.DHInterface.IWebview;
import io.dcloud.common.DHInterface.StandardFeature;
import io.dcloud.common.util.JSUtil;
import io.dcloud.feature.internal.sdk.SDK;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.iflytek.ui.r;

import android.content.Context;
import android.os.Bundle;
import cn.jpush.android.api.BasicPushNotificationBuilder;
import cn.jpush.android.api.CustomPushNotificationBuilder;
import cn.jpush.android.api.JPushInterface;
import cn.jpush.android.api.TagAliasCallback;
import cn.jpush.android.data.JPushLocalNotification;

public class JPushService extends StandardFeature {
	public static final String ID = "JPush";

	public static String notificationAlert;
	public static String openNotificationAlert;
	public static Map<String, Object> notificationExtras = new HashMap<String, Object>();
	public static Map<String, Object> openNotificationExtras = new HashMap<String, Object>();
	
	private static IWebview mIWebview;
    private static boolean shouldCacheMsg = false;
	
	@Override
	public void onStart(Context context, Bundle arg1, String[] arg2) {
		JPushInterface.init(context);
		JPushInterface.setDebugMode(false);
	}

	// 需要手动调用
	public void init(IWebview webview, JSONArray data) {
		mIWebview = webview;
        //如果同时缓存了打开事件 openNotificationAlert 和消息事件 notificationAlert，只向 UI 发打开事件。
        //这样做是为了和 iOS 统一
        if (openNotificationAlert != null) {
            notificationAlert = null;
            transmitOpen(openNotificationAlert, openNotificationExtras);
        }
        if (notificationAlert != null) {
            transmitReceive(notificationAlert, notificationExtras);
        }
	}
	
	@Override
	public void onPause() {
		super.onPause();
		shouldCacheMsg = true;
	}
	
	@Override
	public void onResume() {
		super.onResume();
		shouldCacheMsg = false;
        if (openNotificationAlert != null) {
            notificationAlert = null;
            transmitOpen(openNotificationAlert, openNotificationExtras);
        }
        if (notificationAlert != null) {
            transmitReceive(notificationAlert, notificationExtras);
        }
	}
	
	public void stopPush(IWebview webview, JSONArray data) {
		JPushInterface.stopPush(webview.getContext());
	}
	
	public void resumePush(IWebview webview, JSONArray data) {
		JPushInterface.resumePush(webview.getContext());
	}
	
	public void isPushStopped(IWebview webview, JSONArray data) {
		try {
			String callbackId = data.getString(0);
			boolean isPushStopped = JPushInterface.isPushStopped(webview.getContext());
			if (isPushStopped) {
				JSUtil.execCallback(webview, callbackId, 1, JSUtil.OK, false);
			} else {
				JSUtil.execCallback(webview, callbackId, 0, JSUtil.OK, false);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public static void transmitPush(String msg, Map<String, Object> extras) {
        JSONObject data = getNotificationObject(msg, extras);
        String format = "plus.JPush.receiveMessageInAndroidCallback(%s);";
        final String js = String.format(format, data.toString());
        mIWebview.getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mIWebview.loadUrl("javascript:" + js);
            }
        });
	}

    public static void transmitOpen(String alert, Map<String, Object> extras) {
        if (shouldCacheMsg) {
            return;
        }
        JSONObject data = openNotificationObject(alert, extras);
        String format = "plus.JPush.openNotificationInAndroidCallback(%s);";
        final String js = String.format(format, data.toString());
        mIWebview.getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mIWebview.loadUrl("javascript:" + js);
            }
        });
        openNotificationAlert = null;
    }

    public static void transmitReceive(String alert, Map<String, Object> extras) {
        JSONObject data = openNotificationObject(alert, extras);
        String format = "plus.JPush.receiveNotificationInAndroidCallback(%s);";
        final String js = String.format(format, data.toString());
        mIWebview.getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mIWebview.loadUrl("javascript:" + js);
            }
        });
        notificationAlert = null;
    }
	
	public void getRegistrationId(IWebview webview, JSONArray data) {
		try {
			String callbackId = data.getString(0);
			String regId = JPushInterface.getRegistrationID(webview.getContext());
			JSUtil.execCallback(webview, callbackId, regId, JSUtil.OK, false);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void addLocalNotification(IWebview webview, JSONArray data) {
		try {
	        int builderId = data.getInt(0);
	        String content = data.getString(1);
	        String title = data.getString(2);
	        int notificationId = data.getInt(3);
	        int broadcastTime = data.getInt(4);
	        JSONObject extras = data.isNull(5) ? new JSONObject() : data.getJSONObject(5);
	        
	        JPushLocalNotification jLocalNoti = new JPushLocalNotification();
	        jLocalNoti.setBuilderId(builderId);
	        jLocalNoti.setContent(content);
	        jLocalNoti.setTitle(title);
	        jLocalNoti.setNotificationId(notificationId);
	        jLocalNoti.setBroadcastTime(System.currentTimeMillis() + broadcastTime);
	        jLocalNoti.setExtras(extras.toString());
	        JPushInterface.addLocalNotification(webview.getActivity(), jLocalNoti);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void removeLocalNotification(IWebview webview, JSONArray data) {
		try {
			int notificationId = data.getInt(0);
			JPushInterface.removeLocalNotification(webview.getContext(),
					notificationId);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void clearLocalNotifications(IWebview webview, JSONArray data) {
		JPushInterface.clearLocalNotifications(webview.getContext());
	}
	
	public void clearAllNotification(IWebview webview, JSONArray data) {
		JPushInterface.clearAllNotifications(webview.getContext());
	}
	
	public void clearNotificationById(IWebview webview, JSONArray data) {
		int id = -1;
		try {
			id = data.getInt(0);
			if (id != -1) {
				JPushInterface.clearNotificationById(webview.getActivity(), id);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setTags(IWebview webview, JSONArray data) {
		try {
			HashSet<String> tags = new HashSet<String>();
			for (int i = 0, len = data.length(); i < len; i++) {
				tags.add(data.getString(i));
			}
			JPushInterface.setTags(webview.getContext(), tags, mTagAliasCallback);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setAlias(IWebview webview, JSONArray data) {
		try {
			String alias = data.getString(0);
			JPushInterface.setAlias(webview.getContext(), alias, mTagAliasCallback);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setAliasAndTags(IWebview webview, JSONArray data) {
		HashSet<String> tags = new HashSet<String>();
		String alias = null;
		try {
			alias = data.getString(0);
			JSONArray tagsJson = data.getJSONArray(1);
			for (int i = 0; i < tagsJson.length(); i++) {
				tags.add(tagsJson.getString(i));
			}
			JPushInterface.setAliasAndTags(webview.getContext(), alias, tags,
					mTagAliasCallback);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setDebugMode(IWebview webview, JSONArray data) {
		try {
			boolean isOpenDebugMode = data.getBoolean(0);
			JPushInterface.setDebugMode(isOpenDebugMode);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 设置通知是否触发声音、震动、呼吸灯。 
	 * 方法中的"1"代表该设置的编号，需要服务器端同时指定要发送通知的 builderId = 1, 才会触发。
	 * 可根据需要自行修改，具体请参考：http://docs.jpush.io/client/android_tutorials/#_11
	 */
	public void setBasicPushNotificationBuilder(IWebview webview, JSONArray data) {
		BasicPushNotificationBuilder builder = new BasicPushNotificationBuilder(
				webview.getContext());
		builder.developerArg0 = "Basic builder 1";
		JPushInterface.setPushNotificationBuilder(1, builder);
	}
	
	/**
	 * 设置通知使用自定义样式, 具体使用方法同上。
	 */
	public void setCustomPushNotificationBuilder(IWebview webview, JSONArray data) {
		// 需要自行修改
//		CustomPushNotificationBuilder builder = new CustomPushNotificationBuilder(
//				webview.getActivity(), R.layout.layout, R.id.icon, R.id.title,
//				R.id.text);
//		JPushInterface.setPushNotificationBuilder(2, builder);
	}
	
	public void setLatestNotificationNum(IWebview webview, JSONArray data) {
		int num = -1;
		try {
			num = data.getInt(0);
			if (num != -1) {
				JPushInterface.setLatestNotificationNumber(webview.getContext(), num);
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setPushTime(IWebview webview, JSONArray data) {
		try {
			String callbackId = data.getString(0);

			JSONArray weekDaysArr = data.isNull(1) ? null : data.getJSONArray(1);
			if (weekDaysArr.length() > 7) {
				JSUtil.execCallback(webview, callbackId, "允许推送日期设置不正确",
						JSUtil.ERROR, false);
				return;
			}

			int startHour = data.getInt(1);
			if (isValidHour(startHour)) {
				JSUtil.execCallback(webview, callbackId, "允许推送开始时间设置不正确",
						JSUtil.ERROR, false);
			}

			int endHour = data.getInt(2);
			if (isValidHour(endHour)) {
				JSUtil.execCallback(webview, callbackId, "允许推送结束时间设置不正确",
						JSUtil.ERROR, false);
			}
			
			Set<Integer> weekDays = weekDaysArr == null ? null 
					: new HashSet<Integer>();
			for(int i = 0; i < weekDaysArr.length(); i++) {
				weekDays.add(weekDaysArr.getInt(i));
			}
			JPushInterface.setPushTime(webview.getContext(), weekDays,
					startHour, endHour);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	public void setSilenceTime(IWebview webview, JSONArray data) {
		try {
			String callbackId = data.getString(0);

			int startHour = data.getInt(1);
			int startMin = data.getInt(2);
			int endHour = data.getInt(3);
			int endMin = data.getInt(4);
			if (!isValidHour(startHour) || !isValidHour(endHour) 
					|| !isValidMinute(startMin) || !isValidMinute(endMin)) {
				JSUtil.execCallback(webview, callbackId, "时间设置不正确",
						JSUtil.ERROR, false);
				return;
			}
			JPushInterface.setSilenceTime(webview.getContext(), startHour,
					startMin, endHour, endMin);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 *  用于 Android 6.0 以上系统申请权限，具体可参考：
	 *  http://docs.jpush.io/client/android_api/#android-60
	 */
	public void requestPermission(IWebview webview, JSONArray data) {
		int currentVersion = android.os.Build.VERSION.SDK_INT;
		if (currentVersion > android.os.Build.VERSION_CODES.LOLLIPOP) {
			JPushInterface.requestPermission(webview.getContext());
		}
	}
	
	private static JSONObject getNotificationObject(String msg,
			Map<String, Object> extras) {
        JSONObject data = new JSONObject();
        try {
            data.put("message", msg);
            JSONObject jExtras = new JSONObject();
            for (Entry<String, Object> entry : extras.entrySet()) {
                if (entry.getKey().equals("cn.jpush.android.EXTRA")) {
                    JSONObject jo = new JSONObject((String) entry.getValue());
                    jExtras.put("cn.jpush.android.EXTRA", jo);
                } else {
                    jExtras.put(entry.getKey(), entry.getValue());
                }
            }
            if (jExtras.length() > 0) {
                data.put("extras", jExtras);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return data;
	}
	
    private static JSONObject openNotificationObject(String alert,
            Map<String, Object> extras) {
        JSONObject data = new JSONObject();
        try {
            data.put("alert", alert);
            JSONObject jExtras = new JSONObject();
            for (Entry<String, Object> entry : extras.entrySet()) {
                if (entry.getKey().equals("cn.jpush.android.EXTRA")) {
                    JSONObject jo = new JSONObject((String) entry.getValue());
                    jExtras.put("cn.jpush.android.EXTRA", jo);
                } else {
                    jExtras.put(entry.getKey(), entry.getValue());
                }
            }
            if (jExtras.length() > 0) {
                data.put("extras", jExtras);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return data;
    }
    
    private TagAliasCallback mTagAliasCallback = new TagAliasCallback() {
		@Override
		public void gotResult(int code, String alias, Set<String> tags) {
			JSONObject data = new JSONObject(); 
			try {
				data.put("resultCode", code);
				data.put("tags", tags);
				data.put("alias", alias);
                final String jsEvent = String.format(
                        "plus.JPush.fireDocumentEvent('jpush.setTagsWithAlias', %s)",
                        data.toString());
                mIWebview.getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mIWebview.loadUrl("javascript:" + jsEvent);
                    }
                });
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
	};
	
	private boolean isValidHour(int hour) {
		if (hour < 0 || hour > 23) {
			return false;
		}
		return true;
	}
	
	private boolean isValidMinute(int min) {
		if (min < 0 || min > 59) {
			return false;
		}
		return true;
	}
	
}
