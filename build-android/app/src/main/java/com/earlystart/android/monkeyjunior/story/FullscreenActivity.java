package com.earlystart.android.monkeyjunior.story;

import android.app.Activity;
import android.graphics.Color;
import android.hardware.Camera;
import android.os.Build;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;


import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.QRReaderHelper;
import org.cocos2dx.lib.ResizeLayout;

import java.util.List;

import github.nisrulz.qreader.QRDataListener;
import github.nisrulz.qreader.QREader;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class FullscreenActivity extends Activity {
    public static FullscreenActivity _ac;

    private SurfaceView mySurfaceView;
    private QREader qrEader = null;

    private String detectedString = "";

    private Boolean scanCloseClicked = false;
    private Boolean scanLinkClicked = false;
    private Boolean isCloseButtonEnable = false;

    private int currentApiVersion;

    private final String QRCODE_LINK_STRUCTURE = "https://monkeystories.page.link";
    private final String QRCODE_LINK_STRUCTURE_VM = "https://vmonkey.page.link";
    private Camera mCamera = null;

    private boolean s_isFinishCalled = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //Log.d("QRCode", "created");
        super.onCreate(savedInstanceState);
        _ac = FullscreenActivity.this;

        detectedString = "";

        currentApiVersion = android.os.Build.VERSION.SDK_INT;
        final int flags = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;

        // This work only for android 4.4+
        if(currentApiVersion >= Build.VERSION_CODES.KITKAT)
        {

            getWindow().getDecorView().setSystemUiVisibility(flags);

            // Code below is to handle presses of Volume up or Volume down.
            // Without this, after pressing volume buttons, the navigation bar will
            // show up and won't hide
            final View decorView = getWindow().getDecorView();
            decorView
                    .setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener()
                    {
                        @Override
                        public void onSystemUiVisibilityChange(int visibility)
                        {
                            if((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0)
                            {
                                decorView.setSystemUiVisibility(flags);
                            }
                        }
                    });
        }

        ViewGroup.LayoutParams framelayout_params =
                new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT);

        ResizeLayout frameLayout = new ResizeLayout(this);
        frameLayout.setForegroundGravity(Gravity.NO_GRAVITY);

        frameLayout.setLayoutParams(framelayout_params);
        setContentView(frameLayout);
        setTheme(R.style.FullscreenTheme);

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        int mDisplayWidth = metrics.widthPixels;
        int mDisplayHeight = metrics.heightPixels;

        int closeSize = (int)(mDisplayHeight*0.17f);
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(closeSize, closeSize,Gravity.TOP | Gravity.END);
        final Button closeButton = new Button(this);
        closeButton.setBackgroundColor(Color.argb(0, 0, 0, 0));
        closeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Log.d("FullAc|", "setOnClickListener");

                closeButton.setEnabled(false);

                if(isCloseButtonEnable && qrEader.isCameraRunning())
                {
                    qrEader.stop();

                    AppActivity._ac.shouldPause = true;
                    AppActivity._ac.s_isFullScreenActivityPause = false;
                    AppActivity._ac.cleanDeepLink();
                    cleanQRLink();
                    s_isFinishCalled = true;
                    QRReaderHelper.onScanClosed();
                    finish();
                }
            }
        });
        frameLayout.addView(closeButton, params);

        int linkWidth = (int)(mDisplayWidth*0.3f);
        int linkHeight = (int)(mDisplayHeight*0.12f);

        FrameLayout.LayoutParams params2 = new FrameLayout.LayoutParams(linkWidth, linkHeight,Gravity.TOP | Gravity.START);
        params2.leftMargin = (int)(mDisplayWidth*0.57f);
        params2.topMargin =  100;// (int)(((768-AppActivity._ac.qrButtonTop) / 768f) * mDisplayHeight);
        Button linkButton = new Button(this);
        linkButton.setBackgroundColor(Color.argb(0, 0, 0, 0));
        linkButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scanLinkClicked = true;
            }
        });
        frameLayout.addView(linkButton, params2);

