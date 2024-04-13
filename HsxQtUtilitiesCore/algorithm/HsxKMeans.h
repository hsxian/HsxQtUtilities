#ifndef HSXKMEANS_H
#define HSXKMEANS_H

#include <HsxQtUtilitiesCore_global.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <time.h>
#include <QDebug>

namespace  hsx
{
using namespace std;
template<typename T>
class HsxKMeans
{
public:
    typedef struct Node
    {
        int minIndex; //the index of each node
        T minDist;
        Node(int idx, T dist) : minIndex(idx), minDist(dist) {}
    } tNode;
    enum CentroidSelectModeType
    {
        RAND,
        RAND_KPP,
        FIX,
    };
    HsxKMeans();
    //data to be clustered
    vector< vector<T> > dataSet;
    //cluster centers
    vector< vector<T> > centroids;
    //mark which cluster the data belong to
    vector<tNode>  clusterAssment;

    //load data into dataSet
    void loadData(const vector<vector<T> > &data);
    //running the kmeans algorithm
    void kmeans(int clusterCount);
    void setCentroidSelectMode(CentroidSelectModeType m);

    //default distance function ,defined as dis = (x-y)'*(x-y)
    static T defaultDistEclud(vector<T> &v1, vector<T> &v2);

protected:
    //colLen:the dimension of vector;rowLen:the number of vectors
    size_t colLen, rowLen;
    //count to be clustered
    int k;
    //mark the min and max value of a array
    typedef struct MinMax
    {
        T Min;
        T Max;
        MinMax(T min, T max) : Min(min), Max(max) {}
    } tMinMax;
    //distance function
    //reload this function if necessary
    T(*distEclud)(vector<T> &v1, vector<T> &v2);

