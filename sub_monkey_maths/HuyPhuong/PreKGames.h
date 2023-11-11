//
//  PreKGames.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/5/17.
//
//

#ifndef PreKGames_h
#define PreKGames_h

/**----> QUIZES <----**/
#include "IntroQuiz.h"

#include "CountAndAnswerQuiz.h"
#include "CountAndDragQuiz.h"
#include "CountRowAndDragQuiz.h"

#include "SortedToGroupQuiz.h" //Bỏ option: drag number
#include "MoveNumberObjectToGroupQuiz.h" //Etd: Sorted game: Opt: drag number
#include "DragBetweenTwoGroupsQuiz.h"
#include "MoveCorrectObjectToBoxQuiz.h"
#include "SortObjectInOrderQuiz.h"
#include "MoveFruitToBagQuiz.h"

#include "ChooseCorrectGroupQuiz.h"
#include "ChooseCorrectObjectsQuiz.h"

#include "CompareGroupSizeQuiz.h"
#include "CompareNumberOfObjectsQuiz.h"
#include "CompareTwoObjectsQuiz.h"
#include "CompareWeightTwoObjectsQuiz.h"

#include "PatternCopyQuiz.h"
#include "PatternComeNextQuiz.h"
#include "PatternColorQuiz.h"

#include "WhatObjectIsThisQuiz.h"

#include "TracingQuiz.h"
#include "MoveObjectMakePictureQuiz.h"
#include "ConnectDotsQuiz.h"
#include "MiniSudokuQuiz.h"

#include "ColorUseKeysQuiz.h"
#include "ColorPictureQuiz.h"
#include "MakeEnoughQuiz.h"
#include "EscapeMazeQuiz.h"


#define k_CountAndAnswerQuiz "CountAndAnswerQuiz"
#define k_CountAndDragQuiz "CountAndDragQuiz"
#define k_CountRowAndDragQuiz "CountRowAndDragQuiz"

#define k_SortedToGroupQuiz "SortedToGroupQuiz"
#define k_MoveNumberObjectToGroupQuiz "MoveNumberObjectToGroupQuiz"
#define k_DragBetweenTwoGroupsQuiz "DragBetweenTwoGroupsQuiz"
#define k_MoveCorrectObjectToBoxQuiz "MoveCorrectObjectToBoxQuiz"
#define k_MoveFruitToBagQuiz "MoveFruitToBagQuiz"
#define k_SortObjectInOrderQuiz "SortObjectInOrderQuiz"

#define k_ChooseCorrectObjectsQuiz "ChooseCorrectObjectsQuiz"
#define k_ChooseCorrectGroupQuiz "ChooseCorrectGroupQuiz"

#define k_CompareGroupSizeQuiz "CompareGroupSizeQuiz"
#define k_CompareNumberOfObjectsQuiz "CompareNumberOfObjectsQuiz"
#define k_CompareTwoObjectsQuiz "CompareTwoObjectsQuiz"
#define k_CompareWeightTwoObjectsQuiz "CompareWeightTwoObjectsQuiz"

#define k_PatternCopyQuiz "PatternCopyQuiz"
#define k_PatternComeNextQuiz "PatternComeNextQuiz"
#define k_PatternColorQuiz "PatternColorQuiz"

#define k_WhatObjectIsThisQuiz "WhatObjectIsThisQuiz"

#define k_TracingQuiz "TracingQuiz"
#define k_MoveObjectMakePictureQuiz "MoveObjectMakePictureQuiz"
#define k_ConnectDotsQuiz "ConnectDotsQuiz"
#define k_MiniSudokuQuiz "MiniSudokuQuiz"

#define k_ColorUseKeysQuiz "ColorUseKeysQuiz"
#define k_ColorPictureQuiz "ColorPictureQuiz"
#define k_MakeEnoughQuiz "MakeEnoughQuiz"
#define k_EscapeMazeQuiz "EscapeMazeQuiz"


/**----> QUIZZES ADDITION <----**/
#include "MissingInClockFaceQuiz.h"
#define k_MissingInClockFaceQuiz "MissingInClockFaceQuiz"

#include "TimeComeNextQuiz.h"
#define k_TimeComeNextQuiz "TimeComeNextQuiz"

#include "ChooseCorrectClockQuiz.h"
#define k_ChooseCorrectClockQuiz "ChooseCorrectClockQuiz"

