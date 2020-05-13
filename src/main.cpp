//
//  main.cpp
//  Two_Body_Problem
//
//  Created by Zackary Windham on 3/2/20.
//  Copyright © 2020 Zackary Windham. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <fstream>

#include "body.h"

using namespace std;

void rungeKutta4(const double& stepSize, Body& body1, Body& body2, const double& G);

int main(int argc, const char * argv[]) {
    
    ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        cerr << "Unable to open " << argv[1] << " for input." << endl;
        return 2;
    }
    
    //Get the units that this is using
    string dataLine;
    getline(inputFile, dataLine);
    stringstream iss1(dataLine);
    double G_SCALED, M, L, T;
    iss1 >> G_SCALED >> M >> L >> T;
    
    //Grab the parameters for the first body and initialize the first body
    getline(inputFile, dataLine);
    stringstream iss2(dataLine);
    double MASS_1, x1, y1, p1X, p1Y;
    iss2 >> MASS_1 >> x1 >> y1 >> p1X >> p1Y;
    
    Body body1 = Body(x1, y1, p1X, p1Y, MASS_1);
    
    //Do the same for the second body
    getline(inputFile, dataLine);
    stringstream iss3(dataLine);
    double MASS_2, x2, y2, p2X, p2Y;
    iss3 >> MASS_2 >> x2 >> y2 >> p2X >> p2Y;
    
    Body body2 = Body(x2, y2, p2X, p2Y, MASS_2);
    
    //Prepare the files for output of the bodies
    ofstream firstBody;
    ofstream secondBody;
    firstBody.open(argv[2], ofstream::out | ofstream::trunc);
    secondBody.open(argv[3], ofstream::out | ofstream::trunc);
    string xyline = "x , y";
    firstBody << xyline << endl;
    secondBody << xyline << endl;
    
    //Set parameters for orbit number and step size for the solver
    const double STEP_SIZE = 0.1;
    const int NUM_ORBITS = 100000;
    int orbitCount = 0;
    
    //Evolve the system using the solver
    while (orbitCount < NUM_ORBITS)
    {
        firstBody << body1 << endl;
        secondBody << body2 << endl;
        
        for (unsigned int i = 0; i < 100; ++i)
        {
            double lasty = body2.getPosition()[1];
            
            rungeKutta4(STEP_SIZE, body1, body2, G_SCALED);
            
            cout << i << endl;
            
            if ((body2.getPosition()[0] > 0) && (body2.getPosition()[1] > 0) && (lasty < 0))
            {
                ++orbitCount;
                cout << orbitCount << endl;
            }
        }
    }
    
    firstBody.close();
    secondBody.close();

    return 0;
}

vector<vector<double> > hamiltonian(const Body& body1, const Body& body2, const double& G)
{
    vector<double> d1 = {body1.getMomentum()[0] / body1.getMass(), body1.getMomentum()[1] / body1.getMass()};
    vector<double> d2 = {body2.getMomentum()[0] / body2.getMass(), body2.getMomentum()[1] / body2.getMass()};
    
    double difX = body2.getPosition()[0] - body1.getPosition()[0];
    double difY = body2.getPosition()[1] - body1.getPosition()[1];
    double r = G * pow(pow(difX, 2.0) + pow(difY, 2.0), 1.5);
    double top = body1.getMass() * body1.getMass();
    
    vector<double> dP1 = {top * difX / r, top * difY / r};
    vector<double> dP2 = {- top * difX / r, - top * difY / r};
    
    return {d1, d2, dP1, dP2};
}

vector<vector<double> > change(const Body& body1, const Body& body2, const double& G)
{
    return hamiltonian(body1, body2, G);
}

