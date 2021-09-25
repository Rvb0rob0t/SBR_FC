----------------- MANUAL DE USO -----------------

 - Forma de ejecución:

	El programa "sbr-fc.exe" debe ejecutarse desde la consola de comandos proporcionándole los siguientes parámetros, ambos rutas (relativas o absolutas) del fichero en cada caso que contiene la información necesaria de:
	 - Primero: La Base de Conocimientos (BC).
	 - Segundo: La Base de Hechos (BH).

	Un ejemplo de ejecución sería el siguiente:
C:\Practica2> sbr-fc Pruebas\Prueba1\BCejemplo.txt Pruebas\Prueba1\BHejemplo.txt

	
 - En la carpeta de Pruebas encontrarás 5 pruebas con su respectivos ficheros de salida con las que poder probar el programa. También puedes construir tus propias pruebas con el formato explicado más adelante.


 - El programa creará un fichero de texto plano en la carpeta actual con el nombre "Inferencia_" seguido de los nombres de los ficheros de entrada (en orden) y unidos por un guión bajo. Ejemplo: "Inferencia_BCejemplo_BHejemplo.txt".


 - Formato de los ficheros:

	 - El formato de los ficheros de entrada es el siguiente:

		* Fichero de la Base de Conocimientos:

		  |	Nº de reglas                      |
		  |	Identificador1: Regla1, FC=factor1|
		  |	Identificador2: Regla2, FC=factor2|
		  |	...                               |
		  donde:
			- "Nº de reglas" indica la cantidad de reglas que compone la BC
			- "Identificador" es de la forma Ri (con i=1 hasta el nº de reglas)
			- "Regla" es de la forma 
				Si A Entonces B
			  donde A puede ser un literal (antecedente1), una conjunción de literales (antecedente1 y antecedente2...) o una disyunción de literales (antecedete1 o antecedente2...) y B es un literal (consecuente)
			- "factor" indica el factor de certeza asociado a la regla, que es básicamente un valor entre -1 y 1 inclusives que indica cuánto o con cuánta certeza afirman (si es positivo) o niegan (si es positivo) los antecedentes al consecuente.


		* Fichero de la Base de Hechos:

		  |	Nº de hechos                   |
		  |	hecho1, FC=factor1             |
		  |	hecho2, FC=factor2             |
		  |	...                            |
		  |	Objetivo                       |
		  |	hechoObjetivo                  |
		  donde:
			- "Nº de hechos" indica la cantidad de hechos.
			- "hecho" es la palabra (formada por cualquier tipo de caracteres sin espacio) que representa uno de los hechos de entrada
			- "factor" indica el factor de certeza asignado al hecho, es decir, un valor entre -1 y 1 que indica cómo de cierto (positivo) o falso (negativo) se considera el hecho.
			- "Objetivo" es una palabra reservada que indica que la siguiente línea es el hecho cuya certeza queremos determinar.
			- "hechoObjetivo" es la palabra (con el mismo formato que las anteriores) que representa el hecho cuya certeza queremos determinar.

		* Fichero de salida:

		  |	Base de Conocimientos NombreBC inicializada: |
		  |	R1: Regla1, FC=factor1                       |
		  |	R2: Regla2, FC=factor2                       |
		  |	...                                          |
		  |	                                             |
		  |	Base de Hechos NombreBH inicializada:        |
		  |	hecho1, FC=factorhecho1                      |
		  |	hecho2, FC=factorhecho2                      |
		  |	...                                          |
		  |	Meta: hechoObjetivo                          |
		  |	                                             |
		  |	Inferencia sobre hechoObjetivo               |
		  |	                                             |
		  donde:
			- "NombreBC" es el nombre de la BC
			- "Regla" es de la forma "A --> B" donde A, B y factor son iguales que en las reglas de entrada.
			- "NombreBH" es el nombre de la BH
			- Los hechos están escritos igual que en el fichero de BH de entrada.
			- "hechoObjetivo" es el mismo que en el fichero BH de entrada.
			- "Inferencia sobre hechoObjetivo" es la representación textual del proceso que sigue el sistema para inferir el hecho hechoObjetivo. Tiene la siguiente forma:
			  |  	Procedemos a verificar el hecho hechoObjetivo              |
			  |  	|	Conjunto conflicto = {Ri, Rj, ...}                 | *Calculamos las reglas cuyo consecuente es hechoObjetivo
			  |  	|	Escogemos la regla Ri: Reglai, FC=factori          | *Seleccionamos la regla con menor índice
			  |  	|	Inferencia sobre Antecedente1                      | *Obtenemos el factor de certeza de cada uno de los antecedentes de la regla seleccionada
			  |  	|	Inferencia sobre Antecedente2                      | 
			  |  	|	...                                                | 
			  |  	|	Inferencia CASO 1 calculo                          | *Combinamos los factores obtenidos de varios antecedentes si los hay
			  |  	|	Inferencia CASO 3 calculo                          | *Combinamos los factores obtenidos de los antecedentes de la regla aplicada
			  |  	|	Inferencia CASO 2 calculo                          | *Combinamos los factores obtenidos de varias reglas si las hay
			  |  	Concluimos hechoObjetivo con una certeza de factorfinal    | *Obtenemos el valor final del factor de certeza requerido.

