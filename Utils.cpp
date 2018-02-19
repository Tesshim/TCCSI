/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Utils.cpp
 * Author: marcelo
 * 
 * Created on 19 de Agosto de 2016, 22:21
 */

#include "Utils.h"

string itos(int i) {
   stringstream s;
   s << i;
   return s.str();
}

string dtos(double i) {
   stringstream s;
   s << i;
   return s.str();
}

