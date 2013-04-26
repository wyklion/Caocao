#ifndef __WAR_FIELD_LAYER_H__
#define __WAR_FIELD_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;

#include "Basic.h"
#include "WarriorInfo.h"
#include "FindPath.h"

#define LAYER_FIELD 0
#define LAYER_WARRIOR 10
static const CCPoint s_warfieldStartPoint = ccp(2,50);

class Warrior
{
public:
	Warrior(WarriorType type, bool isMine, int pos)
		:type(type), isMine(isMine), pos(pos)
	{
		sprite = CCSprite::create(s_warriorInfo[type].picName.c_str());
		int row = pos/BLOCK_COL;
		int col = pos%BLOCK_COL;
		sprite->setPosition(ccp(BLOCK_UNIT*col+BLOCK_UNIT/2, BLOCK_UNIT*row+BLOCK_UNIT/2));
		sprite->retain();
	}
	~Warrior()
	{
		sprite->release();
	}
public:
	WarriorType type;
	bool isMine;
	int pos;
public:
	CCSprite* sprite;
};

class FieldMap
{
public:
	FieldMap()
		:m_warrior(NULL)
	{
	}
	~FieldMap()
	{
		if(m_warrior)
			delete m_warrior;
	}

public:
	Warrior* m_warrior;
};


class WarFieldLayer : public CCLayer  
{  
public:
	~WarFieldLayer();
	virtual bool init(); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字
	CREATE_FUNC(WarFieldLayer); //千万别忘了这个风情万种的宏

	//重写触屏相关函数----
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	
public:
	CCPoint getPosPoint(int pos)
	{ 
		return ccp(pos%BLOCK_COL * BLOCK_UNIT + BLOCK_UNIT/2,
				   pos/BLOCK_COL * BLOCK_UNIT + BLOCK_UNIT/2);
	}

private:
	void loadWarriors();
	void showBackgroundBlock();
	void showWarriors();

	bool getTouchPos(const CCPoint& pt, int& pos);
	void selectWarrior(int pos);
	void unselectWarrior();
	void moveWarrior(int from, int to);
	void callbackMoveDone();

private:
	bool m_movable;
	FieldMap m_fieldMap[BLOCK_NUM];
	int m_select;
	int m_from;
	int m_to;
	bool m_myTurn;
	std::set<int> m_blocks;

	std::set<int> m_myWarriors;
	std::set<int> m_otherWarriors;
};

#endif