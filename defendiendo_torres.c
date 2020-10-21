#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "defendiendo_torres.h"


#define MAX_LONGITUD_CAMINO 200
#define MAX_ENEMIGOS 500

#define MAX_DEFENSORES 50

const int VIDA_INICIAL_TORRE = 600;
static const int VIDA_NULA = 0;

static const int DEFENSORES_EXTRA = 10;

const int CRITICO_BUENO = 25;
const int CRITICO_REG = 10;
const int SIN_CRITICO = 0;

static const char ANIMO_BUENO = 'B';
static const char ANIMO_REGULAR = 'R';
//static const char ANIMO_MALO = 'M';

#define JUGANDO 0
#define PERDIDO -1
#define GANADO 1

static const char ENANO = 'G';
static const char ELFO = 'L';
static const char DEFENSOR = 'D';
static const char ORCO = 'O';

static const int AGREGADO = 0;
static const int NO_AGREGADO = -1;

const int FUERZA_ENANO = 60;
const int DANIO_CRITICO = 40;
const int DISTANCIA_ENANO = 1;

const int FUERZA_ELFO = 30;
const int DISTANCIA_ELFO = 3;

const int NO_HAY_ORCO = -1;
const int VIDA_INICIAL_ORCO = 200;
const int SIN_VIDA = 0;

const int POS_INICIAL_CAMINO = 0;

const int RANGO_ALEATORIO = 100;

const int SIN_ENEMIGOS = 0;


const bool HAY_DEFENSOR = true;
const bool NO_HAY_DEFENSOR = false;


// FUNCIÓN PÚBLICA
void obtener_coord_entrada(int nivel, coordenada_t* entrada_1, coordenada_t* entrada_2, int tope) {
	switch(nivel) {
		case ESTE:	// tope-1
			(*entrada_1).fil = (rand() % TOPE_TERRENO_CHICO);
			(*entrada_1).col = (tope-1);
			break;

		case OESTE:
			(*entrada_1).fil = (rand() % TOPE_TERRENO_CHICO);
			(*entrada_1).col = PRIMERA_COL_TERRENO;
			break;

		case NORTE:
			(*entrada_1).fil = PRIMERA_FIL_TERRENO;
			(*entrada_1).col = (rand() % (tope/2));

			(*entrada_2).fil = PRIMERA_FIL_TERRENO;
			(*entrada_2).col = (rand() % (tope/2)) + (tope/2);
			break;

		case SUR:
			(*entrada_1).fil = (tope-1);
			(*entrada_1).col = (rand() % (tope/2));

			(*entrada_2).fil = (tope-1);
			(*entrada_2).col = (rand() % (tope/2)) + (tope/2);
			break;
	}
}

// FUNCIÓN PÚBLICA
void obtener_coord_torre(int nivel, coordenada_t* torre_1, coordenada_t* torre_2, int tope) {
	switch(nivel) {
		case ESTE:
			(*torre_1).fil = (rand() % TOPE_TERRENO_CHICO);
			(*torre_1).col = PRIMERA_COL_TERRENO;
			break;

		case OESTE:
			(*torre_1).fil = (rand() % TOPE_TERRENO_CHICO);
			(*torre_1).col = (tope-1);
			break;

		case NORTE:
			(*torre_1).fil = (tope-1);
			(*torre_1).col = (rand() % (tope/2));

			(*torre_2).fil = (tope-1);
			(*torre_2).col = (rand() % (tope/2)) + (tope/2);
			break;

		case SUR:
			(*torre_1).fil = PRIMERA_FIL_TERRENO;
			(*torre_1).col = (rand() % (tope/2));

			(*torre_2).fil = PRIMERA_FIL_TERRENO;
			(*torre_2).col = (rand() % (tope/2)) + (tope/2);
			break;
	}
}



/*
* Pre: El enemigo debe ser válido.
* Post: Devuelve true si el enemigo está vivo (vida > 0), o false si no lo está.
*/
bool enemigo_esta_vivo(enemigo_t enemigo) {
	return (enemigo.vida > SIN_VIDA);
}

