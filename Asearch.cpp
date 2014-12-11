#include"Asearch.h"
SearchRoad::SearchRoad()
{
	
}
/* 函数名:aSearch
   函数功能:寻找最短路径
   参数：
        Point start 起始点 
 		Point end 结束点
        路径结果存在vector<Point>path中
*/
void SearchRoad::aSearch(Point start,Point end)
{
	bool findDest = false;
	int i,j;
	int startNetX = start.x/10;
	int startNetY = start.y/10;
	int endNetX = end.x/10;
	int endNetY = end.y/10;
	/*把初始点添加进开启列表*/ 
	pathNode startNode;
	
    startNode.parent = NULL;
	startNode.G = 0;
	startNode.H = abs(startNetX - endNetX) + abs(startNetY - endNetY);
	startNode.F = startNode.G + startNode.H;
	startNode.netX = start.x/10;
	startNode.netY = start.y/10;
	
    openList.push(startNode);
	backOpenList.push_back(startNode);

	while(!openList.empty())
	{
		pathNode tempNode = openList.top();
		openList.pop();//取出开启列表中F值最小的点 
        
		closeList.push_back(tempNode);//把这个点加入到关闭列表 
		
		if(tempNode.netX == endNetX && tempNode.netY == endNetY)
		{ 
			Point tempPath;
		    printf("找到终点\n");
			findDest = true;
		    pathNode terminal = tempNode;
            printf("%d %d\n",terminal.netX,terminal.netY);
			tempPath.x = terminal.netX*10+4;
			tempPath.y = terminal.netY*10+4;
			path.push_back(tempPath);
            while(terminal.parent!=NULL)
            {
                terminal = *terminal.parent;
                printf("%d %d\n",terminal.netX,terminal.netY);
				tempPath.x = terminal.netX*10+4;
			    tempPath.y = terminal.netY*10+4;
			    path.push_back(tempPath);
            } 
            break;//找到终点则跳出 
		} 
        printf("\n现在点是%d %d\n",tempNode.netX,tempNode.netY);
	    for(i=-1;i<2;i++)
			for(j=-1;j<2;j++)//处理邻接点
			{
                
				if(tempNode.netX+i >=0 && tempNode.netX +i <=119 && tempNode.netY + j >=0 && tempNode.netY+j <=119 
					&&naviMap[tempNode.netX+i][tempNode.netY+j])//可移动的方格
				{
                    printf("\n可移动方格 %d %d\n",tempNode.netX+i,tempNode.netY+j); 
					bool check = false;
					for(list<pathNode>::iterator iter = closeList.begin();iter!=closeList.end();iter++)//查看关闭列表是否含有
					{
						if(iter->netX == tempNode.netX+i && iter->netY == tempNode.netY+j)
						{
							check = true;
							printf("该点在关闭列表中，跳过\n");
							break;
						}
					}
					if(!check)//如果在关闭列表中没有检查到
					{
                         printf("该点不在关闭列表中\n");
						 bool findOpen = false;
						 pathNode findNode;
                          //查看开启列表
						 for(list<pathNode>::iterator iter = backOpenList.begin();iter!=backOpenList.end();iter++)
						 {
							 if(iter->netX == tempNode.netX+i && iter->netY == tempNode.netY+j)
							 {
								 findNode = *iter;
								 findOpen = true;
							     printf("该点在开启列表中\n");
								 break;
							 }
						 }
						  //如果开启列表中没有，更新F 并且设定好parent 加入开启列表
						  if(findOpen == false)
						  {
                              printf("该点不在开启列表中\n");
							  struct pathNode node;
							  node.parent = &closeList.back();
							  node.netX = tempNode.netX + i ;
							  node.netY = tempNode.netY + j ;
						  
							  if(abs(i)+abs(j) == 2)
							  node.G = node.parent->G + 1.414;
							  else
							  node.G = node.parent->G + 1;
                                
							  node.H = abs(node.netX - endNetX) + abs(node.netY - endNetY);

							  node.F = node.G + node.H;
							  printf("新加入的点%d %d:\nF:%f G:%f H: %f\n",node.netX,node.netY,node.F,node.G,node.H);
							  
							  openList.push(node);
							  backOpenList.push_back(node);
						  }
						  else//如果开启列表中存在
						  {
							  double G;
							  if(abs(i)+abs(j) == 2)
								  G = tempNode.G +1.414;
							  else
								  G= tempNode.G +1;
							  if(G < findNode.G)
							  {
                                  printf("从新路径走比较合适，更新\n");
								  findNode.parent = &closeList.back();
								  findNode.G = G;
								  findNode.F = G+ findNode.H;
							  }
						  }
					}
				}
			}	
	}
	if(!findDest)
		printf("终点不可达");
	
}
/*
   函数名: PtInAnyRect
   功能：判断点是否在四边形内部
   参数：
      Point pCur 指定的当前点 
	  Point pLT, Point pLB, Point pRB, Point pRT, 四边形的四个点
*/
bool SearchRoad::PtInAnyRect(Point pCur, Point pLT, Point pLB, Point pRB, Point pRT ) 
{ 
		//任意四边形有4个顶点
		int nCount = 4;
		Point RectPoints[4] = { pLT, pLB, pRB, pRT };
		int nCross = 0;
		for (int i = 0; i < nCount; i++) 
		{ 
			//依次取相邻的两个点
			Point pStart = RectPoints[i]; 
			Point pEnd = RectPoints[(i + 1) % nCount];

			//相邻的两个点是平行于x轴的，肯定不相交，忽略
			if ( pStart.y == pEnd.y ) 
				continue;

			//交点在pStart,pEnd的延长线上，pCur肯定不会与pStart.pEnd相交，忽略
			if ( pCur.y < min(pStart.y, pEnd.y) || pCur.y > max( pStart.y, pEnd.y ) ) 
				continue;
 
			//求当前点和x轴的平行线与pStart,pEnd直线的交点的x坐标
			double x = (double)(pCur.y - pStart.y) * (double)(pEnd.x - pStart.x) / (double)(pEnd.y - pStart.y) + pStart.x;

			//若x坐标大于当前点的坐标，则有交点
			if ( x > pCur.x ) 
				nCross++; 
		}

		// 单边交点为偶数，点在多边形之外 
		return (nCross % 2 == 1); 
	}
