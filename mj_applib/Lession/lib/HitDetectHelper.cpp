//
//  HitDetectHelper.cpp
//
//  Created by Alexander Wong on 1/21/15.
//
//

#include "HitDetectHelper.h"

USING_NS_CC;

#pragma mark - HitDetectHelper

RenderTexture* HitDetectHelper::_sharedRenderTexture = NULL;
HitDetectHelper::HitDetectHelperNode* HitDetectHelper::_helperNode = NULL;

#pragma mark - Pixel Test

bool HitDetectHelper::hitTest(ui::ImageView * image_view, const Point & point) {
    if (!_sharedRenderTexture) {
        _sharedRenderTexture = RenderTexture::create(1, 1);
        _sharedRenderTexture->retain();

        _sharedRenderTexture->setKeepMatrix(true);
        Size winSize = Director::getInstance()->getWinSize();
        Size winSizeInPixels = Director::getInstance()->getWinSizeInPixels();
        _sharedRenderTexture->setVirtualViewport(Point(0.0f, 0.0f), Rect(0.0f, 0.0f, winSize.width, winSize.height), Rect(0.0f, 0.0f, winSizeInPixels.width, winSizeInPixels.height));
        _sharedRenderTexture->setAutoDraw(false);
    }

    if (!_helperNode) {
        _helperNode = HitDetectHelperNode::create(Point::ZERO);
        _helperNode->retain();
    }
    else {
        _helperNode->reset();
    }

    _sharedRenderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);

//    if (sprite->getBatchNode()) {
//        Sprite* tempSprite = Sprite::createWithTexture(sprite->getTexture());
//        tempSprite->setTextureRect(sprite->getTextureRect(), sprite->isTextureRectRotated(), sprite->getContentSize());
//        sprite = tempSprite;
//    }

    // store transforms
    Point position = image_view->getPosition();
    Point anchorPoint = image_view->getAnchorPoint();
    float rotationX = image_view->getRotationSkewX();
    float rotationY = image_view->getRotationSkewY();
    float skewX = image_view->getSkewX();
    float skewY = image_view->getSkewY();

    // unset transforms
    image_view->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    image_view->setPosition(Point(-point.x, -point.y));
    image_view->setRotationSkewX(0.0f);
    image_view->setRotationSkewY(0.0f);
    image_view->setSkewX(0.0f);
    image_view->setSkewY(0.0f);

    // draw node
    image_view->visit(Director::getInstance()->getRenderer(), Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW), true);

    // add command to read pixel
    _helperNode->visit();

    // revert transforms
    image_view->setAnchorPoint(anchorPoint);
    image_view->setPosition(position);
    image_view->setRotationSkewX(rotationX);
    image_view->setRotationSkewY(rotationY);
    image_view->setSkewX(skewX);
    image_view->setSkewY(skewY);

    _sharedRenderTexture->end();

    Director::getInstance()->getRenderer()->render(); // render texture fix

    return _helperNode->getPixelValue(0) || _helperNode->getPixelValue(1) || _helperNode->getPixelValue(2) || _helperNode->getPixelValue(3);
}

bool HitDetectHelper::hitTest(Sprite* sprite, const Point& point)
{
    if (!_sharedRenderTexture) {
        _sharedRenderTexture = RenderTexture::create(1, 1);
        _sharedRenderTexture->retain();

        _sharedRenderTexture->setKeepMatrix(true);
        Size winSize = Director::getInstance()->getWinSize();
        Size winSizeInPixels = Director::getInstance()->getWinSizeInPixels();
        _sharedRenderTexture->setVirtualViewport(Point(0.0f, 0.0f), Rect(0.0f, 0.0f, winSize.width, winSize.height), Rect(0.0f, 0.0f, winSizeInPixels.width, winSizeInPixels.height));
        _sharedRenderTexture->setAutoDraw(false);
    }

    if (!_helperNode) {
        _helperNode = HitDetectHelperNode::create(Point::ZERO);
        _helperNode->retain();
    }
    else {
        _helperNode->reset();
    }

    _sharedRenderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);

    if (sprite->getBatchNode()) {
        Sprite* tempSprite = Sprite::createWithTexture(sprite->getTexture());
        tempSprite->setTextureRect(sprite->getTextureRect(), sprite->isTextureRectRotated(), sprite->getContentSize());
        sprite = tempSprite;
    }

    // store transforms
    Point position = sprite->getPosition();
    Point anchorPoint = sprite->getAnchorPoint();
    float rotationX = sprite->getRotationSkewX();
    float rotationY = sprite->getRotationSkewY();
    float skewX = sprite->getSkewX();
    float skewY = sprite->getSkewY();

    // unset transforms
    sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    sprite->setPosition(-point.x, -point.y);
    sprite->setRotationSkewX(0.0f);
    sprite->setRotationSkewY(0.0f);
    sprite->setSkewX(0.0f);
    sprite->setSkewY(0.0f);

    // draw node
    sprite->visit();

    // add command to read pixel
    _helperNode->visit();

    // revert transforms
    sprite->setAnchorPoint(anchorPoint);
    sprite->setPosition(position);
    sprite->setRotationSkewX(rotationX);
    sprite->setRotationSkewY(rotationY);
    sprite->setSkewX(skewX);
    sprite->setSkewY(skewY);

    _sharedRenderTexture->end();

    Director::getInstance()->getRenderer()->render(); // render texture fix

    return _helperNode->getPixelValue(0) || _helperNode->getPixelValue(1) || _helperNode->getPixelValue(2) || _helperNode->getPixelValue(3);
}

#pragma mark - HitDetectHelperNode

#pragma mark - Reset

void HitDetectHelper::HitDetectHelperNode::reset()
{
    _pixelRead = false;

    _pixelBuffer[0] = 0;
    _pixelBuffer[1] = 0;
    _pixelBuffer[2] = 0;
    _pixelBuffer[3] = 0;
}

#pragma mark - Pixel Buffer

uint8_t HitDetectHelper::HitDetectHelperNode::getPixelValue(unsigned int index)
{
    CCASSERT(_pixelRead, "Pixel not read yet.");
    CCASSERT(index < 4, "Out of bounds.");

    return _pixelBuffer[index];
}

#pragma mark - Draw

void HitDetectHelper::HitDetectHelperNode::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(HitDetectHelper::HitDetectHelperNode::readPixel, this);
    renderer->addCommand(&_customCommand);
}

#pragma mark - Read Pixel

void HitDetectHelper::HitDetectHelperNode::readPixel()
{
    _pixelRead = true;

    glReadPixels(_pixelPoint.x, _pixelPoint.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, _pixelBuffer);
}

#pragma mark - Constructors

HitDetectHelper::HitDetectHelperNode* HitDetectHelper::HitDetectHelperNode::create(const Point& pixelPoint)
{
    HitDetectHelper::HitDetectHelperNode* node = new HitDetectHelper::HitDetectHelperNode();
    if (node && node->init(pixelPoint)) {
        node->autorelease();
    }
    else {
        CC_SAFE_DELETE(node);
    }
    return node;
}

HitDetectHelper::HitDetectHelperNode::HitDetectHelperNode()

// instance variables
: _pixelRead(false)

{

}

bool HitDetectHelper::HitDetectHelperNode::init(const Point &pixelPoint)
{
    _pixelPoint = pixelPoint;

    reset();

    return Node::init();
}
