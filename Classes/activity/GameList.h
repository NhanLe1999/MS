
//  GameList.h
//  MJStory
//
//  Created by AnhB Nguyen on 10/15/18.
//

#ifndef GameList_h
#define GameList_h

#include "GameBuildWall.h"
#include "BuildWord_GamePlay.h"
#include "GameRabbitClassify.h"
#include "GameKhiXeUi.h"
#include "GameDiamondDig.h"
#include "GameWaterSpray.h"
#include "GameBuildWall.h"
#include "GameRestInNest.h"
#include "GameShookBook.h"
#include "GameUFO.h"
#include "GameXepChu.h"
#include "GameTyping.h"
#include "GameShark.h"
#include "GameLoadTheTrain.h"
#include "GamePlugInBulb2.h"
#include "GameJumpingMonkey.h"
#include "GameWordTrain.h"
#include "GamePhonics.h"
#include "HouseOfWord.h"
#include "Sequencing.h"
#include "MakeFireWorks.h"
#include "SEGameplay.h"
#include "WS_Playarea.h"
#include "GameShark2.h"
//old game
#include "GameQA.h"
#include "GameMakeAMatch.h"
#include "GameJumpingFrog.h"
#include "GameMatching.h"
#include "GameBuildSentence.h"
#include "GameLandThePlane.h"
#include "GameLazer.h"
#include "GameChickensToCoop.h"
#include "GameBirdie.h"
#include "GameWordMachine.h"

//phonic game
#include "../games/puzzle/puzzle.h"
#include "../games/ImmortalCircle/ImmortalCircle.h"
#include "../games/MultipleChoice/MultipleChoice.h"
#include "../games/CompareAndContrast/CompareAndContrast.h"
#include "../games/CoverStory/CoverStory.h"
//#include "QuestionWord.h"
#include "../games/SayWhy/SayWhy.h"
#include "../games/WhatHappenedNext/WhatHappenedNext.h"
#include "../vmonkey_games/FindDifferents/FindDifferents.h"
#include "../vmonkey_games/FindHiddenObject/FindHiddenObject.h"
#include "../vmonkey_games/CompletePicture/CompletePicture.h"
#include "../vmonkey_games/TellTheStory/TellTheStory.h"

#include "../vmonkey_games/NhinBongDoanVat/NhinBongDoanVat.h"
//#include "WhoAreThey.h"
//#include "WhatAreTheyThinking.h"
#include "../vmonkey_games/LuaChon/LuaChon.h"
#include "../vmonkey_games/cau_hoi_mo/CauHoiMo.h"
#include "../vmonkey_games/DanhGia/DanhGia.h"
#include "../vmonkey_games/ChoBoPhanTimTongThe/ChoBoPhanTimTongThe.h"
#include "../vmonkey_games/TimTuCungVanLoai/TimTuCungVanLoai.h"
#include "../vmonkey_games/NoiDiem/GameNoiDiem.h"
#include "../vmonkey_games/NoiDiem/GameNoiDiem.h"
#include "../vmonkey_games/SapXep/GameSapXep.h"
#include "../vmonkey_games/PhanLoai/GamePhanLoai.h"
#include "../vmonkey_games/FindTheWay/FindTheWay.h"
#include "../vmonkey_games/GhepTranh/GameGhepTranh.h"
#include "../vmonkey_games/GameOChu/GameOChu.h"
#include "../vmonkey_games/DienThe/DienThe.h"
#include "../vmonkey_games/GameNoi/GameNoi.h"
#include "../vmonkey_games/ToMau/ToMau.h"
#include "../vmonkey_games/jumping_monkey_vmonkey/GameJumpingMonkey_VMonkey.h"
//#include "../vmonkey_games/ToMauFree/ToMauFree.h"
#include "../vmonkey_games/VMRythmMatching/VMRythmMatching.h"

