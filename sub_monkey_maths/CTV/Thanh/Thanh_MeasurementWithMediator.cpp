//
//  Thanh_MeasurementWithMediator.cpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#include "Thanh_MeasurementWithMediator.h"
#include "Math_Libs.h"

Thanh_MeasurementWithMediator* Thanh_MeasurementWithMediator::createQuiz()
{
	Thanh_MeasurementWithMediator* quiz = new Thanh_MeasurementWithMediator();
	if( quiz->_initQuiz() ) {
		quiz->autorelease();
		return quiz;
	}

	CC_SAFE_DELETE( quiz );
	return NULL;
}

bool Thanh_MeasurementWithMediator::init()
{
	return cocos2d::ui::Layout::init();
}


#pragma mark - Quiz Life-cycle
void Thanh_MeasurementWithMediator::initQuiz( mj::model::Game game )
{
	CCLOG( "%s override", __func__ );

	Math_Quiz::initQuiz( game );

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile( cocos2d::StringUtils::format( "%sconfig.json", _game_path.c_str() ) );
	rapidjson::Document document;
	document.Parse<0>( str.c_str() );

	rapidjson::Value& json_data = document.FindMember( "data" )->value;

	// TODO: GAME CONFIG
	rapidjson::Value& json_game = json_data.FindMember( "game" )->value;
	str = json_game.FindMember( "option_game" )->value.GetString();
	if( str == "measure_leng" ) option_game = OptionGame::measure_leng;
	else if( str == "measure_height" ) option_game = OptionGame::measure_height;

	str = json_game.FindMember( "mode" )->value.GetString();
	if( str == "measurememt_use_box_touch_object" ) _mode = GameMode::measurememt_use_box_touch_object;
	else if( str == "measurememt_use_box_touch_answer" ) _mode = GameMode::measurememt_use_box_touch_answer;
	else if( str == "measurement_use_ruler" ) _mode = GameMode::measurement_use_ruler;

	number_object = json_game.FindMember( "number_object" )->value.GetInt();
    object_bg = objectBG();

	//TODO:
	//lay tat ca icon trong media_set 
	auto list_icons = math::resource::ResHelper::getImages( cocos2d::StringUtils::format( "%sicons.json", _game_path.c_str() ), "", image_prefix );
	std::vector<math::resource::Image*> list_icon_object, list_icon_ruler;
	for( math::resource::Image* img : list_icons ) {
		if( img->getComparisionValue( "role" ) == 0 ) {
			list_icon_object.push_back( img );
		}
		else list_icon_ruler.push_back( img );
	}
	if( list_icon_ruler.size() > 0 )
		icon_ruler = list_icon_ruler[cocos2d::random( 0, ( int )list_icon_ruler.size() - 1 )];
	icon_ruler = math::resource::ResHelper::createImage( getUIPath( "common/frame/purple.png" ) );

	if( _mode == GameMode::measurememt_use_box_touch_object || _mode == GameMode::measurement_use_ruler ) {
		for( int i = 0; i < number_object; i++ ) {
			bool ok = 0;
			while( !ok ) {
				ok = true;
				math::resource::Image *image = list_icon_object[cocos2d::random( 0, ( int )list_icon_object.size() - 1 )];
				int number = image->getComparisionValue( "number" );
				for( math::resource::Image *img : icons_object ) {
					if( number == img->getComparisionValue( "number" ) ) ok = false;
				}
				if( ok ) {
					icons_object.push_back( image );
				}
			}
		}
	}
	else if( _mode == GameMode::measurememt_use_box_touch_answer ) {
		icons_object.push_back( list_icon_object[cocos2d::random( 0, ( int )list_icon_object.size() - 1 )] );
		list_value_answer.clear();
		list_value_answer.push_back( icons_object[0]->getComparisionValue( "number" ) );
		while( list_value_answer.size() < 4 ) {
			int random = cocos2d::random( 1, 10 );
			bool ok = true;
			for( int i = 0; i < list_value_answer.size(); i++ ) {
				if( list_value_answer[i] == random ) {
					ok = false;
				}
			}
			if( ok ) {
				list_value_answer.push_back( random );
			}
		}
	}

	//TODO: Xử lý dữ liệu cấu hình câu hỏi
	if( _mode == GameMode::measurememt_use_box_touch_object ) {
		parseQuestion();
	}
	else if( _mode == GameMode::measurememt_use_box_touch_answer ) {
		parseQuestion();
	}
	else if( _mode == GameMode::measurement_use_ruler ) {
		parseQuestion( true, 2 );
		number_question = 2;
		index_question = 0;
		icons_question.push_back( icons_object[0] );
		icons_question.push_back( icons_object[1] );
	}
	// optional
	auto opt_keys = getQuestion()->getOptionalKeys();
	for( std::string key:opt_keys ) {
		if( key == "number" ) {
			std::vector<math::resource::Text> texts;
			for( math::resource::Image* img : icons_question ) {
				auto img_txt = img->getTextValue( "number", m_lang->getLanguage() );
				img_txt.text = cocos2d::StringUtils::toString( img->getComparisionValue( key ) );
				texts.push_back( img_txt );
			}
			getQuestion()->setOptionalTexts( key, texts );
		}
		else if( key == "number_text" ) {
			std::vector<math::resource::Text> texts;
			for( math::resource::Image* img : icons_question ) {
				auto img_txt = img->getTextValue( "number_text", m_lang->getLanguage() );
				texts.push_back( img_txt );
			}
			getQuestion()->setOptionalTexts( key, texts );
		}
		else {
			getQuestion()->setOptionalTexts( key, icons_question );
		}
	}

	/*
	if (random_sorted) {
	random_shuffle(list_questiondata.begin(), list_questiondata.end());
	}
	*/
	std::random_device rd;
	std::mt19937 g( rd() );
	int obj_id = 0;

	if( option_game == OptionGame::measure_height ) {
		// TODO: HEIGHT OPTION
		// calculator unit_size
		unit_size = ( cocos2d::ui::ImageView::create( icon_ruler->path ) )->getContentSize();
		if( unit_size.height * 10 > 500 ) {
			unit_size = unit_size*( 500 / ( unit_size.height * 10 ) );
		}
		// gen objects cần đo chiều cao
		for( math::resource::Image* img : icons_object ) {
			Math_Object *object = Math_Pool::getInstance()->getObject();
			object->setEnable( obj_id++, math::common::TouchPermission::deny, img, img );
			//object->setTouchCallback(CC_CALLBACK_1(Thanh_MeasurementWithMediator::onTouchedObject, this)); 
			object->setMoveCallback( CC_CALLBACK_1( Thanh_MeasurementWithMediator::onTouchedObject, this ), CC_CALLBACK_2( Thanh_MeasurementWithMediator::onMoveObjectEnded, this ) );
			object->setSelectedAnimation( math::common::SelectedAnimation::fade );
			object->setMoveDirection( 1 );
			object->setBackgroundVisible( true );
			object->setImageVisible( false );
			object->setObjectSize( ( cocos2d::ui::ImageView::create( img->path ) )->getContentSize() );
			scale = ( unit_size.height*img->getComparisionValue( "number" ) ) / ( object->getContentSize().height );
			object->setObjectSize( object->getContentSize()*scale );
			object->getLabelValue()->disableEffect();
			objects.push_back( object );
			_objects.push_back( object );
		}
	}
	else {
		// TODO: LENG OPTION
		// calculator unit_size
		unit_size = ( cocos2d::ui::ImageView::create( icon_ruler->path ) )->getContentSize();
		if( unit_size.width * 10 > 600 ) {
			unit_size = unit_size*( 600 / ( unit_size.width * 10 ) );
		}

		// gen objects cần đo chiều dài
		for( math::resource::Image* img : icons_object ) {
			Math_Object *object = Math_Pool::getInstance()->getObject();
			object->setEnable( obj_id++, math::common::TouchPermission::deny, img, img );
			//object->setTouchCallback(CC_CALLBACK_1(Thanh_MeasurementWithMediator::onTouchedObject, this));
			object->setMoveCallback( CC_CALLBACK_1( Thanh_MeasurementWithMediator::onTouchedObject, this ), CC_CALLBACK_2( Thanh_MeasurementWithMediator::onMoveObjectEnded, this ) );
			object->setSelectedAnimation( math::common::SelectedAnimation::fade );
			object->setMoveDirection( 2 );
			object->setBackgroundVisible( true );
			object->setImageVisible( false );
			object->setObjectSize( ( cocos2d::ui::ImageView::create( img->path ) )->getContentSize() );
			if( option_game == OptionGame::measure_height ) { 
				scale = ( unit_size.height*img->getComparisionValue( "number" ) ) / ( object->getContentSize().height );
			}else {
				scale = ( unit_size.width*img->getComparisionValue( "number" ) ) / ( object->getContentSize().width );
			}
			object->setObjectSize( object->getContentSize()*scale );
			object->getLabelValue()->disableEffect();
			objects.push_back( object );
			_objects.push_back( object );
		}
	}
}

