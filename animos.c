#include "animos.h"
#include <stdio.h>
#include <stdbool.h>

// Rango del día del mes
const int DIA_MES_MIN = 1;
const int DIA_MES_MAX = 30;

// Días los cuales cambia el viento
const int DIA_VIENTO_FUERTE_MAX = 10;
const int DIA_VIENTO_MODERADO_MAX = 20;

// Tipo de viento
const int VIENTO_FUERTE = 75;
const int VIENTO_MODERADO = 50;
const int VIENTO_LEVE = 25;


// Hora del día
const char MANIANA = 'M';
const char TARDE = 'T';
const char NOCHE = 'N';

// Humedad según hora del día
const int HUMEDAD_MANIANA = 75;
const int HUMEDAD_TARDE = 25;
const int HUMEDAD_NOCHE = 50;


// Pie con el que se levantó
const char PIE_DERECHO = 'D';
const char PIE_IZQUIERDO = 'I';


// Cena
const char ENSALADA = 'E';
const char HAMBURGUESA = 'H';
const char PIZZA = 'P';
const char GUISO = 'G';


// Rango de horas de sueño
const int HORAS_SUENIO_MIN = 0;
const int HORAS_MIN_SUENIO_REGULAR = 5;
const int HORAS_MAX_SUENIO_REGULAR = 8;
const int HORAS_SUENIO_MAX = 12;

////////////////////////////////////////////////////////

// Puntos a asignar

// Puntos pie
const int PUNTOS_PIE_DERECHO = 10;
const int PUNTOS_PIE_IZQUIERDO = 0;

// Puntos cena
const int PUNTOS_ENSALADA = 20;
const int PUNTOS_HAMBURGUESA = 15;
const int PUNTOS_PIZZA = 10;
const int PUNTOS_GUISO = 5;

// Puntos sueño
const int PUNTOS_SUENIO_MALO = 0;
const int PUNTOS_SUENIO_REGULAR = 10;
const int PUNTOS_SUENIO_BUENO = 20;

////////////////////////////////////////////////////////

// Ánimo del personaje
const char ANIMO_MALO = 'M';
const char ANIMO_REGULAR = 'R';
const char ANIMO_BUENO = 'B';

// Rangos de puntos totales según ánimo
const int MIN_PUNTOS_TOTALES = 5;
const int MIN_PUNTOS_ANIMO_REGULAR = 21;
const int MAX_PUNTOS_ANIMO_REGULAR = 35;
const int MAX_PUNTOS_TOTALES = 50;

////////////////////////////////////////////////////////


/*
* Pre: -

* Post: Devuelve true si dia está entre los días del mes(entre DIA_MES_MIN y DIA_MES_MAX). Sino, devuelve false.
*/
bool es_dia_valido(int* dia) {

	return( ((*dia >= DIA_MES_MIN) && (*dia <= DIA_MES_MAX)) );
}


/*
* Pre: -

* Post: Devuelve el día ingresado por el usuario, si este es válido. Sino, vuelve a preguntar.
*/
void preguntar_dia(int* dia) {

	do {

		printf("Indique el día del mes (1-30): ");
		scanf(" %i", dia);

	} while( !(es_dia_valido(dia)) );

}

/*
* Pre: -

* Post: Devuelve true si hora es M: MANIANA, T: TARDE, o N: NOCHE. Si no lo es, devuelve false.
*/
bool es_hora_valida(char* hora) {

	return( (*hora == MANIANA) || (*hora == TARDE) || (*hora == NOCHE) );
}

/*
* Pre: -

* Post: Devuelve la hora ingresada por el usuario, si es válida. Sino, vuelve a preguntar.
*/
void preguntar_hora(char* hora) {

	do {

		printf("Indique la hora del día con su inicial, en mayúscula (M - T - N): ");
		scanf(" %c", hora);

	} while ( !(es_hora_valida(hora)) );

}

/*
* Pre: -

* Post: Devuelve true si pie es PIE_DERECHO o PIE_IZQUIERDO. Si no lo es, devuelve false.
*/
bool es_pie_valido(char* pie) {

	return( (*pie == PIE_DERECHO) || (*pie == PIE_IZQUIERDO) );
}

/*
* Pre: -

* Post: Devuelve pie, con el valor ingresado por el usuario, si es válido. Si no lo es, vuelve a preguntar.
*/
void preguntar_pie(char* pie) {

	do {

		printf("Con qué pie se levantó nuestro líder? (I-D): ");
		scanf(" %c", pie);

	} while( !(es_pie_valido(pie)) );

}

/*
* Pre: -

* Post: Devuelve true si la cena está dentro de las cenas válidas (ENSALADA, HAMBURGUESA, PIZZA, GUISO). Devuelve false si no es válida.
*/
bool es_cena_valida(char* cena) {

	return( (*cena == ENSALADA) || (*cena == HAMBURGUESA) || (*cena == PIZZA) || (*cena == GUISO) );
}

