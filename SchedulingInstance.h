/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SchedulingInstance.h
 * Author: marcelo
 *
 * Created on 19 de Agosto de 2016, 18:53
 */


#ifndef SCHEDULINGINSTANCE_H
#define SCHEDULINGINSTANCE_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>


using namespace std;

class SchedulingInstance {
    public:
        /*Nome do arquivo*/
        string fileName;
        /*---------------Dados da instância------------------*/
        int numJobs;
        /*
         * Data de entrega da tarefa i
         */
        int *dueDate;
        /*Custo de preparacao família i, família j*/
        int **setupTime;
        /*Número de familías*/
        int numFamily;
        /*Família da tarefa i*/
        int *family;
        /*Duração da tarefa i*/
        int *processingTime;
        /*Penalidade por adiantamento*/
        int *alpha;
        /*Penalidade por atraso*/
        int *beta;
        /*Limite inferior*/
        int LimInf;
        /*Limite superior*/
        int LimSup;
        SchedulingInstance();
        SchedulingInstance(int numJobs, int numFamily);
        SchedulingInstance(string fileName);
        void Allocation();
        //void Allocation(int numJobs, int numFamily);
        void Read(string fileName);
        void Print();
        void PrintSD();
        void IntanceTest(int numJobs, int numFamily);
        void IntanceTestMarcone();
        void Intance(string instanceFolder, string instanceFile);
        void ReadInstanceHeader(ifstream *myfile, int &numJobs, int &numFamily);
        void ReadInstance(ifstream *myfile);
    private:
};

#endif /* SCHEDULINGINSTANCE_H */

