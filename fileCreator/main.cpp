/*-----------------------------------*
 * EtipiAdventure by Benjamin Bourge *
 *-----------------------------------*/

#include "fileCreator.hpp"

int main()
{
    std::cout << "Hello, World! I'm the file creator." << std::endl;
    std::string path;

    while (std::cin >> path)
    {
        std::string truePath;
        std::string type = path.substr(0, path.find(":"));
        std::cout << "Type: " << type << std::endl;
        if (type == "Component") {
            std::string className = path.substr(path.find(":") + 1);
            std::string lowerName = className;
            lowerName[0] = std::tolower(lowerName[0]);
            truePath = "../src/content/components/" + lowerName + ".cpp";

            std::ofstream file(truePath);
            file << "/*-----------------------------------*\n";
            file << " * CardsOnline by Benjamin Bourge *\n";
            file << " *-----------------------------------*/\n";
            file << "\n";
            file << "#include \"../../../include/components/" << lowerName << ".hpp\"\n\n";
            file << className << "::" << className << "()\n";
            file << "{\n\n";
            file << "}\n\n";
            file << className << "::~" << className << "()\n";
            file << "{\n\n";
            file << "}\n";
            std::cout << "Fichier créé avec succès : " << truePath << std::endl;
            file.close();

            std::string headerTruePath = "../include/components/" + lowerName + ".hpp";
            std::ofstream headerFile(headerTruePath);
            headerFile << "/*-----------------------------------*\n";
            headerFile << " * CardsOnline by Benjamin Bourge *\n";
            headerFile << " *-----------------------------------*/\n";
            headerFile << "\n";
            headerFile << "#pragma once\n";
            headerFile << "#include \"../core/component.hpp\"\n\n";
            headerFile << "class " << className << " : public Component {\n";
            headerFile << "public:\n";
            headerFile << "    " << className << "();\n";
            headerFile << "    ~" << className << "();\n";
            headerFile << "};\n";

            std::cout << "Fichier créé avec succès : " << headerTruePath << std::endl;
            headerFile.close();

            std::ifstream inFile("../src/core/componentManager.cpp");

            if (!inFile.is_open()) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier ComponentManager." << std::endl;
                return 1;
            }

            std::string line;
            std::string output;
            bool initFound = false;

            while (std::getline(inFile, line)) {
                if (line.find("void ComponentManager::init()") != std::string::npos) {
                    initFound = true;
                } else if (initFound && line.find("}") != std::string::npos) {
                    std::string newLine = "    registerComponent<" + className + ">();\n";
                    output += newLine;
                    initFound = false; // Resetting the flag
                }
                output += line + "\n";
            }

            inFile.close();

            std::ofstream outFile("../src/core/componentManager.cpp");
            if (!outFile.is_open()) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier pour l'écriture." << std::endl;
                return 1;
            }

            outFile << output;
            outFile.close();

            std::cout << "Ligne insérée avec succès dans le fichier : " << className << std::endl;

            std::ofstream outFileH("../include/core/myComponents.hpp", std::ios_base::app);

            if (!outFileH.is_open()) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier pour l'écriture." << std::endl;
                return 1;
            }

            std::string newHeader = "#include \"../components/" + lowerName + ".hpp\"\n";
            outFileH << newHeader << std::endl;
            outFileH.close();

            std::cout << "Ligne insérée avec succès à la fin des headers : " << className << std::endl;
        } else if (type == "System") {
            std::string className = path.substr(path.find(":") + 1);
            std::string lowerName = className;
            lowerName[0] = std::tolower(lowerName[0]);
            truePath = "../src/content/systems/" + lowerName + ".cpp";

            std::ofstream file(truePath);
            file << "/*-----------------------------------*\n";
            file << " * CardsOnline by Benjamin Bourge *\n";
            file << " *-----------------------------------*/\n";
            file << "\n";
            file << "#include \"../../../include/core/Scoordinator.hpp\"\n";
            file << "#include \"../../../include/systems/" << className << ".hpp\"\n\n";
            file << className << "::" << className << "()\n{\n\n";
            file << "}\n\n";
            file << className << "::~" << className << "()\n{\n\n";
            file << "}\n\n";
            file << "void " << className << "::update(float deltaTime) {\n";
            file << "    std::shared_ptr<Coordinator> coordinator = getCoordinator();\n\n";
            file << "    for (auto const &entity : _entitiesThisFrame) {\n";
            file << "        // Your update logic here\n";
            file << "    }\n";
            file << "}\n\n";
            file << "void " << className << "::addedEntity(Entity entity)\n{\n";
            file << "    // Your added entity logic here\n";
            file << "}\n\n";
            file << "void " << className << "::removedEntity(Entity entity)\n{\n";
            file << "    // Your removed entity logic here\n";
            file << "}\n";
            std::cout << "Fichier créé avec succès : " << truePath << std::endl;
            file.close();

            std::string headerTruePath = "../include/systems/" + lowerName + ".hpp";
            std::ofstream headerFile(headerTruePath);
            headerFile << "/*-----------------------------------*\n";
            headerFile << " * CardsOnline by Benjamin Bourge *\n";
            headerFile << " *-----------------------------------*/\n";
            headerFile << "\n";
            headerFile << "#pragma once\n";
            headerFile << "#include <vector>\n";
            headerFile << "#include <memory>\n";
            headerFile << "#include <queue>\n";
            headerFile << "#include <map>\n";
            headerFile << "#include \"../core/system.hpp\"\n\n";
            headerFile << "class " << className << " : public System {\n";
            headerFile << "public:\n";
            headerFile << "    " << className << "();\n";
            headerFile << "    ~" << className << "();\n";
            headerFile << "\n";
            headerFile << "    void update(float deltaTime) override;\n";
            headerFile << "    void addedEntity(Entity entity) override;\n";
            headerFile << "    void removedEntity(Entity entity) override;\n";
            headerFile << "};\n";

            std::cout << "Fichier créé avec succès : " << headerTruePath << std::endl;
            headerFile.close();

            std::ifstream inFile("../src/core/systemManager.cpp");

            if (!inFile.is_open()) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier SystemManager." << std::endl;
                return 1;
            }

            std::string line;
            std::string output;
            bool initFound = false;

            while (std::getline(inFile, line)) {
                if (line.find("void SystemManager::init()") != std::string::npos) {
                    initFound = true;
                } else if (initFound && line.find("}") != std::string::npos) {
                    std::string newLine = "    registerSystem<" + className + ">();\n";
                    output += newLine;
                    initFound = false; // Resetting the flag
                }
                output += line + "\n";
            }

            inFile.close();

            std::ofstream outFile("../src/core/systemManager.cpp");
            if (!outFile.is_open()) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier pour l'écriture." << std::endl;
                return 1;
            }

            outFile << output;
            outFile.close();

            std::cout << "Ligne insérée avec succès dans le fichier : " << className << std::endl;

            std::ofstream outFileH("../include/core/mySystems.hpp", std::ios_base::app);

            if (!outFileH.is_open()) {
                std::cerr << "Erreur : Impossible d'ouvrir le fichier pour l'écriture." << std::endl;
                return 1;
            }

            std::string newHeader = "#include \"../systems/" + lowerName + ".hpp\"\n";
            outFileH << newHeader << std::endl;
            outFileH.close();

            std::cout << "Ligne insérée avec succès à la fin des headers : " << className << std::endl;
        } else {
            std::cout << "Invalid type. try 'Component' or 'System'" << std::endl;
            continue;
        }
    }
    return 0;
}