/*
   函数名:InitPoint()
   功能:根据人工标注节点初始化地图和区域
   参数:无
 */
void SearchRoad::InitPoint()
{   
	int i,j,k;
     /*人工标注节点**/
	for(i=0;i<destNumber;i++)
	{
		struct Point tempPoint;
		tempPoint.x = 0;
		tempPoint.y = 1;
		imDest.push_back(tempPoint);
	}

	imDest[0].x = 564;  imDest[0].y = 524;//正门
	imDest[1].x = 474;  imDest[1].y =534;//门口休息区
	imDest[2].x =534;  imDest[2].y = 585;//问讯处
	imDest[3].x = 902; imDest[3].y = 891;//打水处
	imDest[4].x = 934; imDest[4].y = 845;//Project Room
	
	//标注可以行走的四方形区域
	for(i=0;i<areaNumber;i++)
	{
		struct Area tempArea;
		area.push_back(tempArea);		
	}
    
	Point tempBranch;
	area[0].point[0].x = 446; area[0].point[0].y = 516;
	area[0].point[1].x = 534; area[0].point[1].y = 511;
	area[0].point[2].x = 554; area[0].point[2].y = 591;
	area[0].point[3].x = 480; area[0].point[3].y = 608;
	area[0].number = 0;
	tempBranch.x = 544; tempBranch.y = 554;
	area[0].branch.insert(pair<int,Point>(1,tempBranch));

	area[1].point[0].x = 534; area[1].point[0].y = 511;
	area[1].point[1].x = 587; area[1].point[1].y = 504;
	area[1].point[2].x = 676; area[1].point[2].y = 825;
	area[1].point[3].x = 621; area[1].point[3].y = 837;
	area[1].number = 1;
	tempBranch.x = 554; tempBranch.y = 554;
	area[1].branch.insert(pair<int,Point>(0,tempBranch));
	tempBranch.x = 564; tempBranch.y = 514;
	area[1].branch.insert(pair<int,Point>(4,tempBranch));
    tempBranch.x = 654; tempBranch.y = 754;
	area[1].branch.insert(pair<int,Point>(2,tempBranch));

	area[2].point[0].x = 652; area[2].point[0].y = 737;
	area[2].point[1].x = 859; area[2].point[1].y = 675;
	area[2].point[2].x = 871; area[2].point[2].y = 720;
	area[2].point[3].x = 662; area[2].point[3].y = 776;
	area[2].number = 2;
	tempBranch.x = 664; tempBranch.y = 754;
	area[2].branch.insert(pair<int,Point>(1,tempBranch));
	tempBranch.x = 864; tempBranch.y = 694;
	area[2].branch.insert(pair<int,Point>(3,tempBranch));

    area[3].point[0].x = 842; area[3].point[0].y = 615;
	area[3].point[1].x = 875; area[3].point[1].y = 619;
	area[3].point[2].x = 969; area[3].point[2].y = 966;
	area[3].point[3].x = 937; area[3].point[3].y = 976;
	area[3].number = 3;
	tempBranch.x = 874; tempBranch.y =694;
	area[3].branch.insert(pair<int,Point>(2,tempBranch));
	tempBranch.x = 914; tempBranch.y = 874;
	area[3].branch.insert(pair<int,Point>(6,tempBranch));

	area[4].point[0].x = 527; area[4].point[0].y = 456;
	area[4].point[1].x = 580; area[4].point[1].y = 455;
	area[4].point[2].x = 587; area[4].point[2].y = 503;
	area[4].point[3].x = 554; area[4].point[3].y = 507;
	area[4].number = 4;
	tempBranch.x = 565; tempBranch.y = 503;
	area[4].branch.insert(pair<int,Point>(1,tempBranch));
	tempBranch.x = 582; tempBranch.y = 483;
	area[4].branch.insert(pair<int,Point>(5,tempBranch));
	
    area[5].point[0].x = 585; area[5].point[0].y = 475;
	area[5].point[1].x = 620; area[5].point[1].y = 471;
	area[5].point[2].x = 638; area[5].point[2].y = 533;
	area[5].point[3].x = 613; area[5].point[3].y = 534;
	area[5].number = 5;
	tempBranch.x = 593; tempBranch.y = 484;
	area[5].branch.insert(pair<int,Point>(4,tempBranch));
	
	area[6].point[0].x = 907; area[6].point[0].y = 864;
	area[6].point[1].x = 915; area[6].point[1].y = 893;
	area[6].point[2].x = 881; area[6].point[2].y = 901;
	area[6].point[3].x = 874; area[6].point[3].y = 875;
	area[6].number = 6;
	tempBranch.x = 905; tempBranch.y = 875;
	area[6].branch.insert(pair<int,Point>(3,tempBranch));

	//根据区域将地图初始化
	for(i=0;i<120;i++)
		for(j=0;j<120;j++)
		{
			naviMap[i][j] = false;
			struct Point center;
			center.x = i*10 + 4;
			center.y = j*10 + 4;
		    for(k=0;k<areaAccount;k++)
			{
				if(PtInAnyRect(center,area[k].point[0],area[k].point[1],area[k].point[2],area[k].point[3]))
                    {
						naviMap[i][j] = true;
						break;
				    }
			}
		}
	 
}
/*
   函数名:drawRoad
   功能：在地图上画出路径
   参数:IplImage *image 要绘制的图片
        Point start 起始点
		Point end  结束点
*/
void SearchRoad::drawRoad(IplImage *image,Point start,Point end)
{
    int i,j,startArea,endArea;
	//绘制可行走区域
	for(i=0;i<areaAccount;i++)
	{
	 cvLine(image,cvPoint(area[i].point[0].x,area[i].point[0].y),cvPoint(area[i].point[1].x,area[i].point[1].y),CV_RGB(0, 0, 255));
	 cvLine(image,cvPoint(area[i].point[1].x,area[i].point[1].y),cvPoint(area[i].point[2].x,area[i].point[2].y),CV_RGB(0, 0, 255));
	 cvLine(image,cvPoint(area[i].point[2].x,area[i].point[2].y),cvPoint(area[i].point[3].x,area[i].point[3].y),CV_RGB(0, 0, 255));
	 cvLine(image,cvPoint(area[i].point[3].x,area[i].point[3].y),cvPoint(area[i].point[0].x,area[i].point[0].y),CV_RGB(0, 0, 255));
	}
	//绘制区域内节点
	for(i=0;i<120;i++)
		for(j=0;j<120;j++)
		{
			if(naviMap[i][j]==true)
			{
				CvPoint Point;
				Point.x = i*10 + 4;
				Point.y = j*10 + 4;
				cvCircle(image,Point, 2,CV_RGB(255, 0, 0));
			}
		}
		aSearch(start,end);
		for(vector<Point>::iterator iter = path.begin();iter!=path.end();iter++)
		{
			bool repeat = false;
			for(i=0;i<areaAccount;i++)
			{
			   
               if(PtInAnyRect(*iter,area[i].point[0],area[i].point[1],area[i].point[2],area[i].point[3]))//判断区域
		    	{
					for(vector<int>::iterator iter1 = pathArea.begin();iter1!=pathArea.end();iter1++)//查是否重复
					{
						if(*iter1 == i)
						{
							repeat = true;
							cout<<*iter1;
							break;
						}
					}
					if(!repeat)//填入唯一元素
					pathArea.push_back(i);//区域信息压入向量
					break;
					
			    }

			}
		}
		//判断起始和结束点分别所在的区域
		for(i=0;i<areaAccount;i++)
		{
			if(PtInAnyRect(start,area[i].point[0],area[i].point[1],area[i].point[2],area[i].point[3]))//判断区域
			{
				startArea = i;
			}
			if(PtInAnyRect(end,area[i].point[0],area[i].point[1],area[i].point[2],area[i].point[3]))
			{
				endArea = i;
			}
		}
		if(startArea == endArea)//开始和结束点在一个区域
		{
		  cvLine(image,cvPoint(start.x,start.y),cvPoint(end.x,end.y),CV_RGB(0, 0, 255));
		}
		else//在不同的区域
		{
			
			cvLine(image,cvPoint(start.x,start.y),cvPoint(area[startArea].branch[pathArea[pathArea.size()-2]].x,area[startArea].branch[pathArea[pathArea.size()-2]].y),CV_RGB(0, 0, 255));//连接初始点与其本区域与下个区域的连接点
			cvLine(image,cvPoint(end.x,end.y),cvPoint(area[endArea].branch[pathArea[1]].x,area[endArea].branch[pathArea[1]].y),CV_RGB(0, 0, 255));//连接终点和其区域之前的连接点
			
			for(vector<int>::iterator iter = pathArea.begin()+1;iter!=pathArea.end();iter++)
			{
			
				cvLine(image,cvPoint(area[*(iter-1)].branch[*iter].x,area[*(iter-1)].branch[*iter].y),
					         cvPoint(area[*iter].branch[*(iter-1)].x,area[*iter].branch[*(iter-1)].y),
					   CV_RGB(0, 0, 255));//连上个区域与本区域的连接和本区域与上个区域的连接
				if(iter != pathArea.end()-1)
				cvLine(image,cvPoint(area[*iter].branch[*(iter+1)].x,area[*iter].branch[*(iter+1)].y),
					         cvPoint(area[*iter].branch[*(iter-1)].x,area[*iter].branch[*(iter-1)].y),
					   CV_RGB(0, 0, 255));//连本区域与上个区域的连接和本区域与下个区域的连接
			} 
		}
}