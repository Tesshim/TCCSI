/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SMSP.h
 * Author: marcelo
 *
 * Created on 29 de Agosto de 2016, 22:40
 */

#ifndef SMSP_H
#define SMSP_H

#include "/arquivos/Professor/Pesquisa/Gurobi/gurobi702/linux64/include/gurobi_c++.h"
#include "SchedulingInstance.h"
#include "Utils.h"
#include "SchedulingInstance.h"
#include <math.h>



using namespace std;

class SMSP {
public:
    // Create environment
    GRBEnv env = GRBEnv();

    // Create a new model
    GRBModel model = GRBModel(env);
    
    double up = 1000;
    
    void ExactTotalCompletionTime(SchedulingInstance Instance);
    void ExactTotalTardiness(SchedulingInstance Instance);
    void ExactMaximumTardiness(SchedulingInstance Instance);
    void ExactTotalEarlinessTardiness(SchedulingInstance Instance);
    void ExactNumberTardyJobs(SchedulingInstance Instance);
    void ExactMaximumLateness(SchedulingInstance Instance);
private:

};

#endif /* SMSP_H */

