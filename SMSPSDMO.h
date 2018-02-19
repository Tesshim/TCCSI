/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SMSPSDMO.h
 * Author: marcelo
 *
 * Created on 26 de Abril de 2017, 10:09
 */

#ifndef SMSPSDMO_H
#define SMSPSDMO_H

#include "/arquivos/Professor/Pesquisa/Gurobi/gurobi702/linux64/include/gurobi_c++.h"
#include "SchedulingInstance.h"
#include "Utils.h"
#include "SchedulingInstance.h"
#include <math.h>

using namespace std;

class SMSPSDMO {
public:
    // Create environment
    GRBEnv env = GRBEnv();

    // Create a new model
    GRBModel model = GRBModel(env);
    
    double up = 100000;
    
    void ExactTotalTardinessAndMakeSpan(SchedulingInstance Instance);
private:

};

#endif /* SMSPSDMO_H */

