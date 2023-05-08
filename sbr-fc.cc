#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "libgen.h"

using namespace std;

ofstream outfile;  // Stream para la salida del programa

/**
 * Elementos que componen la base de conocimiento.
 * Disponen de:
 *  · un double (fc) con el factor de certeza de la regla
 *  · un booleano (esDisyuncion) que indica si el antecedente es una disyunción de literales,
 *  · un vector (antecedente) con todos literales (strings) que forman el antecedente y
 *  · un string (consecuente) que representa al literal consecuente.
 */
struct Regla {
    double fc;
    bool antecedenteEsDisyuncion;
    vector<string> antecedente;
    string consecuente;
};

ostream& operator<<(ostream& os, const Regla& regla) {
    os << regla.antecedente[0];
    for (size_t j = 1; j < regla.antecedente.size(); ++j) {
        os << (regla.antecedenteEsDisyuncion ? " o " : " y ") << regla.antecedente[j];
    }
    os << " --> " << regla.consecuente << ", FC=" << regla.fc;
    return os;
}

/**
 * Base de Conocimientos. Únicamente compuesta por reglas.
 */
struct BC {
    vector<Regla> reglas;
    vector<bool> esAplicada;
};

/**
 * Base de Hechos.
 * Cuenta con:
 *  · un mapeo (hechos) nombre_de_hecho -> factor_de_certeza y
 *  · un string (meta) que indica el hecho objetivo
 */
struct BH {
    unordered_map<string, double> hechos;
    string meta;
};

/**
 * Parser del fichero que contiene la Base de Conocimientos.
 * Esta función inicializa la BC y devuelve -1 si el formato del fichero es incorrecto.
 */
int leer_BC(char *file, BC& bc) {
    ifstream stream(file);
    int num_reglas;
    string line;

    // Obtenemos el número de reglas
    regex entero_regex(R"([1-9][0-9]*)");
    smatch entero_match;
    getline(stream, line);
    if (!regex_search(line, entero_match, entero_regex)) return -1;
    num_reglas = atoi(entero_match.str().c_str());
    bc.reglas.reserve(num_reglas);
    bc.esAplicada.reserve(num_reglas);

    // Expresiones regulares para parsear el fichero de reglas
    regex regla_regex(R"(R([1-9]): Si (.+) Entonces ([^\s]+), FC=(-?[01](\.[0-9]+)?))");
    regex anteced_regex(R"(([^\s]+)(( y [^\s]+)*|( o [^\s]+)*))");
    regex literal_regex(R"((y |o )?([^\s]+))");
    smatch regla_matches, anteced_matches, literal_matches;
    for (int i = 0; i < num_reglas; ++i) {
        do getline(stream, line);
        while (line.size() == 0);  // Saltar líneas en blanco

        // Comprobación de formato
        if (!regex_search(line, regla_matches, regla_regex)) return -1;
        string antecedentes_str = regla_matches[2];
        if (!regex_match(antecedentes_str, anteced_matches, anteced_regex)) return -1;

        // Parse de una regla
        Regla regla;
        regla.fc = atof(regla_matches[4].str().c_str());  // Convert string to double
        regla.antecedenteEsDisyuncion = anteced_matches[2].str()[1] == 'o';  // Si el primer separador es una 'o'
        // Consigue todos los literales del antecedente
        while (regex_search(antecedentes_str, literal_matches, literal_regex)) {
            regla.antecedente.push_back(literal_matches[2].str());
            antecedentes_str = literal_matches.suffix().str();
        }
        regla.consecuente = regla_matches[3].str();

        // Inserción de la regla
        int index = atoi(regla_matches[1].str().c_str());
        bc.reglas.insert(bc.reglas.begin() + index - 1, regla);
        bc.esAplicada.insert(bc.esAplicada.begin() + index - 1, false);
    }

    return 0;
}

/**
 * Parser del fichero que contiene la Base de Hechos.
 * Esta función inicializa la BH y devuelve -1 si el formato del fichero es incorrecto.
 */
