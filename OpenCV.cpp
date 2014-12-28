#include"Asearch.h"
#include<ctime>
using namespace std;
int main()
{
	CvSize sz;//图像尺寸
	sz.height = 900;
	sz.width  = 450;
	IplImage *image = cvCreateImage(sz, IPL_DEPTH_8U, 3);
	int i;

	SearchRoad searchRoad(0);
	/*
	for(i=0;i<13;i++)
	{
	 cvLine(image,cvPoint(searchRoad.area[i].point[0].x,searchRoad.area[i].point[0].y),cvPoint(searchRoad.area[i].point[1].x,searchRoad.area[i].point[1].y),CV_RGB(0, 0, 255));
	 cvLine(image,cvPoint(searchRoad.area[i].point[1].x,searchRoad.area[i].point[1].y),cvPoint(searchRoad.area[i].point[2].x,searchRoad.area[i].point[2].y),CV_RGB(0, 0, 255));
	 cvLine(image,cvPoint(searchRoad.area[i].point[2].x,searchRoad.area[i].point[2].y),cvPoint(searchRoad.area[i].point[3].x,searchRoad.area[i].point[3].y),CV_RGB(0, 0, 255));
	 cvLine(image,cvPoint(searchRoad.area[i].point[3].x,searchRoad.area[i].point[3].y),cvPoint(searchRoad.area[i].point[0].x,searchRoad.area[i].point[0].y),CV_RGB(0, 0, 255));
	}
	*/
	mapPoint start,end;
	vector<mapPoint> result;
	  clock_t first,second;
	//for(i=0;i<5;i++)
	//{
	 /*
		end.x = searchRoad.imDest[5].x;
		end.y = searchRoad.imDest[5].y;
		for(int j=0;j<=1200;j++)
		 for(int k=0;k<=1200;k++)
		 {
			 start.x = j;
			 start.y = k;
			 cout<<start.x<<" "<<start.y<<endl;
			 result = searchRoad.drawRoad(start,end);
			 for(vector<int>::iterator iter = searchRoad.pathArea.begin();iter!=searchRoad.pathArea.end();iter++)
		        {
			     cout<<*iter<<endl;
		        }
		}
		*/
	//}
	
		cout<<"Projection Room"<<endl;
		end.x = searchRoad.imDest[3].x;
		end.y = searchRoad.imDest[3].y;
		start.x = 565;
		start.y = 515;
		first = clock();
	    result = searchRoad.drawRoad(start,end);
		second = clock();
		//cout<<"11"<<endl;
		/*
		for(vector<int>::iterator iter = searchRoad.pathArea.begin();iter!=searchRoad.pathArea.end();iter++)
		{
			cout<<*iter<<endl;
		}
		*/
		cout<<"耗时  "<<second-first<<endl;
		
		/*
		mapPoint now;
		
		now.x = 300;
		now.y = 808;
		result = searchRoad.updateRoad(now,end);
	    for(vector<mapPoint>::iterator iter = result.begin();iter!=result.end();iter++)
		{
			cout<<iter->x<<" "<<iter->y<<endl;
		}
		cout<<endl;

		now.x = 311;
		now.y = 785;
		searchRoad.updateRoad(now,end);
		result = searchRoad.updateRoad(now,end);
	    for(vector<mapPoint>::iterator iter = result.begin();iter!=result.end();iter++)
		{
			cout<<iter->x<<" "<<iter->y<<endl;
		}
		cout<<endl;
        */
	cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
	cvShowImage("test", image);
	cvSaveImage("test.png", image);
	cvWaitKey(0);
	cvReleaseImage(&image);
	cvDestroyWindow("test");

		 system("pause");
	return 0;
}
