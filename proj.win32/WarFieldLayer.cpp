#include "WarFieldLayer.h"


bool WarFieldLayer::init()
{
	if (!CCLayer::init())  
	{  
		return false;  
	}
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	
	m_select = -1;
	m_myTurn = true;
	m_movable = true;

	//init 
	showBackgroundBlock();
	
	loadWarriors();

	showWarriors();
	
	return true;
}

WarFieldLayer::~WarFieldLayer()
{

}

void WarFieldLayer::showBackgroundBlock()
{
	CCSpriteBatchNode* bg = CCSpriteBatchNode::create(PIC_FIELD, BLOCK_NUM);
	bg->setAnchorPoint(ccp(0,0));
	bg->setPosition(0,0);
	this->addChild(bg, 0);
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		int row = i / 6;
		int col = i % BLOCK_COL;
		CCSprite* block = CCSprite::create(PIC_FIELD);
		block->setAnchorPoint(ccp(0,0));
		block->setPosition(ccp(BLOCK_UNIT*col, BLOCK_UNIT*row));
		bg->addChild(block);
	}
}

void WarFieldLayer::loadWarriors()
{
	Warrior* warrior = new Warrior(WARRIOR_Caocao, true, 9);
	m_fieldMap[9].m_warrior = warrior;
}

void WarFieldLayer::showWarriors()
{
	
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(m_fieldMap[i].m_warrior)
		{
			addChild(m_fieldMap[i].m_warrior->sprite);
		}
	}
}

void WarFieldLayer::selectWarrior(int pos)
{
	if(m_fieldMap[pos].m_warrior)
	{
		m_fieldMap[pos].m_warrior->sprite->setOpacity(128);
		m_select = pos;
	}
}

void WarFieldLayer::unselectWarrior()
{	
	m_fieldMap[m_select].m_warrior->sprite->setOpacity(255);
	m_select = -1;
}

void WarFieldLayer::moveWarrior(int from, int to)
{
	m_movable = false;
	m_from = from;
	m_to = to;
	
	Position start(m_from%BLOCK_COL,m_from/BLOCK_COL);
	Position end(m_to%BLOCK_COL,m_to/BLOCK_COL);
	vector<Position> blocks;
	blocks.push_back(Position(3,3));
	blocks.push_back(Position(3,4));
	blocks.push_back(Position(4,3));
	blocks.push_back(Position(4,4));
	vector<Position> path;
	if(findPath(start, end, blocks, path))
	{
		size_t size = path.size();
		if(size < 2)
			return;
		
		CCArray* actArray = CCArray::create();
		for(size_t i = 0; i < path.size()-1; i++)
		{
			CCPoint p;
			if(path[size-2-i].x == path[size-1-i].x)
			{
				p = ccp(0,BLOCK_UNIT*(path[size-2-i].y-path[size-1-i].y));
			}
			else if(path[size-2-i].y == path[size-1-i].y)
			{
				p = ccp(BLOCK_UNIT*(path[size-2-i].x-path[size-1-i].x),0);
			}
			CCMoveBy* move = CCMoveBy::create(0.2f, p);
			actArray->addObject(move);
			CCLog("%d, %d", path[size-1-i].x, path[size-1-i].y);
		}
		m_fieldMap[from].m_warrior->sprite->runAction(
			CCSequence::create(CCSequence::create(actArray),
			CCCallFunc::create(this, callfunc_selector(WarFieldLayer::callbackMoveDone)),
			NULL));
	}
	/*
	int x1 = from%BLOCK_COL;
	int y1 = from/BLOCK_COL;
	int x2 = to%BLOCK_COL;
	int y2 = to/BLOCK_COL;
	CCArray* actArray = CCArray::create();
	bool right = x2-x1>0;
	int dx = right? x2-x1 : x1-x2;
	CCPoint px = right? ccp(BLOCK_UNIT,0) : ccp(-BLOCK_UNIT,0);
	bool up = y2-y1>0;
	int dy = up? y2-y1 : y1-y2;
	CCPoint py = up? ccp(0,BLOCK_UNIT) : ccp(0,-BLOCK_UNIT);
	for(int i = 0; i < dx; i++)
	{
		CCMoveBy* move = CCMoveBy::create(0.2f, px);
		actArray->addObject(move);
	}
	for(int i = 0; i < dy; i++)
	{
		CCMoveBy* move = CCMoveBy::create(0.2f, py);
		actArray->addObject(move);
	}
	m_fieldMap[from].m_warrior->sprite->runAction(
		CCSequence::create(CCSequence::create(actArray),
		CCCallFunc::create(this, callfunc_selector(WarFieldLayer::callbackMoveDone)),
		NULL));
		*/
}

void WarFieldLayer::callbackMoveDone()
{
	m_fieldMap[m_to].m_warrior = m_fieldMap[m_from].m_warrior;
	m_fieldMap[m_to].m_warrior->pos = m_to;
	m_fieldMap[m_from].m_warrior = NULL;
	
	m_fieldMap[m_to].m_warrior->sprite->setOpacity(255);
	m_select = -1;
	
	m_movable = true;
}

bool WarFieldLayer::getTouchPos(const CCPoint& pt, int& pos)
{
	int col = (pt.x-s_warfieldStartPoint.x)/BLOCK_UNIT;
	int row = (pt.y-s_warfieldStartPoint.y)/BLOCK_UNIT;
	if(row < 0 || row >= BLOCK_ROW || col < 0 || col > BLOCK_COL)
		return false;
	pos = row*BLOCK_COL + col;
	return true;
	//CCLog("touch pos: %d", pos);
}

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool WarFieldLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if(!m_movable || !m_myTurn)
		return false;
	CCPoint touchPoint = touch->getLocation();
	int pos = 0;
	if(getTouchPos(touchPoint, pos))
	{
		if(m_select>-1)
		{
			if(m_select == pos)
			{
				unselectWarrior();
			}
			else
			{
				moveWarrior(m_select, pos);
			}
			return false;
		}
		else
		{
			selectWarrior(pos);
			return true;
		}
	}
	else
		unselectWarrior();
	return false;
}

void WarFieldLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
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

void WarFieldLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
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

void WarFieldLayer::onEnter()
{   
	//JigsawSound::resumeMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}

void WarFieldLayer::onExit()
{   
	//MySound::stopAll();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}
