//
//  NAM_GameTest.cpp
//  MJQuiz
//
//  Created by tunado on 7/29/17.
//
//

#include "Nam_DragObjectToMatrixProperties.h"

Nam_DragObjectToMatrixProperties* Nam_DragObjectToMatrixProperties::createQuiz(){
	Nam_DragObjectToMatrixProperties* quiz = new Nam_DragObjectToMatrixProperties();

	if (quiz->_initQuiz()){
		quiz->autorelease();
		return quiz;
	}
	CC_SAFE_DELETE(quiz);
	return NULL;
}
bool Nam_DragObjectToMatrixProperties::init(){
	return cocos2d::ui::Layout::init();	
}
void Nam_DragObjectToMatrixProperties::initQuiz(mj::model::Game game){

	///////////////////////////////
	// xử lí lấy dữ liệu từ json
	Math_Quiz::initQuiz(game);
    std::string str=cocos2d::FileUtils::getInstance()->getStringFromFile(cocos2d::StringUtils::format("%sconfig.json", _game_path.c_str()));
    rapidjson::Document document;
    document.Parse<0>(str.c_str());

	rapidjson::Value& json_data = document.FindMember("data")->value;
	//TODO: lấy dữ liệu game config
	rapidjson::Value& json_game = json_data.FindMember("game")->value;
	optionCol = json_game.FindMember("game mode")->value.FindMember("option column")->value.GetString();
	optionRow = json_game.FindMember("game mode")->value.FindMember("option row")->value.GetString();
	rowNumberMathSlot = json_game.FindMember("matrix slot")->value.FindMember("num_row")->value.GetInt();
	colNumberMathSlot = json_game.FindMember("matrix slot")->value.FindMember("num_col")->value.GetInt();
	colNumberMathDrager = colNumberMathSlot - 1;
	rowNumberMathDrager = rowNumberMathSlot - 1;
	style_matrix = json_game.FindMember("style matrix")->value.GetString();
	//TODO: lấy dữ liệu object config
	rapidjson::Value& json_object = json_data.FindMember("object")->value;
	objectSize.width = json_object.FindMember("size")->value.FindMember("width")->value.GetDouble();
	objectSize.height = json_object.FindMember("size")->value.FindMember("height")->value.GetDouble();
	//TODO: lấy image
	std::string bg = json_object.FindMember("background")->value.GetString();
	imageBackground = math::resource::ResHelper::createImage(m_theme->getThemePath() + "common/frame/square2.png");
	imageBackground->used = true;
	//TODO: lất dữ liệu cấu hình câu hỏi
	this->parseQuestion();
	// lấy các image để sử dụng
	// IMAGE COLUMN
	if (optionCol == "Color") {
		std::vector <math::resource::Image*> allImagesCol;
        allImagesCol.push_back(this->getColor("white"));
        allImagesCol.push_back(this->getColor("green"));
        allImagesCol.push_back(this->getColor("red"));
        allImagesCol.push_back(this->getColor("purple"));
        allImagesCol.push_back(this->getColor("yellow"));
        math::func::shuffle(allImagesCol);
		int index = 0;
		while (1) {
			if (allImagesCol[index]->getText("color").compare("white") != 0) {
				allImagesCol[index]->used = true;
				imagesObjectCol.push_back(allImagesCol[index]);
				if (imagesObjectCol.size() == colNumberMathDrager) break;
			}	
			index++;
		}
	}
	if (optionCol == "Size") {
        math::resource::Image* imageCol;// = getImageList("icons");
		imageCol = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%sicons.json", _game_path.c_str()));
		imageCol->path = "icons/dots/dot_white.png";
		imageCol->used = true;
		for (int i = 0; i < colNumberMathDrager; i++) {
			imagesObjectCol.push_back(imageCol);
		}
	}
	// IMAGE ROW
	if (optionRow == "id") {
        auto allImagesRow = getImageList("icons");
        math::func::shuffle(allImagesRow);
		if (optionCol == "Color") {
			int index = 0;
			while (1) {
				if (allImagesRow[index]->getText("color").compare("white") == 0) {
					imagesObjectRow.push_back(allImagesRow[index]);
					allImagesRow[index]->used = true;
					if (imagesObjectRow.size() == rowNumberMathDrager) break;
				}
				index++;
			}
			for (int i = 0; i < colNumberMathDrager; i++) {
				for (int j = 0; j < rowNumberMathDrager; j++) {
                    std::string keyName = imagesObjectRow[j]->getText("name_1");
                    std::string keyColor = imagesObjectCol[i]->getText("color");
					for (math::resource::Image* img : allImagesRow) {
                        if ((img->getText("name_1").compare(keyName) == 0)){
                            auto color_img =img->getText("color");
                            if ((color_img.compare(keyColor) == 0)) {
                                img->used = true;
                                imagesObjectResult[i][j] = img;
                                CCLOG("TEST %s", img->path.c_str());
                                break;
                            }
                        }
						
					}
				}
			}
		}
		if (optionCol == "Size") {
			std::vector <math::resource::Image*> allImagesRow = getImageList("icons");
			for (int i = 0; i < 10; i++) {
				int j = RandomHelper::random_int(10, (int)allImagesRow.size() - 1);
				std::swap(allImagesRow[i], allImagesRow[j]);
			}
			//std::random_shuffle(allImagesRow.begin(), allImagesRow.end());
			for (int i = 0; i < rowNumberMathDrager; i++) {
				imagesObjectRow.push_back(allImagesRow[i]);
				allImagesRow[i]->used = true;
			}
			for (int i = 0; i < colNumberMathDrager; i++) {
				for (int j = 0; j < rowNumberMathDrager; j++) {
					imagesObjectResult[i][j] = imagesObjectRow[j];					
				}
			}
		}
	}
	if (optionRow == "Size") {
		std::vector <math::resource::Image*> allImagesRow = getImageList("icons");
		for (int i = 0; i < 10; i++) {
			int j = RandomHelper::random_int(0, (int)allImagesRow.size() - 1);
			std::swap(allImagesRow[i], allImagesRow[j]);
		}
		if (optionCol == "Color") {
			for (math::resource::Image* img : allImagesRow)
				if (img->getText("color").compare("white") == 0) {
					for (int i = 0; i < rowNumberMathDrager; i++) {
						imagesObjectRow.push_back(img);					
					}
					img->used = true;
					break;
				}
			for (int i = 0; i < colNumberMathDrager; i++) {
				for (int j = 0; j < rowNumberMathDrager; j++) {
					for (math::resource::Image* img : allImagesRow) {
						if ((img->getText("color").compare(imagesObjectCol[i]->getText("color")) == 0)
							&& (img->getText("name_1").compare(imagesObjectRow[j]->getText("name_1")) == 0)) {
							img->used = true;
							imagesObjectResult[i][j] = img;
							CCLOG("TEST %s", img->path.c_str());
							break;
						}
					}
				}
			}
		}
	}
	if (optionRow == "Color") {
		std::vector <math::resource::Image*> allImagesRow = this->getColors();
		/*for (int i = 0; i < 10; i++) {
			int j = RandomHelper::random_int(0, (int)allImagesRow.size() - 1);
			std::swap(allImagesRow[i], allImagesRow[j]);
		}*/
		std::random_shuffle(allImagesRow.begin(), allImagesRow.end());
		if (optionCol == "Size") {
			int index = 0;
			while (1) {
				if (allImagesRow[index]->getText("color").compare("white") != 0) {
					allImagesRow[index]->used = true;
					imagesObjectRow.push_back(allImagesRow[index]);
					if (imagesObjectRow.size() == rowNumberMathDrager) break;
				}
				index++;
			}
			for (int i = 0; i < colNumberMathDrager; i++) {
				for (int j = 0; j < rowNumberMathDrager; j++) {
					imagesObjectResult[i][j] = imagesObjectRow[j];
				}
			}
		}
	}

	// FIX GAME CHUNG
	float delay = 0.3;
    //frame
    math::resource::Image* img_frame = math::resource::ResHelper::createImage(getUIPath("common/frame/square.png"));
    img_frame->used = true;
    objectFrame = Math_Pool::getInstance()->getObject();
    objectFrame->setEnable(0, math::common::TouchPermission::deny, img_frame, NULL);
    objectFrame->setObjectSize(Size(objectSize.width*colNumberMathSlot+45,objectSize.height*rowNumberMathSlot+35));
    //objectFrame->setObjectSize(Size(500,500));
    objectFrame->setPosition(Vec2(quiz_size.width/2,quiz_size.height*0.3));
    this->addChildNode(objectFrame,2);
    objectFrame->onShow(0);
	//Gen matrix slot
	float posX_MS, posX_MD, posY_MS, posY_MD, heightOfButton = 100;
    posX_MS = (quiz_size.width - objectSize.width*(colNumberMathSlot - 1)) / 2;
    posX_MD = (quiz_size.width - objectSize.width*(colNumberMathDrager - 1)) / 2;
    posY_MS = ((575 - objectSize.height*(rowNumberMathSlot + rowNumberMathDrager) - heightOfButton) / 3) * 2 + heightOfButton
        + objectSize.height*(rowNumberMathDrager)+objectSize.height / 2;
    posY_MS-=0.3*quiz_size.height;
    posY_MD = ((575 - objectSize.height*(rowNumberMathSlot + rowNumberMathDrager) - heightOfButton) / 3) + heightOfButton + objectSize.height / 2;
	for (int i = 0; i < colNumberMathSlot; i++) {
		for (int j = 0; j < rowNumberMathSlot; j++) {
			Math_Slot* slot = Math_Pool::getInstance()->getSlot();
			this->genSlot(slot, imageBackground, Vec2(posX_MS + i*objectSize.width, posY_MS + (rowNumberMathSlot - 1 - j)*objectSize.height + 20));
			matrixSlot[i][j] = slot;
			slot->setName("1");
			_slots.push_back(slot);
			if (j == 0 && i > 0) {
				slot->setName("0");
				Math_Object* object = Math_Pool::getInstance()->getObject();
				if (optionCol == "Size") {
					this->genObject(object, imagesObjectCol[i - 1], slot->getPosition(), Size(objectSize.width*(1 - dSize*(i - 1)), objectSize.height*(1 - dSize*(i - 1))), true);
				}
				else {
					this->genObject(object, imagesObjectCol[i - 1], slot->getPosition(), objectSize, true);
				}
				this->addChildNode(object,3);
				_objects.push_back(object);
				object->onShow(delay);
				slot->setLinker(object);
			}
			if (i == 0 && j > 0) {
				slot->setName("0");
				Math_Object* object = Math_Pool::getInstance()->getObject();
				if (optionRow == "Size") {
					this->genObject(object, imagesObjectRow[j - 1], slot->getPosition(), Size(objectSize.width*(1 - dSize*(j - 1)), objectSize.height*(1 - dSize*(j - 1))), true);
				}
				else {
					this->genObject(object, imagesObjectRow[j - 1], slot->getPosition(), objectSize, true);
				}
				this->addChildNode(object,3);
				_objects.push_back(object);
				object->onShow(delay);
				slot->setLinker(object);
			}
		}
		delay += 0.1;
	}
	matrixSlot[0][0]->setLinker(NULL);
	matrixSlot[0][0]->setName("0");
    std::vector <Math_Slot*> slots_1;
    for (int i = 0; i < rowNumberMathSlot; i++) {
        for (int j = 0; j < colNumberMathSlot; j++) {
            slots_1.push_back(matrixSlot[j][i]);
        }
    }
    math::func::setPositionListOnFrame(slots_1, colNumberMathSlot, rowNumberMathSlot, objectSize, Vec2(quiz_size.width/2,quiz_size.height*0.3), 0);
    for (auto s : slots_1){
        if (s->getName() == "0" && s!=matrixSlot[0][0]){
            s->getLinker()->setPosition(s->getPosition());
        }
    }
    
    // drager
    std::vector <Vec2> vecPos = Math_Quiz::randomPositionsInRect(objectSize, cocos2d::Rect(50,0,0.7*quiz_size.width,0.28*quiz_size.height), colNumberMathDrager*rowNumberMathDrager);
	for (int i = 0; i < colNumberMathDrager; i++) {
		for (int j = 0; j < rowNumberMathDrager; j++) {
			Math_Slot* slot = Math_Pool::getInstance()->getSlot();
			this->genSlot(slot, NULL, Vec2(posX_MD + i*objectSize.width, posY_MD + (rowNumberMathDrager - 1 - j)*objectSize.height));
            slot->setPosition(vecPos[i*rowNumberMathDrager+j]);
			matrixDrager[i][j] = slot;
			_slots.push_back(slot);
			matrixSlot[i + 1][j + 1]->setValueText(cocos2d::StringUtils::toString(j*colNumberMathDrager + i));
			matrixSlot[i + 1][j + 1]->setValueTextVisible(false);
           
		}
	}
   
	for (int i = 0; i < colNumberMathDrager; i++) {
		for (int j = 0; j < rowNumberMathDrager; j++) {
			int x, y;
			while (1) {
				x = RandomHelper::random_int(0, colNumberMathDrager - 1);
				y = RandomHelper::random_int(0, rowNumberMathDrager - 1);
				if (!matrixDrager[x][y]->isUsed()) {
					break;
				}
			}
			Math_Object* object = Math_Pool::getInstance()->getObject();
			if (optionCol == "Size") {
				this->genObject(object, imagesObjectResult[i][j], matrixDrager[x][y]->getPosition(), Size(objectSize.width*(1 - dSize*i), objectSize.height*(1 - dSize*i)), false);
			}
			else {
				if (optionRow == "Size") {
					this->genObject(object, imagesObjectResult[i][j], matrixDrager[x][y]->getPosition(), Size(objectSize.width*(1 - dSize*j), objectSize.height*(1 - dSize*j)), false);
				}
				else this->genObject(object, imagesObjectResult[i][j], matrixDrager[x][y]->getPosition(), objectSize, false);
			}
			objectsDrager[i][j] = object;
			objectsDrager[i][j]->setValueText(cocos2d::StringUtils::toString(j*colNumberMathDrager + i));
			cocos2d::ui::Text* text = objectsDrager[i][j]->getLabelValue();
			text->setVisible(false);
            object->setPosition(matrixDrager[x][y]->getPosition());
			this->addChildNodeToBottomPanel(object,2);
			_objects.push_back(object);
			object->onShow(delay);
			matrixDrager[x][y]->setLinker(object);
		}
		delay += 0.05;
	}
    
	// show slot and btn
	for (int i = 0; i < colNumberMathSlot; i++) {
		for (int j = 0; j < rowNumberMathSlot; j++) {
            this->addChildNode(matrixSlot[i][j], 2);
			matrixSlot[i][j]->onShow(0);
		}
		delay += 0.1;
	}
	for (int i = 0; i < colNumberMathDrager; i++) {
		for (int j = 0; j < rowNumberMathDrager; j++) {
			this->addChildNodeToBottomPanel(matrixDrager[i][j], 2);
			matrixDrager[i][j]->onShow(delay);
		}
		delay += 0.1;
	}
	//Math_Pool::getInstance()->resetImages();
}
void Nam_DragObjectToMatrixProperties::onStart() {
	
	
	//Gen question
    onShowBottomBar(0, 0.3,true, CC_CALLBACK_1(Nam_DragObjectToMatrixProperties::onAnswerDone, this));
	this->showQuestionBox();
    
}
void Nam_DragObjectToMatrixProperties::startQuestion() {
    for (int i = 0; i < colNumberMathSlot-1; i++) {
        for (int j = 0; j < rowNumberMathSlot-1; j++) {
            objectsDrager[i][j]->enableDrag(_panel_bot);
        }
    }
	enableControlQuiz();
};
void Nam_DragObjectToMatrixProperties::onExplanation(bool correct) {
}
void Nam_DragObjectToMatrixProperties::onComplete() {
	//TODO: Thuc hien them cac lenh o day
	for (Math_Answer* node :_answers) {
		if (node->isEnable()) {
			node->setDisable();
		}
	}

	float delay = showConratulation(0.3);
	float m_delay = delay - 0.3;
    objectFrame->onHide(m_delay);
	for (Math_Object* object : _objects) {
		object->onHide(m_delay);
	}
	for (Math_Slot* slot : _slots) {
		slot->onHide(delay);
	}
	scheduleOnce([=](float dt) {
		hideQuestionBox();
	}, delay, "complete_quiz");
}
void Nam_DragObjectToMatrixProperties::onAnswerDone(Node* node) {
	bool done = true;
    Math_Answer* button = (Math_Answer*)node;
	for (int i = 1; i < colNumberMathSlot; i++) {
		for (int j = 1; j < rowNumberMathSlot; j++) {
			if (matrixSlot[i][j]->isUsed()) {
				if (matrixSlot[i][j]->getValueText().compare(matrixSlot[i][j]->getLinker()->getValueText()) != 0) {
					done = false;
					if (matrixSlot[i][j]->isUsed()) {
						Math_Object* obj = matrixSlot[i][j]->getLinker();
						matrixSlot[i][j]->removeLinker();
						//obj->runAction(MoveTo::create(0.2f, this->slotDragerForBack()->getPosition()));
                        obj->backToStart();
                        this->slotDragerForBack()->setLinker(obj);
					}
				}
			}
			else {
				done = false;
			}
		}
	}
	if (done) {
		button->onAnimationClick(true);
		this->nextQuestion();
	}
	else {
		button->onAnimationClick(false);
	}
}
Math_Slot* Nam_DragObjectToMatrixProperties::slotDragerForBack() {
	for (int i = 0; i < colNumberMathDrager; i++) {
		for (int j = 0; j < rowNumberMathDrager; j++) {
			if (!matrixDrager[i][j]->isUsed()) {
				return matrixDrager[i][j];
			}
		}
	}
}
void Nam_DragObjectToMatrixProperties::onTouchedObject(cocos2d::Node* node) {
	Math_Object* object = (Math_Object*)node;
	if (selected_object == NULL) {
		selected_object = object;
	}
	else {
		object->setAllowMove(false);
	}
}
void Nam_DragObjectToMatrixProperties::onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 position) {
	//CODE ANH PHUONG
	Math_Object* object = (Math_Object*)node;
    position = _panel->convertToNodeSpace(position);
    object->removeFromParentAndCleanup(false);
    object->setPosition(position);
    addChildNode(object, 2);
	//Doi tuong keo tha nam trong 1 slot nao do.
	Math_Slot* m_slot = NULL;
	for (Math_Slot* slot : _slots) {
		if (object->isSame(slot->getLinker())) {
			m_slot = slot;
			break;
		}
	}

	Math_Slot* t_slot = NULL;
	for (Math_Slot* slot : _slots) {
		if (slot->isAvailable(position) && slot->getName()=="1") {
			t_slot = slot;
			break;
		}
	}

	if (m_slot != NULL) {
		if (t_slot != NULL) {
			//swap
			if (m_slot == t_slot) {
				object->backToLast(2);
			}
			else {
				if (t_slot->isUsed()) {
					auto linker = t_slot->getLinker();
					m_slot->removeLinker();
					t_slot->removeLinker();

					t_slot->setLinker(object);
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));

					m_slot->setLinker(linker);
					linker->runAction(cocos2d::MoveTo::create(0.3, m_slot->getPosition()));
				}
				else {
					m_slot->removeLinker();
					t_slot->setLinker(object);
					object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
				}
			}
		}
		else {
			//move to start
			object->backToLast(2);
			//m_slot->removeLinker();
			//object->backToStart();
		}
	}
	else {
		if (t_slot != NULL) {
			if (t_slot->isUsed()) {
				auto linker = t_slot->getLinker();
				t_slot->removeLinker();
				linker->backToStart();
			}
			t_slot->setLinker(object);
			object->runAction(cocos2d::MoveTo::create(0.2, t_slot->getPosition()));
		}
		else {
			//move start
			object->backToStart();
		}
	}
	selected_object = NULL;
}
void Nam_DragObjectToMatrixProperties::genSlot(Math_Slot* slot, math::resource::Image* bg, Vec2 pos) {
	slot->setEndble(1, bg);
	slot->setSlotSize(objectSize);
    //slot->getBackground()->setContentSize(objectSize);
	slot->setPosition(pos);
}
void Nam_DragObjectToMatrixProperties::genObject(Math_Object* object, math::resource::Image* image, Vec2 pos,Size size, bool looked) {
	object->setEnable(0, math::common::TouchPermission::move, image, NULL);
	object->setObjectSize(size);
	object->setPosition(pos);
	object->setBackgroundVisible(false);
	object->setLocked(looked);
	if (looked == false) {
		object->setMoveCallback(CC_CALLBACK_1(Nam_DragObjectToMatrixProperties::onTouchedObject, this), CC_CALLBACK_2(Nam_DragObjectToMatrixProperties::onMoveObjectEnded, this));
	}
}
#define auto_testing_delay 2
void Nam_DragObjectToMatrixProperties::autoTesting() {
	this->scheduleOnce([=](float dt) {
		this->autoTestingRun(1.0);
	}, auto_testing_delay, cocos2d::StringUtils::format("testing-%d", testing_step++));
}
void Nam_DragObjectToMatrixProperties::autoTestingRun(float delaytime) {
	
	
}

