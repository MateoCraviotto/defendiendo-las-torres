#include "config.h"
#include "utiles.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FIN_STRING '\0'

#define SEPARADOR_EXTENSION '.'
#define EXTENSION_CSV ".csv"

const char ARRIBA = 'w';
const char ABAJO = 's';
const char IZQUIERDA = 'a';
const char DERECHA = 'd';

#define PRINT_ENTRADA "█"
#define VERDE_OSCURO "\033[0m\033[32m"

#define RUTA_RANKING_AUX "nuevo_ranking.txt"
#define RUTA_RANKING_DEFAULT "ranking_default.txt"
#define LEIDOS_ESPERADOS 2


/*
* Pre: -
* Post: Pregunta al usuario la configuación general del juego a asignar en las variables
	recibidas como parámetro.
*/
void preguntar_config_general(int* resistencia_torre_1, int* resistencia_torre_2,
		float* velocidad, char caminos[MAX_RUTA]) {
	do {
		printf("Indique la vida de la torre 1: ");
		scanf(" %i", resistencia_torre_1);
	} while((*resistencia_torre_1) < VALOR_POR_DEFECTO);

	do {
		printf("Indique la vida de la torre 2: ");
		scanf(" %i", resistencia_torre_2);
	} while((*resistencia_torre_2) < VALOR_POR_DEFECTO);

	do {
		printf("Indique la velocidad del juego: ");
		scanf(" %f", velocidad);
	} while((*velocidad) < (float)VALOR_POR_DEFECTO);

	printf("Indique la ruta del archivo del camino a utilizar: ");
	scanf(" %s", caminos);
}


/*
* Pre: -
* Post: Pregunta al usuario la configuración del defensor, a almacenar en el
	struct recibido.
*/
void preguntar_config_defensor(defensores_t* defensores) {

	//int predeterminado;
								
	printf("Iniciales, por nivel:\n\n");
	
	for (int i = 0; i < MAX_NIVELES; i++) {
		do {
			printf("\tNivel %i: ", i+1);
			scanf(" %i", &((*defensores).cant_inicial[i]));
			printf("\n");
		} while((*defensores).cant_inicial[i] < VALOR_POR_DEFECTO);
	}

	// Limpiar pantalla


	printf("Extras:\n\n");
	do {
		printf("Cantidad de defensores extra: ");
		scanf(" %i", &((*defensores).cant_extra));
		printf("\n");
	} while((*defensores).cant_extra < VALOR_POR_DEFECTO);

	do {
		printf("Costo por defensor extra a torre 1: ");
		scanf(" %i", &((*defensores).costo_torre_1));
		printf("\n");
	} while((*defensores).costo_torre_1 < VALOR_POR_DEFECTO);

	do {
		printf("Costo por defensor extra a torre 2: ");
		scanf(" %i", &((*defensores).costo_torre_2));
		printf("\n");
	} while((*defensores).costo_torre_2 < VALOR_POR_DEFECTO);

	do {
		printf("Probabilidad de critico: ");
		scanf(" %i", &((*defensores).critico));
		printf("\n");
	} while((*defensores).critico < VALOR_POR_DEFECTO);

	do {
		printf("Probabilidad de fallo: ");
		scanf(" %i", &((*defensores).fallo));
		printf("\n");
	} while((*defensores).fallo < VALOR_POR_DEFECTO);
}

/*
* Pre: -
* Post: Pregunta al usuario los valores de configuración a asignar en el struct recibido.
*/
void preguntar_valores(configuracion_t* configuracion) {

	printf("Ingrese a continuación los valores de configuración deseados.\n");
	printf("(Para dejar los valores por defecto, use -1)\n");

	preguntar_config_general( &((*configuracion).resistencia_torre_1),
	&((*configuracion).resistencia_torre_2), &((*configuracion).velocidad), 
	(*configuracion).ruta_caminos);

	printf("Para los enanos:\n");
	preguntar_config_defensor(&((*configuracion).enanos));
	printf("\n\n\n");
	printf("Para los elfos:\n");
	preguntar_config_defensor(&((*configuracion).elfos));
}

