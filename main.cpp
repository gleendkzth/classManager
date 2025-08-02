#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
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

void juegoPreguntas() {
    limpiarPantalla();
    mostrarEncabezado("🧠 BUG O VERDAD 🎮");
    
    // Definir preguntas y respuestas
    struct Pregunta {
        string texto;
        char correcta; // 'A' o 'B'
        string opcionA;
        string opcionB;
    };
    
    vector<Pregunta> preguntasFaciles = {
        {"¿Qué hace 'cout' en C++?", 'A', 
         "Muestra datos por pantalla", 
         "Captura datos del teclado"},
        
        {"¿Cuál es el tipo de dato correcto para letras en C++?", 'A', 
         "char", 
         "string"},
        
        {"¿El ciclo 'for' puede ejecutarse 0 veces?", 'A', 
         "Sí", 
         "No"},
        
        {"¿HTML es un lenguaje de programación?", 'B', 
         "Sí", 
         "No"},
        
        {"¿Un archivo con extensión .cpp pertenece a C++?", 'A', 
         "Sí", 
         "No"}
    };
    
    vector<Pregunta> preguntasDificiles = {
        {"¿Qué hace el operador '&' en C++?", 'A', 
         "Obtiene la dirección de una variable", 
         "Declara una variable global"},
        
        {"¿Cuál es la principal ventaja de usar punteros?", 'A', 
         "Acceder y manipular memoria directamente", 
         "Evitar usar bucles"},
        
        {"¿Qué significa STL en C++?", 'B', 
         "Standard Template Language", 
         "Standard Template Library"},
        
        {"¿Puedes sobrecargar el operador '==' en C++?", 'A', 
         "Sí", 
         "No"},
        
        {"¿Qué hace `delete` en C++?", 'A', 
         "Libera memoria dinámica", 
         "Cierra archivos abiertos"}
    };
    
    int aciertos = 0;
    vector<Pregunta> preguntasActuales;
    string dificultad;
    
    // Mostrar menú de dificultad
    limpiarPantalla();
    mostrarEncabezado("SELECCIONA DIFICULTAD");
    
    cout << "\n " << BOLD << CYAN << "¡Hola, " << usuarioActivo << "!" << RESET << "\n\n";
    cout << " " << BOLD << "Mejor puntuación: " << RESET << mejorPuntaje << " aciertos";
    if (!mejorJugador.empty()) {
        cout << " (por " << mejorJugador << ")";
    }
    cout << "\n " << string(60, '=') << "\n\n";
    
    cout << " " << BOLD << "Elige la dificultad:" << RESET << "\n\n";
    cout << " " << BOLD << "[1]" << RESET << " 🧾 Normal\n";
    cout << " " << BOLD << "[2]" << RESET << " 🧠 Difícil\n";
    cout << " " << BOLD << "[0]" << RESET << " 🔙 Volver al menú\n\n";
    
    int opcionDificultad;
    cout << " " << BOLD << "👉 Tu elección: " << RESET;
    cin >> opcionDificultad;
    cin.ignore();
    
    if (opcionDificultad == 0) return;
    
    if (opcionDificultad == 1) {
        preguntasActuales = preguntasFaciles;
        dificultad = "NORMAL";
    } else if (opcionDificultad == 2) {
        preguntasActuales = preguntasDificiles;
        dificultad = "DIFÍCIL";
    } else {
        cout << "\n " << RED << "Opción inválida. Volviendo al menú..." << RESET;
        Sleep(1500);
        return;
    }
    
    // Mezclar preguntas
    srand(time(0));
    for (int i = 0; i < 3; i++) {
        limpiarPantalla();
        mostrarEncabezado("PREGUNTA #" + to_string(i+1) + " - " + dificultad);
        
        // Seleccionar pregunta aleatoria
        int idx = rand() % preguntasActuales.size();
        Pregunta p = preguntasActuales[idx];
        
        // Mostrar pregunta
        cout << "\n " << BOLD << MAGENTA << p.texto << RESET << "\n\n";
        cout << " " << BOLD << "[A] " << RESET << p.opcionA << "\n";
        cout << " " << BOLD << "[B] " << RESET << p.opcionB << "\n\n";
        
        // Obtener respuesta
        char respuesta;
        bool respuestaValida = false;
        
        while (!respuestaValida) {
            cout << " " << BOLD << "👉 Tu respuesta (A/B): " << RESET;
            cin >> respuesta;
            cin.ignore();
            
            respuesta = toupper(respuesta);
            if (respuesta == 'A' || respuesta == 'B') {
                respuestaValida = true;
            } else {
                cout << " " << RED << "Por favor, ingresa 'A' o 'B'." << RESET << "\n";
            }
        }
        
        // Verificar respuesta
        if (respuesta == p.correcta) {
            cout << "\n " << GREEN << "✅ ¡Correcto!" << RESET << "\n";
            aciertos++;
        } else {
            cout << "\n " << RED << "❌ Incorrecto. La respuesta era: [" << p.correcta << "]" << RESET << "\n";
        }
        
        cout << "\n " << YELLOW << "Aciertos: " << aciertos << " de " << (i+1) << RESET << "\n";
        
        if (i < 2) {
            cout << "\n " << YELLOW << "Presiona Enter para la siguiente pregunta..." << RESET;
            cin.get();
        }
        
        // Eliminar la pregunta para no repetir
        preguntasActuales.erase(preguntasActuales.begin() + idx);
    }
    
    // Mostrar resultado final
    limpiarPantalla();
    mostrarEncabezado("🏆 RESULTADO FINAL 🏆");
    
    cout << "\n " << BOLD << "Jugador: " << RESET << usuarioActivo << "\n";
    cout << " " << BOLD << "Dificultad: " << RESET << dificultad << "\n";
    cout << " " << BOLD << "Aciertos: " << RESET << aciertos << " de 3\n\n";
    
    // Actualizar mejor puntuación si es necesario
    if (aciertos > mejorPuntaje) {
        mejorPuntaje = aciertos;
        mejorJugador = usuarioActivo;
        cout << " " << BOLD << GREEN << "🎉 ¡Nueva mejor puntuación! 🎉" << RESET << "\n\n";
    }
    
    // Mostrar mensaje según puntuación
    if (aciertos == 3) {
        cout << " " << BOLD << GREEN << "🔥 ¡Perfecto! Eres un crack de la programación!" << RESET << "\n";
    } else if (aciertos >= 2) {
        cout << " " << BOLD << YELLOW << "👍 ¡Buen trabajo! Sigue practicando." << RESET << "\n";
    } else if (aciertos == 1) {
        cout << " " << BOLD << YELLOW << "🤔 No está mal, pero puedes hacerlo mejor." << RESET << "\n";
    } else {
        cout << " " << BOLD << RED << "💀 El compilador te odia. ¡Sigue intentándolo!" << RESET << "\n";
    }
    
    cout << "\n " << string(60, '=') << "\n\n";
    cout << " " << YELLOW << "Presiona cualquier tecla para volver al menú..." << RESET;
    _getch();
}

void mostrarOpcionMenu
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
