#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;

class PlayLayer : public CCLayer  
{  
public:
	~PlayLayer();
	virtual bool init(); //CCLayer�Ķ����Ƕ������������������virtual�ؼ���
	CREATE_FUNC(PlayLayer); //ǧ�����������������ֵĺ�

	//��д������غ���----
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	
public:
	void back (CCObject* pSender);  
	void menuStopCallback(CCObject* pSender);
	void setStopVisible(bool b);

private:
	CCMenu* m_stopMenu;

};

#endif

