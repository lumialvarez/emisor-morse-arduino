/*
    Emisor_Morse.ino
    Autor: Luis Miguel Alvarez
    Fecha: 20/10/2014
    Emisor de codigo Morse por medio de un sistema optico (LASER), lee 
    el mensaje (cadena de texto) por medio del serial y posteriormente 
    es enviado por pulsos luminicos codificados en clave Morse. El 
    mensaje es enviado en minusculas.
    
    - Envie "ON" para encender el sistema optico.
    - Envie "OFF para apagar el sistema optico(por defecto).
    
    Probado con Arduino IDE 1.0.5, tarjetas Arduino MEGA y Leonardo.
*/

/*
Reglas de Codigo Morse internacionales
 1- Una raya equivale a 3 puntos
 2- El espacio entre señales es igual a 1 punto
 3- El espacio entre 2 letras es igual a 3 puntos
 4- El espacio entre 2 palabras es igual a 5 puntos
*/

// Codigos Morce del Alfabeto
char* letras[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

// Codigos Morce de los Numeros
char* numeros[] = {
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
  "---..", "----."}; // 0-9


int tem_punto = 200; // Tiempo en milisegundos de un punto
int PinLED = 13; // Pin salida LED confirmacion
int PinSalida = 8; // Pin salida Laser

short LF = 10;        // ASCII linefeed (Nueva linea)

void setup()
{
  Serial.begin(115200);
  
  pinMode(PinLED, OUTPUT);
  pinMode(PinSalida, OUTPUT);
  
  Serial.println("---------------------------------------");
  Serial.println("Bienvenido al Emisor de clave morse");
  Serial.println("---------------------------------------");
  Serial.println("Digita lo que quieres transmitir...");
}


/* 
   Este método principal que se repite "infinitamente". La funcion de 
   este metodo es leer el serial y obtener los mensajes que se desean 
   transmitir.
   Tambien permite el manejo del estado "DEBUG" para mostrar detalles 
   del funcionamiento.
   * Si la entrada por la consola SERIAL es igual a "on" hace que la 
     salida de los datos ("PinSalida") se quede en estado "HIGH" para 
     verificar el funcionamiento de dicha salida.
   * Si la entrada por la consola SERIAL es igual a "off" hace que 
     la salida de los datos ("PinSalida") se quede en estado "LOW" 
     (POR DEFECTO).
   
   * Se toma la entrada (cadena de texto) por el puerto SERIAL para 
   ser envidadas.
*/
void loop()
{
  if(Serial.available()>0){
    String cadena = Serial.readStringUntil(LF);
    cadena.toLowerCase();
    if(cadena.equals("on")){
      digitalWrite(PinSalida, HIGH);
      Serial.println("Debug ON");
    }
    else if(cadena.equals("off")){
      digitalWrite(PinSalida, LOW);
      Serial.println("Debug OFF");
    }else {
      Serial.println("-\n  Enviando \"" + cadena + "\"...");
      recorrer_cadena(cadena);
      Serial.println("Envio terminado!");
      delay(2000);
    }
  }
}


/* 
   Este método lee cada caracter de la cadena de caracteres que 
   entran como argumento y los traduce a la secuencia de carateres 
   de puntos y guiones(puntos y rayas). 
   
   @param cadena Cadena de caracteres que se van a enviar por el 
                 sistema optico.
*/
void recorrer_cadena(String cadena){
  int longitud_cadena = cadena.length();
  for(int i = 0 ;  i < longitud_cadena ; i++){
    char caracter = cadena[i];
    if (caracter >= 'a' && caracter <= 'z')
    {
      imprimir_resultado(caracter, letras[caracter - 'a']);
      mostrar_secuencia(letras[caracter - 'a']);
    }
    else if (caracter >= '0' && caracter <= '9')
    {
      imprimir_resultado(caracter, numeros[caracter - '0']);
      mostrar_secuencia(numeros[caracter - '0']);
    }
    else if (caracter == ' ')
    {
      while(cadena[i+1] == ' '){ i++; } //Salta todos los espacios en 
      Serial.println();                 //  blanco si hay mas de uno
      delay(tem_punto * 5); // Espacio entre palabras
    }
  }
}


/* 
   Este método recibe la secuencia de carateres de puntos y guiones, 
   los verifica que no sean nulos y llama al metodo mostrar_punto_raya 
   con cada uno de estos. 
   
   @param sequence Array de caracteres (puntos y rayas) resultado 
                   de la traduccion a Morse
*/
void mostrar_secuencia(char* sequence)
{
  int i = 0;
  while (sequence[i] != NULL)
  {
    mostrar_punto_raya(sequence[i]);
    i++;
  }
  delay(tem_punto * 3); // Espacio entre letras
}


/* 
   Este método recibe la carateres de puntos y guiones, y enciende 
   el sistema optico segun sea el caracter.
   
   @param punto_raya Caracter punto o guion
*/
void mostrar_punto_raya(char punto_raya)
{
  digitalWrite(PinLED, HIGH); 
  digitalWrite(PinSalida, HIGH);
  if (punto_raya == '.')  //  Si es punto
  {
    delay(tem_punto);
  }
  else // Si no es punto, deber ser raya
  {
    delay(tem_punto * 3);
  }
  digitalWrite(PinLED, LOW); 
  digitalWrite(PinSalida, LOW);
  delay(tem_punto); // Espacio entre señales
}


/* 
   Este método recibe el caracter original(el que se desea enviar)
   y  su correspondiente traduccion a codigo Morse y los envia al 
   Serial para confirmar al usuario el envio de cada caracter, 
   mostrando el caracter y su correspondiente traduccion a Morse.
   
   @param caracter Caracter que se desea enviar (caracter original)
   @param morse_codigo Cadena de caracteres resultado de la 
                       traduccion a morse
*/
void imprimir_resultado(char caracter, String morse_codigo){
  Serial.print(caracter);
  Serial.print(" -> ");
  Serial.println(morse_codigo);
}

