#include <iostream> // Para cout
#include <sstream> // Para stringstream
#include <cstring> // Para substr y strcmp
#include <cmath> // Para fmod
#include <windows.h> // Para Sleep

using namespace std; // Usaremos el espacio de nombres de la biblioteca estandar

void algoritmo(string operacion, string constantes, bool verbose); // Esta función contendrá nuestro algoritmo
void easter_egg(); // Esta función contiene las acciones del argumento --egg
void ayuda(); // Esta función contiene el mensaje de ayuda que muestra --help
void syntax_error(); // Esta función contiene el mensaje de error de sintaxis
bool revisar_sintaxis(string operacion); // Esta función revisa la sintaxis de la operación dada
// Estas funciones son test unitarios para evaluar la sintaxis, sus nombres se entienden
bool el_primer_parentesis_es_de_apertura(string operacion); 
bool los_parentesis_son_pares(string operacion);
bool no_hay_parentesis_vacios(string operacion);
bool hay_operadores_antes_y_despues_de_parentesis(string operacion);
bool no_hay_operadores_en_los_extremos(string operacion);
bool los_operadores_no_colindan_con_otros(string operacion);
// A partír de aquí son funciones que permiten el funcionamiento del programa, se ejecutan en el algoritmo central
bool comprobar_operadores(string operadores, string operacion); // Esta función comprueba si existen operaciones con los operadores indicados
bool comprobar_numero_negativo(string subconjunto); // Esta función comprueba si el numero dado es negativo
int obtener_operacion_unitaria_index(string operadores, string conjunto); // Esta función obtiene la posición donde comienza la operació unitaria según los operadores dados
int obtener_subconjunto_index(string operacion); // Esta función obtiene la posición inicial del subconjunto de operaciones 
string reescribir_constantes(string operacion, string constantes, bool verbose); // Esta función reescribe las constantes dadas en la operación original
string obtener_subconjunto(string operacion); // Esta función obtiene el subconjunto de operaciónes dentro de los parentesis más profundos
string obtener_operacion_unitaria(string operadores, string conjunto); // Esta función obtiene la primera operación con los operadores indicados
string calcular_resultado_unitario(string operacion_unitaria); // Esta función calcula el resultado de la operación obtenida con la función anterior
// Esta función reescribe en un conjunto la operación unitaria sustituyendola por su valor
string reescribir_operacion_unitaria(int operacion_unitaria_index, string operacion_unitaria, string resultado_unitario, string conjunto, bool borrar_parentesis_externos);

// En la función o método main se contienen las primeras acciones a ejecutar cuando se inicia el programa
// es nuestro vector de entrada, por definición retorna entero y solo puede recibir cero o dos arumentos
// el entero argc, que cuenta los argummentos recibidos desde la terminal, tomando como primer argumento el nombre
// el puntero array de caracteres argv[], que recibe los argumentos como cadenas de caracteres (no string)
int main(int argc, char* argv[]) {
    if(argc == 1){ // Si solo el primer argumento es detectado
        syntax_error(); // Mostramos el error de sintaxis
    } else if (argc > 1) { // Si se detecta uno o más argumentos además del nombre
        if (strcmp(argv[1], "--help") == 0){ // Evaluamos si el primer argumento es help
            ayuda(); // Entonces mostramos la ayuda
        } else if (strcmp(argv[1], "--egg") == 0){ // Si no evaluamos si el primer argumento es egg
            easter_egg(); // Entonces mostramos el egg
        } else { // Si no fue ninguno de los anteriores, se da por sentado que se ha dado una operación
            string arg = argv[1]; // Obtenemos la operación y la guardamos en el string "arg"
            if (argc == 3 && strcmp(argv[2], "--verbose") == 0) // Si el tercer argumento es verbose
            {
                algoritmo(arg, "", true); // Entonces invocamos el argumento sin constantes pero con verbose=true
            } // Si no, si el tercer o cuarto argumento es const
            else if (argc >= 3 && (strcmp(argv[2], "--const") == 0 || strcmp(argv[3], "--const") == 0))
            {
                int op_index = strcmp(argv[2], "--const") == 0 ? 2: 3; // Guardamos el numero de argumento en que estaba en op_index
                string constantes; // Declaramos el string que guardará las constantes
                for (int i = op_index + 1; i < argc; ++i) // Recorremos todos los argumentos después de const
                {
                    constantes += argv[i]; // Los vamos agregando a nuestra variable constantes
                    constantes += " "; // Separando cada uno con un espacio en blanco
                } // Después de eso
                if (strcmp(argv[2], "--verbose") == 0) // Si el argumento anterior era verbose
                {
                    algoritmo(arg, constantes, true); // Entonces invocamos el algoritmo con las coonstantes y detallado
                }
                else  // Si no
                {
                    algoritmo(arg, constantes, false); // Entoonces invocamos el algoritmo solo con las constantes
                }
            }
            else // Si fue ninguno de los argumentos modificadores
            {
                algoritmo(arg, "", false); // Entonces solo pasamos la operación sin detallar ni constantes
            }
        }
    }
}

