#include "PlayLayer.h"
#include "SceneManager.h"
#include "StopLayer.h"
#include "MySound.h"
#include "Basic.h"
#include "WarFieldLayer.h"

//#pragma comment(lib, "pthreadVC2.lib")  //必须加上这句

#include "cocos-ext.h"
USING_NS_CC_EXT;

bool PlayLayer::init()
{	
	if (!CCLayer::init())  
	{  
		return false;  
	}
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	//init 

	//stop button...
	CCMenuItemImage* stopItem = CCMenuItemImage::create(
            PIC_STOPBUTTON,
            PIC_STOPBUTTON_SELECTED,
            this,
            menu_selector(PlayLayer::menuStopCallback));
	stopItem->setPosition(ccp(s.width - 54, s.height - 52));
	// Create a menu with the "close" menu item, it's an auto release object.
	m_stopMenu = CCMenu::create(stopItem, NULL);
	m_stopMenu->setPosition(CCPointZero);
	this->addChild(m_stopMenu, 15);

	WarFieldLayer* warField = WarFieldLayer::create();
	addChild(warField);

	return true;
}

PlayLayer::~PlayLayer()
{

}

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool PlayLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	//if(!m_movable)
	//	return false;
	//if(m_chess.isTurnA() && m_levelA > 0)
	//	return false;
	//else if(!m_chess.isTurnA() && m_levelB > 0)
	//	return false;
	//CCPoint touchPoint = touch->getLocation();
	//int chessPos = 0;
	//if(getTouchChessPos(touchPoint, chessPos))
	//{
	//	if(m_selected)
	//	{
	//		if(chessPos == m_touchChessPos)
	//		{
	//			unselectChessman();
	//		}
	//		else
	//		{
	//			m_moveToChessPos = chessPos;
	//			moveChessman();
	//		}
	//		return false;
	//	}
	//	else
	//	{
	//		selectChessman(chessPos);
	//		return true;
	//	}
	//}
	//else
	//	unselectChessman();
	//return false;
	return true;
}

void PlayLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{   
	////拖动
	//if(m_touchChessPos == -1)
	//	return;
	//CCPoint touchPoint = touch->getLocation();
	//int chessPos = 0;
	//if(getTouchChessPos(touchPoint, chessPos))
	//{
	//	m_moveToChessPos = chessPos;
	//}
	//else
	//{
	//	m_moveToChessPos = -1;
	//}
}

void PlayLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{	
	////拖动
	//if(m_touchChessPos == -1)
	//	return;
	//CCPoint touchPoint = touch->getLocation();
	//int chessPos = 0;
	//if(getTouchChessPos(touchPoint, chessPos))
	//{
	//	m_moveToChessPos = chessPos;
	//	if(m_moveToChessPos == m_touchChessPos)
	//	{
	//		//selectChessman(m_touchChessPos);
	//	}
	//	else
	//	{
	//		moveChessman();
	//	}
	//}
	//else
	//	unselectChessman();
}

void PlayLayer::onEnter()
{   
	//JigsawSound::resumeMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}

void PlayLayer::onExit()
{   
	//MySound::stopAll();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}

void PlayLayer::menuStopCallback(CCObject* pSender)
{
	MySound::playSound(SOUND_MENU);
	StopLayer* stop = StopLayer::create();
	stop->initWithColor(ccc4(0,0,0,125));
	this->addChild(stop, 100);
	setStopVisible(false);
	//SceneManager::goStop(); 
}

void PlayLayer::setStopVisible(bool b)
{
	m_stopMenu->setVisible(b);
}
