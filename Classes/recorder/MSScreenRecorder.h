//
//  MSScreenRecorder.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/4/18.
//

#ifndef MSScreenRecorder_hpp
#define MSScreenRecorder_hpp

#include <functional>

namespace ms {
    namespace recorder {
        void startRecord(std::function<void()> callback = nullptr);
        void stopRecord();
        void cancelRecord();
        bool isRecording();
        
        bool isSupported();
        
        void enableMicro();
        void disableMicro();
    }
}

#endif /* MSScreenRecorder_hpp */