void ayuda(){
    cout << // Aquí se imprime el texto de ayuda en pantalla
    "La sintaxis del programa es:\n\n"
    "\tccalc <argumento>(opcional) <operaci\242n> <modificador>(opcional) <constantes>(opcionales)\n\n"
    "\tPor ejemplo:\n\n"
    "\t\tccalc 3*2/(5-2), muestra en pantalla: 2\n\n"
    "\t\tccalc 3*2/(5-2) --verbose, muestra en pantalla:\n\n"
    "\t\t\t3*2/(5-2)\n"
    "\t\t\t3*2/(3)\n"
    "\t\t\t3*2/3\n"
    "\t\t\t6/3\n"
    "\t\t\t2\n\n"
    "\t\tccalc x/(5-2), --const x=3*2, muestra en pantalla: 2\n\n"
    "\t\tccalc x/(y) --verbose --const x=3*2 y=5-2, muestra en pantalla:\n\n"
    "\t\t\tx/(y)\n\n"
    "\t\t\tx=3*2\n"
    "\t\t\ty=5-2\n\n"
    "\t\t\t3*2/(5-2)\n"
    "\t\t\t3*2/(3)\n"
    "\t\t\t3*2/3\n"
    "\t\t\t6/3\n"
    "\t\t\t2\n\n"
    "Los argumentos disponibles son:\n\n"
    "\t--help, muestra la ayuda del programa.\n\n"
    "\t--egg, muestra un easter egg.\n\n"
    "\t<operaci\242n>, calcula el resultado de la operaci\242n dada.\n\n"
    "Las operaciones deben cumplir con las siguientes reglas de sintaxis:\n\n"
    "\tNo deben contener operadores distintos a los establecidos.\n\n"
    "\t\t*, para la multiplicaci\242n.\n"
    "\t\t/, para la divisi\242n.\n"
    "\t\t%, para el m\242dulo.\n"
    "\t\t+, para la suma.\n"
    "\t\t-, para la resta.\n\n"
    "\tPor cada parentesis de apertura '(', debe haber un parentesis de cierre ')'.\n"
    "\tEl primer parentesis debe ser de apertura '('.\n"
    "\tSi pueden contener numeros de punto flotante.\n"
    "\tSi pueden contener numeros negativos.\n\n"
    "Los modificadores disponibles son:\n\n"
    "\t--verbose, muestra la soluc\242n de la operaci\242n de manera detallada. Una l\241niea por operaci\242n resuelta.\n\n"
    "\t--const, permite el uso de constantes.\n\n"
    "\t*Use siempre --const como \243ltimo modificador y a continuaci\242n defina las constantes.\n\n"
    "Uso de las constantes:\n\n"
    "\tSe puede hacer uso de un numero ilimitado de constantes\n\n"
    "\tLas constantes ser\240n reemplazadas por sus valores antes de comenzar la soluci\242n de la operaci\242n, \n"
    "\tluego esta ser\240 solucionada de forma convensional.\n\n"
    "\tLas constantes deben cumplir con las siguientes reglas:\n\n"
    "\t\tDeben corresponder con la sintaxis <nombre>=<valor>, sin espacios.\n"
    "\t\tLos valores pueden ser enteros o de punto flotante, pero no negativos.\n"
    "\t\tLos valores pueden contener adem\240s de numeros, operaciones completas.\n"
    "\t\tLos nombres de las constantes deben comenzar con una letra y no deben contener caracteres\n"
    "\t\ta excepci\242n del gui\242n bajo, pero pueden contener numeros.\n"
    "\t\tLas constantes deben ir separadas un espacio.\n"
    "\t\tTodas las constantes usadas en la operaci\242n deben tener un valor asignado.\n\n";
}

void syntax_error(){
    cout << // Aquí se imprime el texto de error de sintaxis en pantalla
        " La sintaxis del comando no es correcta.\n"
        " Use la sintaxis \"ccalc <formula>\" (sin comillas) para resolver una operaci\242n.\n"
        " \tEjemplo: \"ccalc 4*(5*4-3)/2\", devuelve: 34.\n"
        " Use \"ccalc --help\" para mostrar la ayuda del programa.\n";
}