/*
* Pre: El archivo debe estar abierto en modo escritura. configuracion debe ser válida con 
	todas sus estructuras válidas.
* Post: Escribe la configuración recibida en el archivo recibido. 
*/
void escribir_configuracion(FILE* archivo, configuracion_t configuracion) {
	
	fprintf(archivo, "RESISTENCIA_TORRES=%i, %i\n", configuracion.resistencia_torre_1,
			configuracion.resistencia_torre_2);
	fprintf(archivo, "ENANOS_INICIO=%i, %i, %i, %i\n",
			configuracion.enanos.cant_inicial[ESTE], configuracion.enanos.cant_inicial[OESTE], 
			configuracion.enanos.cant_inicial[NORTE], configuracion.enanos.cant_inicial[SUR]);
	fprintf(archivo, "ELFOS_INICIO=%i, %i, %i, %i\n",
				configuracion.elfos.cant_inicial[ESTE], configuracion.elfos.cant_inicial[OESTE], 
				configuracion.elfos.cant_inicial[NORTE], configuracion.elfos.cant_inicial[SUR]);

	fprintf(archivo, "ENANOS_EXTRA=%i, %i, %i\n", configuracion.enanos.cant_extra, 
		configuracion.enanos.costo_torre_1, configuracion.enanos.costo_torre_2);

	fprintf(archivo, "ELFOS_EXTRA=%i, %i, %i\n", configuracion.elfos.cant_extra, 
		configuracion.elfos.costo_torre_1, configuracion.elfos.costo_torre_2);

	fprintf(archivo, "ENANOS_ANIMO=%i, %i\n", configuracion.enanos.fallo, 
		configuracion.enanos.critico);

	fprintf(archivo, "ELFOS_ANIMO=%i, %i\n", configuracion.elfos.fallo, 
		configuracion.elfos.critico);

	fprintf(archivo, "VELOCIDAD=%.2f\n", configuracion.velocidad);
	fprintf(archivo, "CAMINOS=%s\n", configuracion.ruta_caminos);
}

/*
* Pre: -
* Post: Precarga el struct de configuración con los valores por defecto (-1)
*/
void precargar_configuracion(configuracion_t* configuracion) {
	// Resistencia torres
	(*configuracion).resistencia_torre_1 = VALOR_POR_DEFECTO;
	(*configuracion).resistencia_torre_2 = VALOR_POR_DEFECTO;
	// Enanos y elfos iniciales
	for (int i = 0; i < MAX_NIVELES; i++) {
		(*configuracion).enanos.cant_inicial[i] = VALOR_POR_DEFECTO;
		(*configuracion).elfos.cant_inicial[i] = VALOR_POR_DEFECTO;
	}
	// Enanos y elfos extra
	(*configuracion).enanos.cant_extra = VALOR_POR_DEFECTO;
	(*configuracion).elfos.cant_extra = VALOR_POR_DEFECTO;

	// Fallos
	(*configuracion).enanos.fallo = VALOR_POR_DEFECTO;
	(*configuracion).elfos.fallo = VALOR_POR_DEFECTO;
	// Críticos
	(*configuracion).enanos.critico = VALOR_POR_DEFECTO;
	(*configuracion).elfos.critico = VALOR_POR_DEFECTO;
	// Velocidad del juego
	(*configuracion).velocidad = (float)VALOR_POR_DEFECTO;
	// Ruta del archivo de caminos
	strcpy((*configuracion).ruta_caminos, RUTA_POR_DEFECTO);

}


// FUNCIÓN PÚBLICA
void crear_configuracion(char ruta[MAX_RUTA], 
		configuracion_t* configuracion) {

	precargar_configuracion(configuracion);

	FILE* archivo = fopen(ruta, ESCRITURA);

	if(archivo == NULL) {
		return;
	}

	preguntar_valores(configuracion);

	// Escribo el archivo
	escribir_configuracion(archivo, *configuracion);
	

	fclose(archivo);

}

