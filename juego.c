#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "animos.h"
#include "defendiendo_torres.h"
#include "utiles.h"
#include "config.h"

// Elfos : L
// Enanos: G
// Orcos: O
// Torre: T
// Entrada: E

#define MAX_LECTURA 50

#define JUGANDO 0
#define PERDIDO -1
#define GANADO 1

#define STRING_VACIO ""

const int MULTIPLICADOR_PUNTAJE = 1000;

static const char ARRIBA = 'W';
static const char ABAJO = 'S';
const char ABAJO_MINUS = 's';
const char ARRIBA_MINUS = 'w';

const char ENANO = 'G';
const char ELFO = 'L';

static const int NO_AGREGADO = -1;

static const int VIDA_INICIAL_TORRE = 600;
static const int DEFENSORES_EXTRA = 10;

const int DEFENSORES_ESTE = 5;
const int DEFENSORES_OESTE = 5;
const int DEFENSORES_NORTE = 3;
const int DEFENSORES_SUR = 4;

const int ENEMIGOS_ESTE = 100;
const int ENEMIGOS_OESTE = 200;
const int ENEMIGOS_NORTE = 300;
const int ENEMIGOS_SUR = 500;

const int SIN_TROPAS = 0;
static const int VIDA_NULA = 0;


const int PREGUNTAR_EXTRA_RAPIDO = 25;
const int PREGUNTAR_EXTRA_LENTO = 50;
const int PUEDE_AGREGAR_EXTRA = 0;
const int NUM_DEFENSOR_EXTRA = -1;
const int COSTE_DEFENSOR_EXTRA = 50;
const char NO_HAY_DEFENSORES = 'X';

const char NO_QUIERE = 'N';
const char NO_QUIERE_MINUS = 'n';
const char QUIERE_MAYUS = 'S';
const char QUIERE_MINUS = 's';

const float TIEMPO_TURNO = 0.25;
const float TIEMPO_INICIAL = 1.5;

const int MIN_ARGUMENTOS = 2;
const int POS_PRIMER_ARGUMENTO = 1;
const int POS_SEGUNDO_ARGUMENTO = 2;
const int POS_TERCER_ARGUMENTO = 3;
const int MAX_SUBARGUMENTOS = 2;
const int CANT_ARGUMENTOS_CONFIG = 3;
const int CANT_ARGUMENTOS_CAMINOS = 3;
const int CANT_ARGUMENTOS_REPE = 3;

const char SEPARADOR_IGUAL = '=';

const float VELOCIDAD_REPE_PREDETERMINADA = 1;

const int LEIDOS_CAMINO = 2;

// Números de los caminos personalizados
const int CAMINO_ESTE = 0;
const int CAMINO_OESTE = 1;
const int CAMINO_1_NORTE = 2;
const int CAMINO_2_NORTE = 3;
const int CAMINO_1_SUR = 4;
const int CAMINO_2_SUR = 5;

// Comandos
#define COMANDO_AYUDA "ayuda"
#define COMANDO_CREAR_CONFIG "crear_configuracion"
#define COMANDO_RANKING "ranking"
#define COMANDO_CREAR_CAMINO "crear_camino"
#define COMANDO_REPE "poneme_la_repe"
#define COMANDO_JUGAR "jugar"

	
/*
* Pre: tipo debe ser ENANO o ELFO. num debe ser el número del defensor a agregar.
* Post: Pregunta al usuario la coordenada del defensor que quiere agregar, según
	el tipo, y la guarda en la posicion recibida.
*/
void preguntar_coord_defensor(coordenada_t* posicion, char tipo, int num) {
	
	if (num == NUM_DEFENSOR_EXTRA)
		printf("Ingrese las coordenadas para el defensor extra tipo ");
	else
		printf("Ingrese las coordenadas para el defensor N° %i de tipo ", num);
	if (tipo == ENANO) printf("Enano:\n");
	else printf("Elfo:\n");

	printf("Fila: ");
	scanf(" %i", &((*posicion).fil));
	printf("Columna: ");
	scanf(" %i", &((*posicion).col));
}




/*
* Pre: nivel_actual debe ser válido.
* Post: Devuelve true si se puede agregar un defensor extra, esto es, cada cierta cantidad
	de enemigos por nivel (PREGUNTAR_EXTRA_RAPIDO o PREGUNTAR_EXTRA_LENTO). Sino, devuelve false.
*/
bool puede_agregar_extra(int tope_enemigos, int nivel_actual, int max_enemigos_nivel) {
	if (nivel_actual == ESTE) 
		return (tope_enemigos % PREGUNTAR_EXTRA_RAPIDO == PUEDE_AGREGAR_EXTRA &&
				tope_enemigos < max_enemigos_nivel);

	else
		return (tope_enemigos % PREGUNTAR_EXTRA_LENTO == PUEDE_AGREGAR_EXTRA &&
				tope_enemigos < max_enemigos_nivel);
}

/*
* Pre: torres debe ser válido, con sus estructuras válidas.
	tipo debe ser ENANO o ELFO.
* Post: Devuelve true si se puede agregar un defensor extra del
	tipo recibido, teniendo en cuenta que la torre tenga la vida
	suficiente, y que haya tropas extras disponibles. Sino,
	devuelve false.
*/
bool puede_agregar_ese_tipo(torres_t torres, char tipo) {

	if (tipo == ENANO)
		return ( (torres.enanos_extra > SIN_TROPAS) && 
			(torres.resistencia_torre_1 > COSTE_DEFENSOR_EXTRA) );
	else
		return ( (torres.elfos_extra > SIN_TROPAS) && 
			(torres.resistencia_torre_2 > COSTE_DEFENSOR_EXTRA) );
}

/*
* Pre: torres deben ser válidas, nivel_actual debe ser válido.
* Post: Determina el tipo de defensor extra a agregar según el nivel_actual.
	Si el nivel es mayor a OESTE, se pregunta el tipo al usuario, y se determina
	si se puede agregar o no. Si no se puede, se devuelve NO_HAY_DEFENSORES. Si se
	puede, devuelve el tipo. 
*/
char tipo_def_extra_segun_nivel(int nivel_actual, torres_t torres) {
	
	char tipo;
	if (nivel_actual == ESTE)
			tipo = ENANO;
	else if (nivel_actual == OESTE)
		tipo = ELFO;
	else {
		do {
			printf("De qué tipo? (G: Enano | L: Elfo): ");
			scanf(" %c", &tipo);
		} while (tipo != ENANO && tipo != ELFO);
	}

	if ( !puede_agregar_ese_tipo(torres, tipo) ) 
		return NO_HAY_DEFENSORES;

	return tipo;
}