/*
* Pre: tipo debe ser DEFENSOR, si el tipo de defensor es irrelevante. Sino, debe
	ser ELFO o ENANO. tope debe ser >= 0 y <= MAX_DEFENSORES.
* Post: Se fija si hay un defensor en la posición (fil, col) recibida. Si tipo es
	DEFENSOR, se fija si hay un defensor, sin importar el tipo. Sino, se fija si
	hay un defensor del tipo recibido. Si hay defensor, devuelve true, sino, false.
*/
bool hay_defensor(defensor_t defensores[MAX_DEFENSORES], int fil, int col, int tope, char tipo) {
	int i = 0;
	bool hay = NO_HAY_DEFENSOR;
	// Si busco si hay un defensor sin importar el tipo
	if (tipo == DEFENSOR) {
		while( ((!hay) && (i < tope)) ) {
			if ( (defensores[i].posicion.fil == fil) && (defensores[i].posicion.col == col) ) 
				hay = HAY_DEFENSOR;
			i++;
		}
	// Si busco si hay un defensor de ese tipo
	} else {
		while( ((!hay) && (i < tope)) ) {
			if ( (defensores[i].posicion.fil == fil) && (defensores[i].posicion.col == col) 
				&& (defensores[i].tipo == tipo) )
				hay = HAY_DEFENSOR;
			i++;
		}
	}

	return hay;
}


/*
* Pre: El camino debe ser válido. El tope debe ser >= 0 y <= MAX_LONGITUD_CAMINO.
* Post: Si la posición es parte del camino recibido, devuelve true. Sino, false.
*/
bool es_camino(coordenada_t camino[MAX_LONGITUD_CAMINO], coordenada_t posicion, int tope) {
	int i = 0;
	bool hay_camino = false;
	while( ((!hay_camino) && (i < tope)) ) {
		if ( (camino[i].fil == posicion.fil) && (camino[i].col == posicion.col) )
			hay_camino = true;
		i++;
	}
	return hay_camino;
}

/*
* Pre: -
* Post: Devuelve el tope del terreno dependiendo del nivel actual. Si el nivel
	es el ESTE u OESTE, devuelve el tope chico, sino, el grande.
*/
int tope_terreno_segun_nivel(int nivel) {
	if ( (nivel== ESTE) || (nivel == OESTE) )
		return TOPE_TERRENO_CHICO;
	else 
		return TOPE_TERRENO_GRANDE;
}



/*
* Pre: clima debe ser válido: 75, 50 o 25. Puede ser humedad o viento.
* Post: Devuelve la mitad del valor del clima, que será el porcentaje
	de fallo del ataque.
*/
int fallo_segun_clima(int clima) {
	return (clima/2);
}

/*
* Pre: animo debe ser válido: Bueno (B), Regular (R) o Malo (M).
* Post: Devuelve CRITICO_BUENO si animo es bueno, CRITICO_REG si animo
	es regular, o SIN_CRITICO si animo es malo.
*/
int critico_segun_animo(char animo) {
	if(animo == ANIMO_BUENO) return CRITICO_BUENO;
	else if (animo == ANIMO_REGULAR) return CRITICO_REG;
	return SIN_CRITICO; 
}


// FUNCIÓN PÚBLICA
void inicializar_juego(juego_t* juego, int viento, int humedad,
	char animo_legolas, char animo_gimli, configuracion_t configuracion) {
	
	// RESISTENCIA TORRES

	(*juego).torres.resistencia_torre_1 = configuracion.resistencia_torre_1;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.resistencia_torre_1 == VALOR_POR_DEFECTO)
		(*juego).torres.resistencia_torre_1 = VIDA_INICIAL_TORRE;


	(*juego).torres.resistencia_torre_2 = configuracion.resistencia_torre_2;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.resistencia_torre_2 == VALOR_POR_DEFECTO) {
		(*juego).torres.resistencia_torre_2 = VIDA_INICIAL_TORRE;
	}

	// DEFENSORES EXTRA

	(*juego).torres.enanos_extra = configuracion.enanos.cant_extra;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.enanos.cant_extra == VALOR_POR_DEFECTO)
		(*juego).torres.enanos_extra = DEFENSORES_EXTRA;

	(*juego).torres.elfos_extra = configuracion.elfos.cant_extra;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.elfos.cant_extra == VALOR_POR_DEFECTO)
		(*juego).torres.elfos_extra = DEFENSORES_EXTRA;

	// FALLO 

	(*juego).fallo_gimli = configuracion.enanos.fallo;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.enanos.fallo == VALOR_POR_DEFECTO)
		(*juego).fallo_gimli = fallo_segun_clima(humedad);
	
	(*juego).fallo_legolas = configuracion.elfos.fallo;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.elfos.fallo == VALOR_POR_DEFECTO)
		(*juego).fallo_legolas = fallo_segun_clima(viento);
	
	// CRÍTICO

	(*juego).critico_gimli = configuracion.enanos.critico;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.enanos.critico == VALOR_POR_DEFECTO)
		(*juego).critico_gimli = critico_segun_animo(animo_gimli);

	(*juego).critico_legolas = configuracion.elfos.critico;
	// Si se usa el valor por defecto, lo asigno
	if (configuracion.elfos.critico == VALOR_POR_DEFECTO)
		(*juego).critico_legolas = critico_segun_animo(animo_legolas);

	// VACÍO LOS CAMINOS (TOPE EN 0)
	(*juego).nivel.tope_camino_1 = POS_INICIAL_CAMINO;
	(*juego).nivel.tope_camino_2 = POS_INICIAL_CAMINO;

	//printf("\n> DEFENDIENDO LAS TORRES - PARTE 2 <\n");
}

