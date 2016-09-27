const unsigned int velocidadMotorDerecha = 150;
const unsigned int velocidadMotorIzquierda = 150;

void setup()
{
	// inicializando comunicación serial a 9600 bauds
	Serial.begin(9600);

	// esperando que el microprocesador se inicialice
	while(Serial.available() < 1);
	
	// mandando código de confirmación de encendido, código "S"
  Serial.write(Serial.read());
}

void loop()
{
	// operar sólo si se recibe un comando por parte del microprocesador
	if(Serial.available() > 0)
	{
		// leer y saber qué comando procesar
    switch(Serial.read())
		{
			// giro a la derecha 90 grados
			case 70:
			{
				moverDerechaAtras(velocidadMotorDerecha);
				moverIzquierdaAdelante(velocidadMotorIzquierda);
				while(!interseccionDetectada());
				Serial.write("0");
			}
			break;

			// giro a la izquierda 90 grados
			case 71:
			{
				moverDerechaAdelante(velocidadMotorDerecha);
				moverIzquierdaAtras(velocidadMotorIzquierda);
				while(!interseccionDetectada());
				Serial.write("0");
			}
			break;

			// avanzar al frente una intersección
			case 72:
			{
				while(!interseccionDetectada() && !obstaculoDetectado())
				{
					moverDerechaAdelante(velocidadMotorDerecha);
					moverIzquierdaAdelante(velocidadMotorIzquierda);
					sigueLineas(velocidadMotorIzquierda, velocidadMotorDerecha);
				}
				if(obstaculoDetectado())
					Serial.write("1");
				else
					Serial.write("0");
			}
			break;

			// levantar rack
			case 73:
			{
				Serial.write("0");
			}
			break;

			// bajar rack
			case 74:
			{
				Serial.write("0");
			}
			break;

			// código en caso de no recibir un comando correcto
			default:
				Serial.write("E");
		}
	}
}
