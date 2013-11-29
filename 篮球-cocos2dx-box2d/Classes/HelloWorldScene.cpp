//
//  HelloWorldScene.cpp
//  篮球-cocos2dx-box2d
//
//  Created by msp on 13-11-6.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <CoreGraphics/CoreGraphics.h>
#include "MyContactListener.h"

using namespace CocosDenshion;

#define PTM_RATIO 32
enum
{
	kTagTileMap = 1,
	kTagSpriteManager = 1,
	kTagAnimation1 = 1,
};

HelloWorld::HelloWorld()
{
    this->setIsTouchEnabled(true);
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    //背景
    CCSprite * bg = CCSprite::spriteWithFile("bg.png");
    bg->setAnchorPoint(ccp(0,0));
    this->addChild(bg,-1);
    
    
    //篮球
    m_pBall = CCSprite::spriteWithFile("ball.png",CCRect(0,0,52,52));
    m_pBall->setPosition(ccp(100,200));
    m_pBall->setTag(BALL_TAG);
    this->addChild(m_pBall);
    
    m_pBall2 = CCSprite::spriteWithFile("ball.png",CCRect(0,0,52,52));
    m_pBall2->setPosition(ccp(50,200));
    m_pBall2->setTag(BALL_TAG);
    this->addChild(m_pBall2);
    
    //世界
    b2Vec2 gravity = b2Vec2(0.0f,-10.0f);//表示x、y方向的作用力
    bool bSleep = true;
    m_pWorld = new b2World(gravity,bSleep);//用(0,0f,-30.0f)作为这个世界的重力系统。允许物体在不发生碰撞时进入sleep状态，节约系统资源
    
    /**********给这个世界制订四个看不见，碰得着的边界*********/
    b2BodyDef groundBodyDef;//定义
    groundBodyDef.position.Set(0, 0);
    
    b2Body * groundBody = m_pWorld->CreateBody(&groundBodyDef);//用world创建body,会自动进行相关的内存管理
    
    b2PolygonShape groundBox;//多边形形状
    b2FixtureDef     boxShapeDef;//边框形状定义
    
    boxShapeDef.shape = &groundBox;
    
    /***box2d的坐标系与cocos2d一样，都是以左下角为原点。***/
    groundBox.SetAsEdge(b2Vec2(0,0), b2Vec2(screenSize.width/PTM_RATIO,0));//下边界
    groundBody->CreateFixture(&boxShapeDef);
    
    groundBox.SetAsEdge(b2Vec2(0,0), b2Vec2(0,screenSize.height/PTM_RATIO));//左边界
    groundBody->CreateFixture(&boxShapeDef);
    
    groundBox.SetAsEdge(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width/PTM_RATIO,screenSize.height/PTM_RATIO));//上边界
    groundBody->CreateFixture(&boxShapeDef);
    
    groundBox.SetAsEdge(b2Vec2(screenSize.width/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width/PTM_RATIO,0));//右边界
    groundBody->CreateFixture(&boxShapeDef);
    
    
    
    
    
    /********篮球的body和形状***********/
    //body
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;//设置为动态刚体
    ballBodyDef.position.Set(100/PTM_RATIO, 200/PTM_RATIO);
    ballBodyDef.userData = m_pBall;//把篮球精灵赋给userData;
    m_pBody = m_pWorld->CreateBody(&ballBodyDef);
    
    ballBodyDef.position.Set(50/PTM_RATIO, 200/PTM_RATIO);
    ballBodyDef.userData = m_pBall2;//把篮球精灵赋给userData;
    m_pBody2 = m_pWorld->CreateBody(&ballBodyDef);
    
    
    //形状
    b2CircleShape circle;
    circle.m_radius = 26.0/PTM_RATIO;
    
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;//密度
    ballShapeDef.friction = 0.2f;//摩擦力
    ballShapeDef.restitution = 1.0f;//恢复力
    m_pBody->CreateFixture(&ballShapeDef);
    
    ballShapeDef.density = 0.5f;//密度
    m_pBody2->CreateFixture(&ballShapeDef);
    
    
    
    MyContactListener * listener = new MyContactListener;
    m_pWorld->SetContactListener(listener);
    
    schedule(schedule_selector(HelloWorld::tick));
    //    schedule(schedule_selector(HelloWorld::SetFouce));
    
    m_iBallNum = 2;
    
    
    
}



HelloWorld::~HelloWorld()
{
	delete m_pWorld;//会一并删除世界对象里面的物体
	m_pWorld = NULL;
    
    m_pBody = NULL;
	
	//delete m_debugDraw;
}