/*
* Pre: tope debe ser menor o igual a MAX_ENEMIGOS. enemigos debe ser válido. 
* Post: Devuelve true si hay al menos un enemigo vivo en el vector
 	enemigos, o false si no hay.
*/
bool hay_orcos_vivos(enemigo_t enemigos[MAX_ENEMIGOS], int tope) {
	int i = 0;
	bool hay_enemigos = false;
	while (!(hay_enemigos) && i < tope) {
		if (enemigo_esta_vivo(enemigos[i])) hay_enemigos = true;
		i++;
	}
	return hay_enemigos;
}

// FUNCIÓN PÚBLICA
int estado_nivel(nivel_t nivel) {
	if (nivel.tope_enemigos == SIN_ENEMIGOS || (hay_orcos_vivos(nivel.enemigos, nivel.tope_enemigos) ||
		(nivel.tope_enemigos < nivel.max_enemigos_nivel))) {
		return JUGANDO;
	} else
		return GANADO;
}

/*
* Pre: Recibe un juego con todas sus estructuras válidas.
* Post: Devuelve true si la resistencia de alguna de las torres es nula.
	Sino, devuelve false.
*/
bool juego_perdido(juego_t juego) {
	return ( (juego.torres.resistencia_torre_1 <= VIDA_NULA) ||
			 (juego.torres.resistencia_torre_2 <= VIDA_NULA) );
}

/*
* Pre: Recibe un juego con todas sus estructuras válidas.
* Post: Devuelve true si el nivel actual es el último (SUR), y su estado es GANADO,
	es decir, si se ganó el juego. Sino, devuelve false.
*/
bool juego_ganado(juego_t juego) {
	return ( (juego.nivel_actual == SUR) && (estado_nivel(juego.nivel) == GANADO) );
}

// FUNCIÓN PÚBLICA
int estado_juego(juego_t juego) {
	if (juego_perdido(juego)) return PERDIDO;
	else if (juego_ganado(juego)) return GANADO;
	else return JUGANDO;
}


/*
* Pre: recibe un nivel con todas sus estructuras válidas, y una posición
	de un defensor, que debe ser válida para el terreno.
* Post: Devuelve true si en la posición recibida no hay ningún camino
	ni ningún defensor. Sino, devuelve false.
*/
bool posicion_defensor_valida(nivel_t nivel, coordenada_t posicion) {
	if ( (es_camino(nivel.camino_1, posicion, nivel.tope_camino_1)) ||
		 (es_camino(nivel.camino_2, posicion, nivel.tope_camino_2)) ||
	  	 (hay_defensor(nivel.defensores, posicion.fil, posicion.col, nivel.tope_defensores, DEFENSOR))) {
		return false;
	}	
	return true;
}

// FUNCIÓN PÚBLICA
int agregar_defensor(nivel_t* nivel, coordenada_t posicion, char tipo) {
	/* Buscar si la posición elegida interseca con algún camino u otro defensor.
	 	Si es así, devuelve que no lo pudo agregar. */
	if (!posicion_defensor_valida(*nivel, posicion)) {
		return NO_AGREGADO;
	}
	// Agrega un defensor y devuelve que lo agregó.
	(*nivel).defensores[(*nivel).tope_defensores].posicion.fil = posicion.fil;
	(*nivel).defensores[(*nivel).tope_defensores].posicion.col = posicion.col;
	(*nivel).defensores[(*nivel).tope_defensores].tipo = tipo;

	if (tipo == ENANO)
		(*nivel).defensores[(*nivel).tope_defensores].fuerza_ataque = FUERZA_ENANO;
	else 
		(*nivel).defensores[(*nivel).tope_defensores].fuerza_ataque = FUERZA_ELFO;
	// Aumento el tope de defensores para saber que tengo uno más que antes.
	((*nivel).tope_defensores)++;
	return AGREGADO;
}

