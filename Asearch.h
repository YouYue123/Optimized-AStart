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
#define areaNumber 7
using namespace std;
static int areaAccount = areaNumber;
static int destNumber = 5;//目的地个数
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
struct Point
{
	int x;
	int y;
};
struct Area//可行走区域结构
{
	Point point[4];
	int number;
	map <int,Point> branch;
};
class SearchRoad
{
public: 
	     SearchRoad();
		 void InitPoint();
		 void drawRoad(IplImage *image,Point start,Point end);
		 vector<struct Point>imDest;//存放目的地信息
private:
	 
	 bool PtInAnyRect(Point pCur, Point pLT, Point pLB, Point pRB, Point pRT ) ;
	 void aSearch(Point start,Point end);
	 

     bool naviMap[seperateX][seperateY];//地图信息
     priority_queue<struct pathNode> openList;//A*所用到的开启列表
     list<struct pathNode> backOpenList;//由于优先队列不可遍历，这里设置一个开启列表的list形式
     list<struct pathNode> closeList;//关闭列表
     vector<struct Point> path;//存放A*返回的路径
     vector<struct Area> area;//可行走区域容器
     vector<int> pathArea;//路径所穿过的区域
};