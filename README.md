[![✗](https://img.shields.io/badge/Release-v0.2.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# Compilador Flex/Bison

Un compilador vacío de ejemplo construido con Flex y Bison.

## Requerimientos

Para construir el compilador, se requieren las siguientes dependencias:

* [Bison v3.8.2](https://www.gnu.org/software/bison/)
* [CMake v3.24.1](https://cmake.org/)
* [Flex v2.6.4](https://github.com/westes/flex)
* [GCC v11.1.0](https://gcc.gnu.org/)
* [Make v4.3](https://www.gnu.org/software/make/)

Si en lugar de trabajar con un entorno _Linux_, se está construyendo el proyecto sobre un entorno _Microsoft Windows_, se debe instalar _Microsoft Visual Studio 2022_ con las extensiones para desarrollar aplicaciones en _C/C++_, así como también las herramientas requeridas, con excepción del compilador _GCC_ y la herramienta _Make_.

## Construcción

Para construir el proyecto por completo, ejecute en la raíz del repositorio los siguientes comandos (en _Linux_):

```bash
user@machine:path/ $ chmod u+x --recursive script
```

luego ejecutar alguno de los siguientes 2 comandos
```bash
user@machine:path/ $ script/build.sh
user@machine:path/ $ make all
```

En un entorno _Microsoft Windows_, en cambio, se debe ejecutar:

```bash
user@machine:path/ $ script\build.bat
```

Luego se deberá abrir la solución generada `bin\Compiler.sln` con el IDE _Microsoft Visual Studio 2022_. Los ejecutables que este sistema construye se depositan dentro del directorio `bin\Debug` y `bin\Release`, según corresponda.

## Ejecución

Para compilar un programa, primero cree un archivo vacío denominado `program` (o el nombre que desee), con el siguiente contenido:

```
// Crear un nuevo arbol RBT
new RBT t1;

// Luego de cada modificacion, el arbol se balancea internamente
insert t1 1;
insert t1 2;
insert t1 3;

// Imprime el arbol balanceado como Red-Black-Tree
print t1;

// Elimina el nodo que contiene el 1
remove t1 1;

// Copia t1 (RBT) en t2 (AVL)
new AVL t2 <- t1;

// Imprime el arbol balanceado como AVL TREE
print t2;
```

Luego, ejecute el compilador desde el directorio raíz del proyecto, o desde cualquier otro lugar indicando el path hacia el script `start.sh` y pasando por parámetro el path hacia el programa a compilar:

```bash
user@machine:path/ $ script/start.sh program
```
o asi:
```bash
user@machine:path/ $ make run FILE=./program
```

En Windows:

```bash
user@machine:path/ $ script\start.bat program
```

Deberia generar 2 imagenes, una con el arbol en RBT otra con el arbol en AVL

## Testing

En Linux:

```bash
user@machine:path/ $ script/test.sh
```
o asi:
```bash
user@machine:path/ $ make test
```

En Windows:

```bash
user@machine:path/ $ script\test.bat
```

Si desea agregar nuevos casos de uso, deberá crear un archivo por cada uno, que contenga el programa a testear dentro de las carpetas `test/accept` o `test/reject` según corresponda (es decir, si el mismo debe ser aceptado o rechazado por el compilador).