int leer_BH(char *file, BH& bh) {
    ifstream stream(file);
    int num_hechos;
    string line;

    // Obtenemos el número de hechos
    regex entero_regex(R"([1-9][0-9]*)");
    smatch entero_match;
    getline(stream, line);
    if (!regex_search(line, entero_match, entero_regex)) return -1;
    num_hechos = atoi(entero_match.str().c_str());
    bh.hechos.reserve(num_hechos);

    regex hecho_regex(R"(([^\s]+), FC=(-?[01](\.[0-9]+)?))");
    smatch hecho_matches;
    for (int i = 0; i < num_hechos; ++i) {
        do getline(stream, line);
        while (line.size() == 0);  // Saltar líneas en blanco

        // Comprobación de formato
        if (!regex_search(line, hecho_matches, hecho_regex)) return -1;

        // Parse e inserción de un hecho
        bh.hechos[hecho_matches[1].str()] = atof(hecho_matches[2].str().c_str());
    }

    do getline(stream, line);
    while (line.size() == 0);  // Saltar líneas en blanco
    if (line.rfind("Objetivo", 0) != 0) return -1;

    do getline(stream, line);
    while (line.size() == 0);  // Saltar líneas en blanco
    regex literal_regex(R"([^\s]+)");
    smatch literal_match;
    if (!regex_search(line, literal_match, literal_regex)) return -1;
    bh.meta = literal_match.str();

    return 0;
}

/**
 * Proceso de equiparación.
 * Consiste en recorrer la BC descartando las reglas que ya han sido aplicadas e insertando en el
 * conjunto conflicto devuelto las reglas que cumplen que su consecuente es meta.
 */
set<int> equiparar(const BC& bc, const BH& bh, const string& meta) {
    set<int> conjunto_conflicto;
    for (size_t i = 0; i < bc.reglas.size(); ++i) {
        if (!bc.esAplicada[i] && bc.reglas[i].consecuente == meta) {
            conjunto_conflicto.insert(i);
        }
    }

    outfile << "Conjunto conflicto = {";
    if (conjunto_conflicto.size() > 0) {
        std::set<int>::iterator it = conjunto_conflicto.begin();
        outfile << "R" << *it + 1;
        for (++it; it != conjunto_conflicto.end(); ++it) {
            outfile << ", R" << *it + 1; 
        }
    }
    outfile << "}" <<  endl;

    return conjunto_conflicto;
}

/**
 * Proceso de resolución.
 * Devuelve el índice de la regla de menor índice del conjunto conflicto.
 */
int resolver(const BC& bc, const set<int>& conjunto_conflicto) {
    int index = *(conjunto_conflicto.rbegin());  // El conjunto está ordenado
    outfile << "Escogemos la regla R" << index+1 << ": " << bc.reglas[index] << endl;
    return index;
}

/**
 * Combinación de antecedentes.
 * Se combinan los factores de certeza de dos piezas de evidencia que forman parte del antecedente
 * de una misma regla.
 */
double combinar1(double fc1, double fc2, bool esDisyuncion) {
    outfile << "Inferencia CASO 1-";
    double fc_combinado;
    if (esDisyuncion) {
        fc_combinado = max(fc1, fc2);
        outfile << "a: Disyunción de antecedentes -- max{";
    } else {
        fc_combinado = min(fc1, fc2);
        outfile << "b: Conjunción de antecedentes -- min{";
    }
    outfile << fc1 << ", " << fc2 << "} = " << fc_combinado << endl;
    return fc_combinado;
}

/**
 * Adquisición incremental de evidencia.
 * Se combinan los factores de certeza de dos piezas de evidencias que soportan una misma hipótesis.
 */
double combinar2(double fc1, double fc2) {
    outfile << "Inferencia CASO 2: Adquisición incremental de evidencia -- ";
    double fc_combinado;

    if (fc1 >= 0 && fc2 >= 0) {
        outfile << "factores positivos: " << fc1 << " + " << fc2 << "*(1 - " << fc1 << ")";
        fc_combinado = fc1 + fc2 * (1 - fc1);
    } else if (fc1 <= 0 && fc2 <= 0) {
        outfile << "factores negativos: " << fc1 << " + " << fc2 << "*(1 + " << fc1 << ")";
        fc_combinado = fc1 + fc2 * (1 + fc1);
    } else {
        outfile << "(" << fc1 << " + " << fc2 << ")/(1 - min{|" << fc1 << "|, |" << fc2 << "|})";
        fc_combinado = (fc1 + fc2)/(1 - min(abs(fc1), abs(fc2)));
    }
    outfile << " = " << fc_combinado << endl;
    return fc_combinado;
}

/**
 * Encadenamiento de evidencia.
 * Se combinan los factores de certeza de dos reglas que cumplen que el consecuente de la primera
 * es el antecedente de la segunda.
 */
double combinar3(double fc_antecedente, double fc_regla) {
    outfile << "Inferencia CASO 3: Encadenamiento de evidencia -- ";
    double fc_combinado = fc_regla * max((double) 0, fc_antecedente);
    outfile << fc_regla << " * max{0, " << fc_antecedente << "} = " << fc_combinado << endl;
    return fc_combinado;
}