void Thanh_MeasurementWithMediator::onStart()
{
	CCLOG( "%s override", __func__ );

	disable_touch = false;
	std::random_device rd;
	std::mt19937 g( rd() );

	if( _mode == GameMode::measurememt_use_box_touch_object ) {
		correct_answer = objects[0];
		int ruler_size = objects[0]->getComparisionValue().number;
		if( option_game == OptionGame::measure_height ) {
			int max_height = 0;
			for( math::resource::Image* img : icons_object ) {
				if( max_height < img->getComparisionValue( "number" ) ) max_height = img->getComparisionValue( "number" );
			}
			for( int i = -max_height / 2; i < ruler_size - max_height / 2; i++ ) {
				auto image_ruler = cocos2d::ui::ImageView::create( icon_ruler->path );
				image_ruler->setScale9Enabled( true );
				image_ruler->setContentSize( unit_size );
				addChildNode( image_ruler, 2 );
				image_ruler->setPosition( cocos2d::Vec2( _panel->getContentSize().width / ( objects.size() + 2), _panel->getContentSize().height / 2 + unit_size.height*i ) );
				img_ruler.push_back( image_ruler );
			}

			// show object
			int index = 1;
			for( Math_Object *object : objects ) {
				index++;
				object->setPosition( cocos2d::Vec2( _panel->getContentSize().width*index / ( objects.size() + 2 ),
													_panel->getContentSize().height / 2 - unit_size.height*max_height / 2 + object->getContentSize().height / 2));
			}

			for( Math_Object *object : objects ) {
				addChildNode( object, 2 );
				object->onShow( 0 );
			}
		}
		else {
			int max_leng = 0;
			for( math::resource::Image* img : icons_object ) {
				if( max_leng < img->getComparisionValue( "number" ) ) max_leng = img->getComparisionValue( "number" );
			}
			for( int i = -max_leng / 2; i < ruler_size - max_leng / 2; i++ ) {
				auto image_ruler = cocos2d::ui::ImageView::create( icon_ruler->path );
				image_ruler->setScale9Enabled( true );
				image_ruler->setContentSize( unit_size );
				addChildNode( image_ruler, 2 );
				image_ruler->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 2 + unit_size.width*i , ( _panel->getContentSize().height-100) / ( objects.size() + 2 )) );
				img_ruler.push_back( image_ruler );
			}

			// show object
			int index = 1;
			for( Math_Object *object : objects ) {
				index++;
				object->setPosition( cocos2d::Vec2(_panel->getContentSize().width / 2 - unit_size.width*max_leng / 2 - unit_size.width / 2 + object->getContentSize().width / 2 ,
					( _panel->getContentSize().height-100)*index / ( objects.size() + 2 )));
			}

			for( Math_Object *object : objects ) {
				addChildNode( object, 2 );
				object->onShow( 0 );
			}
		}
	}
	else if( _mode == GameMode::measurememt_use_box_touch_answer ) {
		if( option_game == OptionGame::measure_height ) {
			onShowRightBar( 0, 0.25 );
			// show object
			Math_Object*object = objects[0];
			addChildNode( object, 2 );
			object->onShow( 0 );
			object->setPosition( cocos2d::Vec2( _panel->getContentSize().width * 2 / 3, ( _panel->getContentSize().height - 100 ) / 2 ) );

			// show ruler
			int height = object->getComparisionValue().number;
			for( int i = -height / 2; i < height - height / 2; i++ ) {
				auto image_ruler = cocos2d::ui::ImageView::create( icon_ruler->path );
				image_ruler->setScale9Enabled( true );
				image_ruler->setContentSize( unit_size );
				addChildNode( image_ruler, 2 );
				image_ruler->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 3, ( _panel->getContentSize().height - 100 ) / 2 - image_ruler->getContentSize().height*i
														 - ( object->getComparisionValue().number % 2 == 0?unit_size.height / 2:0 ) ) );
				img_ruler.push_back( image_ruler );
			}
		}
		else if( option_game == OptionGame::measure_leng ) {
			onShowBottomBar( 0, 0.25 );
			// show object
			Math_Object*object = objects[0];
			addChildNode( object, 2 );
			object->onShow( 0 );
			object->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 2, ( _panel->getContentSize().height - 200 ) * 2 / 3 ) );

			// show ruler
			int width = object->getComparisionValue().number;
			for( int i = -width / 2; i < width - width / 2; i++ ) {
				auto image_ruler = cocos2d::ui::ImageView::create( icon_ruler->path );
				image_ruler->setScale9Enabled( true );
				image_ruler->setContentSize( unit_size );
				addChildNode( image_ruler, 2 );
				image_ruler->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 2 - image_ruler->getContentSize().width*i
														 - ( object->getComparisionValue().number % 2 == 0?unit_size.width / 2:0 ), ( _panel->getContentSize().height - 200 ) / 3 ) );
				img_ruler.push_back( image_ruler );
			}
		}
		for( int i = 0; i < 4; i++ ) {
			Math_Answer *ans = Math_Pool::getInstance()->getAnswer();
			ans->setEnable( cocos2d::StringUtils::format( "%d", list_value_answer[i] ),
							math::common::AnswerType::custom_bg,
							CC_CALLBACK_1( Thanh_MeasurementWithMediator::onAnswerDone, this ) );
			ans->setCustomAnswer( math::resource::ResHelper::createImage( getUIPath( "common/done.png" ) ),
								  cocos2d::Size( 200, 60 ), cocos2d::Size( 200, 60 ).height*0.6, cocos2d::Color4B::WHITE, 0.2 );
			_answers.push_back( ans );
			list_answer.push_back( ans );
		}
		correct_answer = list_answer[0];
		std::shuffle( list_answer.begin(), list_answer.end(), g );
		if( option_game == OptionGame::measure_height ) {
			math::func::setPositionList( list_answer, 1, cocos2d::Rect( 0.025, 0.1, 0.2, 0.8 ) );
		}
		else {
			math::func::setPositionList( list_answer, 2, cocos2d::Rect( 0, 0, 1, 0.2 ) );
		}
	}
	else if( _mode == GameMode::measurement_use_ruler ) {
		// TODO: measurement_use_ruler
		std::random_device rd;
		std::mt19937 g( rd() );
		std::shuffle( objects.begin(), objects.end(), g );
		for( Math_Object *object : objects ) {
			if( object->getComparisionValue().number == icons_question[index_question]->getComparisionValue( "number" ) ) {
				correct_answer = object;
			}
		}
		if( option_game == OptionGame::measure_height ) {
			for( int i = -4; i < 6; i++ ) {
				auto image_ruler = cocos2d::ui::ImageView::create( icon_ruler->path );
				image_ruler->setScale9Enabled( true );
				image_ruler->setContentSize( unit_size );
				addChildNode( image_ruler, 2 );
				image_ruler->setPosition( cocos2d::Vec2( 150, _panel->getContentSize().height / 2 - image_ruler->getContentSize().height*i ) );
				img_ruler.push_back( image_ruler );
			}

			// show object
			math::func::setPositionList( objects, ( int )objects.size(), cocos2d::Rect( 0.225, 0.15, 0.7, 1 ) );
			for( Math_Object *object : objects ) {
				object->setPositionY( _panel->getContentSize().height / 2 - unit_size.height*5.5 + object->getContentSize().height / 2 );
			}
			for( Math_Object *object : objects ) {
				addChildNode( object, 2 );
				object->onShow( 0 );
			}
		}
		else {  
			for( int i = -5; i < 5; i++ ) {
				auto image_ruler = cocos2d::ui::ImageView::create( icon_ruler->path );
				image_ruler->setScale9Enabled( true );
				image_ruler->setContentSize( unit_size );
				addChildNode( image_ruler, 2 );
				image_ruler->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 2 + unit_size.width*i, ( _panel->getContentSize().height - 100 ) / ( objects.size() + 2 ) ) );
				img_ruler.push_back( image_ruler );
			} 
			// show object
			int index = 1;
			for( Math_Object *object : objects ) {
				index++;
				object->setPosition( cocos2d::Vec2( _panel->getContentSize().width / 2 - unit_size.width*10 / 2 - unit_size.width / 2 + object->getContentSize().width / 2,
					( _panel->getContentSize().height - 100 )*index / ( objects.size() + 2 ) ) );
			}

			for( Math_Object *object : objects ) {
				addChildNode( object, 2 );
				object->onShow( 0 );
			}
		}
	}

	//Gen question
	showQuestionBox();
}