/*
* Pre: El separador debe estar en alguna parte del parámetro.
* Post: Corta el string parametro y obtiene la primera parte de este, es decir, hasta
	el separador. Lo almacena en parte_uno. Si logra cortar el string, devuelve la posición
	del separador. Sino, devuelve ERROR.
*/
int cortar_string_parte_uno(char parametro[], char parte_uno[], char separador) {

	int i = 0;								
	while (i < strlen(parametro) && parametro[i] != separador) {
		parte_uno[i] = parametro[i];
		i++;
	}

	parte_uno[i] = FIN_STRING;

	if (parametro[i] == separador) return i;
	return ERROR;
}

/*
* Pre: pos_separador debe ser la posición del separador dentro del string parametro. 
* Post: Obtiene la segunda parte del string parámetro, es decir, desde pos_separador hasta
	el final del string, y lo almacena en parte_dos.
*/
void cortar_string_parte_dos(char parametro[], char parte_dos[], int pos_separador) {

	int contador = 0;
	for (int i = pos_separador+1; i < strlen(parametro); i++) {
		parte_dos[contador] = parametro[i];
		contador++;
	}

	parte_dos[contador] = FIN_STRING;
}


/*
* Pre: el archivo debe estar abierto en modo escritura.
* Post: Escribe una línea del archivo ranking con las variables recibidas como parámetro
*/
void escribir_ranking(FILE* ranking, char nombre[MAX_NOMBRE], int puntos) {
	fprintf(ranking, "%s;%i\n", nombre, puntos);
}

/*
* Pre: el archivo debe estar abierto en modo lectura.
* Post: Lee una línea del archivo de ranking recibido, y almacena sus valores en
	nombre y puntos.
*/
int leer_ranking(FILE* ranking, char nombre[MAX_NOMBRE], int* puntos) {
	return fscanf(ranking, " %[^;];%i\n", nombre, puntos);
}

/*
* Pre: -
* Post: Obtiene la ruta del ranking, según la ruta del archivo de configuración,
	y la almacena en ruta_ranking con extensión .csv. En caso de que obtenga la ruta de 
	configuración por defecto, utiliza la ruta del ranking por defecto.
*/
void obtener_ruta_ranking(char ruta_config[MAX_RUTA], char ruta_ranking[MAX_RUTA]) {
	if (strcmp(ruta_config, RUTA_POR_DEFECTO) == 0) {
		strcpy(ruta_ranking, RUTA_RANKING_DEFAULT);
	} else {
		char ruta_config_sin_extension[MAX_RUTA];
		char separador = SEPARADOR_EXTENSION;
		int pos_separador = cortar_string_parte_uno(ruta_config, 
								ruta_config_sin_extension, separador);

		if (pos_separador != ERROR) {
			strcpy(ruta_ranking, "ranking_");
			strcat(ruta_ranking, ruta_config_sin_extension);
			strcat(ruta_ranking, EXTENSION_CSV);
		}
	}
}

/*
* Pre: Los parámetros recibidos deben ser válidos.
* Post: Guarda en un nuevo archivo de ranking, con la ruta_ranking recibida, el
	jugador con el nombre_nuevo y los puntos_nuevos recibidos.
*/
void guardar_en_nuevo_ranking(char nombre_nuevo[MAX_NOMBRE], int puntos_nuevos, 
		char ruta_ranking[MAX_RUTA]) {

	FILE* ranking_nuevo = fopen(ruta_ranking, ESCRITURA);
	if (!ranking_nuevo) {
		printf("No se pudo abrir un nuevo archivo de ranking. No se guardará su progreso.\n");
		return;
	}
	escribir_ranking(ranking_nuevo, nombre_nuevo, puntos_nuevos);
	fclose(ranking_nuevo);
}

