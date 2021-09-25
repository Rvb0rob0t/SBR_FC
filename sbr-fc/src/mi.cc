#include "MotorInferencia.h"


 int * prioridad;
 int* ReglasUsadas;

void setPrioridades( int numPrio, int * prioridades)
{
    if(prioridad==nullptr) delete prioridad;
    prioridad= new int[numPrio];
    for(int i =0; i<numPrio; i++)
    {
        prioridad[i] = prioridades[i];
    }
}

int equiparar(BaseConocimientos * BC, BaseHechos * BH, Regla * regla,ofstream *f){
    int prio=-1;
    int rule = -1;
    if (f!=nullptr) *f << "  *EQUIPARACION-CONJUNTO_CONFLICTO:"<<endl;
    for(int i =0; i<BC->numReglas; i++){
            //Si la regla no se ha usado y se cumple la precondición
            if(!ReglasUsadas[i] && satisface(*BC->reglas[i], *BH)){

                if (f!=nullptr){
                    *f << "    R"<<i<<": ";
                    print(*BC->reglas[i], *f);
                    *f << ": Prio = " << prioridad[i]<<endl;

                }
                //SI la regla tiene más prioridad que la que está en el conjunto conflicto se sustitute
                if(prioridad[i]>prio){
                    prio= prioridad[i];
                    rule = i;
                }
            }
    }

    if (f!=nullptr) *f << "  *FIN EQUIPARACION\n"<<endl;

    //Si ninguna regla se satisface devolvemos -1
    if(rule == -1) return 1;

    //Devolvemos la solución como parámetro
    *regla = *BC->reglas[rule];
    ReglasUsadas[rule]=1;

    return 0;

}

Hecho* EncaminamientoAdelante(BaseConocimientos * BC, BaseHechos * BH,  string objetivo, ofstream *f)
{
    if(f!=nullptr) *f<< "*****COMENZANDO RAZONAMIENTO*****\n"<<endl;

    Hecho  nuevoHecho  ;
    //Mantenemos un array con las reglas que usamos
    delete ReglasUsadas;
    ReglasUsadas = new int[BC->numReglas];
    for(int i =0; i< BC->numReglas; i++) ReglasUsadas[i]=0;

    int vacioConjConflict = 0;
    Regla R;
    int j =1;

    while(!vacioConjConflict && !cometidoHecho(*BH,objetivo))
    {
        if (f!=nullptr)*f << " **ITERACION "<<j<<endl<<endl;

        //Equiparar
        vacioConjConflict = equiparar(BC, BH, &R,f);
        if(!vacioConjConflict)
        {
            //Aplicar(R,BH)
            nuevoHecho.atributo = R.consecuente.atributo;
            nuevoHecho.valor = R.consecuente.valor;

            if (f!=nullptr){
                *f << "  *RESOLVER: ";
                print(R,*f);
                *f <<endl<<endl<<"  *ACTUALIZAR: ";
                print(nuevoHecho,*f);
                *f << endl<< endl;
            }
            //Actualizar(BH,nuevoHecho)
            addHecho(BH, nuevoHecho);
        }
        j++;
    }

    if(f!=nullptr) *f<< "*****FIN RAZONAMIENTO*****\n"<<endl;

    //Si existe solución
    if (cometidoHecho(*BH,objetivo)){
        //Devolvemos el último hecho deducido ya que es la solución
        Hecho *hch;
        hch = new Hecho;
        *hch = nuevoHecho;
        return hch;

    //Si no
    }else return nullptr;


}

void PrintReglasUsadas(BaseConocimientos * BC , ofstream *f)
{
    for(int i =0; i<BC->numReglas; i++){
        if(ReglasUsadas[i] ){
            print(*BC->reglas[i],*f);
            *f<<endl;
        }
    }

}