#include "ChoosePerfectObjectsQuiz.h"
#define k_ChoosePerfectObjectsQuiz "ChoosePerfectObjectsQuiz"

#include "EverydayEventsQuiz.h"
#define k_EverydayEventsQuiz "EverydayEventsQuiz"

#include "QuickWordProblemsQuiz.h"
#define k_QuickWordProblemsQuiz "QuickWordProblemsQuiz"

#include "SumaryWordProblemsQuiz.h"
#define k_SumaryWordProblemsQuiz "SumaryWordProblemsQuiz"

#include "PictureDescribesQuiz.h"
#define k_PictureDescribesQuiz "PictureDescribesQuiz"

#include "CompleteSymbolsQuiz.h"
#define k_CompleteSymbolsQuiz "CompleteSymbolsQuiz"

#include "WhoStandRightPlaceQuiz.h"
#define k_WhoStandRightPlaceQuiz "WhoStandRightPlaceQuiz"

#include "ChooseObjectAtPositionQuiz.h"
#define k_ChooseObjectAtPositionQuiz "ChooseObjectAtPositionQuiz"

#include "MoveObjectToRightPlaceQuiz.h"
#define k_MoveObjectToRightPlaceQuiz "MoveObjectToRightPlaceQuiz"

#include "AnswerPositionOfObjectQuiz.h"
#define k_AnswerPositionOfObjectQuiz "AnswerPositionOfObjectQuiz"

#include "MatchingTimeQuiz.h"
#define k_MatchingTimeQuiz "MatchingTimeQuiz"


/**----> QUIZES CTV <----**/

/**----> HUY <----**/
//TODO: Huy Define
#include "Huy_AddNumberIntoClock.h"
#include "Huy_AnswerNumberLine.h"
#include "Huy_ReadingNumber.h"

#define k_Huy_AddNumberIntoClock "Huy_AddNumberIntoClock"
#define k_Huy_AnswerNumberLine "Huy_AnswerNumberLine"
#define k_Huy_ReadingNumber "Huy_ReadingNumber"


/**----> MY <----**/
//TODO: My Define
#include "My_AdditionAndAnswer.h"
#include "My_AdditionAndSubtractionTerm.h"
#include "My_AdditionInTable.h"
#include "My_CountingNumber.h"
#include "My_DragInOrder.h"
#include "My_GrowingPattern.h"
#include "My_MatchingNumber.h"
#include "My_MathWithExplain.h"
#include "My_MissingNumber.h"
#include "My_MultiAddition.h"
#include "My_NumberBone.h"
#include "My_NumberLine.h"
#include "My_NumberInSubtraction.h"
#include "My_OddAndEven.h"
#include "My_OneMoreOneLessWhitCount.h"
#include "My_SubtractZeroAndAll.h"
#include "My_TouchObject.h"
#include "My_VennDiagram.h"

#define k_My_AdditionAndAnswer "My_AdditionAndAnswer"
#define k_My_AdditionAndSubtractionTerm "My_AdditionAndSubtractionTerm"
#define k_My_AdditionInTable "My_AdditionInTable"
#define k_My_CountingNumber "My_CountingNumber"
#define k_My_DragInOrder "My_DragInOrder"
#define k_My_GrowingPattern "My_GrowingPattern"
#define k_My_MatchingNumber "My_MatchingNumber"
#define k_My_MathWithExplain "My_MathWithExplain"
#define k_My_MissingNumber "My_MissingNumber"
#define k_My_MultiAddition "My_MultiAddition"
#define k_My_NumberBone "My_NumberBone"
#define k_My_NumberLine "My_NumberLine"
#define k_My_NumberInSubtraction "My_NumberInSubtraction"
#define k_My_OddAndEven "My_OddAndEven"
#define k_My_OneMoreOneLessWhitCount "My_OneMoreOneLessWhitCount"
#define k_My_SubtractZeroAndAll "My_SubtractZeroAndAll"
#define k_My_TouchObject "My_TouchObject"
#define k_My_VennDiagram "My_VennDiagram"


