//
//  body.cpp
//  Two_Body_Problem
//
//  Created by Zackary Windham on 5/11/20.
//  Copyright © 2020 Zackary Windham. All rights reserved.
//

#include "body.h"

Body::Body(): mass(0), position({0, 0}), momentum({0, 0}) {}

Body::Body(const double& x, const double& y, const double& px, const double& py, const double& m): position({x, y}), momentum({px, py}), mass(m) {}

double Body::getMass() const
{
    return mass;
}

std::vector<double> Body::getPosition() const
{
    return position;
}

std::vector<double> Body::getMomentum() const
{
    return momentum;
}

void Body::setPosition(double x, double y)
{
    position = {x , y};
}

void Body::setMomentum(double px, double py)
{
    momentum = {px, py};
}

std::string Body::toString() const
{
    std::stringstream os;
    os << position[0] << ", " << position[1];
    
    return os.str();
}