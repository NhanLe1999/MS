#include "ScoreBoardDetail.h"
#include "MJHelper.h"
#include "ProScore.h"
#define PADDING_HORIZONTAL 20
#define PADDING_VERTICAL 20
#define PADDING_COLLUMNS 10
#define LINE_WEIGHT 5
#define FIRST_ELEMENT_PADDING_TOP PADDING_VERTICAL

ScoreBoardDetail* ScoreBoardDetail::createScoreBoardDetail(int score, std::string sentence, std::vector<std::pair<std::string, float>> wordSore) {
	auto board = ScoreBoardDetail::create();
	board->prepareData(score,sentence, wordSore);
	board->loadUI();
	return board;
}
void ScoreBoardDetail::prepareData(int score, std::string sentence, std::vector<std::pair<std::string, float>> wordScore) {
	_score = score;
	_sentence = sentence;
	_wordScore = wordScore;
}
void ScoreBoardDetail::loadUI() {
	this->createElement();

	this->setContentSize(calculateTotalSize(_headLeft,_rightElements));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	arrangeElement();
	_background = ImageView::create("games/proscore/scoreBoard.png");
	_background->setScale9Enabled(true);
	
	_background->setCapInsets(Rect(Vec2(50, 50), _background->getBoundingBox().size - Size(50*2,50*2)));
	_background->setContentSize(this->getContentSize());
	this->addChild(_background, -1);
	_background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_background->setPosition(this->getContentSize() / 2);
	coloringImage();
	drawLines();
	createTextElements();
	fitToScreen();
	this->setTouchEnabled(true);
	this->setSwallowTouches(true);
	//this->setBackGroundColor(Color3B::RED);
	//this->setBackGroundColorOpacity(100);
	//this->setBackGroundColorType(BackGroundColorType::SOLID);
}

void ScoreBoardDetail::createElement() {
	_headLeft = Layout::create();
	_fullText = createColoringText();
	auto earButton = Button::create("games/proscore/ear.png", "games/proscore/ear.png");
	earButton->setScale(0.75);
	_headLeft->setContentSize(_fullText->getContentSize()+ earButton->getBoundingBox().size + Size(30, 0));
	_headLeft->addChild(_fullText);
	_headLeft->addChild(earButton);
	_fullText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_fullText->setPosition(Vec2(PADDING_HORIZONTAL, _headLeft->getBoundingBox().size.height / 2));
	earButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	earButton->setPosition(_headLeft->getBoundingBox().size - Size(0,20));
	earButton->setTouchEnabled(true);
	earButton->addClickEventListener([=](Ref*) {
		if (_delegate) _delegate->replayAudio();
	});
    //disable ear button
    disableEarButton(earButton);
    
	this->addChild(_headLeft);
	//_headLeft->setBackGroundColor(Color3B::RED);
	//_headLeft->setBackGroundColorOpacity(100);
	//_headLeft->setBackGroundColorType(BackGroundColorType::SOLID);
	
	for (auto elementData : _wordScore) {
		auto layout = Layout::create();
		auto text = Text::create(scoreToMessage(elementData.second), FONT_NAME, _fontSize * 0.6);
		text->setTextColor(Color4B(scoreToColor(elementData.second)));
		auto icon = ImageView::create(scoreToIconPath(elementData.second));
		icon->setColor(scoreToColor(elementData.second));
		layout->addChild(text);
		text->setTag(1);
		layout->addChild(icon);
		icon->setTag(2);

		text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		text->setTextVerticalAlignment(TextVAlignment::CENTER);
		text->setTextHorizontalAlignment(TextHAlignment::LEFT);
		icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		_rightElements.push_back(layout);
		this->addChild(layout);
		//layout->setBackGroundColor(Color3B::RED);
		//layout->setBackGroundColorOpacity(100);
		//layout->setBackGroundColorType(BackGroundColorType::SOLID);
	}
	alignElement(_rightElements);
}

