package io.dcloud.feature.jpush;

import android.text.TextUtils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import cn.jpush.android.api.BasicPushNotificationBuilder;
import cn.jpush.android.api.JPushInterface;
import cn.jpush.android.api.TagAliasCallback;
import cn.jpush.android.data.JPushLocalNotification;
import io.dcloud.common.DHInterface.IWebview;
import io.dcloud.common.DHInterface.StandardFeature;
import io.dcloud.common.util.JSUtil;

public class JPushService extends StandardFeature {

    public static final String TAG = JPushService.class.getSimpleName();

    static String notificationTitle;
    static String notificationAlert;
    static Map<String, Object> notificationExtras = new HashMap<String, Object>();

    static String openNotificationTitle;
    static String openNotificationAlert;
    static Map<String, Object> openNotificationExtras = new HashMap<String, Object>();

    private static IWebview mIWebView;
    private static boolean shouldCacheMsg = false;

    private static String mRegistrationId;

    // 需要手动调用
    public void init(IWebview webView, JSONArray data) {
        JPushInterface.init(webView.getContext().getApplicationContext());

        mIWebView = webView;

        if (openNotificationAlert != null) {
            transmitNotificationOpen(openNotificationTitle, openNotificationAlert, openNotificationExtras);
            openNotificationAlert = null;
        }

        if (notificationAlert != null) {
            transmitNotificationReceive(notificationTitle, notificationAlert, notificationExtras);
            notificationAlert = null;
        }

        if (!TextUtils.isEmpty(mRegistrationId)) {
            transmitGetRegistrationId(mRegistrationId);
            mRegistrationId = null;
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
            transmitNotificationOpen(openNotificationTitle, openNotificationAlert, openNotificationExtras);
            openNotificationAlert = null;
        }

        if (notificationAlert != null) {
            transmitNotificationReceive(notificationTitle, notificationAlert, notificationExtras);
            notificationAlert = null;
        }
    }

    public void stopPush(IWebview webView, JSONArray data) {
        JPushInterface.stopPush(webView.getContext());
    }

    public void resumePush(IWebview webView, JSONArray data) {
        JPushInterface.resumePush(webView.getContext());
    }

    public void isPushStopped(IWebview webView, JSONArray data) {
        try {
            String callbackId = data.getString(0);
            boolean isPushStopped = JPushInterface.isPushStopped(webView.getContext());
            if (isPushStopped) {
                JSUtil.execCallback(webView, callbackId, 1, JSUtil.OK, false);
            } else {
                JSUtil.execCallback(webView, callbackId, 0, JSUtil.OK, false);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    static void transmitGetRegistrationId(String rId) {
        if (shouldCacheMsg) {
            return;
        }

        if (mIWebView == null) {
            mRegistrationId = rId;
        } else {
            String format = "plus.Push.onGetRegistrationId(%s);";
            final String js = String.format(format, rId);

            mIWebView.getActivity().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mIWebView.loadUrl("javascript:" + js);
                }
            });
        }
    }