/*
* Pre: nivel_actual debe ser válido.
* Post: Devuelve true si la coordenada es válida para el terreno (depende
	del nivel), o false, si no lo es.
*/
bool es_coordenada_valida(coordenada_t coordenada, int nivel_actual) {
	
	int max_coordenada;
	if (nivel_actual >= NORTE) max_coordenada = TOPE_TERRENO_GRANDE;
	else max_coordenada = TOPE_TERRENO_CHICO;

	if (coordenada.fil >= PRIMERA_FIL_TERRENO && coordenada.fil < max_coordenada && 
			coordenada.col >= PRIMERA_COL_TERRENO && coordenada.col < max_coordenada)
		return true;

	return false;
}


/*
* Pre: nivel y torres debe ser válidos, con todas sus estructuras válidas. El
	nivel actual debe ser >= ESTE y <= SUR.
* Post: Pregunta al usuario si quiere agregar un defensor extra. Si la respuesta es
	si, lo agrega y le resta la vida correspondiente a la torre correspondiente, y disminuye
	el contador de defensores extra de ese tipo en 1. Sino, imprime por pantalla el mensaje
	de error.
*/
void preguntar_defensor_extra(nivel_t* nivel, torres_t* torres, int nivel_actual) {
	// 
	char quiere_defensor = NO_QUIERE;
	char tipo;
	coordenada_t posicion;

	printf("Quiere agregar un defensor extra? (COSTE: 50 PUNTOS DE VIDA)\n [S/N]: ");
	scanf(" %c", &quiere_defensor);

	if (quiere_defensor == QUIERE_MAYUS || quiere_defensor == QUIERE_MINUS) {

		tipo = tipo_def_extra_segun_nivel(nivel_actual, (*torres));

		if (tipo != NO_HAY_DEFENSORES) {

			preguntar_coord_defensor(&posicion, tipo, NUM_DEFENSOR_EXTRA);

			while (agregar_defensor(nivel, posicion, tipo) == NO_AGREGADO || !es_coordenada_valida(posicion, nivel_actual)) {
						printf("\n Esa posición es inválida. No se pueden agregar defensores dentro del camino, ni sobre otros defensores. Intente nuevamente:\n");
						preguntar_coord_defensor(&posicion, tipo, NUM_DEFENSOR_EXTRA);
			}

			if (tipo == ENANO) { 
				((*torres).resistencia_torre_1) -= COSTE_DEFENSOR_EXTRA;
				((*torres).enanos_extra)--;
			}
			else  {
				((*torres).resistencia_torre_2) -= COSTE_DEFENSOR_EXTRA;
				((*torres).elfos_extra)--;
			}
		} else {
			printf("¡No fue posible agregar un defensor! Quizás la vida de la torre sea muy baja, o te hayas quedado sin defensores de ese tipo.\n");
		}
	}
}

/*
* Pre: -
* Post: Asigna el camino personalizado (fil y col) al camino_juego recibido.
*/
void asignar_camino_personalizado(camino_t* camino, coordenada_t camino_juego[MAX_LONGITUD_CAMINO]) {

	for (int i = 0; i < (*camino).tope_camino; i++) {
		camino_juego[i].fil = (*camino).camino[i].fil;
		camino_juego[i].col = (*camino).camino[i].col;
	}
}

/*
* Pre: El nivel debe estar inicializado y ser válido (ESTE, OESTE, NORTE, SUR). 
	El tipo debe ser ENANO o ELFO, y defensores_inicio representará la cantidad 
	de defensores iniciales del tipo recibido.
* Post: Asgina los defensores iniciales (defensores_inicio) según el nivel recibido y
	el tipo.
*/
void asignar_defensores_por_nivel(int nivel, int* defensores_inicio, char tipo) {
	if(nivel == ESTE) {
		if(tipo == ELFO) {
			(*defensores_inicio) = SIN_TROPAS;
		} else {
			(*defensores_inicio) = DEFENSORES_ESTE;
		}
	} else if(nivel == OESTE) {
		if(tipo == ENANO) {
			(*defensores_inicio) = SIN_TROPAS;
		} else {
			(*defensores_inicio) = DEFENSORES_OESTE;
		}
	} else if(nivel == NORTE) {
		(*defensores_inicio) = DEFENSORES_NORTE;
	} else if(nivel == SUR) {
		(*defensores_inicio) = DEFENSORES_SUR;
	}
}

/*
* Pre: El juego debe ser válido con todas sus estructuras válidas. La cantidad de defensores
	debe ser > 0, y el tipo debe ser ENANO o ELFO.
* Post: Pregunta al usuario las coordenadas para asignar a los defensores del tipo recibido, 
	la cantidad de veces recibida (cant_defensores), y los agrega al juego.
*/
void preguntar_defensores(juego_t* juego, int cant_defensores, char tipo) {
	for (int i = 0; i < cant_defensores; i++) {
		coordenada_t posicion;
		preguntar_coord_defensor(&posicion, tipo, i+1);
		
		while (agregar_defensor(&(*juego).nivel, posicion, tipo) == NO_AGREGADO|| !es_coordenada_valida(posicion, (*juego).nivel_actual)) {
			printf("\nEsa posición es inválida. No se pueden agregar defensores dentro del camino, ni sobre otros defensores. Intente nuevamente:\n");
			preguntar_coord_defensor(&posicion, tipo, i+1);
		}
		mostrar_juego(*juego);
	}
}

/*
* Pre:  nivel_actual debe ser válido (ESTE, OESTE, NORTE o SUR). El juego recibido debe
	ser válido, con todas sus estructuras válidas.
* Post: Asgina los caminos según el nivel_actual al juego recibido. Los obtiene del vector de caminos
	si la ruta es distinta a RUTA_POR_DEFECTO. Sino, usa caminos aleatorios.
*/
void asignar_caminos_por_nivel(int nivel_actual, juego_t* juego, camino_t caminos[MAX_CAMINOS], char ruta_caminos[MAX_RUTA], 
	coordenada_t* entrada_1, coordenada_t* torre_1, coordenada_t* entrada_2, coordenada_t* torre_2) {

	int camino_actual;
	if (nivel_actual <= OESTE) {
		if (nivel_actual == ESTE) {
			camino_actual = CAMINO_ESTE;
			(*juego).nivel.max_enemigos_nivel = ENEMIGOS_ESTE;
		} else {
			camino_actual = CAMINO_OESTE;
			(*juego).nivel.max_enemigos_nivel = ENEMIGOS_OESTE;
		}

		if(strcmp(ruta_caminos, RUTA_POR_DEFECTO) != 0) {
			(*juego).nivel.tope_camino_1 = caminos[camino_actual].tope_camino;
			asignar_camino_personalizado(&(caminos[camino_actual]), (*juego).nivel.camino_1);
		} else {
			obtener_camino((*juego).nivel.camino_1,
						&((*juego).nivel.tope_camino_1), (*entrada_1), (*torre_1));
		}

	} else {
		int camino_actual_2;
		if (nivel_actual == NORTE) {
			camino_actual = CAMINO_1_NORTE;
			camino_actual_2 = CAMINO_2_NORTE;
			(*juego).nivel.max_enemigos_nivel = ENEMIGOS_NORTE;
		} else {
			camino_actual = CAMINO_1_SUR;
			camino_actual_2 = CAMINO_2_SUR;
			(*juego).nivel.max_enemigos_nivel = ENEMIGOS_SUR;
		}

		if(strcmp(ruta_caminos, RUTA_POR_DEFECTO) != 0) {
			(*juego).nivel.tope_camino_1 = caminos[camino_actual].tope_camino;
			(*juego).nivel.tope_camino_2 = caminos[camino_actual_2].tope_camino;
			asignar_camino_personalizado(&(caminos[camino_actual]), (*juego).nivel.camino_1);
			asignar_camino_personalizado(&(caminos[camino_actual_2]), (*juego).nivel.camino_2);
		} else {
			obtener_camino((*juego).nivel.camino_1,
						&((*juego).nivel.tope_camino_1), (*entrada_1), (*torre_1));
			obtener_camino((*juego).nivel.camino_2,
						&((*juego).nivel.tope_camino_2), (*entrada_2), (*torre_2));
		}
	}
}