/*
* Pre: critico debe ser válido (menor a 100 y mayor a cero).
* Post:	Si es critico devuelve true. Esto es, si critico es menor o igual
	a un número aleatorio entre 0 y 100. Sino, devuelve false.
*/
bool es_critico(int critico) {
	return ((rand() % RANGO_ALEATORIO) < critico);
}


/*
* Pre: fallo debe ser válido (menor a 100 y mayo a cero).
* Post:	Si es fallo devuelve true. Esto es, si fallo es menor o igual
	a un número aleatorio entre 0 y 100. Sino, devuelve false.
*/
bool es_fallo(int fallo) {
	return ((rand() % RANGO_ALEATORIO) <= fallo);
}

/*
* Pre: pos debe ser válida para el vector enemigos. El camino debe ser válido.
* Post: Devuelve true si la posicíón (pos) del vector de enemigos enviada, corresponde a un enemigo
	en la fila y columna recibidas, del camino recibido como parámetro.
*/
bool enemigo_esta_en_camino(coordenada_t camino[MAX_LONGITUD_CAMINO], int pos,
						 enemigo_t enemigos[MAX_ENEMIGOS], int fil, int col) {
	
	return( ((camino[enemigos[pos].pos_en_camino].fil == fil) &&
			 (camino[enemigos[pos].pos_en_camino].col == col)) );
}

/*
* Pre: La posición debe ser válida para alguno de los caminos.
* Post: Devuelve true si hay algún enemigo vivo en la posición (fil, col) del camino recibido.
	Si no lo hay, devuelve false.
*/
bool hay_enemigo_en_camino(coordenada_t camino[MAX_LONGITUD_CAMINO], int fil, int col,
								 enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos) {

	bool hay_enemigo = false;
	for (int i = 0; i < tope_enemigos; i++) {
		if(enemigo_esta_en_camino(camino, i, enemigos, fil, col) && enemigo_esta_vivo(enemigos[i])) 
			hay_enemigo = true;
	}
	return hay_enemigo;
}


/*
* Pre: Las posiciones deben ser válidas.
* Post: Devuelve true si la distancia entre posicion_1 y posicion_2, es decir, entre el enano y
el enemigo, está en el rango de ataque del enano. Devuelve false si no lo está.
*/
bool esta_en_rango_enano(coordenada_t posicion_1, coordenada_t posicion_2) {
	return ( (abs(posicion_1.fil - posicion_2.fil) <= DISTANCIA_ENANO) &&
			(abs(posicion_1.col - posicion_2.col) <= DISTANCIA_ENANO) );
}


/*
* Pre: defensor y enemigos deben ser válidos, con sus estructuras válidas. tope_enemigos debe ser 
	mayor a cero. El fallo y crítico deben ser mayores a 0 y menores o iguales a 100.
* Post: Realiza el ataque del enano, que puede ser crítico o no, a uno de los orcos que tiene
	alrededor, en una distancia de 1 (DISTANCIA_ENANO). Si no hay orco, no ataca.
*/
void ataque_enano(defensor_t defensor, enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos, 
	coordenada_t camino_1[MAX_LONGITUD_CAMINO], coordenada_t camino_2[MAX_LONGITUD_CAMINO], 
	int critico_gimli, int fallo_gimli) {

	bool esta_en_rango = false;
	bool hay_orco_a_atacar = false;
	int i = 0;
	while(!hay_orco_a_atacar && i < tope_enemigos) {
		if (enemigos[i].camino == NUM_CAMINO_1)
			esta_en_rango = esta_en_rango_enano(defensor.posicion, camino_1[enemigos[i].pos_en_camino]);			
		else
			esta_en_rango = esta_en_rango_enano(defensor.posicion, camino_2[enemigos[i].pos_en_camino]);

		if ( esta_en_rango && enemigo_esta_vivo(enemigos[i]) && (!es_fallo(fallo_gimli)) ) {
			
			hay_orco_a_atacar = true;
			if (es_critico(critico_gimli))
				enemigos[i].vida -= DANIO_CRITICO;
			
			enemigos[i].vida -= defensor.fuerza_ataque;
		}
		i++;
	}
}


