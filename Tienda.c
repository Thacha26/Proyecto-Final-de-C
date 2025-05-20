#include <stdio.h>
#include <stdlib.h> // Para la memeria deinámica (malloc)
#include <string.h>

// Estructura para los producost
typedef struct Producto {
    char nombre[100];
    float precio;
    int cantidad;
    struct Producto* siguiente; // Apuntador al siguiente producto (nodo)
    struct Producto* anterior; // Apuntador al producto anterior (nodo)
} Producto;

// Estructura para el usuario
typedef struct Usuario {
    char nombre[100];
    char celular[10];
    float totalPagar;
    Producto* carrito; // Un apuntador que apunte al inicio de la lista del carrito de compras (nodo)
} Usuario;

Producto* crearProducto(char* nombre, float precio, int cantidad) {
    Producto* nuevo = (Producto*)malloc(sizeof(Producto));
    strcpy(nuevo->nombre, nombre);
    nuevo->precio = precio;
    nuevo->cantidad = cantidad;
    nuevo->siguiente = NULL;
    nuevo->anterior = NULL; 
    return nuevo;
}

Producto* cargarProductos(const char* archivo) { // Crea una a enlazada de Producto
    FILE* f = fopen(archivo, "r"); // Abrimos el archivo con permisos de lectura (Read)
    if (f == NULL) { //si el archivo no existe imprime un mensaje de error
        printf("No se pudo abrir el archivo: %s\n", archivo);
        return NULL; // retorna Null si el archivo no existe
    }

    Producto *inicio = NULL, *ultimo = NULL; //Se apunta al primer y último nodo de la a, se inicializan en null
    char nombre[100];
    float precio;
    int cantidad;

    // Se generan nodos individuales de la a
    while (fscanf(f, "%s %f %d", nombre, &precio, &cantidad) == 3) {
        // Se asigna memoria para un nuevo producto
        Producto *nuevo = crearProducto(nombre, precio, cantidad); // mientras el producto tenga tres atributos se creará uno nuevo

        if (!inicio) {
            inicio = nuevo; // Se crea el primer nodo de la a
        } else {
            ultimo->siguiente = nuevo; // Enlaza al final
            nuevo->anterior = ultimo; // e enlaza hacia atrás
        }
        ultimo = nuevo; // aiza el último nodo
    }

    fclose(f);
    return inicio;
  
} // Se cierra la función que lee el archivo   

// Función para agregar un producto al carrito
void agregarAlCarrito(Usuario *usuario, Producto *producto) {
    Producto *nuevo = (Producto*)malloc(sizeof(Producto));
    strcpy(nuevo->nombre, producto->nombre);
    nuevo->precio = producto->precio;
    nuevo->siguiente = usuario->carrito; 
    usuario->carrito = nuevo; // Actualiza el carrito
    nuevo->cantidad = producto->cantidad;
    usuario->totalPagar += producto->precio;
    printf("Se ha agregado el nuevo producto al carrito: %s", producto->nombre);
}

// Muestra un producto a la vez y permite navegar
void mostrarProductos(Producto* inicio, Usuario* usuario) {
    if (!inicio) {
        printf("No hay productos disponibles.\n");
        return;
    }

    Producto* actual = inicio;
    char opcion;

    do {
        printf("\n ------ PRODUCTO ACTUAL ------\n");
        printf("Nombre: %s\n", actual->nombre);
        printf("Precio: %.2f\n", actual->precio);
        printf("Cantidad: %d\n", actual->cantidad);

        printf("\nOpciones:\n");
        printf("[A] Anterior\n[S] Siguiente\n[C] Comprar\n[M] Menú principal\n");
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'S':
            case 's':
                if (actual->siguiente != NULL)
                    actual = actual->siguiente;
                else
                    printf("Este es el último producto.\n");
                break;
            case 'A':
            case 'a':
                if (actual->anterior != NULL)
                    actual = actual->anterior;
                else
                    printf("Este es el primer producto.\n");
                break;
            case 'C':
            case 'c':
            if (actual->cantidad > 0) {
                agregarAlCarrito(usuario, actual);
                actual->cantidad--; // se reduce la cantidad de productos
            } else {
                printf("Producto agotado, vuelve después\n");
            }
            break;
            case 'M':
            case 'm':
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 'M' && opcion != 'm');
}


