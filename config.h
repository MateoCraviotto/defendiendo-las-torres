#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "defendiendo_torres.h"

#define MAX_NIVELES 4
#define MAX_RUTA 80
#define MAX_NOMBRE 50
#define ERROR -1
#define EXITO 0
//const int EXITO = 0;
#define LECTURA "r"
#define ESCRITURA "w"


//const int VALOR_POR_DEFECTO = -1;

typedef struct jugador {
	char nombre[MAX_NOMBRE];
	int puntos;
} jugador_t;

/* 			
	Función del TP2
* Pre: nivel debe ser válido.
* Post: Asigna las coordenadas de la(s) torre(s) según el nivel.
*/
void coord_torre(int nivel, coordenada_t* torre_1, coordenada_t* torre_2, int tope);


/*
	Función del TP2
* Pre: nivel debe ser válido.
* Post: Asigna las coordenadas de la(s) entrada(s) según el nivel.
*/
void coord_entrada(int nivel, coordenada_t* entrada_1, coordenada_t* entrada_2, int tope);

/*
* Pre: La ruta debe ser válida, y la velocidad > 0.
* Post: Muestra la repetición de una partda, leyendo el archivo recibido, 
	avanzando de turno según la velocidad recibida.
*/
void poneme_la_repe(char ruta[MAX_RUTA], float velocidad);


/*
* Pre: El separador debe estar en alguna parte del parámetro.
* Post: Corta el string parametro y obtiene la primera parte de este, es decir, hasta
	el separador. Lo almacena en parte_uno. Si logra cortar el string, devuelve la posición
	del separador. Sino, devuelve ERROR.
*/
int cortar_string_parte_uno(char parametro[], char parte_uno[], char separador);

/*
* Pre: pos_separador debe ser la posición del separador dentro del string parametro. 
* Post: Obtiene la segunda parte del string parámetro, es decir, desde pos_separador hasta
	el final del string, y lo almacena en parte_dos.
*/
void cortar_string_parte_dos(char parametro[], char parte_dos[], int pos_separador);


/*
* Pre: La ruta debe ser válida.
* Post: Pide al usuario los valores de los campos de configuración del juego y 
	los almacena en el archivo de la ruta recibida.
*/
void crear_configuracion(char ruta[MAX_RUTA], configuracion_t* configuracion);


/*
* Pre: La ruta debe ser válida.
* Post: Pide al usuario que cree los caminos para los 4 niveles y los almacena en el archivo 
	de la ruta recibida, para ser utilizados en una partida.
*/
int crear_camino(char ruta[MAX_RUTA]);

/*
* Pre: La ruta debe ser válida y cantidad_jugadores mayor a 0.
* Post: Lista todos los jugadores que hayan jugado con la configuración almacenada en la
	ruta_config recibida, con sus respectivos puntajes obtenidos. Si se recibe la ruta por 
	defecto, se mostrará el ranking por defecto.
*/
void mostrar_ranking(int cantidad_jugadores, char ruta_config[MAX_RUTA]);

/*
* Pre: La ruta de configuración recibida debe ser válida o ser la ruta por defecto. Los puntos
	recibidos deben ser >= 0.
* Post: Escribe el nombre del jugador recibido y sus respectivos puntos, en el ranking de la configuración
	recibida. Si se recibe la ruta_config por defecto, se utiliza el ranking por defecto.
*/
void guardar_jugador_en_ranking(char nombre[MAX_NOMBRE], int puntos, char ruta_config[MAX_RUTA]);

#endif /* __CONFIG_H__ */