    //get the min and max value in idx-dimension of dataSet
    tMinMax getMinMax(int idx);
    // 获取点到最近聚类中心的距离
    T getClosestDist(std::vector<T> point, std::vector<std::vector<T>> centroids);
    // Kmeans++算法选择初始聚类中心
    std::vector<std::vector<T>> kppCenters(std::vector<std::vector<T>> data_set, int k);
    //generate clusterCount centers randomly
    void randCent();
    CentroidSelectModeType m_CentroidSelectMode;
    void updateCent();
};

template<typename T>
HsxKMeans<T>::HsxKMeans()
{
    k = 0;
    colLen = 0;
    rowLen = 0;
    distEclud = defaultDistEclud;
}

template<typename T>
void HsxKMeans<T>::loadData(const vector<vector<T> > &data)
{
    this->dataSet = data; //kmeans do not change the original data;
    this->rowLen = data.size();
    this->colLen = data.at(0).size();
}

template<typename T>
void HsxKMeans<T>::kmeans(int clusterCount)
{
    this->k = clusterCount;

    //initial clusterAssment
    this->clusterAssment.clear();
    tNode node(-1, -1);
    for(size_t i = 0; i < rowLen; i++)
    {
        clusterAssment.push_back(node);
    }

    //initial cluster center
    this->updateCent();

    bool clusterChanged = true;
    //the termination condition can also be the loops less than some number such as 1000
    while(clusterChanged)
    {
        clusterChanged = false;
        for(size_t i = 0; i < rowLen; i++)
        {
            int minIndex = -1;
            T minDist = INT_MAX;
            for(int j = 0; j < k; j++)
            {
                T distJI = distEclud(centroids[j], dataSet[i]);
                if(distJI < minDist)
                {
                    minDist = distJI;
                    minIndex = j;
                }
            }
            if(clusterAssment[i].minIndex != minIndex)
            {
                clusterChanged = true;
                clusterAssment[i].minIndex = minIndex;
                clusterAssment[i].minDist = minDist;
            }
        }

        //step two : update the centroids
        for(int cent = 0; cent < k; cent++)
        {
            vector<T> vec(colLen, 0);
            int cnt = 0;
            for(size_t i = 0; i < rowLen; i++)
            {
                if(clusterAssment[i].minIndex == cent)
                {
                    ++cnt;
                    //sum of two vectors
                    for(size_t j = 0; j < colLen; j++)
                    {
                        vec[j] += dataSet[i].at(j);
                    }
                }
            }

            //mean of the vector and update the centroids[cent]
            for(size_t i = 0; i < colLen; i++)
            {
                if(cnt != 0)
                {
                    vec[i] /= cnt;
                }
                centroids[cent].at(i) = vec[i];
            }
        }
    }
}

template<typename T>
T HsxKMeans<T>::defaultDistEclud(vector<T> &v1, vector<T> &v2)
{
    T sum = 0;
    size_t size = v1.size();
    for(size_t i = 0; i < size; i++)
    {
        sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return sum;
}

template<typename T>
void HsxKMeans<T>::setCentroidSelectMode(CentroidSelectModeType m)
{
    m_CentroidSelectMode = m;
}

template<typename T>
typename HsxKMeans<T>::tMinMax HsxKMeans<T>::getMinMax(int idx)
{
    T min, max;
    dataSet[0].at(idx) > dataSet[1].at(idx) ? (max = dataSet[0].at(idx), min = dataSet[1].at(idx)) : (max = dataSet[1].at(idx), min = dataSet[0].at(idx));

    for(int i = 2; i < rowLen; i++)
    {
        if(dataSet[i].at(idx) < min)
        {
            min = dataSet[i].at(idx);
        }
        else if(dataSet[i].at(idx) > max)
        {
            max = dataSet[i].at(idx);
        }
        else
        {
            continue;
        }
    }

    tMinMax tminmax(min, max);
    return tminmax;
}

template<typename T>
T HsxKMeans<T>::getClosestDist(std::vector<T> point, std::vector<std::vector<T> > centroids)
{
    T min_dist = std::numeric_limits<T>::infinity();
    for(int i = 0; i < centroids.size(); i++)
    {
        T dist = distEclud(centroids[i], point);
        if(dist < min_dist)
        {
            min_dist = dist;
        }
    }
    return min_dist;
}

template<typename T>
std::vector<std::vector<T> > HsxKMeans<T>::kppCenters(std::vector<std::vector<T> > data_set, int k)
{
    std::vector<std::vector<T>> cluster_centers;
    auto idx = std::rand() % data_set.size();
    cluster_centers.push_back(data_set[idx]);
    std::vector<T> distances(data_set.size(), 0);
    for(int it = 1; it < k; it++)
    {
        T total = 0.0;
        for(size_t i = 0; i < data_set.size(); i++)
        {
            distances[i] = getClosestDist(data_set[i], cluster_centers);
            total += distances[i];
        }
        total *= (T)std::rand() / RAND_MAX;
        for(size_t i = 0; i < data_set.size(); i++)
        {
            total -= distances[i];
            if(total > 0)
            {
                continue;
            }
            cluster_centers.push_back(data_set[i]);
            break;
        }
    }
    return cluster_centers;
}
template<typename T>
void HsxKMeans<T>::updateCent()
{
    centroids.clear();
    if(m_CentroidSelectMode == FIX)
    {
        auto step = dataSet.size() / k;
        for(int i = 0; i < k; i++)
        {
            centroids.push_back(dataSet[i * step]);
        }
    }
    else if(m_CentroidSelectMode == RAND_KPP)
    {
        centroids = kppCenters(dataSet, k);
    }
    else
    {
        randCent();
    }
}
template<typename T>
void HsxKMeans<T>::randCent()
{
    //init centroids
    vector<T> vec(colLen, 0);
    for(int i = 0; i < k; i++)
    {
        centroids.push_back(vec);
    }

    //set values by column
    srand(time(NULL));
    for(int j = 0; j < colLen; j++)
    {
        tMinMax tminmax = getMinMax(j);
        T rangeIdx = tminmax.Max - tminmax.Min;
        for(int i = 0; i < k; i++)
        {
            /* generate float data between 0 and 1 */
            centroids[i].at(j) = tminmax.Min + rangeIdx * (rand() / (T)RAND_MAX);
        }
    }
}
}

#endif // HSXKMEANS_H