/**----> THANH <----**/
//TODO: Thanh Define
#include "Thanh_ChartHundered.h"
#include "Thanh_ChartHundered_SelectNumber.h"
#include "Thanh_DoubleAddition.h"
#include "Thanh_LocationInGrid.hpp"
#include "Thanh_Matching.h"
#include "Thanh_Matching2C.h"
#include "Thanh_Matching3C.h"
#include "Thanh_MeasureDragNumber.h"
#include "Thanh_MeasureDragObject.h"
#include "Thanh_MeasurementWithMediator.h"
#include "Thanh_ReadClockAndWriteTime.h"
#include "Thanh_WhereDoesClockHandPointing.h"
#include "Thanh_WhichGraphShowNumber.h"
#include "Thanh_WhichPictureShowSentence.h"

#define k_Thanh_ChartHundered "Thanh_ChartHundered"
#define k_Thanh_ChartHundered_SelectNumber "Thanh_ChartHundered_SelectNumber"
#define k_Thanh_DoubleAddition "Thanh_DoubleAddition"
#define k_Thanh_LocationInGrid "Thanh_LocationInGrid"
#define k_Thanh_Matching "Thanh_Matching"
#define k_Thanh_Matching2C "Thanh_Matching2C"
#define k_Thanh_Matching3C "Thanh_Matching3C"
#define k_Thanh_MeasureDragNumber "Thanh_MeasureDragNumber"
#define k_Thanh_MeasureDragObject "Thanh_MeasureDragObject"
#define k_Thanh_MeasurementWithMediator "Thanh_MeasurementWithMediator"
#define k_Thanh_ReadClockAndWriteTime "Thanh_ReadClockAndWriteTime"
#define k_Thanh_WhereDoesClockHandPointing "Thanh_WhereDoesClockHandPointing"
#define k_Thanh_WhichGraphShowNumber "Thanh_WhichGraphShowNumber"
#define k_Thanh_WhichPictureShowSentence "Thanh_WhichPictureShowSentence"


/**----> NAM <----**/
//TODO: Nam Define
#include "Nam_AddObjectAndAnswerQuiz.h"
#include "Nam_AnswerAndTouchPuzzle.h"
#include "Nam_ChooseRelatedFact.h"
#include "Nam_ColorObject.h"
#include "Nam_Commutative.h"
#include "Nam_CompareObjectVsGroup.h"
#include "Nam_DragArrowToMeasure.h"
#include "Nam_DragObjectToMatrixProperties.h"
#include "Nam_DragObjectToMeasureHeight.h"
#include "Nam_DragObjectToTwoGroupAndCaculate.h"
#include "Nam_FactFamilies.h"
#include "Nam_PartitionShapeColor.h"
#include "Nam_PartitionShapeEqual.h"
#include "Nam_RomaNumber.h"
#include "Nam_SortAndDisplayResult.h"
#include "Nam_TouchToAddOrRemove.h"
#include "Nam_WayToMakeANumber.h"
#include "Nam_WhichCardEqual.h"
#include "Nam_WhichSentenceIsTrue.h"
#include "Nam_IspyWithSortInLineQuiz.h"
#include "Nam_MultiSelectNumberQuiz.h"

#define k_Nam_AddObjectAndAnswerQuiz "Nam_AddObjectAndAnswerQuiz"
#define k_Nam_AnswerAndTouchPuzzle "Nam_AnswerAndTouchPuzzle"
#define k_Nam_ChooseRelatedFact "Nam_ChooseRelatedFact"
#define k_Nam_ColorObject "Nam_ColorObject"
#define k_Nam_Commutative "Nam_Commutative"
#define k_Nam_CompareObjectVsGroup "Nam_CompareObjectVsGroup"
#define k_Nam_DragArrowToMeasure "Nam_DragArrowToMeasure"
#define k_Nam_DragObjectToMatrixProperties "Nam_DragObjectToMatrixProperties"
#define k_Nam_DragObjectToMeasureHeight "Nam_DragObjectToMeasureHeight"
#define k_Nam_DragObjectToTwoGroupAndCaculate "Nam_DragObjectToTwoGroupAndCaculate"
#define k_Nam_FactFamilies "Nam_FactFamilies"
#define k_Nam_PartitionShapeColor "Nam_PartitionShapeColor"
#define k_Nam_PartitionShapeEqual "Nam_PartitionShapeEqual"
#define k_Nam_RomaNumber "Nam_RomaNumber"
#define k_Nam_SortAndDisplayResult "Nam_SortAndDisplayResult"
#define k_Nam_TouchToAddOrRemove "Nam_TouchToAddOrRemove"
#define k_Nam_WayToMakeANumber "Nam_WayToMakeANumber"
#define k_Nam_WhichCardEqual "Nam_WhichCardEqual"
#define k_Nam_WhichSentenceIsTrue "Nam_WhichSentenceIsTrue"
#define k_Nam_MultiSelectNumberQuiz "MultiSelectNumberQuiz"
#define k_Nam_IspyWithSortInLineQuiz "Nam_IspyWithSortInLineQuiz"


