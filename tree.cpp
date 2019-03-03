#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <math.h>
#include<bits/stdc++.h>  
//RNG rng(12345);

using namespace std;
using namespace cv;

Point2f finds(Mat);
int thresh=100;

typedef struct {int x;int y;}cordi;
struct node{ 
              node *parent;
              cordi position;};


int main(int argc, char *argv[])
{

        int i,j;
        cordi last,ini,fin,newp,q;
        namedWindow("win", 0);
        VideoCapture Video("dynamic_obstacles.mp4");
        Mat frame;
        vector<cordi> vec;
vector<cordi>::iterator ptr;
        srand(time(NULL));
        Video>>frame;
        //cout<<frame.rows<<frame.cols<<endl;
        Mat img1(frame.rows,frame.cols,CV_8UC3,Scalar(0,0,0));

        Mat new1(frame.rows,frame.cols,CV_8UC1,Scalar(0));
        Mat new2(frame.rows,frame.cols,CV_8UC1,Scalar(0));

        for(int i=0;i<frame.rows;i++)
        {
                for(int j=0;j<frame.cols;j++)
                {
                        if ((frame.at<Vec3b>(i,j)[0]<200) && (frame.at<Vec3b>(i,j)[1]<200)&&(frame.at<Vec3b>(i,j)[2]>100))
                        {
                                new1.at<uchar>(i,j)=255;
                                frame.at<Vec3b>(i,j)={255,255,255};
                        }
                        if ((frame.at<Vec3b>(i,j)[0]<200) && (frame.at<Vec3b>(i,j)[1]>100)&&(frame.at<Vec3b>(i,j)[2]<200))
                        {
                                new2.at<uchar>(i,j)=255;
                                frame.at<Vec3b>(i,j)={255,255,255};
                        }
                }
        }

        
        Point2f p1=finds(new1);
        ini.x=(int)p1.x;
        ini.y=(int)p1.y;

        Point2f p2=finds(new2);
        fin.x=(int)p2.x;
        fin.y=(int)p2.y;

        //cout<<inix<<" "<<iniy<<endl;
        //cout<<finx<<" "<<finy<<endl;

        vec.push_back(ini);
                
        
        float dist;
        while(1)
        {
                int min=100000000;
                Video >> frame;
                //Mat img1 = frame.clone();
                int step=25;
                q.y=rand()%frame.rows;
                q.x=(frame.cols-frame.rows)/2+rand()%frame.rows;
                //cout<<qx<<" "<<qy<<endl;
                Mat framebw(frame.rows,frame.cols,CV_8UC1,Scalar(255));
                for(int i=0 ; i<frame.rows ; i++)               
                {
                        for(int j=0 ; j<frame.cols ; j++)
                        {
                                

                                if((frame.at<Vec3b>(i,j)[0]<5)&&(frame.at<Vec3b>(i,j)[1]<5)&&(frame.at<Vec3b>(i,j)      [2]<5))
                                {
                                        framebw.at<uchar>(i,j)=0;
                                }       
        
                
                                if((frame.at<Vec3b>(i,j)[0]>200)&&(frame.at<Vec3b>(i,j)[1]<15)&&(frame.at<Vec3b>(i,j)   [2]<15))
                                {
                                        framebw.at<uchar>(i,j)=0;
                                }
                        }
                }
                //imshow("win",framebw);
                //waitKey(0);
                for(ptr=vec.begin();ptr<vec.end();ptr++)                                                 //finding the next node
                {
                        //cout<<vecx[i]<<" "<<vecy[i]<<endl;
                        dist=(q.x-(*ptr).x)*(q.x-(*ptr).x)+(q.y-(*ptr).y)*(q.y-(*ptr).y);
                        //cout<<dist<<endl;
                        if (min>dist)
                        {
                                //cout<<min;
                                min=dist;
                                last=(*ptr);
                                
                        }
                }
                //cout<<"----------------------------------"<<endl;

                //cout<<lastx<<lasty<<endl;
                
                float t = ((float)(q.y - last.y))/(q.x - last.x);
                float theta = abs(atan(t)); 

                if((q.x>last.x)&&(q.y>last.y))
                {
                        newp.x = (int)(last.x + step*cos(theta));
                        newp.y = (int)(last.y + step*sin(theta));
                }
                else if((q.x>last.x)&&(q.y<last.y))
                {
                        newp.x = (int)(last.x + step*cos(theta));
                        newp.y = (int)(last.y - step*sin(theta));
                }
                else if((q.x<last.x)&&(q.y>last.y))
                {
                        newp.x = (int)(last.x - step*cos(theta));
                        newp.y = (int)(last.y + step*sin(theta));
                }
                else
                {
                        newp.x = (int)(last.x - step*cos(theta));
                        newp.y = (int)(last.y - step*sin(theta));
                }

                /*if ((qx-finx)*(qx-finx)+(qy-finy)*(qy-finy)<=step*step)
                {
                        break;
                }*/


                int k=0;
                if (framebw.at<uchar>(newp.y,newp.x) == 255) 
                        k=1; 
                        //cout<<0;
                      
                //cout<<lastx<<" "<<lasty<<" "<<qx<<" "<<qy;       
                if((newp.x>=0)&&(newp.y>=0)&&(newp.x<frame.cols)&&(newp.y<frame.rows)&&(k==1))
                {
                        //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                        line(img1, Point(last.x,last.y), Point(newp.x,newp.y), Scalar(255,0,0), 2,6,0);
                        vec.push_back(newp);
                        //cout<<1;
                }


                //cout<<qx<<qy<<endl;
                /*if (frame.at<Vec3b>(qx,qy)[0]<20 && frame.at<Vec3b>(qx,qy)[1]<20 && frame.at<Vec3b>(qx,qy)[2]<20)
                        continue;*/
                /*if (frame.at<Vec3b>(qx,qy)[0]>=190 && frame.at<Vec3b>(qx,qy)[0]<220 && frame.at<Vec3b>(qx,qy)[1]>=0 && frame.at<Vec3b>(qx,qy)[1]<20 && frame.at<Vec3b>(qx,qy)[2]>=0 && frame.at<Vec3b>(qx,qy)[2]<20)
                        continue;*/
                //cout<<lastx<<qx<<endl;
                //cout<<lasty<<qy<<endl;

                //line(frame, Point(lastx,lasty), Point(qx,qy), Scalar(255), 2);

                //for (int i=lastx;i<)
                
                for(int i=0;i<img1.rows;i++)
                {
                        for(int j=0;j<img1.cols;j++)
                        {
                                if (img1.at<Vec3b>(i,j)[0]!=0 || img1.at<Vec3b>(i,j)[0]!=0 || img1.at<Vec3b>(i,j)[0]!=0)
                                {
                                        frame.at<Vec3b>(i,j)=img1.at<Vec3b>(i,j);
                                }
                        }
                }

                if(5000>=((fin.x-newp.x)*(fin.x-newp.x)+(fin.y-newp.y)*(fin.y-newp.y)))
                {
                  line(frame,Point(fin.x,fin.y),Point(newp.x,newp.y),Scalar(255,0,0),2,6,0);
                  imshow("win",frame);
                waitKey(1000);
                cout<<"path found"<<endl;
                  break;
                }              
                
                //imshow("win1",img1);
                //waitKey(1);
                imshow("win",frame);
                waitKey(1);
        }
}

       /*Node up, down;
        down.cord.x =finx ;
        down.cord.y=finy;
        up = *(end_node.parent);
        while(1)
        {
            line(img, Point(up.position.y, up.position.x), Point(down.position.y, down.position.x), Scalar(0, 255, 0), 2, 8);
            if(up.parent == NULL)
              break;
            up = *(up.parent);
            down = *(down.parent);
        }*/




Point2f finds(Mat img)
{
        Mat canny_output;
        Canny( img, canny_output, thresh, thresh*2 );
        vector<vector<Point> > contours;
        findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Point2f>centers( contours.size() );
        vector<float>radius( contours.size() );
        int max_radius=0,maxR_index;

        for(int i = 0; i < contours.size(); i++ )
        {
                approxPolyDP( contours[i], contours_poly[i], 3, true );
                minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
                if(radius[i]>max_radius) 
                {       
                        max_radius=radius[i];
                        maxR_index=i;
                }
        }
        Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
                

        Scalar color = Scalar(255,255,255);
        circle( drawing, centers[maxR_index], (int)radius[maxR_index], color, 1 );

        return centers[maxR_index];
}