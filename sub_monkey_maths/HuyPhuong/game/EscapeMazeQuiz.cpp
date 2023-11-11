//
//  EscapeMazeQuiz.h
//  MJQuiz
//
//  Created by tunado on 8/28/17.
//
//

#include "EscapeMazeQuiz.h"

EscapeMazeQuiz* EscapeMazeQuiz::createQuiz(){
    EscapeMazeQuiz* quiz = new EscapeMazeQuiz();
    
    if (quiz->_initQuiz()){
        quiz->autorelease();
        return quiz;
    }
    CC_SAFE_DELETE(quiz);
    return NULL;
}

EscapeMazeQuiz::~EscapeMazeQuiz()
{
    if(!the_mazes.empty())
    {
        for (int i = 0; i <the_mazes.size() ; i++)
        {
            auto obj = the_mazes.at(i);
            if(obj)
            {
                CC_SAFE_DELETE(obj);
            }
        }
    }
}

bool EscapeMazeQuiz::init(){
    return cocos2d::ui::Layout::init();
}

void EscapeMazeQuiz::initQuiz(mj::model::Game game) {
    Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(str.c_str());
    
    rapidjson::Value& json_data = document.FindMember("data")->value;
    
    //TODO: Game config
    rapidjson::Value& json_game = json_data.FindMember("game")->value;
    
    //Pattern
    rapidjson::Value& json_pattern = json_game.FindMember("pattern")->value;
    std::string pattern_kind = getJsonString(json_pattern, "kind", "normal");
    int pattern_piece = randomRangeJson(json_pattern, "piece");
    int pattern_size = randomRangeJson(json_pattern, "size");
    
    //maze path size
    int path_size = randomRangeJson(json_game, "path_size");
    
    //maze type
    std::string maze_type = getJsonString(json_game, "maze_type", "pattern");
    
    //fix icons
    bool is_fix_icons = false;
    std::vector<int> icon_indexes;
    if (json_game.HasMember("fix_icons")) {
        rapidjson::Value& fix_icons = json_game.FindMember("fix_icons")->value;
        is_fix_icons = getJsonBool(fix_icons, "enable", false);
        if (is_fix_icons) {
            rapidjson::Value& fix_json = getJsonMember(fix_icons, "indexes");
            for (rapidjson::SizeType i=0; i<fix_json.Size(); i++) {
                icon_indexes.push_back(fix_json[i].GetInt());
            }
        }
    }
    
    //skip count
    bool is_skip_count = false;
    int skip_counting = 1;
    int start_count = 1;
    if (json_game.HasMember("skip_count")) {
        rapidjson::Value& json_skip_count = getJsonMember(json_game, "skip_count");
        is_skip_count = getJsonBool(json_skip_count, "enable", false);
        if (is_skip_count) {
            skip_counting = getJsonInt(json_skip_count, "value", 1);
            start_count = randomRangeJson(json_skip_count, "start");
        }
    }
    
    //Mazes
    rapidjson::Value& json_maze = json_game.FindMember("mazes")->value;
    MazeConfig maze_conf;
    maze_conf.column = randomRangeJson(json_maze, "column");
    maze_conf.row = randomRangeJson(json_maze, "row");
    maze_conf.start = 0;
    maze_conf.end = maze_conf.column-1;
    maze_conf.size = path_size;
    mazes_list.push_back(maze_conf);
    
    //Generate
    //mazes
    pattern_length = 0;
    for (MazeConfig config : mazes_list) {
        auto maze = new MazeCreator(config.column, config.row, config.start, config.end, config.size, !(is_skip_count || (pattern_kind=="normal")));
        the_mazes.push_back(maze);
        pattern_length += ((int)maze->result.size());
    }
    
    //pattern
    if (pattern_kind != "normal" || is_skip_count) {
        select_type = "by_icon";
        number_kind = pattern_piece;
        if (is_skip_count) {
            pattern_piece = pattern_length;
            number_kind = pattern_length;
            pattern_size = pattern_length;
            if (pattern_kind == "normal") {
                pattern_kind = "a-z";
            }
        }
        pattern = new PatternCreator(pattern_piece, pattern_size, "icon", false);
        if (pattern_kind == "a-z") {
            if (is_skip_count) {
                int fix_skip_count = 0;
                if (skip_counting == 2) {
                    fix_skip_count = cocos2d::random(0, 1);
                }
                for (int i=0; i<pattern_size; i++) {
                    auto imgs = getNumbers((start_count + i)*skip_counting + fix_skip_count, (start_count + i)*skip_counting + fix_skip_count);
                    img_list.push_back(imgs.front());
                }
                std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                    return a->getComparisionValue("number") < b->getComparisionValue("number");
                });
            } else {
                img_list = getImageList("icons");
                std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool {
                    auto string_a = a->getText();
                    auto string_b = b->getText();
                    string_a = math::func::upperString(string_a);
                    string_b = math::func::upperString(string_b);
                    return string_a.compare(string_b) < 0;
                });
            }
        } else {
            if (is_skip_count) {
                for (int i=0; i<pattern_size; i++) {
                    auto img = math::number::getNumber((start_count + i)*skip_counting);
                    img_list.push_back(img);
                }
                std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                    return a->getComparisionValue("number") > b->getComparisionValue("number");
                });
            } else {
                img_list = getImageList("icons");
                std::sort(img_list.begin(), img_list.end(), [=](math::resource::Image* a, math::resource::Image* b)->bool{
                    auto string_a = a->getText();
                    auto string_b = b->getText();
                    string_a = math::func::upperString(string_a);
                    string_b = math::func::upperString(string_b);
                    return string_a.compare(string_b) > 0;
                });
            }
        }
        path_size = (int)img_list.size();
    } else {
        pattern = new PatternCreator(pattern_piece, pattern_size, "icon", false);
        
        number_kind = pattern_piece + 1;
        number_kind = number_kind < 3 ? 3 : number_kind;
        
        rapidjson::Value& json_icon = json_data.FindMember("icon")->value;
        std::string icon_same = getJsonString(json_icon, "same", "none");
        std::string icon_different = getJsonString(json_icon, "different", "id");
        select_type = getJsonString(json_icon, "select_type", "by_list");
        int number_fuzzy = randomRangeJson(json_icon, "number_fuzzy");
        
        if (maze_type == "same") {
            pattern_size = 1;
            select_type = "by_list";
            if (number_fuzzy==0) {
                number_fuzzy = cocos2d::random(1, pattern_size);
            }
        }
        if (maze_type == "pattern" && pattern_size == 1) {
            number_fuzzy = 2;
        }
        
        // check properties
        if (icon_different == "name") {
            icon_different = "name_1";
        }
        bool filter_prop = true;
        auto all_images = getImageList("icons");
        for (math::resource::Property prop : all_images.front()->comparisions) {
            if (prop.key == icon_different) {
                filter_prop = false;
                break;
            }
        }
        
        if (select_type == "by_list") {
            if (is_fix_icons) {
                for (int index : icon_indexes) {
                    img_list.push_back(all_images[index]);
                }
                while (img_list.size() > pattern_piece) {
                    img_list.pop_back();
                }
                if (img_list.size() < pattern_piece) {
                    if (filter_prop) {
                        if (icon_different == "id") {
                            for (math::resource::Image* img : all_images) {
                                bool valid = true;
                                for (math::resource::Image* img2 : img_list) {
                                    if (img2->code == img->code) {
                                        valid = false;
                                        break;
                                    }
                                }
                                if (valid && img_list.size() < pattern_piece) {
                                    img_list.push_back(img);
                                }
                                if (img_list.size() >= pattern_piece) {
                                    break;
                                }
                            }
                        } else {
                            for (math::resource::Image* img : all_images) {
                                bool valid = true;
                                for (math::resource::Image* img2 : img_list) {
                                    if (img2->getText(icon_different) == img->getText(icon_different)) {
                                        valid = false;
                                        break;
                                    }
                                }
                                if (valid && img_list.size() < pattern_piece) {
                                    img_list.push_back(img);
                                }
                                if (img_list.size() >= pattern_piece) {
                                    break;
                                }
                            }
                        }
                    } else {
                        for (math::resource::Image* img : all_images) {
                            bool valid = true;
                            for (math::resource::Image* img2 : img_list) {
                                if (img2->getComparisionValue(icon_different) == img->getComparisionValue(icon_different)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid && img_list.size() < pattern_piece) {
                                img_list.push_back(img);
                            }
                            if (img_list.size() >= pattern_piece) {
                                break;
                            }
                        }
                    }
                }
            } else {
                img_list = getImageList(all_images, pattern_piece, icon_different, icon_same);
            }
            
            for (math::resource::Image* img : img_list) {
                std::vector<math::resource::Image*> list;
                for (math::resource::Image* img_2 : all_images) {
                    if (filter_prop) {
                        if (icon_different == "id") {
                            if (img->code == img_2->code) {
                                list.push_back(img_2);
                            }
                        } else {
                            if (img->getText(icon_different) == img_2->getText(icon_different)) {
                                list.push_back(img_2);
                            }
                        }
                    } else {
                        if (img->getComparisionValue(icon_different) == img_2->getComparisionValue(icon_different)) {
                            list.push_back(img_2);
                        }
                    }
                }
                img_list_same.push_back(list);
            }
            
            math::func::shuffle(all_images);
            if (filter_prop) {
                if (icon_different == "id") {
                    for (math::resource::Image* img : all_images) {
                        bool valid = true;
                        for (math::resource::Image* img2 : img_list) {
                            if (img2->code == img->code) {
                                valid = false;
                                break;
                            }
                        }
                        if (valid && img_list_fuzzy.size() < number_fuzzy) {
                            img_list_fuzzy.push_back(img);
                        }
                        if (img_list_fuzzy.size() >= number_fuzzy) {
                            break;
                        }
                    }
                } else {
                    for (math::resource::Image* img : all_images) {
                        bool valid = true;
                        for (math::resource::Image* img2 : img_list) {
                            if (img->getText(icon_different) == img2->getText(icon_different)) {
                                valid = false;
                                break;
                            }
                        }
                        if (valid && img_list_fuzzy.size() < number_fuzzy) {
                            img_list_fuzzy.push_back(img);
                        }
                        if (img_list_fuzzy.size() >= number_fuzzy) {
                            break;
                        }
                    }
                }
            } else {
                for (math::resource::Image* img : all_images) {
                    bool valid = true;
                    for (math::resource::Image* img2 : img_list) {
                        if (img->getComparisionValue(icon_different) == img2->getComparisionValue(icon_different)) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid && img_list_fuzzy.size() < number_fuzzy) {
                        img_list_fuzzy.push_back(img);
                    }
                    if (img_list_fuzzy.size() >= number_fuzzy) {
                        break;
                    }
                }
            }
        } else {
            if (is_fix_icons) {
                for (int index : icon_indexes) {
                    img_list.push_back(all_images[index]);
                }
                while (img_list.size() > pattern_piece) {
                    img_list.pop_back();
                }
                if (img_list.size() < pattern_piece) {
                    std::vector<math::resource::Image*> img_same;
                    if (icon_same == "none") {
                        img_same = all_images;
                    } else if (icon_same == "pair") {
                        for (math::resource::Image* img : all_images) {
                            if (img->getComparisionValue(icon_same) == img_list.front()->getComparisionValue(icon_same)) {
                                img_same.push_back(img);
                            }
                        }
                    } else {
                        for (math::resource::Image* img : all_images) {
                            if (img->getText(icon_same) == img_list.front()->getText(icon_same)) {
                                img_same.push_back(img);
                            }
                        }
                    }
                    
                    if (filter_prop) {
                        if (icon_different == "id") {
                            for (math::resource::Image* img : img_same) {
                                bool valid = true;
                                for (math::resource::Image* img2 : img_list) {
                                    if (img2->code == img->code) {
                                        valid = false;
                                        break;
                                    }
                                }
                                if (valid && img_list.size() < pattern_piece) {
                                    img_list.push_back(img);
                                }
                                if (img_list.size() >= pattern_piece) {
                                    break;
                                }
                            }
                        } else {
                            for (math::resource::Image* img : img_same) {
                                bool valid = true;
                                for (math::resource::Image* img2 : img_list) {
                                    if (img2->getText(icon_different) == img->getText(icon_different)) {
                                        valid = false;
                                        break;
                                    }
                                }
                                if (valid && img_list.size() < pattern_piece) {
                                    img_list.push_back(img);
                                }
                                if (img_list.size() >= pattern_piece) {
                                    break;
                                }
                            }
                        }
                    } else {
                        for (math::resource::Image* img : img_same) {
                            bool valid = true;
                            for (math::resource::Image* img2 : img_list) {
                                if (img2->getComparisionValue(icon_different) == img->getComparisionValue(icon_different)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid && img_list.size() < pattern_piece) {
                                img_list.push_back(img);
                            }
                            if (img_list.size() >= pattern_piece) {
                                break;
                            }
                        }
                    }
                }
            } else {
                img_list = getImageList(all_images, pattern_piece, icon_different, icon_same);
            }
            
            math::func::shuffle(all_images);
            if (filter_prop) {
                if (icon_different == "id") {
                    for (math::resource::Image* img : all_images) {
                        bool valid = true;
                        for (math::resource::Image* img2 : img_list) {
                             if (img2->code == img->code) {
                                 valid = false;
                                 break;
                             }
                        }
                        if (valid && img_list_fuzzy.size() < number_fuzzy) {
                            img_list_fuzzy.push_back(img);
                        }
                        if (img_list_fuzzy.size() >= number_fuzzy) {
                            break;
                        }
                    }
                } else {
                    for (math::resource::Image* img : all_images) {
                        bool valid = true;
                        for (math::resource::Image* img2 : img_list) {
                            if (img2->getText(icon_different) == img->getText(icon_different)) {
                                valid = false;
                                break;
                            }
                        }
                        if (valid && img_list_fuzzy.size() < number_fuzzy) {
                            img_list_fuzzy.push_back(img);
                        }
                        if (img_list_fuzzy.size() >= number_fuzzy) {
                            break;
                        }
                    }
                }
            } else {
                for (math::resource::Image* img : all_images) {
                    bool valid = true;
                    for (math::resource::Image* img2 : img_list) {
                        if (img2->getComparisionValue(icon_different) == img->getComparisionValue(icon_different)) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid && img_list_fuzzy.size() < number_fuzzy) {
                        img_list_fuzzy.push_back(img);
                    }
                    if (img_list_fuzzy.size() >= number_fuzzy) {
                        break;
                    }
                }
            }
            
        }
    }
    pattern->setPatternListSize(pattern_length);
    
    int percent_choose = 100;
    if (!img_list_fuzzy.empty()) {
        percent_choose = 45;
    }
    
    //Sinh object
    setBackGroundQuiz("gameui/maze/bg.jpg");
    
    //Generate maze icon
    auto maze = the_mazes.front();
    int maze_size = maze->number_column * maze->number_row;
    
    for (int i1 = 0; i1 < maze_size; i1++) {
        math::resource::Image* img = img_list.front();
        bool is_maze_path = false;
        int index = 0;
        for (int i2=0; i2 < maze->result.size(); i2++) {
            int value = maze->result[i2];
            if (value == i1) {
                is_maze_path = true;
                index = i2;
                break;
            }
        }
        if (is_maze_path) {
            if (maze_type == "pattern") {
                if (select_type == "by_icon") {
                    img = img_list[pattern->pattern[index % pattern_size]];
                } else {
                    img = math::func::getRandomNode(img_list_same[pattern->pattern[index % pattern_size]]);
                }
            } else {
                img = math::func::getRandomNode(img_list_same[pattern->pattern[index % pattern_size]]);
                //img = math::func::getRandomNode(img_list);
            }
        } else {
            if (cocos2d::random(0, 99) < percent_choose) {
                auto check_fuzzy = maze->checkPoint(i1);
                if (check_fuzzy.empty()) {
                    img = math::func::getRandomNode(img_list);
                } else {
                    if (maze_type == "pattern") {
                        std::vector<int> index_pattern;
                        for (int value : check_fuzzy) {
                            int new_index = pattern->pattern_match[value];
                            bool is_valid = true;
                            for (int index : index_pattern) {
                                if (index == new_index) {
                                    is_valid = false;
                                    break;
                                }
                            }
                            if (is_valid) {
                                index_pattern.push_back(new_index);
                            }
                        }
                        
                        if (index_pattern.size() < img_list.size()) {
                            int index_img = 0;
                            auto random_indexes = randomArrayIndexes((int)img_list.size(), (int)img_list.size());
                            bool checked = true;
                            for (int t_index : random_indexes) {
                                index_img = t_index;
                                checked = false;
                                for (int index : index_pattern) {
                                    if (index == t_index) {
                                        checked = true;
                                        break;
                                    }
                                }
                                if (!checked) {
                                    break;
                                }
                            }
                            if (select_type == "by_icon") {
                                img = img_list[index_img];
                            } else {
                                img = math::func::getRandomNode(img_list_same[index_img]);
                            }
                        } else {
                            if (img_list_fuzzy.empty()) {
                                if (select_type == "by_icon") {
                                    img = math::func::getRandomNode(img_list);
                                } else {
                                    img = math::func::getRandomNode(math::func::getRandomNode(img_list_same));
                                }
                            } else {
                                img = math::func::getRandomNode(img_list_fuzzy);
                            }
                        }
                        
                    } else {
                        img = math::func::getRandomNode(img_list_fuzzy);
                    }
                }
            } else {
                //use fuzzy imgae
                img = math::func::getRandomNode(img_list_fuzzy);
            }
        }
        
        auto object = mpool->getObject();
        object->setEnable(i1, math::common::touch, img, nullptr);
        object->setObjectSize(cocos2d::Size(100, 100)*0.85);
        if (is_skip_count) {
            object->setImageVisible(false);
            object->setValueTextByKey("number");
            object->getLabelValue()->setFontSize(object_size * 0.5);
            object->showValueTextWithAnimation(false);
        }
        addChildNode(object, 3);
        _objects.push_back(object);
    }
    
    //TODO: Xử lý dữ liệu cấu hình câu hỏi
    parseQuestion();
    auto opt_keys = getQuestion()->getOptionalKeys();
    for (std::string key : opt_keys) {
        getQuestion()->setOptionalTexts(key, img_list);
    }
}

void EscapeMazeQuiz::onStart() {
    startGuide();
    quizBegan(0, false);
    
    int total_column = mazes_list.front().column;
    int total_row = mazes_list.front().row;
    
    int padding = 20;
    float quiz_height = quiz_size.height;
    int object_width = quiz_size.width/(total_column + 2.5);
    int object_height = (quiz_height-80)/total_row;
    object_size = object_width > object_height ? object_height : object_width;
    object_size = object_size > 125 ? 125:object_size;
    int start_x = (quiz_size.width - object_size*total_column)/2 + object_size/2;
    int start_y = quiz_height - (quiz_height - object_size*total_row)/2 - object_size/2;
    
    auto maze_bot_shadow = cocos2d::ui::ImageView::create(getUIPath("gameui/maze/bot_border.png"));
    maze_bot_shadow->setScale9Enabled(true);
    maze_bot_shadow->setCapInsets(cocos2d::Rect(1, 1, maze_bot_shadow->getVirtualRendererSize().width-2, maze_bot_shadow->getVirtualRendererSize().height-2));
    maze_bot_shadow->setContentSize(cocos2d::Size(object_size * (total_column + 1) + padding *1.5, padding));
    maze_bot_shadow->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    maze_bot_shadow->setPosition(cocos2d::Vec2(start_x - (object_size + padding)/2, start_y - object_size*(total_row - 0.5)-padding/2));
    this->addChildNode(maze_bot_shadow, 0);
    maze_bot_shadow->retain();
    img_views.push_back(maze_bot_shadow);
    
    auto maze_board = cocos2d::ui::ImageView::create(getUIPath("gameui/maze/box.png"));
    maze_board->setScale9Enabled(true);
    maze_board->setCapInsets(cocos2d::Rect(1, 1, maze_board->getVirtualRendererSize().width-2, maze_board->getVirtualRendererSize().height-2));
    maze_board->setContentSize(cocos2d::Size(object_size * total_column + padding, object_size * total_row + padding));
    maze_board->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    maze_board->setPosition(cocos2d::Vec2(quiz_size.width/2, quiz_height/2));
    this->addChildNode(maze_board, 0);
    maze_board->retain();
    img_views.push_back(maze_board);
    
    auto start_board = cocos2d::ui::ImageView::create(getUIPath("gameui/maze/box.png"));
    start_board->setScale9Enabled(true);
    start_board->setCapInsets(cocos2d::Rect(1, 1, maze_board->getVirtualRendererSize().width-2, maze_board->getVirtualRendererSize().height-2));
    start_board->setContentSize(cocos2d::Size(object_size + padding, object_size + padding));
    start_board->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    start_board->setPosition(cocos2d::Vec2(start_x - object_size - padding/2, start_y));
    this->addChildNode(start_board, 0);
    start_board->retain();
    img_views.push_back(start_board);
    
    auto start_icon = cocos2d::ui::ImageView::create(getUIPath("gameui/maze/start.png"));
    start_icon->setScale9Enabled(true);
    start_icon->setCapInsets(cocos2d::Rect(1, 1, start_icon->getVirtualRendererSize().width-2, start_icon->getVirtualRendererSize().height-2));
    start_icon->setContentSize(cocos2d::Size(object_size, object_size));
    start_icon->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    start_icon->setPosition(start_board->getPosition());
    this->addChildNode(start_icon, 0);
    start_icon->retain();
    img_views.push_back(start_icon);
    
    auto end_board = cocos2d::ui::ImageView::create(getUIPath("gameui/maze/box.png"));
    end_board->setScale9Enabled(true);
    end_board->setCapInsets(cocos2d::Rect(1, 1, maze_board->getVirtualRendererSize().width-2, maze_board->getVirtualRendererSize().height-2));
    end_board->setContentSize(cocos2d::Size(object_size+padding, object_size + padding));
    end_board->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    end_board->setPosition(cocos2d::Vec2(start_x + object_size * total_column + padding/2, start_y - object_size * (total_row -1)));
    this->addChildNode(end_board, 0);
    end_board->retain();
    img_views.push_back(end_board);
    
    auto end_icon = cocos2d::ui::ImageView::create(getUIPath("gameui/maze/finish.png"));
    end_icon->setScale9Enabled(true);
    end_icon->setCapInsets(cocos2d::Rect(1, 1, end_icon->getVirtualRendererSize().width-2, end_icon->getVirtualRendererSize().height-2));
    end_icon->setContentSize(cocos2d::Size(object_size, object_size));
    end_icon->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    end_icon->setPosition(end_board->getPosition());
    this->addChildNode(end_icon, 0);
    end_icon->retain();
    img_views.push_back(end_icon);
    
    auto maze = the_mazes.front();
    int maze_size = maze->number_column * maze->number_row;
    
    auto bg_path1 = getUIPath("gameui/maze/box_1.png");
    auto bg_path2 = getUIPath("gameui/maze/box_2.png");
    std::string bg_path = "";
    
    for (int i1 = 0; i1 < maze_size; i1++) {
        _objects[i1]->setObjectSize(cocos2d::Size(object_size, object_size)*0.85);
        _objects[i1]->setPosition(cocos2d::Vec2(start_x + object_size*(i1%maze->number_column), start_y - object_size*(i1/maze->number_column)));
        _objects[i1]->getLabelValue()->setFontSize(object_size * 0.5);
        _objects[i1]->onShow(0);
        
        bg_path = bg_path1;
        int index_col = (i1 % maze->number_column) % 2;
        int index_row = (i1 / maze->number_column) % 2;
        if (index_col - index_row != 0) {
            bg_path = bg_path2;
        }
        auto bgcell = cocos2d::ui::ImageView::create(bg_path);
        bgcell->setScale9Enabled(true);
        bgcell->setCapInsets(cocos2d::Rect(1, 1, bgcell->getVirtualRendererSize().width-2, bgcell->getVirtualRendererSize().height-2));
        bgcell->setContentSize(cocos2d::Size(object_size, object_size));
        bgcell->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        bgcell->setPosition(_objects[i1]->getPosition());
        this->addChildNode(bgcell, 2);
        bgcell->retain();
        img_views.push_back(bgcell);
    }
    
    index_path = 0;
    addEvents();
}

void EscapeMazeQuiz::startQuestion() {
    enableControlQuiz();
}

void EscapeMazeQuiz::onExplanation(bool correct) {
    if (!correct) {
        disableControlQuiz(0.25);
    } else {
        disableControlQuiz();
        this->nextQuestion(1.0);
    }
}

void EscapeMazeQuiz::onComplete() {
    for (Math_Object* object : _objects) {
        object->onHide(0, math::animation::No_Effect);
    }
    
    for (cocos2d::ui::ImageView* img : img_views) {
        img->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0), cocos2d::FadeOut::create(0.5), cocos2d::CallFunc::create([=](){
            img->removeFromParentAndCleanup(true);
        }), NULL));
    }
    this->getEventDispatcher()->removeEventListenersForTarget(this);
    quizEnded(0.5, true);
}

