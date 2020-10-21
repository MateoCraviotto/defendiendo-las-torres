compilar: 
	gcc *.c utiles.o -Wall -Werror -Wconversion -std=c99 -o juego
jugar: compilar
	./juego jugar
repe: compilar
	./juego poneme_la_repe grabacion=mi_partida.dat velocidad=0.2
personalizado: compilar
	./juego jugar config=mi_config.txt
