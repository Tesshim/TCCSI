/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SMSP.cpp
 * Author: marcelo
 * 
 * Created on 29 de Agosto de 2016, 22:40
 */

#include "SMSP.h"

/*
 * Problema de máquina simples
 * não tem tempo de preparação
 * Objetivo: minimizar a soma dos tempo de términos
 */
void SMSP::ExactTotalCompletionTime(SchedulingInstance Instance){
    try{
        int numJobs = Instance.numJobs;
        
        GRBVar x[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                x[i][j] = model.addVar(0, 1, 1, GRB_BINARY, "x[" + itos(i) + "][" + itos(j) + "]");
            }
        }
        
        GRBVar C[numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            C[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "C[" + itos(i) + "]");
        }
        
        // Integrate new variables
        model.update();
        
        //Objetivo
        //[fo] min = @sum(Tarefas(i) | i #NE# 1: C(i));
        GRBLinExpr somaTempoTermino = 0;
        for (int i = 1; i <= numJobs; i++) {
            somaTempoTermino += C[i];
        }
        model.setObjective(somaTempoTermino, GRB_MINIMIZE);
        
        // Restrições
        //i é predecessora de j
        //garante que i seja predecessora de apenas uma tarefa j
        //(@for(Tarefas(j):
        //  @sum(Tarefas(i) | i #NE# j: x(i,j)) = 1);
        for (int j = 0; j <= numJobs; j++) {
            GRBLinExpr quantSeq = 0;
            for (int i = 0; i <= numJobs; i++) {
                if(i != j){
                    quantSeq += x[i][j];
                }
            }
            model.addConstr(quantSeq, GRB_EQUAL, 1, "Sequencia" + itos(j));
        }
        
        //j é sucessora de i
        //garante que j seja sucessora de apenas uma tarefa i
        //@for(Tarefas(i):
        //Para cada tarefa i tem apenas uma tarefa imediatamente sucessora
        //  @sum(Tarefas(j) | j #NE# i: x(i,j)) = 1);
        for (int i = 0; i <= numJobs; i++) {
            GRBLinExpr quantExec = 0;
            for (int j = 0; j <= numJobs; j++) {
                if(i != j){
                    quantExec += x[i][j];
                }
            }
            model.addConstr(quantExec, GRB_EQUAL, 1, "Execucao" + itos(i));
        }
        
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(x[j][j], GRB_EQUAL, 0, "Sequencia" + itos(j));
        }
        
        //@for(Tarefas(j) | j #NE# 1:
        //  @for(Tarefas(i):
        //      C(j) >= C(i) - M + (p(j) + M)*x(i,j)));
        GRBLinExpr cj;
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;
        for (int j = 1; j <= numJobs; j++) {
            for (int i = 0; i <= numJobs; i++) {
                cj = C[i] - this->up + (Instance.processingTime[j-1] + this->up)*x[i][j];
                model.addConstr(C[j], GRB_GREATER_EQUAL, cj, "C[" + itos(j) + "][" + itos(i) + "]") ;
            }
        }
        
        // Optimize model
        model.optimize();

        cout << "Soma dos tempo de términos: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        
        cout << "Matriz:" << endl;
        int solucao[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                solucao[i][j] = round(x[i][j].get(GRB_DoubleAttr_X));
                cout << solucao[i][j] << " \t";
            }
            cout << endl;
        }
        
        int sequencia[numJobs];
        for (int j = 0; j <= numJobs; j++) {
            if(solucao[0][j] == 1){
                sequencia[0] = j;
            }
        }
        cout << "Sequencia: ";
        cout << sequencia[0] << "\t";
        for (int i = 1; i < numJobs; i++) {
            for (int j = 0; j < numJobs; j++) {
                if(solucao[sequencia[i-1]][j] == 1){
                    sequencia[i] = j;
                    cout << sequencia[i] << "\t";
                }
            }
        }
        cout << endl;
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}

/*
 * Problema de máquina simples
 * não tem tempo de preparação
 * Objetivo: minimizar a soma dos atrasos
 */