// Esta es la función que contiene el proceso de evaluación de sintaxis
// Aquí los nombres de los métodos lo dicen todo
bool revisar_sintaxis(string operacion){
    // Tests de parentesis
    if (el_primer_parentesis_es_de_apertura(operacion))
    {
        if (los_parentesis_son_pares(operacion))
        {
            if (no_hay_parentesis_vacios(operacion))
            {
                if (hay_operadores_antes_y_despues_de_parentesis(operacion))
                {
                    // Tests de operadores
                    if (no_hay_operadores_en_los_extremos(operacion))
                       {
                           if (los_operadores_no_colindan_con_otros(operacion))
                           {
                               return true;
                           }
                       }   
                }
            }
        }
    }
    return false;
}

// Funciones Test de sintaxis.
bool el_primer_parentesis_es_de_apertura(string operacion){
    for (int i = 0; i < operacion.size(); ++i) // Recorremos toda la operación
    {
        if (operacion[i]=='(') // Si encontramos primero un parentesis de apertura, devolvemos true
        {
            return true;
        }
        else if (operacion[i]== ')') // Si encontramos primero un parentesis de cierre, devolvemos false
        {
            return false;
        }
    }
    return true; // De cualquier otro modo, terminaremos devolviendo true... por cualquier cosa
}

bool los_parentesis_son_pares(string operacion){
    int paridad = 0; // Declaramos la variable de paridad
    for (int i = 0; i < operacion.size(); ++i) // recorremos toda la operación
    {
        if (operacion[i]=='(') // Si encoontramos un parentesis de apertura 
        {
            paridad++; // Sumamos uno a la paridad
        }
        else if (operacion[i]==')') // Si encontramos un parentesis de cierre
        {
            paridad--; // Restamos uno a la paridad
        }
    }
    if (paridad==0) // Si la paridad se mantuvo en cero, hay igual número de ambos
    {
        return true; // Devolvemos true
    } 
    else // Si no
    { 
        return false; // Devolvemos false
    }
}

bool no_hay_parentesis_vacios(string operacion){
    for (int i = 0; i < operacion.size(); ++i) // Recorremos toda la operación (ya  no pondré esto...)
    {
        if (operacion[i]=='(' && operacion[i+1]==')') // Si encontramos un parentesis de apertura seguido de uno de cierre
        {
            return false; // Hay parentesis vacíos, devolvemos false
        }
    }
    return true; // Todo salió bien, devolvemos true
}

bool hay_operadores_antes_y_despues_de_parentesis(string operacion){
    for (int i = 1; i < operacion.size()-1; ++i)
    {   // Si encontramos un parentesis de apertura a su izquierda NO hay cualquier otro simbolo operador
        if (operacion[i]=='(' && !(operacion[i-1]=='('||operacion[i-1]==')'||operacion[i-1]=='*'||operacion[i-1]=='/'||operacion[i-1]=='%'||operacion[i-1]=='+'||operacion[i-1]=='-'))
        {
            return false; // Devolvemos false, deseamos que haya siempre alguien operando con el parentesis
        } // Si encontramos un parentesis de cierre que a su derecha NO tiene algún operador
        if (operacion[i]==')' && !(operacion[i+1]=='('||operacion[i+1]==')'||operacion[i+1]=='*'||operacion[i+1]=='/'||operacion[i+1]=='%'||operacion[i+1]=='+'||operacion[i+1]=='-'))
        {
            return false; // Devolvemos false
        }
    }
    return true; // Si llegamos aquí todo ha salido bien, devolvemos true
}

bool no_hay_operadores_en_los_extremos(string operacion){
    for (int i = 0; i < operacion.size(); ++i)
    {
        // Si hay un operador al inicio o al final de la operación.
        if ((i==operacion.size()-1&&(operacion[i]=='*'||operacion[i]=='/'||operacion[i]=='%'||operacion[i]=='+'))||(i==0&&(operacion[i]=='*'||operacion[i]=='/'||operacion[i]=='%'||operacion[i]=='+')))
        {
            return false; // devolvemos false
        } 
    }
    return true; // Todo ha ido de maravilla, true
}

bool los_operadores_no_colindan_con_otros(string operacion){
    for (int i = 0; i < operacion.size(); ++i)
    {   // Si el acual y el próxomo caractéres son operadores (exceptuando el menos que no genera conflictos)
        if ((operacion[i+1]=='*'||operacion[i+1]=='/'||operacion[i+1]=='%'||operacion[i+1]=='+')&&(operacion[i]=='*'||operacion[i]=='/'||operacion[i]=='%'||operacion[i]=='+'))
        {
            return false; // Devolvemos false, nos interesa que los operadores tengan numeros a los lados (puede ser un negativo)
        }
    }
    return true; // Todo está en regla, devolvemos true
}

