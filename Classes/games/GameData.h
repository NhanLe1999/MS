//
//  GameMatchingData.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//

#ifndef GameMatchingData_hpp
#define GameMatchingData_hpp

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "Math_Libs.h"

using namespace cocos2d;

namespace mj {

    enum GameDifficulity {
        EASY = 1,
        MEDIUM = 3,
        HARD = 5
    };

    enum GameType {
        BUILD_SENTENCE = 14,
        PLUG_IN_BULBS_TT = 8,
        PLUG_IN_BULBS_TI = 15,
        LAND_THE_PLANE = 2,
        LIGHT_THEM_UP = 17,
        PICTURE_MATCH = 10,
        SING_BIRDIE = 13,
        WORD_MACHINE = 11,
        CHICKENS_TO_COOP = 6,
        MAKE_A_MATCH_TT = 9,
        MAKE_A_MATCH_TI = 16,
        JUMPING_FROG = 5,
        Q_AND_A = 12,
        BUILD_WALL = 18,
    };

    const std::unordered_map<int, std::string> TITLE_GAME_DEFAULT = {
        {mj::GameType::BUILD_SENTENCE, "Build Sentence"},
        {mj::GameType::PLUG_IN_BULBS_TI, "Plug in Bulbs"},
        {mj::GameType::PLUG_IN_BULBS_TT, "Plug in Bulbs"},
        {mj::GameType::LAND_THE_PLANE, "Land the Plane"},
        {mj::GameType::WORD_MACHINE, "Word Machine"},
        {mj::GameType::JUMPING_FROG, "Jumping Frog"},
        {mj::GameType::LIGHT_THEM_UP, "Light 'em Up"},
        {mj::GameType::MAKE_A_MATCH_TI, "Make a Match"},
        {mj::GameType::MAKE_A_MATCH_TT, "Make a Match"},
        {mj::GameType::PICTURE_MATCH, "Picture Match"},
        {mj::GameType::SING_BIRDIE, "Sing, Birdie!"},
        {mj::GameType::CHICKENS_TO_COOP, "Chickens to Coop"},
        {mj::GameType::Q_AND_A, "Q & A"},
    };

    const std::unordered_map<int, std::string> INSTRUCTION_AUDIO_PATH_DEFAULT = {
        {mj::GameType::BUILD_SENTENCE, "sounds/instructions/build_sentence.mp3"},
        {mj::GameType::PLUG_IN_BULBS_TI, "sounds/instructions/plug_in_bulbs.mp3"},
        {mj::GameType::PLUG_IN_BULBS_TT, "sounds/instructions/plug_in_bulbs.mp3"},
        {mj::GameType::LAND_THE_PLANE, "sounds/instructions/land_the_plane.mp3"},
        {mj::GameType::WORD_MACHINE, "sounds/instructions/word_machine.mp3"},
        {mj::GameType::JUMPING_FROG, "sounds/instructions/jumping_frog.mp3"},
        {mj::GameType::LIGHT_THEM_UP, "sounds/instructions/light_them_up.mp3"},
        {mj::GameType::MAKE_A_MATCH_TI, "sounds/instructions/make_a_match"},
        {mj::GameType::MAKE_A_MATCH_TT, "sounds/instructions/make_a_match"},
        {mj::GameType::PICTURE_MATCH, "sounds/instructions/picture_match.mp3"},
        {mj::GameType::SING_BIRDIE, "sounds/instructions/sing_birdie.mp3"},
        {mj::GameType::CHICKENS_TO_COOP, "sounds/instructions/chickens_to_coop.mp3"},
        {mj::GameType::Q_AND_A, "sounds/instructions/qa.mp3"},
    };

