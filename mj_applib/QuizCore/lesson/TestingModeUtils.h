//
//  TestingModeUtils.h
//  MJQuiz
//
//  Created by HuyPhuong on 11/21/17.
//

#ifndef TestingModeUtils_h
#define TestingModeUtils_h

#include <stdio.h>
#include "MathNetworking.h"

namespace testing {
    
    enum TestMode {
        game = 0,
        content,
        activity,
        stories
    };
    
    class TestManager {
        
    public:
        TestManager(){};
        ~TestManager(){};
        
        static TestManager* getInstance();
        
        void setTestMode(TestMode mode);
        TestMode getTestMode();
        
        void setGrade(int grade_id);
        int getGrade();
        
        void setClassname(std::string name);
        std::string getClassName();
        
        void setActivity(mj::network::api::Activity activity);
        mj::network::api::Activity getActivity();
        
    private:
        TestMode mode;
        mj::network::api::Activity activity;
        
        int grade_id;
        std::string class_name;
        
    };
    
}

#endif /* TestingModeUtils_h */
