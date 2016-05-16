#CCALC 

Es un programa de terminal de comandos que permite resolver operaciones con multiples operadores en una sola vez,
siguiendo el orden de importancia de los operadores y las reglas de programación.

##Argumentos y modificadores:

[ccalc] () puede recibir como primer argumento una operación compuesta por los operadores básicos (*, /, %, +, -), y parentesis, que puede ser de tamaño indefinido siempre y cuando no contenga espacios.
Alternativamente puede recibir como primer argumento el comando [--help] (), que muestra la pantalla de ayuda completa.

También permite usar los modificadores [--verbose] () y [--const] () despues de la operacion, [--verbose] (), para resolver de forma detallada mostrando cada paso realizado, y [--const] () para definir constantes que serán reemplazadas en la operación original antes de comenzar a resolverla, hay que destacar que son constantes y no variables por lo que solo permiten un valor fijo, las constantes pueden contener tanto numeros como operadores y ser de tamaño indefinido.

##Explicación:

 Sabiendo que las reglas para la solución de operaciones matemáticas por computadora son:

 0) Todas las operaciones se resuelven de izquierda a derecha y segun las 3 reglas de prioridad.
 1) Primero se resuelve lo que está entre parentesis.
 2) Después se resuelven operaciones de multiplicación (*), división (/) y módulo (%).
 3) Finalmente se resuelven sumas (+) y restas (-).

 Podemos deducir, como uno de los tantos posibles algoritmos de solución (y probablemente no el más optimo, pero si sencillo de exponer), el siguiente:

###Algoritmo:
```c++
Algoritmo:
{
    reescribimos la operación sustituyendo las constantes por su valor
    si la sintaxis de la operación es correcta
    {
        repetimos mientras no esté resuelto
        {
            si la operación contiene parentesis
            {
                obtenemos el subconjunto de operaciones que está en los parentesis mas internos
                si la operación contiene al menos una multiplicación, una división ó un módulo
                {
                    obtenemos la primera operación que contenga una multiplicación, una división ó un módulo,
                    calculamos el resultado de la operación anterior,
                    y reescribimos la operación obtenida por su resultado en el subconjunto de operaciones
                }
                si no, si el subconjunto de operaciones contiene al menos una suma, una resta ó un numero negativo
                {
                    si es un numero negativo
                    {
                        reescibimos el numero sin parentesis en la operación original                      
                    } 
                    si no
                    {
                        obtenemos la primera operación que contenga una suma ó una resta,
                        calculamos el resultado de la operación anterior,
                        y reescribimos la operación obtenida por su resultado en el subconjunto de operaciones 
                    }
                }
                si no 
                {
                    si solo contiene un numero entre parentesis,
                    reescribimos el numero en la operación quitando los parentesis
                }
            }
            si no, si la operación contiene al menos una multiplicación, una división o un módulo
            {
                obenemos la primera operación que contenga una multiplicación o una división o un módulo,
                calculamos el resultado de la operación anterior,
                y reescribimos la operación obenida por su resultado en la operación original 
            }
            si no, si la operación contiene al menos una suma, una resta o un numero negativo
            {
                si es un numero negativo
                {
                    consideramos el problema resuelto
                }
                si no
                {
                    obtenemos la primera operación que contenga una suma o una resta,
                    calculamos el resultado de la operación anterior,
                    y reescribimos la operación obtenida por su resultado en la operación original
                }
            }
            si no
            {
                consideramos el problema resuelto
            }
        }
    } si no {
        mostramos el error de sintaxis.
    }
}
```
* Además del algoritmo, se incluyen secciones de menu, y definición de mensajes largos para ahorro de espacio visual, y facilidad de lectura del código.

##Uso:

El uso de este programa es sencillo, al estilo de la mayoria de programas de terminal de comandos, se invoca desde la misma con su nombre, en este caso ccalc, incluyendo a continuación los argumentos para su fincionamiento.

use [ccalc --help] () para más información.

##Notas:

* La mayoria de los métodos devuelven un valor tipo string, esto es por que se ha elegido usar streams y strings para facilitar las conversiones entre tipos de tados y el procesamiento de las cadenas de texto que suponen los conjuntos de numeros y simbolos, ya que la máquina no interpreta directamente como numeros u operadores los simbolos que recibe por consola, si no como array de caracteres, que es posible convertir y manipular como strings.

Este programa está bajo los Terminos de la Licencia LGPL, creado por: [carlosmart7104] (https://github.com/carlosmart7104/.