bool comprobar_operadores(string operadores, string operacion){
    for (int i = 0; i < operacion.size(); ++i)
    {
        if (   // La primera comparación es lo que nos piden, la segunda es averiguar si se cumple
               (operadores == "()" && (operacion[i] == '(' || operacion[i] == ')') )
            || (operadores == "*/%" && (operacion[i] == '*' || operacion[i] == '/' || operacion[i] == '%') )
            || (operadores == "+-" && (operacion[i] == '+' || operacion[i] == '-' ) ) 
            )
        {
            return true; // Si existe el tipo de operador por el que se pregunta devolvemos true
        }
    }
    return false; // Si llegamos aquí significa que no hubo coincidencias con lo preguntado, devolvemos false
}

bool comprobar_numero_negativo(string numero){
    // Esta es fácil...
    if (numero[0]=='-')
    {
        return true;
    }
    return false;
}


// Aquí tenemos el algoritmo central, las funciones utilizadas tienen nombres muy descriptivos
// y el algoritmo ya está explicado como seudocódigo en el README adjunto, así que no va comentado
void algoritmo(string operacion, string constantes, bool verbose){
    if (verbose) { cout << operacion; }
    operacion = reescribir_constantes(operacion, constantes, verbose);
    bool resuelto;
    if (revisar_sintaxis(operacion))
    {
        while (!resuelto){
            if (comprobar_operadores("()", operacion))
            {
                string subconjunto = obtener_subconjunto(operacion);
                int subconjunto_index = obtener_subconjunto_index(operacion);
                if (comprobar_operadores("*/%", subconjunto))
                {
                    string operacion_unitaria = obtener_operacion_unitaria("*/%", subconjunto);
                    int operacion_unitaria_index = obtener_operacion_unitaria_index("*/%", subconjunto) + subconjunto_index;
                    string resultado_unitario = calcular_resultado_unitario(operacion_unitaria);
                    operacion = reescribir_operacion_unitaria(operacion_unitaria_index, operacion_unitaria, resultado_unitario, operacion, false);
                    if (verbose) { cout << endl << operacion; }
                }
                else if (comprobar_operadores("+-", subconjunto))
                {
                    if (comprobar_numero_negativo(subconjunto))
                    {
                        operacion = reescribir_operacion_unitaria(subconjunto_index, subconjunto, subconjunto, operacion, true);
                        if (verbose) { cout << endl << operacion; }                        
                    } 
                    else
                    {
                        string operacion_unitaria = obtener_operacion_unitaria("+-", subconjunto);
                        int operacion_unitaria_index = obtener_operacion_unitaria_index("+-", subconjunto) + subconjunto_index;
                        string resultado_unitario = calcular_resultado_unitario(operacion_unitaria);
                        operacion = reescribir_operacion_unitaria(operacion_unitaria_index, operacion_unitaria, resultado_unitario, operacion, false);  
                        if (verbose) { cout << endl << operacion; }
                    }
                }
                else 
                {
                    operacion = reescribir_operacion_unitaria(subconjunto_index, subconjunto, subconjunto, operacion, true);
                    if (verbose) { cout << endl << operacion; }
                }
            }
            else if (comprobar_operadores("*/%", operacion))
            {
                string operacion_unitaria = obtener_operacion_unitaria("*/%", operacion);
                int operacion_unitaria_index = obtener_operacion_unitaria_index("*/%", operacion);
                string resultado_unitario = calcular_resultado_unitario(operacion_unitaria);
                operacion = reescribir_operacion_unitaria(operacion_unitaria_index, operacion_unitaria, resultado_unitario, operacion, false);   
                if (verbose) { cout << endl << operacion; }
            }
            else if (comprobar_operadores("+-", operacion))
            {
                if (comprobar_numero_negativo(operacion))
                {
                    resuelto = true;
                }
                else
                {
                    string operacion_unitaria = obtener_operacion_unitaria("+-", operacion);
                    int operacion_unitaria_index = obtener_operacion_unitaria_index("+-", operacion);
                    string resultado_unitario = calcular_resultado_unitario(operacion_unitaria);
                    operacion = reescribir_operacion_unitaria(operacion_unitaria_index, operacion_unitaria, resultado_unitario, operacion, false);
                    if (verbose) { cout << endl << operacion; }
                }
            }
            else
            {
                resuelto = true;
            }
        }
        if (!verbose) { cout << operacion; }
    } else {
        syntax_error();
    }
}

