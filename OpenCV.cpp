#include"Asearch.h"
using namespace std;
int main()
{
	int i;
	CvSize sz;//图像尺寸
	sz.height =1200;
	sz.width = 1200;
	IplImage *image = cvCreateImage(sz, IPL_DEPTH_8U, 3);
	SearchRoad searchRoad;
	
	searchRoad.InitPoint();

	Point start,end;
	FILE *fp = fopen("Points.txt", "r");
    
	int cnt = 1029795 + 1211580 + 661958;//点的个数

	char line[1024];
	for(int i = 0; i < cnt; i++)
	{
		float x, y;
		fgets(line, 1000, fp);
		sscanf(line, "%f %f", &x, &y);
		if(x < -20 || x > 20 || y < -20 || y > 20)
			continue;

		CvPoint Point;
		Point.x = (int)((x + 20) * 30);
		Point.y = (int)((-y + 20) * 30);
		cvCircle(image, Point, 2, CV_RGB(0, 191, 255));
	}
	fclose(fp);
	//确定起始和结束点
		end.x = searchRoad.imDest[2].x;
		end.y = searchRoad.imDest[2].y;
		start.x = 625;
		start.y = 524;
	    
		searchRoad.drawRoad(image,start,end);

	cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
	cvShowImage("test", image);
	cvSaveImage("map.png", image);
	cvWaitKey(0);
	cvReleaseImage(&image);
	cvDestroyWindow("test");

	return 0;
}