struct Point{
    int x;
    int y;

    bool operator==(const Point& rhs)
    {
        return this->x == rhs.x && this->y == rhs.y;
    }
    bool operator!=(const Point& rhs){
        return !(*this==rhs);
    }
    Point operator+(const Point& rhs){
        return Point{x+rhs.x,y+rhs.y};
    }
    Point operator/(const int& devisor){
        return Point{x/devisor,y/devisor};
    }
};

void print_point_vec(std::vector<Point> vec){
    for(auto iter = vec.begin();iter != vec.end();++iter){
        std::cout << "x: " <<iter->x << ", y: " << iter->y << '\n';
    }
}

inline int parallelepipedprodukt(Point &s, Point &e, Point &p){
    return -e.y*p.x+e.x*p.y+e.y*s.x-p.y*s.x-e.x*s.y+p.x*s.y;
}

bool is_strictly_to_right_of(Point start_point, Point end_point, Point to_test){
    return parallelepipedprodukt(start_point,end_point,to_test)>0;
}

bool is_strictly_to_left_of(Point start_point, Point end_point, Point to_test){
    return parallelepipedprodukt(start_point,end_point,to_test)<0;
}

void visualize_results(std::vector<Point> points,std::vector<Point> hull){
    //find range
    auto range_x = std::minmax_element(points.begin(),points.end(),[] (Point a,Point b){
        return a.x < b.x;
    });
    auto range_y = std::minmax_element(points.begin(),points.end(),[] (Point a,Point b){
        return a.y < b.y;
    });

    int min_x = range_x.first->x;
    int max_x = range_x.second->x;

    int min_y = range_y.first->y;
    int max_y = range_y.second->y;

    cv::Size imgSize(
        (max_x-min_x)+1,
        (max_y-min_y)+1);

    cv::Mat img(imgSize, CV_8UC3, cv::Scalar(255, 255, 255));
    for (Point p:points)
    {
        auto &pixel = img.at<cv::Vec3b>(p.y - min_y, p.x - min_x);
        pixel[0] = 0;
        pixel[1] = 0;
        pixel[2] = 0;
    }
    for (Point p:hull){
        auto &pixel = img.at<cv::Vec3b>(p.y - min_y, p.x - min_x);
        pixel[0] = 0;
        pixel[1] = 255;
        pixel[2] = 0;
    }
    cv::imwrite("../point_visualization.png", img);
}