#include "phonic_games/GymWithTheMonkey/GymGame.h"
#include "phonic_games/ChasePenguinToIgloo/PenguinGame.h"
#include "phonic_games/Snail/Snail.h"
#include "phonic_games/DinosaurChasing/DinosaurChasingGame.h"
#include "phonic_games/WhackAMole/WhackAMoleGame.h"
#include "phonic_games/WizardCauldron/WizardCauldronGame.h"
#include "phonic_games/FrogSentence/FrogSentenceGame.h"
#include "phonic_games/CatchingThief/CatchingThiefGame.h"
#include "phonic_games/CarOnHighway/CarOnHighwayGame.h"
#include "phonic_games/SpaceStation/SpaceStationGame.h"
#include "phonic_games/FireFighter/FireFighter.h"
#include "phonic_games/Robot/Robot.h"
#include "phonic_games/FishingBoat/FishingShip.h"
#include "phonic_games/MonsterTeeth/MonsterTeeth.h"
#include "phonic_games/BalloonTheClown/BalloonTheClown.h"
#include "phonic_games/BalloonTheClown_VM/BalloonTheClown_VM.h"
#include "phonic_games/HatchingEggs/HatchingEggs.h"
#include "phonic_games/CaterpillaRacing/CaterpillaRacing.h"
#include "phonic_games/WordWheel/WordWheel.h"
#include "phonic_games/FireFly/Firefly.h"
#include "phonic_games/MPBubbleBoom/MPBubbleBoom.h" // NC
#include "phonic_games/CloudCatcher/CloudCatcher.h" // NC
#include "phonic_games/DeliveryFood//DeliveryFood.h" // NC
#include "phonic_games/FoodRestaurant/FoodRestaurant.h" // NC
#include "phonic_games/SheepVsWolves/SheepVsWolves.h"  // NC
#include "phonic_games/SpaceShip/SpaceShip.h" // NC
#include "phonic_games/DefeatTheDragon/DefeatTheDragon.h"
#include "phonic_games/WaterBalloon/WaterBalloon.h"
#include "phonic_games/FootballKick/FootballKick.h"
#include "phonic_games/GearsForCar/GearsForCar.h"
#include "phonic_games/BreakTheEgg/BreakTheEgg.h"
#include "phonic_games/piggy/GamePiggy.h"
#include "phonic_games/UFO/Ufo.h"
#include "phonic_games/ShootingStar/ShootingStar.h"
#include "phonic_games/GingerBreadMan/GingerBreadMan.h"
#include "phonic_games/WordTv/WordTv.h"
//#include "SkiingRacing/SkiingRacing.h"
#include "phonic_games/MexicanCactus/MexicanCactus.h" // NC
#include "phonic_games/MexicanCactus_VM/MexicanCactus_vm.h"
#include "phonic_games/BuggiesV2/BuggiesV2.h" // NC
#include "phonic_games/BuildBridge/GameBuildBridge.h" // NC
#include "phonic_games/Ninja/GameNinja.h"  // NC
#include "phonic_games/ClownOnBike/ClownOnBike.h" // NC
#include "phonic_games/Seapearl/Seapearl.h"  // NC
#include "phonic_games/FruitJumpRope/FruitJumpRope.h" // NC
#include "phonic_games/SweetLand/SweetLand.h" // MC
#include "phonic_games/HotAirBalloon/HotAirBalloon.h" // OK
#include "phonic_games/TracingLetter/TracingLetter.h" // OK
#include "phonic_games/WaterPipe/WaterPipeGame.h" // OK
#include "phonic_games/RythmMatching/RythmMatching.h"
#include "phonic_games/SkiingRacing/SkiingRacing.h"
#include "phonic_games/SheepVsWolves_VM/SheepVsWolves_VM.h"
#include "phonic_games/SpaceShip_VM/SpaceShip_VM.h"
#include "phonic_games/Seapearl_VM/Seapearl_VM.h"
#include "phonic_games/GingerBreadMan_VM/GingerBreadMan_VM.h"
#include "phonic_games/WizardCauldron_VM/WizardCauldronGame_VM.h"
#include "phonic_games/BuildBridge_VM/GameBuildBridge_VM.h"
const std::vector<int> GAME_LIST_ID {
    12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 37, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 70, 71, 73, 76, 139, 140, 180, 181, 184, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 203, 204, 205, 206, 208, 209, 210, 211, 212, 213, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 238, 241, 243, 244, 245, 246, 247, 255, 249, 250, 251, 252, 253, 254, 256, 257, 258, /*259, 261,*/ 263, 264, 265, 266, 267, 268, 269, 270, 272, 273, 274, 276, 278, 279, 281, 282, 283,284, 285,287, 290, 291, 329, 331, 372, 373, 374, 375, 376, 377, 378, 379
};
#endif /* GameList_h */
