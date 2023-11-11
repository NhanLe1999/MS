
package org.cocos2dx.cpp;


import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Handler;
import android.support.v4.media.MediaDescriptionCompat;
import android.support.v4.media.session.MediaSessionCompat;
import android.util.Log;
import android.os.Build;

import androidx.annotation.Nullable;

import com.google.android.exoplayer2.C;
import com.google.android.exoplayer2.ExoPlayer;
import com.google.android.exoplayer2.MediaItem;
import com.google.android.exoplayer2.PlaybackException;
import com.google.android.exoplayer2.PlaybackParameters;
import com.google.android.exoplayer2.Player;
import com.google.android.exoplayer2.Timeline;
import com.google.android.exoplayer2.database.DatabaseProvider;
import com.google.android.exoplayer2.database.ExoDatabaseProvider;
import com.google.android.exoplayer2.ext.mediasession.MediaSessionConnector;
import com.google.android.exoplayer2.ext.mediasession.TimelineQueueNavigator;
import com.google.android.exoplayer2.source.ConcatenatingMediaSource;
import com.google.android.exoplayer2.source.MediaSource;
import com.google.android.exoplayer2.source.ProgressiveMediaSource;
import com.google.android.exoplayer2.trackselection.TrackSelector;
import com.google.android.exoplayer2.ui.PlayerNotificationManager;
import com.google.android.exoplayer2.upstream.DataSource;
import com.google.android.exoplayer2.upstream.DefaultDataSource;
import com.google.android.exoplayer2.upstream.DefaultHttpDataSource;
import com.google.android.exoplayer2.upstream.FileDataSource;
import com.google.android.exoplayer2.upstream.cache.Cache;
import com.google.android.exoplayer2.upstream.cache.CacheDataSource;
import com.google.android.exoplayer2.upstream.cache.NoOpCacheEvictor;
import com.google.android.exoplayer2.upstream.cache.SimpleCache;
import com.google.android.exoplayer2.util.Util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import io.hypertrack.smart_scheduler.Job;
import io.hypertrack.smart_scheduler.SmartScheduler;

class DescriptionAdapter implements PlayerNotificationManager.MediaDescriptionAdapter {
    @Override
    public String getCurrentContentTitle(Player player) {
        return AudiobookHelper.getTitleOfIndex(player.getCurrentMediaItemIndex());
    }

    @Nullable
    @Override
    public Bitmap getCurrentLargeIcon(Player player, PlayerNotificationManager.BitmapCallback callback) {
        String path = AudiobookHelper.getThumbOfIndex(player.getCurrentMediaItemIndex());
        if (path.isEmpty()) {
            return null;
        }

        BitmapFactory.Options opts = new BitmapFactory.Options();
        opts.inPreferredConfig = Bitmap.Config.ARGB_8888;
        try {
            return BitmapFactory.decodeStream(new FileInputStream(path), null, opts);
        } catch (FileNotFoundException e) {
            return null;
        }
    }

    @Nullable
    @Override
    public String getCurrentContentText(Player player) {
        return AudiobookHelper.getExtraOfIndex(player.getCurrentMediaItemIndex());
    }

    @Nullable
    @Override
    public PendingIntent createCurrentContentIntent(Player player) {
        Intent intent = new Intent(AppActivity._ac, AppActivity.class);
        final int flag =  Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE : PendingIntent.FLAG_UPDATE_CURRENT;
        return PendingIntent.getActivity(AppActivity._ac, 0, intent, flag);
    }
}

public class AudiobookHelper{
    public static ExoPlayer simplePlayer;
    private static TrackSelector trackSelector;
    public static ConcatenatingMediaSource playlist;
    private static DataSource.Factory dataSourceFactory =null;
    private static int currentIndex;
    public static PlayerNotificationManager playerNotificationManager = null;
    private static MediaSessionCompat mediaSessionCompat = null;
    public static Notification _notification = null;
    private static ArrayList<String> arr_title = null;
    private static ArrayList<String> arr_extra = null;
    private static ArrayList<String> arr_thumb = null;

    private static MediaSessionConnector mediaSessionConnector;

    private static int playlist_duration;
    private static long start_timestamp;
    private static long playerPostion = 0;

    public static final String CHANNEL_ID = "Audiobook Playlist";
    public static final int NOTIFICATION_ID = 1023;