void SMSP::ExactTotalTardiness(SchedulingInstance Instance){
    try{
        int numJobs = Instance.numJobs;
        
        GRBVar x[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                x[i][j] = model.addVar(0, 1, 1, GRB_BINARY, "x[" + itos(i) + "][" + itos(j) + "]");
            }
        }
        
        GRBVar T[numJobs+1], C[numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            T[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "T[" + itos(i) + "]");
            C[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "C[" + itos(i) + "]");
        }
        
        // Integrate new variables
        model.update();
        
        //Objetivo
        //min soma(T_i)
        GRBLinExpr atrasoTotal = 0;
        for (int i = 1; i <= numJobs; i++) {
            atrasoTotal += T[i];
        }
        model.setObjective(atrasoTotal, GRB_MINIMIZE);
        
        // Restrições
        //S1
        //(@for(Tarefas(j):
        //  @sum(Tarefas(i) | i #NE# j: x(i,j)) = 1);
        for (int j = 0; j <= numJobs; j++) {
            GRBLinExpr quantSeq = 0;
            for (int i = 0; i <= numJobs; i++) {
                if(i != j){
                    quantSeq += x[i][j];
                }
            }
            model.addConstr(quantSeq, GRB_EQUAL, 1, "Sequencia" + itos(j));
        }
        
        //S2
        //@for(Tarefas(i):
        //  @sum(Tarefas(j) | j #NE# i: x(i,j)) = 1);
        for (int i = 0; i <= numJobs; i++) {
            GRBLinExpr quantExec = 0;
            for (int j = 0; j <= numJobs; j++) {
                if(i != j){
                    quantExec += x[i][j];
                }
            }
            model.addConstr(quantExec, GRB_EQUAL, 1, "Execucao" + itos(i));
        }
        
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(x[j][j], GRB_EQUAL, 0, "Sequencia" + itos(j));
        }
        
        //S3
        //@for(Tarefas(j) | j #NE# 1:
        //  @for(Tarefas(i):
        //      C(j) >= C(i) - M + (p(j) + M)*x(i,j)));
        GRBLinExpr cj;
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;
        for (int j = 1; j <= numJobs; j++) {
            for (int i = 0; i <= numJobs; i++) {
                cj = C[i] - this->up + (Instance.processingTime[j-1] + this->up)*x[i][j];
                model.addConstr(C[j], GRB_GREATER_EQUAL, cj, "C[" + itos(j) + "][" + itos(i) + "]") ;
            }
        }
        
        //T_i >= C_i − d_i ∀i ∈ N
        GRBLinExpr ti;
        for (int i = 1; i <= numJobs; i++) {
            ti = C[i] - Instance.dueDate[i-1];
            model.addConstr(T[i], GRB_GREATER_EQUAL, ti, "T[" + itos(i) + "]") ;
        }
        
        // Optimize model
        model.optimize();

        cout << "Atraso total: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        
        cout << "Matriz:" << endl;
        int solucao[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                solucao[i][j] = round(x[i][j].get(GRB_DoubleAttr_X));
                cout << solucao[i][j] << " \t";
            }
            cout << endl;
        }
        
        int sequencia[numJobs];
        for (int j = 0; j <= numJobs; j++) {
            if(solucao[0][j] == 1){
                sequencia[0] = j;
            }
        }
        cout << "Sequencia: ";
        cout << sequencia[0] << "\t";
        for (int i = 1; i < numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                if(solucao[sequencia[i-1]][j] == 1){
                    sequencia[i] = j;
                    cout << sequencia[i] << "\t";
                }
            }
        }
        cout << endl;
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}

/*
 * Problema de máquina simples
 * não tem tempo de preparação
 * Objetivo: minimizar o atraso máximo
 */
