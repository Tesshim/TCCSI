/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SMSPSDMO.cpp
 * Author: marcelo
 * 
 * Created on 26 de Abril de 2017, 10:09
 */

#include "SMSPSDMO.h"

/*
 * Problema de máquina simples
 * Cada tarefa tem tempo de preparação que depende da sequência
 * Objetivo: minimizar a soma dos atrasos
 * http://ac.els-cdn.com/S2212827114006350/1-s2.0-S2212827114006350-main.pdf?_tid=a41b0bba-6adc-11e6-a30e-00000aacb362&acdnat=1472140836_e585a3d1ddbbc33df3a708d79a9b3bc9
 */
void SMSPSDMO::ExactTotalTardinessAndMakeSpan(SchedulingInstance Instance){
    try{
        int numJobs;
        
        numJobs = Instance.numJobs;
        
        //x_{i, j} \in \{0, 1\} \forall i = 0, \dots, n-1, j = 0, \dots, n - 1
        GRBVar x[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                //x_{i,j} indica se a tarefa j é executada imediatamente
                //após a tarefa i
                x[i][j] = model.addVar(0, 1, 1, GRB_BINARY, "x[" + itos(i) + "][" + itos(j) + "]");
            }
        }
        
        GRBVar T[numJobs+1], C[numJobs+1];
        for (int j = 0; j <= numJobs; j++) {
            T[j] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "T[" + itos(j) + "]");
            C[j] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "C[" + itos(j) + "]");
        }
        
        
        
        // Integrate new variables
        model.update();
        
        //Objetivo
        //obj1 => min \sum_{j=1}^{n}(T_j)
        //obj2 => min max(C_j)
        
        /*--------------------------------*/
        
        // Variable to represent the total tardines
        GRBVar totalTardines = model.addVar(0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, 
                "totTardines");
        
        GRBLinExpr le = 0;
        for (int j = 1; j <= numJobs; j++) {
            le += T[j];
        }
        
        model.addConstr(le == totalTardines, "totalTardines");
        
        // Variable to represent the max completion time
        GRBVar makeSpan = model.addVar(0, GRB_INFINITY, 0, GRB_CONTINUOUS, 
                "makeSpan");
        
        //C_max >= C_j ∀j ∈ N
        /*for (int j = 1; j <= numJobs; j++) {
            model.addConstr(makeSpan, GRB_GREATER_EQUAL, 45454, "CMAX[" + itos(j) + "]") ;
        }*/

        // Set global sense for ALL objectives
        model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);

        // Set number of objectives
        model.set(GRB_IntAttr_NumObj, 2);

        // Set and configure objective 0
        model.set(GRB_IntParam_ObjNumber, 0);
        model.set(GRB_IntAttr_ObjNPriority, 2);
        //model.set(GRB_DoubleAttr_ObjNWeight, 1.0);
        model.set(GRB_StringAttr_ObjNName, "totalTardinesObj");
        //model.set(GRB_DoubleAttr_ObjNRelTol, 0.0);
        //model.set(GRB_DoubleAttr_ObjNAbsTol, 0.0);
        totalTardines.set(GRB_DoubleAttr_ObjN, 1.0);

        
        model.addGenConstrMax(makeSpan, C, Instance.numJobs, -GRB_INFINITY, "makeSpan");

        //set and configure objective 1
        model.set(GRB_IntParam_ObjNumber, 1);
        model.set(GRB_IntAttr_ObjNPriority, 1);
        //model.set(GRB_DoubleAttr_ObjNWeight, 1.0);
        model.set(GRB_StringAttr_ObjNName, "makeSpanObj");
        makeSpan.set(GRB_DoubleAttr_ObjN, 1.0);
        
        /*----------------------------*/
        
        // Restrições
        
        //R1
        //Cada tarefa j tem apenas uma tarefa anterior
        //  \sum_{i=0}^{n-1} x_{i,j} = 1, \forall j = 1, \dots, n
        for (int j = 0; j <= numJobs; j++) {
            GRBLinExpr quantSeq = 0;
            for (int i = 0; i <= numJobs; i++) {
                quantSeq += x[i][j];
            }
            model.addConstr(quantSeq, GRB_EQUAL, 1, "Sequencia" + itos(j));
        }
        
        //R2
        //Cada tarefa i tem apenas uma tarefa posterior
        //\sum_{j=1}^{n} x_{i,j} = 1, \forall i = 0, \dots, n - 1
        for (int i = 0; i <= numJobs; i++) {
            GRBLinExpr quantExec = 0;
            for (int j = 0; j <= numJobs; j++) {
                quantExec += x[i][j];
            }
            model.addConstr(quantExec, GRB_EQUAL, 1, "Execucao" + itos(i));
        }
        
        //R3
        //Uma tarefa j não pode ser a tarefa anterior nem posterior dela mesma
        //x_{j,j} = 0, \forall j=0, 2, \dots, n - 1
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(x[j][j], GRB_EQUAL, 0, "SeqProp" + itos(j));
        }
        
        
        //R4
        //Tempo de término da tarefa zero é igual a zero
        //C_0 = 0
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;
        model.addConstr(T[0], GRB_EQUAL, 0, "T[0]") ;

        
        //R5
        //Tempo de término da tarefa j
        //C_j >= C_i + s_{i, j} + p_j − (1 − x_{i, j})M
        //\forall i = 0, ..., n, j = 1, ..., n + 1

        //C[0]
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;

        //i = 0
        GRBLinExpr exp = 0;
        for (int j = 1; j <= numJobs; j++) {
            for (int i = 0; i <= 0; i++) {
                //if(i != j)
                {
                    exp = Instance.processingTime[j-1] 
                            - (1 - x[i][j])*this->up ;
                    model.addConstr(C[j], GRB_GREATER_EQUAL, exp, "C[" + itos(j) + "]") ;
                    //cout << endl <<"C[" << j << "] >= " << exp << endl;
                }
            }
        }
        
        //i = {1..n}
        for (int j = 0; j < numJobs; j++) {
            for (int i = 0; i < numJobs; i++) {
                //if(i != j)
                {
                    //cout << "Instance.family[i]: " << Instance.family[i]
                    //        << "\tInstance.family[j]: " << Instance.family[j] << endl;
                    exp = C[i+1] + Instance.processingTime[j] 
                            + Instance.setupTime[Instance.family[i]][Instance.family[j]] 
                            - (1 - x[i+1][j+1])*this->up ;
                    model.addConstr(C[j+1], GRB_GREATER_EQUAL, exp, "C[" + itos(j+1) + "]") ;
                    //cout << endl <<"C[" << j+1 << "] >= " << exp << endl;
                }
            }
        }
        
        //R6
        //Atraso da tarefa j
        //T_j ≥ C_j − d_j \forall j = 1, ..., n
        GRBLinExpr ti;
        for (int i = 1; i <= numJobs; i++) {
            ti = C[i] - Instance.dueDate[i-1];
            model.addConstr(T[i], GRB_GREATER_EQUAL, ti, "T[" + itos(i) + "]") ;
            //cout << endl <<"T[" << i << "] >= " << ti << endl;
        }
        
        //R7
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(C[j], GRB_GREATER_EQUAL, 0) ;
            model.addConstr(T[j], GRB_GREATER_EQUAL, 0) ;
        }
        
        // Optimize model
        model.optimize();

        /*for(int i=0;i<=numJobs;++i){
            cout << C[i].get(GRB_StringAttr_VarName) << " " << C[i].get(GRB_DoubleAttr_X) << endl;
        }
        
        for(int i=0;i<=numJobs;++i){
            cout << T[i].get(GRB_StringAttr_VarName) << " " << T[i].get(GRB_DoubleAttr_X) << endl;
        }

        cout << "Atraso total: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        
        cout << "Matriz:" << endl;
        int solucao[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {                
                solucao[i][j] = round(x[i][j].get(GRB_DoubleAttr_X));
                cout << solucao[i][j] << "\t";
            }
            cout << endl;
        }
        
        int sequencia[numJobs];
        cout << "Sequencia: ";
        for (int i = 0; i < numJobs; i++) {
            for (int j = 0; j < numJobs; j++) {
                if(solucao[i][j] == 1){
                    sequencia[i] = j;
                    cout << sequencia[i] << "\t";
                }
            }
        }
        
        cout << endl;
        */
        
        cout << "Atraso total: " << totalTardines.get(GRB_DoubleAttr_X) << endl;
        cout << "Makespan: " << makeSpan.get(GRB_DoubleAttr_X) << endl;
        
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
    
}

