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

    std::string cursos[] = {"DESARROLLO FRONT END", 
                            "BASE DE DATOS", 
                            "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                            "ALGORITMOS Y PROGRAMACIÓN DE COMPUTADORAS", 
                            "INGLÉS"};

    std::cout << "\n--- REGISTRAR NOTA ---\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << i + 1 << ". " << cursos[i] << "\n";
    }

    int opcionCurso;
    std::cout << "Seleccione el curso: ";
    std::cin >> opcionCurso;
    std::cin.ignore();

    if (opcionCurso < 1 || opcionCurso > 5) {
        std::cout << "Curso inválido.\n";
        return;
    }

    int indicador;
    std::cout << "Numero de Indicador de Logro (1-5): ";
    std::cin >> indicador;
    std::cin.ignore();

    if (indicador < 1 || indicador > 5) {
        std::cout << "Indicador de logro no valido.\n";
        return;
    }

    std::ofstream archivo(usuarioActivo + "_notas.txt", std::ios::app);

    std::string nota;
    int contador = 0;

    std::cout << "\nIngrese las notas maximo 5. Escriba 'fin' para terminar:\n";
    while (contador < 5) {
        std::cout << "Nota " << contador + 1 << ": ";
        std::getline(std::cin, nota);
        if (nota == "fin") break;
        if (!nota.empty()) {
            archivo << cursos[opcionCurso - 1] 
                    << " | Indicador de logro: " << indicador 
                    << " | Nota " << contador + 1 
                    << ": " << nota << "\n";
            contador++;
        }
    }

    std::cout << "\nNotas guardadas correctamente.\n";
}



void mostrarNotas() {
    std::string cursos[] = {"DESARROLLO FRONT END", 
                            "BASE DE DATOS", 
                            "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                            "ALGORITMOS Y PROGRAMACIÓN DE COMPUTADORAS", 
                            "INGLÉS"};

    cout << "--- VER NOTAS POR CURSO ---\n";
    for (int i = 0; i < 5; ++i) {
        cout << i + 1 << ". " << cursos[i] << "\n";
    }

    int opcionCurso;
    cout << "Seleccione el curso: ";
    cin >> opcionCurso;
    cin.ignore();

    if (opcionCurso < 1 || opcionCurso > 5) {
        cout << "Curso invalido.\n";
        return;
    }

    string cursoSeleccionado = cursos[opcionCurso - 1];

    ifstream archivo(usuarioActivo + "_notas.txt");
    string linea;

    cout << "\n===============================\n";
    cout << "Notas de " << usuarioActivo << " - " << "\n";
    cout << "===============================\n";

    bool hayNotas = false;
    while (getline(archivo, linea)) {
        if (linea.find(cursoSeleccionado) != string::npos) {
            cout << "- " << linea << endl;
            hayNotas = true;
        }
    }

    if (!hayNotas) {
        cout << "No hay notas registradas para este curso.\n";
    }

    cout << "\nPresiona enter para regresar...";
    cin.ignore();
    cin.get();
}


void menuUsuario() {
    int opcion;
    do {
        limpiarPantalla();

        // Encabezado tipo tarjeta limpia
        cout << "===============================\n";
        cout << "======== CLASS MANAGER ========\n";
        cout << "===============================\n";
        cout << "\nUsuario: " << usuarioActivo << "\n";
        cout << "-------------------------------\n";
        cout << "        Menu Principal\n";
        cout << "-------------------------------\n";
        cout << "  1. Registrar Nota\n";
        cout << "  2. Ver Notas\n";
        cout << "  3. Editar Nota\n";
        cout << "  4. Eliminar Nota\n";
        cout << "  5. Ver Promedios\n";
        cout << "  6. Exportar Notas\n";
        cout << "  7. Cambiar Contrasena\n";
        cout << "  8. Cerrar Sesión\n";
        cout << "-------------------------------\n";
        cout << "Seleccione una opción (1-8): ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nEntrada invalida. Inténtelo nuevamente.\n";
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
