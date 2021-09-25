#include "BaseConocimientos.h"

//Función auxiliar que convierte un caracter a un entero
int toOp(string str){

    if(str.compare("=")==0) return IGUAL;
    else if (str.compare("<")==0) return MENOR;
    else if (str.compare("<=")==0) return MENORIGUAL;
    else if (str.compare(">")==0) return MAYOR;
    else if (str.compare(">=")==0) return MAYORIGUAL;
    else return -1;


}


Literal inicializaLiteral( string atributo, string op, string valor){
    Literal lit ;
    lit.atributo = atributo;
    lit.op = toOp(op);
    lit.valor = valor;
    return lit;

}



Regla inicializaRegla( vector<Literal> v, Hecho consecuente)
{   Regla regla;
    regla.numLiterales = v.size();
    regla.literales = new Literal[v.size()];
    int i =0;
    for (vector<Literal>::iterator it = v.begin() ; it != v.end(); it++)
    {
        regla.literales[i] = *it;
        i++;
    }
    regla.consecuente.atributo=consecuente.atributo;
    regla.consecuente.valor=consecuente.valor;

    return regla;
}



BaseConocimientos inicializaBC(int reglas)
{
    BaseConocimientos BC;
    BC.reglas = new Regla*[reglas];
    BC.numReglas = 0;
    BC.reglasMax = reglas;
    return BC;
}



void anadirRegla(BaseConocimientos * BC, Regla regla)
{
    if(BC->numReglas < BC->reglasMax)
    {
        //Creamos una copia de la regla
        Regla * regla1 = new Regla;
        regla1->numLiterales=regla.numLiterales;
        regla1->literales=new Literal[regla.numLiterales];
        for(int i=0; i<regla.numLiterales; i++){
            regla1->literales[i] = regla.literales[i];
        }
        regla1->consecuente=regla.consecuente;


        BC->reglas[BC->numReglas] = regla1;
        BC->numReglas++;
    }

}

int satisfaceLiteral( Literal hch,BaseHechos bh){
    for (vector<Hecho*>::iterator it=bh.hechos.begin(); it != bh.hechos.end(); ++it){
        Hecho hch1 =**it;
        if(hch.atributo.compare(hch1.atributo)  == 0){
            if(hch.op == IGUAL){
                if(hch.valor.compare(hch1.valor)  == 0) return 1;
            }
            else {
                int k1 = atoi(hch1.valor.c_str());
                int k = atoi(hch.valor.c_str());

                int res;
                switch (hch.op) {
                    case MENOR:
                        res = k1 < k;
                        break;
                    case MENORIGUAL:
                        res = k1 <= k;
                        break;
                    case MAYOR:
                        res = k1 > k;
                        break;
                    case MAYORIGUAL:
                        res = k1 >= k;
                        break;

                }
                if (res) return 1;
            }
        }
    }
    return 0;
}

int satisface(Regla regla, BaseHechos BH){
    for(int i =0; i<regla.numLiterales; i++){
        if (!satisfaceLiteral(regla.literales[i],BH))return 0;
    }
    return 1;
}

//Función auxiliar para imprimir una operación que viene representada por un entero
string toString(int op){
    switch (op ){
        case IGUAL:
            return "=";
        case MENOR:
            return "<";
        case MENORIGUAL:
            return "<=";
        case MAYOR:
            return ">";
        case MAYORIGUAL:
            return "=>";
        default:
            return "--";
    }
}


//Funiones de impresion
void print(Literal lit, ofstream &f){
    f << lit.atributo << toString(lit.op) << lit.valor ;
}

void print(Regla regla, ofstream &f)
{
    f <<"SI ";
    print(regla.literales[0], f);
    for(int i =1; i< regla.numLiterales ; i++)
    {
        f << " && ";
        print(regla.literales[i],f);
    }
    f << " --> ";
    print(regla.consecuente,f);

}

void print(BaseConocimientos BC,ofstream f)
{
    for(int i =0; i< BC.reglasMax; i++){
        f << "R"<<i<<": ";
        print(*BC.reglas[i],f);
        f << endl;
    }
}



