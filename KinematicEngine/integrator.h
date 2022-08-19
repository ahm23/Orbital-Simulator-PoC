#pragma once
#include <valarray>
#include <iostream>
#include <Eigen/Dense>
#include <vector>

const size_t DIM = 2;

typedef std::valarray<Eigen::Vector3d> vec;



Eigen::Vector3d solve(Eigen::Vector3d initial, double dt, double t, Eigen::Vector3d(*f) (double, Eigen::Vector3d, std::vector<void*>), std::vector<void*> f_params) {
    //Eigen::Vector3d blank = Eigen::Vector3d::Zero();
    Eigen::Vector3d k1 = f(t, initial, f_params);
    Eigen::Vector3d k2 = f(t, initial.array() + k1.array() + dt / 2.0, f_params);
    Eigen::Vector3d k3 = f(t, initial.array() + k2.array() + dt / 2.0, f_params);
    Eigen::Vector3d k4 = f(t, initial.array() + k3.array() + dt, f_params);

    Eigen::Vector3d dr = dt / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    return dr;
}
/*
std::vector<Eigen::Vector3d> solve2(std::vector<Eigen::Vector3d> initial, double dt, double t, std::vector<Eigen::Vector3d>(*f) (double, std::vector<Eigen::Vector3d>, std::vector<Eigen::Vector3d>, double, std::vector<void*>), std::vector<void*> f_params) {
    std::vector<Eigen::Vector3d> blank = {};
    std::vector<Eigen::Vector3d> k1 = f(t, initial, blank, 0, f_params);
    std::vector<Eigen::Vector3d> k2 = f(t, initial, k1, dt/2.0, f_params);
    std::vector<Eigen::Vector3d> k3 = f(t, initial, k2, dt/2.0, f_params);
    std::vector<Eigen::Vector3d> k4 = f(t, initial, k3, dt, f_params);

    //vec X(init, DIM);
    //double t = 0.;
    //double tMax = 5.;
    // Eigen::Vector3d k1 = pair(t, X, dv);
    //vec k2 = pair(t + dt / 2.0, initial + dt / 2.0 * k1, dv);
    //vec k3 = pair(t + dt / 2.0, X + dt / 2.0 * k2, dv);
    //vec k4 = pair(t + dt, X + dt * k3, dv);

    initial += dt / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    return initial;
}*/