/*
* Pre: El archivo de ranking_actual debe estar abierto en modo LECTURA, y el de
	ranking_nuevo en modo ESCRITURA.
* Post: Guarda el jugador con nombre_nuevo y puntos_nuevos en el archivo de 
	ranking_actual, de manera ordenada, respetando lo que había en el ranking_actual.
	Si hay algún error, devuelve false, sino, true.
*/
bool guardar_en_viejo_ranking(FILE* ranking_actual, FILE* ranking_nuevo, 
	char nombre_nuevo[MAX_NOMBRE], int puntos_nuevos) {

	char nombre_actual[MAX_NOMBRE];
	int puntos_actuales;
	bool jugador_agregado = false;

	int leido = leer_ranking(ranking_actual, nombre_actual, &puntos_actuales);
	if (leido != LEIDOS_ESPERADOS)
		return jugador_agregado;

	while(leido == LEIDOS_ESPERADOS && !jugador_agregado) {

		if (puntos_nuevos < puntos_actuales) {
				// Escribo el actual
			escribir_ranking(ranking_nuevo, nombre_actual, puntos_actuales);
		} else if (puntos_nuevos > puntos_actuales) {
				// Escribo el nuevo
			escribir_ranking(ranking_nuevo, nombre_nuevo, puntos_nuevos);
			escribir_ranking(ranking_nuevo, nombre_actual, puntos_actuales);
			jugador_agregado = true;
		} else { //Si sus puntos son iguales
			if (strcmp(nombre_nuevo, nombre_actual) < 0) {
				// Escribo el nuevo
				escribir_ranking(ranking_nuevo, nombre_nuevo, puntos_nuevos);
				escribir_ranking(ranking_nuevo, nombre_actual, puntos_actuales);
				jugador_agregado = true;
			} else {
				// Escribo el actual, si su nombre es menor, o si tienen igual nombre.
				escribir_ranking(ranking_nuevo, nombre_actual, puntos_actuales);
			}
		}
		leido = leer_ranking(ranking_actual, nombre_actual, &puntos_actuales);
	}
	// Si leyó todo el ranking, pero el jugador a agregar no fue agreagado aún
	if (!jugador_agregado) {
		// Escribo el nuevo
		escribir_ranking(ranking_nuevo, nombre_nuevo, puntos_nuevos);
		jugador_agregado = true;
	}
	// Si quedaron del actual sin escribir
	while(leido == LEIDOS_ESPERADOS) {
		// Escribo el actual
		escribir_ranking(ranking_nuevo, nombre_actual, puntos_actuales);
	}
	return jugador_agregado;
}


/*
* Pre: Recibe un nombre válido y la cantidad de puntos del jugador en esa partida.
* Post: Escribe el nombre del jugador en el archivo del ranking correspondiente a la 
	ruta de configuración recibida, junto a su cantidad de puntos conseguida en la partida.
*/
void guardar_jugador_en_ranking(char nombre_nuevo[MAX_NOMBRE], int puntos_nuevos, 
	char ruta_config[MAX_RUTA]) {
	
	// Saco la extensión a la ruta del archivo de configuración
	char ruta_ranking[MAX_RUTA];
	obtener_ruta_ranking(ruta_config, ruta_ranking);

	FILE* ranking_actual = fopen(ruta_ranking, LECTURA);

	if(ranking_actual == NULL) {
		printf("No se ha podido abrir el archivo de ranking. Se crea uno nuevo.\n");
		guardar_en_nuevo_ranking(nombre_nuevo, puntos_nuevos, ruta_ranking);	
		return;
	}

	FILE* ranking_nuevo = fopen(RUTA_RANKING_AUX, ESCRITURA);

	if(ranking_nuevo == NULL) {
		printf("No se pudo abrir el archivo del nuevo ranking.\n");
		fclose(ranking_actual);
		return;
	}
	// Intenta guardar, si no puede lo muestra por pantalla y sale, sino, sigue.
	if (!guardar_en_viejo_ranking(ranking_actual, ranking_nuevo, nombre_nuevo, puntos_nuevos)) {
	 	printf("No se pudo leer correctamente el archivo de ranking para esta configuración. Verificar que el archivo tenga el formato correcto.\n");
	} else {
		// Borro el archivo viejo
		remove(ruta_ranking);
		// Renombro el archivo nuevo
		if (rename(RUTA_RANKING_AUX, ruta_ranking) != 0) {
			printf("No se pudo renombrar el archivo de ranking.\n");
		}
		printf("Se ha guardado su puntaje en el ranking de esta configuración.\n");
	}

	fclose(ranking_nuevo); // Cierro solo un archivo porque el otro lo borro dentro de la función guardar_en_viejo_ranking
}