//        if (!AppActivity._ac.getPermissionToUseCamera()) {
//            AppActivity._ac.requestCameraPermission();
//            finish();
//            return;
//        }
        // Init SurfaceView
        // ------------
        mySurfaceView = new SurfaceView(this);

        int padding = (int)(mDisplayHeight * 0.03f);
        int previewWidth = (int)(mDisplayWidth * 0.36f)-padding;
        int previewWHeight = mDisplayHeight-padding*2;

        FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        lParams.leftMargin = padding;
        lParams.topMargin = padding;
        lParams.width = previewWidth;
        lParams.height = previewWHeight;
        lParams.gravity = Gravity.TOP | Gravity.START;

        mySurfaceView.setZOrderMediaOverlay(true);
        mySurfaceView.setZOrderOnTop(true);
        mySurfaceView.setLayoutParams(lParams);
        frameLayout.addView(mySurfaceView);

        try
        {
            releaseCameraAndPreview();

            int cameraID = findCamera();

            if(cameraID < 0)
            {
                AppActivity._ac.toastOnUIThread(AppActivity._ac.isVietnameseLanguage()?" Không tìm thấy camera. Vui lòng kiểm tra lại. ":"Camera not found. Please try again.");

                return;
            }

            mCamera = Camera.open(findCamera());

            if(mCamera ==null)
            {
                //Log.e(getString(R.string.app_name), "failed to open Camera");
                try
                {
                    AppActivity._ac.toastOnUIThread(AppActivity._ac.isVietnameseLanguage()?"Có thể camera đang được sử dụng bởi ứng dụng khác hoặc camera có vấn đề. Vui lòng kiểm tra lại. ":"Your camera has been occupied by other apps. Please release and try again.");
                }
                catch (Exception e)
                {
                    Log.e(getString(R.string.app_name), "cannot toast !");
                    e.printStackTrace();
                }

                return;
            }

            //AppActivity._ac.toastOnUIThread(AppActivity._ac.isVietnameseLanguage()?"Có thể camera đang được sử dụng bởi ứng dụng khác hoặc camera có vấn đề. Vui lòng kiểm tra lại. ":"Your camera has been occupied by other apps. Please release and try again.");

            Camera.Size cs = getOptimalPreviewSize(mCamera.getParameters().getSupportedPreviewSizes(), lParams.width, lParams.height);
            previewWidth = cs.width;
            previewWHeight = cs.height;

            // Init QREader
            // ------------------------------------------------------------------------------------------------

            qrEader = new QREader.Builder(this, mySurfaceView, new QRDataListener() {
                @Override
                public void onDetected(final String data)
                {
                    if(detectedString == data && data.contains(QRCODE_LINK_STRUCTURE))
                    {
                        return;
                    }

                    if (!s_isFinishCalled && data != detectedString && data.contains(QRCODE_LINK_STRUCTURE))
                    {
                        detectedString = data;
                        AppActivity._ac.parseShortLinkQRCameraCode(data);
                        //Log.d("FullAc|", "finishFromHandle");
                        s_isFinishCalled =true;
                        finish();
                    }
                }
            }).facing(QREader.BACK_CAM)
                    .enableAutofocus(true)
                    .height(previewWHeight)
                    .width(previewWidth)
                    .build();
        }
        catch (Exception e)
        {
            Log.e(getString(R.string.app_name), "Failed to open Camera or qrReader create failed");
            e.printStackTrace();
        }
    }

    private void releaseCameraAndPreview()
    {
        if (mCamera != null) {
            mCamera.release();
            mCamera = null;
        }
    }

    private int findCamera()
    {
        int cameraId = -1;

        // find back camera first.
        int numberOfCameras = Camera.getNumberOfCameras();

        for (int i = 0; i < numberOfCameras; i++)
        {
            Camera.CameraInfo info = new Camera.CameraInfo();
            Camera.getCameraInfo(i, info);
            if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK)
            {
                cameraId = i;
                break;
            }
        }

        if(cameraId==-1) //cannot found the back camrea, so we need to find the front camera.
        {
            for (int i = 0; i < numberOfCameras; i++)
            {
                Camera.CameraInfo info = new Camera.CameraInfo();
                Camera.getCameraInfo(i, info);
                if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT)
                {
                    cameraId = i;
                    break;
                }
            }
        }

        return cameraId;
    }

    private Camera.Size getOptimalPreviewSize(List<Camera.Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.1;
        double targetRatio = (double) w/h;

        if (sizes==null) return null;

        Camera.Size optimalSize = null;

        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        // Find size
//        for (Camera.Size size : sizes) {
//            double ratio = (double) size.width / size.height;
//
//            //Log.d("Camera", " 1 : "+ratio);
//            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;
//
//            if (Math.abs(size.height - targetHeight) < minDiff) {
//                optimalSize = size;
//                minDiff = Math.abs(size.height - targetHeight);
//            }
//        }

        //find nearest size
//        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Camera.Size size : sizes) {
                double ratio = (double) size.width / size.height;
                if (Math.abs(ratio - targetRatio) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(ratio - targetRatio);
                };

            }
//        }

        return optimalSize;
    }

    @Override
    protected void onResume() {
        super.onResume();

       // Log.d("FullAc|", "onResume");

        if (qrEader != null)
            qrEader.initAndStart(mySurfaceView);

        isCloseButtonEnable = true;
        AppActivity._ac.s_isFullScreenActivityPause = false;
        QRReaderHelper.setCloseButtonEnable();
    }

    @Override
    protected void onPause() {
        super.onPause();

       // Log.d("FullAc|", "onPause");

        if (qrEader != null)
        {
            if(qrEader.isCameraRunning())
            {
                qrEader.stop();
            }
            qrEader.releaseAndCleanup();
        }

        if(!s_isFinishCalled)
        {
            AppActivity._ac.s_isFullScreenActivityPause = true;
            finish();
        }
    }

    public void startScan() {}

    public void stopScan() {

        if (qrEader != null)
        {
            if(qrEader.isCameraRunning())
            {
                qrEader.stop();
            }
            qrEader.releaseAndCleanup();

            qrEader = null;
        }

        finish();
        _ac = null;
    }

    public void stopCamera()
    {
        AppActivity._ac.s_isFullScreenActivityPause = false;
        s_isFinishCalled = true;
        finish();
    }

    public String getQRLink()
    {
        return detectedString;
    }

    public void cleanQRLink() {
        detectedString = "";
    }

    public boolean isScanCloseClicked () {
        QRReaderHelper.onScanClosed();
        cleanQRLink();
        return false;
    }

    public boolean isScanLinkClicked () {
        if (scanLinkClicked) {
            scanLinkClicked = false;
            return true;
        }
        return false;
    }

    @Override
    public void onBackPressed() {

        AppActivity._ac.s_isFullScreenActivityPause = false;
        s_isFinishCalled = true;
        stopScan();
        QRReaderHelper.onScanClosed();
        super.onBackPressed();
    }

}