    static void transmitMessageReceive(String msg, Map<String, Object> extras) {
        if (shouldCacheMsg) {
            return;
        }

        if (mIWebView != null) {
            JSONObject data = getMessageObject(msg, extras);
            String format = "plus.Push.receiveMessageInAndroidCallback(%s);";
            final String js = String.format(format, data.toString());

            mIWebView.getActivity().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mIWebView.loadUrl("javascript:" + js);
                }
            });
        }
    }

    static void transmitNotificationOpen(String title, String alert, Map<String, Object> extras) {
        if (shouldCacheMsg) {
            return;
        }

        if (mIWebView != null) {
            JSONObject data = getNotificationObject(title, alert, extras);
            String format = "plus.Push.openNotificationInAndroidCallback(%s);";
            final String js = String.format(format, data.toString());

            mIWebView.getActivity().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mIWebView.loadUrl("javascript:" + js);
                }
            });

            openNotificationTitle = null;
            openNotificationAlert = null;
        }
    }

    static void transmitNotificationReceive(String title, String alert, Map<String, Object> extras) {
        if (shouldCacheMsg) {
            return;
        }

        if (mIWebView != null) {
            JSONObject data = getNotificationObject(title, alert, extras);
            String format = "plus.Push.receiveNotificationInAndroidCallback(%s);";
            final String js = String.format(format, data.toString());

            mIWebView.getActivity().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mIWebView.loadUrl("javascript:" + js);
                }
            });
            notificationTitle = null;
            notificationAlert = null;
        }
    }

    public void getRegistrationID(IWebview webView, JSONArray data) {
        try {
            String callbackId = data.getString(0);
            String regId = JPushInterface.getRegistrationID(webView.getActivity());
            JSUtil.execCallback(webView, callbackId, regId, JSUtil.OK, false);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    /**
     * 点击推送启动应用的时候原生会将该 notification 缓存起来，该方法用于获取缓存 notification
     * 注意：调用一次此方法后缓存清除，直到下一次点击推送后重新缓存notification
     */
    public void getLaunchAppCacheNotification(IWebview webView, JSONArray data) {
        try {
            String callbackId = data.getString(0);
            if(TextUtils.isEmpty(JPushReceiver.openNotificationAlert)){
                String notification=null;
                JSUtil.execCallback(webView, callbackId, notification, JSUtil.OK, false);
            }else {
                JSONObject notification = getNotificationObject(JPushReceiver.openNotificationTitle, JPushReceiver.openNotificationAlert, JPushReceiver.openNotificationExtras);
                JSUtil.execCallback(webView, callbackId, notification, JSUtil.OK, false);

            }
            clearLaunchAppCacheNotification(null,null);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    /**
     * 清除点击推送被缓存的notification
     */
    public void clearLaunchAppCacheNotification(IWebview webView, JSONArray data) {
        JPushReceiver.openNotificationTitle = null;
        JPushReceiver.openNotificationAlert = null;
    }

    public void addLocalNotification(IWebview webView, JSONArray data) {
        try {
            int builderId = data.getInt(1);
            String content = data.getString(2);
            String title = data.getString(3);
            int notificationId = data.getInt(4);
            int broadcastTime = data.getInt(5);
            String extrasStr = data.isNull(6) ? "" : data.getString(6);
            JSONObject extras = new JSONObject();
            if (!TextUtils.isEmpty(extrasStr)) {
                extras = new JSONObject(extrasStr);
            }

            JPushLocalNotification jLocalNoti = new JPushLocalNotification();
            jLocalNoti.setBuilderId(builderId);
            jLocalNoti.setContent(content);
            jLocalNoti.setTitle(title);
            jLocalNoti.setNotificationId(notificationId);
            jLocalNoti.setBroadcastTime(System.currentTimeMillis() + broadcastTime);
            jLocalNoti.setExtras(extras.toString());

            JPushInterface.addLocalNotification(webView.getActivity(), jLocalNoti);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void removeLocalNotification(IWebview webView, JSONArray data) {
        try {
            int notificationId = data.getInt(1);
            JPushInterface.removeLocalNotification(webView.getContext(), notificationId);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void clearLocalNotifications(IWebview webView, JSONArray data) {
        JPushInterface.clearLocalNotifications(webView.getContext());
    }

    public void clearAllNotification(IWebview webView, JSONArray data) {
        JPushInterface.clearAllNotifications(webView.getContext());
    }

    public void clearNotificationById(IWebview webView, JSONArray data) {
        int id;
        try {
            id = data.getInt(1);
            if (id != -1) {
                JPushInterface.clearNotificationById(webView.getActivity(), id);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void setTags(IWebview webView, JSONArray data) {
        try {
            JSONArray jsonArr = data.getJSONArray(1);
            HashSet<String> tags = new HashSet<String>();
            for (int i = 0, len = jsonArr.length(); i < len; i++) {
                tags.add(jsonArr.getString(i));
            }
            JPushInterface.setTags(webView.getContext(), tags, mTagAliasCallback);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void setAlias(IWebview webView, JSONArray data) {
        try {
            String alias = data.getString(1);
            JPushInterface.setAlias(webView.getContext(), alias, mTagAliasCallback);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void setTagsWithAlias(IWebview webView, JSONArray data) {
        HashSet<String> tags = new HashSet<String>();
        try {
            String alias = data.getString(1);
            JSONArray tagsJson = data.getJSONArray(2);
            for (int i = 0; i < tagsJson.length(); i++) {
                tags.add(tagsJson.getString(i));
            }
            JPushInterface.setAliasAndTags(webView.getContext(), alias, tags, mTagAliasCallback);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void setDebugMode(IWebview webView, JSONArray data) {
        try {
            boolean isOpenDebugMode = data.getBoolean(1);
            JPushInterface.setDebugMode(isOpenDebugMode);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    /**
     * 设置通知是否触发声音、震动、呼吸灯。
     * 方法中的"1"代表该设置的编号，需要服务器端同时指定要发送通知的 builderId = 1, 才会触发。
     * 可根据需要自行修改，具体请参考：http://docs.Push.io/client/android_tutorials/#_11
     */
    public void setBasicPushNotificationBuilder(IWebview webView, JSONArray data) {
        BasicPushNotificationBuilder builder = new BasicPushNotificationBuilder(
                webView.getContext());
        builder.developerArg0 = "Basic builder 1";
        JPushInterface.setPushNotificationBuilder(1, builder);
    }

    /**
     * 设置通知使用自定义样式, 具体使用方法同上。
     */
    public void setCustomPushNotificationBuilder(IWebview webView, JSONArray data) {
        // 需要自行修改
//		CustomPushNotificationBuilder builder = new CustomPushNotificationBuilder(
//				webView.getActivity(), R.layout.layout, R.id.icon, R.id.title,
//				R.id.text);
//		PushInterface.setPushNotificationBuilder(2, builder);
    }

    public void setLatestNotificationNum(IWebview webView, JSONArray data) {
        int num;
        try {
            num = data.getInt(1);
            if (num != -1) {
                JPushInterface.setLatestNotificationNumber(webView.getContext(), num);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void setPushTime(IWebview webView, JSONArray data) {
        try {
            String callbackId = data.getString(0);

            JSONArray weekDaysArr = data.isNull(1) ? null : data.getJSONArray(1);

            if (weekDaysArr == null || weekDaysArr.length() > 7) {
                JSUtil.execCallback(webView, callbackId, "允许推送日期设置不正确",
                        JSUtil.ERROR, false);
                return;
            }

            int startHour = data.getInt(1);
            if (isValidHour(startHour)) {
                JSUtil.execCallback(webView, callbackId, "允许推送开始时间设置不正确",
                        JSUtil.ERROR, false);
                return;
            }

            int endHour = data.getInt(2);
            if (isValidHour(endHour)) {
                JSUtil.execCallback(webView, callbackId, "允许推送结束时间设置不正确",
                        JSUtil.ERROR, false);
                return;
            }

            Set<Integer> weekDays = new HashSet<Integer>();
            for (int i = 0; i < weekDaysArr.length(); i++) {
                weekDays.add(weekDaysArr.getInt(i));
            }
            JPushInterface.setPushTime(webView.getContext(), weekDays, startHour, endHour);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void setSilenceTime(IWebview webView, JSONArray data) {
        try {
            String callbackId = data.getString(0);
            int startHour = data.getInt(1);
            int startMin = data.getInt(2);
            int endHour = data.getInt(3);
            int endMin = data.getInt(4);
            if (!isValidHour(startHour) || !isValidHour(endHour)
                    || !isValidMinute(startMin) || !isValidMinute(endMin)) {
                JSUtil.execCallback(webView, callbackId, "时间设置不正确",
                        JSUtil.ERROR, false);
                return;
            }
            JPushInterface.setSilenceTime(webView.getContext(), startHour,
                    startMin, endHour, endMin);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    /**
     * 用于 Android 6.0 以上系统申请权限，具体可参考：
     * http://docs.Push.io/client/android_api/#android-60
     */
    public void requestPermission(IWebview webView, JSONArray data) {
        JPushInterface.requestPermission(webView.getContext());
    }

    public void getConnectionState(IWebview webView, JSONArray data) throws JSONException {
        String callbackId = data.getString(0);
        boolean state = JPushInterface.getConnectionState(mApplicationContext);
        JSUtil.execCallback(webView, callbackId, String.valueOf(state), JSUtil.OK, false);
    }

    private static JSONObject getMessageObject(String msg, Map<String, Object> extras) {
        JSONObject data = new JSONObject();
        try {
            data.put("message", msg);
            JSONObject jExtras = new JSONObject();
            for (Entry<String, Object> entry : extras.entrySet()) {
                if (entry.getKey().equals("cn.JPush.android.EXTRA")) {
                    JSONObject jo = new JSONObject((String) entry.getValue());
                    String key;
                    Iterator<String> keys = jo.keys();
                    while (keys.hasNext()) {
                        key = keys.next();
                        jExtras.put(key, jo.getString(key));
                    }
                    jExtras.put("cn.JPush.android.EXTRA", jo);
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

    private static JSONObject getNotificationObject(String title, String alert,
                                                    Map<String, Object> extras) {
        JSONObject data = new JSONObject();
        try {
            data.put("title", title);
            data.put("alert", alert);
            JSONObject jExtras = new JSONObject();
            for (Entry<String, Object> entry : extras.entrySet()) {
                if (entry.getKey().equals("cn.JPush.android.EXTRA")) {
                    JSONObject jo = new JSONObject((String) entry.getValue());
                    String key;
                    Iterator<String> keys = jo.keys();
                    while (keys.hasNext()) {
                        key = keys.next();
                        jExtras.put(key, jo.getString(key));
                    }
                    jExtras.put("cn.JPush.android.EXTRA", jo);
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
                        "plus.Push.fireDocumentEvent('jpush.setTagsWithAlias', %s)",
                        data.toString());
                mIWebView.getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mIWebView.loadUrl("javascript:" + jsEvent);
                    }
                });
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    };

    private boolean isValidHour(int hour) {
        return !(hour < 0 || hour > 23);
    }

    private boolean isValidMinute(int min) {
        return !(min < 0 || min > 59);
    }
}
