#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <bits/stdc++.h>

#define rows img.rows
#define cols img.cols
#define ll long long int

using namespace cv;
using namespace std;

struct point{
    ll x;                   //x represents the xth row
    ll y;                   //y represents the yth column
};
struct Line{
    float a;
    float b;
    float c;
    //ax+by+c=0
};
struct Edge{
    point p1; //this is the parent node.
    point p2; //this is the child node.
};

Mat img = imread("Test2.png",1);
Mat cpy;
Mat vis(rows,cols,CV_8UC1, Scalar(0));

vector<point> nodes;
vector<Edge> edges;
vector<point> qu;

vector<point>::iterator ptr;
vector<point>::iterator itr;
vector<Edge>::iterator itr2;
vector<Edge>::iterator ptr2;



bool isvalid(point p){
    if((p.x<0 || p.x>=rows) || (p.y<0 || p.y>cols))
        return 0;
    else if(img.at<Vec3b>(p.x,p.y)[0]>230 && img.at<Vec3b>(p.x,p.y)[1]>230 && img.at<Vec3b>(p.x,p.y)[2]>230)
        return 0;
   else if(img.at<Vec3b>(p.x,p.y)[0]>230 && img.at<Vec3b>(p.x,p.y)[1]<20 && img.at<Vec3b>(p.x,p.y)[2]<20)
       return 0;
    else
        return 1;
}
bool lineSatisfy(Line l, point p){
    if(abs(l.a*p.x+l.b*p.y+l.c) < 10000000)
        return 1;
    return 0;
}
Line eqnOfLine(point p1, point p2){
    if(p1.y - p2.y == 0)
        return {0, 1, (float)(-p1.y)};
    return {1, (float)((p1.x-p2.x)/(p2.y-p1.y)), (float)((p1.x*p2.y-p2.x-p1.y)/(p1.y-p2.y))};
}
bool isValid(Edge v){
    Line l = eqnOfLine(v.p1, v.p2);
    for(int i= min(v.p1.x,v.p2.x); i<=max(v.p1.x,v.p2.x); i++){
        for(int j=min(v.p1.y,v.p2.y); j<= max(v.p1.y,v.p2.y); j++){
            if(lineSatisfy(l, {i,j})){
                if(!isvalid({i,j})) return 0;
            }   
        }
    }
    return 1;
}
float dist(point A, point B){
    int dx = A.x-B.x;
    int dy = A.y - B.y;
    return sqrt(dx*dx + dy*dy);
}
point randompoint(){
    point random;
    random.x = rand()%rows;
    random.y = rand()%cols;
    if(isvalid(random))
        return random;
    else
        return randompoint();
}
point nearestpoint(point rand){
    float min = 100000000000;
    point near;


    for(auto u: nodes){
        if(dist(u, rand) < min){
            near = u;
            min = dist(u, rand);
        }
    }
    return near;
}
point steer(point np, point rp){
    float minDist = dist(np,rp);
    point newpoint;
    Line l = eqnOfLine(np,rp);
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(lineSatisfy(l,{i,j})){
                if(dist({i,j}, np)==5){
                    if(dist({i,j},rp)<=minDist){
                        newpoint = {i,j};
                        minDist = dist(newpoint,rp);   
                    }
                }
            }
        }
    }
    return newpoint;
}
void binary(){
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if(img.at<Vec3b>(i,j)[0]>100 && img.at<Vec3b>(i,j)[1]>100 && img.at<Vec3b>(i,j)[2]>100)
                img.at<Vec3b>(i,j)={255,255,255};
            if(img.at<Vec3b>(i,j)[0]<100 && img.at<Vec3b>(i,j)[1]<100 && img.at<Vec3b>(i,j)[2]<100)
                img.at<Vec3b>(i,j)={0,0,0};
            if(img.at<Vec3b>(i,j)[0]<100 && img.at<Vec3b>(i,j)[1]<100 && img.at<Vec3b>(i,j)[2]>200)
                img.at<Vec3b>(i,j)={0,0,255};
            if(img.at<Vec3b>(i,j)[0]<100 && img.at<Vec3b>(i,j)[1]>200 && img.at<Vec3b>(i,j)[2]<100)
                img.at<Vec3b>(i,j)={0,255,0};
        }
    }
}
point centre(int chnl){
    Mat img2=img.clone();
    int sumx=0, sumy=0, ctr=0;
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(img.at<Vec3b>(i,j)[0]>=220 && img.at<Vec3b>(i,j)[1]>=220 && img.at<Vec3b>(i,j)[2]>=220){
                img2.at<Vec3b>(i,j)[0]=0;
                img2.at<Vec3b>(i,j)[1]=0;
                img2.at<Vec3b>(i,j)[2]=0;
            }
        }
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            if(img2.at<Vec3b>(i,j)[chnl] >= 230){
                sumx += i;
                sumy += j;
                ctr++;
            }
        }
    }   
    point centre = {sumx/ctr, sumy/ctr};
    return centre;
}
void coloredge(Mat* obj, Edge v){
    Line l = eqnOfLine(v.p1, v.p2);
    for(int i= min(v.p1.x,v.p2.x); i<=max(v.p1.x,v.p2.x); i++){
        for(int j=min(v.p1.y,v.p2.y); j<= max(v.p1.y,v.p2.y); j++){
            if(lineSatisfy(l, {i,j})){
                obj->at<Vec3b>(i,j) = {0,0,255};
            }   
        }
    }
}
int main(){
    namedWindow("RRTree", WINDOW_NORMAL);
    namedWindow("2", WINDOW_NORMAL);
    binary();
     cpy = img.clone();
    srand(time(0));
    point init = centre(1);
    nodes.push_back(init);
    for(int i=1; i<=4000; i++){
        point rand = randompoint();

              for(ptr=nodes.begin(),ptr2=edges.begin();ptr!=nodes.end(),ptr2!=edges.end();ptr++,ptr2++)
        {
            if(cpy.at<Vec3b>((*ptr).x,(*ptr).y)[0]>=230&&cpy.at<Vec3b>((*ptr).x,(*ptr).y)[1]<20&&cpy.at<Vec3b>((*ptr).x,(*ptr).y)[2]<=20)
            {
                        
                for(itr=ptr,itr2=ptr2;itr!=nodes.end(),itr2!=edges.end();itr++,itr2++)
                    {line(cpy, Point {(*(ptr)).y, (*(ptr)).x}, Point {(*(ptr+1)).y,(*(ptr+1)).x}, Scalar(255,255,0), 1, 8);
                nodes.erase(itr);edges.erase(itr2);}
                
            }
        }


        point nearestNode = nearestpoint(rand);
        point newNode = steer(nearestNode, rand);
        if(!isvalid(newNode) || vis.at<uchar>(newNode.x, newNode.y) == 255){
            i--;
            continue;
        }
        Edge v = {nearestNode, newNode};
        if(!isValid(v)){
            i--;
            continue;
        }
        nodes.push_back(newNode);
        edges.push_back(v);
        line(cpy, Point {v.p1.y, v.p1.x}, Point {v.p2.y, v.p2.x}, Scalar(0, 255, 0), 1, 8);
        vis.at<uchar>(newNode.x, newNode.y) = 255;
        cpy.at<Vec3b>(newNode.x, newNode.y) = {0,255,0};
        printf("<<<<<<Iteration-%d>>>>>>>\nRandom Point generated : (%lld, %lld)\nnearestNode is (%lld, %lld)\nnewNode is (%lld, %lld)\n\n\n", i, rand.x, rand.y, nearestNode.x, nearestNode.y, newNode.x, newNode.y);
        imshow("RRTree", cpy);
        waitKey(1);
        if(img.at<Vec3b>(newNode.x, newNode.y)[2]>230)    break;
        if(i==4000){
            cout<<"Ran out of nodes, sorry for being the disappoinment.\n";
            return 0;
        }
    }
    point tmp = *(nodes.end()-1);
    qu.push_back(tmp);
    int j=0;
    int length = 0;
    cout<<"\n\n\n<<<<<<<<<Geneated Path>>>>>>>>>>\n\n\n";
    cout<<++j<<". ("<<tmp.x<<","<<tmp.y<<")\n";
    for(int i = edges.size()-1; i>=0; i--){
        if(edges[i].p2.x == tmp.x && edges[i].p2.y == tmp.y){
            tmp = edges[i].p1;
            qu.push_back(tmp);
            cout<<++j<<". ("<<tmp.x<<","<<tmp.y<<")\n";
            cpy.at<Vec3b>(edges[i].p2.x, edges[i].p2.y) = {0,255,0};
            length += dist(edges[i].p1, edges[i].p2);
            line(cpy, Point {edges[i].p1.y, edges[i].p1.x}, Point {edges[i].p2.y, edges[i].p2.x}, Scalar(0,0,255), 1, 8);
            imshow("RRTree", cpy);
            waitKey(1);
        }
    }
    cout<<"\n\n\nPath Lenght = "<<length<<"\n";
    waitKey(0);
    return 0;
}