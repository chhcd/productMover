//Creado por Luis Arturo Gonzalez Benson
//ITESM Campus Monterrey
//Semana i Septiembre 2016

//Todas las perspectivas de giros y deteccion de lineas se asumen viendo el robot de frente

//El lado izquiero del robot es la zona B y el lado derecho la A

//Para utilizar los sensores de proximidad se debe adecuar su respectiva funcion con interrupciones, ya que el utiizarlos sin interrupciones alenta   
//el micro y el programa, lo que crea errores en otras funciones, como el detector de linea

//Se refiere a 0 como deteccion del color blanco y 1 como deteccion del color negro

//valores de sensores en cero
int valSLA = 0;    //SLA = Sensor seguidor de linea A
int valSLB = 0;    //SLB = Sensor seguidor de linea B    
int valSIA = 0;    //SIA = Sensor detector de interseccion A
int valSIB = 0;    //SIA = Sensor detector de interseccion B

//pines a utilizar
int SLA = 2;             //Sensor de deteccion de linea A en pin 1
int SLB = 4;             //Sensor de deteccion de linea B en pin 2
int SIA = 14;            //Sensor de interseccion A en pin A0
int SIB = 15;            //Sensor de interseccion B en pin A1
int US1s = 5;            //Salida sensor ultrasonica 1 en pin 5    //Va conectado a TRIG
int US1e = 6;            //Entrada sensor ultrasonico 1 en pin 6   //Va a ECHO
int US2s = A2;           //Salida sensor ultrasonico 2 en pin A2   //Va conectado a TRIG
int US2e = A3;           //Entrada sensor ultrasonico 2 en pin A3  //Va a ECHO
int E1MA = 7;            //Entrada 1 Motor A en pin 7
int E2MA = 8;            //Entrada 2 Motor A en pin 8
int SAMA = 9;            //Salida analogica Motor A en pin 9
int E1MB = 10;           //Entrada 1 Motor B en pin 10
int E2MB = 12;           //Entrada 2 Motor b en pin 12
int SAMB = 11;           //Salida analogica Motor B en pin 11
const int actuador=2;    //Se define pin del arduino del actuador
const int pinServo=A4;   //Se define el pin del servo

//set-up sensor de proximidad
unsigned int dista1;
unsigned int dista2;
int valor;
int tiempo1;
int tiempo2;

//set-up para el servo
#include <Servo.h> //se incluye header Arduino
//los angulos tanto de origen como de desplazamiento son unicos para cada robot, para el 1 se utilizan los valores mostrados, para los otros dos se debe utilizar 70 en origen y 90 en desplazado pero hay que ajustarlos haciendo pruebas
#define origen 0 //Se define el angulo inicial
#define desplazado 90 //Se define el angulo al que debe cerrar
Servo seguro; //definicion de servo del "seguro"


//funciones
//funcion para cuando el robot llega a una interseccion
bool interseccionDetectada()
{
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  if ((valSLA == 0 || valSLA == 1) && (valSLB == 0 || valSLB == 1) && valSIA == 0 && valSIB == 0)
    return true;
  return false;
}

//funcion para hacer que los motores hagan que el robot avance
int avanzaMotores(){
  digitalWrite(E1MA, LOW);
  digitalWrite(E2MA, HIGH);
  analogWrite(SAMA, 100);       
  digitalWrite(E1MB, LOW);
  digitalWrite(E2MB, HIGH);
  analogWrite(SAMB, 100);
}

//funcion para hacer que los motores hagan que el robot gire a la derecha
int mueveMotoresD(){
  digitalWrite(E1MA, HIGH);
  digitalWrite(E2MA, LOW);
  analogWrite(SAMA, 100);       
  digitalWrite(E1MB, LOW);
  digitalWrite(E2MB, HIGH);
  analogWrite(SAMB, 100);
}

//funcion para hacer que los motores hagan que el robot gire a la izquierda
int mueveMotoresI(){
  digitalWrite(E1MA, LOW);
  digitalWrite(E2MA, HIGH);
  analogWrite(SAMA, 100);       
  digitalWrite(E1MB, HIGH);
  digitalWrite(E2MB, LOW);
  analogWrite(SAMB, 100); 
}

