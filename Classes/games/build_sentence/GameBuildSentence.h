//
//  MJGameBuildSentence.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/28/17.
//
//

#ifndef MJGameBuildSentence_hpp
#define MJGameBuildSentence_hpp

#include "GameBase.h"
#include "Owl.h"
#include "cocos2d.h"

class MJOwl;

class MJGameBuildSentence : public MJGameBase<MJGameBuildSentence, mj::BuildSentenceDataSet>, public MJOwlDelegate {
public:
    CREATE_FUNC(MJGameBuildSentence);
    static MJGameBuildSentence * createGame(mj::GameDataSet<mj::BuildSentenceDataSet> data);
    static void initReader();
    static mj::GameDataSet<mj::BuildSentenceDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::BuildSentenceDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::BuildSentenceDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);

private:
    void didLoadFromCSB(mj::GameDataSet<mj::BuildSentenceDataSet> data);
	void getTimeSynctextASync(mj::BuildSentenceDataSet* set);
    void adjustDifficulity();
    
    void onEnter() override;
    void onExit() override;

    void doAppear();
    void doDisappear();
    void readOutLoud(int index);
    void read(int index);
    void readSentence();
    void onOwlFliedToStand(MJOwl * sender) override;
    void onOwlRead(MJOwl * sender, int index, bool with_sound) override;
    void onOwlDroppedOnPosition(MJOwl * sender, cocos2d::Point position) override;
	void onPictureClicked(Ref * sender);
    void shuffle();
	~MJGameBuildSentence();
    std::function<void()> _callbackAfterDelay = nullptr;
    std::vector<MJOwl *> _owls;
//    mj::GameDataSet<mj::BuildSentenceDataSet> _data;
	mj::BuildSentenceDataSet *_current_sentence = nullptr;

    bool m_speakable;
    bool _separated_word_read;

    int _total_owl_on_stand;
    int _shuffle_word_index;

    bool loadQuestion();

    mj::BuildSentenceType _last_sentence_type;

    std::once_flag _call_once_read_instruction;
    
    GAFWrapper* _bling = nullptr;
    void showGuide(cocos2d::Point, cocos2d::Point);
    bool _is_guide_showed = false;
};

CREATE_READER(MJGameBuildSentence);

#endif /* MJGameBuildSentence_hpp */
