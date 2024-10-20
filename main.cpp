#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <opencv2/opencv.hpp>

#include "help.h"

#define dbg(x) std::cout<<#x<<": "<<x<<std::endl

std::vector<Point> grahamsScan(std::vector<Point> points){
    std::sort(points.begin(),points.end(),[] (Point a,Point b){
        return a.x<b.x;
    });

    if(points.size() < 3)
        return points;

    //calculate upper hull
    std::vector<Point> upper_hull;
    auto point_iter = points.begin();

    upper_hull.push_back(*point_iter++);
    upper_hull.push_back(*point_iter++);

    while (++point_iter != points.end())
    {
        upper_hull.push_back(*point_iter);
        while (upper_hull.size() >= 3 && !is_strictly_to_right_of(upper_hull[upper_hull.size()-3],upper_hull[upper_hull.size()-2],*point_iter))
        {
            upper_hull.erase(upper_hull.begin() + upper_hull.size()-2);
        }
    }

    std::vector<Point> lower_hull;
    point_iter = points.begin();

    lower_hull.push_back(*point_iter++);
    lower_hull.push_back(*point_iter++);

    while (++point_iter != points.end())
    {
        lower_hull.push_back(*point_iter);
        while (lower_hull.size() >= 3 && !is_strictly_to_left_of(lower_hull[lower_hull.size()-3],lower_hull[lower_hull.size()-2],*point_iter))
        {
            lower_hull.erase(lower_hull.begin() + lower_hull.size()-2);
        }
    }

    //merge the two hulls
    auto hull = upper_hull; //kinda unneccesary
    for (auto iter = lower_hull.end()-1; iter!=lower_hull.begin(); iter--){
        hull.push_back(*iter);
    }

    return hull;
}

std::vector<Point> hull_bf(std::vector<Point> points){
    std::vector<Point> hull;
    bool include = true;
    for(auto p0:points){
        for(auto p1:points){
            if(p0 == p1)
                continue;
            include = true;
            for(auto p2:points){
                if(p0 == p2 || p1 == p2)
                    continue; //must be distinct
                if(!is_strictly_to_right_of(p0,p1,p2))
                    include = false;
            }
            if(include)
                hull.push_back(p1);
        }
    }
    return hull;
}

//calculates the Hull of a set of points.
std::pair<std::vector<Point>::iterator,std::vector<Point>::iterator> upperTagent(std::vector<Point>::iterator begin,std::vector<Point>::iterator split_plane,std::vector<Point>::iterator end){
    auto current_left = std::max_element(begin,split_plane,[](Point rhs,Point lhs){
        return rhs.x < lhs.x;
    });
    auto current_right = std::max_element(split_plane,end,[](Point rhs,Point lhs){
        return rhs.x > lhs.x;
    });
    while (true)
    // {
        // if(!left_valid){
            // current_left--;
        // } else if(!right_valid){
            // current_right++;
        // } else break;
    // }
    return std::pair<std::vector<Point>::iterator,std::vector<Point>::iterator>{current_left,current_right};
}

void hull_recursive(std::vector<Point>::iterator& begin,std::vector<Point>::iterator& end){
    int size = begin-end;
    if (size <= 1) return; //break recursion

    Point avg_point = std::reduce(begin,end)/size;

    auto split_plane = std::partition(begin,end,[&](Point p){
        return p.x < avg_point.x || (p.x == avg_point.x && p.y < avg_point.y);
    });
}

std::vector<Point> hull_recursive(std::vector<Point> points){
    auto begin = points.begin();
    auto end = points.end();
    hull_recursive(begin,end);
    std::vector<Point> hull;
    do{
        hull.push_back(*begin);
    }while(begin++ != end);
    return hull;
}

int main(){
    //initialize points
    std::vector<Point> points;
    points.push_back(Point{0,5});
    points.push_back(Point{1,2});
    points.push_back(Point{2,6});
    points.push_back(Point{3,3});
    points.push_back(Point{4,9});
    points.push_back(Point{5,12});
    points.push_back(Point{6,0});
    points.push_back(Point{7,10});
    points.push_back(Point{8,1});

    auto hull = grahamsScan(points);
    //auto hull = hull_bf(points);
    //auto hull = hull_recursive(points);

    visualize_results(points,hull);
    return 0;
}