//funcion que hace que el robot gire a la derecha en una interseccion
int giraDerecha() {
  int contGD = 0;                    //Contador para el giro hacia la derecha
  valSLA = digitalRead(SLA);         //Se leen los valores de los sensores seguidores de linea
  valSLB = digitalRead(SLB);
  if (valSLA == 0 && valSLB == 0)    //Puede ser que el robot no detecte solo la linea blanca al momento de llegar a la interseccion, puede que alguno
    contGD = 1;                      //de los sensores detecte la cinta negra, por eso se consideran los 3 posibles casos y para cada caso se adecua su 
  if (valSLA == 0 && valSLB == 1)    //respectivo giro
    contGD = 2;
  if (valSLA == 1 && valSLB == 0)
    contGD = 3;
  
  switch (contGD) {
    case 1:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while ((valSLA == 0 || valSLA == 1) && valSLB == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("1 Etapa!");
      }
      while ((valSLA == 0 || valSLA == 1) && valSLB == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("2 Etapa!");
      }
      avanzaMotores();
      //Serial.println("Caso 1 Giro a la Derecha!");
      break;

    case 2:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while(valSLA == 0 && valSLB == 1){
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD(); 
        //Serial.println("1 Etapa!");
      }
      while ((valSLA == 0 || valSLA == 1) && valSLB == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("2 Etapa!");
      }
      while ((valSLA == 0 || valSLA == 1) && valSLB == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("3 Etapa!");
      }
      avanzaMotores();
      //Serial.println("Caso 2 Giro a la Derecha!");
      break;

    case 3:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while (valSLA == 1 && valSLB == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("1 Etapa!");
      }
      while ((valSLA == 0 || valSLA == 1) && valSLB == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("2 Etapa!");
      }
      avanzaMotores();
      //Serial.println("Caso 3 Giro a la Derecha!");
      break;
      
    default:
      break;
  }
  //Serial.println("termina giro a la derecha!");
  delay(300);                                      //Se utiliza el delay para permitir que el robot avance para que asi se slaga de la interseccion y no se cicle el caso
}

//funcion que hace que el robot gire a la derecha en una interseccion
int giraIzquierda(){
  int contGI = 0;                     //Contador para el giro hacia la izquierda
  valSLA = digitalRead(SLA);          //Se leen los valores de los sensores seguidores de linea
  valSLB = digitalRead(SLB);
  if (valSLA == 0 && valSLB == 0)    //Puede ser que el robot no detecte solo la linea blanca al momento de llegar a la interseccion, puede que alguno
    contGI = 1;                      //de los sensores detecte la cinta negra, por eso se consideran los 3 posibles casos y para cada caso se adecua su
  if (valSLB == 0 && valSLA == 1)    //respectivo giro
    contGI = 2;
  if (valSLB == 1 && valSLA == 0)
    contGI = 3;
  
  switch (contGI) {
    case 1:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while ((valSLB == 0 || valSLB == 1) && valSLA == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("1 Etapa Giro a la Izquierda!");
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("2 Etapa Giro a la Izquierda!");
      }
      avanzaMotores();
      break;

    case 2:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while(valSLB == 0 && valSLA == 1){
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI(); 
        //Serial.println("1 Etapa Giro a la Izquierda!")
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("2 Etapa Giro a la Izquierda!");
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("3 Etapa Giro a la Izquierda!");
      }
      avanzaMotores();
      break;

    case 3:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while (valSLB == 1 && valSLA == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("1 Etapa Giro a la Izquierda!");
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("2 Etapa Giro a la Izquierda!");
      }
      avanzaMotores();
      break;
      
    default:
      break;
  }  
  delay(300);        //Se utiliza el delay para permitir que el robot avance para que asi se slaga de la interseccion y no se cicle el caso
}

//funcion para hacer que el robot se siga derecho en una interseccion
int sigueDerecho(){
 int contSD = 0;                      //Contador para seguirse derecho
  valSLA = digitalRead(SLA);          //Se leen los valores de los sensores seguidores de linea
  valSLB = digitalRead(SLB);
  if (valSLA == 0 && valSLB == 0)     //Puede ser que el robot no detecte solo la linea blanca al momento de llegar a la interseccion, puede que alguno
    contSD = 1;                       //de los sensores detecte la cinta negra, por eso se consideran los 3 posibles casos y para cada caso se adecua su
  if (valSLB == 0 && valSLA == 1)     //respectivo giro
    contSD = 2;
  if (valSLB == 1 && valSLA == 0)
    contSD = 3;
  
  switch (contSD) {
    case 1:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      avanzaMotores();
      delay(250);
      break;
      
    case 2:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      avanzaMotores();
      delay(250);
      break;
      
    case 3:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      avanzaMotores();
      delay(250);
      break;
      
    default:
      break;     
  }
}

//Funciones para el actuador y el servo
void FUNsubir(){ //Funcion para que suba el actuador y el servo gire y ponga "seguro" y levantado el anaquel
analogWrite(actuador,255);
delay(1000);
cerrarservo();
alto();
}
void FUNbajar(){
analogWrite(actuador,255);
delay(1000);
abrirservo();
alto();
}

void alto(){
analogWrite(actuador,0); //Se desenergiza el actuador
}  

void cerrarservo(){
seguro.write(desplazado); //Se cierra el servo y se pone en el seguro
}
void abrirservo(){
seguro.write(origen); //Se abre el servo y regresa al origen
}


