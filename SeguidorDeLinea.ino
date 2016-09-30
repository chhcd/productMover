//valores de sensores en cero
int valSLA = 0;
int valSLB = 0;
int valSIA = 0;
int valSIB = 0;

//pines a utilizar
int SLA = 2;     //Sensor de deteccion de linea A en pin 1
int SLB = 4;     //Sensor de deteccion de linea B en pin 2
int SIA = 14;    //Sensor de interseccion A en pin A0
int SIB = 15;    //Sensor de interseccion B en pin A1
int US1s = 5;    //Salida sensor ultrasonica 1 en pin 5    //Va conectado a TRIG
int US1e = 6;    //Entrada sensor ultrasonico 1 en pin 6   //Va a ECHO
int US2s = A2;   //Salida sensor ultrasonico 2 en pin A2   //Va conectado a TRIG
int US2e = A3;   //Entrada sensor ultrasonico 2 en pin A3  //Va a ECHO
int E1MA = 7;    //Entrada 1 Motor A en pin 7
int E2MA = 8;    //Entrada 2 Motor A en pin 8
int SAMA = 9;    //Salida analogica Motor A en pin 9
int E1MB = 10;   //Entrada 1 Motor B en pin 10
int E2MB = 12;   //Entrada 2 Motor b en pin 12
int SAMB = 11;   //Salida analogica Motor B en pin 11
const int actuador=3; //Se define pin del arduino del actuador
const int pinServo=A4; //Se define el pin del servo

//set-up sensor optico
unsigned int dista1;
unsigned int dista2;
int valor;
int tiempo1;
int tiempo2;

//set-up para el servo
#include <Servo.h> //se incluye header Arduino
#define origen 70 //Se define el angulo inicial
#define desplazado 90 //Se define el angulo al que debe cerrar
Servo seguro; //definicion de servo del "seguro"


//funciones
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

int avanzaMotores(){
  digitalWrite(E1MA, LOW);
  digitalWrite(E2MA, HIGH);
  analogWrite(SAMA, 42);       
  digitalWrite(E1MB, LOW);
  digitalWrite(E2MB, HIGH);
  analogWrite(SAMB, 44);
}

int mueveMotoresD(){
  digitalWrite(E1MA, HIGH);
  digitalWrite(E2MA, LOW);
  analogWrite(SAMA, 40);       
  digitalWrite(E1MB, LOW);
  digitalWrite(E2MB, HIGH);
  analogWrite(SAMB, 44);
}

int mueveMotoresI(){
  digitalWrite(E1MA, LOW);
  digitalWrite(E2MA, HIGH);
  analogWrite(SAMA, 40);       
  digitalWrite(E1MB, HIGH);
  digitalWrite(E2MB, LOW);
  analogWrite(SAMB, 44); 
}

int giraDerecha() {
  int contGD = 0;    //Contador para el giro hacia la derecha
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  if (valSLA == 0 && valSLB == 0)
    contGD = 1;
  if (valSLA == 0 && valSLB == 1)
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
      delay(700);
      break;

    case 2:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while(valSLA == 0 && valSLB == 1){
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD(); 
        //erial.println("1 Etapa!");
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
        //Serial.println(" Etapa!");
      }
      avanzaMotores();
      delay(700);
      break;

    case 3:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while (valSLA == 1 && valSLB == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("0 Etapa!");
      }
      while ((valSLA == 0 || valSLA == 1) && valSLB == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("Primera Etapa!");
      }
      avanzaMotores();
      delay(700);
      break;
      
    default:
      break;
  }
}

int giraIzquierda(){
  int contGI = 0;    //Contador para el giro hacia la izquierda
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  if (valSLA == 0 && valSLB == 0)
    contGI = 1;
  if (valSLB == 0 && valSLA == 1)
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
        //Serial.println("0 Etapa!");
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("Primera Etapa!");
      }
      avanzaMotores();
      delay(700);
      break;

    case 2:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while(valSLB == 0 && valSLA == 1){
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI(); 
        //Serial.println("0 Etapa!")
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("1 Etapa!");
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresD();
        //Serial.println("2 Etapa!");
      }
      avanzaMotores();
      delay(700);
      break;

    case 3:
      valSLA = digitalRead(SLA);
      valSLB = digitalRead(SLB);
      while (valSLB == 1 && valSLA == 0) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("0 Etapa!");
      }
      while ((valSLB == 0 || valSLB == 1) && valSLA == 1) {
        valSLA = digitalRead(SLA);
        valSLB = digitalRead(SLB);
        mueveMotoresI();
        //Serial.println("Primera Etapa!");
      }
      avanzaMotores();
      delay(700);
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
  pinMode(US2s, OUTPUT);    //Salida pulso ultrasonico 2 trigger
  pinMode(US2e, INPUT);    //Entrada rebote ultrasonico 2 echo
  pinMode(E1MA, OUTPUT);    //Eentrada 1 Motor A
  pinMode(E2MA, OUTPUT);    //Entrada 2 Motor A
  pinMode(SAMA, OUTPUT);    //Salida analogica Motor A
  pinMode(E1MB, OUTPUT);    //Eentrada 1 Motor B
  pinMode(E2MB, OUTPUT);    //Entrada 2 Motor B
  pinMode(SAMB, OUTPUT);    //Salida analogica Motor B
  /*pinMode(actuador, OUTPUT);
  pinMode(pinServo, OUTPUT);
  seguro.attach(pinServo);  // Pin del servomotor*/

}

