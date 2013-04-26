#ifndef __FIND_PATH_H__
#define __FIND_PATH_H__

class FindPath
{
public:
	FindPath(void);
	~FindPath(void);
public:
	
};

#include <list>
#include <math.h>
#include <vector>

using namespace std;

// �����
struct Position
{
    int x;
    int y;
    int isblock;
    Position()
    {
        x = 0;
        y = 0;
    }
    Position(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};
//���
struct Node
{
    Position pt;
    int f, g, h;
    struct Node *preNode;//���ڵ�
};
/************************************************************************/
/* A ���㷨                                                                     */
/************************************************************************/
class AStar{
public:
    AStar(void);
    ~AStar(void);
public:
    static const int SPEND_H = 10;    //Hֵ�ķ�
    static const int SPEND_G_FAR = 14;//Զ��Gֵ�ķ�
    static const int SPEND_G_NEAR = 10;//����Gֵ�ķ�
    list<Node*> psOpenList;             //�����б�
    list<Node*> psCloseList;          //����б�
    Node *sStartNode;   //��ʼ�ڵ�
    Node *sEndNode;      //��ֹ�ڵ�
    Node *pcurrNode;//�Ƿ����յ�? NOT NULL : NULL
	std::vector<Position> m_blocks;
public:
    void SetStartNode(const Position& st);//�趨��ʼ�ڵ�
    void SetEndNode(const Position& et);//�趨�����ڵ�
	void setBlocks(const std::vector<Position>& blocks);
    bool Search(std::vector<Position>& path);//������
    void AddToOpenList(Node *src);//��ӵ������б�
    void GetNodeByPoint(Node *pareNode, int _x, int _y);//ͨ��������ӵ������б�
    bool IsClosed(Node *src);//������Ƿ��ڷ���б���
    bool IsInOpenList(Node *src);//������Ƿ��ڿ����б���
    bool IsBlock(Node *src);//������Ƿ����赲��
    void SetH(Node *node);//�趨H�ĺķ�
    void SetG(Node *node);//�趨G�ĺķ�
    void SetF(Node *node);//�趨F�ĺķ�
    Node* GetBestNode(); //�õ����ŵĽڵ�(Fֵ��͵Ľڵ�)
    bool isPath(list<Position> *path, int i, int j);//���Է���-�������
};

extern bool findPath(const Position& start, const Position& end,
	const std::vector<Position>& blocks, std::vector<Position>& path);

#endif