//
//  HelloWorldScene.h
//  篮球-cocos2dx-box2d
//
//  Created by msp on 13-11-6.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"




using namespace cocos2d;


//像素到米的转换
#define PTM_TATIO 32.0
#define BALL_TAG -1111

class HelloWorld : public cocos2d::CCLayer {
    b2World     * m_pWorld;
    b2Body      * m_pBody;
    b2Body      * m_pBody2;
    CCSprite    * m_pBall;
    CCSprite    * m_pBall2;
    int           m_iBallNum;
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void SetFouce();
    bool AddBall(CCPoint point);
    void tick(cocos2d::ccTime dt);
    void bomb(CCPoint position);
};

#endif // __HELLO_WORLD_H__