void Thanh_MeasurementWithMediator::startQuestion()
{
	CCLOG( "%s override", __func__ );

	if( this->getQuestion()->getData().index == 0 ) {
		enableControlQuiz();

		for( Math_Answer* done : _answers ) {
			if( option_game == OptionGame::measure_height ) {
				done->onShowInParent( _panel_right, 2 );
			}
			else {
				done->onShowInParent( _panel_bot, 2 );
			}
		}

		for( Math_Object*object : objects ) {
			object->enableDrag( _panel );
		}
	}
}

void Thanh_MeasurementWithMediator::onExplanation( bool correct )
{
	CCLOG( "%s override", __func__ );
}

void Thanh_MeasurementWithMediator::onAnswerDone( cocos2d::Node *node )
{
	if( node == correct_answer ) {
		( ( Math_Answer* )node )->onAnimationClick( true );
		this->nextQuestion();
	}
	else ( ( Math_Answer* )node )->onAnimationClick( false );
}

void Thanh_MeasurementWithMediator::onComplete()
{
	CCLOG( "%s override", __func__ );

	for( Math_Answer* node : _answers ) {
		if( node->isEnable() ) {
			node->setDisable();
		}
	}

	float delay = showConratulation( 0.3 );

	float m_delay = delay - 0.75;
	if( m_delay < 0 ) {
		m_delay = 0;
	}

	m_delay = delay - 0.7;

	this->runAction( cocos2d::Sequence::create( cocos2d::DelayTime::create( m_delay - 0.4 ), cocos2d::CallFunc::create( [this]()
	{
		for( cocos2d::ui::ImageView *img : img_ruler ) {
			img->runAction( cocos2d::Spawn::createWithTwoActions( cocos2d::FadeOut::create( 0.4 ), cocos2d::RotateBy::create( 0.4, 360 ) ) );
		}
	} ), cocos2d::DelayTime::create( 0.5 ), cocos2d::CallFunc::create( [this]()
	{
		for( Math_Object* object : objects ) object->onHide( 0 );
		for( cocos2d::ui::ImageView *img : img_ruler ) {
			_panel->removeAllChildrenWithCleanup( img );
		}
	} ), nullptr ) );

	scheduleOnce( [=]( float dt )
	{
		hideQuestionBox();
	}, delay, "complete_quiz" );
}

