#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

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

    std::string cursos[] = {"DESARROLLO FRONT END", "BASE DE DATOS", "DESARROLLO DE APLICACIONES DE ESCRITORIO", "ALGORITMOS Y PROGRAMACIÓN DE COMPUTADORAS", "INGLES"};
    
    std::cout << "\n--- REGISTRAR NOTA ---\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << i + 1 << ". " << cursos[i] << "\n";
    }

    int opcion;
    std::cout << "Seleccione una Unidad Didactica: ";
    std::cin >> opcion;

    if (opcion < 1 || opcion > 5) return;

    std::cin.ignore(); // limpiar el buffer por si acaso

    std::ofstream archivo(usuarioActivo + "_notas.txt", std::ios::app);

    std::string nota;
    std::cout << "Ingrese las notas separadas, escriba 'fin' para terminar:\n";

    while (true) {
        std::getline(std::cin, nota);
        if (nota == "fin") break;
        archivo << cursos[opcion - 1] << ": " << nota << "\n";
    }

    std::cout << "Notas guardadas.\n";
}

void mostrarNotas() {
    ifstream archivo(usuarioActivo + "_notas.txt");
    string linea;
    cout << "Notas de " << usuarioActivo << ":\n";
    while (getline(archivo, linea)) {
        cout << "- " << linea << endl;
    }
}

void menuUsuario() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===============================\n";
        cout << "    Bienvenido, " << usuarioActivo << "\n";
        cout << "===============================\n";
        cout << " \n";
        cout << "1. Registrar Nota\n";
        cout << "2. Ver Notas\n";
        cout << "3. Cerrar Sesion\n";
        cout << " \n";
        cout << "===============================\n";
        cout << "= Opcion: ";
        cin >> opcion;

        limpiarPantalla();

        if (opcion == 1) {
            registrarNota(usuarioActivo);
        } else if (opcion == 2) {
            mostrarNotas();
        }
        if (opcion != 3) Sleep(2000);
    } while (opcion != 3);
}

void menuLogin() {
    string usuario, clave;
    int opcion;

    do {
        limpiarPantalla();
        cout << "===============================\n";
        cout << "======== MENU PRINCIPAL =======\n";
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
            cout << "Usuario: ";
            cin >> usuario;
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
