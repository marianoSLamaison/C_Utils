# C_UTILS

   Es un cumpilado de funciones y structs que uso comunmente al 
trabajar en C. Es para uso personal, y tiene apuntes por si alguien es 
curioso, pero mi idea es que se pueda usar en cualquier projecto.
Le ire añadiendo partes a medida que valla necesitando cosas, pero por ahora
las cosas que tengo planeado son
1. Manejo de listas linkeadas simples.
2. Manejo de Hash Maps
3. Manejo o al menos funciones generales de arrays
4. BitMaps y funciones para trabar estos
5. Algo para leer archivos de configuración

## Nota respecto de la creacion de librerias C

*Recordatorios*
Recuerda poner los headers en algun lugar facil de encontrar, 
los usuarios tiene que poder llamar a estos para usar tu libreria.
	Tambien recuerda usar -I$(CURDIR) junto a -L(Donde sea que hallas metido el .a o .so)
	De otra manera gcc no sabra que hacer