// Aquí se definen las funciones de apoyo para el algoritmo principal
string reescribir_constantes(string operacion, string constantes, bool verbose){
    int anterior_index = - 1;
    string clave, valor;
    if (constantes.size() > 0)
    {
        if (verbose) { cout << endl; }
        for (int i = 0; i < constantes.size(); ++i)
        { 
            if (constantes[i]=='=')
            {
                clave = constantes.substr(anterior_index + 1, i - anterior_index - 1);
                anterior_index = i;
            } 
            else if (constantes[i]==' ')
            {
                valor = constantes.substr(anterior_index + 1, i - anterior_index - 1);
                anterior_index = i;
                if (verbose) { cout << endl << clave << "=" << valor; }
                operacion.replace(operacion.find(clave), clave.size(), valor); 
            }
        }  
        if (verbose) { cout << endl << endl << operacion; }
    }
    return operacion;
}

string obtener_subconjunto(string operacion){
    string subconjunto;
    int apertura_index, subconjunto_size;
    char parentesis_anterior;
    for (int i = 0; i < operacion.size(); ++i){
        if (operacion[i]=='(' || operacion[i]==')') 
        { 
            if (operacion[i]=='(') 
            {
                apertura_index = i;
            } else if (operacion[i]==')' && parentesis_anterior=='(')
            { 
                subconjunto_size = i - apertura_index;
                break;
            }
            parentesis_anterior = operacion[i];
        }
    }
    if (subconjunto_size > 0)
    {
        subconjunto = operacion.substr(apertura_index + 1, subconjunto_size - 1);
    }
    return subconjunto;
}

int obtener_subconjunto_index(string operacion){
    string subconjunto;
    int apertura_index, subconjunto_size;
    char parentesis_anterior;
    for (int i = 0; i < operacion.size(); ++i){
        if (operacion[i]=='(' || operacion[i]==')') 
        { 
            if (operacion[i]=='(') 
            {
                apertura_index = i;
            } else if (operacion[i]==')' && parentesis_anterior=='(')
            { 
                subconjunto_size = i - apertura_index;
                break;
            }
            parentesis_anterior = operacion[i];
        }
    }
    if (subconjunto_size > 0)
    {
        subconjunto = operacion.substr(apertura_index + 1, subconjunto_size - 1);
    }
    return apertura_index + 1;
}

string obtener_operacion_unitaria(string operadores, string conjunto){
    string operacion_unitaria = conjunto;
    int operador_index = 0, operacion_inicio_index = 0, operacion_fin_index = conjunto.size() - 1;  
    for (int i = 0; i < conjunto.size(); ++i)
    {
        if ((operadores=="*/%" && (conjunto[i]=='*' || conjunto[i]=='/' || conjunto[i]=='%')) 
            || (operadores=="+-" && (conjunto[i]=='+' || conjunto[i]=='-')))
        {
            if (conjunto[i]=='-' && (i == 0 || conjunto[i-1]=='*' || conjunto[i-1]=='/' || conjunto[i-1]=='%' || conjunto[i-1]=='+' || conjunto[i-1]=='-'))
            {
                // si el operador actual es '-' y el de su izquierda es igual a cualquier simbolo o no existe, estamos ante un numero negativo.
                // lo ignoramos como operador
            } else {
                // De lo contrario estamos ante una resta
                operador_index = i;
                break;
            }
            
        }
    }
    if (operador_index > 0)
    {
        // buscamos del operador hacia adelante hasta el proximo simbolo operador (no simbolo de negativo).
        for (int i = operador_index + 2; i < conjunto.size(); ++i) // sumamos dos al index en lugar de uno por si es negativo.
        {
            if (conjunto[i]=='*' || conjunto[i]=='/' || conjunto[i]=='%' || conjunto[i]=='+' || conjunto[i]=='-')
            {
                operacion_fin_index = i - 1;
                break;
            }
        }
        // buscamos del operador hacia atras hasta el proximo simbolo o el index 0, evitando tomar en cuenta '-' cuando representa un negativo
        for (int i = operador_index - 1; i >= 0 ; --i)
        {
            if (conjunto[i]=='*' || conjunto[i]=='/' || conjunto[i]=='%' || conjunto[i]=='+' || conjunto[i]=='-')
            {
                // si es un menos, y se encuentra despues de un operador o es el primer caracter, estamos ante un negativo.
                if (conjunto[i]=='-' && (i == 0 || conjunto[i-1]=='*' || conjunto[i-1]=='/' || conjunto[i-1]=='%' || conjunto[i-1]=='+' || conjunto[i-1]=='-') ) 
                {
                    // lo ignoramos como operador pero acotamos el index de inicio en i, ya que ahi comienza nuestro numero.
                    if (i==0)
                    {
                        operacion_inicio_index = 0;
                    }
                    else
                    {
                        operacion_inicio_index = i;
                        break;
                    }
                }
                else
                {
                    // si no, estams ante una resta, la acotamos aqui.
                    operacion_inicio_index = i + 1;
                    break;
                }
            }
        }
        operacion_unitaria = conjunto.substr(operacion_inicio_index, operacion_fin_index - operacion_inicio_index + 1);
    }
    return operacion_unitaria;
}