void SMSP::ExactMaximumTardiness(SchedulingInstance Instance){
    try{
        int numJobs = Instance.numJobs;
        
        GRBVar x[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                x[i][j] = model.addVar(0, 1, 1, GRB_BINARY, "x[" + itos(i) + "][" + itos(j) + "]");
            }
        }
        
        GRBVar T[numJobs+1], C[numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            T[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "T[" + itos(i) + "]");
            C[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "C[" + itos(i) + "]");
        }
        
        
        GRBVar TMax;
        TMax = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "TMax");
        
        
        // Integrate new variables
        model.update();
        
        //Objetivo
        //min soma(T_max)
        GRBLinExpr atrasoMax = TMax;
        model.setObjective(atrasoMax, GRB_MINIMIZE);
        
        
        // Restrições
        //S1
        //(@for(Tarefas(j):
        //  @sum(Tarefas(i) | i #NE# j: x(i,j)) = 1);
        for (int j = 0; j <= numJobs; j++) {
            GRBLinExpr quantSeq = 0;
            for (int i = 0; i <= numJobs; i++) {
                if(i != j){
                    quantSeq += x[i][j];
                }
            }
            model.addConstr(quantSeq, GRB_EQUAL, 1, "Sequencia" + itos(j));
        }
        
        //S2
        //@for(Tarefas(i):
        //  @sum(Tarefas(j) | j #NE# i: x(i,j)) = 1);
        for (int i = 0; i <= numJobs; i++) {
            GRBLinExpr quantExec = 0;
            for (int j = 0; j <= numJobs; j++) {
                if(i != j){
                    quantExec += x[i][j];
                }
            }
            model.addConstr(quantExec, GRB_EQUAL, 1, "Execucao" + itos(i));
        }
        
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(x[j][j], GRB_EQUAL, 0, "Sequencia" + itos(j));
        }
        
        //S3
        //@for(Tarefas(j) | j #NE# 1:
        //  @for(Tarefas(i):
        //      C(j) >= C(i) - M + (p(j) + M)*x(i,j)));
        GRBLinExpr cj;
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;
        for (int j = 1; j <= numJobs; j++) {
            for (int i = 0; i <= numJobs; i++) {
                cj = C[i] - this->up + (Instance.processingTime[j-1] + this->up)*x[i][j];
                model.addConstr(C[j], GRB_GREATER_EQUAL, cj, "C[" + itos(j) + "][" + itos(i) + "]") ;
            }
        }
        
        //T_max >= T_i ∀i ∈ N
        for (int i = 1; i <= numJobs; i++) {
            model.addConstr(TMax, GRB_GREATER_EQUAL, T[i], "TMax[" + itos(i) + "]") ;
        }
        
        //T_i >= C_i − d_i ∀i ∈ N
        for (int i = 1; i <= numJobs; i++) {
            model.addConstr(T[i], GRB_GREATER_EQUAL, C[i] - Instance.dueDate[i-1], "T[" + itos(i) + "]") ;
        }
        
        // Optimize model
        model.optimize();

        cout << "Atraso máximo: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        
        cout << "Matriz:" << endl;
        int solucao[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                solucao[i][j] = round(x[i][j].get(GRB_DoubleAttr_X));
                cout << solucao[i][j] << " \t";
            }
            cout << endl;
        }
        
        int sequencia[numJobs];
        for (int j = 0; j <= numJobs; j++) {
            if(solucao[0][j] == 1){
                sequencia[0] = j;
            }
        }
        cout << "Sequencia: ";
        cout << sequencia[0] << "\t";
        for (int i = 1; i < numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                if(solucao[sequencia[i-1]][j] == 1){
                    sequencia[i] = j;
                    cout << sequencia[i] << "\t";
                }
            }
        }
        cout << endl;
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}

/*
 * Problema de máquina simples
 * não tem tempo de preparação
 * Objetivo: minimizar a soma do atraso e adiantamento
 */
