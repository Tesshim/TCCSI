/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExactMethod.h
 * Author: marcelo
 *
 * Created on 19 de Agosto de 2016, 20:44
 */

#ifndef EXACTMETHOD_H
#define EXACTMETHOD_H

#include "/arquivos/Professor/Pesquisa/Gurobi/gurobi702/linux64/include/gurobi_c++.h"
#include "SchedulingInstance.h"


using namespace std;

class ExactMethod {
public:
    // Create environment
    GRBEnv env = GRBEnv();

    // Create a new model
    GRBModel model = GRBModel(env);
    ExactMethod();
    ExactMethod(const ExactMethod& orig);
    virtual ~ExactMethod();
    void HelloWorld();
    
private:

};

#endif /* EXACTMETHOD_H */

