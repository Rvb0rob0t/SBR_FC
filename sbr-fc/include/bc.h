#ifndef SBRFC_INCLUDE_BC_H_
#define SBRFC_INCLUDE_BC_H_

#include <string>
#include <vector>
#include "bh.h"
using namespace std;

struct Regla {
    vector<Hecho> antecedente;
    Hecho consecuente;
};

typedef vector<Regla> BC;

/**
 * \brief Construye un nuevo literal.
 *
 * \param Nombre del atributo del literal.
 * \param Cadena que contine los simbolos de la operaci�n.
 * \param Valor del atributo del literal.
 * 
 * \return Devuelve el literal construido.
 */
Literal inicializaLiteral( string atributo, string op, string valor);

/** \brief Construye una nueva regla.
 *
 * \param Conjunto de literales que forman la precondici�n de la regla.
 * \param Post-condici�n de la regla.
 * \return Devuelve la regla construida.
 *
 */
Regla inicializaRegla( vector<Literal> v, Hecho consecuente);

/** \brief Crea una nueva Base de conocimientos
 *
 * \param representa el numero de reglas que contendr� la base de conocimientos.
 * \return Devuelve una Base de Conocimientos inicializada
 *
 */
BaseConocimientos inicializaBC(int reglas);

/** \brief Permite a�adir una nueva regla a una Base de conomientos
 *
 * \param Base de conocimientos a la que a�adir la regla
 * \param Regla a a�adir en la Base de Conocimiento
 *
 */
void anadirRegla(BaseConocimientos *BC, Regla regla);



/** \brief Comprueba qla precondici�n de una regla se cumpla en cierta Base de Hechos.
 *
 * \param Regla a comprobar si es cierta.
 * \param Base de Hechos en la que se comprueba si existe la Regla.
 * \return Devuelve 1 en caso de que se cumpla y 0 en caso de que no.
 *
 */
int satisface(Regla regla, BaseHechos BH);

//Funciones auxiliares para imprimir reglas y la Base de conocimientos
void print(Regla regla, ofstream &f);
void print(BaseConocimientos BC, ofstream f);

#endif // SBRFC_INCLUDE_BC_H_