/*
* Pre: Las posiciones y la distancia deben ser válidas (>= 0).
* Post: Devuelve true si la distancia entre las 2 posiciones es menor o igual a la distancia
	recibida, usando distancia manhattan. Devuelve false si no es distancia manhattan.
*/
bool es_distancia_manhattan(coordenada_t posicion_1, coordenada_t posicion_2, int distancia) {
	return ( ( abs(posicion_1.fil - posicion_2.fil) + abs(posicion_1.col- posicion_2.col)) <= distancia );
}

/*
* Pre: enemigo debe ser válido con su pos_en_camino válida, y camino válido. Los
	caminos deben ser válidos.
* Post: Devuelve en "coordenada_enemigo" la coordenada en el camino 
	correspondiente del enemigo recibido. 
*/
void obtener_coordenada_enemigo(enemigo_t enemigo, coordenada_t camino_1[MAX_LONGITUD_CAMINO],
 coordenada_t camino_2[MAX_LONGITUD_CAMINO], coordenada_t* coordenada_enemigo) {
	if (enemigo.camino == NUM_CAMINO_1) 
		(*coordenada_enemigo) = camino_1[enemigo.pos_en_camino];
	else
		(*coordenada_enemigo) = camino_2[enemigo.pos_en_camino];
	
}



/*
* Pre: El defensor debe ser válido, y de tipo elfo. tope_enemigos debe ser menor
	o igual a MAX_ENEMIGOS. Ambos caminos deben ser válidos. critico_legolas y
	fallo_legolas deben ser ambos >= 0 y <= 100.
* Post: Realiza el ataque del elfo, restándole la fuerza_ataque del elfo 
	a todos los enemigos que estén vivos en	distancia manhattan 3. Tiene en cuenta si
	se falla o no cada ataque,	según fallo_legolas, y si se suma el daño crítico,
 	según critico_legolas.
*/
void ataque_elfo(defensor_t defensor, enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos, 
	coordenada_t camino_1[MAX_LONGITUD_CAMINO], coordenada_t camino_2[MAX_LONGITUD_CAMINO], 
	int critico_legolas, int fallo_legolas) {

	coordenada_t coordenada_enemigo;
	for (int i = 0; i < tope_enemigos; i++) {
		if (!es_fallo(fallo_legolas)) {
			obtener_coordenada_enemigo(enemigos[i], camino_1, camino_2, &coordenada_enemigo);
			// Si el enemigo actual está en distancia manhattan, y está vivo
			if (es_distancia_manhattan(defensor.posicion, coordenada_enemigo, DISTANCIA_ELFO) && 
				enemigo_esta_vivo(enemigos[i])) {

				// Si es crítico
				if(es_critico(critico_legolas))
					enemigos[i].vida -= DANIO_CRITICO;
				
				enemigos[i].vida -= defensor.fuerza_ataque;
			}
		}
	}
}

/*
* Pre: tope_enemigos debe ser >= 0. El camino recibido debe ser válido.
* Post: Agrega un orco, inicializándo su camino, pos_en_camino y vida, en el
	camino recibido como parámetro y aumenta el tope_enemigos en uno.
*/
void agregar_orco(enemigo_t enemigos[MAX_ENEMIGOS], int* tope_enemigos, int camino) {

	enemigos[(*tope_enemigos)].camino = camino;
	enemigos[(*tope_enemigos)].pos_en_camino = POS_INICIAL_CAMINO;
	enemigos[(*tope_enemigos)].vida = (VIDA_INICIAL_ORCO + (rand() % 100));

	(*tope_enemigos)++;
}

/*
* Pre: tope_enemigos debe ser >= 0 y <= MAX_ENEMIGOS. Los enemigos en el vector
	deben estar inicializados.
* Post: Recorre el vector de enemigos y los avanza una posición si están vivos.
	Sino, no los avanza.
*/
void avanzar_enemigos(enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos) {

	for (int i = 0; i < tope_enemigos; i++) {
		if (enemigo_esta_vivo(enemigos[i]))
		enemigos[i].pos_en_camino++;	
		//if ((*nivel).enemigos[i].camino == CAMINO_2)
		//	(*nivel).enemigos[i].pos_en_camino++;
	}	
}