// FUNCIÓN PÚBLICA
void mostrar_ranking(int cantidad_jugadores, char ruta_config[MAX_RUTA]) {

	char ruta_ranking[MAX_RUTA];
	obtener_ruta_ranking(ruta_config, ruta_ranking);

	if(strcmp(ruta_config, RUTA_POR_DEFECTO) == 0) {
		strcpy(ruta_ranking, RUTA_RANKING_DEFAULT);
	}

	FILE* ranking = fopen(ruta_ranking, LECTURA);

	if (ranking == NULL) {
		printf("El archivo de ranking %s no existe o no pudo abrirse.\n", ruta_ranking);
		return;
	}

	char nombre[MAX_NOMBRE];
	int puntos;
	if (strcmp(ruta_config, RUTA_POR_DEFECTO) == 0)
		printf("\t\t\t\tRANKING POR DEFECTO\n\n");
	else
		printf("\t\t\t\tRANKING %s\n\n", ruta_config);

	int num_jugador = 0;
	if(cantidad_jugadores == VALOR_POR_DEFECTO) {
		int leido = leer_ranking(ranking, nombre, &puntos);
		while(leido != EOF) {
			printf("\t\t%i\t%s  %i\n\n", num_jugador, nombre, puntos);
			leido = leer_ranking(ranking, nombre, &puntos);
			num_jugador++;
		}
		fclose(ranking);
		return;
	}

	int leido = leer_ranking(ranking, nombre, &puntos);

	while(leido != EOF && num_jugador < cantidad_jugadores) {
		printf("\t\t%i\t%s  %i\n\n", num_jugador, nombre, puntos);
		leido = leer_ranking(ranking, nombre, &puntos);
		num_jugador++;
	}

	fclose(ranking);
}

/*
* Pre: archivo debe estar abierto, y pos_actual debe ser válida.
* Post: Escribe la posición actual del usuario en el terreno, en el archivo recibido.
*/
void escribir_pos_actual(FILE* archivo, coordenada_t pos_actual) {
	fprintf(archivo, "%i;%i\n", pos_actual.fil, pos_actual.col);
}

/*
* Pre: El archivo debe estar abierto y num_camino ser 1 o 2.
* Post: Esribe el número de nivel camino en el archivo recibido.
*/
void escribir_num_camino(FILE* archivo, int num_camino) {
	fprintf(archivo, "CAMINO=%i\n", num_camino);
}

/*
* Pre: El archivo debe estar abierto y num_nivel entre ESTE Y SUR.
* Post: Esribe el número de nivel actual en el archivo recibido.
*/
void escribir_num_nivel(FILE* archivo, int num_nivel) {
	fprintf(archivo, "NIVEL=%i\n", num_nivel);

}

/*
* Pre: tope_terreno debe ser válido.
* Post: Devuelve true, si la fila y columna recibidas constituyen una posición válida, 
	para el tope del terreno recibido. Sino, devuelve false.
*/
bool es_posicion_valida(int fil, int col, int tope_terreno) {
	return (fil >= PRIMERA_FIL_TERRENO && fil < tope_terreno && 
			col >= PRIMERA_COL_TERRENO && col < tope_terreno);
}