void setup() {
  //declarar pines como salida y entrada
  Serial.begin(9600);
  pinMode(SLA, INPUT);       //Entrada sensor de deteccion de linea A
  pinMode(SLB, INPUT);       //Entrada sensor de deteccion de linea b
  pinMode(SIA, INPUT);       //Entrada sensor de deteccion de interseccion A
  pinMode(SIB, INPUT);       //Entrada sensor de deteccion de interseccion B
  pinMode(US1s, OUTPUT);     //Salida pulso ultrasonico 1 trigger
  pinMode(US1e, INPUT);      //Entrada, rebote ultrasonico 1 echo
  pinMode(US2s, OUTPUT);     //Salida pulso ultrasonico 2 trigger
  pinMode(US2e, INPUT);      //Entrada rebote ultrasonico 2 echo
  pinMode(E1MA, OUTPUT);     //Eentrada 1 Motor A
  pinMode(E2MA, OUTPUT);     //Entrada 2 Motor A
  pinMode(SAMA, OUTPUT);     //Salida analogica Motor A
  pinMode(E1MB, OUTPUT);     //Eentrada 1 Motor B
  pinMode(E2MB, OUTPUT);     //Entrada 2 Motor B
  pinMode(SAMB, OUTPUT);     //Salida analogica Motor B
  pinMode(actuador, OUTPUT);
  pinMode(pinServo, OUTPUT);
  seguro.attach(pinServo);  // Pin del servomotor
}

void loop() {
  //se cambian valores de los sensores de acuerdo al valor leido
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);

//Si se desea saber que valores estan midiendo los sensores seguidores de linea e interseccion  
//   Serial.print("medicion A1  ");
//   Serial.println(valSLA);
//   Serial.print("medicion B1  ");
//   Serial.println(valSLB);
//   Serial.print("medicion SIA  ");
//   Serial.println(valSIA);
//   Serial.print("medicion SIB  ");
//   Serial.println(valSIB);

//Si se desea habilitar los sensores de proximidad se debe descomentar el siguiente bloque
/* 
digitalWrite(US1s, LOW);          // Estabilizacion sensor ultrasonico 1
delayMicroseconds(5);
digitalWrite(US1s, HIGH);         //Pulso ultrasonico en sensor 1
delayMicroseconds(10);
digitalWrite(US1s, LOW);
tiempo1 = pulseIn(US1e, HIGH);    // Función para medir la longitud del pulso entrante del sensor 1
dista1 = 0.017 * tiempo1;         //Calculo de la distancia 1
//Serial.println("DistS1:   ");     // Display del monitor serial
//Serial.print(dista1);
//Serial.println(" cm");
digitalWrite(US2s, LOW);        // Estabilizacion sensor ultrasonico 2
delayMicroseconds(5);
digitalWrite(US2s, HIGH);         //Pulso ultrasonico en sensor 2
delayMicroseconds(10);
digitalWrite(US2s, LOW);
tiempo2 = pulseIn(US2e, HIGH);    // Función para medir la longitud del pulso entrante del sensor 2
dista2 = 0.017 * tiempo2;         //Calculo de la distancia 2
//Serial.println("DistS2:  ");       // Display del monitor serial
//Serial.print(dista2);
//Serial.println(" cm");
*/ 

  //si robot llega a interseccion, se manda llamar la funcion creada para este fin 
  if (interseccionDetectada()) {
    delay(250);                  //Se utiiza este dilay para que el robot se pueda colocar justo en medio de la interseccion, ya que sin el se para justo despues de detectar la interseccion 
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, LOW);
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, LOW);
    //aqui va la funcion que se desee implementar 
    giraDerecha();

  }

 //Si se encuentra un obstaculo el robot, aqui es donde se utilizan los sensores de proximidad, igual se debe descomentar este bloque para poderlos utilizar  
 /*
 if ((dista1 >= 0 && dista1 <= 10) || (dista2 >= 0 && dista2 <= 10)) {
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, LOW);
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, LOW);
    valSLA = 1;
    valSLB = 1;
    Serial.println("Parado");
  }
 */ 

  //si robot va derecho sobre linea blanca, hara que avance
  if (valSLA == 0 && valSLB == 0 && valSIA == 1 && valSIB == 1) {
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, HIGH);
    analogWrite(SAMA, 100);       
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, HIGH);
    analogWrite(SAMB, 100);
    Serial.println("Avanza");
  }
  
  //Se vuelven a leer valores de los sensores
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);
  
  //si robot va derecho pero se desvia hacia la izquierda, hara que el robot se mueva hacia la derecha para compensar
  if (valSLA == 0 && valSLB == 1 && valSIA == 1 && valSIB == 1) {
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, HIGH);
    analogWrite(SAMA, 80);       //170
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, HIGH);
    analogWrite(SAMB, 130);      //185
    Serial.println("Derecha");
  }
  
  //Se vuelven a leer valores de los sensores
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);
  
  //si robot va derecho pero se desvia hacia la derecha, hara que el robot se mueva hacia la izquierda para compensar
  if (valSLA == 1 && valSLB == 0 && valSIA == 1 && valSIB == 1) {
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, HIGH);
    analogWrite(SAMA, 130);
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, HIGH);
    analogWrite(SAMB, 80);
    Serial.println("Izquierda");
  }
  
  //si el robot se sale por completo del carril (ya se hacia la derecha o izquierda) se detendra
  if(valSLA==1&&valSLB==1){
    digitalWrite(E1MA,LOW);
    digitalWrite(E2MA,LOW);
    digitalWrite(E1MB,LOW);
    digitalWrite(E2MB,LOW);
    Serial.println("Detenido");
  }
}
