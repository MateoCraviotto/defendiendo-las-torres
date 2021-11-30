# Defendiendo las torres
Proyecto para la materia Algoritmos y Programación I [75.40 - 95.14] - Facultad de Ingeniería, UBA.

## Introducción
El presente trabajo consiste en la unión de tres trabajos prácticos, hechos uno a continuación del otro, durante la materia. El trabajo resultante consiste en un juego tipo Tower Defense por consola, desarrollado en el lenguaje C. 

Los enunciados de las 3 partes del trabajo se encuentran en la carpeta ```enunciados```.


## Compilación y ejecución

Se incluye un Makefile que facilita la compilación y ejecución del juego. Los comandos son los siguientes:

- ```make jugar```: ejecuta el juego listo para jugar
    
- ```make repe```: ejecuta la repetición del juego, que se debe guardar en el archivo llamado ```mi_partida.dat```, con velocidad 0.2.
    
- ```make personalizado```: ejecuta el juego con la configuración personalizada del archivo llamado ```mi_config.txt```
  
 Si se desea compilar y ejecutar por separado, se puede hacer de la siguiente manera:
 
 - Compilación:
   	    
        gcc *.c utiles.o -Wall -Werror -Wconversion -std=c99 -o juego
   
 - Ejecución del juego:
        
        ./juego jugar
        
 - Ejecución con parámetros opcionales (pueden utilizarse en conjunto):
  
    - Grabación de partida:
  
          ./juego jugar grabacion=mi_grabacion
        
      Donde ```mi_grabacion``` será el archivo donde se guardará la grabación (puede sobreescribir archivos existentes con el mismo nombre).
   
   
    - Configuración personalizada:

          ./juego jugar config=archivo_configuración
        
      Donde ```archivo_configuración``` corresponde al archivo de configuración personalizada. Para más detalles, ver el enunciado de la parte 3.
  

 - Ejecución de la repetición de una partida del juego:
 
        ./juego poneme_la_repe grabacion=mi_grabacion velocidad=mi_velocidad
        
    Donde ```mi_grabacion``` corresponde al archivo de grabación que debe ser grabado previamente.