/**----> BAO ANH <----**/
//TODO: Bao Anh Define
#include "BA_ChooseCorrectGraphQuiz.hpp"
#include "BA_ChooseTwoMakeOneQuiz.hpp"
#include "BA_ColoredBrickGraphQuiz.hpp"
#include "BA_CountBlockQuiz.hpp"
#include "BA_GroupInGroupQuiz.hpp"
#include "BA_HowManyKindQuiz.hpp"
#include "BA_InterpretDataQuiz.hpp"
#include "BA_MoreOrLessDragQuiz.hpp"
#include "BA_OrganizeDataTableQuiz.hpp"
#include "BA_PictoGraphQuiz.hpp"
#include "BA_ReadDataGraphQuiz.hpp"
#include "BA_ReGroupQuiz.hpp"
#include "BA_SmartFruitQuiz.hpp"
#include "BA_SumAndSubQuiz.hpp"
#include "BA_TableValueQuiz.hpp"
#include "BA_TapToAddQuiz.hpp"
#include "BA_TensAndOnesQuiz.hpp"
#include "BA_TouchObjectInPicture.hpp"

#define k_BA_TouchObjectInPicture "BA_TouchObjectInPictureQuiz"
#define k_BA_ChooseCorrectGraphQuiz "BA_ChooseCorrectGraphQuiz"
#define k_BA_ChooseTwoMakeOneQuiz "BA_ChooseTwoMakeOneQuiz"
#define k_BA_ColoredBrickGraphQuiz "BA_ColoredBrickGraphQuiz"
#define k_BA_CountBlockQuiz "BA_CountBlockQuiz"
#define k_BA_GroupInGroupQuiz "BA_GroupInGroupQuiz"
#define k_BA_HowManyKindQuiz "BA_HowManyKindQuiz"
#define k_BA_InterpretDataQuiz "BA_InterpretDataQuiz"
#define k_BA_MoreOrLessDragQuiz "BA_MoreOrLessDragQuiz"
#define k_BA_OrganizeDataTableQuiz "BA_OrganizeDataTableQuiz"
#define k_BA_PictoGraphQuiz "BA_PictoGraphQuiz"
#define k_BA_ReadDataGraphQuiz "BA_ReadDataGraphQuiz"
#define k_BA_ReGroupQuiz "BA_ReGroupQuiz"
#define k_BA_SmartFruitQuiz "BA_SmartFruitQuiz"
#define k_BA_SumAndSubQuiz "BA_SumAndSubQuiz"
#define k_BA_TableValueQuiz "BA_TableValueQuiz"
#define k_BA_TapToAddQuiz "BA_TapToAddQuiz"
#define k_BA_TensAndOnesQuiz "BA_TensAndOnesQuiz"


/**----> MINH <----**/
//TODO: Minh define
#include "Minh_AdditionThenComparison.h"
#include "Minh_Calendar.h"
#include "Minh_CompareClock.h"
#include "Minh_EstimateNearestNumber.h"
#include "Minh_EstimationQuantity.h"
#include "Minh_RememberActivities.h"
#include "Minh_SetTheCalculation.h"
#include "Minh_FillTimeShowAtClockQuiz.h"
#include "Minh_IdentifySideAndCornerQuiz.h"

#define k_Minh_AdditionThenComparison "Minh_AdditionThenComparison"
#define k_Minh_Calendar "Minh_Calendar"
#define k_Minh_CompareClock "Minh_CompareClock"
#define k_Minh_EstimateNearestNumber "Minh_EstimateNearestNumber"
#define k_Minh_EstimationQuantity "Minh_EstimationQuantity"
#define k_Minh_RememberActivities "Minh_RememberActivities"
#define k_Minh_SetTheCalculation "Minh_SetTheCalculation"
#define k_Minh_FillTimeShowAtClockQuiz "FillTimeShowAtClockQuiz"
#define k_Minh_IdentifySideAndCornerQuiz "Minh_IdentifySideAndCornerQuiz"

