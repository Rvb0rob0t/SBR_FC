#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "BaseHechos.h"
#include "BaseConocimientos.h"
#include "MotorInferencia.h"
using namespace std;


BaseHechos BH;
BaseConocimientos BC;
string dominio;
string objetivo;


void parseRegla(string regla);
Hecho parseHecho(string hecho);
Literal parseLiteral(string hecho);
void leerFicheros(char * nombreBC, char * nombreConfig, char * nombreBH);
void leerBaseConocimientos(char * nombreBC);
void leerConfig(char * nombreConfig);
void leerBaseHechos(char * nombreBH);





int main (int argc, char** argv)
{
    ofstream f1("Salida1.txt");
    ofstream f2("Salida2.txt");

  if(argc <= 3){
    cerr << "Uso incorrecto:\nSBR.exe 'fichero BC' 'fichero config' 'fichero BH'"<< endl;
    return 1;
  }
  leerFicheros(argv[1], argv[2], argv[3]);

  f1 <<"+DOMINIO: " << dominio <<endl << endl;
  f1 <<"+OBJETIVO: " << objetivo << endl << endl;
  f1 <<"+BASE HECHOS INICIAL: "<< endl;
  print(BH,f1);
  f1 << endl;

  f2 <<"+DOMINIO: "<< dominio << endl << endl;
  f2 <<"+OBJETIVO: " << objetivo << endl << endl;


  Hecho * solucion = EncaminamientoAdelante(&BC, &BH,  objetivo, &f1 );

  if(solucion == nullptr) {
        f1 << "+SOLUCION: No se ha alcanzado soluci�n" ;
        f2 << "+SOLUCION: No se ha alcanzado soluci�n" ;
  }
  else{
        f1 << "+SOLUCION: " ;
        print(*solucion, f1);

        f2 << "+SOLUCION: " ;
        print(*solucion, f2);
        f2 <<"\n\n+REGLAS NECESARIAS PARA ALCANZAR LA SOLUCION: "<< endl;
        PrintReglasUsadas(&BC,&f2);
  }



  delete solucion;
}


void leerFicheros(char * nombreBC, char * nombreConfig, char * nombreBH){

    string entrada;

    ifstream fileBC(nombreBC);
    getline(fileBC, dominio);

    getline(fileBC, entrada);
    int numReglas = atoi(entrada.c_str());
    BC =inicializaBC(numReglas);
    for(int i=0; i<numReglas; i++){
        getline(fileBC, entrada);
        int ind=entrada.find(": ");
        parseRegla(entrada.substr(ind+2, -1));

    }


    fileBC.close();

    ifstream fileConfig(nombreConfig);
    //Leemos la l�nea ATRIBUTOS
    getline(fileConfig, entrada);
    //N�mero de Atributos
    getline(fileConfig, entrada);
    //Conversion
    int atributos = atoi(entrada.c_str());

    for(int i=0; i<atributos; i++){
        getline(fileConfig, entrada);
        //TODO
    }
    //Linea OBJETIVO
    getline(fileConfig, entrada);
    //Almacenamos el objetivo
    getline(fileConfig, objetivo);

    //Linea PRIORIDADES
    getline(fileConfig, entrada);
    //N�mero de Atributos
    getline(fileConfig, entrada);
    int numReglas1 = atoi(entrada.c_str());
    int prioridades[numReglas1];
    for(int i=0; i<numReglas1; i++){
        getline(fileConfig, entrada);
        prioridades[i]= atoi(entrada.c_str());
    }
    setPrioridades(BC.reglasMax, prioridades);

    fileConfig.close();


    ifstream fileBH(nombreBH);

    getline(fileBH, entrada);
    int hechos = atoi(entrada.c_str());

    string nombre;
    string valor;

    for(int i=0; i<hechos; i++){
        getline(fileBH, entrada);
        addHecho(&BH, parseHecho(entrada));
    }

    fileBH.close();


}

void parseRegla(string regla){

    string antecedente;
    string consecuente;
    int k = regla.find(" Entonces ");
    antecedente = regla.substr(3, k-3);
    consecuente = regla.substr(k+10, -1);

    int ind=0;
    int ind1 = antecedente.find(" y ");
    string literal,atributo, valor;
    vector<Literal> v;
    while(ind1!=-1){
        literal = antecedente.substr(ind,ind1-ind);
        ind = ind1+3;
        ind1 = antecedente.find(" y ", ind);
        v.push_back(parseLiteral(literal));

    }
    literal = antecedente.substr(ind,ind1);
    v.push_back(parseLiteral(literal));
    Regla rule = inicializaRegla(v, parseHecho(consecuente));
    anadirRegla(&BC, rule);
}

Hecho parseHecho(string hecho){
    Hecho hch ;
    int k;

    k = hecho.find(" = ");
    hch.atributo = hecho.substr(0,k);
    hch.valor = hecho.substr(k+3,-1);

    return hch;
}

Literal parseLiteral(string literal){
    int k1,k2;

    k1 = literal.find(" ");
    k2 = literal.find(" ",k1+1);
    return inicializaLiteral(literal.substr(0,k1),literal.substr(k1+1,k2-k1-1),literal.substr(k2+1,-1));
}
