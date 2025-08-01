#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <limits>

using namespace std;

string usuarioActivo;

// Función para limpiar pantalla con breve pausa
void limpiarPantalla() {
    Sleep(500);
    system("cls");
}

bool verificarLogin(const string& usuario, const string& clave) {
    ifstream archivo("usuarios.txt");
    string linea;

    while (getline(archivo, linea)) {
        size_t coma = linea.find(',');
        if (coma != string::npos) {
            string u = linea.substr(0, coma);
            string c = linea.substr(coma + 1);
            if (usuario == u && clave == c) {
                usuarioActivo = usuario;
                return true;
            }
        }
    }
    return false;
}

void registrarUsuario(const string& usuario, const string& clave) {
    ofstream archivo("usuarios.txt", ios::app);
    archivo << usuario << "," << clave << endl;
}

void registrarNota(const std::string& usuarioActivo) {
    if (usuarioActivo.empty()) return;

    std::string cursos[] = {
        "DESARROLLO FRONT END",
        "BASE DE DATOS",
        "DESARROLLO DE APLICACIONES DE ESCRITORIO",
        "ALGORITMOS Y PROGRAMACIÓN DE COMPUTADORAS",
        "INGLES"
    };

    limpiarPantalla();
    std::cout << "===============================\n";
    std::cout << "       REGISTRO DE NOTAS       \n";
    std::cout << "===============================\n";
    std::cout << "Seleccione una Unidad Didáctica:\n\n";

    for (int i = 0; i < 5; ++i) {
        std::cout << "  " << (i + 1) << ". " << cursos[i] << "\n";
    }

    int opcion;
    std::cout << "\nOpción (1-5): ";
    std::cin >> opcion;

    if (opcion < 1 || opcion > 5) {
        std::cout << "\nOpción no válida. Retornando al menú...\n";
        Sleep(1500);
        return;
    }

    std::cin.ignore(); // Limpiar buffer
    std::string cursoSeleccionado = cursos[opcion - 1];

    std::ofstream archivo(usuarioActivo + "_notas.txt", std::ios::app);

    std::cout << "\nIngrese las notas para '" << cursoSeleccionado << "'.\n";
    std::cout << "Escriba una nota por línea. Escriba 'fin' para terminar:\n\n";

    std::string entradaNota;
    while (true) {
        std::cout << "Nota: ";
        std::getline(std::cin, entradaNota);
        if (entradaNota == "fin") break;

        if (!entradaNota.empty())
            archivo << cursoSeleccionado << ": " << entradaNota << "\n";
    }

    std::cout << "\nNotas guardadas correctamente.\n";
    Sleep(1500);
}


void mostrarNotas() {
    limpiarPantalla();

    std::ifstream archivo(usuarioActivo + "_notas.txt");
    std::string linea;

    std::cout << "===============================\n";
    std::cout << "        NOTAS REGISTRADAS      \n";
    std::cout << "===============================\n";
    std::cout << "Usuario: " << usuarioActivo << "\n";
    std::cout << "-------------------------------\n";

    bool hayNotas = false;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            std::cout << "• " << linea << "\n";
            hayNotas = true;
        }
    }

    if (!hayNotas) {
        std::cout << "No hay notas registradas aún.\n";
    }

    std::cout << "\nPresione Enter para volver al menú...";
    std::cin.get(); // esperar Enter
}


void menuUsuario() {
    int opcion;
    do {
        limpiarPantalla();

        // Encabezado tipo tarjeta limpia
        cout << "===============================\n";
        cout << "======== CLASS MANAGER ========\n";
        cout << "===============================\n";
        cout << "\n";
        cout << "\nUsuario activo: " << usuarioActivo << "\n";
        cout << "-------------------------------\n";
        cout << " Menú Principal\n";
        cout << "-------------------------------\n";
        cout << "  1. Registrar Nota\n";
        cout << "  2. Ver Notas\n";
        cout << "  3. Editar Nota\n";
        cout << "  4. Eliminar Nota\n";
        cout << "  5. Ver Promedios\n";
        cout << "  6. Exportar Notas\n";
        cout << "  7. Cambiar Contraseña\n";
        cout << "  8. Cerrar Sesión\n";
        cout << "-------------------------------\n";
        cout << "Seleccione una opción (1-8): ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nEntrada inválida. Inténtelo nuevamente.\n";
            Sleep(1500);
            continue;
        }

        limpiarPantalla();

        switch (opcion) {
            case 1:
                registrarNota(usuarioActivo);
                break;
            case 2:
                mostrarNotas();
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                cout << "\nEsta función está en desarrollo.\n";
                break;
            case 8:
                cout << "\nCerrando sesión...\n";
                Sleep(1000);
                break;
            default:
                cout << "\nOpción no válida. Inténtelo nuevamente.\n";
                Sleep(1500);
                break;
        }

    } while (opcion != 8);
}


void menuLogin() {
    string usuario, clave;
    int opcion;

    do {
        limpiarPantalla();
        cout << "===============================\n";
        cout << "======== CLASS MANAGER ========\n";
        cout << "===============================\n";
        cout << "\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar sesion\n";
        cout << "3. Salir\n";
        cout << "\n";
        cout << "===============================\n";
        cout << "= Opcion: ";
        cin >> opcion;

        limpiarPantalla();

        if (opcion == 1) {
            cout << "Nuevo usuario: ";
            cin >> usuario;
            cout << "Clave: ";
            cin >> clave;
            registrarUsuario(usuario, clave);
            cout << "Usuario registrado correctamente.\n";
            Sleep(1500);
        } else if (opcion == 2) {
            cout << "===============================\n";
            cout << "======== CLASS MANAGER ========\n";
            cout << "===============================\n";
            cout << " \n";
            cout << "Usuario: ";
            cin >> usuario;
            cout << "\n-------\n";
            cout << "Clave: ";
            cin >> clave;
            if (verificarLogin(usuario, clave)) {
                cout << "\n------------------\n";
                cout << "  Iniciando....\n";
                cout << "------------------\n";
                Sleep(1000);
                menuUsuario();
            } else {
                cout << "Usuario o clave incorrectos.\n";
                Sleep(1500);
            }
        }
    } while (opcion != 3);
}

int main() {
    menuLogin();
    limpiarPantalla();
    cout << "Hasta pronto...\n";
    return 0;
}
