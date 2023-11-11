package org.cocos2dx.cpp;

import static org.cocos2dx.cpp.AudiobookHelper._notification;
import static org.cocos2dx.cpp.AudiobookHelper.getLimitTimePlay;
import static org.cocos2dx.cpp.AudiobookHelper.playlist;
import static org.cocos2dx.cpp.AudiobookHelper.simplePlayer;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.CountDownTimer;
import android.os.IBinder;
import android.os.PowerManager;

import androidx.annotation.Nullable;

import com.google.android.exoplayer2.C;
import com.google.android.exoplayer2.ExoPlayer;

/*Using MyAudioService to play exoplayer in foreground service, enable lockWiFi lockCPU to allway play*/
public class ExoPlayerAudioService extends Service implements AudioManager.OnAudioFocusChangeListener {

    private WifiManager.WifiLock mWiFiLock;
    private PowerManager.WakeLock mWakeLock;
    private AudioManager audioManager;
    private ExoPlayer player;
    CountDownTimer timerLimitPlayer = null;
    @Override
    public void onCreate() {
        audioManager = (AudioManager) getApplicationContext().getSystemService(AUDIO_SERVICE);
        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        player = simplePlayer;
        String input = intent.getStringExtra("playStop");
        if (input != null && input.equals("initPlayer")) {
            if (requestFocus()) {
                initPlayer();
            }
        }

        if (input != null && input.equals("play")) {
            if (requestFocus()) {
                play();
            }
        }
        else if (input != null && input.equals("playFromIndex")) {
            int index = intent.getIntExtra("playIndex", 0);
            if (requestFocus()) {
                playFromIndex(index);
            }
        }
        else {

        }
        if(_notification!=null)
            startForeground(1, _notification);
        return START_NOT_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        stop();
        destroyPlayer();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onAudioFocusChange(int focusChange) {
//        if (focusChange == AudioManager.AUDIOFOCUS_LOSS_TRANSIENT || focusChange == AudioManager.AUDIOFOCUS_LOSS) {
//            stop();
//            System.exit(0);
//        }
    }

    private boolean requestFocus() {
        return (audioManager.requestAudioFocus(this, AudioManager.STREAM_MUSIC, AudioManager.AUDIOFOCUS_GAIN) == AudioManager.AUDIOFOCUS_REQUEST_GRANTED);
    }

    private void play() {
        if(player== null)
        {
            if(simplePlayer!=null)
                player = simplePlayer;
            else
                return;
        }


        player.setForegroundMode(true);
        player.setPlayWhenReady(true);
    }
    private void playFromIndex(int index) {
        if(player== null)
        {
            if(simplePlayer!=null)
                player = simplePlayer;
            else
                return;
        }
        player.setForegroundMode(true);
        player.seekTo(index, C.TIME_UNSET);
        player.setPlayWhenReady(true);
        int durationTimeInsecond = getLimitTimePlay()*1000;
        timerLimitPlayer = new CountDownTimer(durationTimeInsecond, 1000) {
            public void onTick(long millisUntilFinished) {
            }

            public void onFinish() {
                if(player!=null)
                    player.setPlayWhenReady(false);

                if(!AudiobookHelper.isStopped())
                    AudiobookHelper.onReleasePlayer();
            }
        }.start();

    }
    private void stop() {
        if(player==null)
            return;
        player.setPlayWhenReady(false);
        player.stop();
        player = null;
    }

    private void initPlayer() {
        if (player==null)
            return;
        player.setMediaSource(playlist);
        player.setMediaSource(playlist, false);
        player.prepare();
        lockWiFi();
        lockCPU();
    }

    private void destroyPlayer() {

        if(timerLimitPlayer!=null)
            timerLimitPlayer.cancel();
        unlockCPU();
        unlockWiFi();
    }

    private void lockCPU() {
        PowerManager mgr = (PowerManager) getSystemService(Context.POWER_SERVICE);
        if (mgr == null) {
            return;
        }
        mWakeLock = mgr.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, this.getClass().getSimpleName());
        mWakeLock.acquire(getLimitTimePlay());
    }

    private void unlockCPU() {
        if (mWakeLock != null && mWakeLock.isHeld()) {
            mWakeLock.release();
            mWakeLock = null;
        }
    }

    private void lockWiFi() {
        ConnectivityManager connManager = (ConnectivityManager) getApplicationContext().getSystemService(CONNECTIVITY_SERVICE);
        if (connManager == null) {
            return;
        }
        NetworkInfo lWifi = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        if (lWifi != null && lWifi.isConnected()) {
            WifiManager manager = ((WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE));
            if (manager != null) {
                mWiFiLock = manager.createWifiLock(WifiManager.WIFI_MODE_FULL, ExoPlayerAudioService.class.getSimpleName());
                mWiFiLock.acquire();
            }
        }
    }

    private void unlockWiFi() {
        if (mWiFiLock != null && mWiFiLock.isHeld()) {
            mWiFiLock.release();
            mWiFiLock = null;
        }
    }

}
