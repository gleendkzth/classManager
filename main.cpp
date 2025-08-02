#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random> // Para std::shuffle
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <locale>
#include <codecvt>
#include <vector>

using namespace std;

// Constantes para colores de consola
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BG_WHITE = "\033[47m";
const string BG_BLUE = "\033[44m";

string usuarioActivo;
const int CONSOLE_WIDTH = 80;

// Función para centrar texto
string centrarTexto(const string& texto) {
    if (texto.length() >= CONSOLE_WIDTH) return texto;
    int espacios = (CONSOLE_WIDTH - texto.length()) / 2;
    return string(espacios, ' ') + texto;
}

// Función para mostrar el encabezado
void mostrarEncabezado(const string& titulo = "") {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    // Mover el cursor al inicio
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hConsole, coord);
    
    // Guardar la posición actual del cursor
    COORD cursorPos = csbi.dwCursorPosition;
    
    // Mover el cursor al inicio
    SetConsoleCursorPosition(hConsole, {0, 0});
    
    // Dibujar el encabezado
    cout << BOLD << string(CONSOLE_WIDTH, ' ') << RESET << "\n";
    cout << BOLD << centrarTexto("=======================================") << string(CONSOLE_WIDTH - 38, ' ') << RESET << "\n";
    cout << BOLD << centrarTexto("          " + string("CLASS MANAGER", 0, 12) + "           ") << string(CONSOLE_WIDTH - 38, ' ') << RESET << "\n";
    cout << BOLD << centrarTexto("=======================================") << string(CONSOLE_WIDTH - 38, ' ') << RESET << "\n";
    
    // Mostrar título de sección si se proporciona
    if (!titulo.empty()) {
        cout << "\n" << BOLD << CYAN << centrarTexto("» " + titulo + " «") << RESET << "\n\n";
    } else {
        cout << "\n";
    }
    
    // Mover el cursor a la posición debajo del encabezado
    COORD newPos = {0, 5};
    SetConsoleCursorPosition(hConsole, newPos);
}