void loop() {
  //Serial.println("ciclo");
  //se cambian valores de los sensores de acuerdo al valor leido
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);

  
//  Serial.print("medicion A1  ");
//  Serial.println(valSLA);
//   Serial.print("medicion B1  ");
//   Serial.println(valSLB);
//   Serial.print("medicion SIA  ");
//   Serial.println(valSIA);
//   Serial.print("medicion SIB  ");
//   Serial.println(valSIB);

  digitalWrite(US1s, LOW);          // Estabilizacion sensor ultrasonico 1
  delayMicroseconds(5);
  digitalWrite(US1s, HIGH);         //Pulso ultrasonico en sensor 1
  delayMicroseconds(10);
  digitalWrite(US1s, LOW);
  tiempo1 = pulseIn(US1e, HIGH);    // Función para medir la longitud del pulso entrante del sensor 1
  dista1 = 0.017 * tiempo1;         //Calculo de la distancia 1
  /*Serial.println("DistS1:   ");     // Display del monitor serial
  Serial.print(dista1);
  Serial.println(" cm");*/
  digitalWrite(US2s, LOW);        // Estabilizacion sensor ultrasonico 2
  delayMicroseconds(5);
  digitalWrite(US2s, HIGH);         //Pulso ultrasonico en sensor 2
  delayMicroseconds(10);
  digitalWrite(US2s, LOW);
  tiempo2 = pulseIn(US2e, HIGH);    // Función para medir la longitud del pulso entrante del sensor 2
  dista2 = 0.017 * tiempo2;         //Calculo de la distancia 2
  /*Serial.println("DistS2:  ");       // Display del monitor serial
  Serial.print(dista2);
  Serial.println(" cm");
  //delay(1000);*/

  //si robot llega a interseccion por el momento
  if (interseccionDetectada()) {
    delay(250);
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, LOW);
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, LOW);
    giraDerecha();

  }

  //Si se encuentra un obstaculo
  if ((dista1 >= 0 && dista1 <= 10) || (dista2 >= 0 && dista2 <= 10)) {
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, LOW);
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, LOW);
    valSLA = 1;
    valSLB = 1;
    //Serial.println("Parado");
  }

  //si robot va derecho sobre linea blanca
  if (valSLA == 0 && valSLB == 0 && valSIA == 1 && valSIB == 1) {
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, HIGH);
    analogWrite(SAMA, 40);       //170
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, HIGH);
    analogWrite(SAMB, 40);
    //Serial.println("Avanza");
  }
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);
  //si robot se desvia hacia la izquierda
  if (valSLA == 0 && valSLB == 1 && valSIA == 1 && valSIB == 1) {
    //Se mueve hacia la derecha
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, HIGH);
    analogWrite(SAMA, 30);       //170
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, HIGH);
    analogWrite(SAMB, 150);      //185
    Serial.println("Derecha");
  }
  valSLA = digitalRead(SLA);
  valSLB = digitalRead(SLB);
  valSIA = digitalRead(SIA);
  valSIB = digitalRead(SIB);
  //si robot se desvia hacia la derecha
  if (valSLA == 1 && valSLB == 0 && valSIA == 1 && valSIB == 1) {
    //Se mueve hacia la izquierda
    digitalWrite(E1MA, LOW);
    digitalWrite(E2MA, HIGH);
    analogWrite(SAMA, 130);
    digitalWrite(E1MB, LOW);
    digitalWrite(E2MB, HIGH);
    analogWrite(SAMB, 30);
    Serial.println("Izquierda");
  }
  
  if(valSLA==1&&valSLB==1){
    digitalWrite(E1MA,LOW);
    digitalWrite(E2MA,LOW);
    digitalWrite(E1MB,LOW);
    digitalWrite(E2MB,LOW);
    //Serial.println("Detenido");
  }
  //delay(1000);





}