/*
* Pre: El juego recibido debe ser válido, con todas sus estructuras válidas.
* Post: Inicializa el nivel, dependiendo del nivel_actual, con los caminos, defensores, etc.
*/
void inicializar_nivel(juego_t* juego, coordenada_t* entrada_1, coordenada_t* entrada_2,
		coordenada_t* torre_1, coordenada_t* torre_2, int* enanos_inicio, int* elfos_inicio, 
		camino_t caminos[MAX_CAMINOS], char ruta_caminos[MAX_RUTA]) {

	int nivel = (*juego).nivel_actual;

	int tope_terreno;
	if (nivel == ESTE || nivel == OESTE) tope_terreno = TOPE_TERRENO_CHICO;
	else tope_terreno = TOPE_TERRENO_GRANDE;

	// "Vacío" los vectores que contienen defensores y enemigos, reiniciando su tope.
	(*juego).nivel.tope_defensores = SIN_TROPAS;
	(*juego).nivel.tope_enemigos = SIN_TROPAS;

	obtener_coord_entrada((*juego).nivel_actual, entrada_1, entrada_2, tope_terreno);
	obtener_coord_torre((*juego).nivel_actual, torre_1, torre_2, tope_terreno);

	asignar_caminos_por_nivel(nivel, juego, caminos, ruta_caminos, 
		entrada_1, torre_1, entrada_2, torre_2);

	mostrar_juego(*juego);

	if ((*enanos_inicio) == VALOR_POR_DEFECTO)
		asignar_defensores_por_nivel(nivel, enanos_inicio, ENANO);
	if ((*elfos_inicio) == VALOR_POR_DEFECTO)
		asignar_defensores_por_nivel(nivel, elfos_inicio, ELFO);

	if (nivel != OESTE)
		preguntar_defensores(juego, (*enanos_inicio), ENANO);
	if (nivel != ESTE)
		preguntar_defensores(juego, (*elfos_inicio), ELFO);

	detener_el_tiempo(TIEMPO_INICIAL);
}

/*
* Pre: accion debe ser ARRIBA o ABAJO.
* Post: Muestra el menú inicial del juego, según la acción del usuario.
*/
void mostrar_menu_inicial(char accion, int nivel_inicial) {

	system("clear");
	
	printf("\n");
	printf("\t██████╗ ███████╗███████╗███████╗███╗   ██╗██████╗ ██╗███████╗███╗   ██╗██████╗  ██████╗         ██╗      █████╗ ███████╗        ████████╗ ██████╗ ██████╗ ██████╗ ███████╗███████╗\n");
	printf("\t██╔══██╗██╔════╝██╔════╝██╔════╝████╗  ██║██╔══██╗██║██╔════╝████╗  ██║██╔══██╗██╔═══██╗        ██║     ██╔══██╗██╔════╝        ╚══██╔══╝██╔═══██╗██╔══██╗██╔══██╗██╔════╝██╔════╝\n");
	printf("\t██║  ██║█████╗  █████╗  █████╗  ██╔██╗ ██║██║  ██║██║█████╗  ██╔██╗ ██║██║  ██║██║   ██║        ██║     ███████║███████╗           ██║   ██║   ██║██████╔╝██████╔╝█████╗  ███████╗\n");
	printf("\t██║  ██║██╔══╝  ██╔══╝  ██╔══╝  ██║╚██╗██║██║  ██║██║██╔══╝  ██║╚██╗██║██║  ██║██║   ██║        ██║     ██╔══██║╚════██║           ██║   ██║   ██║██╔══██╗██╔══██╗██╔══╝  ╚════██║\n");
	printf("\t██████╔╝███████╗██║     ███████╗██║ ╚████║██████╔╝██║███████╗██║ ╚████║██████╔╝╚██████╔╝        ███████╗██║  ██║███████║           ██║   ╚██████╔╝██║  ██║██║  ██║███████╗███████║\n");
	printf("\t╚═════╝ ╚══════╝╚═╝     ╚══════╝╚═╝  ╚═══╝╚═════╝ ╚═╝╚══════╝╚═╝  ╚═══╝╚═════╝  ╚═════╝         ╚══════╝╚═╝  ╚═╝╚══════╝           ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝\n");
	printf("\n\n\n");

	if (accion == ARRIBA) {

		printf("\t\t\t\t\t\t\t\t\t     __ _     __   ___ _  _           __ __ _ \n");
		printf("\t\t\t\t\t\t\t\t\t\\\\  /  / \\|V||_ |\\| _/|_||_)     || ||_ /__/ \\\n");
		printf("\t\t\t\t\t\t\t\t\t//  \\__\\_/| ||__| |/__| || \\   \\_||_||__\\_|\\_/\n");
	} else if (accion == ABAJO) {

	printf("\t\t\t\t\t\t\t\t\t   __ _     __   ___ _  _           __ __ _ \n");
	printf("\t\t\t\t\t\t\t\t\t  /  / \\|V||_ |\\| _/|_||_)     || ||_ /__/ \\\n");
	printf("\t\t\t\t\t\t\t\t\t  \\__\\_/| ||__| |/__| || \\   \\_||_||__\\_|\\_/\n");

	}

	printf("\n\n");

	printf("\t\t\t\t\t\t\t\t\t /\\\n");
	printf("\t\t\t\t\t\t\t\t\t || W\n");
	printf("\t\t\t\t\t\t\t\t\t ||\n");

	printf("\n\n");

	printf("\t\t\t\t\t\t\t\t\t ||\n");
	printf("\t\t\t\t\t\t\t\t\t || S\n");
	printf("\t\t\t\t\t\t\t\t\t \\/\n");
	
	printf("\n\n");

	if (accion == ARRIBA) {

		printf("\t\t\t\t\t\t\t\t\t   __ _     __   ___ _  _     _  __ __ _  __      ___    __   \n");
		printf("\t\t\t\t\t\t\t\t\t  /  / \\|V||_ |\\| _/|_||_)   | \\|_ (_ | \\|_    |\\| |    |_ |  \n");
		printf("\t\t\t\t\t\t\t\t\t  \\__\\_/| ||__| |/__| || \\   |_/|____)|_/|__   | |_|_\\_/|__|__\n");

	} else if (accion == ABAJO) {

		printf("\t\t\t\t\t\t\t\t\t     __ _     __   ___ _  _     _  __ __ _  __      ___    __   \n");
		printf("\t\t\t\t\t\t\t\t\t\\\\  /  / \\|V||_ |\\| _/|_||_)   | \\|_ (_ | \\|_    |\\| |    |_ |  \n");
		printf("\t\t\t\t\t\t\t\t\t//  \\__\\_/| ||__| |/__| || \\   |_/|____)|_/|__   | |_|_\\_/|__|__\n");
	}	
}

