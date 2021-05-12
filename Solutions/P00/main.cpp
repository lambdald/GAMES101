#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>

#include <eigen3/Eigen/Geometry>
#include<iostream>

Eigen::Vector2f transform2d(const Eigen::Vector2f& pt, const Eigen::Matrix3f& trans) {
    Eigen::Vector3f pt_homo(pt.x(), pt.y(), 1.f);
    pt_homo = trans * pt_homo;

    pt_homo /= pt_homo.z();
    return {pt_homo.x(), pt_homo.y()};
}
int main(){

    Eigen::Vector2f pt(2,1);

    Eigen::Vector3f P(2,1,1);
    Eigen::Matrix3f M = Eigen::Matrix3f::Identity();

    float rad = 45.0f * M_PI / 180.f;
    Eigen::Rotation2Df rot(rad);
    Eigen::Vector2f tvec(1, 2);

    M.topRows(2).leftCols(2) = rot.matrix();
    M.col(2).topRows(2) = tvec;
    Eigen::Vector3f P2 = M * P;
    std::cout<<P2<<std::endl;
    return 0;
}