Producto* buscarProducto(Producto* a, char* nombre) {
    while (a != NULL) {
        if (strcmp(a->nombre, nombre) == 0) {
            return a;
        }
        a = a->siguiente; // Se avanza al siguiente elemento
    }
}    

void mostrarCarrito(Usuario* usuario) { // Se apunta al usuario
    if (!usuario->carrito) { // Si el carrito está vacío (NULL)
        printf("El carrito está vacío. Regresa al menú principal.\n"); // Devuelve un mensaje de error
        return;
    }

    Producto* a = usuario->carrito;
    while (a) {

        printf("Nombre: %s\n", a->nombre);
        printf("Precio: %.2f\n\n", a->precio ); 
        printf("Cantidad: %d\n", a->cantidad);
        a = a->siguiente; // Se avanza al siguiente elemento
    }

    printf("Total a pagar: %.2f\n", usuario->totalPagar);
}


// Verifica que el número de usuario sea válido
int UsuarioCelular_valido(const char* celular) {
    if (strlen(celular) != 10) return 0;
    for (int i = 0; celular[i] != '\0'; ++i) {
        if (celular[i] < '0' || celular[i] > '9') return 0;
    }
    return 1;
}

// Muestra la información del usuario
void mostrarUsuario(Usuario* usuario) {
    printf("\n=== INFORMACIÓN DEL USUARIO ===\n");
    printf("Nombre: %s\n", usuario->nombre);
    printf("Celular: %s\n", usuario->celular);
    printf("Total acumulado: %.2f\n", usuario->totalPagar);
}

// Función principal
int main() {
    Usuario usuario;
    printf("Bienvenido a la tiendita virtual\n");
    printf("Por favor, ingresa tu nombre: ");
    scanf("%s", usuario.nombre);
    do {
        printf("Ahora, tu número de celular (10 dígitos): ");
        scanf("%s", usuario.celular);
        if (!UsuarioCelular_valido(usuario.celular)) {
            printf("El número de celular no es válido, por favor intenta de nuevoo\n");
        }
    } while (!UsuarioCelular_valido(usuario.celular));

    usuario.totalPagar = 0;
    usuario.carrito = NULL;
    char nombreArchivo[100];
    Producto* productos = NULL;
    do {
        printf("Por favor, ingrese el nombre del archivo .txt\n");
        scanf("%s", nombreArchivo);
        productos = cargarProductos(nombreArchivo);

        if(productos == NULL) { // Muestra un mensaje de error cuando el archivo no existe 
            printf("\nEl archivo '%s' no existe o está vacío.\n", nombreArchivo);
            printf("Vuelve a intentar\n");
            printf("Por favor, ingrese el nombre del archivo .txt\n");
            scanf("%s", nombreArchivo);
        }
    } while(productos == NULL);
       
    int opcion;
    do {
        printf("\n==== MENÚ PRINCIPAL ====\n");
        printf("1. Ver mi carrito de compras\n");
        printf("2. Ver mi información de usuario\n");
        printf("3. Ver la lista de productos\n");
        printf("4. Salir\n");
        char entrada[10];
        printf("Selecciona una opción: ");
        scanf("%s", entrada);

        if (sscanf(entrada, "%d", &opcion) != 1 || opcion < 1) {
            printf("Opción inválida. Intenta de nuevo.\n");
            continue; // <- Esto evita que se ejecute el switch si la entrada no es válida
        }
        switch (opcion) {
            case 1:
                mostrarCarrito(&usuario);
                break;
            case 2:
                mostrarUsuario(&usuario);
                break;
            case 3:
                mostrarProductos(productos, &usuario);
                break;
            case 4:
                printf("See you later....\n");
                printf("Usted ha decidido dalir de esta maravillosa tiendita");
                break;
            default:
                printf("Opción inválida. Intenta de nuevo.\n");
                break;
               
        }    
    } while (opcion != 4);

    return 0;
}
