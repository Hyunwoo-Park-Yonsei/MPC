/*
 *    This file is part of ACADO Toolkit.
 *
 *    ACADO Toolkit -- A Toolkit for Automatic Control and Dynamic Optimization.
 *    Copyright (C) 2008-2014 by Boris Houska, Hans Joachim Ferreau,
 *    Milan Vukov, Rien Quirynen, KU Leuven.
 *    Developed within the Optimization in Engineering Center (OPTEC)
 *    under supervision of Moritz Diehl. All rights reserved.
 *
 *    ACADO Toolkit is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    ACADO Toolkit is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with ACADO Toolkit; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */



 /**
 *    \file   examples/ocp/time_optimal_rocket.cpp
 *    \author Boris Houska, Hans Joachim Ferreau
 *    \date   2009
 */


#include <acado_optimal_control.hpp>
#include <acado_gnuplot.hpp>
#include <iostream>

int main( ){

    USING_NAMESPACE_ACADO


    // INTRODUCE THE VARIABLES:
    // -------------------------



    DifferentialState x;
    DifferentialState y;
    DifferentialState v;
    DifferentialState phi;
    DifferentialState delta;
    // AlgebraicState H;
    DifferentialState H;



    Control a;
    Control deltarate;



    DifferentialEquation  f;

    double L = 2.875;

    // DEFINE A DIFFERENTIAL EQUATION:
    // -------------------------------

    f << dot(x) == v*cos(phi);
    f << dot(y) == v*sin(phi);
    f << dot(v) == a;
    f << dot(phi) == v*tan(delta)/L;
    f << dot(delta) == deltarate;
//    f << H == sqrt((y-1)*(y-1));

//    f << 0 ==  -H + T;
    f << dot(H) ==  sqrt((y-1)*(y-1));


    // DEFINE AN OPTIMAL CONTROL PROBLEM:
    // ----------------------------------
    const int N  = 50;
    const double EndTime = 5;

    OCP ocp(0, EndTime, N);


    ocp.minimizeMayerTerm( H );
    ocp.subjectTo( f );

    ocp.subjectTo( AT_START, x ==  0.0 );
    ocp.subjectTo( AT_START, y ==  3.0 );
    ocp.subjectTo( AT_START, v ==  1.0 );
    ocp.subjectTo( AT_START, phi ==  0.0 );
    ocp.subjectTo( AT_START, delta ==  0.0 );
    ocp.subjectTo( AT_START, H ==  0.0 );


    ocp.subjectTo( AT_END, x ==  10.0 );
    ocp.subjectTo( AT_END, y ==  1.0 );


    ocp.subjectTo( -1.0 <= a <= 1.0 );
    ocp.subjectTo( -M_PI/2 <= delta <= M_PI/2 );
    ocp.subjectTo( -M_PI/4 <= deltarate <= M_PI/4 );

    // VISUALIZE THE RESULTS IN A GNUPLOT WINDOW:
    // ------------------------------------------
    GnuplotWindow window;
        window.addSubplot( x, "THE POSITION x");
        window.addSubplot( y, "THE POSITION y");
        window.addSubplot( v, "THE VELOCITY v");
        window.addSubplot( phi, "THE Angle phi");
        window.addSubplot( delta, "THE STEER delta");
        window.addSubplot( deltarate, "THE STEER deltarate");
        window.addSubplot( a, "THE ACCELERATION a");
        window.addSubplot( H, "THE COST FUNCTION H");




    // DEFINE AN OPTIMIZATION ALGORITHM AND SOLVE THE OCP:
    // ---------------------------------------------------
    OptimizationAlgorithm algorithm(ocp);

    algorithm.set( MAX_NUM_ITERATIONS, 100 );
    algorithm.set( ABSOLUTE_TOLERANCE, 1e-1);
    algorithm.set( INTEGRATOR_TOLERANCE, 1e-1);

    // algorithm.set( INTEGRATOR_TYPE , INT_RK78);

    // algorithm.set( DISCRETIZATION_TYPE, SINGLE_SHOOTING);
    algorithm.set( KKT_TOLERANCE, 1e-2);
//    algorithm.set( HESSIAN_APPROXIMATION, EXACT_HESSIAN );
// 	algorithm.set( HESSIAN_PROJECTION_FACTOR, 1.0 );
//    algorithm.set(USE_IMMEDIATE_FEEDBACK, YES);
//    algorithm.set(USE_REALTIME_ITERATIONS, YES);
    // algorithm.set(HESSIAN_APPROXIMATION, GAUSS_NEWTON);
    algorithm << window;


//     algorithm.initializeDifferentialStates("tor_states.txt");
//     algorithm.initializeParameters("tor_pars.txt");
//     algorithm.initializeControls("tor_controls.txt");

    algorithm.solve();

//     algorithm.getDifferentialStates("tor_states.txt");
//     algorithm.getParameters("tor_pars.txt");
//     algorithm.getControls("tor_controls.txt");

    return 0;
}