/*
* Pre: tope_enemigos debe ser >= 0 y <= MAX_ENEMIGOS. Los enemigos del vector
	enemigos deben estar inicializados. camino debe ser el número del camino
	del que quiero saber la posición del orco en la torre.
* Post: Devuelve la posición en el vector de enemigos del enemigo que está en 
 	la última posición del camino recibido. 
*/
int posicion_orco_en_torre(enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos, int tope_camino,
 int camino) {
	
	int posicion = NO_HAY_ORCO;
	int i = 0;

	while(posicion == NO_HAY_ORCO && i < tope_enemigos) {
		if ( (enemigos[i].pos_en_camino == (tope_camino-1)) && (enemigos[i].camino == camino) && 
			enemigo_esta_vivo(enemigos[i])) {
			posicion = i;
		}
		i++;
	}

	return posicion;
}

/*
* Pre: vida_orco_en_torre y resistencia_torre deben ser mayores a 0.
* Post: Le resta la vida del orco que llegó a la torre, a la resistencia de la
	torre recibida, y después deja al orco sin vida.
*/
void atacar_torre(int* vida_orco_en_torre, int* resistencia_torre) {

	(*resistencia_torre) -= (*vida_orco_en_torre);

	(*vida_orco_en_torre) = SIN_VIDA;
}

// FUNCIÓN PÚBLICA
void jugar_turno(juego_t* juego) {
	
	int i = 0;
	// ENANOS
	while ( (i < (*juego).nivel.tope_defensores) && ((*juego).nivel.defensores[i].tipo == ENANO) ) {
			
		ataque_enano((*juego).nivel.defensores[i], (*juego).nivel.enemigos, 
			(*juego).nivel.tope_enemigos, (*juego).nivel.camino_1, (*juego).nivel.camino_2, 
			(*juego).critico_gimli, (*juego).fallo_gimli);
		i++;
	}

	// Como primero van los enanos, el iterador i queda en la posición del primer elfo.
	// ELFOS
	while (i < (*juego).nivel.tope_defensores) {

		ataque_elfo((*juego).nivel.defensores[i], (*juego).nivel.enemigos, 
			(*juego).nivel.tope_enemigos, (*juego).nivel.camino_1, (*juego).nivel.camino_2, 
			(*juego).critico_legolas, (*juego).fallo_legolas);
		i++;
	}

	// ORCOS
	avanzar_enemigos((*juego).nivel.enemigos, (*juego).nivel.tope_enemigos);

	
	// Posición del orco en el camino 1, si llegó a la torre
	int pos_orco_torre = posicion_orco_en_torre((*juego).nivel.enemigos,
	 	(*juego).nivel.tope_enemigos, (*juego).nivel.tope_camino_1, NUM_CAMINO_1);

	// Si hay un orco en la torre, restarle la vida de ese orco a la torre
	if (pos_orco_torre != NO_HAY_ORCO)  {
		if ((*juego).nivel_actual == OESTE) // Torre 2
			atacar_torre(&((*juego).nivel.enemigos[pos_orco_torre].vida), 
				&((*juego).torres.resistencia_torre_2));
		else // Torre 1
			atacar_torre(&((*juego).nivel.enemigos[pos_orco_torre].vida), 
				&((*juego).torres.resistencia_torre_1));
	}

	// Posición del orco en el camino 2, si llegó a la torre
	pos_orco_torre = posicion_orco_en_torre((*juego).nivel.enemigos,
	 	(*juego).nivel.tope_enemigos, (*juego).nivel.tope_camino_2, NUM_CAMINO_2);

	// Si hay un orco en la torre 2, restarle la vida de ese orco a la torre
	if (pos_orco_torre != NO_HAY_ORCO)  {
		atacar_torre(&((*juego).nivel.enemigos[pos_orco_torre].vida), 
			&((*juego).torres.resistencia_torre_2));
	}

	// Si todavía no se agregaron todos los orcos de ese nivel, agregar un orco
	if ( (*juego).nivel.tope_enemigos < ((*juego).nivel.max_enemigos_nivel) ) {
		agregar_orco((*juego).nivel.enemigos, &((*juego).nivel.tope_enemigos), NUM_CAMINO_1);
		// Si el nivel es Norte o Sur, agregar un orco al camino 2
		if ( (*juego).nivel_actual >= NORTE ) {
			agregar_orco((*juego).nivel.enemigos, &((*juego).nivel.tope_enemigos), NUM_CAMINO_2);

		}
	}


}

