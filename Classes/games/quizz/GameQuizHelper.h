//
//  PreKGames.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/5/17.
//
//

#ifndef GameQuizHelper_h
#define GameQuizHelper_h

/**----> QUIZES <----**/
#include "TestingModeUtils.h"

#include "BA_HowManyKindQuiz.hpp"
#include "ChooseCorrectGroupQuiz.h"
#include "ChooseCorrectObjectsQuiz.h"
#include "ColorPictureQuiz.h"
#include "ColorUseKeysQuiz.h"
#include "CountAndAnswerQuiz.h"
#include "EscapeMazeQuiz.h"
#include "Huy_ReadingNumber.h"
#include "MoveNumberObjectToGroupQuiz.h"
#include "My_MissingNumber.h"
#include "SortedToGroupQuiz.h"
#include "TracingQuiz.h"


#define k_BA_HowManyKindQuiz "BA_HowManyKindQuiz"
#define k_ChooseCorrectGroupQuiz "ChooseCorrectGroupQuiz"
#define k_ChooseCorrectObjectsQuiz "ChooseCorrectObjectsQuiz"
#define k_ColorPictureQuiz "ColorPictureQuiz"
#define k_ColorUseKeysQuiz "ColorUseKeysQuiz"
#define k_CountAndAnswerQuiz "CountAndAnswerQuiz"
#define k_EscapeMazeQuiz "EscapeMazeQuiz"
#define k_Huy_ReadingNumber "Huy_ReadingNumber"
#define k_MoveNumberObjectToGroupQuiz "MoveNumberObjectToGroupQuiz"
#define k_My_MissingNumber "My_MissingNumber"
#define k_SortedToGroupQuiz "SortedToGroupQuiz"
#define k_TracingQuiz "TracingQuiz"


namespace math {
    namespace quiz {
        static Math_Quiz* createQuiz (mj::model::Game _game) {
            std::string game_classify= _game.getClassName();//  math_statistic->getGameType(_game);
            if (testing::TestManager::getInstance()->getTestMode() == testing::activity || testing::TestManager::getInstance()->getTestMode() == testing::stories) {
                game_classify= testing::TestManager::getInstance()->getClassName();
            }
            
            if (game_classify.compare(k_BA_HowManyKindQuiz)==0) {
                auto quiz=BA_HowManyKindQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ChooseCorrectGroupQuiz)==0) {
                auto quiz=ChooseCorrectGroupQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ChooseCorrectObjectsQuiz)==0) {
                auto quiz=ChooseCorrectObjectsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ColorPictureQuiz)==0) {
                auto quiz=ColorPictureQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ColorUseKeysQuiz)==0) {
                auto quiz=ColorUseKeysQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_CountAndAnswerQuiz)==0) {
                auto quiz=CountAndAnswerQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_EscapeMazeQuiz)==0) {
                auto quiz=EscapeMazeQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_Huy_ReadingNumber)==0) {
                auto quiz=Huy_ReadingNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MoveNumberObjectToGroupQuiz)==0) {
                auto quiz=MoveNumberObjectToGroupQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_My_MissingNumber)==0) {
                auto quiz=My_MissingNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_SortedToGroupQuiz)==0) {
                auto quiz=SortedToGroupQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_TracingQuiz)==0) {
                auto quiz=TracingQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            return nullptr;
        }
    }
}

#endif /* PreKGames_h */