void EscapeMazeQuiz::onGuideStarted() {
    disableControlQuiz();
    std::vector<Math_Object*> not_actives;
    
    for (int i = index_path; i<pattern_length; i++) {
        if (i < the_mazes.front()->maze_size) {
            not_actives.push_back(_objects[the_mazes.front()->result[i]]);
        } else {
            not_actives.push_back(_objects[the_mazes.back()->result[i] - the_mazes.front()->maze_size]);
        }
    }
    
    if (not_actives.empty()) {
        GuideCenter->guideNone();
    } else if (not_actives.size() == 1) {
        GuideCenter->guideByTouch(not_actives.front()->getWorldPosition());
    } else {
        std::vector<cocos2d::Vec2> points;
        if (not_actives.size() < 3) {
            for (Math_Object* object : not_actives) {
                points.push_back(object->getWorldPosition());
            }
        } else {
            for (int i=0; i<3; i++) {
                points.push_back(not_actives[i]->getWorldPosition());
            }
        }
        GuideCenter->guideBySwipe(points);
    }
}

void EscapeMazeQuiz::onGuideDone() {
    enableControlQuiz();
}

void EscapeMazeQuiz::endingMazes (float dt) {
    if (index_ending < 0) {
        scheduleOnce([=](float){
            for (cocos2d::ui::ImageView* img : img_circles) {
                img->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.35), cocos2d::CallFunc::create([=](){
                    img->removeFromParentAndCleanup(true);
                }), NULL));
            }
            
            for (Math_Object* obj : _objects) {
                obj->setLocked(true);
                bool is_maze_path = false;
                for (Math_Object * object_path : maze_paths) {
                    if (obj->isSame(object_path)) {
                        is_maze_path = true;
                        break;
                    }
                }
                if (!is_maze_path) {
                    obj->getImage()->setColor(cocos2d::Color3B::BLACK);
                    obj->setOpacity(100);
                }
            }
            this->index_ending = 0;
            this->endingMazes(dt);
        }, 1, "start ending maze");
    } else {
        if (index_ending < maze_paths.size()) {
            scheduleOnce([=](float){
                maze_paths[index_ending]->onTouchObject(true);
                index_ending++;
                this->endingMazes(dt);
            }, dt, cocos2d::StringUtils::format("endingMazes_%d", index_ending));
        } else {
            this->onExplanation(true);
        }
    }
}