    private static boolean isPaused = false;

    private static Cache downloadCache;
    private static File downloadDirectory;
    private static DatabaseProvider databaseProvider;
    protected static String userAgent;
    private static final String DOWNLOAD_CONTENT_DIRECTORY = "downloads";
    private static SeekDispatcher seekDispatcher;
    private static Timeline.Window currentWindow;
    private static int countPagePlay = 0, currentPlaybackState = 0;
    private static boolean isPlayWhenReady = false;
    private static Player.PositionInfo currentPositionInfo;

    static Timer timerUpdatePlayers = new Timer();
    static Handler timerHandler = new Handler();
    static Runnable timerRunnable = new Runnable() {

        @Override
        public void run() {
            if(simplePlayer!=null)
                playerPostion = simplePlayer.getCurrentPosition();
            timerHandler.postDelayed(this, 1000);
        }
    };
    static class UpdatePlayerPostionTask extends TimerTask {

        public void run() {
            //calculate the new position of myBall
            timerHandler.postDelayed(timerRunnable, 0);
        }
    }
    static void StartUpdatePlayerStatus(){
        timerUpdatePlayers = new Timer();
        timerUpdatePlayers.schedule(new UpdatePlayerPostionTask(),0, 1000 );
    }
    static void StopUpdatePlayerStatus(){
        if(timerUpdatePlayers!=null) {
            timerUpdatePlayers.cancel();
            timerUpdatePlayers.purge();
        }
    }
    public interface SeekDispatcher {

        boolean dispatchSeek(ExoPlayer player, int windowIndex, long positionMs);

    }

    public static final SeekDispatcher DEFAULT_SEEK_DISPATCHER = new SeekDispatcher() {

        @Override
        public boolean dispatchSeek(ExoPlayer player, int windowIndex, long positionMs) {
            player.seekTo(windowIndex, positionMs);
            return true;
        }

    };

    static void initMediaSessionCompat()
    {
        mediaSessionCompat = new MediaSessionCompat(AppActivity._ac.getApplicationContext(), "mediaSessionCompat");
        mediaSessionCompat.setActive(true);
    }

    static void setStartTimestamp(long value) {
        start_timestamp = System.currentTimeMillis() / 1000;
    }

    static void setPlaylistDuration(int value) {
        playlist_duration = value;
    }

    static void initInfor()
    {
        userAgent = Util.getUserAgent(AppActivity.getContext(), "ExoPlayerDemo");

        if(arr_title==null)
        {
            arr_title = new ArrayList<>();
        }

        if(arr_extra==null)
        {
            arr_extra = new ArrayList<>();
        }

        if(arr_thumb==null)
        {
            arr_thumb = new ArrayList<>();
        }

        if(playlist==null)
        {
            playlist = new ConcatenatingMediaSource();
        }

        if(dataSourceFactory==null)
        {
            dataSourceFactory = buildDataSourceFactory();
        }

        arr_title.clear();
        arr_extra.clear();
        arr_thumb.clear();
        playlist.clear();
    }

