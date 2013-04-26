#ifndef __WARRIOR_INFO_H__
#define __WARRIOR_INFO_H__


enum WarriorType
{
	WARRIOR_Caocao,
	WARRIOR_Liubei,
	WARRIOR_Guanyu,
	WARRIOR_Zhangfei,

	WARRIOR_NUM
};

struct WarriorInfo
{
	std::string picName;
};

static const WarriorInfo s_warriorInfo[WARRIOR_NUM] = 
{
	{"warrior_caocao.png"}
};



#endif