    const std::unordered_map<int, std::string> TITLE_AUDIO_PATH_DEFAULT = {
        {mj::GameType::BUILD_SENTENCE, "sounds/titles/build_sentence.mp3"},
        {mj::GameType::PLUG_IN_BULBS_TI, "sounds/titles/plug_in_bulbs.mp3"},
        {mj::GameType::PLUG_IN_BULBS_TT, "sounds/titles/plug_in_bulbs.mp3"},
        {mj::GameType::LAND_THE_PLANE, "sounds/titles/land_the_plane.mp3"},
        {mj::GameType::WORD_MACHINE, "sounds/titles/word_machine.mp3"},
        {mj::GameType::JUMPING_FROG, "sounds/titles/jumping_frog.mp3"},
        {mj::GameType::LIGHT_THEM_UP, "sounds/titles/light_them_up.mp3"},
        {mj::GameType::MAKE_A_MATCH_TI, "sounds/titles/make_a_match.mp3"},
        {mj::GameType::MAKE_A_MATCH_TT, "sounds/titles/make_a_match.mp3"},
        {mj::GameType::PICTURE_MATCH, "sounds/titles/picture_match.mp3"},
        {mj::GameType::SING_BIRDIE, "sounds/titles/sing_birdie.mp3"},
        {mj::GameType::CHICKENS_TO_COOP, "sounds/titles/chickens_to_coop.mp3"},
        {mj::GameType::Q_AND_A, "sounds/titles/qa.mp3"},
    };

    struct TextTiming {
        std::string text;
        float time_start;
        float time_end;
    };

    struct GameObject {
        std::string text;
        std::string image_path;
        std::string audio_path;

        bool operator == (const GameObject & other) const {
            return (text == other.text) && (image_path == other.image_path) && (audio_path == other.audio_path);
        }
    };

    enum MatchingGameType {
        MATCHING_LIGHT_UP,
        MATCHING_FISH
    };

    struct MatchingGameDataSet  {
        MatchingGameType type;
        std::vector<GameObject> objects;
    };

    enum LazerGameType {
        LAZER_IMAGE_IMAGE,
        LAZER_IMAGE_TEXT,
        LAZER_TEXT_IMAGE,
        LAZER_TEXT_TEXT
    };

    enum TextImageType {
        IMAGE_AND_TEXT,
        TEXT_AND_TEXT
    };

    struct LazerGameDataSet {
        LazerGameType type;
        std::vector<GameObject> top;
        std::vector<GameObject> bottom;
        std::string audioIntroGame;
    };

    struct WordMachineDataSet {
        std::vector<GameObject> text;
        std::vector<math::resource::Sync_Text> listTextHight;
        GameObject sentence;
        std::string answer;
    };

    enum BuildSentenceType {
        BY_POSITION,
        BY_READ_ORDER
    };

    struct BuildSentenceDataSet {
        BuildSentenceType type;
        std::vector<GameObject> words;
        std::vector<GameObject> shuffle_words;
        GameObject sentence;
        GameObject sentence_slow;
        std::vector<TextTiming> sync_slow;
        std::vector<math::resource::Sync_Text> sync_text;
        
        BuildSentenceDataSet() {

        }

        BuildSentenceDataSet(const BuildSentenceDataSet& obj) {

            type = obj.type;

            for (auto w : obj.words) {

                words.push_back(w);
            }

            for (auto w : obj.shuffle_words) {

                shuffle_words.push_back(w);
            }

            sentence = obj.sentence;

            sentence_slow = obj.sentence_slow;

            for (auto w : obj.sync_slow) {

                sync_slow.push_back(w);
            }

            for (auto w : obj.sync_text) {

                sync_text.push_back(w);
            }
        }

    };

    struct ChickensToCoopDataSet {
        std::vector<GameObject> words;
    };

    struct MakeAMatchDataSet {
        std::vector<GameObject> objects;
    };
    
    struct JumpingFrogDataSet {
        std::vector<GameObject> objects;
        std::string hoc_am_hoc_van;
    };
    
    struct LandThePlaneDataSet {
        std::vector<GameObject> objects;
        std::string hoc_am_hoc_van;
    };

    struct BirdieDataSet {
        std::vector<GameObject> options;
        std::vector<GameObject> pictures;
    };

    struct QA {
        GameObject question;
        std::vector<GameObject> answers;
        std::string answer;
    };

    struct QADataSet {
        std::vector<QA> questions;
    };

    struct GameInfo {
        int game_id;
        int story_id;
        int lang_id;
        int level;
        GameObject title;
        std::vector<GameObject> instructions;

        std::string story_title;
        std::string story_thumb;
        
        std::unordered_map<std::string, std::string> extras;
    };

    template <class T>
    class GameDataSet {
    public:
        std::queue<T> sets;
        GameInfo info;
    };
}

#define kEventBirdieOneSetDone "k_event_birdie_one_done"

#endif /* GameMatchingData_hpp */