#pragma mark - Auto Testing

#define auto_testing_delay 2
void Thanh_MeasurementWithMediator::autoTesting()
{

	this->scheduleOnce( [=]( float dt )
	{
		this->autoTestingRun( 1.0 );
	}, auto_testing_delay, cocos2d::StringUtils::format( "%s-%d", __func__, testing_step++ ) );
}

void Thanh_MeasurementWithMediator::autoTestingRun( float dt )
{

}

#pragma mark - on this game
void Thanh_MeasurementWithMediator::onTouchedObject( cocos2d::Node* node )
{
	CCLOG( "%s override", __func__ );
	position_on_touch_begin = node->getPosition();
	return;
	if( disable_touch ) return;
	if( _mode == GameMode::measurememt_use_box_touch_object ) {
		Math_Object *result = ( Math_Object* )node;
		if( node != correct_answer ) {
			this->runAction( cocos2d::Sequence::create( cocos2d::CallFunc::create( [=]()
			{
				result->setSelected( true );
			} ), cocos2d::DelayTime::create( 1 ), cocos2d::CallFunc::create( [=]()
			{
				result->setSelected( false );
			} ), nullptr ) );
		}
		else {
			cocos2d::Repeat::create( cocos2d::Sequence::createWithTwoActions( cocos2d::ScaleTo::create( 0.25, 1.1 ), cocos2d::ScaleTo::create( 0.25, 1 ) ), 3 );
			disable_touch = true;
			this->nextQuestion();
		}
	}
	else if( _mode == GameMode::measurement_use_ruler ) {
		Math_Object *result = ( Math_Object* )node;
		if( node != correct_answer ) {
			this->runAction( cocos2d::Sequence::create( cocos2d::CallFunc::create( [=]()
			{
				result->setSelected( true );
			} ), cocos2d::DelayTime::create( 1 ), cocos2d::CallFunc::create( [=]()
			{
				result->setSelected( false );
			} ), nullptr ) );
		}
		else {
			result->runAction( cocos2d::Repeat::create( cocos2d::Sequence::create( cocos2d::ScaleTo::create( 0.25, 1.1 ), cocos2d::ScaleTo::create( 0.25, 1 ), nullptr ), 6 ) );
			disable_touch = true;
			this->runAction( cocos2d::Sequence::create( cocos2d::DelayTime::create( 3 ), cocos2d::CallFunc::create( [=]()
			{
				disable_touch = false;
				index_question++;
				if( index_question < number_question )
				{
					for( Math_Object *object : objects ) {
						if( object->getComparisionValue().number == icons_question[index_question]->getComparisionValue( "number" ) ) {
							correct_answer = object;
						}
					}
				}
				this->nextQuestion();
			} ), nullptr ) );
		}
	}
}

