# Tarea_4_estructura_de_datos

Universidad de Magallanes  
Facultad de Ingeniería - Estructuras de Datos 2025  
**Profesor**: Christian Vásquez Rebolledo  

## Problema del Vendedor Viajante (TSP)
---
### Descripción
Implementación en C del Problema del Vendedor Viajante (Travelling Salesman Problem) usando grafos y backtracking. El programa encuentra la ruta más corta que visita todas las ciudades exactamente una vez y regresa al origen.

### Integrantes del Grupo
- Dario Duarte
- Alexander Lucero
- Diego Oyarzo
- Alan Sanchez

### Estructura del Proyecto
```
.
├── pvv.c           # Programa principal y CLI
├── functions.c     # Implementación de funciones
├── functions.h     # Declaraciones de funciones
├── structures.h    # Definición de estructuras de datos
├── commands.h      # Comandos auxiliares
├── Makefile        # Compilación automática
├── map.txt         # Archivo de ejemplo con conexiones
└── README.md       # Este archivo
```

### Compilación

#### 
```bash
make
```

#### Limpiar archivos compilados:
```bash
make clean
```

#### Recompilar desde cero:
```bash
make rebuild
```

### Uso

#### Iniciar el programa:
```bash
./pvv
```

#### Comandos disponibles:

| Comando          | Descripción                    |  Ejemplo       |
|------------------|--------------------------------|----------------|
| `start [n]`      | Crea un grafo con n ciudades   | `start 5`      |
| `read <archivo>` | Carga conexiones desde archivo | `read map.txt` |
| `all`            | Muestra el grafo actual        | `all`          |
| `hamil`          | Busca ciclo hamiltoniano       | `hamil`        |
| `exit`           | Libera memoria y sale          | `exit`         |
| `help`           | Muestra ayuda                  | `help`         |

### Formato del archivo de entrada

El archivo debe contener una conexión por línea con el formato:
```
ORIGEN DESTINO COSTO
```

**Ejemplo (map.txt):**
```
A B 15
A D 10
B C 25
B E 15
C D 20
C E 16
D E 30
```

**Notas:**
- Los nodos se identifican con letras mayúsculas
- El costo debe ser un número entero positivo
- Las conexiones son bidireccionales (si existe A→B, también existe B→A con el mismo costo)

### Ejemplo de ejecución

```
=== Bienvenido al Problema del Viajante (PVV) ===
Escribe 'help' para ver los comandos disponibles.

> start 5
Grafo preparado para 5 ciudades.
Usa 'read <archivo>' para cargar las conexiones.

> read map.txt
Grafo leido desde 'map.txt' con exito.
Conexiones vertice [A]: D(10) B(15)
Conexiones vertice [B]: E(15) C(25) A(15)
Conexiones vertice [D]: E(30) C(20) A(10)
Conexiones vertice [C]: E(16) D(20) B(25)
Conexiones vertice [E]: D(30) C(16) B(15)

Buscando ciclo hamiltoniano...

=== Verificando la existencia de una ruta viable ===
Existe un camino viable que recorre todas las ciudades y vuelve al origen.
Ruta a seguir: A B E C D A
Costo total del viaje: 76

> exit
Memoria liberada completamente. ¡Adios!
```

### Pruebas con Valgrind

Para verificar que no hay fugas de memoria:

```bash
valgrind --leak-check=full ./pvv
```

### Estructuras de Datos

- **Grafo**: Representado con lista de adyacencia
- **Vértices**: Array de estructuras que contienen id, letra y lista de aristas
- **Aristas**: Lista enlazada con destino, costo y siguiente arista


### Debugging

Si encuentras problemas:

1. Verifica que el archivo de entrada existe y tiene el formato correcto
2. Asegúrate de hacer `start` antes de `read`
3. Usa `all` para visualizar el grafo cargado
4. Revisa que hay suficientes conexiones para formar un ciclo

---

**Fecha de entrega**: 15 de diciembre de 2025, 23:59  