/*
* Pre: pos_actual debe ser válida según el tope del terreno. tope_terreno debe ser válido.
* Post: Pide un movimiento al usuario, que debe ingresar hacia donde quiere moverse con las
	teclas w (arriba), a (izquierda), s (abajo), d (derecha).
*/
void pedir_movimiento(coordenada_t* pos_actual, int tope_terreno) {
	char movimiento;
	do {
		scanf(" %c", &movimiento);
		if ( movimiento == ARRIBA && es_posicion_valida( ((*pos_actual).fil)-1, (*pos_actual).col, tope_terreno) ) {
			(*pos_actual).fil--; 
		} else if ( movimiento == ABAJO && es_posicion_valida( ((*pos_actual).fil)+1, (*pos_actual).col, tope_terreno) ) {
			(*pos_actual).fil++;
		} else if ( movimiento == IZQUIERDA && es_posicion_valida((*pos_actual).fil, ((*pos_actual).col)-1, tope_terreno) ) {
			(*pos_actual).col--;
		} else if ( movimiento == DERECHA && es_posicion_valida((*pos_actual).fil, ((*pos_actual).col)+1, tope_terreno) ) {
			(*pos_actual).col++;
		} else {
			printf("Ese movimiento no es válido!\n");
		}
	} while(movimiento != ARRIBA && movimiento != ABAJO && movimiento != IZQUIERDA && movimiento != DERECHA);
}

/*
* Pre: Tope terreno debe ser menor o igual a MAX_TERRENO
* Post: Vacía el terreno recibido según el tope.
*/
void vaciar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int tope_terreno) {
	for (int i = 0; i < tope_terreno; i++) {
		for (int j = 0; j < tope_terreno; j++) {
			terreno[i][j] = VACIO;
		}
	}
}

/*
* Pre: torre debe ser una coordenada válida. tipo es el tipo de torre a asignar al terreno.
	Este debe ser TORRE_1 o TORRE_2.
* Post: Asigna al terreno la torre del tipo recibido, en la coordenada recibida (torre).
*/
void determinar_pos_torre(char terreno[MAX_TERRENO][MAX_TERRENO], coordenada_t torre, char tipo) {
	if (tipo == TORRE_1)
		terreno[torre.fil][torre.col] = TORRE_1;
	else
		terreno[torre.fil][torre.col] = TORRE_2;
}

/*
* Pre: nivel debe ser válido. tope_terreno debe ser <= MAX_TERRENO.
* Post: Muestra el camino por pantalla al usuario.
*/
void mostrar_camino(int nivel, char terreno[MAX_TERRENO][MAX_TERRENO], int tope_terreno) {

	//system("clear");

	printf("\n┌");
	for (int i = 0; i <= tope_terreno; i++) {
		printf("───");
	}
	printf("──┐\n");

	if (nivel < NORTE) {
		printf("│\t\t\t\t\t\t   │");
	} else {
		printf("│\t\t\t\t\t\t\t\t  │");
	}

	// Números de columna
	printf("\n│    ");
	for (int i = 0; i < tope_terreno; i++) {
		printf("%.2i|", i);
	}
	printf(" │");
	printf("\n");


	mostrar_terreno(terreno, tope_terreno);


	if (nivel < NORTE) {
		printf("│\t\t\t\t\t\t   │\n");
	} else {
		printf("│\t\t\t\t\t\t\t\t  │\n");

	}

	printf("└");
	for (int i = 0; i <= tope_terreno; i++) {
		printf("───");
	}
	printf("──┘");
	printf("\n");
}

/*
* Pre: Las coordenadas deben ser válidas.
* Post: Asigna la posición actual a la matriz terreno. Si la coordeanda actual es la entrada, 
	asigna una entrada al terreno.	Sino, asigna CAMINO al terreno.
*/
void asignar_pos_actual(char terreno[MAX_TERRENO][MAX_TERRENO], coordenada_t entrada, coordenada_t pos) {
	// Si es la entrada, muestro la entrada, sino, camino.
	if (pos.fil == entrada.fil && pos.col == entrada.col)
		terreno[pos.fil][pos.col] = ENTRADA;
	else
		terreno[pos.fil][pos.col] = CAMINO;
}