    static void init()
    {
        currentIndex = -1;

        if (simplePlayer == null)
        {
            currentWindow = new Timeline.Window();
            seekDispatcher = DEFAULT_SEEK_DISPATCHER;
            Player.Listener listener = new Player.Listener() {
                @Override
                public void onTimelineChanged(Timeline timeline, int reason) {
                }

//                @Override
//                public void  onTracksInfoChanged(TracksInfo tracksInfo) {
//                    Log.d("playlist", "track changed");
//                    if (isPlayWhenReady && currentPlaybackState == Player.STATE_READY)
//                    {
//                        // media actually playing
//                        didStartPlayingQueueId(simplePlayer.getCurrentMediaItemIndex());
//                    }
//                    else
//                    {
//                        // player paused in any state
//                    }
//                }

                @Override
                public void onPlayWhenReadyChanged(boolean playWhenReady, int reason) {
                    isPlayWhenReady = playWhenReady;
                }

                @Override
                public void onPlaybackStateChanged(int playbackState) {
                    currentPlaybackState = playbackState;
                    if (isPlayWhenReady && playbackState == Player.STATE_READY)
                    {
                        // media actually playing
                        didStartPlayingQueueId(simplePlayer.getCurrentMediaItemIndex());
                    }
                    else if (isPlayWhenReady)
                    {
                        if (playbackState == Player.STATE_ENDED)
                        {
                            didFinishPlayingQueueId(simplePlayer.getCurrentMediaItemIndex());
                        }

                        // might be idle (plays after prepare()),
                        // buffering (plays when data available)
                        // or ended (plays when seek away from end)
                    }
                    else
                    {
                        // player paused in any state
                    }

                    String stateString;
                    switch (playbackState) {
                        case ExoPlayer.STATE_IDLE:
                            stateString = "P16|ExoPlayer.STATE_IDLE      -";
                            break;
                        case ExoPlayer.STATE_BUFFERING:
                            stateString = "P16|ExoPlayer.STATE_BUFFERING -";
                            break;
                        case ExoPlayer.STATE_READY:
                            stateString = "P16|ExoPlayer.STATE_READY     -";
                            break;
                        case ExoPlayer.STATE_ENDED:
                            stateString = "P16|ExoPlayer.STATE_ENDED     -";
                            break;
                        default:
                            stateString = "P16|UNKNOWN_STATE             -";
                            break;
                    }
                    Log.d("P16|", "changed state to " + stateString
                            + " playWhenReady: " + isPlayWhenReady);
                }

                @Override
                public void onRepeatModeChanged(int repeatMode) {

                }

                @Override
                public void onShuffleModeEnabledChanged(boolean shuffleModeEnabled) {

                }

                @Override
                public void onPlayerError(PlaybackException error) {
                    onError(error.toString());
                    //error.type
                    Log.d("PX|onPlayerError", "error:"+error);
                    Log.d("PX|onPlayerError", "errorStr:"+error.toString());
                }


                @Override
                public void onPositionDiscontinuity(Player.PositionInfo oldPosition, Player.PositionInfo newPosition, int reason) {
                    currentPositionInfo = newPosition;
                    Log.d("P16X|", "changed state to " +  currentPlaybackState
                            + " playWhenReady: " + isPlayWhenReady);
                    if(reason==1)
                    {

                    }
                }


                @Override
                public void onPlaybackParametersChanged(PlaybackParameters playbackParameters) {

                }
            };
            simplePlayer = new ExoPlayer.Builder(AppActivity.getContext()).build();
            simplePlayer.addListener(listener);

            playerNotificationManager = new PlayerNotificationManager.Builder(AppActivity.getContext(), NOTIFICATION_ID,CHANNEL_ID).
                    setMediaDescriptionAdapter(new DescriptionAdapter()).
                    setNotificationListener(new PlayerNotificationManager.NotificationListener()
                    {
                        @Override
                        public  void onNotificationPosted(int notificationId, Notification notification, boolean ongoing) {
                            //startForeground(notificationId, notification);
                            _notification = notification;
                        }
                        @Override
                        public void onNotificationCancelled(int notificationId,boolean dismissedByUser) {
                            // stopSelf();
                        }
                    })
                    .build();
            //we will call two lines regarding mediaSsessionCompat in AppActivty.
            playerNotificationManager.setMediaSessionToken(mediaSessionCompat.getSessionToken());

            mediaSessionConnector = new MediaSessionConnector(mediaSessionCompat);
            mediaSessionConnector.setQueueNavigator(new TimelineQueueNavigator(mediaSessionCompat) {
                @Override
                public MediaDescriptionCompat getMediaDescription(Player player, int windowIndex) {

                    int currentWindow = player.getCurrentMediaItemIndex();

                    String path = AudiobookHelper.getThumbOfIndex(currentWindow);

                    if (path.isEmpty())
                    {
                        return null;
                    }

                    Bitmap bitmap;

                    BitmapFactory.Options opts = new BitmapFactory.Options();
                    opts.inPreferredConfig = Bitmap.Config.ARGB_8888;
                    try {
                        bitmap = BitmapFactory.decodeStream(new FileInputStream(path), null, opts);
                    } catch (FileNotFoundException e) {
                        bitmap = null;
                    }

                    MediaDescriptionCompat mediaDescription = new MediaDescriptionCompat.Builder()
                            .setTitle(AudiobookHelper.getTitleOfIndex(currentWindow))
                            .setDescription(AudiobookHelper.getExtraOfIndex(currentWindow))
                            .setSubtitle(AudiobookHelper.getExtraOfIndex(currentWindow))
                            .setIconBitmap(bitmap)
                            .build();

                    return mediaDescription;
                }
            });
            Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

            mainHandler.post(new Runnable() {

                @Override
                public void run() {
                    mediaSessionConnector.setPlayer(simplePlayer);
                    playerNotificationManager.setPlayer(simplePlayer);
                }
            });

            Intent servicePlayIntent = new Intent(AppActivity.getContext().getApplicationContext(), ExoPlayerAudioService.class);
            servicePlayIntent.putExtra("playStop", "initPlayer");
            AppActivity.getContext().getApplicationContext().startService(servicePlayIntent);

            //importance, we need to call  playerNotificationManager.setPlayer(simplePlayer) in MainThread;
        }
    }

