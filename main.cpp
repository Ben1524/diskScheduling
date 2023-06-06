#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

const int MAX=200; //最大磁道数

class diskScheduling
{
private:
    static int startIndexCon;
    int startIndex; // 磁头初始位置
    vector<int> request; // 请求序列
public:
    diskScheduling(int startIndex=0):startIndex(startIndex)
    {
        initRequestAndStartIndex();
    }
    void initRequestAndStartIndex(); // 初始化请求序列
    void FCFS(int ss=startIndexCon); // 先来先服务
    void SSTF(int ss=startIndexCon); // 最短寻道时间优先
    void SCAN(int ss=startIndexCon,bool opreational=true); // 扫描算法,opreational为true表示向外扫描，false表示向内扫描
    void C_SCAN(int ss=startIndexCon,bool opreational = true); // 循环扫描算法
};

int diskScheduling::startIndexCon=0;

void diskScheduling::initRequestAndStartIndex()
{
    string str;
    getline(cin,str);
    stringstream ss(str); // 将字符串转换为流
    int temp;
    while(ss>>temp)
    {
        request.push_back(temp);
    }
    startIndexCon=startIndex;
}

void diskScheduling::FCFS(int ss)
{
    startIndex=ss;
    int sum=0;
    cout<<"先来先服务算法："<<endl;
    cout<<"磁头初始位置："<<startIndex<<endl;
    cout<<"磁道访问序列：";
    for(int i=0;i<request.size();i++)
    {
        cout<<request[i]<<" ";
        sum+=abs(startIndex-request[i]); // 计算移动磁道数
        startIndex=request[i];
    }
    cout<<endl;
    cout<<"移动磁道数："<<sum<<endl;
    cout<<"平均寻道长度："<<(double)sum/request.size()<<endl;
}

void diskScheduling::SSTF(int ss)
{
    startIndex=ss;
    int sum=0;
    cout<<"最短寻道时间优先算法："<<endl;
    cout<<"磁头初始位置："<<startIndex<<endl;
    cout<<"磁道访问序列：";
    vector<int> temp=request;
    sort(temp.begin(),temp.end()); // 将请求序列排序
    int index=0;
    while(!temp.empty())
    {
        int min=MAX;
        int minIndex=0;
        for(int i=0;i<temp.size();i++)
        {
            if(abs(startIndex-temp[i])<min) // 找到最短寻道时间
            {
                min=abs(startIndex-temp[i]);
                minIndex=i; // 记录最短寻道时间的下标
            }
        }
        cout<<temp[minIndex]<<" ";
        sum+=min;
        startIndex=temp[minIndex];
        temp.erase(temp.begin()+minIndex); // 删除已经访问的磁道
    }
    cout<<endl;
    cout<<"移动磁道数："<<sum<<endl;
    cout<<"平均寻道长度："<<(double)sum/request.size()<<endl;
}


// 单向扫描函数用于辅助SCAN和C_SCAN函数
int oneWayScan(const vector<int> &temp,int startIndex,bool opreational)
{
    int sum=0;
    // temp为已经排序的请求序列
    if(opreational) // 向外扫描
    {
        for(int i=0;i<temp.size();i++)
        {
            cout<<temp[i]<<" ";
            sum+=abs(startIndex-temp[i]); // 计算移动磁道数
            startIndex=temp[i];
        }
    }
    else // 向内扫描
    {
        for(int i=temp.size()-1;i>=0;i--)
        {
            cout<<temp[i]<<" ";
            sum+=abs(startIndex-temp[i]); // 计算移动磁道数
            startIndex=temp[i];
        }
    }
    return sum;
}

void diskScheduling::SCAN(int ss,bool opreational)
{
    startIndex=ss;
    int sum=0;
    cout<<"扫描算法："<<endl;
    cout<<"磁头初始位置："<<startIndex<<endl;
    cout<<"磁道访问序列：";
    vector<int> temp=request;
    sort(temp.begin(),temp.end()); // 将请求序列排序
    int index=0,i;
    if(opreational) // 向外扫描
    {
        for(i=0;i<temp.size();i++)
        {
            if(temp[i]>=startIndex) // 找到第一个大于等于磁头位置的磁道
            {
                index=i;
                break;
            }
        }
        while(index<temp.size())
        {
            sum+=abs(startIndex-temp[index]); // 计算移动磁道数
            cout<<temp[index]<<" ";
            startIndex=temp[index];
            index++;
        }
        temp.resize(i); // 删除已经访问的磁道
        opreational=!opreational; // 改变扫描方向
        sum+=oneWayScan(temp,startIndex,opreational); // 计算向内扫描的移动磁道数
    }
    else // 向内扫描
    {
        int i;
        for(i=temp.size()-1;i>=0;i--)
        {
            if(temp[i]<=startIndex) // 找到第一个小于等于磁头位置的磁道
            {
                index=i;
                break;
            }
        }
        while(index>=0)
        {
            sum+=abs(startIndex-temp[index]); // 计算移动磁道数
            cout<<temp[index]<<" ";
            startIndex=temp[index];
            index--;
        }
        temp.erase(temp.begin(),temp.begin()+i+1); // 删除已经访问的磁道
        opreational=!opreational; // 改变扫描方向
        sum+=oneWayScan(temp,startIndex,opreational); // 计算向外扫描的移动磁道数
    }

    cout<<endl;
    cout<<"移动磁道数："<<sum<<endl;
    cout<<"平均寻道长度："<<(double)sum/request.size()<<endl;
}

void diskScheduling::C_SCAN(int ss,bool opreation)
{
    startIndex=ss;
    int sum=0;
    cout<<"循环扫描算法："<<endl;
    cout<<"磁头初始位置："<<startIndex<<endl;
    cout<<"磁道访问序列：";
    vector<int> temp=request;
    sort(temp.begin(),temp.end()); // 将请求序列排序
    int index=0,i;
    if(opreation)
    {
        for(i=0;i<temp.size();i++)
        {
            if(temp[i]>=startIndex) // 找到第一个大于等于磁头位置的磁道
            {
                index=i;
                break;
            }
        }
        while(index<temp.size())
        {
            sum+=abs(startIndex-temp[index]); // 计算移动磁道数
            cout<<temp[index]<<" ";
            startIndex=temp[index];
            index++;
        }
        temp.resize(i); // 删除已经访问的磁道
        sum+=oneWayScan(temp,startIndex,opreation); // 计算向内扫描的移动磁道数
    }
    else
    {
        int i;
        for(i=temp.size()-1;i>=0;i--)
        {
            if(temp[i]<=startIndex) // 找到第一个小于等于磁头位置的磁道
            {
                index=i;
                break;
            }
        }
        while(index>=0)
        {
            sum+=abs(startIndex-temp[index]); // 计算移动磁道数
            cout<<temp[index]<<" ";
            startIndex=temp[index];
            index--;
        }
        temp.erase(temp.begin(),temp.begin()+i+1); // 删除已经访问的磁道
        sum+=oneWayScan(temp,startIndex,opreation); // 计算向外扫描的移动磁道数
    }

    cout<<"移动磁道数："<<sum<<endl;
    cout<<"平均寻道长度："<<(double)sum/request.size()<<endl;
}

int main()
{
    diskScheduling ds(100);
    cout<<"------------------FCFS------------------"<<endl;
    ds.FCFS();
    cout<<"------------------SSTF------------------"<<endl;
    ds.SSTF();
    cout<<"------------------SCAN------------------"<<endl;
    ds.SCAN();
    cout<<"------------------CSCAN------------------"<<endl;
    ds.C_SCAN();
    return 0;
}

// 55 58 39 18 90 160 150 38 184