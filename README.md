# Morralla_Pesos

PDF tutorial paso a paso de como armar esta maquina
https://drive.google.com/file/d/1STIBDis3MC1J_WjcIFt2KaZz-80KG3DJ/view?usp=sharing

Codigo para Maquina Morralla_Pesos. 
// Codigo para Prototypo de Maquina "La Moralla"
// Este codigo detecta los pulsos del monedero electronico multimoneda para luego accionar un relevador que prende el hopper solo siempre y cuando los pulsos del sensor del hopper no excedan a los pulsos contados por el monedero*/
// Basicamente esta maquina te da feria de a peso $1 al introducirle monedas de $10.00,$5.00,$2.00, esta te arroja x9,x4,x1 peso rexpectivamente, ya que cobra un pedo por feriarte.
// Tiene la funcion de aceptar 3 monedas de $0.50 para darte una moneda de $1.00 peso.

###¿Por que publico este codigo?
Pues yo creo que cualquiera que tenga una maquina de Fruititas* y no quiera tirar su inversion a la basura pero tampoco busca correr el riesgo de que se la lleven los Malos* o la policia, puede convertir de esa maquina de apostar a una maquina que ferea monedas grandes en pesos. Con solo un poco de inversion extra y este codigo podra stener tu maquina fereadora de monedas a pesos. 

###Hardware para convertir tu maquina de Apostar Fruititas en una maquina para ferear
Solo ocupas comprar lo siguiente:
Un relevador ($70-$110 pesos Mexicanos) 
Un arduino Uno ($250-$450 pesos Mexicanos) 
Un Monedero electronico que acepte minimo 4 diferentes monedas ($750-$850 Pesos Mexicanos)
Costos por Modificar el Mueble, (Tapara entrada del teclado y pantalla)

###¿Que es la Maquina Moralla_Pesos?
La maquina moralla_pesos es una modificacion de las maquinas de apostar Frutitas, Pikachu. Muy populares en Mexico. Es basicamente un juego de azar de ruleta, introduces un peso y escojes una fruta, estas tienen diferentes valores, la maquina simula una ruleta con luzes led, esta gira alrededor de una matriz de leds, para simular la bolita girando en una ruleta, y si cae en la fruta seleccionada te da un premio segun el valor de esa fruta. Esta modificacion le quita la placa madre, el teclado y las bocinas para remplazarlas por un arduino uno y un relevador de 12v. Para ferear monedas grandes en pesos utilizamos un monedero electronico, que acepta hasta 4 monedas diferentes, emite pulsos segun la moneda introducida, estos pulsos los lee y cuenta el arduino uno, segun la cantidad de pulsos contados, prende el relevador que acciona el hopper, un motor que gira y aroja pesos, determinados segundos para dar los pesos segun la moneda introducida. 

###¿Cuando fue creado y para quien?
Esta maquina fue un prototipo para una maquina que deseaba ferear billetes, puesto que no tenia el presupuesto ni el vendedor para adquerir un lector de billetes por pulsos, ope por una version que ferea monedas de $10, $5, $2, y $0.50 en monedas de un peso. Para remplazar a los empleados de los egocios de las maquinas de videojuegos y maquinitas en mi estado, San Luis Potosi. El proyecto inicio en abril del 2015 y concluyo en Junio del 2015, le dedicaba mi tiempo minimo, despues de clases o trabajo y en fines de semana. 
###¿Donde fue creada?
Esta maquina fue creada en Ahualulco, San Luis Potosi, Mexico.
El codigo original fue tomado de "Usuario" para su proyecto de una amquina "" con arduino
Aqui la liga del foro donde pide ayuda para hacer correr el codigo.
Aqui las ligas de sus videos de su maquina funcionando.

###Ligas videos
Maquina Morralla_v1 Ferea monedas de $10,$5,$2,$0.50 en monedas de $1
https://www.youtube.com/watch?v=imXKcqyR9X4

Ensamble de maquina Morralla_Pesos
[Link de Video Youtube]

Explicacion de partes de la maquina Morralla_Pesos
[]

Modificacion del mueble Fruititas a una version cutre de una maquina feradora 
[]

###Libreria de funciones ultilizadas

##Glosario Terminos usados en la explicacion
Maquina de Frutitas: 
Los Malos:

