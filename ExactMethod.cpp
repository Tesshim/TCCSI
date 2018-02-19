/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExactMethod.cpp
 * Author: marcelo
 * 
 * Created on 19 de Agosto de 2016, 20:44
 */

#include <math.h>

#include "ExactMethod.h"


ExactMethod::ExactMethod() {
}

ExactMethod::ExactMethod(const ExactMethod& orig) {
}

ExactMethod::~ExactMethod() {
}

void ExactMethod::HelloWorld(){
    
    try{
        // Create variables
        GRBVar x = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "x");
        GRBVar y = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, "y");

        // Integrate new variables
        model.update();

        // Set objective
        model.setObjective(3000*x + 4000*y, GRB_MINIMIZE);


        // Add constraint: 5x + 6y >= 10
        model.addConstr(5*x + 6*y >= 10, "c0");

        // Add constraint: 7x + 5y >= 5
        model.addConstr(7*x + 5*y >= 5, "c1");

        // Add constraint:  x, y >= 0
        model.addConstr(x >= 0, "c2");
        model.addConstr(y >= 0, "c3");

        // Optimize model
        model.optimize();

        cout << x.get(GRB_StringAttr_VarName) << " = " << x.get(GRB_DoubleAttr_X) << endl;
        cout << y.get(GRB_StringAttr_VarName) << " = " << y.get(GRB_DoubleAttr_X) << endl;

        cout << "Objetivo: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}



