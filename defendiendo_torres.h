#ifndef __DEFENDIENDO_TORRES_H__
#define __DEFENDIENDO_TORRES_H__

#include <stdlib.h>
#include <stdio.h>
#define MAX_FILAS 30
#define MAX_COLUMNAS 30
#define MAX_LONGITUD_CAMINO 200
#define MAX_ENEMIGOS 500
#define MAX_DEFENSORES 50
#define MAX_RUTA 80
#define MAX_CAMINOS 6


#define FIN "\033[0m"
#define ROJO "\033[0m\033[31m"
#define ROJO_CLARO "\033[1m\033[31m"
#define VERDE_OSCURO "\033[0m\033[32m"
#define VERDE_CLARO "\033[1m\033[32m"
#define AMARILLO  "\033[1m\033[33m"
#define PLATEADO "\033[0m\033[37m"
#define AZUL "\033[1m\033[34m"
#define CYAN "\033[1m\033[36m"
#define MARRON "\033[0m\033[33m"
#define MARRON_OSCURO "\033[1m\033[33m"
#define MAGENTA "\033[1m\033[35m"
#define GRIS "\033[1m\033[30m"

#define VACIO ' '
#define PRINT_ORCO "¤"
#define PRINT_ENTRADA "█"
#define PRINT_TORRE_1 "|1|"
#define PRINT_TORRE_2 "|2|"

#define ENTRADA 'E'
#define TORRE_1 '1'
#define TORRE_2 '2'
#define CAMINO 'C'

#define NUM_CAMINO_1 1
#define NUM_CAMINO_2 2

// Niveles
#define ESTE 1
#define OESTE 2
#define NORTE 3
#define SUR 4
#define MAX_NIVELES 4

#define PRIMERA_FIL_TERRENO 0
#define PRIMERA_COL_TERRENO 0
#define TOPE_TERRENO_CHICO 15
#define TOPE_TERRENO_GRANDE 20
#define MAX_TERRENO 20

#define RUTA_POR_DEFECTO "-1"
#define VALOR_POR_DEFECTO -1


typedef struct coordenada {
	int fil;
	int col;
} coordenada_t;

typedef struct defensor {
	char tipo;
	int fuerza_ataque;
	coordenada_t posicion;
} defensor_t;

typedef struct torres {
	int resistencia_torre_1;
	int resistencia_torre_2;
	int enanos_extra;
	int costo_enanos_1;
	int costo_enanos_2;
	int elfos_extra;
	int costo_elfos_1;
	int costo_elfos_2;
} torres_t;

typedef struct enemigo {
	int camino;
	int pos_en_camino;
	int vida;
} enemigo_t;

typedef struct nivel {
	coordenada_t camino_1[MAX_LONGITUD_CAMINO];
	int tope_camino_1;
	
	coordenada_t camino_2[MAX_LONGITUD_CAMINO];
	int tope_camino_2;

	defensor_t defensores[MAX_DEFENSORES];
	int tope_defensores;

	enemigo_t enemigos[MAX_ENEMIGOS];
	int tope_enemigos;

	int max_enemigos_nivel;
} nivel_t;

typedef struct juego {
	nivel_t nivel;
	torres_t torres;
	int nivel_actual;
	int critico_legolas;
	int critico_gimli;
	int fallo_legolas;
	int fallo_gimli;
} juego_t;

typedef struct defensores {

	int cant_inicial[MAX_NIVELES];
	int cant_extra;
	int costo_torre_1;
	int costo_torre_2;
	int critico;
	int fallo;

} defensores_t;

typedef struct camino {
	coordenada_t camino[MAX_LONGITUD_CAMINO];
	int tope_camino;
	int num_camino;
} camino_t;

typedef struct configuracion {

	int resistencia_torre_1;
	int resistencia_torre_2;

	defensores_t enanos;
	defensores_t elfos;

	float velocidad;

	char ruta_caminos[MAX_RUTA];
	char ruta_config[MAX_RUTA];
	char ruta_ranking[MAX_RUTA];
	char ruta_grabacion[MAX_RUTA];

	char pos_actual_camino;

	camino_t caminos[MAX_CAMINOS];

} configuracion_t;


/*
* Pre: -
* Post: Devuelve el tope del terreno dependiendo del nivel actual. Si el nivel
	es el ESTE u OESTE, devuelve el tope chico, sino, el grande.
*/
int tope_terreno_segun_nivel(int nivel);

/*
	Función del TP2
* Pre: nivel debe ser válido.
* Post: Asigna las coordenadas de la(s) entrada(s) según el nivel.
*/
void obtener_coord_entrada(int nivel, coordenada_t* entrada_1, coordenada_t* entrada_2, int tope);

/* 			
	Función del TP2
* Pre: nivel debe ser válido.
* Post: Asigna las coordenadas de la(s) torre(s) según el nivel.
*/
void obtener_coord_torre(int nivel, coordenada_t* torre_1, coordenada_t* torre_2, int tope);


/*
* Pre: tope_terreno <= MAX_TERRENO. El terreno debe estar inicializado.
* Post: Imprime el terreno por pantalla, con sus respectivos componentes
	(camino, enemigos, defensores, etc.).
*/
void mostrar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int tope_terreno);

/*
 * Inicializará el juego, cargando la informacion de las torres y
 * los ataques críticos y fallo de Legolas y Gimli.
 * NO inicializará el primer nivel.
 */
void inicializar_juego(juego_t* juego, int viento, int humedad, char animo_legolas, char animo_gimli, configuracion_t configuracion);

/*
 * Recibe un juego con todas sus estructuras válidas. 
 * El juego se dará por ganado si el juego está en el ultimo nivel y éste ha sido terminado.
 * El juego se dará por perdido, si alguna de las torres llega a 0 en su resistencia.
 * Devolverá:
 * >  0 si el estado es jugando.
 * > -1 si el estado es perdido.
 * >  1 si el estado es ganado.
 */
int estado_juego(juego_t juego);

/*
 * Recibe un nivel con todas sus estructuras válidas. 
 * El nivel se dará por ganado cuando estén TODOS los orcos de ese 
 * nivel muertos (esto es, con vida menor o igual a 0).
 * Devolverá:
 * >  0 si el estado es jugando.
 * >  1 si el estado es ganado.
 */
int estado_nivel(nivel_t nivel);

/* 
 * Agregará un defensor en el nivel recibido como parametro.
 * Devolverá:
 * >  0 si pudo agregar el defensor correctamente.
 * > -1 si no pudo (la coordenada es parte del camino de ese nivel, 
 * existe otro defensor, etc.)
 */
int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo);

/*
 * Jugará un turno y dejará el juego en el estado correspondiente.
 * Harán su jugada enanos, elfos y orcos en ese orden.
 */
void jugar_turno(juego_t* juego);

/* 
 * Mostrará el mapa dependiendo del nivel en que se encuentra el jugador.
 */
void mostrar_juego(juego_t juego);

#endif /* __DEFENDIENDO_TORRES_H__ */
