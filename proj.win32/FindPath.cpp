#include "FindPath.h"
#include "cocos2d.h"
USING_NS_CC;

FindPath::FindPath(void)
{
}


FindPath::~FindPath(void)
{
}

AStar::AStar(void)
{
    sStartNode = new Node();
    sEndNode = new Node();
    pcurrNode = NULL;
}
AStar::~AStar(void)
{
    delete sStartNode;
    delete sEndNode;
    delete pcurrNode;
}

void AStar::SetStartNode(const Position& st)
{
	sStartNode->pt.x = st.x;sStartNode->pt.y = st.y;
}

void AStar::SetEndNode(const Position& et)
{
	sEndNode->pt.x = et.x;sEndNode->pt.y = et.y;
}

void AStar::setBlocks(const std::vector<Position>& blocks)
{
	for(size_t i = 0; i < blocks.size(); i++)
		m_blocks.push_back(blocks[i]);
}

bool AStar::Search(std::vector<Position>& path)
{
    Node *currentNode;//当前节点
    currentNode = sStartNode;
    currentNode->preNode = NULL;//初始节点的父节点为空
    currentNode->g = 0;
    SetH(currentNode);
    SetF(currentNode);
    psOpenList.push_front(sStartNode);//添加起始点到开放列表
    do
    {
        currentNode = GetBestNode();//从开放列表得到最优节点
        //遍历当前结点周围的结点并加入开放列表，当前节点从开放列表中移到封闭列表中
        AddToOpenList(currentNode);
 
        psCloseList.push_front(currentNode);//添加到关闭列表中
        if(psOpenList.size() < 1 || pcurrNode)
            break; //如果目标节点已经存在于开放列表或开放列表是空则退出 
    } while (true);

	if(!pcurrNode)
		return false;

    //*******************************测试方法Start*******************************//
    //list<Position> path; //用于存放寻路后的数据
	path.push_back(sEndNode->pt);
    do
    {
        path.push_back(currentNode->pt);
        //CCLog("%d, %d", currentNode->pt.x, currentNode->pt.y);
        if(currentNode->pt.x == sStartNode->pt.x && currentNode->pt.y == sStartNode->pt.y)
            break;
        currentNode = currentNode->preNode;
 
    } while (true);

	return true;

    //输出模拟的地图和寻路后的路径	
    /*for(int j=0;j<10;++j)
    {
        for (int i=0;i<10;++i)
        {
            if(isPath(&path,i,j))
                cout<<"〇"<<"/t";
            else
                cout<<"×"<<"□";
        }
        cout<<endl;
    }*/
    //*******************************测试方法End*******************************//
 
}
//*******************************测试方法Start*******************************//
bool AStar::isPath(list<Position> *path, int i, int j)
{
    for(list<Position>::iterator openIterator = path->begin(); openIterator != path->end(); ++openIterator)
    {
        if((*openIterator).x == i && (*openIterator).y == j){return true;}
    }
    return false;
}
//*******************************测试方法End*******************************//
//添加到开放列表
void AStar::AddToOpenList(Node *src){
    //添加当前坐标的周围坐标(一共8个方向)
    GetNodeByPoint(src, src->pt.x, src->pt.y + 1);//上
    GetNodeByPoint(src, src->pt.x, src->pt.y - 1);//下
    GetNodeByPoint(src, src->pt.x - 1, src->pt.y);//左
    GetNodeByPoint(src, src->pt.x + 1, src->pt.y);//右
	/*
    GetNodeByPoint(src, src->pt.x - 1, src->pt.y + 1);//左上
    GetNodeByPoint(src, src->pt.x - 1, src->pt.y - 1);//左下
    GetNodeByPoint(src, src->pt.x + 1, src->pt.y + 1);//右上
    GetNodeByPoint(src, src->pt.x + 1, src->pt.y - 1);//右下
	*/
}
void AStar::GetNodeByPoint(Node *pareNode, int _x, int _y)
{
    //如果坐标已经越界则不添加---具体参数读配置文件
    if(_x < 0 || _y< 0 || _x >=6 || _y>=8)
        return ;
    Node *sNode = new Node();
    //设定坐标值
    sNode->pt.x = _x;
    sNode->pt.y = _y;
    //如果此坐标存在于封闭列表或是阻挡物的话，不进行添加
    if(IsClosed(sNode) || IsBlock(sNode))
    {
        return;
    }
    else
    {
        //设定父结点
        sNode->preNode = pareNode;
        SetG(sNode);SetH(sNode);SetF(sNode);//设定各种耗费
        psOpenList.push_front(sNode);//添加到开放列表
        if(sNode->pt.x == sEndNode->pt.x && sNode->pt.y == sEndNode->pt.y)
            pcurrNode = sNode;//终点坐标已经存在于开放列表
    }
}
//是否存在于封闭列表
bool AStar::IsClosed(Node *src)
{
    for(list<Node*>::iterator openIterator = psCloseList.begin(); openIterator != psCloseList.end(); ++openIterator)
    {
        if((*openIterator)->pt.x == src->pt.x && (*openIterator)->pt.y == src->pt.y){return true;}
    }
    return false;
}
//是否存在于开放列表
bool AStar::IsInOpenList(Node *src)
{
    for(list<Node*>::iterator openIterator = psOpenList.begin(); openIterator != psOpenList.end(); ++openIterator)
    {
        if((*openIterator)->pt.x == src->pt.x && (*openIterator)->pt.y == src->pt.y){return true;}
    }
    return false;
}
//是否是阻挡坐标
bool AStar::IsBlock(Node *src)
{
	for(size_t i = 0; i < m_blocks.size(); i++)
	{
		if(src->pt.x == m_blocks[i].x && 
			src->pt.y == m_blocks[i].y)
			return true;
	}
    return false;
}
//计算从B点(某点)到终点的耗费
void AStar::SetH(Node *node)
{
    node->h = (abs(node->pt.x - sEndNode->pt.x) + abs(node->pt.y - sEndNode->pt.y)) * SPEND_H;
}
//计算从起点到B点(某点)的耗费
void AStar::SetG(Node *node)
{
    if(node->pt.x != node->preNode->pt.x && node->pt.y != node->preNode->pt.y)
        node->g = node->preNode->g + SPEND_G_FAR;
    else
        node->g = node->preNode->g + SPEND_G_NEAR;
}
//计算总体耗费 F = G + H
void AStar::SetF(Node *node)
{
    node->f = node->g + node->h;
}
//从开放列表中得到最优值(F值最低)
Node* AStar::GetBestNode()
{
    Node* bNode;
    int bestF = 9999999;
    list<Node*>::iterator iterT;//记录最优值的位置方便删除
    for(list<Node*>::iterator openIterator = psOpenList.begin(); openIterator != psOpenList.end(); ++openIterator)
    {
        if(bestF > (*openIterator)->f){
            bestF = (*openIterator)->f; bNode = *openIterator;
            iterT = openIterator;
        }
    }
    if(bNode)
        psOpenList.erase(iterT);//找到最优值后从开放列表中删除
    return bNode;
}

bool findPath(const Position& start, const Position& end,
	const std::vector<Position>& blocks, std::vector<Position>& path)
{
    AStar astar;
	astar.SetStartNode(start);
	astar.SetEndNode(end);
	astar.setBlocks(blocks);
    return astar.Search(path);
}