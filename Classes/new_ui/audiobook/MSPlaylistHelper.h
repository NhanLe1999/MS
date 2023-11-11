//
//  MSPlaylistHelper.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/21/18.
//

#ifndef MSPlaylistHelper_hpp
#define MSPlaylistHelper_hpp

#include "MJStoryInfoModel.h"
#include "MSAudiobookPlayer.h"

namespace ms {
    namespace playlist {
        struct AudiobookMediaInfo {
            std::string title;
            std::string author;
            std::string cover_path;
            double duration;
            double elapsed_time;
        };
        
        enum ABState {
            ABStateReady,
            ABStateRunning = 1,
            ABStatePlaying = (1 << 1) | ABStateRunning,
            ABStateBuffering = (1 << 2) | ABStateRunning,
            ABStatePaused = (1 << 3) | ABStateRunning,
            ABStateStopped = (1 << 4),
            ABStateError = (1 << 5),
            ABStateDisposed = (1 << 6)
        };
        
        void play(std::vector<std::string> queue);
        void setupRemoteCommand();
        void setNowPlayingInfo(AudiobookMediaInfo info);
        void setNowPlayingInfo(mj::model::AudiobookInfo info, double elapsed_time);
        void resetNowPlayingInfo();
        
        class ABPlayer {
        public:
            static ABPlayer & getInstance();
            
            void init();
            void initInfor();
            void releasePlayer();
            void backStopPlayer();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            void beginReceivingRemoteEvents();
            void endReceivingRemoteEvents();
            bool isAudioSessionInterrupted(); // hieunt: check trạng thái interrupted của audio session, phục vụ cho việc nếu quay lại app trên iOS mà interrupt thì thoát app luôn
#endif
            
            void setOriginalQueue(std::vector<mj::model::AudiobookInfo> audiobooks);
            std::vector<mj::model::AudiobookInfo> getOriginalQueue();
            void enqueue(mj::model::AudiobookInfo info);
            void play();
            void resume();
            void pause();
            void next();
            void back();
            void onError(const char* i_msg);
            bool isSimplePlayerNull();

            void replaceSourceAtIndex(int index, std::string source);

            bool hasPreviousItem();
            bool hasNextItem();
            int getCurrentPlayingIdWithIndex(int i_index);
            bool hasPreviousItem(int i_id);
            bool hasNextItem(int i_id);
            
            void stop(std::function<void()> callback = nullptr);
            double getCurrentProgress();
            void seekToTime(double timestamp);
            
            void playFromBookId(int book_id, bool first_time = false,MSAudiobookPlayer::MSAudiobookPlayerType i_type =MSAudiobookPlayer::MSAudiobookPlayerType::NORMAL_LIST );
            
            ABState getState();
            int getCurrentPlayingId();
            std::vector<mj::model::AudiobookInfo> getQueuedAudiobooks();
            void setDuration(int duration);
            int getDuration() {return _playlist_duration;}

            void setStartTimestamp(long timestamp);
            long getStartTimestamp() { return _start_timestamp;}

            void didStartPlayingQueueItemId(int item_id,int i_index = -1, bool playWhenReady = true);
            void didFinishPlayingQueueItemId(int item_id);
            void didCancelQueuedItems();
            void onAppDidEnterForeground();
            
            bool isBackgroundState();
            bool checkInvalidState();
            void resetPlayer();
            
            std::vector<int> getNext5Ids();

            void clearPlaylist();      // Clear playlist and then update it after each change to "Audiobook list" order
            
        protected:
            explicit ABPlayer();
            
        private:
            std::function<void()> _stop_callback = nullptr;
            ABPlayer(ABPlayer const &);
            void operator = (ABPlayer const &);
            
            int _playlist_duration = 5;
            long _start_timestamp;
            bool _isAlertShowing = false;
            std::vector<mj::model::AudiobookInfo> _queue, _original_queue;

            CC_SYNTHESIZE( MSAudiobookPlayer::MSAudiobookPlayerType,m_currentAudioBookPlayerType,CurrentAudioBookPlayerType);



        };
    }
}

#endif
