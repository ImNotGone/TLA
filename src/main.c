#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "backend/semantic-analysis/tree-utils.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include "backend/semantic-analysis/symbol-table.h"
#include <stdio.h>

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.result = 0;
	state.succeed = false;

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

    bool testMode = false;
    if (argumentCount == 2) {
        char *test = "--test";
        if (strcmp(arguments[1], test) == 0) {
            testMode = true;
        }
    }

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
    symbolTableInit();
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilacion fue exitosa.");

                
                if (testMode) {
                    symbolTableDestroy();
                    freeProgram(state.program);
                    LogInfo("Fin.");
                    return 0;
                }
                
                FILE * file = fopen("./src/backend/domain-specific/src/main/java/Main.java", "w");

                SetOutputFile(file);

				GeneratorProgram(state.program);

                fclose(file);

                LogInfo("El archivo Main.java fue generado exitosamente.");

                // Compile the generated file
                system("mvn -f ./src/backend/domain-specific/pom.xml clean compile");
                
                // Run the generated file
                system("mvn -f ./src/backend/domain-specific/pom.xml exec:java -Dexec.mainClass=\"Main\"");
			}
			else {
				LogError("Se produjo un error en la aplicacion.");
                symbolTableDestroy();
				return -1;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}
    symbolTableDestroy();
    freeProgram(state.program);
	LogInfo("Fin.");
	return result;
}
