#include"Asearch.h"
SearchRoad::SearchRoad(int situation)
{
  switch(situation)
  {	
case 0:
	areaAccount = areaRoseNumber;
	InitRosePoint();
	break;
case 1:
	areaAccount = areaChinaNumber;
	InitChinaPoint();
	break;
  }
}
SearchRoad::~SearchRoad()
{
}
/* 
   函数名:aSearch
   函数功能:寻找最短路径
   参数：
        mapPoint start 起始点 
 		mapPoint end 结束点
        路径结果存在vector<mapPoint>path中
*/
void SearchRoad::aSearch(mapPoint start,mapPoint end)
{
	while(openList.size()!=0)
		openList.pop();
	backOpenList.clear();
	closeList.clear();
	
	path.clear();
	pathArea.clear();

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
			mapPoint tempPath;
		    //printf("找到终点\n");
			findDest = true;
		    pathNode terminal = tempNode;
            //printf("%d %d\n",terminal.netX,terminal.netY);
			tempPath.x = terminal.netX*10+4;
			tempPath.y = terminal.netY*10+4;
			path.push_back(tempPath);
            while(terminal.parent!=NULL)
            {
                terminal = *terminal.parent;
                //printf("%d %d\n",terminal.netX,terminal.netY);
				tempPath.x = terminal.netX*10+4;
			    tempPath.y = terminal.netY*10+4;
			    path.push_back(tempPath);
            } 
            break;//找到终点则跳出 
		} 
        //printf("\n现在点是%d %d\n",tempNode.netX,tempNode.netY);
	    for(i=-1;i<2;i++)
			for(j=-1;j<2;j++)//处理邻接点
			{
                
				if(tempNode.netX+i >=0 && tempNode.netX +i <=119 && tempNode.netY + j >=0 && tempNode.netY+j <=119 
					&&naviMap[tempNode.netX+i][tempNode.netY+j])//可移动的方格
				{
                    //printf("\n可移动方格 %d %d\n",tempNode.netX+i,tempNode.netY+j); 
					bool check = false;
					for(list<pathNode>::iterator iter = closeList.begin();iter!=closeList.end();iter++)//查看关闭列表是否含有
					{
						if(iter->netX == tempNode.netX+i && iter->netY == tempNode.netY+j)
						{
							check = true;
							//printf("该点在关闭列表中，跳过\n");
							break;
						}
					}
					if(!check)//如果在关闭列表中没有检查到
					{
                         //printf("该点不在关闭列表中\n");
						 bool findOpen = false;
						 pathNode findNode;
                          //查看开启列表
						 for(list<pathNode>::iterator iter = backOpenList.begin();iter!=backOpenList.end();iter++)
						 {
							 if(iter->netX == tempNode.netX+i && iter->netY == tempNode.netY+j)
							 {
								 findNode = *iter;
								 findOpen = true;
							     //printf("该点在开启列表中\n");
								 break;
							 }
						 }
						  //如果开启列表中没有，更新F 并且设定好parent 加入开启列表
						  if(findOpen == false)
						  {
                              //printf("该点不在开启列表中\n");
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
							  //printf("新加入的点%d %d:\nF:%f G:%f H: %f\n",node.netX,node.netY,node.F,node.G,node.H);
							  
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
                                  //printf("从新路径走比较合适，更新\n");
								  findNode.parent = &closeList.back();
								  findNode.G = G;
								  findNode.F = G+ findNode.H;
							  }
						  }
					}
				}
			}	
	}
	//if(!findDest)
		//printf("终点不可达");
	
}
/*
   函数名: PtInAnyRect
   功能：判断点是否在四边形内部
   参数：
      mapPoint pCur 指定的当前点 
	  mapPoint pLT, mapPoint pLB, mapPoint pRB, mapPoint pRT, 四边形的四个点
*/
bool SearchRoad::PtInAnyRect(mapPoint pCur, mapPoint pLT, mapPoint pLB, mapPoint pRB, mapPoint pRT ) 
{ 
		//任意四边形有4个顶点
		int nCount = 4;
		mapPoint RectPoints[4] = { pLT, pLB, pRB, pRT };
		int nCross = 0;
		for (int i = 0; i < nCount; i++) 
		{ 
			//依次取相邻的两个点
			mapPoint pStart = RectPoints[i]; 
			mapPoint pEnd = RectPoints[(i + 1) % nCount];
			/*
			//相邻的两个点是平行于x轴的，肯定不相交，忽略
			if ( pStart.y == pEnd.y ) 
				continue;

			//交点在pStart,pEnd的延长线上，pCur肯定不会与pStart.pEnd相交，忽略
			if ( pCur.y < min(pStart.y, pEnd.y) || pCur.y > max( pStart.y, pEnd.y )) 
				continue;

			//求当前点和x轴的平行线与pStart,pEnd直线的交点的x坐标
			double x = (double)(pCur.y - pStart.y) * (double)(pEnd.x - pStart.x) / (double)(pEnd.y - pStart.y) + pStart.x;

			//若x坐标大于当前点的坐标，则有交点
			if ( x > pCur.x ) 
				nCross++;
				*/
			if(crossProduct(pStart,pEnd,pCur) <= 0 )
				return false;
		}

		// 单边交点为偶数，点在多边形之外 
		//return (nCross % 2 == 1);
		        return true;
	}