void ScoreBoardDetail::alignElement(std::vector<Layout*> elements) {
	Size maxSize = Size(0, 0);
	for (auto element : elements) {
		Size totalElementSize = Size(0, 0);
		if (utils::findChild(element, 1)) {//text
			totalElementSize.width += utils::findChild(element, 1)->getBoundingBox().size.width;
			totalElementSize.height = utils::findChild(element, 1)->getBoundingBox().size.height;
		}
		if (utils::findChild(element, 2)) {//ICON
			totalElementSize.width += utils::findChild(element, 2)->getBoundingBox().size.width;
			if (utils::findChild(element, 2)->getBoundingBox().size.height > totalElementSize.height) {
				totalElementSize.height = utils::findChild(element, 2)->getBoundingBox().size.height;
			}
		}
		if (maxSize.width < totalElementSize.width) maxSize.width = totalElementSize.width;
		if (maxSize.height < totalElementSize.height) maxSize.height = totalElementSize.height;
	}
	float lefPadding = 10;//text left padding
	float margin = 30;
	maxSize.width += margin + PADDING_HORIZONTAL + lefPadding;
	for (auto element : elements) {
		element->setContentSize(maxSize);
		if (utils::findChild(element, 1)) {//text
			utils::findChild(element, 1)->setPosition(lefPadding, element->getContentSize().height / 2);
		}
		if (utils::findChild(element, 2)) {//ICON
			utils::findChild(element, 2)->setPosition(element->getContentSize().width - PADDING_HORIZONTAL, element->getContentSize().height / 2);
		}
	}
}
Size ScoreBoardDetail::calculateTotalSize(Layout* _headLeft, std::vector<Layout*> elements) {
	Size boardSize = Size(0, 0);
	boardSize.width = _headLeft->getBoundingBox().size.width + elements.front()->getBoundingBox().size.width + PADDING_COLLUMNS;
	boardSize.height += _headLeft->getBoundingBox().size.height;
	for (auto element : elements) {
		boardSize.height += element->getBoundingBox().size.height + PADDING_VERTICAL;
	}
	boardSize.height += FIRST_ELEMENT_PADDING_TOP;
	return boardSize;

}
void ScoreBoardDetail::arrangeElement() {
	_headLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_headLeft->setPosition(Vec2(0, this->getContentSize().height));
	for (int i = 0; i < _rightElements.size(); i++) {
		_rightElements[i]->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		auto pos = Vec2(this->getContentSize().width, this->getContentSize().height - _headLeft->getBoundingBox().size.height - i * (_rightElements[i]->getBoundingBox().size.height + PADDING_VERTICAL) - FIRST_ELEMENT_PADDING_TOP);
		_rightElements[i]->setPosition(pos);
	}
}
std::string ScoreBoardDetail::scoreToIconPath(int score) {
	if (score < 20) {
		return "games/proscore/incorrect.png";
	}
	return "games/proscore/correct.png";
}
void ScoreBoardDetail::coloringImage() {
	ImageView* color = ImageView::create("games/proscore/r_"+scoreToColorString(_score)+".png");
	color->setScale9Enabled(true);
	color->setCapInsets(Rect(Vec2(50,50), color->getContentSize() - Size(100,100)));
	color->setContentSize(Size(_rightElements.front()->getBoundingBox().size.width, _headLeft->getBoundingBox().size.height) - Size(LINE_WEIGHT, LINE_WEIGHT));
	color->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	this->addChild(color);
	color->setPosition(this->getContentSize() - Size(LINE_WEIGHT, LINE_WEIGHT));
	color->setName("color");

	std::string textScore = std::to_string(_score) + "%";

	auto score = Text::create(textScore, FONT_NAME, mj::helper::getMaxFontSize({ textScore }, color->getBoundingBox().size * 0.6, INT_MAX, FONT_NAME));
	score->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	color->addChild(score);
	score->setPosition(color->getContentSize() / 2);
}
void ScoreBoardDetail::drawLines() {
	auto lineDrawner = DrawNode::create(LINE_WEIGHT);
	auto color = utils::findChild<ImageView*>(this, "color");
	auto verticalLineBegin = color->getPosition() - Vec2(color->getContentSize().width,0);
	lineDrawner->drawLine(verticalLineBegin, verticalLineBegin - Vec2(0, this->getContentSize().height - LINE_WEIGHT),Color4F::BLACK);
	this->addChild(lineDrawner);
	for (int i = 0; i < _rightElements.size();i++) {
		auto lineDrawner = DrawNode::create(LINE_WEIGHT);
		auto lineBegin = _rightElements[i]->getPosition() + Vec2(0,PADDING_VERTICAL/2);
		if (i == 0) {
			lineBegin.y += PADDING_VERTICAL / 2;
		}
		auto lineEnd = lineBegin + Vec2(-this->getContentSize().width, 0);
		lineDrawner->drawLine(lineBegin, lineEnd, Color4F::BLACK);
		this->addChild(lineDrawner);

	}
}
Color3B ScoreBoardDetail::scoreToColor(int score) {
	if (score < 20) return Color3B::RED;
	if (score >= 20 && score < 60) return Color3B::ORANGE;
	if (score >= 60) return Color3B(14, 168, 69);//green
	return Color3B(14, 168, 69);//green
}
std::string ScoreBoardDetail::scoreToMessage(int score) {
	if ( score < 20) return "Incorrect";
	if (score >= 20 && score < 60) return "Almost correct";
	if (score >= 60) return "Excellent";
	return "Excellent";
}
void ScoreBoardDetail::createTextElements() {
	for (int i = 0; i < _wordScore.size(); i++) {
		auto textElement = Text::create(_wordScore[i].first, FONT_NAME, _fontSize);
		textElement->setTextVerticalAlignment(TextVAlignment::CENTER);
		textElement->setTextHorizontalAlignment(TextHAlignment::LEFT);
		textElement->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(textElement);
		textElement->setPosition(Vec2(_headLeft->getBoundingBox().size.width/2, _rightElements[i]->getPositionY() - _rightElements[i]->getBoundingBox().size.height/2));
		textElement->setTextColor(Color4B(scoreToColor(_wordScore[i].second)));
		_listTextElements.push_back(textElement);
	}
}
RichText* ScoreBoardDetail::createColoringText() {
	auto maskText = Text::create(_sentence, FONT_NAME, _fontSize);
	ui::RichText* fulltext = ui::RichText::create();
	fulltext->ignoreContentAdaptWithSize(true);
	fulltext->setContentSize(maskText->getContentSize());
	for (int i = 0; i < _wordScore.size();i++) {
		ui::RichElementText* re = ui::RichElementText::create(i, scoreToColor(_wordScore[i].second), 255, _wordScore[i].first, FONT_NAME, _fontSize);
		fulltext->pushBackElement(re);
	}
	return fulltext;
}
void ScoreBoardDetail::fitToScreen() {
	auto screenSize = Director::getInstance()->getVisibleSize();
	if (this->getBoundingBox().size.width <= screenSize.width * 0.8 && this->getBoundingBox().size.height <= screenSize.height * 0.8) {
		return;
	}
	float scale, scaleByWidth, scaleByHeight;
	scaleByWidth = screenSize.width * 0.8 / this->getBoundingBox().size.width;
	scaleByHeight = screenSize.height * 0.8 / this->getBoundingBox().size.height;
	if (scaleByWidth < scaleByHeight) scale = scaleByWidth;
	else scale = scaleByHeight;
	this->setScale(scale);
}
std::string ScoreBoardDetail::scoreToColorString(int score) {
	if (score < 20) return "red";
	if (score >= 20 && score < 60) return "orange";
	if (score >= 60) return "green";
	return "green";
}
void ScoreBoardDetail::disableEarButton(Button* earButton) {
	earButton->setTouchEnabled(false);
	earButton->setVisible(false);
	_fullText->setPosition(_fullText->getPosition() + Vec2(20, 0));
}
