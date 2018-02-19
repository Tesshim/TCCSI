/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: marcelo
 *
 * Created on 19 de Agosto de 2016, 18:38
 */

#include <cstdlib>

#include <stdlib.h>
#include <iostream>

#include "SchedulingInstance.h"
#include "ExactMethod.h"
#include "SMSP.h"
#include "SMSPSD.h"
#include "SMSPSDMO.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    string instanceFile, instanceFolder;
    //SchedulingInstance Instancia("intance.txt");
    SchedulingInstance Instancia;
    
    //Instancia.IntanceTest(5);
    //Instancia.IntanceTestMarcone();
    
    //Nome da pasta instancia
    instanceFolder = "Testing/Instance/";
    //Nome do arquivo instancia
    //instanceFile = "100-2-3.5-L.txt";
    //instanceFile = "100-5-3.5-L.txt";
    //instanceFile = "80-4-3.5-M.txt";
    instanceFile = "60-2-0.5-L.txt";
    //instanceFile = "7-2-0.5-L.txt";
    
    Instancia.Intance(instanceFolder, instanceFile);
    
    
    //Instancia.PrintSD();
    
    
    
    /*ExactMethod MetodoExato;
    MetodoExato.HelloWorld();*/
    
    /*SMSP MaquinaSimples;
    MaquinaSimples.ExactNumberTardyJobs(Instancia);*/
    
    //SMSPSD MaquinaSimplesSD;
    //MaquinaSimplesSD.ExactTotalTardinessRelax(Instancia);
    
    SMSPSDMO MaquinaSimplesSDMO;
    MaquinaSimplesSDMO.ExactTotalTardinessAndMakeSpan(Instancia);
    
    return 0;
}