int obtener_operacion_unitaria_index(string operadores, string conjunto){
    int operador_index = 0, operacion_inicio_index = 0, operacion_fin_index = conjunto.size() - 1;  
    for (int i = 0; i < conjunto.size(); ++i)
    {
        if ((operadores=="*/%" && (conjunto[i]=='*' || conjunto[i]=='/' || conjunto[i]=='%')) 
            || (operadores=="+-" && (conjunto[i]=='+' || conjunto[i]=='-')))
        {
            if (conjunto[i]=='-' && (i == 0 || conjunto[i-1]=='*' || conjunto[i-1]=='/' || conjunto[i-1]=='%' || conjunto[i-1]=='+' || conjunto[i-1]=='-'))
            {
                // si el operador actual es '-' y el de su izquierda es igual a cualquier simbolo o no existe, estamos ante un numero negativo.
                // lo ignoramos como operador
            } else {
                // De lo contrario estamos ante una resta
                operador_index = i;
                break;
            }
            
        }
    }
    if (operador_index > 0)
    {
        // buscamos del operador hacia adelante hasta el proximo simbolo operador (no simbolo de negativo).
        for (int i = operador_index + 2; i < conjunto.size(); ++i) // sumamos dos al index en lugar de uno por si es negativo.
        {
            if (conjunto[i]=='*' || conjunto[i]=='/' || conjunto[i]=='%' || conjunto[i]=='+' || conjunto[i]=='-')
            {
                operacion_fin_index = i - 1;
                break;
            }
        }
        // buscamos del operador hacia atras hasta el proximo simbolo o el index 0, evitando tomar en cuenta '-' cuando representa un negativo
        for (int i = operador_index - 1; i >= 0 ; --i)
        {
            if (conjunto[i]=='*' || conjunto[i]=='/' || conjunto[i]=='%' || conjunto[i]=='+' || conjunto[i]=='-')
            {
                // si es un menos, y se encuentra despues de un operador o es el primer caracter, estamos ante un negativo.
                if (conjunto[i]=='-' && (i == 0 || conjunto[i-1]=='*' || conjunto[i-1]=='/' || conjunto[i-1]=='%' || conjunto[i-1]=='+' || conjunto[i-1]=='-') ) 
                {
                    // lo ignoramos como operador pero acotamos el index de inicio en i, ya que ahi comienza nuestro numero.
                    if (i==0)
                    {
                        operacion_inicio_index = 0;
                    }
                    else
                    {
                        operacion_inicio_index = i;
                        break;
                    }
                }
                else
                {
                    // si no, estams ante una resta, la acotamos aqui.
                    operacion_inicio_index = i + 1;
                    break;
                }
            }
        }
    }
    return operacion_inicio_index;
}

string calcular_resultado_unitario(string operacion_unitaria){
    int operador_index;
    string resultado_unitario = operacion_unitaria;
    for (int i = 1; i < operacion_unitaria.size(); ++i) // comenzamos abuscar a partir del segundo caracter para evitar confusiones con negativo al iniciio
    {
        if (operacion_unitaria[i]=='*' || operacion_unitaria[i]=='/' || operacion_unitaria[i]=='%' || operacion_unitaria[i]=='+' || operacion_unitaria[i]=='-')
        {
            // Aqui no hay problema con los negativos, el primero simbolo es el operador.
            operador_index = i;
            break;
        }
    }
    if (operador_index > 0)
    {
        // si se encontró un operador.
        char operador = operacion_unitaria[operador_index];

        string numero_b = operacion_unitaria.substr(operador_index + 1, operacion_unitaria.size() - operador_index);
        string numero_a = operacion_unitaria.substr(0, operador_index);
        
        float a=0, b=0, c=0;
        stringstream stream_a;
        stream_a << numero_a;
        stream_a >> a;
        stream_a.clear();

        stringstream stream_b;
        stream_b << numero_b;
        stream_b >> b;
        stream_b.clear();

        if (operador=='*')
        {
            c = a * b;
        }
        else if (operador=='/')
        {
            c = a / b;
        }
        else if (operador=='%')
        {
            c = fmod(a, b);
        }
        else if (operador=='+')
        {
            c = a + b;
        }
        else if (operador=='-')
        {
            c = a - b;
        }

        stringstream stream_c;
        stream_c << c;
        stream_c >> resultado_unitario;
        stream_c.clear();
    } 
    return resultado_unitario;
}

