#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

using namespace std;

// Estructuras para almacenar datos
unordered_map<string, vector<int>> tokenToDocuments; // Token -> IDs de documentos
unordered_map<int, string> documentIDToName;         // Document ID -> Nombre del archivo

// Función para convertir a minúsculas
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Cargar diccionario desde el archivo
void loadDictionary(const string& dictionaryFile) {
    ifstream inFile(dictionaryFile);
    if (!inFile) {
        cerr << "Error: No se pudo abrir el archivo de diccionario." << endl;
        exit(1);
    }
    string token;
    int docID;
    while (inFile >> token >> docID) {
        tokenToDocuments[token].push_back(docID);
    }
    inFile.close();
}

// Cargar documento desde el archivo
void loadDocuments(const string& documentsFile) {
    ifstream inFile(documentsFile);
    if (!inFile) {
        cerr << "Error: No se pudo abrir el archivo de documentos." << endl;
        exit(1);
    }
    int docID;
    string docName;
    while (inFile >> docID >> docName) {
        documentIDToName[docID] = docName;
    }
    inFile.close();
}

// Buscar un token en el diccionario y mostrar los documentos correspondientes
void searchToken(const string& token) {
    string lowerToken = toLowerCase(token);
    auto it = tokenToDocuments.find(lowerToken);
    if (it != tokenToDocuments.end()) {
        cout << "Documentos que contienen el token '" << token << "':" << endl;
        for (int docID : it->second) {
            cout << "- " << documentIDToName[docID] << endl;
        }
    } else {
        cout << "El token '" << token << "' no se encontró en el diccionario." << endl;
    }
}

// Función principal
int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " <archivo_diccionario> <archivo_documentos> <token>" << endl;
        return 1;
    }

    string dictionaryFile = argv[1];
    string documentsFile = argv[2];
    string token = argv[3];

    auto startTime = chrono::high_resolution_clock::now();

    // Cargar archivos
    loadDictionary(dictionaryFile);
    loadDocuments(documentsFile);

    // Realizar búsqueda
    searchToken(token);

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;

    // Registrar tiempos en el log
    ofstream logFile("a12_matricula.txt", ios::app);
    if (logFile) {
        logFile << "Token: " << token << ", Tiempo de búsqueda: " << elapsed.count() << " segundos" << endl;
        logFile.close();
    }

    return 0;
}