Math_Object* EscapeMazeQuiz::getTarget() {
    if (index_path == pattern_length) {
        return nullptr;
    }
    Math_Object* target = nullptr;
    if (index_path < the_mazes.front()->maze_size) {
        target = _objects[the_mazes.front()->result[index_path]];
    } else {
        target = _objects[the_mazes.back()->result[index_path] - the_mazes.front()->maze_size];
    }
    return target;
}

void EscapeMazeQuiz::activeTarget(Math_Object *object) {
    if (!object || object->isLocked()) {
        return;
    }
    object->setLocked(true);
    
    last_object = object;
    if (index_path > 0) {
        img_circles.back()->stopAllActions();
        img_circles.back()->setScale(1, 1);
    }
    maze_paths.push_back(object);
    auto icon_correct = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sgameui/maze/selected.png", m_theme->getThemePath().c_str()));
    icon_correct->setScale9Enabled(true);
    icon_correct->setCapInsets(cocos2d::Rect(1, 1, icon_correct->getVirtualRendererSize().width-2, icon_correct->getVirtualRendererSize().height-2));
    icon_correct->setContentSize(cocos2d::Size(object_size, object_size)* 1.3);
    icon_correct->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    icon_correct->setPosition(object->getPosition());
    this->addChildNode(icon_correct, 4);
    
    icon_correct->runAction(RepeatForever::create(cocos2d::Sequence::createWithTwoActions(cocos2d::ScaleTo::create(0.5, 0, 1), cocos2d::ScaleTo::create(0.5, 1, 1))));
    img_circles.push_back(icon_correct);
    
    object->onTouchObject(true);
    index_path++;
    GuideCenter->resetIncorrectCount();
    CCLOG("EscapeMazeQuiz:: index path: %d", index_path);
}