/*
   函数名:InitPoint()
   功能:根据人工标注节点初始化地图和区域
   参数:无
 */
void SearchRoad::InitRosePoint()
{   
	int i,j,k;
     /*人工标注节点**/
	imDest.clear();
	for(i=0;i<destRoseNumber;i++)
	{
		struct mapPoint tempPoint;
		tempPoint.x = 0;
		tempPoint.y = 1;
		imDest.push_back(tempPoint);
	}

	imDest[0].x = 564;  imDest[0].y = 524;//正门
	imDest[1].x = 474;  imDest[1].y =534;//门口休息区
	imDest[2].x =534;  imDest[2].y = 585;//问讯处
	imDest[3].x = 902; imDest[3].y = 891;//打水处
	imDest[4].x = 934; imDest[4].y = 845;//Project Room
	
	area.clear();
	//标注可以行走的四方形区域
	for(i=0;i<areaRoseNumber;i++)
	{
		struct Area tempArea;
		area.push_back(tempArea);		
	}
    
	mapPoint tempBranch;
	area[0].point[0].x = 446; area[0].point[0].y = 516;
	area[0].point[1].x = 534; area[0].point[1].y = 511;
	area[0].point[2].x = 554; area[0].point[2].y = 591;
	area[0].point[3].x = 480; area[0].point[3].y = 608;
	area[0].number = 0;
	tempBranch.x = 544; tempBranch.y = 554;
	area[0].branch.insert(pair<int,mapPoint>(1,tempBranch));

	area[1].point[0].x = 534; area[1].point[0].y = 511;
	area[1].point[1].x = 587; area[1].point[1].y = 504;
	area[1].point[2].x = 676; area[1].point[2].y = 825;
	area[1].point[3].x = 621; area[1].point[3].y = 837;
	area[1].number = 1;
	tempBranch.x = 554; tempBranch.y = 554;
	area[1].branch.insert(pair<int,mapPoint>(0,tempBranch));
	tempBranch.x = 564; tempBranch.y = 514;
	area[1].branch.insert(pair<int,mapPoint>(4,tempBranch));
    tempBranch.x = 654; tempBranch.y = 754;
	area[1].branch.insert(pair<int,mapPoint>(2,tempBranch));

	area[2].point[0].x = 648; area[2].point[0].y = 721;
	area[2].point[1].x = 856; area[2].point[1].y = 659;
	area[2].point[2].x = 871; area[2].point[2].y = 720;
	area[2].point[3].x = 662; area[2].point[3].y = 776;
	area[2].number = 2;
	tempBranch.x = 664; tempBranch.y = 754;
	area[2].branch.insert(pair<int,mapPoint>(1,tempBranch));
	tempBranch.x = 864; tempBranch.y = 694;
	area[2].branch.insert(pair<int,mapPoint>(3,tempBranch));

    area[3].point[0].x = 842; area[3].point[0].y = 615;
	area[3].point[1].x = 875; area[3].point[1].y = 619;
	area[3].point[2].x = 969; area[3].point[2].y = 966;
	area[3].point[3].x = 937; area[3].point[3].y = 976;
	area[3].number = 3;
	tempBranch.x = 874; tempBranch.y =694;
	area[3].branch.insert(pair<int,mapPoint>(2,tempBranch));
	tempBranch.x = 914; tempBranch.y = 874;
	area[3].branch.insert(pair<int,mapPoint>(6,tempBranch));

	area[4].point[0].x = 527; area[4].point[0].y = 456;
	area[4].point[1].x = 578; area[4].point[1].y = 455;
	area[4].point[2].x = 586; area[4].point[2].y = 503;
	area[4].point[3].x = 536; area[4].point[3].y = 513;
	area[4].number = 4;
	tempBranch.x = 565; tempBranch.y = 503;
	area[4].branch.insert(pair<int,mapPoint>(1,tempBranch));
	tempBranch.x = 582; tempBranch.y = 483;
	area[4].branch.insert(pair<int,mapPoint>(5,tempBranch));
	
    area[5].point[0].x = 585; area[5].point[0].y = 475;
	area[5].point[1].x = 620; area[5].point[1].y = 471;
	area[5].point[2].x = 638; area[5].point[2].y = 533;
	area[5].point[3].x = 613; area[5].point[3].y = 534;
	area[5].number = 5;
	tempBranch.x = 593; tempBranch.y = 484;
	area[5].branch.insert(pair<int,mapPoint>(4,tempBranch));
	
	area[6].point[0].x = 907; area[6].point[0].y = 864;
	area[6].point[1].x = 915; area[6].point[1].y = 893;
	area[6].point[2].x = 881; area[6].point[2].y = 901;
	area[6].point[3].x = 874; area[6].point[3].y = 875;
	area[6].number = 6;
	tempBranch.x = 905; tempBranch.y = 875;
	area[6].branch.insert(pair<int,mapPoint>(3,tempBranch));

	//根据区域将地图初始化
	for(i=0;i<120;i++)
		for(j=0;j<120;j++)
		{
			naviMap[i][j] = false;
			struct mapPoint center;
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
   函数名:InitChinaPoint()
   功能:根据人工标注节点初始化地图和区域
   参数:无
 */
void SearchRoad::InitChinaPoint()
{   
	int i,j,k;
     /*人工标注节点**/
	imDest.clear();
	for(i=0;i<destChinaNumber;i++)
	{
		struct mapPoint tempPoint;
		tempPoint.x = 0;
		tempPoint.y = 1;
		imDest.push_back(tempPoint);
	}

	imDest[0].x = 259;  imDest[0].y = 817;//正门
	imDest[1].x = 277;  imDest[1].y = 779;//gallery
	imDest[2].x = 315;  imDest[2].y = 417;//other chinese
	imDest[3].x = 281; imDest[3].y = 297;//generation 1
	imDest[4].x = 299; imDest[4].y = 286;//generation 2
	imDest[5].x = 205; imDest[5].y = 373;//chinese nationlism
	imDest[6].x = 234; imDest[6].y = 436;
	area.clear();
	//标注可以行走的四方形区域
	for(i=0;i<areaChinaNumber;i++)
	{
		struct Area tempArea;
		area.push_back(tempArea);		
	}
    
	mapPoint tempBranch;
	area[0].point[0].x = 293; area[0].point[0].y = 764;
	area[0].point[1].x = 381; area[0].point[1].y = 764;
	area[0].point[2].x = 381; area[0].point[2].y = 823;
	area[0].point[3].x = 293; area[0].point[3].y = 823;
	area[0].number = 0;
	tempBranch.x = 294; tempBranch.y = 809;
	area[0].branch.insert(pair<int,mapPoint>(1,tempBranch));
	tempBranch.x = 293; tempBranch.y = 772;
	area[0].branch.insert(pair<int,mapPoint>(2,tempBranch));

	area[1].point[0].x = 293; area[1].point[0].y = 796;
	area[1].point[1].x = 293; area[1].point[1].y = 823;
	area[1].point[2].x = 211; area[1].point[2].y = 823;
	area[1].point[3].x = 211; area[1].point[3].y = 796;
	area[1].number = 1;
	tempBranch.x = 287; tempBranch.y = 808;
	area[1].branch.insert(pair<int,mapPoint>(0,tempBranch));
	tempBranch.x = 214; tempBranch.y = 812;
	area[1].branch.insert(pair<int,mapPoint>(3,tempBranch));

	area[2].point[0].x = 292; area[2].point[0].y = 782;
	area[2].point[1].x = 230; area[2].point[1].y = 782;
	area[2].point[2].x = 230; area[2].point[2].y = 528;
	area[2].point[3].x = 292; area[2].point[3].y = 528;
	area[2].number = 2;
	tempBranch.x = 283; tempBranch.y = 772;
	area[2].branch.insert(pair<int,mapPoint>(0,tempBranch));
	tempBranch.x = 283; tempBranch.y = 543;
	area[2].branch.insert(pair<int,mapPoint>(4,tempBranch));

    area[3].point[0].x = 211; area[3].point[0].y = 823;
	area[3].point[1].x = 167; area[3].point[1].y = 823;
	area[3].point[2].x = 167; area[3].point[2].y = 481;
	area[3].point[3].x = 211; area[3].point[3].y = 481;
	area[3].number = 3;
	tempBranch.x = 211; tempBranch.y = 810;
	area[3].branch.insert(pair<int,mapPoint>(1,tempBranch));
	tempBranch.x = 186; tempBranch.y = 486;
	area[3].branch.insert(pair<int,mapPoint>(5,tempBranch));

	area[4].point[0].x = 292; area[4].point[0].y = 747;
	area[4].point[1].x = 292; area[4].point[1].y = 420;
	area[4].point[2].x = 334; area[4].point[2].y = 420;
	area[4].point[3].x = 334; area[4].point[3].y = 747;
	area[4].number = 4;
	tempBranch.x = 291; tempBranch.y = 543;
	area[4].branch.insert(pair<int,mapPoint>(2,tempBranch));
	tempBranch.x = 313; tempBranch.y = 427;
	area[4].branch.insert(pair<int,mapPoint>(6,tempBranch));
	
    area[5].point[0].x = 169; area[5].point[0].y = 431;
	area[5].point[1].x = 279; area[5].point[1].y = 431;
	area[5].point[2].x = 279; area[5].point[2].y = 482;
	area[5].point[3].x = 169; area[5].point[3].y = 482;
	area[5].number = 5;
	tempBranch.x = 182; tempBranch.y = 475;
	area[5].branch.insert(pair<int,mapPoint>(3,tempBranch));
	tempBranch.x = 237; tempBranch.y = 434;
	area[5].branch.insert(pair<int,mapPoint>(12,tempBranch));
	
	area[6].point[0].x = 277; area[6].point[0].y = 420;
	area[6].point[1].x = 277; area[6].point[1].y = 306;
	area[6].point[2].x = 354; area[6].point[2].y = 321;
	area[6].point[3].x = 354; area[6].point[3].y = 420;
	area[6].number = 6;
	tempBranch.x = 282; tempBranch.y = 314;
	area[6].branch.insert(pair<int,mapPoint>(7,tempBranch));
	tempBranch.x = 313; tempBranch.y = 421;
	area[6].branch.insert(pair<int,mapPoint>(4,tempBranch));

	area[7].point[0].x = 289; area[7].point[0].y = 305;
	area[7].point[1].x = 245; area[7].point[1].y = 305;
	area[7].point[2].x = 245; area[7].point[2].y = 41;
	area[7].point[3].x = 289; area[7].point[3].y = 41;
	area[7].number = 7;
	tempBranch.x = 287; tempBranch.y = 287;
	area[7].branch.insert(pair<int,mapPoint>(8,tempBranch));
	tempBranch.x = 258; tempBranch.y = 307;
	area[7].branch.insert(pair<int,mapPoint>(9,tempBranch));
	tempBranch.x = 282; tempBranch.y = 306;
	area[7].branch.insert(pair<int,mapPoint>(6,tempBranch));

	area[8].point[0].x = 289; area[8].point[0].y = 299;
	area[8].point[1].x = 289; area[8].point[1].y = 231;
	area[8].point[2].x = 351; area[8].point[2].y = 231;
	area[8].point[3].x = 351; area[8].point[3].y = 299;
	area[8].number = 8;
	tempBranch.x = 296; tempBranch.y = 290;
	area[8].branch.insert(pair<int,mapPoint>(7,tempBranch));

	area[9].point[0].x = 235; area[9].point[0].y = 305;
	area[9].point[1].x = 266; area[9].point[1].y = 305;
	area[9].point[2].x = 266; area[9].point[2].y = 385;
	area[9].point[3].x = 235; area[9].point[3].y = 385;
	area[9].number = 9;
	tempBranch.x = 255; tempBranch.y = 311;
	area[9].branch.insert(pair<int,mapPoint>(7,tempBranch));
    tempBranch.x = 242; tempBranch.y = 373;
	area[9].branch.insert(pair<int,mapPoint>(10,tempBranch));

	area[10].point[0].x = 234; area[10].point[0].y = 383;
	area[10].point[1].x = 176; area[10].point[1].y = 383;
	area[10].point[2].x = 176; area[10].point[2].y = 43;
	area[10].point[3].x = 234; area[10].point[3].y = 43;
	area[10].number = 10;
	tempBranch.x = 234; tempBranch.y = 372;
	area[10].branch.insert(pair<int,mapPoint>(9,tempBranch));
    tempBranch.x = 182; tempBranch.y = 379;
	area[10].branch.insert(pair<int,mapPoint>(11,tempBranch));
    
	area[11].point[0].x = 160; area[11].point[0].y = 369;
	area[11].point[1].x = 177; area[11].point[1].y = 369;
	area[11].point[2].x = 177; area[11].point[2].y = 417;
	area[11].point[3].x = 160; area[11].point[3].y = 417;
	area[11].number = 11;
	tempBranch.x = 173; tempBranch.y = 378;
	area[11].branch.insert(pair<int,mapPoint>(10,tempBranch));
    tempBranch.x = 172; tempBranch.y = 412;
	area[11].branch.insert(pair<int,mapPoint>(12,tempBranch));

	area[12].point[0].x = 178; area[12].point[0].y = 396;
	area[12].point[1].x = 266; area[12].point[1].y = 396;
	area[12].point[2].x = 266; area[12].point[2].y = 432;
	area[12].point[3].x = 178; area[12].point[3].y = 432;
	area[12].number = 12;
	tempBranch.x = 179; tempBranch.y = 408;
	area[12].branch.insert(pair<int,mapPoint>(11,tempBranch));
    tempBranch.x = 236; tempBranch.y = 424;
	area[12].branch.insert(pair<int,mapPoint>(5,tempBranch));
	//根据区域将地图初始化
	for(i=0;i<120;i++)
		for(j=0;j<120;j++)
		{
			naviMap[i][j] = false;
			struct mapPoint center;
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
        mapPoint start 起始点
		mapPoint end  结束点
*/
vector<struct mapPoint> SearchRoad::drawRoad(mapPoint start,mapPoint end)
{
	resultPath.clear();
    int i,endArea;
	int startArea = -1;
	vector<struct mapPoint> returnPoint;
	    aSearch(start,end);
		for(vector<mapPoint>::iterator iter = path.begin();iter!=path.end();iter++)
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
				//cout<<area[i].point[0].x<<" "<<area[i].point[1].x<<" "<<area[i].point[2].x<<" "<<area[i].point[3].x<<endl;
				//cout<<area[i].point[0].y<<" "<<area[i].point[1].y<<" "<<area[i].point[2].y<<" "<<area[i].point[3].y<<endl;
				startArea = i;
			}
			if(PtInAnyRect(end,area[i].point[0],area[i].point[1],area[i].point[2],area[i].point[3]))
			{
				endArea = i;
			}
		}
		//startArea = judgeArea(start);
		//cout<<startArea<<" "<<endArea<<endl;
	if(startArea != -1)
	 {
		cout<<startArea<<" "<<endArea<<endl;
		if(startArea == endArea)//开始和结束点在一个区域
		{
		  mapPoint tempPoint;
		  tempPoint.x = start.x; tempPoint.y = start.y;
		  returnPoint.push_back(tempPoint);
		  tempPoint.x = end.x; tempPoint.y = end.y;
		  returnPoint.push_back(tempPoint);
		}
		else //在不同的区域
		{
			if(pathArea.size()>=2)
			{
			mapPoint tempPoint;
	        tempPoint.x = start.x; tempPoint.y = start.y;
		    returnPoint.push_back(tempPoint);
			for(vector<int>::iterator iter = pathArea.end()-1;iter!=pathArea.begin();iter--)
			{
				
				tempPoint.x = area[*iter].branch[*(iter-1)].x;
                tempPoint.y = area[*iter].branch[*(iter-1)].y;
				returnPoint.push_back(tempPoint);

				tempPoint.x = area[*(iter-1)].branch[*iter].x;
                tempPoint.y = area[*(iter-1)].branch[*iter].y;
				returnPoint.push_back(tempPoint);
			
				
			}
			tempPoint.x=end.x;tempPoint.y=end.y;
			returnPoint.push_back(tempPoint);
			}
		}
	 }
		resultPath = returnPoint;
		return returnPoint;
}
/*
   函数名:judgeArea
   功能：判断点所在区域
   参数: mapPoint now 点的信息
*/
 int SearchRoad::judgeArea(mapPoint now)
 {
	 int i;
	 for(i=0;i<areaAccount;i++)
		{
			if(PtInAnyRect(now,area[i].point[0],area[i].point[1],area[i].point[2],area[i].point[3]))//判断区域
			{
				return i;
			}
	    }
	 return -1;
 }
 /*
   函数名:isOnTheWay
   功能：判断点是否在路线区域上
   参数: int nowArea 点所属区域
         vector<mapPoint> result 路线容器
*/
 int SearchRoad::isOnTheWay(int nowArea,vector<mapPoint> result)
 {
     int max = -1;
	 for(map<int,mapPoint>::iterator iter = area[nowArea].branch.begin();iter != area[nowArea].branch.end();iter++)
	 {
		 int i=0;
		 for(vector<mapPoint>::iterator iter1 = result.begin();iter1!=result.end();iter1++)
	    {
			if(iter1->x == iter->second.x && iter1->y == iter->second.y)//找到相同的
			{
				 if(max <= i)
				    max = i;
			}
			i++;
	    }
	 }
	 return max;
 }
  /*
   函数名: updateRoad
   功能：每一帧更新路径
   参数: mapPoint now 现在位置
         mapPoint end 目的位置
*/
 vector<struct mapPoint> SearchRoad::updateRoad(mapPoint now,mapPoint end)
 {
	    int i;
	    int nowArea,endArea;
		vector<mapPoint> returnPoint;
		//每一帧得到一个新位置,判断新位置属于哪个区域
		nowArea =  judgeArea(now);
		endArea =  judgeArea(end);
		//如果不在任何区域，则抛弃
		if(nowArea == -1)
		{
			cout<<"Discard"<<endl;
			returnPoint = resultPath;
		}
		//如果与目的点在一个区域直接连上
		else if(nowArea == endArea)
		{
			returnPoint.push_back(now);
			returnPoint.push_back(resultPath[resultPath.size()-1]);
		}
		else if(nowArea != endArea)
		{
		//如果不在一个区域则分俩种情况:
		//1.属于路线上的区域，则根据result容器来连接
			int nextNumber = isOnTheWay(nowArea,resultPath);
			if(nextNumber>=0)
			{
				returnPoint.push_back(now);
				for(i=nextNumber;i<resultPath.size();i++)
				{
					returnPoint.push_back(resultPath[i]);
				}
			}
		//2.如果不属于路线上的区域，则重新计算路径
			else
			{
	           drawRoad(now,end);
			   return resultPath;
			}
		}
		return returnPoint;
 }
 /*
   函数名: crossProduct
   功能：叉乘计算
   参数: mapPoint A 
         mapPoint B  一条边的俩个点
		 mapPoint M  要检测的点
*/  
 inline int SearchRoad::crossProduct(mapPoint A,mapPoint B,mapPoint M)
 {
	 int result;
	 mapPoint temp1,temp2;
	 temp1.x = B.x - A.x;
	 temp1.y = B.y - A.y;

	 temp2.x = M.x - A.x;
	 temp2.y = M.y - A.y;
	 
	 result= temp1.x * temp2.y - temp1.y * temp2.x;
	 return result;
 }