void HelloWorld::SetFouce()
{
    for(b2Body * body = m_pWorld->GetBodyList();body;body = body->GetNext())
    { 
//        bool bNegative = (arc4random()% 2 != 0);
//
//        b2Vec2 fouce1;
//        fouce1.Set(bNegative*arc4random()%100,arc4random()%100);
//        b2Vec2 position = m_pBody->GetWorldCenter();
//        body->ApplyForce(fouce1, b2Vec2(position.x/PTM_RATIO,position.y/PTM_RATIO));
    }
    
}


void HelloWorld::tick(ccTime dt)
{
    //    CCLOG("%f",dt);
    m_pWorld->Step(dt, 10, 10);//dt为时间步，后面两个参数分别为速度迭代次数和位置迭代次数，一般为8～10。每次查询物体和关节的信息之前，都要先执行这个函数来获得最新信息。dt通常为1/60.
    for(b2Body * b = m_pWorld->GetBodyList();b;b = b->GetNext())
    {
        if(b->GetUserData() != NULL)
        {
            CCSprite * ball = (CCSprite*)b->GetUserData();
            
            //位置
            ball->setPosition(ccp(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
            
            //角度
            ball->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));        }
    }
    
    
    //碰撞检测
    for (b2Contact * contact = m_pWorld->GetContactList(); contact; contact = contact->GetNext())
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        CCSprite* spriteA = (CCSprite*)bodyA->GetUserData();
        CCSprite* spriteB = (CCSprite*)bodyB->GetUserData();
        

        
        if(spriteA != NULL && spriteB != NULL)
        {
            if(spriteA->getTag() == BALL_TAG && spriteB ->getTag() == BALL_TAG)
            {
                CCParticleSystem* pEmitter = CCParticleFire::particleWithFile("bom.plist");
                this->addChild(pEmitter, 10);
                CCParticleSystem* pEmitter2 = CCParticleFire::particleWithFile("bom.plist");
                this->addChild(pEmitter2, 10);
                CCPoint pA = ccp(bodyA->GetWorldCenter().x*PTM_RATIO,bodyA->GetWorldCenter().y*PTM_RATIO);
                CCPoint pB = ccp(bodyB->GetWorldCenter().x*PTM_RATIO,bodyB->GetWorldCenter().y*PTM_RATIO);
                
               
                
                pEmitter->setPosition(ccp(pA.x,pA.y));
                pEmitter2->setPosition(ccp(pB.x,pB.y));
                
                this->removeChild(spriteA, true);
                this->removeChild(spriteB, true);
                m_pWorld->DestroyBody(bodyA);
                m_pWorld->DestroyBody(bodyB);
                bodyA = NULL;
                bodyB = NULL;
                
                m_iBallNum -= 2;
                break;
                
            }
        }
    }
    
    //随机给球施加力
    if(arc4random()%5 == 0)
    {
        this->SetFouce();
    }
    
}



CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::node();
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

#pragma mark - touch
bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return false;
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    

}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCTouch * touch = (CCTouch*)touches->anyObject();
    CCPoint location = touch->locationInView(touch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    this->AddBall(ccp(location.x,location.y));
}

#pragma mark -

bool HelloWorld::AddBall(CCPoint point)
{
    if(m_iBallNum > 6)
    {
        return false;
    }
    else
    {
        m_iBallNum++;
    }
    
    //篮球
    CCSprite * pBall = CCSprite::spriteWithFile("ball.png",CCRect(0,0,52,52));
    pBall->setPosition(point);
    pBall->setTag(BALL_TAG);
    this->addChild(pBall);
    

    /********篮球的body和形状***********/
    //body
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;//设置为动态刚体
    ballBodyDef.position.Set(point.x/PTM_RATIO,point.y/PTM_RATIO);
    ballBodyDef.userData = pBall;//把篮球精灵赋给userData;
    b2Body *  pBody = m_pWorld->CreateBody(&ballBodyDef);
    
   
    
    //形状
    b2CircleShape circle;
    circle.m_radius = 26.0/PTM_RATIO;
    
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;//密度
    ballShapeDef.friction = 0.2f;//摩擦力
    ballShapeDef.restitution = 1.0f;//恢复力
    pBody->CreateFixture(&ballShapeDef);
    
    
    //正负
    b2Vec2 fouce1;
    bool bNegative = (arc4random()% 2 != 0);
    
    //施加一个随机产生的力
    fouce1.Set(bNegative*arc4random()%1000,bNegative*arc4random()%1000);
    b2Vec2 position = pBody->GetWorldCenter();
    pBody->ApplyForce(fouce1, b2Vec2(position.x,position.y));
    
    return true;

}

void HelloWorld::bomb(CCPoint position)
{
    printf("bomb");
}