void EscapeMazeQuiz::addEvents()
{
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(EscapeMazeQuiz::onTouchBegan, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(EscapeMazeQuiz::onTouchMoved, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(EscapeMazeQuiz::onTouchEnded, this);
    touch_listener->onTouchCancelled = CC_CALLBACK_2(EscapeMazeQuiz::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
}

Math_Object* EscapeMazeQuiz::touchOnObject(cocos2d::Vec2 point) {
    for (Math_Object* object : _objects) {
        if (!object->isLocked() && object->isTouchedAt(point)) {
            return object;
        }
    }
    return nullptr;
}


bool EscapeMazeQuiz::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event){
    moveable = true;
    auto t_object = touchOnObject(touch->getLocation());
    if (t_object) {
        if (t_object != getTarget()) {
            t_object->onTouchObject(false);
            GuideCenter->increamentIncorrect();
            moveable = false;
        } else {
            activeTarget(t_object);
            if (index_path == pattern_length) {
                moveable = false;
                index_ending = -1;
                this->getEventDispatcher()->removeEventListenersForTarget(this);
                endingMazes(0.2);
            }
        }
    }
    return moveable;
}

void EscapeMazeQuiz::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) {
    if (moveable) {
        auto t_object = touchOnObject(touch->getLocation());
        if (t_object) {
            if (t_object != getTarget()) {
                t_object->onTouchObject(false);
                GuideCenter->increamentIncorrect();
                moveable = false;
            } else {
                activeTarget(t_object);
                //check move done
                if (index_path == pattern_length) {
                    moveable = false;
                }
            }
        }
    }
}

void EscapeMazeQuiz::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (index_path == pattern_length) {
        index_ending = -1;
        this->getEventDispatcher()->removeEventListenersForTarget(this);
        endingMazes(0.2);
    }
}

void EscapeMazeQuiz::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (index_path == pattern_length) {
        index_ending = -1;
        this->getEventDispatcher()->removeEventListenersForTarget(this);
        endingMazes(0.2);
    }
}


#define auto_testing_delay 2
void EscapeMazeQuiz::autoTesting() {
    this->scheduleOnce([=](float dt) {
        this->autoTestingRun(1.0);
    }, auto_testing_delay, cocos2d::StringUtils::format("autoTesting-%d",testing_step++));
}

void EscapeMazeQuiz::autoTestingRun(float delaytime) {
    
};

