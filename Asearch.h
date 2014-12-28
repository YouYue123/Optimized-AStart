#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include<algorithm>
#define seperateX 120
#define seperateY 120
#define areaRoseNumber 7
#define areaChinaNumber 13
using namespace std;
static int destRoseNumber = 5;
static int destChinaNumber = 8;
struct pathNode//A*路径节点 F越小优先级越小
{
	int netX,netY;
	struct pathNode *parent;
	double F;
	double G;
	double H;
  friend bool operator< (pathNode a, pathNode b)
   {
	  return a.F > b.F;
   }
};
struct mapPoint
{
	int x;
	int y;
};
struct Area//可行走区域结构
{
	mapPoint point[4];
	int number;
	map <int,mapPoint> branch;
};
class SearchRoad
{
public: 
	     SearchRoad(int situation);
		 ~SearchRoad();
		 vector<struct mapPoint> drawRoad(mapPoint start,mapPoint end);
		 vector<struct mapPoint> updateRoad(mapPoint now,mapPoint end);

		 vector<struct mapPoint> imDest;//存放目的地信息
private:
	 
	 bool PtInAnyRect(mapPoint pCur, mapPoint pLT, mapPoint pLB, mapPoint pRB, mapPoint pRT ) ;
	 void aSearch(mapPoint start,mapPoint end);
	 void InitRosePoint();
	 void InitChinaPoint();
	 int judgeArea(mapPoint now);
     int isOnTheWay(int nowArea,vector<mapPoint> result);
     inline int crossProduct(mapPoint A,mapPoint B,mapPoint M);
	 
	 bool naviMap[seperateX][seperateY];//地图信息
    
	 priority_queue<struct pathNode> openList;//A*所用到的开启列表
     list<struct pathNode> backOpenList;//由于优先队列不可遍历，这里设置一个开启列表的list形式
     list<struct pathNode> closeList;//关闭列表
     vector<struct mapPoint> path;//存放A*返回的路径
     vector<int> pathArea;//路径所穿过的区域
     vector<struct Area> area;//可行走区域容器

	 int areaAccount;
	 vector<mapPoint> resultPath;//最终的路径信息
};