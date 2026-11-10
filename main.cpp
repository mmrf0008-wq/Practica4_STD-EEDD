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
    else {

    }

}

void printParque(Parque *pq) {
    cout << "\tCodigo: " << pq->getCodigoParque() << endl;
    cout << "\tNombre: " << pq->getNombreParque() << endl;
    cout << "\tArboles: " << pq->get_contenedor().size() << endl;
    cout << endl;
}

int main() {
    try {
        ControlParques control(
            RUTA_ESPECIES,
            RUTA_PARQUES,
            RUTA_ARBOLES);

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

        /*cout << "\nEJERCICIO 2: ESPECIE MÁS COMÚN EN PARQUE PRINCESA LEONOR\n";
        Especie *especieMasComun = control.especieMasComun("PARQUE PRINCESA LEONOR");
        if (especieMasComun == nullptr) {
            cout << "\t" << "No se ha encontrado el parque" << endl;
        } else {
            cout << "\t" << "Especie más común: " << especieMasComun->get_nombre_cientifico() << endl;
        }

        cout << "\nEJERCICIO 3: BUSCAR PARQUES CON ABETO\n";
        auto parques = control.listarParqueArbol("Abeto");
        for (Parque *pq: parques) {
            printParque(pq);
        }

        cout << "\nEJERCICIO 4: BUSCAR BUSCAR PINO RESINERO Y PINO\n";
        string unicoArbol = "Pino resinero";
        Arbol *arbol = control.buscarArbolNombreComun(unicoArbol);
        if (arbol != nullptr) {
            cout << "\tBuscar Pino resinero" << endl;
            printArbol(arbol);
        } else {
            cout << "\t" << "No se ha encontrado el árbol " << unicoArbol << endl;
        }

        vector<Arbol *> arbolesPino = *control.buscarArbolNombreComunComienzo("Pino");
        cout<<"\tPinos encontrado: "<<arbolesPino.size()<<endl;

        cout << "\nEJERCICIO 5: ÁRBOLES EN BARRIOS 43 Y 91\n";
        int id_barrios[] = {43, 91};
        for (int barrio: id_barrios) {
            int total = control.contarArbolesBarrio(barrio);
            cout << "\t" << "Total árboles barrio " << barrio << ": " << total << endl;
        }

        cout << "\nEJERCICIO DE PAREJAS/OPCIONAL: BUSCAR ÁRBOLES POR NOMBRE Y BARRIO\n";
        vector<Arbol *> arbolesNombreDistrito = control.buscarArbolesPorEspecieYDistrito("Pino", 9);
        cout<<"Arboles encontrados: "<<arbolesNombreDistrito.size()<<endl;
        for (auto arbol: arbolesNombreDistrito) {
            cout << "\t" << "assetnum: " << arbol->getAssetNum() << endl;
            cout << "\t" << "nombre barrio: " << arbol->get_nombre_barrio() << endl;
            cout << "\t" << "parque: " << arbol->getParque()->getNombreParque() << endl;
            cout << endl;
        }*/
    } catch (const std::exception &error) {
        std::cerr << "\nError: " << error.what() << '\n';
        return 1;
    }

    return 0;
};