/*
* Pre: camino debe ser válido, y su tope también. El camino debe estar dentro del terreno. 
	terreno debe ser válido.
* Post: Asigna la torre en la última posición del camino.
*/
void asignar_torre(char terreno[MAX_TERRENO][MAX_TERRENO], coordenada_t camino[MAX_LONGITUD_CAMINO],
 int tope, int num_camino) {
	if (num_camino == NUM_CAMINO_1)
		terreno[camino[tope-1].fil][camino[tope-1].col] = TORRE_1;
	else
		terreno[camino[tope-1].fil][camino[tope-1].col] = TORRE_2;

}

/*
* Pre: camino debe ser válido. El camino debe estar dentro del terreno. El terreno debe ser válido.
* Post: Asigna la entrada en la primera posición del camino.
*/
void asignar_entrada(char terreno[MAX_TERRENO][MAX_TERRENO], coordenada_t camino[MAX_LONGITUD_CAMINO]) {
	terreno[camino[POS_INICIAL_CAMINO].fil][camino[POS_INICIAL_CAMINO].col] = ENTRADA;
}


/*
* Pre: tope debe ser menor o igual a MAX_DEFENSORES. El terreno debe ser válido.
* Post: Asigna los defensores del vector de defensores al terreno, según su tipo.
*/
void asignar_defensores(char terreno[MAX_TERRENO][MAX_TERRENO], defensor_t defensores[MAX_DEFENSORES],
	int tope) {

	for (int i = 0; i < tope; i++) {
		if (defensores[i].tipo == ENANO)
			terreno[defensores[i].posicion.fil][defensores[i].posicion.col] = ENANO;
		else 
			terreno[defensores[i].posicion.fil][defensores[i].posicion.col] = ELFO;
	}
}

/*
* Pre: enemigo y terreno deben ser válidos. El camino debe estar dentro del terreno.
* Post: Asigna un enemigo que esté en el camino recibido, solo si está vivo, al terreno.
*/
void asignar_enemigo(char terreno[MAX_TERRENO][MAX_TERRENO], coordenada_t camino[MAX_LONGITUD_CAMINO],
	 enemigo_t enemigo) {
	if (enemigo_esta_vivo(enemigo))
		terreno[camino[enemigo.pos_en_camino].fil][camino[enemigo.pos_en_camino].col] = ORCO;
}

/*
* Pre: El camino debe estar dentro del terreno, y ambos ser válidos.
* Post: Asigna el camino al terreno recibido, posición por posición.
*/
void asignar_camino(char terreno[MAX_TERRENO][MAX_TERRENO], coordenada_t camino[MAX_LONGITUD_CAMINO],
 int tope) {
	for (int i = 0; i < tope; i++) {
		terreno[camino[i].fil][camino[i].col] = CAMINO;
	}
}

/*
* Pre: tope_terreno debe ser >= 0 y <= MAX_TERRENO. El nivel debe ser válido con todas sus 
	estructuras válidas.
* Post: Inicializa el terreno, asignando los caminos y los enemigos si es que hay, y los defensores.
	Además asigna las entradas y torres de los caminos en la primera y última posición de estos,
	respectivamente.
*/
void inicializar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int tope_terreno, nivel_t nivel, 
	int nivel_actual) {
	
	for (int i = 0; i < tope_terreno; i++) {
		for (int j = 0; j < tope_terreno; j++) {
			terreno[i][j] = VACIO;
		}
	}
	// Caminos
	asignar_camino(terreno, nivel.camino_1, nivel.tope_camino_1);
	if (nivel_actual >= NORTE)
		asignar_camino(terreno, nivel.camino_2, nivel.tope_camino_2);

	//  Enemigos
	for (int i = 0; i < nivel.tope_enemigos; i++) {
		if (nivel.enemigos[i].camino == NUM_CAMINO_1)
			asignar_enemigo(terreno, nivel.camino_1, nivel.enemigos[i]);
		else
			asignar_enemigo(terreno, nivel.camino_2, nivel.enemigos[i]);
	}
	// Defensores
	asignar_defensores(terreno, nivel.defensores, nivel.tope_defensores);

	// Entradas y Torres
	asignar_entrada(terreno, nivel.camino_1);
	asignar_torre(terreno, nivel.camino_1, nivel.tope_camino_1, NUM_CAMINO_1);

	if (nivel_actual >= NORTE) {
		asignar_entrada(terreno, nivel.camino_2);
		asignar_torre(terreno, nivel.camino_2, nivel.tope_camino_2, NUM_CAMINO_2);

	}
}