/*
* Pre: -

* Post: Devuelve cena, con el valor ingresado por el usuario, si es válido. Si no lo es, vuelve a preguntar.
*/
void preguntar_cena(char* cena) {

	do {

		printf("Qué cenó la noche anterior? [Ensalada (E), Hamburguesa (H), Pizza (P), Guiso (G)]: ");
		scanf(" %c", cena);

	} while( !(es_cena_valida(cena)) );

}

/*
* Pre: -

* Post: Devuelve true si las horas de sueño están entre HORAS_SUENIO_MIN Y HORAS_SUENIO_MAX, o false si están fuera de ese rango.
*/
bool es_suenio_valido(int* horas_suenio) {

	return( (*horas_suenio >= HORAS_SUENIO_MIN) && (*horas_suenio <= HORAS_SUENIO_MAX) );

}

/*
* Pre: -

* Post: Devuelve horas_suenio, con el valor ingresado por el usuario, si es válido. Si no lo es, vuelve a preguntar.
*/
void preguntar_suenio(int* horas_suenio) {

	do {

		printf("Cuántas horas durmió nuestro querido líder? (0 a 12): ");
		scanf(" %i", horas_suenio);

	} while( !(es_suenio_valido(horas_suenio)) );

}

/*
* Pre: -

* Post: -
*/
void preguntar_animo_legolas(char* pie, char* cena, int* horas_suenio) {

	printf("Legolas es un tipo raro, y suele cambiar de ánimo fácilmente. Haremos unas preguntas para determinar su estado de ánimo.\n");

	preguntar_pie(pie);
	preguntar_cena(cena);
	preguntar_suenio(horas_suenio);

}

/*
* Pre: -

* Post: - 
*/
void preguntar_animo_gimli(char* pie, char* cena, int* horas_suenio) {

	printf("Gimli... ¿qué decir sobre él? Su estado de ánimo depende de hasta con qué pie que se levantó. Bueno, acá van unas preguntas para averiguar cómo se siente hoy.\n");

	preguntar_pie(pie);
	preguntar_cena(cena);
	preguntar_suenio(horas_suenio);

}

/*
* Pre: Recibe pie, que debe ser D: Derecho o I: Izquierdo, cena, que debe ser una de las cenas posibles(E: Ensalada, H: Hamburguesa, P: Pizza, G: Guiso), y horas_suenio que debe cumplir con el rango de horas válidas (HORAS_SUENIO_MIN a HORAS_SUENIO_MAX). Además, recibe puntos, que debe ser 0.

* Post: Devuelve la cantidad de puntos sumados, de acuerdo a los valores de pie, cena y horas_suenio. Los puntos deben estar entre MIN_PUNTOS_TOTALES y MAX_PUNTOS_TOTALES.
*/
void sumar_puntos(char pie, char cena, int horas_suenio, int* puntos) {

	if (pie == PIE_DERECHO) {

		*puntos += PUNTOS_PIE_DERECHO;

	} else {

		*puntos += PUNTOS_PIE_IZQUIERDO;
	}


	if (cena == ENSALADA) {

		*puntos += PUNTOS_ENSALADA;

	} else if (cena == HAMBURGUESA) {

		*puntos += PUNTOS_HAMBURGUESA;

	} else if (cena == PIZZA) {

		*puntos += PUNTOS_PIZZA;

	} else {

		*puntos += PUNTOS_GUISO;
	}


	if ( (horas_suenio >= HORAS_MIN_SUENIO_REGULAR) && (horas_suenio <= HORAS_MAX_SUENIO_REGULAR) ) {

		*puntos += PUNTOS_SUENIO_REGULAR;

	} else if (horas_suenio > HORAS_MAX_SUENIO_REGULAR) {

		*puntos += PUNTOS_SUENIO_BUENO;

	} else {

		*puntos += PUNTOS_SUENIO_MALO;
	}


}

/*
* Pre: Recibe pie, que debe ser D: Derecho o I: Izquierdo, cena, que debe ser una de las cenas posibles(E: Ensalada, H: Hamburguesa, P: Pizza, G: Guiso), y horas_suenio que debe cumplir con el rango de horas válidas (HORAS_SUENIO_MIN a HORAS_SUENIO_MAX).

* Post: Devuelve el estado de animo, M: Malo, B: Bueno, o R: Regular, que dependerá de la cantidad de puntos sumados.
*/
char estado_animo(char* pie, char* cena, int* horas_suenio) {

	int puntos = 0;
	char animo;
	sumar_puntos(*pie, *cena, *horas_suenio, &puntos);

	if ( (puntos >= MIN_PUNTOS_TOTALES) && (puntos < MIN_PUNTOS_ANIMO_REGULAR) ) {

		animo = ANIMO_MALO;

	} else if ( (puntos >= MIN_PUNTOS_ANIMO_REGULAR) && (puntos <= MAX_PUNTOS_ANIMO_REGULAR) ) {

		animo = ANIMO_REGULAR;

	} else if ( (puntos > MAX_PUNTOS_ANIMO_REGULAR) && (puntos <= MAX_PUNTOS_TOTALES) ) {

		animo = ANIMO_BUENO;

	}


	return animo;

}