void rungeKutta4(const double& stepSize, Body& body1, Body& body2, const double& G)
{
    //Establish k1 for the RungeKutta algorithm, use the known derivatives from the Hamiltonian
    vector<vector<double> > change1 = change(body1, body2, G);
    vector<vector<double> > k1 = {{change1[0][0] * stepSize, change1[0][1] * stepSize}, {change1[1][0] * stepSize, change1[1][1] * stepSize}};
    vector<vector<double> > kP1 = {{change1[2][0] * stepSize, change1[2][1] * stepSize}, {change1[3][0] * stepSize, change1[3][1] * stepSize}};
    
    vector<vector<double> > k12 = {{k1[0][0] / 2, k1[0][1] / 2}, {k1[1][0] / 2, k1[1][1] / 2}};
    vector<vector<double> > kP12 = {{kP1[0][0] / 2, kP1[0][1] / 2}, {kP1[1][0] / 2, kP1[1][1] / 2}};
    
    Body bodyk11 = Body(body1.getPosition()[0] + k12[0][0], body1.getPosition()[1] + k12[0][1], body1.getMomentum()[0] + kP12[0][0], body1.getMomentum()[1] + kP12[0][1], body1.getMass());
    Body bodyk12 = Body(body2.getPosition()[0] + k12[1][0], body2.getPosition()[1] + k12[1][1], body2.getMomentum()[0] + kP12[1][0], body2.getMomentum()[1] + kP12[1][1], body2.getMass());
    
    //Establish k2 for the algorithm
    vector<vector<double> > change2 = change(bodyk11, bodyk12, G);
    vector<vector<double> > k2 = {{change2[0][0] * stepSize, change2[0][1] * stepSize}, {change2[1][0] * stepSize, change2[1][1] * stepSize}};
    vector<vector<double> > kP2 = {{change2[2][0] * stepSize, change2[2][1] * stepSize}, {change2[3][0] * stepSize, change2[3][1] * stepSize}};
    
    vector<vector<double> > k22 = {{k2[0][0] / 2, k2[0][1] / 2}, {k2[1][0] / 2, k2[1][1] / 2}};
    vector<vector<double> > kP22 = {{kP2[0][0] / 2, kP2[0][1] / 2}, {kP2[1][0] / 2, kP2[1][1] / 2}};
    
    Body bodyk21 = Body(body1.getPosition()[0] + k22[0][0], body1.getPosition()[1] + k22[0][1], body1.getMomentum()[0] + kP22[0][0], body1.getMomentum()[1] + kP22[0][1], body1.getMass());
    Body bodyk22 = Body(body2.getPosition()[0] + k22[1][0], body2.getPosition()[1] + k22[1][1], body2.getMomentum()[0] + kP22[1][0], body2.getMomentum()[1] + kP22[1][1], body2.getMass());
    
    //Establish k3
    vector<vector<double> > change3 = change(bodyk21, bodyk22, G);
    vector<vector<double> > k3 = {{change3[0][0] * stepSize, change3[0][1] * stepSize}, {change3[1][0] * stepSize, change3[1][1] * stepSize}};
    vector<vector<double> > kP3 = {{change3[2][0] * stepSize, change3[2][1] * stepSize}, {change3[3][0] * stepSize, change3[3][1] * stepSize}};
    
    Body bodyk31 = Body(body1.getPosition()[0] + k3[0][0], body1.getPosition()[1] + k3[0][1], body1.getMomentum()[0] + kP3[0][0], body1.getMomentum()[1] + kP3[0][1], body1.getMass());
    Body bodyk32 = Body(body2.getPosition()[0] + k3[1][0], body2.getPosition()[1] + k3[1][1], body2.getMomentum()[0] + kP3[1][0], body2.getMomentum()[1] + kP3[1][1], body2.getMass());
    
    //Establish k4
    vector<vector<double> > change4 = change(bodyk31, bodyk32, G);
    vector<vector<double> > k4 = {{change4[0][0] * stepSize, change4[0][1] * stepSize}, {change4[1][0] * stepSize, change4[1][1] * stepSize}};
    vector<vector<double> > kP4 = {{change4[2][0] * stepSize, change4[2][1] * stepSize}, {change4[3][0] * stepSize, change4[3][1] * stepSize}};
    
    //Modify k1, k2, k3, and k4 so that they are weighted correctly
    vector<vector<double> > k1F = {{k1[0][0] / 6, k1[0][1] / 6}, {k1[1][0] / 6, k1[1][1] / 6}};
    vector<vector<double> > kP1F = {{kP1[0][0] / 6, kP1[0][1] / 6}, {kP1[1][0] / 6, kP1[1][1] / 6}};
    
    vector<vector<double> > k2F = {{k2[0][0] / 3, k2[0][1] / 3}, {k2[1][0] / 3, k2[1][1] / 3}};
    vector<vector<double> > kP2F = {{kP2[0][0] / 3, kP2[0][1] / 3}, {kP2[1][0] / 3, kP2[1][1] / 3}};
    
    vector<vector<double> > k3F = {{k3[0][0] / 3, k3[0][1] / 3}, {k3[1][0] / 3, k3[1][1] / 3}};
    vector<vector<double> > kP3F = {{kP3[0][0] / 3, kP3[0][1] / 3}, {kP3[1][0] / 3, kP3[1][1] / 3}};
    
    vector<vector<double> > k4F = {{k4[0][0] / 6, k4[0][1] / 6}, {k4[1][0] / 6, k4[1][1] / 6}};
    vector<vector<double> > kP4F = {{kP4[0][0] / 6, kP4[0][1] / 6}, {kP4[1][0] / 6, kP4[1][1] / 6}};
    
    //Use the weighted k1, k2, k3, and k4 to create the steps
    vector<vector<double> > step = {{k1F[0][0] + k2F[0][0] + k3F[0][0] + k4F[0][0], k1F[0][1] + k2F[0][1] + k3F[0][1] + k4F[0][1]}, {k1F[1][0] + k2F[1][0] + k3F[1][0] + k4F[1][0], k1F[1][1] + k2F[1][1] + k3F[1][1] + k4F[1][1]}};
    
    vector<vector<double> > stepP = {{kP1F[0][0] + kP2F[0][0] + kP3F[0][0] + kP4F[0][0], kP1F[0][1] + kP2F[0][1] + kP3F[0][1] + kP4F[0][1]}, {kP1F[1][0] + kP2F[1][0] + kP3F[1][0] + kP4F[1][0], kP1F[1][1] + kP2F[1][1] + kP3F[1][1] + kP4F[1][1]}};
    
    //Add the step to the original position and momentum
    body1.setPosition(body1.getPosition()[0] + step[0][0], body1.getPosition()[1] + step[0][1]);
    body2.setPosition(body2.getPosition()[0] + step[1][0], body2.getPosition()[1] + step[1][1]);
    body1.setMomentum(body1.getMomentum()[0] + stepP[0][0], body1.getMomentum()[1] + stepP[0][1]);
    body2.setMomentum(body2.getMomentum()[0] + stepP[1][0], body2.getMomentum()[1] + stepP[1][1]);
}