/*
* Pre: tope_terreno <= MAX_TERRENO. El terreno debe estar inicializado.
* Post: Imprime el terreno por pantalla, con sus respectivos componentes
	(camino, enemigos, defensores, etc.).
*/
void mostrar_terreno(char terreno[MAX_TERRENO][MAX_TERRENO], int tope_terreno) {

	for (int i = 0; i < tope_terreno; i++) {
		printf("│ %.2i|", i);
		for (int j = 0; j < tope_terreno; j++) {
			if (terreno[i][j] == VACIO) printf(VERDE_OSCURO " ' " FIN);
			if (terreno[i][j] == CAMINO) printf(VERDE_CLARO "{ }" FIN);
			if (terreno[i][j] == ORCO) printf(VERDE_CLARO "{" AMARILLO "%s" VERDE_CLARO "}" FIN, PRINT_ORCO);
			if (terreno[i][j] == ENANO) printf(CYAN " %c " FIN, ENANO);
			if (terreno[i][j] == ELFO) printf(MAGENTA " %c " FIN, ELFO);
			if (terreno[i][j] == TORRE_1) printf(PLATEADO "%s" FIN, PRINT_TORRE_1);
			if (terreno[i][j] == TORRE_2) printf(PLATEADO "%s" FIN, PRINT_TORRE_2);
			if (terreno[i][j] == ENTRADA) printf(VERDE_OSCURO " %s " FIN, PRINT_ENTRADA);
		}
		printf(" │");
		printf("\n");
	}
	
}

/*
* Pre: -
* Post: Imprime por pantalla la vida de las torres. Si alguna torre no
	tiene vida(vida <= 0), imprime que su vida es 0.
*/
void mostrar_vida_torres (int vida_torre_1, int vida_torre_2 ) {
	if (vida_torre_1 <= SIN_VIDA)
		printf("\nVIDA TORRE 1: 0\t\t");
	else 
		printf("\nVIDA TORRE 1: %i\t\t", vida_torre_1);	
	
	if (vida_torre_2 <= SIN_VIDA) 
		printf("VIDA TORRE 2: 0\n");
	else 
		printf("VIDA TORRE 2: %i\n", vida_torre_2);
	
}

/*
* Pre: -
* Post: Imprime por pantalla la cantidad de elfos y enanos extra
	restantes.
*/
void mostrar_defensores_extra(int enanos_extra, int elfos_extra) {
	printf("\nENANOS EXTRA: %i\t\t\t", enanos_extra);
	printf("ELFOS EXTRA: %i\n", elfos_extra);
}

/*
* Pre: -.
* Post: Imprime por pantalla la cantidad de orcos que ya entraron en el 
	tablero, hayan muerto o no. Si la cantidad de enemigos sobrepasa los
	enemigos máximos del nivel, muestra estos últimos.
	(Esto ocurre porque se muestra el juego y luego se juega un turno).
*/
void mostrar_orcos_actuales(int cant_enemigos, int max_enemigos_nivel) {
 
	printf("\n\t\tORCOS ACTUALES: %i\n", cant_enemigos);
}


// FUNCIÓN PÚBLICA
void mostrar_juego(juego_t juego) {

	system("clear");
	int tope_terreno = tope_terreno_segun_nivel(juego.nivel_actual);
	char terreno[MAX_TERRENO][MAX_TERRENO];

	mostrar_vida_torres(juego.torres.resistencia_torre_1, juego.torres.resistencia_torre_2);

	printf("\t\t\t%i", juego.nivel_actual);

	mostrar_defensores_extra(juego.torres.enanos_extra, juego.torres.elfos_extra);

	mostrar_orcos_actuales(juego.nivel.tope_enemigos, juego.nivel.max_enemigos_nivel);

	printf("\n┌");
	for (int i = 0; i <= tope_terreno; i++) {
		printf("───");
	}
	printf("──┐\n");

	if (juego.nivel_actual < NORTE) {
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

	inicializar_terreno(terreno, tope_terreno, juego.nivel, juego.nivel_actual);

	mostrar_terreno(terreno, tope_terreno);
	if (juego.nivel_actual < NORTE) {
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