/*
* Pre: nivel_inicial debe ser ESTE.
* Post: Realiza la interacción entre el usuario y el menú inicial.
	Si la acción es ARRIBA o ARRIBA_MINUS, inicia el juego. Sino, 
	si es ABAJO o ABAJO_MINUS, pide al usuario el nivel por el que
	quiere comenzar.
*/
void interaccion_menu_inicial(char* accion, int* nivel_inicial) {
	bool iniciar_juego = false;
	while (!iniciar_juego) {
		do {
			printf("\nAcción -> ");
			scanf(" %c", accion);
		} while ((*accion) != ABAJO && (*accion) != ARRIBA &&
		 	(*accion) != ABAJO_MINUS && (*accion) != ARRIBA_MINUS);

		if ((*accion) == ABAJO || (*accion) == ABAJO_MINUS) {

			mostrar_menu_inicial(ABAJO, (*nivel_inicial));
			do {
				printf("\nNivel -> ");
				scanf(" %i", nivel_inicial);
			} while ((*nivel_inicial) > SUR || (*nivel_inicial) < ESTE);
		}

		else if ((*accion) == ARRIBA || (*accion) == ARRIBA_MINUS) {

			iniciar_juego = true;
		}
	}
	system("clear");
}

/*
* Pre: -
* Post: Imprime por pantalla el mensaje para cuando el usuario perdió el juego.
*/
void mostrar_juego_perdido() {
	printf("\n\n\n");
	//printf("\n\n\t\t\t G A M E   O V E R\n\n");	

	printf("\t\t\t\t\t\t\t\t @@@@@@@@      @@@@@@      @@@@@@@@@@      @@@@@@@@               @@@@@@      @@@  @@@     @@@@@@@@     @@@@@@@\n");
	printf("\t\t\t\t\t\t\t\t@@@@@@@@@     @@@@@@@@     @@@@@@@@@@@     @@@@@@@@              @@@@@@@@     @@@  @@@     @@@@@@@@     @@@@@@@@\n");
	printf("\t\t\t\t\t\t\t\t!@@           @@!  @@@     @@! @@! @@!     @@!                   @@!  @@@     @@!  @@@     @@!          @@!  @@@  \n");
	printf("\t\t\t\t\t\t\t\t!@!           !@!  @!@     !@! !@! !@!     !@!                   !@!  @!@     !@!  @!@     !@!          !@!  @!@  \n");
	printf("\t\t\t\t\t\t\t\t!@! @!@!@     @!@!@!@!     @!! !!@ @!@     @!!!:!                @!@  !@!     @!@  !@!     @!!!:!       @!@!!@!   \n");
	printf("\t\t\t\t\t\t\t\t!!! !!@!!     !!!@!!!!     !@!   ! !@!     !!!!!:                !@!  !!!     !@!  !!!     !!!!!:       !!@!@!    \n");
	printf("\t\t\t\t\t\t\t\t:!!   !!:     !!:  !!!     !!:     !!:     !!:                   !!:  !!!     :!:  !!:     !!:          !!: :!!   \n");
	printf("\t\t\t\t\t\t\t\t:!:   !::     :!:  !:!     :!:     :!:     :!:                   :!:  !:!      ::!!:!      :!:          :!:  !:!  \n");
	printf("\t\t\t\t\t\t\t\t ::: ::::     ::   :::     :::     ::       :: ::::              ::::: ::       ::::        :: ::::     ::   :::  \n");
	printf("\t\t\t\t\t\t\t\t :: :: :       :   : :      :      :       : :: ::                : :  :         :         : :: ::       :   : :  \n");

	printf("\n\n");

	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´´´´´´´ ¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶´´´´´´´´´´´´´´´´´´´`\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´´´´´¶¶¶¶¶¶´´´´´´´´´´´´´¶¶¶¶¶¶¶´´´´´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´´¶¶¶¶´´´´´´´´´´´´´´´´´´´´´´´¶¶¶¶´´´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´¶¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´¶¶´´´´´´´´´´´´´´´´´´´´´`´´´´´´´´´´´¶¶´´´´´´´´´´`\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´¶¶´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´¶´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´¶´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶´´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´¶¶´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶´´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´¶¶´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´¶¶´¶¶´´´¶¶¶¶¶¶¶¶´´´´´¶¶¶¶¶¶¶¶´´´¶¶´¶¶´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´¶¶¶¶´¶¶¶¶¶¶¶¶¶¶´´´´´¶¶¶¶¶¶¶¶¶¶´¶¶¶¶¶´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´¶¶¶´¶¶¶¶¶¶¶¶¶¶´´´´´¶¶¶¶¶¶¶¶¶¶´¶¶¶´´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´¶¶¶´´´´´´´¶¶´´¶¶¶¶¶¶¶¶´´´´´´´¶¶¶¶¶¶¶¶¶´´¶¶´´´´´´¶¶¶¶´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´¶¶¶¶¶´´´´´¶¶´´´¶¶¶¶¶¶¶´´´¶¶¶´´´¶¶¶¶¶¶¶´´´¶¶´´´´´¶¶¶¶¶¶´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´¶¶´´´¶¶´´´´¶¶´´´´´¶¶¶´´´´¶¶¶¶¶´´´´¶¶¶´´´´´¶¶´´´´¶¶´´´¶¶´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´¶¶¶´´´´¶¶¶¶´´¶¶´´´´´´´´´´¶¶¶¶¶¶¶´´´´´´´´´´¶¶´´¶¶¶¶´´´´¶¶¶´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t¶¶´´´´´´´´´¶¶¶¶¶¶¶¶´´´´´´´¶¶¶¶¶¶¶´´´´´´´¶¶¶¶¶¶¶¶¶´´´´´´´´¶¶\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t¶¶¶¶¶¶¶¶¶´´´´´¶¶¶¶¶¶¶¶´´´´¶¶¶¶¶¶¶´´´´¶¶¶¶¶¶¶¶´´´´´´¶¶¶¶¶¶¶¶\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´¶¶¶¶´¶¶¶¶¶´´´´´´¶¶¶¶¶´´´´´´´´´´´´´´¶¶¶´¶¶´´´´´¶¶¶¶¶¶´¶¶¶´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´¶¶¶¶¶¶´´¶¶¶´´¶¶´´´´´´´´´´´¶¶´´¶¶¶´´¶¶¶¶¶¶´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´´¶¶¶¶¶¶´¶¶´¶¶¶¶¶¶¶¶¶¶¶´¶¶´¶¶¶¶¶¶´´´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´´´´´´¶¶´¶¶´¶´¶´¶´¶´¶´¶´¶´¶´¶¶´´´´´´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´´´´´¶¶¶¶´´¶´¶´¶´¶´¶´¶´¶´¶´´´¶¶¶¶¶´´´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´´´´´´¶¶¶¶¶´¶¶´´´¶¶¶¶¶¶¶¶¶¶¶¶¶´´´¶¶´¶¶¶¶¶´´´´´´´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´¶¶¶¶¶¶¶¶¶¶´´´´´¶¶´´´´´´´´´´´´´´´´´¶¶´´´´´´¶¶¶¶¶¶¶¶¶´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´¶¶´´´´´´´´´´´¶¶¶¶¶¶¶´´´´´´´´´´´´´¶¶¶¶¶¶¶¶´´´´´´´´´´¶¶´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´¶¶¶´´´´´¶¶¶¶¶´´´´´¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶´´´´´¶¶¶¶¶´´´´´¶¶¶´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´¶¶´´´¶¶¶´´´´´´´´´´´¶¶¶¶¶¶¶¶¶´´´´´´´´´´´¶¶¶´´´¶¶´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´¶¶´´¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶´´¶¶´´´´´´\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t\t´´´´´´´¶¶¶¶´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¶¶¶¶´´´´´´´\n");


	printf("\n\n\n");

}

/*
* Pre: -
* Post: Imprime por pantalla el mensaje para cuando el usuario perdió el juego.
*/
void mostrar_juego_ganado() {
	printf("\n\n\n");

	printf("__/\\\\\\________/\\\\\\_________________/\\\\\\\\\\_________________/\\\\\\________/\\\\\\________________________________/\\\\\\______________/\\\\\\____________/\\\\\\\\\\\\\\\\\\\\\\____________/\\\\\\\\\\_____/\\\\\\_\n");
	printf(" _\\///\\\\\\____/\\\\\\/________________/\\\\\\///\\\\\\______________\\/\\\\\\_______\\/\\\\\\_______________________________\\/\\\\\\_____________\\/\\\\\\___________\\/////\\\\\\///____________\\/\\\\\\\\\\\\___\\/\\\\\\_\n");
	printf("  ___\\///\\\\\\/\\\\\\/________________/\\\\\\/__\\///\\\\\\____________\\/\\\\\\_______\\/\\\\\\_______________________________\\/\\\\\\_____________\\/\\\\\\_______________\\/\\\\\\_______________\\/\\\\\\/\\\\\\__\\/\\\\\\_\n"); 
	printf("   _____\\///\\\\\\/_________________/\\\\\\______\\//\\\\\\___________\\/\\\\\\_______\\/\\\\\\_______________________________\\//\\\\\\____/\\\\\\____/\\\\\\________________\\/\\\\\\_______________\\/\\\\\\//\\\\\\_\\/\\\\\\_\n");
	printf("    _______\\/\\\\\\_________________\\/\\\\\\_______\\/\\\\\\___________\\/\\\\\\_______\\/\\\\\\________________________________\\//\\\\\\__/\\\\\\\\\\__/\\\\\\_________________\\/\\\\\\_______________\\/\\\\\\\\//\\\\\\\\/\\\\\\_\n");
	printf("     _______\\/\\\\\\_________________\\//\\\\\\______/\\\\\\____________\\/\\\\\\_______\\/\\\\\\_________________________________\\//\\\\\\/\\\\\\/\\\\\\/\\\\\\__________________\\/\\\\\\_______________\\/\\\\\\_\\//\\\\\\/\\\\\\_\n");
	printf("      _______\\/\\\\\\__________________\\///\\\\\\__/\\\\\\______________\\//\\\\\\______/\\\\\\___________________________________\\//\\\\\\\\\\\\//\\\\\\\\\\___________________\\/\\\\\\_______________\\/\\\\\\__\\//\\\\\\\\\\\\_ \n");
	printf("       _______\\/\\\\\\____________________\\///\\\\\\\\\\/________________\\///\\\\\\\\\\\\\\\\\\/_____________________________________\\//\\\\\\__\\//\\\\\\_________________/\\\\\\\\\\\\\\\\\\\\\\___________\\/\\\\\\___\\//\\\\\\\\\\_\n");
	printf("        _______\\///_______________________\\/////____________________\\/////////________________________________________\\///____\\///_________________\\///////////____________\\///_____\\/////__\n");

	printf("\n\n\n");
}

/*
* Pre: -
* Post: Pregunta el nombre al usuario, y lo guarda en la variable nombre.
*/
void preguntar_nombre_jugador(char nombre[MAX_NOMBRE]) {
	printf("Ingrese su nombre para el ranking de puntos: ");
	scanf(" %s", nombre);
}


/*
* Pre: tope_enemigos debe ser válido.
* Post: Devuelve la cantidad total de orcos muertos que hay en el vector
	enemigos recibido.
*/
int orcos_muertos_segun_nivel(enemigo_t enemigos[], int tope_enemigos) {
	int orcos_muertos = 0;
	for (int i = 0; i < tope_enemigos; i++) {
		// Si el enemigo está muerto y no está en alguna torre
		if (enemigos[i].vida <= VIDA_NULA)
			orcos_muertos++;
	}
	return orcos_muertos;
}

/*
* Pre: el archivo debe estar abierto en modo lectura (Puede estar desordenado).	
* Post: Lee el contenido del archivo recibido, y lo asigna al struct configuracion,
	según corresponda.
*/
int leer_configuracion(configuracion_t* configuracion, FILE* config) {

	char lectura[MAX_LECTURA];
	int leido = fscanf(config, "%[^=]=", lectura);
	if (strcmp(lectura, "RESISTENCIA_TORRES") == 0) {
		leido = fscanf(config, "%i, %i\n", 
			&((*configuracion).resistencia_torre_1), &((*configuracion).resistencia_torre_2));
	}
	if (strcmp(lectura, "ENANOS_INICIO") == 0) {
		leido = fscanf(config, "%i, %i, %i, %i\n", 
			&((*configuracion).enanos.cant_inicial[ESTE-1]), 
			&((*configuracion).enanos.cant_inicial[OESTE-1]), 
			&((*configuracion).enanos.cant_inicial[NORTE-1]), 
			&((*configuracion).enanos.cant_inicial[SUR-1]));
	}
	if (strcmp(lectura, "ELFOS_INICIO") == 0) {
		leido = fscanf(config, "%i, %i, %i, %i\n", 
			&((*configuracion).elfos.cant_inicial[ESTE-1]), 
			&((*configuracion).elfos.cant_inicial[OESTE-1]), 
			&((*configuracion).elfos.cant_inicial[NORTE-1]), 
			&((*configuracion).elfos.cant_inicial[SUR-1]));
	}
	if (strcmp(lectura, "ENANOS_EXTRA") == 0) {
		leido = fscanf(config, "%i, %i, %i\n", &((*configuracion).enanos.cant_extra), 
			&((*configuracion).enanos.costo_torre_1), &((*configuracion).enanos.costo_torre_2));
	}
	if (strcmp(lectura, "ELFOS_EXTRA") == 0) {
		leido = fscanf(config, "%i, %i, %i\n", &((*configuracion).elfos.cant_extra), 
			&((*configuracion).elfos.costo_torre_1), &((*configuracion).elfos.costo_torre_2));
	}
	if (strcmp(lectura, "ENANOS_ANIMO") == 0) {
		leido = fscanf(config, "%i, %i\n", &((*configuracion).enanos.fallo), 
			&((*configuracion).enanos.critico));
	}
	if (strcmp(lectura, "ELFOS_ANIMO") == 0) {
		leido = fscanf(config, "%i, %i\n", &((*configuracion).elfos.fallo), 
			&((*configuracion).elfos.critico));
	}
	if (strcmp(lectura, "VELOCIDAD") == 0) {
		leido = fscanf(config, "%f\n", &((*configuracion).velocidad));
		printf("Velocidad: %f\n", (*configuracion).velocidad);
	}
	if (strcmp(lectura, "CAMINOS") == 0) {
		leido = fscanf(config, "%s\n", (*configuracion).ruta_caminos);
		printf("Ruta de caminos: %s\n\n", (*configuracion).ruta_caminos);
	}

	return leido;
}
/*
* Pre: -
* Post: Precarga la configuración recibida con VALOR_POR_DEFECTO (-1)
*/
void precargar_config_por_defecto (configuracion_t* configuracion) {
	(*configuracion).resistencia_torre_1 = VALOR_POR_DEFECTO;
	(*configuracion).resistencia_torre_2 = VALOR_POR_DEFECTO;
	(*configuracion).enanos.cant_extra = VALOR_POR_DEFECTO;
	(*configuracion).elfos.cant_extra = VALOR_POR_DEFECTO;
	for (int i = 0; i < MAX_NIVELES; i++) {
		(*configuracion).enanos.cant_inicial[i] = VALOR_POR_DEFECTO;
		(*configuracion).elfos.cant_inicial[i] = VALOR_POR_DEFECTO;
	}
	(*configuracion).enanos.fallo = VALOR_POR_DEFECTO;
	(*configuracion).enanos.critico = VALOR_POR_DEFECTO;
	(*configuracion).elfos.fallo = VALOR_POR_DEFECTO;
	(*configuracion).elfos.critico = VALOR_POR_DEFECTO;
	(*configuracion).velocidad = VALOR_POR_DEFECTO;
	strcpy((*configuracion).ruta_caminos, RUTA_POR_DEFECTO);
}

/*
* Pre: configuracion debe ser válido, con todas sus estructuras válidas.
* Post: Asgina la configuración personalizada al struct configuración.
*/
bool asignar_config_personalizada(configuracion_t* configuracion) {

	FILE* config = fopen((*configuracion).ruta_config, LECTURA);

	if(config == NULL) {
		return false;
	}

	int leido = leer_configuracion(configuracion, config);

	while(leido != EOF) {
		leido = leer_configuracion(configuracion, config);
	}

	fclose(config);
	return true;
}


/*
* Pre: Las variables recibidas deben ser > 0
* Post: Devuelve la sumatoria total de puntos acumulados en la partida.
*/
int suma_puntos_totales(int orcos_muertos, int resistencia_torre_1, 			
		int resistencia_torre_2, int enanos_inicio, int elfos_inicio, 
		int enanos_extra, int elfos_extra) {
	if (resistencia_torre_1 == VALOR_POR_DEFECTO) resistencia_torre_1 = VIDA_INICIAL_TORRE;
	if (resistencia_torre_2 == VALOR_POR_DEFECTO) resistencia_torre_2 = VIDA_INICIAL_TORRE;
	if (enanos_extra == VALOR_POR_DEFECTO) enanos_extra = DEFENSORES_EXTRA;
	if (elfos_extra == VALOR_POR_DEFECTO) elfos_extra = DEFENSORES_EXTRA;

	return ( (orcos_muertos * MULTIPLICADOR_PUNTAJE) / (resistencia_torre_1 + 
		resistencia_torre_2 + enanos_inicio + elfos_inicio + enanos_extra + elfos_extra) );
}

/*
* Pre: -
* Post: Lee el nivel del archivo y lo guarda en el nivel recibido.
*/
void leer_nivel(FILE* archivo, int* nivel) {
	fscanf(archivo, "NIVEL=%i\n", nivel);	
}


/*
* Pre: -
* Post: Lee el número de camino del archivo y lo guarda en el parámetro recibido.
*/
void leer_num_camino(FILE* archivo, int* num_camino) {
	fscanf(archivo, "CAMINO=%i\n", num_camino);
}

/*
* Pre: -
* Post: Lee una coordenada del camino del archivo y la guarda en la coordenada recibida.
*/
int leer_camino(FILE* archivo, coordenada_t* camino) {
	return fscanf(archivo, "%i;%i\n", &((*camino).fil), &((*camino).col));
}

/*
* Pre: El archivo de caminos debe estar ordenado por nivel, y luego por numero de camino. La ruta debe ser válida.
* Post: Lee el archivo de caminos de la ruta recibida y lo asigna al vector de caminos recibido.
*/
int leer_archivo_caminos(char ruta[MAX_RUTA], camino_t caminos[MAX_CAMINOS]) {

	FILE* archivo = fopen(ruta, "r");
	int aux;
	if (archivo == NULL) {
		printf("No pudo abrirse el archivo de caminos.\n");
		return ERROR;
	}

	for (int i = 0; i < MAX_CAMINOS; i++) {
		leer_nivel(archivo, &aux);
		leer_num_camino(archivo, &(caminos[i].num_camino));
		caminos[i].tope_camino = 0;
		int leidos = leer_camino(archivo, &(caminos[i].camino[caminos[i].tope_camino]));
		while(leidos == LEIDOS_CAMINO) {
			caminos[i].tope_camino++;
			leidos = leer_camino(archivo, &(caminos[i].camino[caminos[i].tope_camino]));
		}
	}

	fclose(archivo);

	return EXITO;
}

/*
* Pre: configuracion debe ser válida, con todas sus estructuras válidas.
* Post: Actualiza el ranking, preguntándole el nombre al jugador, y calculando
	los puntos.
*/
void actualizar_ranking(configuracion_t configuracion, int orcos_muertos, 
	int enanos_inicio, int elfos_inicio) {

	char nombre[MAX_NOMBRE];
	preguntar_nombre_jugador(nombre);
	int puntos = suma_puntos_totales(orcos_muertos, configuracion.resistencia_torre_1, 
		configuracion.resistencia_torre_2, enanos_inicio, elfos_inicio, 
		configuracion.enanos.cant_extra, configuracion.elfos.cant_extra);

	guardar_jugador_en_ranking(nombre, puntos, configuracion.ruta_config);
}

/*
* Pre: Las rutas deben ser válidas.
* Post: Juega la partida.
*/
void jugar_partida(juego_t* juego, configuracion_t* configuracion) {

	int viento, humedad;
	char animo_legolas, animo_gimli;
	srand ((unsigned)time(NULL));
	int nivel_inicial = ESTE;
	char accion = ARRIBA;

	mostrar_menu_inicial(accion, nivel_inicial);
	interaccion_menu_inicial(&accion, &nivel_inicial);

	// Función del TP1
	animos(&viento, &humedad, &animo_legolas, &animo_gimli);
	detener_el_tiempo(TIEMPO_INICIAL);

	int enanos_inicio_totales = 0;
	int elfos_inicio_totales = 0;
	// Si juega con caminos personalizados
	if (strcmp((*configuracion).ruta_caminos, RUTA_POR_DEFECTO) != 0) {
		leer_archivo_caminos((*configuracion).ruta_caminos, (*configuracion).caminos);
	}

	if ((*configuracion).velocidad == VALOR_POR_DEFECTO)
		(*configuracion).velocidad = TIEMPO_TURNO;

	inicializar_juego(juego, viento, humedad, animo_legolas, animo_gimli, *configuracion);

	coordenada_t entrada_1;
	coordenada_t entrada_2;

	coordenada_t torre_1;
	coordenada_t torre_2;
		
	int i = nivel_inicial;
	int orcos_muertos = 0;

	FILE* archivo_grabacion; // Si hay ruta de grabación, abro el archivo para grabar
	if(strcmp((*configuracion).ruta_grabacion, RUTA_POR_DEFECTO) != 0) {
		archivo_grabacion = fopen((*configuracion).ruta_grabacion, ESCRITURA);
	}

	while( (estado_juego(*juego) == JUGANDO) && (i <= MAX_NIVELES) ) {
		
		(*juego).nivel_actual = i;
		inicializar_nivel(juego, &entrada_1, &entrada_2, &torre_1, &torre_2,			
			&((*configuracion).enanos.cant_inicial[i-1]), &((*configuracion).elfos.cant_inicial[i-1]), 
			(*configuracion).caminos, (*configuracion).ruta_caminos);

		do {
			mostrar_juego(*juego);
			jugar_turno(juego);

			if(strcmp((*configuracion).ruta_grabacion, RUTA_POR_DEFECTO) != 0)
				fwrite(juego, sizeof(juego_t), 1, archivo_grabacion);

			if ( puede_agregar_extra((*juego).nivel.tope_enemigos, (*juego).nivel_actual, (*juego).nivel.max_enemigos_nivel)  )
				preguntar_defensor_extra(&((*juego).nivel), &((*juego).torres), (*juego).nivel_actual);

			detener_el_tiempo((*configuracion).velocidad);
		} while ( (estado_nivel((*juego).nivel) == JUGANDO) && (estado_juego(*juego) == JUGANDO) );

		orcos_muertos += orcos_muertos_segun_nivel((*juego).nivel.enemigos, (*juego).nivel.tope_enemigos);

		enanos_inicio_totales += (*configuracion).enanos.cant_inicial[i-1];
		elfos_inicio_totales += (*configuracion).elfos.cant_inicial[i-1];
		
		i++;
	}
	// Si estaba grabando, cierro el archivo de grabación
	if(strcmp((*configuracion).ruta_grabacion, RUTA_POR_DEFECTO) != 0) {
		fclose(archivo_grabacion);
	}
	
	mostrar_juego(*juego);

	if (estado_juego(*juego) == PERDIDO)
		mostrar_juego_perdido();
	else 
		mostrar_juego_ganado();

	actualizar_ranking((*configuracion), orcos_muertos, enanos_inicio_totales, elfos_inicio_totales);
}

/*
* Pre: separador debe ser '='
* Post: Gestiona los parámetros del ranking. Si alguno es listar, asigna la cantidad de jugadores del
	parámetro. Si alguno es config, asigna la ruta del parámetro a la ruta_config.
*/
void gestionar_parametro_ranking(char parametro[], char parte_uno[MAX_RUTA], char parte_dos[MAX_RUTA], 
	char separador, int* cantidad_jugadores, char ruta_config[MAX_RUTA]) {

	int pos_separador = cortar_string_parte_uno(parametro, parte_uno, separador);
	cortar_string_parte_dos(parametro, parte_dos, pos_separador);


	if (strcmp(parte_uno, "listar") == 0)
		(*cantidad_jugadores) = atoi(parte_dos);

	if (strcmp(parte_uno, "config") == 0)
		strcpy(ruta_config, parte_dos);
}

/*
* Pre: separador debe ser '='
* Post: Gestiona los parámetros del comando de la repe. Si alguno es grabacion, asigna la 
	ruta recibida en el parámetro a ruta_grabación. Si alguno es velocidad, asigna la velocidad
	del parámetro la velocidad recibida.
*/
void gestionar_parametro_repe(char parametro[], char parte_uno[MAX_RUTA], char parte_dos[MAX_RUTA], 
	char separador, float* velocidad, char ruta_grabacion[MAX_RUTA]) {

	int pos_separador = cortar_string_parte_uno(parametro, parte_uno, separador);
	cortar_string_parte_dos(parametro, parte_dos, pos_separador);

	if (strcmp(parte_uno, "grabacion") == 0)
		strcpy(ruta_grabacion, parte_dos);

	if (strcmp(parte_uno, "velocidad") == 0)
		(*velocidad) = (float)atof(parte_dos);
}

/*
* Pre: separador debe ser '='
* Post: Gestiona los parámetros del comando jugar. Si alguno es grabación, asigna la ruta del parámetro
	a la ruta_grabacion de configuracion. Si alguno es config, asigna la ruta del parámetro a ruta_config.
*/
int gestionar_parametro_jugar(char parametro[], char parte_uno[MAX_RUTA], char parte_dos[MAX_RUTA], 
	char separador, configuracion_t* configuracion) {

	int pos_separador = cortar_string_parte_uno(parametro, parte_uno, separador);
	cortar_string_parte_dos(parametro, parte_dos, pos_separador);

	if (strcmp(parte_uno, "grabacion") == 0)
		strcpy((*configuracion).ruta_grabacion, parte_dos);

	if (strcmp(parte_uno, "config") == 0) {
		strcpy((*configuracion).ruta_config, parte_dos);
		// Leo la configuración y la asigno al struct
		bool config_asignada = asignar_config_personalizada(configuracion);
		// Si no se pudo asignar la configuración
		if (!config_asignada) {
			printf("No pudo abrirse el archivo de configuración solicitado.\n");
			return ERROR;
		}
	}
	return EXITO;
}

/*
* Pre: -
* Post: Inicializa las rutas del struct de configuración con
	los valores por defecto.
*/
void inicializar_rutas(configuracion_t* configuracion) {
	strcpy((*configuracion).ruta_config, RUTA_POR_DEFECTO);
	strcpy((*configuracion).ruta_ranking, RUTA_POR_DEFECTO);
	strcpy((*configuracion).ruta_grabacion, RUTA_POR_DEFECTO);
	strcpy((*configuracion).ruta_caminos, RUTA_POR_DEFECTO);
}

/*
* Pre: -
* Post: Muestra por pantalla el menú de ayuda de comandos.
*/
void mostrar_menu_ayuda() {
	printf("\n\t\t\tDEFENDIENDO LAS TORRES - MENÚ DE AYUDA\n\n\n");
	printf(VERDE_CLARO "- Para jugar con valores por defecto" FIN ": ./juego jugar\n\n");
	printf(VERDE_CLARO "- Para jugar con configuración personalizada" FIN ": ./juego jugar config=archivo_config.txt\n");
	printf(VERDE_CLARO "- Para crear una configuración personalizada" FIN ": ./juego crear_configuracion archivo_configuracion.txt\n");
	printf("- Se le solicitarán los datos de la configuración (los caminos personalizados se asignan ahí) y se sobreescribirá el archivo!\n\n");
	printf(VERDE_CLARO"- Para jugar y grabar la partida" FIN ": ./juego jugar grabacion=archivo_grabacion.dat\n");
	printf(VERDE_CLARO"- Para ver la grabación de una partida" FIN ": ./juego poneme_la_repe grabacion=archivo_grabacion.dat velocidad=0.5\n");
	printf("- Puede elegir la velocidad y archivo que desee!\n\n");
	printf(VERDE_CLARO"- Para crear caminos personalizados y así luego jugar con ellos" FIN ": ./juego crear_camino archivo_caminos.txt\n");
	printf("- Se le solicitarán los caminos personalizados y se sobreescribirá el archivo!\n\n");


	printf(VERDE_CLARO "- Para mostrar el ranking del juego" FIN ": ./juego ranking listar=N config=archivo_configuracion.txt\n");
	printf("- listar=N es la cantidad máxima de jugadores que listará el ranking, siendo N un número mayor a 0. En caso de no recibir la cantidad de jugadores a mostrar, se mostrarán todos.\n");
	printf("- config=archivo_configuracion.txt es el archivo de configuración para el cuál se mostrará el ranking. Cada archivo tiene un ranking distinto. En caso de no recibir el archivo de configuración se mostrará el ranking de la configuración por defecto.\n\n");
}

/*
* Pre: -
* Post: Gestiona los parámetros recibidos.
*/
int gestionar_parametros(int argc, char* argv[]) {
	
	juego_t juego;
	configuracion_t configuracion;

	char parte_uno[MAX_RUTA] = STRING_VACIO;
	char separador = SEPARADOR_IGUAL; 
	char parte_dos[MAX_RUTA] = STRING_VACIO;

	int cantidad_jugadores = VALOR_POR_DEFECTO;

	inicializar_rutas(&configuracion);
	// COMANDO AYUDA
	if (strcmp(argv[POS_PRIMER_ARGUMENTO], COMANDO_AYUDA) == 0){	

		mostrar_menu_ayuda();
	// COMANDO CREAR CONFIGURACIÓN
	} else if (strcmp(argv[POS_PRIMER_ARGUMENTO], COMANDO_CREAR_CONFIG) == 0) {

		if (argc == CANT_ARGUMENTOS_CONFIG) crear_configuracion(argv[POS_SEGUNDO_ARGUMENTO], &configuracion);
		else return ERROR;
	// COMANDO RANKING
	} else if (strcmp(argv[POS_PRIMER_ARGUMENTO], COMANDO_RANKING) == 0) {

		for (int i = MAX_SUBARGUMENTOS; i < argc; i++) {
			gestionar_parametro_ranking(argv[i], parte_uno, parte_dos, separador, 
					&cantidad_jugadores, configuracion.ruta_config);
		}

		mostrar_ranking(cantidad_jugadores, configuracion.ruta_config);
	// COMANDO CREAR CAMINO
	} else if (strcmp(argv[POS_PRIMER_ARGUMENTO], COMANDO_CREAR_CAMINO) == 0) {
		
		// Esto crea el camino si funciona
		if (argc != CANT_ARGUMENTOS_CAMINOS || crear_camino(argv[POS_SEGUNDO_ARGUMENTO]) == ERROR){
			return ERROR;
		}
	// COMANDO REPE
	} else if (strcmp(argv[POS_PRIMER_ARGUMENTO], COMANDO_REPE) == 0) {

		if (argc < CANT_ARGUMENTOS_REPE)
			return ERROR;

		configuracion.velocidad = VELOCIDAD_REPE_PREDETERMINADA;
		for (int i = MAX_SUBARGUMENTOS; i < argc; i++) {
			gestionar_parametro_repe(argv[i], parte_uno, parte_dos, separador, 
					&(configuracion.velocidad), configuracion.ruta_grabacion);
		}

		poneme_la_repe(configuracion.ruta_grabacion, configuracion.velocidad);
	// COMANDO JUGAR
	} else if (strcmp(argv[POS_PRIMER_ARGUMENTO], COMANDO_JUGAR) == 0)/* Se juega */ {

		for (int i = MAX_SUBARGUMENTOS; i < argc; i++) {
			printf("%s\n", argv[i]);

			gestionar_parametro_jugar(argv[i], parte_uno, parte_dos, 
				separador, &configuracion);
		}

		if (strcmp(configuracion.ruta_config, RUTA_POR_DEFECTO) == 0)
			precargar_config_por_defecto(&configuracion);

		jugar_partida(&juego, &configuracion);
	}
	return EXITO;
}


int main(int argc, char* argv[]) {
	if (argc < MIN_ARGUMENTOS) {
		printf("Argumentos insuficientes. Usar el argumento 'ayuda' para recibir ayuda con los comandos.\n");
		return ERROR;
	}
	return gestionar_parametros(argc, argv);
}
