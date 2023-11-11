//
//  CoverStoryActivity.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 9/6/18.
//

#include "ActivityLayout.h"

ActivityLayout* ActivityLayout::createGame(int game_id, std::vector<math::resource::Image*> images){

    auto act = ActivityLayout::create() ;
    if(act){
        act->addGame(game_id, images);
        return act;
    }
    CC_SAFE_DELETE(act);
    return nullptr;
}

void ActivityLayout::addGame(int game_id, std::vector<math::resource::Image *> images){
    auto path = FileUtils::getInstance()->getSearchPaths();
    if(game_id == 205){
//        auto game = CoverStory::createGame("config.json", images);
//         game->setDelegate(this);
//        this->addChild(game,100);
    }

    if(game_id == 206){
//        auto game = QuestionWord::createGame("config.json", images);
//         game->setDelegate(this);
//        this->addChild(game,100);
    }

}

ActivityLayout* ActivityLayout::createGame(int game_id, string game_path){
    auto act = ActivityLayout::create() ;
    if(act){
        act->addGame(game_id, game_path);
        return act;
    }
    CC_SAFE_DELETE(act);
    return nullptr;
}

void ActivityLayout::addGame(int game_id, string game_path){
    FileUtils::getInstance()->addSearchPath("game");
    auto path = FileUtils::getInstance()->getSearchPaths();
    auto config_path = game_path + "config.json";
	std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");
    MJMainGBase* game = nullptr;
    
    switch(game_id){
        case 180:
            game = GymGame::createGame(config_path, false);
            break;
        case 181:
            game = PenguinGame::createGame(config_path, false);
            break;
#if CC_TARGET_PLATFORM != CC_PLATFORM_MAC && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
        case 184:
            game = Snail::createGame(config_path);
            break;
#endif
        case 188:
            game = DinosaurChasingGame::createGame(config_path, false);
            break;
        case 189:
            game = WhackAMoleGame::createGame(config_path, false);
            break;
        case 190:
            game = WizardCauldronGame::createGame(config_path, false);
            break;
        case 191:
            game = FrogSentenceGame::createGame(config_path, false);
            break;
        case 192:
            game = CatchingThiefGame::createGame(config_path, false);
            break;
        case 193:
            game = CarOnHighwayGame::createGame(config_path, false);
            break;
        case 194:
            game = SpaceStationGame::createGame(config_path, false);
            break;
        case 195:
            game = FireFighter::createGame(config_path, false);
            break;
        case 196:
            game = Robot::createGame(config_path, false);
            break;
        case 197:
            game = FishingShip::createGame(config_path, false);
            break;
        case 198:
            game = MonsterTeeth::createGame(config_path, false);
            break;
        case 199:
            game = BalloonTheClown::createGame(config_path, false);
            break;
        case 200:
            game = HatchingEggs::createGame(config_path, false);
            break;
        case 201:
            game = CaterpillaRacing::createGame(config_path, false);
            break;
        case 203:
            game = WordWheel::createGame(config_path, false);
            break;
        case 204:
            game = Firefly::createGame(config_path, false);
            break;
        case 205:
            game = CoverStory::createGame(game_path);
            break;
//        case 206:
//            game = QuestionWord::createGame(game_path);
//            break;
        case 208:
            game = MPBubbleBoom::createGame(config_path, images);
            break;
        case 209:
            game = CloudCatcher::createGame(config_path, images);
            break;
        case 210:
            game = DeliveryFood::createGame(config_path, images);
            break;
        case 211:
            game = FoodRestaurant::createGame(config_path, images);
            break;
        case 212:
            game = SheepVsWolves::createGame(config_path, false);
            break;
        case 213:
            game = SpaceShip::createGame(config_path);
            break;
        case 215:
            game = DefeatTheDragon::createGame(config_path);
            break;
        case 216:
            game = WaterBalloon::createGame(config_path);
            break;
        case 217:
            game = FootballKick::createGame(config_path);
            break;
//        case 218:
//            game = WhoAreThey::createGame(game_path);
//            break;
//        case 219:
//            game = WhatAreTheyThinking::createGame(game_path);
//            break;
        case 220:
            game = GearsForCar::createGame(config_path);
            break;
        case 221:
            game = SayWhy::createGame(game_path);
            break;
        case 222:
            game = BreakTheEgg::createBreakTheEgg(config_path);
            break;
        case 223:
            game = WhatHappenedNext::createGame();
            break;
        case 224:
            game = GamePiggy::createGame(config_path, false);
            break;
        case 225:
            game = MultipleChoice::createGame();
            break;
        case 226:
            game = Ufo::createGame(config_path, false);
            break;
        case 238:
            game = ImmortalCircle::createGame();
            break;
        case 243:
            game = CompareAndContrast::createGame();
            break;
        case 244:
            game = Puzzle::createGame();
            break;
        case 246:
            game = ShootingStar::createGame(config_path);
            break;
        case 250:
            game = GingerBreadMan::createGame(config_path);
            break;
        case 251:
            game = WordTv::createGame(config_path);
            break;
        case 252:
            game = SkiingRacing::createGame(config_path, false);
            break;
        case 253:
            game = MexicanCactus::createGame(config_path);
            break;
        case 254:
            game = BuggiesV2::createGame(config_path);
            break;
        case 255:
            game = GameBuildBridge::createGame(config_path);
            break;
        case 256:
            game = GameNinja::createView(config_path);
            break;
        case 257:
            game = ClownOnBike::createGame(config_path);
            break;
        case 258:
            game = Seapearl::createGame(config_path);
            break;
        case 263:
            game = FruitJumpRope::createGame(config_path);
            break;
        case 264:
            game = SweetLand::createGame(config_path);
            break;
        case 265:
            game = HotAirBalloon::createGame(config_path);
            break;
		case 266:
			//game = RythmMatching::createGame(config_path);
            game = VMRythmMatching::createGame(config_path);
			break;
        case 267:
            game = TracingLetter::createGame(config_path);
            break;
        case 268:
            game = WaterPipe::create();
            break;
        case 269:
            game = CauHoiMo::createGame();
            break;
        case 270:
            game = GameNoiDiem::createGame();
            break;
        case 272:
            game = LuaChon::createGame();
            break;
        case 273:
            game = GameSapXep::createGame();
            break;
        case 274:
            game = DanhGia::createGame();
            break;
        case 276:
            game = ChoBoPhanTimTongThe::createGame();
            break;
        case 278:
            game = GamePhanLoai::createGame();
            break;
        case 279:
            game = TimTuCungVanLoai::createGame();
            break;
        case 280:
#if CC_TARGET_PLATFORM != CC_PLATFORM_MAC && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
            game = TellTheStory::createGame();
#endif
            break;
        case 281:
//            game = GameGhepTranh::createGame();
            break;
        case 282:
            game = FindTheWay::createGame();
            break;
        case 283:
            game = NhinBongDoanVat::createGame();
            break;
        case 284:
            game = GameOChu::createGame();
            break;
        case 285:
            game = DienThe::createGame();
            break;
        case 287:
            game = GameNoi::createGame();
            break;
        case 288:
            game = TimTuCungVanLoai::createGame();
            break;
        case 329:
            game = VMRythmMatching::createGame(config_path);
            break;
        case 372:
            game = MexicanCactus_VM::createGame(config_path);
            break;
        case 373:
            game = BalloonTheClown_VM::createGame(config_path, false);
            break;
        case 374:
            game = SheepVsWolves_VM::createGame(config_path, false);
            break;
        case 375:
            game = SpaceShip_VM::createGame(config_path, false);
            break;
        case 376:
            game = Seapearl_VM::createGame(config_path);
            break;
        case 377:
            game = GingerBreadMan_VM::createGame(config_path);
            break;
        case 378:
            game = WizardCauldronGame_VM::createGame(config_path, false);
            break;
        case 379:
            game = GameBuildBridge_VM::createGame(config_path);
            break;
        default:
            break;
    }

    if(game){
		game->setConfigImages(images);
        game->setDelegate(this);
        this->addChild(game,100,"game_phonic");
    }

    if(game_id == 271){
        generateData([=]{
            auto game = FindDifferents::createGame();
            game->setDelegate(this);
            this->addChild(game,100);
        });
    }

    if(game_id == 275){
        generateData([=](){
            auto game = FindHiddenObject::createGame();
            game->setDelegate(this);
            this->addChild(game,100);
        });
    }
    if(game_id == 277){
        generateData([=]{
            auto game = CompletePicture::createGame();
            game->setDelegate(this);
            this->addChild(game,100);
        });
    }

    if (game_id == 291) {
        generateData([=]{
            auto game = ToMau::createGame();
            game->setDelegate(this);
            this->addChild(game, 100);
        });
    }
}