    static void setPlayerInMainThread(final Player i_value)
    {
        if(playerNotificationManager==null)
        {
            return;
        }

        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                playerNotificationManager.setPlayer(i_value);
            }
        });
    }

    static void onStart()
    {

    }

    static void onStop()
    {

    }

    static void onBack()
    {
        onReleasePlayer();
    }
    static void onReleasePlayer(){
        if(isStopped() || simplePlayer== null || mediaSessionCompat!=null)
            return;
        if(mediaSessionCompat!=null)
        {
            mediaSessionCompat.release();
        }

        if(mediaSessionConnector!=null)
        {
            mediaSessionConnector.setPlayer(null);
        }

        setPlayerInMainThread(null);
        StopUpdatePlayerStatus();
        NotificationManager notificationManager = (NotificationManager) AppActivity.getContext().getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.cancel(AudiobookHelper.NOTIFICATION_ID);
        //didFinishPlayingQueueId(simplePlayer.getCurrentWindowIndex());
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                AppActivity.getContext().stopService(new Intent(AppActivity.getContext(), ExoPlayerAudioService.class));
                if (simplePlayer != null)
                {
                    //simplePlayer.removeListener();
                    simplePlayer.stop();
                    simplePlayer.clearMediaItems();
                    simplePlayer.release();
                    simplePlayer = null;
                }
            }
        });

    }
    static void onDestroy()
    {
        if(mediaSessionCompat!=null)
        {
            mediaSessionCompat.release();
        }

        if(mediaSessionConnector!=null)
        {
            mediaSessionConnector.setPlayer(null);
        }

        setPlayerInMainThread(null);

        NotificationManager notificationManager = (NotificationManager) AppActivity.getContext().getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.cancel(AudiobookHelper.NOTIFICATION_ID);

        releasePlayer();

    }

    static DataSource.Factory buildDataSourceFactory()
    {
        DefaultDataSource.Factory upstreamFactory = new DefaultDataSource.Factory(AppActivity.getContext(),
                new DefaultHttpDataSource.Factory().
                        setUserAgent(userAgent).
                        setConnectTimeoutMs(DefaultHttpDataSource.DEFAULT_CONNECT_TIMEOUT_MILLIS).
                        setReadTimeoutMs(DefaultHttpDataSource.DEFAULT_READ_TIMEOUT_MILLIS).setAllowCrossProtocolRedirects(true));

        return buildReadOnlyCacheDataSource(upstreamFactory, getDownloadCache());
    }

    protected static DataSource.Factory buildReadOnlyCacheDataSource(DataSource.Factory upstreamFactory, Cache cache) {
        return new CacheDataSource.Factory().
                setCache(cache).
                setUpstreamDataSourceFactory(upstreamFactory).
                setCacheReadDataSourceFactory(new FileDataSource.Factory()).
                setCacheWriteDataSinkFactory(null).
                setFlags(CacheDataSource.FLAG_IGNORE_CACHE_ON_ERROR).
                setEventListener(null);
    }

    static synchronized Cache getDownloadCache() {
        if (downloadCache == null) {
            File downloadContentDirectory = new File(getDownloadDirectory(), DOWNLOAD_CONTENT_DIRECTORY);
            downloadCache =
                    new SimpleCache(downloadContentDirectory, new NoOpCacheEvictor(), getDatabaseProvider());
        }
        return downloadCache;
    }

    static DatabaseProvider getDatabaseProvider()
    {
        if (databaseProvider == null) {
            databaseProvider = new ExoDatabaseProvider(AppActivity.getContext());
        }
        return databaseProvider;
    }

    static File getDownloadDirectory()
    {
        if (downloadDirectory == null)
        {
            downloadDirectory =  AppActivity._ac.getExternalFilesDir(null);
            if (downloadDirectory == null) {
                downloadDirectory = AppActivity._ac.getFilesDir();
            }
        }
        return downloadDirectory;
    }

    static void enqueue(String path, boolean isLocal, String name, String extra, String thumb)
    {
        //Log.d("playlist", path);
        MediaSource audioSource = buildMediaSource(Uri.parse(path));
        playlist.addMediaSource(audioSource);
        if(arr_title!=null)
            arr_title.add(name);
        if(arr_title!=null)
            arr_extra.add(extra);
        if(arr_title!=null)
            arr_thumb.add(thumb);
    }

    private static MediaSource buildMediaSource(Uri uri)
    {
        @C.ContentType int type = Util.inferContentType(uri, null);
        switch (type)
        {
            case C.TYPE_OTHER:
                return new ProgressiveMediaSource.Factory(dataSourceFactory).createMediaSource(MediaItem.fromUri(uri));
            default:
                throw new IllegalStateException("Unsupported type: " + type);
        }
    }

    static void play()
    {
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {

                if(simplePlayer == null) return;
                Intent servicePlayIntent = new Intent(AppActivity.getContext(), ExoPlayerAudioService.class);
                servicePlayIntent.putExtra("playStop", "play");
                AppActivity.getContext().startService(servicePlayIntent);
                isPaused = false;
            }
        });
        StartUpdatePlayerStatus();
    }

    static void playFromIndex(final int index, boolean first_time)
    {
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {

                if(simplePlayer == null) {
                    init();
                }
                isPaused = false;
                //simplePlayer.seekTo(index, C.TIME_UNSET);
                Intent servicePlayIntent = new Intent(AppActivity.getContext(), ExoPlayerAudioService.class);
                servicePlayIntent.putExtra("playStop", "playFromIndex");
                servicePlayIntent.putExtra("playIndex", index);

                AppActivity.getContext().startService(servicePlayIntent);
                StartUpdatePlayerStatus();
            }
        });

    }

    static void resume()
    {
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {

                if(simplePlayer == null) return;

                isPaused = false;

                simplePlayer.setPlayWhenReady(true);
            }
        });
    }

    static void pause()
    {
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {

                if(simplePlayer == null) return;

                isPaused = true;

                simplePlayer.setPlayWhenReady(false);
            }
        });
    }

    static boolean isStopped() {
        if (simplePlayer != null) {
            if ( currentPlaybackState == Player.STATE_ENDED ||  currentPlaybackState == Player.STATE_IDLE)
                return true;

            return false;
        }
        return true;
    }

    static boolean isPlaying() {

        return (simplePlayer!=null && ( currentPlaybackState == Player.STATE_READY && isPlayWhenReady));

    }

    static boolean playerPaused() {
        return isPaused;
    }

    static void replaceSourceAtIndex(int index, String new_path)
    {
//        Log.d("playlist", "replace source at index " + index + ": " + new_path);
//        playlist.removeMediaSource(index);
//        playlist.addMediaSource(index, new ExtractorMediaSource.Factory(dataSourceFactory).createMediaSource(Uri.fromFile(new File(new_path))));
    }

    static void releasePlayer()
    {
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                if (simplePlayer != null)
                {
                    //simplePlayer.removeListener();
                    simplePlayer.stop();
                    simplePlayer.clearMediaItems();
                    simplePlayer.release();
                    simplePlayer = null;
                }

                if(arr_title!=null) arr_title.clear();
                if(arr_extra!=null) arr_extra.clear();
                if(arr_thumb!=null) arr_thumb.clear();
                if(playlist!=null) playlist.clear();
            }
        });

    }

    static void clearPlaylist() {
        // clear play list
        // This function must be called after each change to "Audiobook list" to update "Audiobook list"
        // Change "Audiobook list" means dragging item.
        if(playlist!=null)
            playlist.clear();
    }

    static void stop()
    {
        if(isStopped())
            return;
        //didFinishPlayingQueueId(simplePlayer.getCurrentWindowIndex());
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());
        StopUpdatePlayerStatus();
        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                AppActivity.getContext().stopService(new Intent(AppActivity.getContext(), ExoPlayerAudioService.class));
                onDestroy();
            }
        });
    }

    static boolean isSimplePlayerNull()
    {
        return (simplePlayer == null);
    }

    static int getCurrentIndex() {

        return ((simplePlayer != null) ? currentPositionInfo.mediaItemIndex : 0);
    }

    static String getTitleOfIndex(int index)
    {
        if (index < arr_title.size())
            return arr_title.get(index);

        return "title not found";
    }

    static String getExtraOfIndex(int index)
    {
        if (index < arr_extra.size())
            return arr_extra.get(index);

        return "extra not found";
    }

    static String getThumbOfIndex(int index)
    {
        if (index < arr_thumb.size())
            return arr_thumb.get(index);

        return "";
    }

    static double getCurrentPosition()
    {
        return ((simplePlayer == null)  ? 0 :(playerPostion / 1000.0));
    }

    static void next() {

        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                Timeline currentTimeline = simplePlayer.getCurrentTimeline();

                if (currentTimeline.isEmpty())
                {
                    return;
                }

                int currentWindowIndex = simplePlayer.getCurrentMediaItemIndex();

                if (currentWindowIndex < currentTimeline.getWindowCount() - 1)
                {
                    seekDispatcher.dispatchSeek(simplePlayer, currentWindowIndex + 1, C.TIME_UNSET);
                }
                else if (currentTimeline.getWindow(currentWindowIndex, currentWindow, 0).isDynamic)
                {
                    seekDispatcher.dispatchSeek(simplePlayer, currentWindowIndex, C.TIME_UNSET);
                }

            }
        });

    }

    static void back()
    {
        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                Timeline currentTimeline = simplePlayer.getCurrentTimeline();

                if (currentTimeline.isEmpty())
                {
                    return;
                }

                int currentWindowIndex = simplePlayer.getCurrentMediaItemIndex();

                currentTimeline.getWindow(currentWindowIndex, currentWindow);

                if (currentWindowIndex > 0 )
                {
                    seekDispatcher.dispatchSeek(simplePlayer, currentWindowIndex - 1, C.TIME_UNSET);
                }
                else
                {
                    seekDispatcher.dispatchSeek(simplePlayer, simplePlayer.getCurrentMediaItemIndex(), 0);
                }

            }
        });

    }

    static void seekToTime(final int timestamp) {

        Handler mainHandler = new Handler(AppActivity.getContext().getMainLooper());

        mainHandler.post(new Runnable() {

            @Override
            public void run() {
                if(simplePlayer == null) return;
                simplePlayer.seekTo(timestamp);

            }
        });
    }

    static void scheduleOff(int minutes) {
        SmartScheduler.JobScheduledCallback callback = new SmartScheduler.JobScheduledCallback() {
            @Override
            public void onJobScheduled(Context context, Job job) {
                stop();
            }
        };
        Job.Builder builder = new Job.Builder(10192, callback, Job.Type.JOB_TYPE_NONE, "")
                .setRequiredNetworkType(Job.NetworkType.NETWORK_TYPE_ANY)
                .setRequiresCharging(false)
                .setIntervalMillis(minutes * 60 * 1000);
        Job job = builder.build();
        SmartScheduler jobScheduler = SmartScheduler.getInstance(AppActivity.getContext());
        jobScheduler.addJob(job);
    }

    static int getCurrentState()
    {
        if(simplePlayer==null) return -1;


        if (isPlayWhenReady &&  currentPlaybackState == Player.STATE_READY)
        {
            // media actually playing
            return 1;
        }
        else if (isPlayWhenReady)
        {
            if (currentPlaybackState == Player.STATE_ENDED)
            {
                return 2;
            }
            // might be idle (plays after prepare()),
            // buffering (plays when data available)
            // or ended (plays when seek away from end)
        }
        else
        {
            // player paused in any state
            return 0;
        }

        return  -1;
    }

    static native void didStartPlayingQueueId(int item_id);
    static native void didFinishPlayingQueueId(int item_id);
    static native int getLimitTimePlay();
    static native long getStartTimePlay();
    static native void onError(String i_msg);

}


