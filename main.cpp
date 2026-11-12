
#include <algorithm>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ControlParques.h"
#include "Arbol.h"
using namespace std;

//Rutas de los archivos
const string RUTA_ESPECIES = "../data/arbolado-especies.csv";
const string RUTA_PARQUES = "../data/parques.csv";
const string RUTA_ARBOLES = "../data/arboladoMadrid.csv";


void printArbol(Arbol *arbol) {
    if(arbol) {
        cout << "\t" << "identificador: " << arbol->getAssetNum() << endl;
        cout << "\t" << "codigo de parque: " << arbol->getParque()->getCodigoParque() << endl;
        cout << "\t" << "nombre común: " << arbol->getEspecie()->get_nombre_comun() << endl;
        cout << "\t" << "nombre científico: " << arbol->getEspecie()->get_nombre_cientifico() << endl;
        cout << "\t" << "tipo planta: " << Especie::tptostring(arbol->getEspecie()->get_tipo_planta()) << endl;
        cout << endl;
    }
}
void printParque(Parque *pq) {
    if(pq) {
        cout << "\tCodigo: " << pq->getCodigoParque() << endl;
        cout << "\tNombre: " << pq->getNombreParque() << endl;
        cout << "\tArboles: " << pq->get_contenedor().size() << endl;
        cout << endl;
    }
}

/**
*   Realizado por Alberto Martínez Martín amm00530@red.ujaen.es
*   y por Maitena María Rosa Fiedler mmrf0008@red.ujaen.es
**/
int main() {
    try {
        ControlParques control(
            RUTA_ESPECIES,
            RUTA_PARQUES,
            RUTA_ARBOLES);

        cout << "Especies cargadas " << control.getVariantes().size() << endl;
        cout << "Parques cargados " << control.getparques().size() << endl;
        cout << "Arboles cargados " << control.getArboles().size() << endl;

        std::cout << "\nEJERCICIO 1: BUSCAR VARIOS ID\n";
        string identificadores[] = {"6951831", "10152826", "8887777", "6906686"};
        for (string id: identificadores) {
            Arbol *arbol = control.buscarArbol(id);
            if (arbol == nullptr) {
                cout << "\t" << id << " no encontrado" << endl << endl;
                continue;
            }
                printArbol(arbol);
        }

        cout << "\nEJERCICIO 2: ESPECIE MÁS COMÚN EN PARQUE PRINCESA LEONOR\n";
        Especie *especieMasComun = control.especieMasComunEnParque("PARQUE PRINCESA LEONOR");
        if (especieMasComun == nullptr) {
            cout << "\t" << "No se ha encontrado el parque" << endl;
        } else {
            cout << "\t" << "Especie más común: " << especieMasComun->get_nombre_cientifico() << endl;
        }

        cout << "\nEJERCICIO 3: BUSCAR PARQUES CON ABETO\n";
        list<Parque> *parques = control.listarParqueArbol("Abeto");
        for (list<Parque>::iterator i = parques->begin(); i != parques->end(); i++) {
            printParque(&i.operator*());
        }

        cout << "\nEJERCICIO 4: BUSCAR BUSCAR PINO RESINERO Y PINO\n";
        string unicoArbol = "Pino resinero";
        vector<Arbol*> *arbol = control.buscarArbolNombreComun(unicoArbol);

        if(arbol == nullptr) throw invalid_argument("no se ha encontrado ningun arbol");

        cout << "Arboles encontrados con el nombre 'Pino Resinero': " << arbol->size() << endl;
        for(int i=0; i < 5; i++) {
            printArbol((*(arbol))[i]);
        }

        vector<Arbol *> *arbolesPino = control.buscarArbolNombreComunComienzo("Pino");
        if(arbolesPino == nullptr) throw invalid_argument("arboles con comienzo pino no encontrados") ;
        cout << "Arboles encontrados con el prefijo 'Pino': " << arbolesPino->size() << endl;
        for(int i= arbolesPino->size(); i > arbolesPino->size()-5; i--) {
            printArbol((*(arbolesPino))[i]);
        }

        cout << "\nEJERCICIO 5: ÁRBOLES EN BARRIOS 43 Y 91\n";
        int id_barrios[] = {43, 91};
        for (int barrio: id_barrios) {
            int total = control.contarArbolesBarrio(barrio);
            cout << "\t" << "Total árboles barrio " << barrio << ": " << total << endl;
        }

        cout << "\nEJERCICIO DE PAREJAS/OPCIONAL: BUSCAR ÁRBOLES POR NOMBRE Y BARRIO\n";
        list<Arbol *> *arbolesNombreDistrito = control.buscarArbolesPorEspecieYDistrito("Pino", 9);
        cout<<"Arboles encontrados: "<<arbolesNombreDistrito->size()<<endl;
        for (list<Arbol*>::iterator i = arbolesNombreDistrito->begin(); i != arbolesNombreDistrito->end(); i++) {
            cout << "\t" << "assetnum: " << (*i)->getAssetNum() << endl;
            cout << "\t" << "nombre barrio: " << (*i)->get_nombre_barrio() << endl;
            cout << "\t" << "parque: " << (*i)->getParque()->getNombreParque() << endl;
            cout << endl;
        }

    } catch (const std::exception &error) {
        std::cerr << "\nError: " << error.what() << '\n';
        return 1;
    }
};