void ActivityLayout::generateData(std::function<void()> callback ){

    //    // get data
    string json_file = "config.json";

    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    document.Accept(writer);
    CCLOG("JSON: \n%s", rj_buffer.GetString());
#endif
    if(JSON_SAFE_CHECK(document, "zip", Array )){
        auto img_list = document.FindMember("zip")->value.GetArray();
        std::vector<std::pair<std::string, std::string>> unzip_data;
        for(int i = 0; i < img_list.Size(); i++){
            auto zip_file_id = img_list[i].GetString();
            auto zip_file = math::resource::ResHelper::getImage(images, zip_file_id)->path;
            std::string zip_path = FileUtils::getInstance()->getWritablePath()+ "extracted/0/" + zip_file ;

            unzip_data.push_back(std::make_pair(zip_path, FileUtils::getInstance()->getWritablePath() + "extracted/" + StringUtils::toString(0) + "/"));

        }

        mj::helper::unzipAsync(unzip_data, [=](bool success) {
            if (success) {
                CCLOG("unzip success");
                if(callback){
                    callback();
                }
            } else {
                CCLOG("Cannot unzipped");
            }
        });
    }
}

void ActivityLayout::onEnter(){
    ms::BaseActivity::onEnter();
}

#include "StoryPageVC.h"
void ActivityLayout::getGameStatus(string sender){
	if (sender == "finnish_no_end_game") {
		this->resume();
		this->scheduleOnce([=](float dt) {
			_activitydelegate ? _activitydelegate->onActivityEnd(this) : (void) nullptr;
		}, 0.5f, "finish_game");
	}
    else if (sender == "finish_game_Vmonkey") {
        auto parent = dynamic_cast<StoryPageVC *>(this->getParent()->getParent());
        commonEndGame(EGT_COMMON);
    }
    else if(sender == "finish"){
        commonEndGame(EGT_COMMON);
    }

}

void ActivityLayout::onCloseGame(){
    CCLOG("close act");
    AUDIO_ENGINE->stopAllAudio();
    auto parent = dynamic_cast<StoryPageVC *>(this->getParent()->getParent());
    if (parent) {
        parent->showGameOver(true);
    }else{
        commonEndGame(EGT_COMMON,"",false,true);
    }
}
//void ActivityLayout::commonEndGame(){
////    ms::BaseActivity::commonEndGame(EGT_CHAR);
//
//}
