/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SchedulingInstance.cpp
 * Author: marcelo
 * 
 * Created on 19 de Agosto de 2016, 18:53
 */


#include "SchedulingInstance.h"

SchedulingInstance::SchedulingInstance(){
    this->numJobs = 0;
}

SchedulingInstance::SchedulingInstance(int numJobs, int numFamily){
    this->numJobs = numJobs;
    this->numFamily = numFamily;
    this->Allocation();
}

SchedulingInstance::SchedulingInstance(string fileName){
    if(!fileName.empty()){
        this->fileName = fileName;
    }
    if(!this->fileName.empty()){
        ifstream myfile(this->fileName.c_str());
        if(myfile.is_open()){
            myfile >> this->numJobs;
            myfile >> this->LimInf;
            myfile >> this->LimSup;
            
            this->numFamily = 0;
            //Alocar memória
            this->Allocation();
            
            for (int i = 0; i < this->numJobs; i++) {
                myfile >> this->processingTime[i];
            }
            for (int i = 0; i < this->numJobs; i++) {
                myfile >> this->dueDate[i];
            }
            for (int i = 0; i <= this->numJobs; i++) {
                for (int j = 0; j < this->numJobs; j++) {
                    myfile >> this->setupTime[i][j];
                }
            }
        }
    }
}

/*void SchedulingInstance::Allocation(int numJobs){
    this->dueDate = new int[numJobs];
    this->processingTime = new int[numJobs];
    this->setupTime = new int*[numJobs+1];
    for (int i = 0; i <= numJobs; i++) {
        this->setupTime[i] = new int[numJobs];
    }
    this->alpha = new int[numJobs];
    this->beta = new int[numJobs];
}*/

void SchedulingInstance::Allocation(){
    this->dueDate = new int[this->numJobs];
    this->processingTime = new int[this->numJobs];
    this->setupTime = new int*[this->numFamily];
    for (int i = 0; i < numFamily; i++) {
        this->setupTime[i] = new int[this->numFamily+1];
    }
    this->family = new int[this->numJobs]; 
    this->alpha = new int[this->numJobs];
    this->beta = new int[this->numJobs];
}

void SchedulingInstance::Print(){
    cout << "Número de Tarefas: " << this->numJobs << endl;
    cout << "Tempo de processamento:" << endl;
    for (int i = 0; i < this->numJobs; i++) {
        cout << this->processingTime[i]<< " ";
    }
    cout << endl;
    cout << "Data de entrega:" << endl;
    for (int i = 0; i < this->numJobs; i++) {
        cout << this->dueDate[i]<< " ";
    }
    cout << endl;
    cout << "Tempo de preparação:" << endl;
    for (int i = 0; i <= this->numJobs; i++) {
        for (int j = 0; j < this->numJobs; j++) {
            cout << this->setupTime[i][j]<< " ";
        }
        cout << endl;
    }
}

void SchedulingInstance::IntanceTest(int numJobs, int numFamily){
    
    this->numJobs = numJobs;
    this->numFamily = numFamily;
    
    //Alocar memória
    this->Allocation();

    for (int i = 0; i < this->numJobs; i++) {
        this->processingTime[i] = rand()%200;
    }
    for (int i = 0; i < this->numJobs; i++) {
        this->dueDate[i] = rand()%1000;
    }
    for (int i = 0; i <= this->numJobs; i++) {
        for (int j = 0; j < this->numJobs; j++) {
            if(i != j)
                this->setupTime[i][j] = rand()%40;
            else
                this->setupTime[i][j] = 0;
        }
    }
}

void SchedulingInstance::IntanceTestMarcone(){
    
    /*this->numJobs = 8;
    this->numFamily = 2;
    this->Allocation();
    
    //p
    this->processingTime[0] = 0;
    this->processingTime[1] = 7;
    this->processingTime[2] = 4;
    this->processingTime[3] = 2;
    this->processingTime[4] = 5;
    this->processingTime[5] = 6;
    this->processingTime[6] = 3;
    this->processingTime[7] = 1;
    
    //d
    this->dueDate[0] = 0;
    this->dueDate[1] = 13;
    this->dueDate[2] = 10;
    this->dueDate[3] = 6;
    this->dueDate[4] = 9;
    this->dueDate[5] = 3;
    this->dueDate[6] = 20;
    this->dueDate[7] = 4;
    
    //f
    this->family[0] = 0;
    this->family[1] = rand()%2;
    this->family[2] = rand()%2;
    this->family[3] = rand()%2;
    this->family[4] = rand()%2;
    this->family[5] = rand()%2;
    this->family[6] = rand()%2;
    this->family[7] = rand()%2;
    */
    
    this->numJobs = 7;
    this->numFamily = 2;
    this->Allocation();
    
    //p
    this->processingTime[0] = 7;
    this->processingTime[1] = 4;
    this->processingTime[2] = 2;
    this->processingTime[3] = 5;
    this->processingTime[4] = 6;
    this->processingTime[5] = 3;
    this->processingTime[6] = 1;
    
    //d
    this->dueDate[0] = 13;
    this->dueDate[1] = 10;
    this->dueDate[2] = 6;
    this->dueDate[3] = 9;
    this->dueDate[4] = 3;
    this->dueDate[5] = 20;
    this->dueDate[6] = 4;
    
    //f
    this->family[0] = rand()%2;
    this->family[1] = rand()%2;
    this->family[2] = rand()%2;
    this->family[3] = rand()%2;
    this->family[4] = rand()%2;
    this->family[5] = rand()%2;
    this->family[6] = rand()%2;
    
    //cout << "Famílias: " << endl;
    for(int i = 0; i < this->numJobs; i++){
        //cout << "[" << i << "] => " << this->family[i] << endl; 
    }
    
    //cout << "Tempo de preparação: " << endl;
    
    for (int i = 0; i < this->numFamily; i++) {
        for (int j = 0; j < this->numFamily; j++) {
            if(i==j){
                this->setupTime[i][j] = 0;
            }
            else{
                this->setupTime[i][j] = rand()%30 + 534;
            }
            
            //cout << "[" << i << "][" << j << "] => " << this->setupTime[i][j] << endl; 
        }
    }

}

