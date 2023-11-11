package org.cocos2dx.cpp;

import android.util.Log;

import com.earlystart.android.monkeyjunior.story.FullscreenActivity;

public class QRReaderHelper {
    public static void startScan(int topButton){
        if (AppActivity._ac != null) {
            AppActivity._ac.startScan(topButton);
        }
        if (FullscreenActivity._ac != null) {
            FullscreenActivity._ac.startScan();
        }
    }

    public static void stopScan() {
        if (AppActivity._ac != null) {
            AppActivity._ac.stopScan();
        }
        if (FullscreenActivity._ac != null) {
            FullscreenActivity._ac.stopScan();
        }
    }

    public static String getQRLink() {
        if (FullscreenActivity._ac != null) {
            return FullscreenActivity._ac.getQRLink();
        }
        return "";
    }

    public static void parseDeepLink(String qrLink) {
        if (AppActivity._ac != null) {
            AppActivity._ac.parseShortLink(qrLink);
        }
    }

    public static String getDeepLink() {
        if (AppActivity._ac != null) {
            return AppActivity._ac.getDeepLink();
        }
        return "";
    }

    public static void cleanDeepLink() {
        if (AppActivity._ac != null) {
            AppActivity._ac.cleanDeepLink();
        }
    }

    public static void cleanQRLink() {
        if (FullscreenActivity._ac != null) {
            FullscreenActivity._ac.cleanQRLink();
        }
    }

    public static String getReadedDeepLink() {
        if (AppActivity._ac != null) {
            return AppActivity._ac.getReadedDeepLink();
        }
        return "";
    }

    public static void cleanReadedDeepLink()
    {
        if (AppActivity._ac != null) {
            AppActivity._ac.cleanReadedDeepLink();
        }
    }

    public static boolean isScanCloseClicked()
    {
        if (FullscreenActivity._ac != null) {
            return FullscreenActivity._ac.isScanCloseClicked();
        }
        return false;
    }

    public static boolean isScanLinkClicked()
    {
        if (FullscreenActivity._ac != null) {
            return FullscreenActivity._ac.isScanLinkClicked();
        }
        return false;
    }

    public static void stopCamera()
    {
        if (FullscreenActivity._ac != null) {
            FullscreenActivity._ac.stopCamera();
        }
    }

    public native static void onDeeplinkHandled(String i_deepLink);
    public native static void onScanClosed();
    public native static void setCloseButtonEnable();

}
