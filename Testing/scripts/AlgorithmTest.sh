#!/bin/bash

cd ../..

#Pasta em que estão as instâncias
instanceFolder=$1

#Nome do arquivo ou *.txt
instanceFile=$2

#Instancias
FILES=$instanceFolder$instanceFile

#Nome do algoritmo
algorithm=$3

#tamanho inicial (usado apenas para solucao inicial aleatoria)
initialSize=$4

#tempo em milisegundos (valor sera multiplicado pela numero de tarefas da instancia)
time=$5

#Sementes geradas aleatoriamente
seed[1]=870380201

#Pasta onde serão salvas as soluções
folderSolution="Testing/Solution"

#Arquivo para salvar o logs
fileLog="Testing/log/error.txt"

#percorre as 1 sementes
for((i=1;i<=1;i++))
do
    #se nao existe a pasta para guardar as solucoes do algoritmo
    if ! [ -d $folderSolution"/"$algorithm"/" ];
    then
        #crie a pasta
        mkdir -p $folderSolution"/"$algorithm"/"
        #echo "Criando pasta"
    fi

    #percorre todos os arquivos
    for file in $FILES
    do
        #Tira a extensao do nome do arquivo
        f=${file%.*}
        #Arquivo com a solucao nomeinstancia_semente
        fileSolution=$folderSolution"/"$algorithm"/"$f"_"${seed[$i]}".txt"
        #Nao verifica se existe o arquivo
        echo "Processando: "$file" Com a semente: "${seed[$i]}
        #Executar o algoritmo com os parametros e redirecionar as mensagens de erro para o arquivo log/error.txt
        ./dist/Debug/GNU-Linux-x86/singlemachine $instanceFolder ${file:${#instanceFolder}} $algorithm $initialSize $time ${seed[$i]} $fileSolution 2>> $fileLog
        #./dist/Debug/GNU-Linux-x86/singlemachine Instance/ 60-2-0.5-L.txt H_MOVNS 10 100 870380201 Solution/H_MOVNS/60-2-0.5-L.txt 2>> $fileLog
    done
done