// Función para limpiar pantalla
void limpiarPantalla(bool mostrarHeader = true) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Obtener el tamaño de la ventana de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    // Obtener el tamaño de la ventana
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    DWORD dwConSize;
    
    // Calcular el número de celdas en la ventana actual
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    
    // Llenar toda la pantalla con espacios
    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    
    // Restaurar los atributos de color
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    
    // Mover el cursor a la posición inicial
    SetConsoleCursorPosition(hConsole, coordScreen);
    
    // Mostrar el encabezado si se solicita
    if (mostrarHeader) {
        mostrarEncabezado();
    }
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

    string cursos[] = {"DESARROLLO FRONT END", 
                     "BASE DE DATOS", 
                     "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                     "ALGORITMOS Y PROGRAMACION DE COMPUTADORAS", 
                     "INGLES"};
    
    string colores[] = {CYAN, MAGENTA, GREEN, BLUE, YELLOW};
    
    // Mostrar lista de cursos con formato y colores
    limpiarPantalla();
    mostrarEncabezado("REGISTRAR NOTA");
    
    cout << " " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n\n";
    cout << " " << BOLD << "SELECCIONE EL CURSO:" << RESET << "\n";
    cout << " " << string(60, '=') << "\n";
    
    for (int i = 0; i < 5; ++i) {
        cout << " " << BOLD << colores[i] << "[" << (i+1) << "] " << RESET 
             << BOLD << colores[i] << cursos[i] << RESET << "\n";
    }
    cout << " " << string(60, '=') << "\n\n";

    int opcionCurso;
    cout << " " << BOLD << "Seleccione el curso (1-5): " << RESET;
    cin >> opcionCurso;
    cin.ignore();

    if (opcionCurso < 1 || opcionCurso > 5) {
        cout << "\n" << RED << "Error: Curso inválido.\n" << RESET;
        cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
        _getch();
        return;
    }

    cout << "\n" << BOLD << "CURSO SELECCIONADO: " << RESET << GREEN << cursos[opcionCurso - 1] << RESET << "\n\n";
    
    int indicador;
    cout << " " << BOLD << "Indicador de Logro (1-5): " << RESET;
    cin >> indicador;
    cin.ignore();

    if (indicador < 1 || indicador > 5) {
        cout << "\n" << RED << "Error: Indicador de logro no válido.\n" << RESET;
        cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
        _getch();
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



void exportarNotas() {
    limpiarPantalla();
    mostrarEncabezado("EXPORTAR NOTAS");
    
    string nombreArchivo = "notas_" + usuarioActivo + "_" + to_string(time(0)) + ".txt";
    ofstream archivoSalida(nombreArchivo);
    
    if (!archivoSalida) {
        cout << "\n" << RED << "Error al crear el archivo de exportación.\n" << RESET;
        cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
        _getch();
        return;
    }
    
    // Escribir encabezado
    archivoSalida << "========================================\n";
    archivoSalida << "         REPORTE DE NOTAS - " << usuarioActivo << "         \n";
    archivoSalida << "         " << __DATE__ << " - " << __TIME__ << "         \n";
    archivoSalida << "========================================\n\n";
    
    string cursos[] = {"DESARROLLO FRONT END", 
                     "BASE DE DATOS", 
                     "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                     "ALGORITMOS Y PROGRAMACION DE COMPUTADORAS", 
                     "INGLES"};
    
    bool hayNotas = false;
    
    // Para cada curso
    for (int curso = 0; curso < 5; curso++) {
        ifstream archivoEntrada(usuarioActivo + "_notas.txt");
        string linea;
        bool tieneNotas = false;
        
        archivoSalida << "CURSO: " << cursos[curso] << "\n";
        archivoSalida << string(50, '-') << "\n";
        
        // Buscar notas para este curso
        while (getline(archivoEntrada, linea)) {
            if (linea.find(cursos[curso]) != string::npos) {
                archivoSalida << "- " << linea << "\n";
                tieneNotas = true;
                hayNotas = true;
            }
        }
        
        if (!tieneNotas) {
            archivoSalida << "No hay notas registradas para este curso.\n";
        }
        
        archivoSalida << "\n";
        archivoEntrada.close();
    }
    
    // Calcular promedios
    archivoSalida << "\n" << string(50, '=') << "\n";
    archivoSalida << "RESUMEN DE PROMEDIOS\n";
    archivoSalida << string(50, '=') << "\n\n";
    
    for (int curso = 0; curso < 5; curso++) {
        ifstream archivoEntrada(usuarioActivo + "_notas.txt");
        string linea;
        int totalNotas = 0;
        double sumaNotas = 0.0;
        
        // Buscar notas para este curso
        while (getline(archivoEntrada, linea)) {
            if (linea.find(cursos[curso]) != string::npos) {
                size_t posNota = linea.rfind(": ");
                if (posNota != string::npos) {
                    try {
                        double nota = stod(linea.substr(posNota + 2));
                        sumaNotas += nota;
                        totalNotas++;
                    } catch (...) {
                        // Ignorar líneas con formato incorrecto
                    }
                }
            }
        }
        archivoEntrada.close();
        
        archivoSalida << left << setw(45) << cursos[curso] << "\t";
        
        if (totalNotas > 0) {
            double promedio = sumaNotas / totalNotas;
            archivoSalida << "Promedio: " << fixed << setprecision(2) << promedio << "  ";
            archivoSalida << (promedio >= 10.5 ? "[APROBADO]" : "[REPROBADO]");
        } else {
            archivoSalida << "Sin notas registradas";
        }
        archivoSalida << "\n";
    }
    
    archivoSalida.close();
    
    if (hayNotas) {
        cout << "\n" << GREEN << "¡Notas exportadas correctamente!" << RESET << "\n";
        cout << "Archivo: " << BOLD << nombreArchivo << RESET << "\n\n";
    } else {
        cout << "\n" << YELLOW << "No hay notas para exportar. Se ha creado un archivo vacío." << RESET << "\n\n";
    }
    
    cout << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
    _getch();
}

void verPromedios() {
    limpiarPantalla();
    mostrarEncabezado("PROMEDIOS DE NOTAS");
    
    string cursos[] = {"DESARROLLO FRONT END", 
                     "BASE DE DATOS", 
                     "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                     "ALGORITMOS Y PROGRAMACION DE COMPUTADORAS", 
                     "INGLES"};
    
    string colores[] = {CYAN, MAGENTA, GREEN, BLUE, YELLOW};
    bool hayNotas = false;
    
    cout << " " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n\n";
    cout << " " << string(70, '=') << "\n";
    
    // Para cada curso
    for (int curso = 0; curso < 5; curso++) {
        ifstream archivo(usuarioActivo + "_notas.txt");
        string linea;
        int totalNotas = 0;
        double sumaNotas = 0.0;
        
        // Buscar notas para este curso
        while (getline(archivo, linea)) {
            if (linea.find(cursos[curso]) != string::npos) {
                // Extraer la nota del formato: CURSO | Indicador de logro: X | Nota Y: Z
                size_t posNota = linea.rfind(": ");
                if (posNota != string::npos) {
                    try {
                        double nota = stod(linea.substr(posNota + 2));
                        sumaNotas += nota;
                        totalNotas++;
                        hayNotas = true;
                    } catch (...) {
                        // Ignorar líneas con formato incorrecto
                    }
                }
            }
        }
        archivo.close();
        
        // Mostrar el promedio para este curso
        cout << " " << BOLD << colores[curso] << "[" << curso+1 << "] " << left << setw(45) << cursos[curso] << RESET << "\t";
        
        if (totalNotas > 0) {
            double promedio = sumaNotas / totalNotas;
            cout << "Promedio: " << BOLD << fixed << setprecision(2) << promedio << RESET << "  ";
            
            // Mostrar estado (Aprobado/Reprobado)
            if (promedio >= 13.0) {
                cout << GREEN << "[APROBADO]" << RESET;
            } else {
                cout << RED << "[REPROBADO]" << RESET;
            }
            
            // Mostrar estrellas según el promedio (máximo 5 estrellas)
            cout << "  ";
            int estrellas = min(5, static_cast<int>(promedio / 4));
            cout << YELLOW;
            for (int i = 0; i < estrellas; i++) cout << "★";
            for (int i = estrellas; i < 5; i++) cout << "☆";
            cout << RESET;
        } else {
            cout << YELLOW << "Sin notas registradas" << RESET;
        }
        cout << "\n";
    }
    
    if (!hayNotas) {
        cout << "\n" << YELLOW << "No hay notas registradas para mostrar promedios." << RESET << "\n";
    }
    
    cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
    _getch();
}

void mostrarNotas() {
    string cursos[] = {"DESARROLLO FRONT END", 
                     "BASE DE DATOS", 
                     "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                     "ALGORITMOS Y PROGRAMACION DE COMPUTADORAS", 
                     "INGLES"};
    
    string colores[] = {CYAN, MAGENTA, GREEN, BLUE, YELLOW};
    
    // Mostrar lista de cursos con formato y colores
    limpiarPantalla();
    mostrarEncabezado("VER NOTAS");
    
    cout << " " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n\n";
    cout << " " << BOLD << "SELECCIONE EL CURSO:" << RESET << "\n";
    cout << " " << string(60, '=') << "\n";
    
    for (int i = 0; i < 5; ++i) {
        cout << " " << BOLD << colores[i] << "[" << (i+1) << "] " << RESET 
             << BOLD << colores[i] << cursos[i] << RESET << "\n";
    }
    cout << " " << string(60, '=') << "\n\n";

    int opcionCurso;
    cout << " " << BOLD << "Seleccione el curso (1-5): " << RESET;
    cin >> opcionCurso;
    cin.ignore();

    if (opcionCurso < 1 || opcionCurso > 5) {
        cout << "\n" << RED << "Error: Curso inválido.\n" << RESET;
        cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
        _getch();
        return;
    }

    string cursoSeleccionado = cursos[opcionCurso - 1];
    string colorCurso = colores[opcionCurso - 1];

    ifstream archivo(usuarioActivo + "_notas.txt");
    string linea;

    limpiarPantalla();
    mostrarEncabezado("NOTAS DE " + cursoSeleccionado);
    
    cout << "\n " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n";
    cout << " " << BOLD << "CURSO: " << RESET << BOLD << colorCurso << cursoSeleccionado << RESET << "\n\n";
    cout << " " << string(70, '=') << "\n";

    bool hayNotas = false;
    while (getline(archivo, linea)) {
        if (linea.find(cursoSeleccionado) != string::npos) {
            cout << " " << BOLD << colorCurso << "• " << RESET << linea << endl;
            hayNotas = true;
        }
    }

    if (!hayNotas) {
        cout << " " << YELLOW << "No hay notas registradas para este curso." << RESET << "\n";
    }

    cout << "\n " << string(70, '=') << "\n\n";
    cout << " " << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
    _getch();
}

void editarNota() {
    string cursos[] = {"DESARROLLO FRONT END", 
                     "BASE DE DATOS", 
                     "DESARROLLO DE APLICACIONES DE ESCRITORIO", 
                     "ALGORITMOS Y PROGRAMACION DE COMPUTADORAS", 
                     "INGLES"};
    
    string colores[] = {CYAN, MAGENTA, GREEN, BLUE, YELLOW};
    
    // Mostrar lista de cursos con formato y colores
    limpiarPantalla();
    mostrarEncabezado("EDITAR NOTA");
    
    cout << " " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n\n";
    cout << " " << BOLD << "SELECCIONE EL CURSO:" << RESET << "\n";
    cout << " " << string(60, '=') << "\n";
    
    for (int i = 0; i < 5; ++i) {
        cout << " " << BOLD << colores[i] << "[" << (i+1) << "] " << RESET 
             << BOLD << colores[i] << cursos[i] << RESET << "\n";
    }
    cout << " " << string(60, '=') << "\n\n";

    int opcionCurso;
    cout << " " << BOLD << "Seleccione el curso (1-5): " << RESET;
    cin >> opcionCurso;
    cin.ignore();

    if (opcionCurso < 1 || opcionCurso > 5) {
        cout << "\n" << RED << "Error: Curso inválido.\n" << RESET;
        cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
        _getch();
        return;
    }

    string cursoSeleccionado = cursos[opcionCurso - 1];
    string colorCurso = colores[opcionCurso - 1];
    
    // Leer todas las notas del usuario
    std::vector<std::string> lineasNotas;
    std::vector<int> indicesNotasCurso;
    std::string linea;
    int contador = 0;
    
    ifstream archivoEntrada(usuarioActivo + "_notas.txt");
    while (getline(archivoEntrada, linea)) {
        lineasNotas.push_back(linea);
        if (linea.find(cursoSeleccionado) != string::npos) {
            indicesNotasCurso.push_back(contador);
        }
        contador++;
    }
    archivoEntrada.close();
    
    if (indicesNotasCurso.empty()) {
        cout << "\n" << YELLOW << "No hay notas para editar en este curso.\n" << RESET;
        cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
        _getch();
        return;
    }
    
    // Mostrar notas del curso seleccionado
    limpiarPantalla();
    mostrarEncabezado("EDITAR NOTA - " + cursoSeleccionado);
    cout << " " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n\n";
    cout << " " << BOLD << "NOTAS DISPONIBLES:" << RESET << "\n";
    cout << " " << std::string(70, '=') << "\n";
    
    for (std::size_t i = 0; i < indicesNotasCurso.size(); i++) {
        cout << " " << BOLD << colorCurso << "[" << (i+1) << "] " << RESET 
             << BOLD << colorCurso << lineasNotas[indicesNotasCurso[i]] << RESET << "\n";
    }
    cout << " " << std::string(70, '=') << "\n\n";
    
    // Seleccionar nota a editar
    int opcionNota;
    cout << " " << BOLD << "Seleccione el número de la nota a editar (1-" << indicesNotasCurso.size() 
         << ") o 0 para cancelar: " << RESET;
    cin >> opcionNota;
    cin.ignore();
    
    if (opcionNota < 1 || opcionNota > static_cast<int>(indicesNotasCurso.size())) {
        cout << "\n" << YELLOW << "Operación cancelada o selección inválida.\n" << RESET;
        cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
        _getch();
        return;
    }
    
    // Obtener la nota seleccionada
    int indiceNota = indicesNotasCurso[opcionNota - 1];
    string notaActual = lineasNotas[indiceNota];
    
    // Mostrar detalles de la nota actual
    limpiarPantalla();
    mostrarEncabezado("EDITAR NOTA - " + cursoSeleccionado);
    cout << " " << BOLD << "USUARIO: " << RESET << GREEN << usuarioActivo << RESET << "\n\n";
    cout << " " << BOLD << "NOTA ACTUAL: " << RESET << BOLD << colorCurso << notaActual << RESET << "\n\n";
    
    // Solicitar nuevo indicador y nota
    int nuevoIndicador;
    double nuevaNota;
    
    cout << " " << BOLD << "Nuevo indicador de logro (1-5, 0 para mantener actual): " << RESET;
    string input;
    getline(cin, input);
    
    if (!input.empty()) {
        try {
            nuevoIndicador = stoi(input);
            if (nuevoIndicador < 1 || nuevoIndicador > 5) {
                cout << "\n" << RED << "Error: Indicador de logro debe estar entre 1 y 5.\n" << RESET;
                cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
                _getch();
                return;
            }
        } catch (...) {
            cout << "\n" << RED << "Error: Entrada inválida.\n" << RESET;
            cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
            _getch();
            return;
        }
    } else {
        // Extraer el indicador actual
        size_t posIndicador = notaActual.find("Indicador ");
        if (posIndicador != string::npos) {
            nuevoIndicador = stoi(notaActual.substr(posIndicador + 10, 1));
        } else {
            nuevoIndicador = 1; // Valor por defecto si no se puede extraer
        }
    }
    
    cout << " " << BOLD << "Nueva nota (0-20, presione Enter para mantener actual): " << RESET;
    getline(cin, input);
    
    if (!input.empty()) {
        try {
            nuevaNota = stod(input);
            if (nuevaNota < 0 || nuevaNota > 20) {
                cout << "\n" << RED << "Error: La nota debe estar entre 0 y 20.\n" << RESET;
                cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
                _getch();
                return;
            }
        } catch (...) {
            cout << "\n" << RED << "Error: Entrada inválida.\n" << RESET;
            cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
            _getch();
            return;
        }
    } else {
        // Extraer la nota actual
        size_t posNota = notaActual.rfind(": ");
        if (posNota != string::npos) {
            try {
                nuevaNota = stod(notaActual.substr(posNota + 2));
            } catch (...) {
                nuevaNota = 0.0; // Valor por defecto si no se puede extraer
            }
        } else {
            nuevaNota = 0.0;
        }
    }
    
    // Construir la nueva línea de nota
    std::stringstream nuevaLinea;
    nuevaLinea << cursoSeleccionado << " - Indicador " << nuevoIndicador << ": " << fixed << setprecision(2) << nuevaNota;
    
    // Actualizar la línea en el vector
    lineasNotas[indiceNota] = nuevaLinea.str();
    
    // Escribir todas las líneas de vuelta al archivo
    std::ofstream archivoSalida(usuarioActivo + "_notas.txt");
    if (archivoSalida.is_open()) {
        for (const std::string& linea : lineasNotas) {
            archivoSalida << linea << "\n";
        }
        archivoSalida.close();
        
        cout << "\n" << GREEN << "¡Nota actualizada correctamente!" << RESET << "\n";
        cout << " " << BOLD << "NUEVA NOTA: " << RESET << BOLD << colorCurso << nuevaLinea.str() << RESET << "\n\n";
    } else {
        cout << "\n" << RED << "Error al guardar los cambios.\n" << RESET;
    }
    
    cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
    _getch();
}

// Variables globales para el juego
int mejorPuntaje = 0;
string mejorJugador = "";

// Función para mostrar una barra de progreso
void mostrarBarraProgreso(int actual, int total, int ancho = 30) {
    float progreso = (float)actual / total;
    int pos = (int)(ancho * progreso);
    
    cout << "\n " << BOLD << "Progreso: [" << RESET;
    for (int i = 0; i < ancho; ++i) {
        if (i < pos) cout << GREEN << "█" << RESET;
        else cout << " ";
    }
    cout << BOLD << "] " << (int)(progreso * 100.0) << "%" << RESET << "\n\n";
}

// Función para mostrar un mensaje con un marco decorativo
void mostrarMensaje(const string& mensaje, const string& color = "") {
    string linea(mensaje.length() + 4, '=');
    cout << "\n " << BOLD << color << " ╔" << linea << "╗" << RESET << "\n";
    cout << " " << BOLD << color << " ║  " << mensaje << "  ║" << RESET << "\n";
    cout << " " << BOLD << color << " ╚" << linea << "╝" << RESET << "\n\n";
}

void juegoPreguntas() {
    limpiarPantalla();
    mostrarEncabezado("🎮 JUEGO DE PREGUNTAS DE PROGRAMACIÓN 🎮");

    // Definir preguntas y respuestas
    struct Pregunta {
        string texto;
        int correcta; // 1 o 2
        string opcion1;
        string opcion2;
        string explicacion; // Explicación de la respuesta correcta
    };

    vector<Pregunta> preguntasFaciles = {
        {"¿Qué operador se usa para la asignación en C++?", 1,
         "=",
         "==",
         "'=' asigna valor, '==' compara igualdad."},
    
        {"¿Qué palabra clave define una constante en C++?", 1,
         "const",
         "define",
         "'const' se usa para definir constantes."},
    
        {"¿Qué ciclo es mejor si conoces cuántas veces repetir?", 1,
         "for",
         "while",
         "'for' es ideal cuando conoces el número de repeticiones."},
    
        {"¿Qué indica 'endl' en C++?", 1,
         "Salto de línea",
         "Fin de programa",
         "'endl' inserta un salto de línea en la consola."},
    
        {"¿Qué simboliza '//' en C++?", 1,
         "Comentario de una línea",
         "División",
         "'//' se usa para comentarios de una sola línea."},
    
        {"¿Qué hace 'cin'?", 1,
         "Lee datos del teclado",
         "Muestra texto",
         "'cin' captura entrada desde el teclado."},
    
        {"¿Qué operador se usa para módulo en C++?", 1,
         "%",
         "&",
         "'%' devuelve el resto de una división."},
    
        {"¿Se puede usar 'else' sin 'if'?", 2,
         "Sí",
         "No",
         "'else' siempre debe ir después de un 'if'."},
    
        {"¿Cuál es el tipo de dato para números con decimales?", 1,
         "float",
         "int",
         "'float' permite almacenar números decimales."},
    
        {"¿Qué tipo de dato usar para verdadero/falso?", 1,
         "bool",
         "int",
         "'bool' almacena valores booleanos: true o false."},
    
        {"¿Cómo termina una instrucción en C++?", 1,
         "Con punto y coma (;)",
         "Con dos puntos (:)",
         "Cada instrucción en C++ debe terminar con ';'."},
    
        {"¿Es 'main' obligatorio en un programa C++?", 1,
         "Sí",
         "No",
         "Todo programa C++ comienza en la función 'main'."},
    
        {"¿Cuál es el operador de incremento?", 1,
         "++",
         "+=",
         "'++' incrementa una variable en 1."},
    
        {"¿Qué hace '#include'?", 1,
         "Incluye librerías",
         "Crea funciones",
         "'#include' inserta librerías o archivos al código."},
    
        {"¿'int' puede almacenar decimales?", 2,
         "Sí",
         "No",
         "'int' almacena enteros. Para decimales usa 'float' o 'double'."},
    
        {"¿Se puede anidar estructuras 'if'?", 1,
         "Sí",
         "No",
         "Puedes tener 'if' dentro de otros 'if'."},
    
        {"¿Cuál es el operador lógico AND?", 1,
         "&&",
         "&",
         "'&&' evalúa dos condiciones booleanas."},
    
        {"¿Una variable puede comenzar con número?", 2,
         "Sí",
         "No",
         "Una variable no puede iniciar con un número."},
    
        {"¿Qué estructura permite múltiples opciones?", 1,
         "switch",
         "if",
         "'switch' facilita elegir entre muchas opciones."},
    
        {"¿Qué se necesita para usar 'cout'?", 1,
         "iostream",
         "stdio.h",
         "Para usar 'cout' debes incluir <iostream>."},
        };

        vector<Pregunta> preguntasDificiles = {
            {"¿Qué significa que C++ es un lenguaje compilado?", 1,
             "1. Requiere ser convertido a código máquina",
             "2. Se ejecuta línea por línea",
             "Un compilador traduce todo el programa antes de ejecutarlo."},
        
            {"¿Qué hace una función recursiva?", 1,
             "1. Se llama a sí misma",
             "2. Se ejecuta una vez",
             "La recursividad implica que una función se invoque a sí misma."},
        
            {"¿Qué operador se sobrecarga para acceso tipo array?", 1,
             "1. []",
             "2. ()",
             "El operador [] puede sobrecargarse para acceder como si fuera arreglo."},
        
            {"¿Qué palabra clave impide sobrescribir una función virtual?", 1,
             "1. final",
             "2. override",
             "final evita que las clases hijas sobrescriban una función virtual."},
        
            {"¿Qué es el puntero 'this'?", 1,
             "1. Apunta al objeto actual",
             "2. Apunta a la clase base",
             "'this' representa al objeto que llama un método."},
        
            {"¿Qué permite la sobrecarga de constructores?", 1,
             "1. Varios constructores con diferentes parámetros",
             "2. Constructores duplicados en varias clases",
             "Permite crear objetos con diferentes formas de inicialización."},
        
            {"¿Para qué se usa 'friend' en C++?", 1,
             "1. Dar acceso a funciones externas a miembros privados",
             "2. Evitar el uso de punteros",
             "Permite que funciones o clases accedan a miembros privados."},
        
            {"¿Qué tipo de herencia permite heredar de varias clases?", 1,
             "1. Herencia múltiple",
             "2. Herencia virtual",
             "La herencia múltiple combina miembros de varias clases base."},
        
            {"¿Dónde deben definirse las plantillas?", 1,
             "1. En el archivo de cabecera (.h)",
             "2. En un archivo .cpp",
             "Deben definirse completamente en encabezados por cómo funciona la compilación."},
        
            {"¿Qué contiene la clase std::map?", 1,
             "1. Elementos ordenados por clave",
             "2. Elementos desordenados",
             "std::map mantiene los elementos ordenados por clave automáticamente."},
        
            {"¿Qué es una función virtual pura?", 1,
             "1. Una función sin implementación en la clase base",
             "2. Una función privada",
             "Se define con '= 0' y obliga a implementarla en las clases derivadas."},
        
            {"¿Qué es un 'segmentation fault'?", 1,
             "1. Acceso a memoria no válida",
             "2. División entre cero",
             "Se produce al acceder a memoria prohibida o inexistente."},
        
            {"¿Para qué se usa el operador '->'?", 1,
             "1. Acceder a miembros de un objeto desde un puntero",
             "2. Crear una referencia",
             "Se usa con punteros a objetos para acceder a sus miembros."},
        
            {"¿Diferencia entre 'delete' y 'delete[]'?", 1,
             "1. delete[] libera arreglos dinámicos",
             "2. No hay diferencia",
             "delete[] debe usarse para arreglos creados con new[]."},
        
            {"¿Qué pasa si no defines un destructor virtual en una clase base?", 1,
             "1. Puede causar fugas de memoria",
             "2. No se puede crear el objeto",
             "Al liberar un objeto derivado desde un puntero base, solo se llama al destructor base."},
        
            {"¿Qué memoria usa 'new'?", 1,
             "1. Heap",
             "2. Stack",
             "new reserva memoria dinámica en el heap."},
        
            {"¿Qué es una clase abstracta?", 1,
             "1. Clase con al menos una función virtual pura",
             "2. Clase sin métodos",
             "No se puede instanciar, sirve como base para herencia."},
        
            {"¿Qué permite el polimorfismo?", 1,
             "1. Tratar objetos derivados como base",
             "2. Crear múltiples objetos iguales",
             "Permite invocar métodos en objetos derivados a través de punteros base."},
        
            {"¿Qué es 'linkage externo'?", 1,
             "1. Compartir variables o funciones entre archivos fuente",
             "2. Hacer variables privadas",
             "Se logra con la palabra clave extern."}
        };
        

    int aciertos = 0;
    vector<Pregunta> preguntasActuales;
    string dificultad;
    int tiempoRespuesta = 0;
    time_t inicio, fin;

    // Pantalla de selección de dificultad
    while (true) {
        limpiarPantalla();
        mostrarEncabezado("🎮 SELECCIONA DIFICULTAD 🎮");

        cout << "\n " << BOLD << CYAN << "¡Hola, " << usuarioActivo << "!" << RESET << "\n\n";
        
        // Mostrar mejor puntuación con estilo
        cout << " " << BOLD << "🏆 Mejor puntuación: " << RESET << BOLD << mejorPuntaje << " aciertos" << RESET;
        if (!mejorJugador.empty()) {
            cout << " (por " << BOLD << MAGENTA << mejorJugador << RESET << ")";
        }
        cout << "\n " << string(60, '=') << "\n\n";

        // Opciones de dificultad con mejor formato
        cout << " " << BOLD << "Elige la dificultad:" << RESET << "\n\n";
        cout << " " << BOLD << "[1] " << RESET << "Normal   " << CYAN << "(Fácil para empezar)" << RESET << "\n";
        cout << " " << BOLD << "[2] " << RESET << "Difícil  " << MAGENTA << "(Para expertos en C++)" << RESET << "\n";
        cout << " " << BOLD << "[0] " << RESET << "Volver al menú principal\n\n";

        int opcionDificultad;
        cout << " " << BOLD << "Tu elección: " << RESET;
        cin >> opcionDificultad;
        cin.ignore();

        if (opcionDificultad == 0) return;

        if (opcionDificultad == 1) {
            preguntasActuales = preguntasFaciles;
            dificultad = "NORMAL";
            break;
        } else if (opcionDificultad == 2) {
            preguntasActuales = preguntasDificiles;
            dificultad = "DIFÍCIL";
            break;
        } else {
            mostrarMensaje("Opción inválida. Intenta de nuevo.", RED);
            Sleep(1000);
        }
    }

    // Mezclar preguntas
    srand((unsigned int)time(0));
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(preguntasActuales), std::end(preguntasActuales), rng);
    
    // Tomar solo 5 preguntas
    if (preguntasActuales.size() > 5) {
        preguntasActuales.resize(5);
    }

    // Juego principal
    for (size_t i = 0; i < preguntasActuales.size(); i++) {
        limpiarPantalla();
        mostrarEncabezado("PREGUNTA #" + to_string(i + 1) + " - " + dificultad + " ❓");
        
        // Mostrar barra de progreso
        mostrarBarraProgreso(i, preguntasActuales.size());
        
        Pregunta& p = preguntasActuales[i];
        
        // Mostrar pregunta con formato
        cout << "\n " << BOLD << BLUE << "Pregunta:" << RESET << "\n";
        cout << " " << BOLD << p.texto << RESET << "\n\n";
        
        // Mostrar opciones con mejor formato
        cout << " " << BOLD << CYAN << "Opciones:" << RESET << "\n";
        cout << " " << BOLD << "[1] " << RESET << p.opcion1 << "\n";
        cout << " " << BOLD << "[2] " << RESET << p.opcion2 << "\n\n";

        // Obtener respuesta del usuario
        time(&inicio);
        int respuesta = 0;
        while (respuesta != 1 && respuesta != 2) {
            cout << " " << BOLD << "Tu respuesta (1/2): " << RESET;
            string input;
            getline(cin, input);
            
            // Validar entrada
            if (input == "1" || input == "2") {
                respuesta = stoi(input);
            } else {
                cout << " " << RED << "Por favor, ingresa solo 1 o 2." << RESET << "\n";
            }
        }
        time(&fin);
        tiempoRespuesta += difftime(fin, inicio);

        // Verificar respuesta
        limpiarPantalla();
        mostrarEncabezado(respuesta == p.correcta ? "RESPUESTA CORRECTA" : "RESPUESTA INCORRECTA");
        
        cout << "\n " << BOLD << BLUE << "Pregunta:" << RESET << "\n";
        cout << " " << p.texto << "\n\n";
        
        cout << " " << BOLD << (respuesta == p.correcta ? GREEN : RED) << 
             (respuesta == p.correcta ? "Tu respuesta fue correcta!" : "Tu respuesta fue incorrecta") << RESET << "\n";
        
        cout << "\n " << BOLD << "Respuesta correcta: " << RESET << "[" << p.correcta << "] " << 
             (p.correcta == 1 ? p.opcion1 : p.opcion2) << "\n";
        
        cout << "\n " << BOLD << "Explicación: " << RESET << p.explicacion << "\n\n";
        
        if (respuesta == p.correcta) {
            aciertos++;
            cout << " " << GREEN << "¡Bien hecho! " << RESET << "Llevas " << BOLD << aciertos << " aciertos" << RESET << "\n";
        } else {
            cout << " " << RED << "¡No te rindas! " << RESET << "Llevas " << BOLD << aciertos << " aciertos" << RESET << "\n";
        }

        // Mostrar progreso
        mostrarBarraProgreso(i + 1, preguntasActuales.size());
        
        if (i < preguntasActuales.size() - 1) {
            cout << "\n " << YELLOW << "Tiempo en responder: " << (int)difftime(fin, inicio) << " segundos" << RESET << "\n";
            cout << "\n " << YELLOW << "Presiona Enter para la siguiente pregunta..." << RESET;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cout << "\n " << BOLD << MAGENTA << "🎉 ¡Última pregunta completada!" << RESET << "\n";
            cout << " " << YELLOW << "Tiempo promedio por respuesta: " << (tiempoRespuesta / preguntasActuales.size()) << " segundos" << RESET << "\n";
            cout << "\n " << YELLOW << "Presiona Enter para ver tus resultados finales..." << RESET;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Pantalla de resultados finales
    limpiarPantalla();
    string mensajeResultado;
    
    if (aciertos == preguntasActuales.size()) {
        mensajeResultado = "¡PERFECTO! Eres un experto en programación";
    } else if (aciertos >= preguntasActuales.size() * 0.7) {
        mensajeResultado = "¡Excelente trabajo! Sigue así";
    } else if (aciertos >= preguntasActuales.size() * 0.4) {
        mensajeResultado = "Buen intento, sigue practicando";
    } else {
        mensajeResultado = "No te rindas, la práctica hace al maestro";
    }
    
    mostrarEncabezado(" RESULTADO FINAL " );
    
    cout << "\n " << BOLD << "Jugador: " << RESET << MAGENTA << usuarioActivo << RESET << "\n";
    cout << " " << BOLD << "Dificultad: " << RESET << (dificultad == "NORMAL" ? CYAN : MAGENTA) << dificultad << RESET << "\n";
    cout << " " << BOLD << "Aciertos: " << RESET << BOLD << 
         (aciertos == preguntasActuales.size() ? GREEN : (aciertos >= preguntasActuales.size() / 2 ? YELLOW : RED)) << 
         aciertos << " de " << preguntasActuales.size() << RESET << "\n";
    cout << " " << BOLD << "Tiempo promedio: " << RESET << (tiempoRespuesta / preguntasActuales.size()) << " segundos por respuesta\n";
    
    // Mostrar barra de progreso final
    cout << "\n " << BOLD << "Progreso: " << RESET << "\n ";
    float porcentaje = (float)aciertos / preguntasActuales.size();
    int anchoBarra = 40;
    int pos = (int)(anchoBarra * porcentaje);
    
    cout << "[";
    for (int i = 0; i < anchoBarra; i++) {
        if (i < pos) {
            if (porcentaje > 0.7) cout << GREEN << "█" << RESET;
            else if (porcentaje > 0.4) cout << YELLOW << "█" << RESET;
            else cout << RED << "█" << RESET;
        } else {
            cout << " ";
        }
    }
    cout << "] " << (int)(porcentaje * 100) << "%\n\n";
    
    // Actualizar mejor puntuación si corresponde
    if (aciertos > mejorPuntaje) {
        mejorPuntaje = aciertos;
        mejorJugador = usuarioActivo;
        cout << " " << BOLD << GREEN << " ¡Nueva mejor puntuación! " << RESET << "\n\n";
    }
    
    // Mostrar mensaje final
    cout << "\n " << BOLD << (porcentaje > 0.7 ? GREEN : (porcentaje > 0.4 ? YELLOW : RED)) << 
         " " << mensajeResultado << " "<< RESET << "\n";

    // Mostrar sugerencia basada en el rendimiento
    cout << "\n " << BOLD << "Consejo: " << RESET;
    if (porcentaje == 1.0) {
        cout << "¡Eres increíble! ¿Por qué no intentas la dificultad máxima?" << "\n";
    } else if (porcentaje > 0.7) {
        cout << "Estás muy cerca de la perfección. ¡Solo un poco más!" << "\n";
    } else if (porcentaje > 0.4) {
        cout << "Sigue practicando los conceptos básicos para mejorar." << "\n";
    } else {
        cout << "No te desanimes, revisa los conceptos básicos y vuelve a intentarlo." << "\n";
    }

    cout << "\n " << string(60, '=') << "\n\n";
    cout << " " << YELLOW << "Presiona cualquier tecla para volver al menú..." << RESET;
    _getch();
}

void mostrarOpcionMenu(const string& texto, int numero, bool seleccionada) {
    string margen = string(20, ' ');
    if (seleccionada) {
        cout << BOLD << GREEN << "  -> " << margen << "|" << numero << "| " << texto << "\n" << RESET;
    } else {
        cout << "    " << margen << "|" << numero << "| " << texto << "\n";
    }
}

bool cambiarContrasena(const string& usuario) {
    string viejaClave, nuevaClave, confirmarClave;
    bool claveCorrecta = false;
    
    // Verificar la contraseña actual
    do {
        limpiarPantalla();
        mostrarEncabezado("CAMBIAR CONTRASEÑA");
        
        cout << " " << BOLD << "Usuario: " << RESET << usuario << "\n\n";
        cout << " " << BOLD << "Ingrese su contraseña actual: " << RESET;
        
        // Ocultar la contraseña
        char c;
        viejaClave = "";
        while ((c = _getch()) != '\r') {
            if (c == '\b' && !viejaClave.empty()) { // Manejar retroceso
                cout << "\b \b";
                viejaClave.pop_back();
            } else if (c != '\b') {
                cout << '*';
                viejaClave += c;
            }
        }
        
        // Verificar la contraseña actual
        ifstream archivo("usuarios.txt");
        string linea;
        bool encontrado = false;
        
        while (getline(archivo, linea)) {
            size_t pos = linea.find(',');
            if (pos != string::npos && linea.substr(0, pos) == usuario) {
                encontrado = true;
                string claveGuardada = linea.substr(pos + 1);
                if (viejaClave == claveGuardada) {
                    claveCorrecta = true;
                    break;
                }
            }
        }
        archivo.close();
        
        if (!claveCorrecta) {
            cout << "\n\n" << RED << "Contraseña incorrecta. Intente de nuevo." << RESET << endl;
            cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
            _getch();
        }
    } while (!claveCorrecta);
    
    // Solicitar nueva contraseña
    bool coinciden = false;
    do {
        limpiarPantalla();
        mostrarEncabezado("CAMBIAR CONTRASEÑA");
        cout << " " << BOLD << "Usuario: " << RESET << usuario << "\n";
        cout << " " << BOLD << "Contraseña actual: " << string(viejaClave.length(), '*') << "\n\n";
        
        cout << " " << BOLD << "Ingrese la nueva contraseña: " << RESET;
        nuevaClave = "";
        char c;
        while ((c = _getch()) != '\r') {
            if (c == '\b' && !nuevaClave.empty()) {
                cout << "\b \b";
                nuevaClave.pop_back();
            } else if (c != '\b') {
                cout << '*';
                nuevaClave += c;
            }
        }
        
        cout << "\n " << BOLD << "Confirme la nueva contraseña: " << RESET;
        confirmarClave = "";
        while ((c = _getch()) != '\r') {
            if (c == '\b' && !confirmarClave.empty()) {
                cout << "\b \b";
                confirmarClave.pop_back();
            } else if (c != '\b') {
                cout << '*';
                confirmarClave += c;
            }
        }
        
        if (nuevaClave != confirmarClave) {
            cout << "\n\n" << RED << "Las contraseñas no coinciden. Intente de nuevo." << RESET << endl;
            cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
            _getch();
        } else if (nuevaClave.empty()) {
            cout << "\n\n" << RED << "La contraseña no puede estar vacía." << RESET << endl;
            cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
            _getch();
        } else {
            coinciden = true;
        }
    } while (!coinciden);
    
    // Actualizar la contraseña en el archivo
    ifstream archivoEntrada("usuarios.txt");
    ofstream archivoTemp("temp_usuarios.txt");
    string linea;
    bool actualizado = false;
    
    while (getline(archivoEntrada, linea)) {
        size_t pos = linea.find(',');
        if (pos != string::npos && linea.substr(0, pos) == usuario) {
            archivoTemp << usuario << "," << nuevaClave << endl;
            actualizado = true;
        } else {
            archivoTemp << linea << endl;
        }
    }
    
    archivoEntrada.close();
    archivoTemp.close();
    
    if (actualizado) {
        remove("usuarios.txt");
        rename("temp_usuarios.txt", "usuarios.txt");
        cout << "\n\n" << GREEN << "¡Contraseña actualizada correctamente!" << RESET << endl;
    } else {
        remove("temp_usuarios.txt");
        cout << "\n\n" << RED << "Error al actualizar la contraseña." << RESET << endl;
    }
    
    cout << "\n" << YELLOW << "Presione cualquier tecla para continuar..." << RESET;
    _getch();
    return actualizado;
}

void menuUsuario() {
    int opcion = 1;
    int tecla;
    const int OPCIONES = 8;
    
    do {
        limpiarPantalla();
        mostrarEncabezado("MENÚ PRINCIPAL");
        
        // Mostrar información del usuario
        cout << " " << BOLD << "Usuario:" << RESET << " " << GREEN << usuarioActivo << RESET << "\n\n";
        
        // Mostrar opciones del menú
        mostrarOpcionMenu("Registrar Nota", 1, opcion == 1);
        mostrarOpcionMenu("Ver Notas", 2, opcion == 2);
        mostrarOpcionMenu("Editar Nota", 3, opcion == 3);
        mostrarOpcionMenu("Ver Promedios", 4, opcion == 4);
        mostrarOpcionMenu("Exportar Notas", 5, opcion == 5);
        mostrarOpcionMenu("Cambiar Contraseña", 6, opcion == 6);
        mostrarOpcionMenu("Juego de Preguntas", 7, opcion == 7);
        mostrarOpcionMenu("Cerrar Sesión", 8, opcion == 8);
        
        cout << "\n" << YELLOW << " ↑/↓: Navegar   ENTER: Seleccionar   ESC: Salir" << RESET;

        // Leer tecla sin esperar por Enter
        tecla = _getch();
        
        // Manejar teclas de flecha
        if (tecla == 0 || tecla == 224) { // Código de tecla extendida
            tecla = _getch();
            switch(tecla) {
                case 72: // Flecha arriba
                    opcion = (opcion > 1) ? opcion - 1 : OPCIONES;
                    break;
                case 80: // Flecha abajo
                    opcion = (opcion < OPCIONES) ? opcion + 1 : 1;
                    break;
            }
        } 
        // Si se presiona Enter
        else if (tecla == 13) {
            limpiarPantalla();
            
            switch (opcion) {
                case 1:
                    mostrarEncabezado("REGISTRAR NOTA");
                    registrarNota(usuarioActivo);
                    break;
                case 2:
                    mostrarEncabezado("VER NOTAS");
                    mostrarNotas();
                    break;
                case 3:
                    editarNota();
                    break;
                case 4:
                    verPromedios();
                    break;
                case 5:
                    exportarNotas();
                    break;
                case 6:
                    cambiarContrasena(usuarioActivo);
                    break;
                case 7:
                    juegoPreguntas();
                    break;
                case 8:
                    cout << "\n" << YELLOW << "Cerrando sesión..." << RESET << "\n";
                    Sleep(1000);
                    return;
            }
            
            cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
            _getch();
        }
        // Si se presiona ESC
        else if (tecla == 27) {
            opcion = 8; // Salir
        }
    } while (true);
}


void mostrarOpcionLogin(const string& texto, int numero, bool seleccionada = false) {
    string margen = string(25, ' ');
    if (seleccionada) {
        cout << BOLD << GREEN << "  > " << margen << " |" << numero << "| " << texto << "\n" << RESET;
    } else {
        cout << "    " << margen << "|" << numero << "| " << texto << "\n";
    }
}

void menuLogin() {
    string usuario, clave;
    int opcion = 1;
    int tecla;
    const int OPCIONES = 3;
    bool salir = false;

    do {
        limpiarPantalla();
        mostrarEncabezado("BIENVENIDO");
        cout << " \n";
        cout << " \n";
        // Mostrar opciones del menú
        mostrarOpcionLogin("Registrarse", 1, opcion == 1);
        mostrarOpcionLogin("Iniciar Sesión", 2, opcion == 2);
        mostrarOpcionLogin("Salir", 3, opcion == 3);
        
        cout << "\n" << YELLOW << " ↑/↓: Navegar   ENTER: Seleccionar   ESC: Salir" << RESET;

        // Leer tecla sin esperar por Enter
        tecla = _getch();
        
        // Manejar teclas de flecha
        if (tecla == 0 || tecla == 224) { // Código de tecla extendida
            tecla = _getch();
            switch(tecla) {
                case 72: // Flecha arriba
                    opcion = (opcion > 1) ? opcion - 1 : OPCIONES;
                    break;
                case 80: // Flecha abajo
                    opcion = (opcion < OPCIONES) ? opcion + 1 : 1;
                    break;
            }
        } 
        // Si se presiona Enter
        else if (tecla == 13) {
            limpiarPantalla();
            
            switch (opcion) {
                case 1: { // Registrarse
                    mostrarEncabezado("REGISTRO DE USUARIO");
                    cout << "\n " << BOLD << "Nuevo usuario: " << RESET;
                    cin >> usuario;
                    cout << " " << BOLD << "Clave: " << RESET;
                    cin >> clave;
                    registrarUsuario(usuario, clave);
                    cout << "\n" << GREEN << "Usuario registrado correctamente.\n" << RESET;
                    cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
                    _getch();
                    break;
                }
                case 2: { // Iniciar sesión
                    mostrarEncabezado("INICIAR SESIÓN");
                    cout << "\n " << BOLD << "Usuario: " << RESET;
                    cin >> usuario;
                    cout << " " << BOLD << "Clave: " << RESET;
                    cin >> clave;
                    
                    if (verificarLogin(usuario, clave)) {
                        cout << "\n" << GREEN << "✓ Iniciando sesión...\n" << RESET;
                        Sleep(1000);
                        menuUsuario();
                    } else {
                        cout << "\n" << RED << "Usuario o clave incorrectos.\n" << RESET;
                        cout << "\n" << YELLOW << "Presione una tecla para continuar..." << RESET;
                        _getch();
                    }
                    break;
                }
                case 3: // Salir
                    salir = true;
                    break;
            }
        }
        // Si se presiona ESC
        else if (tecla == 27) {
            salir = true;
        }
    } while (!salir);
}

// Función para configurar la consola para soportar caracteres especiales
void configurarConsola() {
    // Configurar la consola para soportar caracteres Unicode
    SetConsoleOutputCP(CP_UTF8);
    
    // Configurar la fuente de la consola para soportar caracteres especiales
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    
    // Habilitar modo de salida de consola ENABLE_VIRTUAL_TERMINAL_PROCESSING
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

int main() {
    // Configurar la consola
    configurarConsola();
    
    // Iniciar la aplicación
    menuLogin();
    
    // Limpiar y salir
    limpiarPantalla();
    cout << "Hasta pronto...\n";
    return 0;
}