void SMSP::ExactTotalEarlinessTardiness(SchedulingInstance Instance){
    try{
        int numJobs = Instance.numJobs;
        
        GRBVar x[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                x[i][j] = model.addVar(0, 1, 1, GRB_BINARY, "x[" + itos(i) + "][" + itos(j) + "]");
            }
        }
        
        GRBVar T[numJobs+1], E[numJobs+1], C[numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            T[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "T[" + itos(i) + "]");
            E[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "E[" + itos(i) + "]");
            C[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "C[" + itos(i) + "]");
        }
        
        
        // Integrate new variables
        model.update();
        
        //Objetivo
        //min soma(T_i + E_i) ∀i ∈ N
        GRBLinExpr somaAtrasoAdiantamento = 0;
        for (int i = 1; i <= numJobs; i++) {
            somaAtrasoAdiantamento += T[i] + E[i];
        }
        model.setObjective(somaAtrasoAdiantamento, GRB_MINIMIZE);
        
        
        
        // Restrições
        //S1
        //(@for(Tarefas(j):
        //  @sum(Tarefas(i) | i #NE# j: x(i,j)) = 1);
        for (int j = 0; j <= numJobs; j++) {
            GRBLinExpr quantSeq = 0;
            for (int i = 0; i <= numJobs; i++) {
                if(i != j){
                    quantSeq += x[i][j];
                }
            }
            model.addConstr(quantSeq, GRB_EQUAL, 1, "Sequencia" + itos(j));
        }
        
        //S2
        //@for(Tarefas(i):
        //  @sum(Tarefas(j) | j #NE# i: x(i,j)) = 1);
        for (int i = 0; i <= numJobs; i++) {
            GRBLinExpr quantExec = 0;
            for (int j = 0; j <= numJobs; j++) {
                if(i != j){
                    quantExec += x[i][j];
                }
            }
            model.addConstr(quantExec, GRB_EQUAL, 1, "Execucao" + itos(i));
        }
        
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(x[j][j], GRB_EQUAL, 0, "Sequencia" + itos(j));
        }
        
        //S3
        //@for(Tarefas(j) | j #NE# 1:
        //  @for(Tarefas(i):
        //      C(j) >= C(i) - M + (p(j) + M)*x(i,j)));
        GRBLinExpr cj;
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;
        for (int j = 1; j <= numJobs; j++) {
            for (int i = 0; i <= numJobs; i++) {
                cj = C[i] - this->up + (Instance.processingTime[j-1] + this->up)*x[i][j];
                model.addConstr(C[j], GRB_GREATER_EQUAL, cj, "C[" + itos(j) + "][" + itos(i) + "]") ;
            }
        }
        
        //T_i >= C_i − d_i ∀i ∈ N
        for (int i = 1; i <= numJobs; i++) {
            model.addConstr(T[i], GRB_GREATER_EQUAL, C[i] - Instance.dueDate[i-1], "T[" + itos(i) + "]") ;
        }
        
        //E_i >= d_i - C_i ∀i ∈ N
        for (int i = 1; i <= numJobs; i++) {
            model.addConstr(E[i], GRB_GREATER_EQUAL, Instance.dueDate[i-1] - C[i], "E[" + itos(i) + "]") ;
        }
        
        // Optimize model
        model.optimize();

        cout << "Atraso e adiantamento total: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        
        cout << "Matriz:" << endl;
        int solucao[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                solucao[i][j] = round(x[i][j].get(GRB_DoubleAttr_X));
                cout << solucao[i][j] << " \t";
            }
            cout << endl;
        }
        
        int sequencia[numJobs];
        for (int j = 0; j <= numJobs; j++) {
            if(solucao[0][j] == 1){
                sequencia[0] = j;
            }
        }
        cout << "Sequencia: ";
        cout << sequencia[0] << "\t";
        for (int i = 1; i < numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                if(solucao[sequencia[i-1]][j] == 1){
                    sequencia[i] = j;
                    cout << sequencia[i] << "\t";
                }
            }
        }
        cout << endl;
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}

/*
 * Problema de máquina simples
 * não tem tempo de preparação
 * Objetivo: minimizar o número de tarefas atrasadas
 */
