#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    // TODO: Implement de Casteljau's algorithm

    auto points = control_points;

    while(points.size() != 1)
    {
        std::vector<cv::Point2f> next_points;
        for(int i = 0; i < points.size() - 1; i++)
        {
            auto& p1 = points[i];
            auto& p2 = points[i+1];
            auto d = p2 - p1;
            auto m = p1 + t * d;
            next_points.push_back(m);
        }
        points = next_points;
    }
    return points.front();
}


void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    float step = 0.001;
    float t = step;
    while(t < 1.f)
    {
        auto p = recursive_bezier(control_points, t);


        int minx = std::floor(p.x);
        int miny = std::floor(p.y);

        cv::Point2f p00(minx, miny);
        cv::Point2f p10(minx+1, miny);
        cv::Point2f p01(minx, miny+1);
        cv::Point2f p11(minx+1, miny+1);
        cv::Point2f cp(std::round(p.x), std::round(p.y));

        float md = cv::norm(p-cp);

        for(auto& pt: {p00, p10, p01, p11})
        {
            float d = cv::norm(pt-p);
            float ratio = md/d;


            cv::Vec3b& color = window.at<cv::Vec3b>(pt.y, pt.x);
            color[1] = std::max(color[1], static_cast<uchar>(255* ratio));
        }

//        window.at<cv::Vec3b>(p.y, p.x)[1] = 255;
        t+=step;
    }
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4) 
        {
//            naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
