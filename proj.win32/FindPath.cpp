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
    Node *currentNode;//��ǰ�ڵ�
    currentNode = sStartNode;
    currentNode->preNode = NULL;//��ʼ�ڵ�ĸ��ڵ�Ϊ��
    currentNode->g = 0;
    SetH(currentNode);
    SetF(currentNode);
    psOpenList.push_front(sStartNode);//�����ʼ�㵽�����б�
    do
    {
        currentNode = GetBestNode();//�ӿ����б�õ����Žڵ�
        //������ǰ�����Χ�Ľ�㲢���뿪���б���ǰ�ڵ�ӿ����б����Ƶ�����б���
        AddToOpenList(currentNode);
 
        psCloseList.push_front(currentNode);//��ӵ��ر��б���
        if(psOpenList.size() < 1 || pcurrNode)
            break; //���Ŀ��ڵ��Ѿ������ڿ����б�򿪷��б��ǿ����˳� 
    } while (true);

	if(!pcurrNode)
		return false;

    //*******************************���Է���Start*******************************//
    //list<Position> path; //���ڴ��Ѱ·�������
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

    //���ģ��ĵ�ͼ��Ѱ·���·��	
    /*for(int j=0;j<10;++j)
    {
        for (int i=0;i<10;++i)
        {
            if(isPath(&path,i,j))
                cout<<"��"<<"/t";
            else
                cout<<"��"<<"��";
        }
        cout<<endl;
    }*/
    //*******************************���Է���End*******************************//
 
}
//*******************************���Է���Start*******************************//
bool AStar::isPath(list<Position> *path, int i, int j)
{
    for(list<Position>::iterator openIterator = path->begin(); openIterator != path->end(); ++openIterator)
    {
        if((*openIterator).x == i && (*openIterator).y == j){return true;}
    }
    return false;
}
//*******************************���Է���End*******************************//
//��ӵ������б�
void AStar::AddToOpenList(Node *src){
    //��ӵ�ǰ�������Χ����(һ��8������)
    GetNodeByPoint(src, src->pt.x, src->pt.y + 1);//��
    GetNodeByPoint(src, src->pt.x, src->pt.y - 1);//��
    GetNodeByPoint(src, src->pt.x - 1, src->pt.y);//��
    GetNodeByPoint(src, src->pt.x + 1, src->pt.y);//��
	/*
    GetNodeByPoint(src, src->pt.x - 1, src->pt.y + 1);//����
    GetNodeByPoint(src, src->pt.x - 1, src->pt.y - 1);//����
    GetNodeByPoint(src, src->pt.x + 1, src->pt.y + 1);//����
    GetNodeByPoint(src, src->pt.x + 1, src->pt.y - 1);//����
	*/
}
void AStar::GetNodeByPoint(Node *pareNode, int _x, int _y)
{
    //��������Ѿ�Խ�������---��������������ļ�
    if(_x < 0 || _y< 0 || _x >=6 || _y>=8)
        return ;
    Node *sNode = new Node();
    //�趨����ֵ
    sNode->pt.x = _x;
    sNode->pt.y = _y;
    //�������������ڷ���б�����赲��Ļ������������
    if(IsClosed(sNode) || IsBlock(sNode))
    {
        return;
    }
    else
    {
        //�趨�����
        sNode->preNode = pareNode;
        SetG(sNode);SetH(sNode);SetF(sNode);//�趨���ֺķ�
        psOpenList.push_front(sNode);//��ӵ������б�
        if(sNode->pt.x == sEndNode->pt.x && sNode->pt.y == sEndNode->pt.y)
            pcurrNode = sNode;//�յ������Ѿ������ڿ����б�
    }
}
//�Ƿ�����ڷ���б�
bool AStar::IsClosed(Node *src)
{
    for(list<Node*>::iterator openIterator = psCloseList.begin(); openIterator != psCloseList.end(); ++openIterator)
    {
        if((*openIterator)->pt.x == src->pt.x && (*openIterator)->pt.y == src->pt.y){return true;}
    }
    return false;
}
//�Ƿ�����ڿ����б�
bool AStar::IsInOpenList(Node *src)
{
    for(list<Node*>::iterator openIterator = psOpenList.begin(); openIterator != psOpenList.end(); ++openIterator)
    {
        if((*openIterator)->pt.x == src->pt.x && (*openIterator)->pt.y == src->pt.y){return true;}
    }
    return false;
}
//�Ƿ����赲����
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
//�����B��(ĳ��)���յ�ĺķ�
void AStar::SetH(Node *node)
{
    node->h = (abs(node->pt.x - sEndNode->pt.x) + abs(node->pt.y - sEndNode->pt.y)) * SPEND_H;
}
//�������㵽B��(ĳ��)�ĺķ�
void AStar::SetG(Node *node)
{
    if(node->pt.x != node->preNode->pt.x && node->pt.y != node->preNode->pt.y)
        node->g = node->preNode->g + SPEND_G_FAR;
    else
        node->g = node->preNode->g + SPEND_G_NEAR;
}
//��������ķ� F = G + H
void AStar::SetF(Node *node)
{
    node->f = node->g + node->h;
}
//�ӿ����б��еõ�����ֵ(Fֵ���)
Node* AStar::GetBestNode()
{
    Node* bNode;
    int bestF = 9999999;
    list<Node*>::iterator iterT;//��¼����ֵ��λ�÷���ɾ��
    for(list<Node*>::iterator openIterator = psOpenList.begin(); openIterator != psOpenList.end(); ++openIterator)
    {
        if(bestF > (*openIterator)->f){
            bestF = (*openIterator)->f; bNode = *openIterator;
            iterT = openIterator;
        }
    }
    if(bNode)
        psOpenList.erase(iterT);//�ҵ�����ֵ��ӿ����б���ɾ��
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