void SMSP::ExactNumberTardyJobs(SchedulingInstance Instance){
    try{
        int numJobs = Instance.numJobs;
        
        GRBVar x[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                x[i][j] = model.addVar(0, 1, 1, GRB_BINARY, "x[" + itos(i) + "][" + itos(j) + "]");
            }
        }
        
        GRBVar T[numJobs+1], C[numJobs+1], y[numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            y[i] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY, "y[" + itos(i) + "]");
            T[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "T[" + itos(i) + "]");
            C[i] = model.addVar(0.0, GRB_MAXINT, 1.0, GRB_CONTINUOUS, "C[" + itos(i) + "]");
        }
        
        // Integrate new variables
        model.update();
        
        //Objetivo
        //min soma(y_i) ∀i ∈ N
        GRBLinExpr numTarefasAtrasas = 0;
        for (int i = 1; i <= numJobs; i++) {
            numTarefasAtrasas += y[i];
        }
        model.setObjective(numTarefasAtrasas, GRB_MINIMIZE);
        
        // Restrições
        //S1
        //(@for(Tarefas(j):
        //  @sum(Tarefas(i) | i #NE# j: x(i,j)) = 1);
        for (int j = 0; j <= numJobs; j++) {
            GRBLinExpr quantSeq = 0;
            for (int i = 0; i <= numJobs; i++) {
                if(i != j){
                    quantSeq += x[i][j];
                }
            }
            model.addConstr(quantSeq, GRB_EQUAL, 1, "Sequencia" + itos(j));
        }
        
        //S2
        //@for(Tarefas(i):
        //  @sum(Tarefas(j) | j #NE# i: x(i,j)) = 1);
        for (int i = 0; i <= numJobs; i++) {
            GRBLinExpr quantExec = 0;
            for (int j = 0; j <= numJobs; j++) {
                if(i != j){
                    quantExec += x[i][j];
                }
            }
            model.addConstr(quantExec, GRB_EQUAL, 1, "Execucao" + itos(i));
        }
        
        for (int j = 0; j <= numJobs; j++) {
            model.addConstr(x[j][j], GRB_EQUAL, 0, "Sequencia" + itos(j));
        }
        
        //S3
        //@for(Tarefas(j) | j #NE# 1:
        //  @for(Tarefas(i):
        //      C(j) >= C(i) - M + (p(j) + M)*x(i,j)));
        GRBLinExpr cj;
        model.addConstr(C[0], GRB_EQUAL, 0, "C[0]") ;
        for (int j = 1; j <= numJobs; j++) {
            for (int i = 0; i <= numJobs; i++) {
                cj = C[i] - this->up + (Instance.processingTime[j-1] + this->up)*x[i][j];
                model.addConstr(C[j], GRB_GREATER_EQUAL, cj, "C[" + itos(j) + "][" + itos(i) + "]") ;
            }
        }
        
        //T_i >= C_i − d_i ∀i ∈ N
        GRBLinExpr ti;
        for (int i = 1; i <= numJobs; i++) {
            ti = C[i] - Instance.dueDate[i-1];
            model.addConstr(T[i], GRB_GREATER_EQUAL, ti, "T[" + itos(i) + "]") ;
        }
        
        //T_i ≤ M y_i ∀i ∈ N
        for (int i = 1; i <= numJobs; i++) {
            model.addConstr(T[i], GRB_LESS_EQUAL, this->up*y[i], "T[" + itos(i) + "]") ;
        }
        
        // Optimize model
        model.optimize();

        cout << "Atraso total: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        
        for (int i = 1; i <= numJobs; i++) {
            cout << "Atraso: " << round(y[i].get(GRB_DoubleAttr_X)) << endl;
        }

        
        cout << "Matriz:" << endl;
        int solucao[numJobs+1][numJobs+1];
        for (int i = 0; i <= numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                solucao[i][j] = round(x[i][j].get(GRB_DoubleAttr_X));
                cout << solucao[i][j] << " \t";
            }
            cout << endl;
        }
        
        int sequencia[numJobs];
        for (int j = 0; j <= numJobs; j++) {
            if(solucao[0][j] == 1){
                sequencia[0] = j;
            }
        }
        cout << "Sequencia: ";
        cout << sequencia[0] << "\t";
        for (int i = 1; i < numJobs; i++) {
            for (int j = 0; j <= numJobs; j++) {
                if(solucao[sequencia[i-1]][j] == 1){
                    sequencia[i] = j;
                    cout << sequencia[i] << "\t";
                }
            }
        }
        cout << endl;
        
    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }
}

void SMSP::ExactMaximumLateness(SchedulingInstance Instance){
    
}