#include "TestingModeUtils.h"

namespace math {
    namespace quiz {
        static inline Math_Quiz* createQuiz (mj::model::Game _game) {
            std::string game_classify= _game.getClassName();
            if (testing::TestManager::getInstance()->getTestMode() == testing::activity) {
                game_classify= testing::TestManager::getInstance()->getClassName();
            }
                        
            /*--- Game count ---*/
            if (game_classify.compare(k_CountAndAnswerQuiz)==0) {
                auto quiz=CountAndAnswerQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_CountAndDragQuiz)==0) {
                auto quiz=CountAndDragQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_CountRowAndDragQuiz)==0) {
                auto quiz=CountRowAndDragQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            /*--- game drag ---*/
            if (game_classify.compare(k_SortedToGroupQuiz)==0) {
                auto quiz=SortedToGroupQuiz::createQuiz();
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
            
            if (game_classify.compare(k_DragBetweenTwoGroupsQuiz)==0) {
                auto quiz=DragBetweenTwoGroupsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MoveCorrectObjectToBoxQuiz)==0) {
                auto quiz=MoveCorrectObjectToBoxQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MoveFruitToBagQuiz)==0) {
                auto quiz=MoveFruitToBagQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_SortObjectInOrderQuiz)==0) {
                auto quiz=SortObjectInOrderQuiz::createQuiz();
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
            
            if (game_classify.compare(k_CompareGroupSizeQuiz)==0) {
                auto quiz=CompareGroupSizeQuiz::createQuiz();
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
            
            if (game_classify.compare(k_CompareNumberOfObjectsQuiz)==0) {
                auto quiz=CompareNumberOfObjectsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_CompareTwoObjectsQuiz)==0) {
                auto quiz=CompareTwoObjectsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_CompareWeightTwoObjectsQuiz)==0) {
                auto quiz=CompareWeightTwoObjectsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_PatternCopyQuiz)==0) {
                auto quiz=PatternCopyQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_PatternComeNextQuiz)==0) {
                auto quiz=PatternComeNextQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_PatternColorQuiz)==0) {
                auto quiz=PatternColorQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            
            if (game_classify.compare(k_WhatObjectIsThisQuiz)==0) {
                auto quiz=WhatObjectIsThisQuiz::createQuiz();
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

            if (game_classify.compare(k_MoveObjectMakePictureQuiz)==0) {
                auto quiz=MoveObjectMakePictureQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ConnectDotsQuiz)==0) {
                auto quiz=ConnectDotsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MiniSudokuQuiz)==0) {
                auto quiz=MiniSudokuQuiz::createQuiz();
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

            if (game_classify.compare(k_ColorPictureQuiz)==0) {
                auto quiz=ColorPictureQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MakeEnoughQuiz)==0) {
                auto quiz=MakeEnoughQuiz::createQuiz();
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

            if (game_classify.compare(k_MissingInClockFaceQuiz)==0) {
                auto quiz=MissingInClockFaceQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_TimeComeNextQuiz)==0) {
                auto quiz=TimeComeNextQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ChooseCorrectClockQuiz)==0) {
                auto quiz=ChooseCorrectClockQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_ChoosePerfectObjectsQuiz)==0) {
                auto quiz=ChoosePerfectObjectsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_EverydayEventsQuiz)==0) {
                auto quiz=EverydayEventsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_QuickWordProblemsQuiz)==0) {
                auto quiz=QuickWordProblemsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_SumaryWordProblemsQuiz)==0) {
                auto quiz=SumaryWordProblemsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_PictureDescribesQuiz)==0) {
                auto quiz=PictureDescribesQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            if (game_classify.compare(k_CompleteSymbolsQuiz)==0) {
                auto quiz=CompleteSymbolsQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_WhoStandRightPlaceQuiz)==0) {
                auto quiz=WhoStandRightPlaceQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_ChooseObjectAtPositionQuiz)==0) {
                auto quiz=ChooseObjectAtPositionQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MoveObjectToRightPlaceQuiz)==0) {
                auto quiz=MoveObjectToRightPlaceQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_AnswerPositionOfObjectQuiz)==0) {
                auto quiz=AnswerPositionOfObjectQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            if (game_classify.compare(k_MatchingTimeQuiz)==0) {
                auto quiz=MatchingTimeQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            
            //TODO: Quiz Huy
            if (game_classify.compare(k_Huy_AddNumberIntoClock)==0) {
                auto quiz=Huy_AddNumberIntoClock::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Huy_AnswerNumberLine)==0) {
                auto quiz=Huy_AnswerNumberLine::createQuiz();
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

            //TODO: Quiz My
            if (game_classify.compare(k_My_AdditionAndAnswer)==0) {
                auto quiz=My_AdditionAndAnswer::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_AdditionAndSubtractionTerm)==0) {
                auto quiz=My_AdditionAndSubtractionTerm::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_AdditionInTable)==0) {
                auto quiz=My_AdditionInTable::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_CountingNumber)==0) {
                auto quiz=My_CountingNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_DragInOrder)==0) {
                auto quiz=My_DragInOrder::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_GrowingPattern)==0) {
                auto quiz=My_GrowingPattern::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_MatchingNumber)==0) {
                auto quiz=My_MatchingNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_MathWithExplain)==0) {
                auto quiz=My_MathWithExplain::createQuiz();
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
            if (game_classify.compare(k_My_MultiAddition)==0) {
                auto quiz=My_MultiAddition::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_NumberBone)==0) {
                auto quiz=My_NumberBone::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_NumberInSubtraction)==0) {
                auto quiz=My_NumberInSubtraction::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_NumberLine)==0) {
                auto quiz=My_NumberLine::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_OddAndEven)==0) {
                auto quiz=My_OddAndEven::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_OneMoreOneLessWhitCount)==0) {
                auto quiz=My_OneMoreOneLessWhitCount::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_SubtractZeroAndAll)==0) {
                auto quiz=My_SubtractZeroAndAll::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_TouchObject)==0) {
                auto quiz=My_TouchObject::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_My_VennDiagram)==0) {
                auto quiz=My_VennDiagram::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            //TODO: quiz Thành
            if (game_classify.compare(k_Thanh_ChartHundered)==0) {
                auto quiz=Thanh_ChartHundered::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_ChartHundered_SelectNumber)==0) {
                auto quiz=Thanh_ChartHundered_SelectNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_MeasurementWithMediator)==0) {
                auto quiz=Thanh_MeasurementWithMediator::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_DoubleAddition)==0) {
                auto quiz=Thanh_DoubleAddition::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_LocationInGrid)==0) {
                auto quiz=Thanh_LocationInGrid::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_Matching)==0) {
                auto quiz=Thanh_Matching::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_Matching2C)==0) {
                auto quiz=Thanh_Matching2C::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_Matching3C)==0) {
                auto quiz=Thanh_Matching3C::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_MeasureDragNumber)==0) {
                auto quiz=Thanh_MeasureDragNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_MeasureDragObject)==0) {
                auto quiz=Thanh_MeasureDragObject::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_ReadClockAndWriteTime)==0) {
                auto quiz=Thanh_ReadClockAndWriteTime::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_WhereDoesClockHandPointing)==0) {
                auto quiz=Thanh_WhereDoesClockHandPointing::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_WhichGraphShowNumber)==0) {
                auto quiz=Thanh_WhichGraphShowNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Thanh_WhichPictureShowSentence)==0) {
                auto quiz=Thanh_WhichPictureShowSentence::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            
            
            //TODO: Quiz Nam
            if (game_classify.compare(k_Nam_AddObjectAndAnswerQuiz)==0) {
                auto quiz=Nam_AddObjectAndAnswerQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_AnswerAndTouchPuzzle)==0) {
                auto quiz=Nam_AnswerAndTouchPuzzle::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_ChooseRelatedFact)==0) {
                auto quiz=Nam_ChooseRelatedFact::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_ColorObject)==0) {
                auto quiz=Nam_ColorObject::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_Commutative)==0) {
                auto quiz=Nam_Commutative::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_CompareObjectVsGroup)==0) {
                auto quiz=Nam_CompareObjectVsGroup::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_DragArrowToMeasure)==0) {
                auto quiz=Nam_DragArrowToMeasure::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_DragObjectToMatrixProperties)==0) {
                auto quiz=Nam_DragObjectToMatrixProperties::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_DragObjectToMeasureHeight)==0) {
                auto quiz=Nam_DragObjectToMeasureHeight::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_DragObjectToTwoGroupAndCaculate)==0) {
                auto quiz=Nam_DragObjectToTwoGroupAndCaculate::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_FactFamilies)==0) {
                auto quiz=Nam_FactFamilies::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_PartitionShapeEqual)==0) {
                auto quiz=Nam_PartitionShapeEqual::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_PartitionShapeColor)==0) {
                auto quiz=Nam_PartitionShapeColor::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_RomaNumber)==0) {
                auto quiz=Nam_RomaNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_SortAndDisplayResult)==0) {
                auto quiz=Nam_SortAndDisplayResult::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_TouchToAddOrRemove)==0) {
                auto quiz=Nam_TouchToAddOrRemove::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_WayToMakeANumber)==0) {
                auto quiz=Nam_WayToMakeANumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_WhichCardEqual)==0) {
                auto quiz=Nam_WhichCardEqual::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_WhichSentenceIsTrue)==0) {
                auto quiz=Nam_WhichSentenceIsTrue::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_MultiSelectNumberQuiz)==0) {
                auto quiz=Nam_MultiSelectNumberQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Nam_IspyWithSortInLineQuiz)==0) {
                auto quiz=Nam_IspyWithSortInLineQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            
            //TODO: Quiz Bao Anh
            if (game_classify.compare(k_BA_ChooseCorrectGraphQuiz)==0) {
                auto quiz=BA_ChooseCorrectGraphQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_ChooseTwoMakeOneQuiz)==0) {
                auto quiz=BA_ChooseTwoMakeOneQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_ColoredBrickGraphQuiz)==0) {
                auto quiz=BA_ColoredBrickGraphQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_CountBlockQuiz)==0) {
                auto quiz=BA_CountBlockQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_GroupInGroupQuiz)==0) {
                auto quiz=BA_GroupInGroupQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_HowManyKindQuiz)==0) {
                auto quiz=BA_HowManyKindQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_InterpretDataQuiz)==0) {
                auto quiz=BA_InterpretDataQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_MoreOrLessDragQuiz)==0) {
                auto quiz=BA_MoreOrLessDragQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_OrganizeDataTableQuiz)==0) {
                auto quiz=BA_OrganizeDataTableQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_PictoGraphQuiz)==0) {
                auto quiz=BA_PictoGraphQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_ReadDataGraphQuiz)==0) {
                auto quiz=BA_ReadDataGraphQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_ReGroupQuiz)==0) {
                auto quiz=BA_ReGroupQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_SmartFruitQuiz)==0) {
                auto quiz=BA_SmartFruitQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_SumAndSubQuiz)==0) {
                auto quiz=BA_SumAndSubQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_TableValueQuiz)==0) {
                auto quiz=BA_TableValueQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_TapToAddQuiz)==0) {
                auto quiz=BA_TapToAddQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_TensAndOnesQuiz)==0) {
                auto quiz=BA_TensAndOnesQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_BA_TouchObjectInPicture)==0) {
                auto quiz=BA_TouchObjectInPicture::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            
            //TODO: Quiz Minh
            if (game_classify.compare(k_Minh_AdditionThenComparison)==0) {
                auto quiz=Minh_AdditionThenComparison::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_Calendar)==0) {
                auto quiz=Minh_Calendar::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_CompareClock)==0) {
                auto quiz=Minh_CompareClock::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_EstimateNearestNumber)==0) {
                auto quiz=Minh_EstimateNearestNumber::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_EstimationQuantity)==0) {
                auto quiz=Minh_EstimationQuantity::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_RememberActivities)==0) {
                auto quiz=Minh_RememberActivities::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_SetTheCalculation)==0) {
                auto quiz=Minh_SetTheCaculation::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_FillTimeShowAtClockQuiz)==0) {
                auto quiz=Minh_FillTimeShowAtClockQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }
            if (game_classify.compare(k_Minh_IdentifySideAndCornerQuiz)==0) {
                auto quiz=Minh_IdentifySideAndCornerQuiz::createQuiz();
                quiz->initQuiz(_game);
                quiz->retain();
                return quiz;
            }

            
            //TODO: Quiz Intro
            auto quiz_1=IntroQuiz::createQuiz();
            quiz_1->initQuiz(_game);
            quiz_1->retain();
            return quiz_1;
        }
    }
}

#endif /* PreKGames_h */