/*
* Pre: Recibe el día, que debe estar en el rango de los días del mes, de DIA_MES_MIN a DIA_MES_MAX inclusive.

* Post: Devuelve 75(VIENTO_FUERTE) si dia está hasta DIA_VIENTO_FUERTE_MAX inclusive, sino 50(VIENTO_MODERADO) si está hasta DIA_VIENTO_MODERADO_MAX inclusive, y sino 25(VIENTO_LEVE).
*/
int viento_segun_dia(int* dia) {

	int viento;

	if ( (*dia <= DIA_VIENTO_FUERTE_MAX) ) {

		viento = VIENTO_FUERTE;

	} else if ( (*dia > DIA_VIENTO_FUERTE_MAX) && (*dia <= DIA_VIENTO_MODERADO_MAX) ) {

		viento = VIENTO_MODERADO;

	} else {

		viento = VIENTO_LEVE;
	}

	return viento;
}

/*
* Pre: Recibe hora, que debe ser M: MANIANA, T: TARDE, o N: NOCHE.

* Post: Devuelve humedad según hora. Si la hora es mañana devuelve 75(HUMEDAD_MANIANA), si la hora es tarde devuelve 25(HUMEDAD_TARDE), y sino devuelve 50(HUMEDAD_NOCHE).
*/
int humedad_segun_hora(char* hora) {

	int humedad;

	if ( (*hora == MANIANA) ) {

		humedad = HUMEDAD_MANIANA;

	} else if ( (*hora == TARDE) ) {

		humedad = HUMEDAD_TARDE;

	} else {

		humedad = HUMEDAD_NOCHE;
	}

	return humedad;
}

/*
* Pre: Recibe viento que debe ser VIENTO_FUERTE, VIENTO_MODERADO o VIENTO_LEVE, y humedad, que debe ser HUMEDAD_MANIANA, HUMEDAD_TARDE O HUMEDAD_NOCHE. Tambíén recibe animo_legolas y animo_gimli, que deben ser M: ANIMO_MALO, R: ANIMO_REGULAR, o B: ANIMO_BUENO.

* Post: -
*/
void resumen(int viento, int humedad, char animo_legolas, char animo_gimli) {

	if(viento == VIENTO_FUERTE) {

		printf("Fuerte tormenta de viento del oeste, de 75km/h, arrasando con todo, ");

	} else if(viento == VIENTO_MODERADO) {

		printf("Hay un viento de 50km/h que dificulta un poco la puntería de los Elfos, ");

	} else {

		printf("Suave brisa de 25 km/h corre de norte a sur, ");
	}

	if(humedad == HUMEDAD_MANIANA) {

		printf("se siente un ambiente pesado, que denota gran humedad... Se ve claramente que los Enanos están incómodos. ");

	} else if (humedad == HUMEDAD_TARDE) {

		printf("y la humedad es tan baja que los Enanos están incluso haciendo piruetas. ");
	} else {

		printf("los Enanos están un poco molestos por la humedad, pero se la bancan. ");
	}

	if(animo_legolas == ANIMO_BUENO) {

		printf("Excelente día para Legolas, se lo ve muy fresco. ");
	
	} else if (animo_legolas == ANIMO_REGULAR) {

		printf("No tan buen día para Legolas, aún así se lo ve tranquilo. ");

	} else {

		printf("Terrible día para Legolas, está hecho un desastre. No sabe ni donde está parado. ");
	}

	if (animo_gimli == ANIMO_BUENO) {

		printf("Gimli se levantó con toda la energía para pelear. Se lo ve mejor que nunca.\n");

	} else if (animo_gimli == ANIMO_REGULAR) {

		printf("A Gimli se lo ve normal, ni muy motivado ni muy desanimado. Es un tipo rústico.\n");

	} else {

		printf("Gimli tuvo una noche de terror. Se despertó todo despeinado y sin ganas de pelear.\n");
	}


	printf("\nResumen: %i %i %c %c.\n", viento, humedad, animo_legolas, animo_gimli);

}

void animos(int* viento, int* humedad, char* animo_legolas, char* animo_gimli) {

	int dia;
	char hora;
	char pie;
	char cena;
	int horas_suenio;

	preguntar_dia(&dia);
	preguntar_hora(&hora);

	printf("Día: %i. Hora: %c.\n", dia, hora);

	(*viento) = viento_segun_dia(&dia);
	(*humedad) = humedad_segun_hora(&hora);


	preguntar_animo_legolas(&pie, &cena, &horas_suenio);
	printf("Calculando el ánimo de Legolas...\n\n");
	(*animo_legolas) = estado_animo(&pie, &cena, &horas_suenio);

	
	preguntar_animo_gimli(&pie, &cena, &horas_suenio);
	printf("Calculando el ánimo de Gimli...\n\n");
	(*animo_gimli) = estado_animo(&pie, &cena, &horas_suenio);


	resumen((*viento), (*humedad), (*animo_legolas), (*animo_gimli));

}