/*
* Pre: archivo debe ser válido y estar abierto, las coordenadas recibidas deben ser válidas.
	nivel debe ser >= ESTE y <= SUR, y camino debe ser 1 o 2. tope_terreno debe ser <= MAX_TERRENO.
* Post: Pide el camino al usuario y lo escribe en el archivo recibido.
*/
void pedir_camino(FILE* archivo, coordenada_t entrada, coordenada_t torre, int nivel, int camino,
	char terreno[MAX_TERRENO][MAX_TERRENO], int tope_terreno) {
	
	coordenada_t pos_actual;
	// Asigno la posición actual a la entrada
	pos_actual.fil = entrada.fil;
	pos_actual.col = entrada.col;
	// Escribo qué camino es
	escribir_num_camino(archivo, camino);
	// Escribo la posición de la entrada
	escribir_pos_actual(archivo, entrada);
	do {
		asignar_pos_actual(terreno, entrada, pos_actual);
		mostrar_camino(nivel, terreno, tope_terreno);
		pedir_movimiento(&pos_actual, tope_terreno);
		escribir_pos_actual(archivo, pos_actual);
	} while (pos_actual.fil != torre.fil || pos_actual.col != torre.col);
}


// FUNCIÓN PÚBLICA
int crear_camino(char ruta[MAX_RUTA]) {

	FILE* archivo_caminos = fopen(ruta, ESCRITURA);

	if(!archivo_caminos) {
		printf("Error al abrir el archivo de caminos.\n");
		return ERROR;
	}

	char terreno[MAX_TERRENO][MAX_TERRENO];
	int num_camino;
	int tope_terreno;
	coordenada_t entrada_1, entrada_2, torre_1, torre_2;

	// Llamo a una función que muestre el camino
	for (int i = 1; i <= MAX_NIVELES; i++) {
		system("clear");
		printf("\n\n\tCamino(s) del nivel %i\n\n", i);
		printf("\tUsted se encuentra en la entrada" VERDE_OSCURO " %s" FIN "\n", PRINT_ENTRADA);
		printf("\tPara moverse, usar w - a - s - d\n");

		tope_terreno = tope_terreno_segun_nivel(i);
		// Vacío el terreno a mostrar
		vaciar_terreno(terreno, tope_terreno);

		// Obtengo las coordenadas de torres y entradas del nivel actual
		obtener_coord_entrada(i, &entrada_1, &entrada_2, tope_terreno);
		obtener_coord_torre(i, &torre_1, &torre_2, tope_terreno);

		// Escribo el nivel en el archivo
		escribir_num_nivel(archivo_caminos, i);

		num_camino = NUM_CAMINO_1;
		if(i < NORTE) {
			if (i == ESTE)
				determinar_pos_torre(terreno, torre_1, TORRE_1);
			else
				determinar_pos_torre(terreno, torre_1, TORRE_2); // Visualmente es la torre 2

			pedir_camino(archivo_caminos, entrada_1, torre_1, i, num_camino, terreno, tope_terreno);

		} else {
			determinar_pos_torre(terreno, torre_1, TORRE_1);
			determinar_pos_torre(terreno, torre_2, TORRE_2);
			pedir_camino(archivo_caminos, entrada_1, torre_1, i, num_camino, terreno, tope_terreno);
			num_camino++;
			pedir_camino(archivo_caminos, entrada_2, torre_2, i, num_camino, terreno, tope_terreno);
		}

	}

	fclose(archivo_caminos);

	return EXITO;
}

// FUNCIÓN PÚBLICA
void poneme_la_repe(char ruta[MAX_RUTA], float velocidad) {
	
	juego_t juego;
	FILE* archivo = fopen(ruta, LECTURA);

	if (archivo == NULL) {
		printf("Fue imposible abrir el archivo de la repe!\n");
		return;
	}

	fread(&juego, sizeof(juego_t), 1, archivo);
	while(!feof(archivo)) {
		fread(&juego, sizeof(juego_t), 1, archivo);
		mostrar_juego(juego);
		detener_el_tiempo(velocidad);
	}
	fclose(archivo);
}