string reescribir_operacion_unitaria(int operacion_unitaria_index, string operacion_unitaria, string resultado_unitario, string conjunto, bool borrar_parentesis_externos){
    if (borrar_parentesis_externos)
    {
        conjunto.replace(operacion_unitaria_index - 1, operacion_unitaria.size() + 2, resultado_unitario);
    }
    else {
        conjunto.replace(operacion_unitaria_index, operacion_unitaria.size(), resultado_unitario);
    }
    return conjunto;
}

void easter_egg(){
    // Ejecúta el código y descubre lo que hay aquí. xD
    // (No olvides subirle al volumen...)
    const char Art[1024] = {
        10,9,255,255,255,255,255,255,255,255,255,255,255,255,255,255,178,178,178,178,178,178,178,255,255,178,178,178,178,178,178,178,10,
        9,255,255,255,255,255,255,255,255,255,255,178,178,178,178,177,177,177,177,177,177,178,178,178,178,176,176,176,176,176,178,178,10,
        9,255,255,255,255,255,255,255,255,178,178,177,177,177,177,177,177,177,177,177,177,177,177,178,178,176,176,176,176,176,176,178,178,10,
        9,255,255,255,255,255,255,178,178,177,177,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,176,176,176,176,178,178,10,
        9,255,255,255,255,178,178,177,177,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,176,178,178,10,
        9,255,255,255,255,178,178,177,177,178,178,178,178,176,176,176,176,176,176,176,176,176,176,176,176,178,178,178,178,178,178,10,
        9,255,255,178,178,178,178,178,178,178,178,176,176,176,176,176,176,178,178,176,176,178,178,176,176,178,178,177,177,177,177,178,178,10,
        9,255,255,178,178,176,176,178,178,178,178,176,176,176,176,176,176,178,178,176,176,178,178,176,176,178,178,177,177,177,177,178,178,10,
        9,178,178,176,176,176,176,178,178,178,178,178,178,176,176,176,176,176,176,176,176,176,176,176,176,176,176,178,178,177,177,178,178,10,
        9,178,178,176,176,176,176,176,176,178,178,176,176,176,176,178,178,176,176,176,176,176,176,176,176,176,176,178,178,177,177,178,178,10,
        9,255,255,178,178,176,176,176,176,176,176,176,176,176,178,178,178,178,178,178,178,176,176,176,176,178,178,178,178,178,178,10,
        9,255,255,255,255,178,178,178,178,176,176,176,176,176,176,176,178,178,178,178,178,178,178,178,178,178,178,177,177,178,178,10,
        9,255,255,255,255,255,255,178,178,178,178,178,178,176,176,176,176,176,176,176,176,176,176,178,178,177,177,177,177,178,178,10,
        9,255,255,255,255,178,178,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,178,178,10,
        9,255,255,178,178,177,177,177,177,177,177,177,177,178,178,178,178,176,176,176,176,176,176,178,178,178,178,10,
        9,178,178,178,178,177,177,177,177,177,177,177,177,178,178,176,176,176,176,176,176,176,176,176,176,178,178,10,
        9,178,178,178,178,177,177,177,177,177,177,177,177,178,178,176,176,176,176,176,176,176,176,176,176,178,178,10,
        9,178,178,178,178,178,178,177,177,177,177,177,177,177,177,178,178,176,176,176,176,176,176,178,178,178,178,178,178,178,178,10,
        9,255,255,178,178,178,178,178,178,177,177,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,10,
        9,255,255,255,255,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,178,178,10,
        9,255,255,178,178,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,177,177,178,178,10,
        9,178,178,178,178,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,177,177,178,178,10,
        9,178,178,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,177,177,178,178,10,
        9,178,178,177,177,177,177,178,178,178,178,178,178,178,178,178,178,255,255,255,255,255,255,178,178,177,177,177,177,178,178,178,178,10,
        9,178,178,177,177,177,177,178,178,178,178,255,255,255,255,255,255,255,255,255,255,255,255,255,255,178,178,178,178,178,178,10,10
    };
    cout << Art;
    Beep (330,100);Sleep(100);
    Beep (330,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (330,100);Sleep(300);
    Beep (392,100);Sleep(700);
    Beep (196,100);Sleep(700);
    Beep (262,300);Sleep(300);
    Beep (196,300);Sleep(300);
    Beep (164,300);Sleep(300);
    Beep (220,300);Sleep(100);
    Beep (246,100);Sleep(300);
    Beep (233,200);
    Beep (220,100);Sleep(300);
    Beep (196,100);Sleep(150);
    Beep (330,100);Sleep(150);
    Beep (392,100);Sleep(150);
    Beep (440,100);Sleep(300);
    Beep (349,100);Sleep(100);
    Beep (392,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(100);
    Beep (247,100);Sleep(500);
    Beep (262,300);Sleep(300);
    Beep (196,300);Sleep(300);
    Beep (164,300);Sleep(300);
    Beep (220,300);Sleep(100);
    Beep (246,100);Sleep(300);
    Beep (233,200);
    Beep (220,100);Sleep(300);
    Beep (196,100);Sleep(150);
    Beep (330,100);Sleep(150);
    Beep (392,100);Sleep(150);
    Beep (440,100);Sleep(300);
    Beep (349,100);Sleep(100);
    Beep (392,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(100);
    Beep (247,100);Sleep(900);
    Beep (392,100);Sleep(100);
    Beep (370,100);Sleep(100);
    Beep (349,100);Sleep(100);
    Beep (311,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (207,100);Sleep(100);
    Beep (220,100);Sleep(100);
    Beep (262,100);Sleep(300);
    Beep (220,100);Sleep(100);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(500);
    Beep (392,100);Sleep(100);
    Beep (370,100);Sleep(100);
    Beep (349,100);Sleep(100);
    Beep (311,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (523,100);Sleep(300);
    Beep (523,100);Sleep(100);
    Beep (523,100);Sleep(1100);
    Beep (392,100);Sleep(100);
    Beep (370,100);Sleep(100);
    Beep (349,100);Sleep(100);
    Beep (311,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (207,100);Sleep(100);
    Beep (220,100);Sleep(100);
    Beep (262,100);Sleep(300);
    Beep (220,100);Sleep(100);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(500);
    Beep (311,300);Sleep(300);
    Beep (296,300);Sleep(300);
    Beep (262,300);Sleep(1300);
    Beep (262,100);Sleep(100);
    Beep (262,100);Sleep(300);
    Beep (262,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(300);
    Beep (330,200);Sleep(50);
    Beep (262,200);Sleep(50);
    Beep (220,200);Sleep(50);
    Beep (196,100);Sleep(700);
    Beep (262,100);Sleep(100);
    Beep (262,100);Sleep(300);
    Beep (262,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(100);
    Beep (330,100);Sleep(700);
    Beep (440,100);Sleep(300);
    Beep (392,100);Sleep(500);
    Beep (262,100);Sleep(100);
    Beep (262,100);Sleep(300);
    Beep (262,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (294,100);Sleep(300);
    Beep (330,200);Sleep(50);
    Beep (262,200);Sleep(50);
    Beep (220,200);Sleep(50);
    Beep (196,100);Sleep(700);
    Beep (330,100);Sleep(100);
    Beep (330,100);Sleep(300);
    Beep (330,100);Sleep(300);
    Beep (262,100);Sleep(100);
    Beep (330,100);Sleep(300);
    Beep (392,100);Sleep(700);
    Beep (196,100);Sleep(700);
    Beep (196,100);Sleep(125);
    Beep (262,100);Sleep(125);
    Beep (330,100);Sleep(125);
    Beep (392,100);Sleep(125);
    Beep (523,100);Sleep(125);
    Beep (660,100);Sleep(125);
    Beep (784,100);Sleep(575);
    Beep (660,100);Sleep(575);
    Beep (207,100);Sleep(125);
    Beep (262,100);Sleep(125);
    Beep (311,100);Sleep(125);
    Beep (415,100);Sleep(125);
    Beep (523,100);Sleep(125);
    Beep (622,100);Sleep(125);
    Beep (830,100);Sleep(575);
    Beep (622,100);Sleep(575);
    Beep (233,100);Sleep(125);
    Beep (294,100);Sleep(125);
    Beep (349,100);Sleep(125);
    Beep (466,100);Sleep(125);
    Beep (587,100);Sleep(125);
    Beep (698,100);Sleep(125);
    Beep (932,100);Sleep(575);
    Beep (932,100);Sleep(125);
    Beep (932,100);Sleep(125);
    Beep (932,100);Sleep(125);
    Beep (1046,675);
}