/**
 * Algoritmo principal del motor de inferencia.
 * Se aplican los cálculos de factores de certeza al algoritmo principal de encadenamiento hacia
 * atrás expuesto en el documento de fundamentos teóricos de la práctica.
 * Utilizamos el parámetro profundidad para conocer la profundidad de recursión y poder ofrecer
 * una salida más legible.
 */
double verificar(BC& bc, BH& bh, const string& meta, int profundidad) {

    // Cálculo de la identación de cada línea en esta llamada;
    string prev_ident = "";
    for (int i = 0; i < profundidad; ++i) {
        prev_ident += "|\t";
    }

    std::unordered_map<std::string,double>::const_iterator got = bh.hechos.find(meta);
    if (got != bh.hechos.end()) {
        outfile << prev_ident << "El hecho " << meta << " ya se encontraba en la base de hechos con una certeza de ";
        outfile << got->second << endl;
        return got->second;
    }

    outfile << prev_ident << "Procedemos a verificar el hecho " << meta << endl;
    string ident = prev_ident;
    ident += "|\t";

    
    double fc_final = 0;
    bool first_regla = true;  // booleano para determinar si es la primera inferencia con esta hipótesis
    outfile << ident;
    set<int> conjunto_conflicto = equiparar(bc, bh, meta);
    while (!conjunto_conflicto.empty()) {
        outfile << ident;
        int regla_index = resolver(bc, conjunto_conflicto);
        Regla regla = bc.reglas[regla_index];
        bc.esAplicada[regla_index] = true;
        conjunto_conflicto.erase(regla_index);

        vector<string> nuevas_metas = regla.antecedente;
        double fc_combinado;
        bool first_literal = true;  // booleano para determinar si es el primer antecedente de la regla
        do {
            string nueva_meta = nuevas_metas.back();
            nuevas_metas.pop_back();
            double fc_literal = verificar(bc, bh, nueva_meta, profundidad + 1);
            if (first_literal) {
                fc_combinado = fc_literal;
                first_literal = false;
            } else {
                outfile << ident;
                fc_combinado = combinar1(fc_combinado, fc_literal, regla.antecedenteEsDisyuncion);
            }
        } while (nuevas_metas.size() > 0);
        
        outfile << ident;
        fc_combinado = combinar3(fc_combinado, regla.fc);
        if (first_regla) {
            fc_final = fc_combinado;
            first_regla = false;
        } else {
            outfile << ident;
            fc_final = combinar2(fc_final, fc_combinado);
        }
    }

    outfile << prev_ident << "Concluimos " << meta << " con una certeza de " << fc_final << endl; 
    if (fc_final != 0) bh.hechos[meta] = fc_final;
    return fc_final;
}

// Función antigua que no incluía la profundidad
void verificar(BC& bc, BH& bh, const string& meta) {
    verificar(bc, bh, meta, 0);
}

// función para obtener el nombre de un fichero sin su extensión, dada su ruta
string obtener_nombre(char* file) {
    string file_name = basename(file);

    // Remove extension
    size_t extension_index = file_name.find_last_of(".");
    file_name = file_name.substr(0, extension_index);

    return file_name;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "ERROR: Faltan parámetros." << endl;
        cerr << "\t(1) Nombre del fichero de la BC." << endl;
        cerr << "\t(2) Nombre del fichero de la BH." << endl;
        return EXIT_FAILURE;
    }

    // Fichero de salida
    string bc_file_name = obtener_nombre(argv[1]);
    string bh_file_name = obtener_nombre(argv[2]);
    string outfile_name = "Inferencia_";
    outfile_name += bc_file_name;
    outfile_name += "_";
    outfile_name += bh_file_name;
    outfile_name += ".txt";
    outfile.open(outfile_name);

    BC bc;
    if (leer_BC(argv[1], bc) < 0) cerr << "Mal formato en el fichero de BC " << argv[1] << endl;

    outfile << "Base de Conocimientos " << bc_file_name << " inicializada:" << endl;
    for (size_t i = 0; i < bc.reglas.size(); ++i) {
        outfile << "R" << i+1 << ": " << bc.reglas[i] << endl;
    }
    outfile << endl;


    BH bh;
    if (leer_BH(argv[2], bh) < 0) cerr << "Mal formato en el fichero de la BH " << argv[2] << endl;

    outfile << "Base de Hechos " << bh_file_name << " inicializada:" << endl;
    for (auto hecho : bh.hechos) {
        outfile << hecho.first << ", FC=" << hecho.second << endl;
    }
    outfile << "Meta: " << bh.meta << endl << endl;
    
    verificar(bc, bh, bh.meta);
    outfile.close();

    return 0;
}