void SchedulingInstance::Intance(string instanceFolder, string instanceFile){
    
    //SchedulingInstance *instance;
    string fileNameIn;
    int numJobs, numFamily;
    //int maxTime;

    //O arquivo da instancia eh nome.txt
    fileNameIn = instanceFolder + instanceFile;
    
    //Arquivo de entrada
    ifstream myfile(fileNameIn.c_str());

    ReadInstanceHeader(&myfile, numJobs, numFamily);
    
    this->numJobs = numJobs;
    this->numFamily = numFamily;

    myfile.seekg(0);

    //instance = new SchedulingInstance(numJobs, numFamily);
    this->Allocation();
    

    ReadInstance(&myfile);

    //Tempo maximo de execucao proporcional ao tamanho da instancia
    //maxTime = time*numJobs;

    //vector<Solution> nonDominatedSet;
    
    /*char *pch, *str;
    vector<string> name;
    //str = strdup(algorithm.c_str());
    pch = strtok(str, "_");
    
    //Criar um vetor de string com as partes do nome do algoritmo
    while (pch != NULL) {
        name.push_back(pch);
        pch = strtok(NULL, "_");
    }*/
    
    //free(instance);
}

void SchedulingInstance::ReadInstanceHeader(ifstream *myfile, int &numJobs, int &numFamily){
    
    string line, str;
    char *caracter;
    /*Lendo no arquivo o numero de tarefas e numero de familias*/
    if(myfile->is_open())
    {
        getline(*myfile,line);
        caracter = strdup(line.c_str());
        str = strtok(caracter, " ");
        numJobs = atoi(str.c_str());
        str = strtok (NULL, " ");
        numFamily = atoi(str.c_str());
    }
    
}


void SchedulingInstance::ReadInstance(ifstream *myfile){
    
    string line, str;
    char *caracter;
    
    if(myfile->is_open())
    {
        getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Tempo de processamento
        
        caracter = strdup(line.c_str());
        str = strtok(caracter, " ");
        str = strtok (NULL, " ");
        
        /*Leitura de tempo de processamento*/
        for (int i = 0; i < this->numJobs; i++) {
            str = strtok (NULL, " ");
            this->processingTime[i] = atoi(str.c_str());
        }
        
        getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Data de entrega
        
        caracter = strdup(line.c_str());
        str = strtok(caracter, " ");
        str = strtok (NULL, " ");
        
        /*Leitura da data de entrega*/
        for (int i = 0; i < this->numJobs; i++) {
            str = strtok (NULL, " ");
            this->dueDate[i] = atoi(str.c_str());
            
        }
        
        getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Família
        
        caracter = strdup(line.c_str());
        str = strtok(caracter, " ");
        str = strtok (NULL, " ");
        
        /*Leitura da família de cada tarefa*/
        for (int j = 0; j < this->numJobs; j++) {
            str = strtok (NULL, " ");
            this->family[j] = atoi(str.c_str());
            this->family[j] --;
            /*Insere na familia i a tarefa j(lida na instancia)*/
            //instance->j[instance->family[j]-1].push_back(j);
        }
        
        getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Setup
        
        line = line.substr(5);  //Substring começando da posição 5
        caracter = strdup(line.c_str());
        
        /*Leitura da família de cada tarefa*/
        for (int i = 0; i < this->numFamily; i++) {
            str = strtok(caracter, " ");
            this->setupTime[i][0] = atoi(str.c_str());
            for (int j = 1; j < this->numFamily; j++) {
                str = strtok (NULL, " ");
                this->setupTime[i][j] = atoi(str.c_str());

            }
            getline(*myfile,line);  //Setup
            caracter = strdup(line.c_str());

        }
        
        //getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Alpha
        
        caracter = strdup(line.c_str());
        str = strtok(caracter, " ");
        str = strtok (NULL, " ");
        
        /*Leitura de Alpha*/
        for (int i = 0; i < this->numJobs; i++) {
            str = strtok (NULL, " ");
            this->alpha[i] = atoi(str.c_str());
            
        }
        
        getline(*myfile,line);  //Linha em branco
        getline(*myfile,line);  //Beta
        
        caracter = strdup(line.c_str());
        str = strtok(caracter, " ");
        str = strtok (NULL, " ");
        
        /*Leitura de Beta*/
        for (int i = 0; i < this->numJobs; i++) {
            str = strtok (NULL, " ");
            this->beta[i] = atoi(str.c_str());
            
        }

    }
}

void SchedulingInstance::PrintSD(){
    
    cout << "Número de Tarefas: " << this->numJobs << endl;
    
    cout << "Tempo de processamento:" << endl;
    for (int i = 0; i < this->numJobs; i++) {
        cout << this->processingTime[i] << " ";
    }
    cout << endl;
    cout << "Prazo de entrega:" << endl;
    for (int i = 0; i < this->numJobs; i++) {
        cout << this->dueDate[i] << " ";
    }
    cout << endl;
    cout << "Família:" << endl;
    for (int i = 0; i < this->numJobs; i++) {
        cout << this->family[i] << " ";
    }
    cout << endl;
    cout << "Tempo de preparação:" << endl;
    for (int i = 0; i < this->numFamily; i++) {
        for (int j = 0; j < this->numFamily; j++) {
            cout << this->setupTime[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}