void Thanh_MeasurementWithMediator::onMoveObjectEnded( cocos2d::Node *node, cocos2d::Vec2 point )
{
	CCLOG( "%s override", __func__ );
	if( disable_touch ) return;
	Math_Object *object = ( Math_Object* )node;
	( ( Math_Object* )node )->backToStart();
	if( ( option_game == OptionGame::measure_height && std::abs( node->getPosition().x - position_on_touch_begin.x ) < 10 )
		|| ( option_game == OptionGame::measure_leng && std::abs( node->getPosition().y - position_on_touch_begin.y ) < 10 ) ) {
		if( _mode == GameMode::measurememt_use_box_touch_object ) {
			Math_Object *result = ( Math_Object* )node;
			if( node != correct_answer ) {
				this->runAction( cocos2d::Sequence::create( cocos2d::CallFunc::create( [=]()
				{
					result->setSelected( true );
				} ), cocos2d::DelayTime::create( 1 ), cocos2d::CallFunc::create( [=]()
				{
					result->setSelected( false );
				} ), nullptr ) );
			}
			else {
				auto action = cocos2d::Repeat::create( cocos2d::Sequence::createWithTwoActions( cocos2d::ScaleTo::create( 0.25, 1.1 ), cocos2d::ScaleTo::create( 0.25, 1 ) ), 3 );
				object->runAction( action );
				disable_touch = true;
				this->runAction( cocos2d::Sequence::create( cocos2d::DelayTime::create( 1.5 ), cocos2d::CallFunc::create( [=]()
				{
					this->nextQuestion();
				} ), nullptr ) ); 
			}
		}
		else if( _mode == GameMode::measurement_use_ruler ) {
			Math_Object *result = ( Math_Object* )node;
			if( node != correct_answer ) {
				this->runAction( cocos2d::Sequence::create( cocos2d::CallFunc::create( [=]()
				{
					result->setSelected( true );
				} ), cocos2d::DelayTime::create( 1 ), cocos2d::CallFunc::create( [=]()
				{
					result->setSelected( false );
				} ), nullptr ) );
			}
			else {
				result->runAction( cocos2d::Repeat::create( cocos2d::Sequence::create( cocos2d::ScaleTo::create( 0.25, 1.1 ), cocos2d::ScaleTo::create( 0.25, 1 ), nullptr ), 3 ) );
				disable_touch = true;
				this->runAction( cocos2d::Sequence::create( cocos2d::DelayTime::create( 1.5 ), cocos2d::CallFunc::create( [=]()
				{
					disable_touch = false;
					index_question++;
					if( index_question < number_question )
					{
						for( Math_Object *object : objects ) {
							if( object->getComparisionValue().number == icons_question[index_question]->getComparisionValue( "number" ) ) {
								correct_answer = object;
							}
						}
					}
					this->nextQuestion();
				} ), nullptr ) );
			}
		}
	}
	else {

	}
}
