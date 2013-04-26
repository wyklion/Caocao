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

// 坐标点
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
//结点
struct Node
{
    Position pt;
    int f, g, h;
    struct Node *preNode;//父节点
};
/************************************************************************/
/* A 星算法                                                                     */
/************************************************************************/
class AStar{
public:
    AStar(void);
    ~AStar(void);
public:
    static const int SPEND_H = 10;    //H值耗费
    static const int SPEND_G_FAR = 14;//远的G值耗费
    static const int SPEND_G_NEAR = 10;//近的G值耗费
    list<Node*> psOpenList;             //开放列表
    list<Node*> psCloseList;          //封闭列表
    Node *sStartNode;   //起始节点
    Node *sEndNode;      //终止节点
    Node *pcurrNode;//是否是终点? NOT NULL : NULL
	std::vector<Position> m_blocks;
public:
    void SetStartNode(const Position& st);//设定起始节点
    void SetEndNode(const Position& et);//设定结束节点
	void setBlocks(const std::vector<Position>& blocks);
    bool Search(std::vector<Position>& path);//主搜索
    void AddToOpenList(Node *src);//添加到开放列表
    void GetNodeByPoint(Node *pareNode, int _x, int _y);//通过坐标添加到开放列表
    bool IsClosed(Node *src);//坐标点是否在封闭列表中
    bool IsInOpenList(Node *src);//坐标点是否在开放列表中
    bool IsBlock(Node *src);//坐标点是否是阻挡点
    void SetH(Node *node);//设定H的耗费
    void SetG(Node *node);//设定G的耗费
    void SetF(Node *node);//设定F的耗费
    Node* GetBestNode(); //得到最优的节点(F值最低的节点)
    bool isPath(list<Position> *path, int i, int j);//测试方法-用于输出
};

extern bool findPath(const Position& start, const Position& end,
	const std::vector<Position>& blocks, std::vector<Position>& path);

#endif