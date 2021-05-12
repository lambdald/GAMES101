#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>

#include <eigen3/Eigen/Geometry>
#include<iostream>

int main(){

    Eigen::Vector2f pt(2,1);

    Eigen::Vector3f P(2,1,1);
    Eigen::Matrix3f M = Eigen::Matrix3f::Identity();

    float rad = 45.0f * M_PI / 180.f;
    Eigen::Rotation2Df rot(rad);    // rotation matrix.
    Eigen::Vector2f tvec(1, 2); // translation vector.

    // [R t; 0, 1]
    M.topRows(2).leftCols(2) = rot.matrix();
    M.col(2).topRows(2) = tvec;
    Eigen::Vector3f P2 = M * P;
    std::cout<<P2<<std::endl;
    return 0;
}