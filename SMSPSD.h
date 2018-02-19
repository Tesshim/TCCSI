/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SMSPSD.h
 * Author: marcelo
 *
 * Created on 29 de Agosto de 2016, 22:53
 */

#ifndef SMSPSD_H
#define SMSPSD_H

#include "/arquivos/Professor/Pesquisa/Gurobi/gurobi702/linux64/include/gurobi_c++.h"
#include "SchedulingInstance.h"
#include "Utils.h"
#include "SchedulingInstance.h"
#include <math.h>

using namespace std;

class SMSPSD {
public:
    // Create environment
    GRBEnv env = GRBEnv();

    // Create a new model
    GRBModel model = GRBModel(env);
    
    double up = 100000;
    
    void ExactMakeSpan(SchedulingInstance Instance);
    void ExactTotalTardiness(SchedulingInstance Instance);
    void ExactTotalTardinessRelax(SchedulingInstance Instance);
private:

};

#endif /* SMSPSD_H */

