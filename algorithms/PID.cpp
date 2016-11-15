#ifdef PID_H_
#else
#define PID_H_

class PID
{
public:
	int left = 0, right = 0;
	PID(int velocidad = 150, float k_p = 20.0f, float k_i = 10000.0f, float k_d = 1.5f) :
        k_p(k_p),
        k_i(k_i),
        k_d(k_d),
        velocidad(velocidad)
        {};

	~PID(){};

    void reset(){
        error_integrado = 0;
        ultimo_proporcoional = 0; 
    };

	void add_reading(unsigned int posicion){
    	int resultado_pid;
        /************************************/
        // Valor máximo para el error: 2000 //
        /************************************/
        int proporcion_error = ((int)posicion) - 2000;                      //PROPORCIONAL    -> ERROR INSTANTANEO
        int derivativo_error = proporcion_error - ultimo_proporcoional;     //DIFERENCIAL     -> ERROR ESPERADO
        error_integrado += proporcion_error;                                       //INTEGRAL        -> ERROR ACUMULADO
        // Incremento instantáneo      = +-4000
        // Incremento a controlador    = 10 máximo, multiplicador = 0.2
        //
        // Incremento diferenciado     = +-4000
        // Incremento a controlador    = multiplicador = 1

        // Inremento por integral = ilimitado, restringido solamente por
        // velocidad de lectura. Dado que está seteada para 1000, que es el
        // valor por defecto es  400 microsegundos, y el incremento de este
        // puede ser de a unidades de 2000 en promedio, la idea es que en
        // aproximadamente 4000 microsegundos de error, incremente en 1 punto
        // el diferencial. El punto máximo de error por diferenciación
        // se alcanza a los 100 milisegundos a una velocidad media.
        // Incremento a controlador    = multiplicador = 0.00002
        ultimo_proporcoional = proporcion_error;                            //guarda el último valor para ser usado en el siguiente cálculo

        resultado_pid = (proporcion_error/k_p) + (error_integrado/k_i) + (derivativo_error*k_d);

        // resultado_pid *= 10;
        // resultado_pid /= 12; 

        if(resultado_pid > velocidad)
            resultado_pid = velocidad;
        if(resultado_pid < -velocidad)
            resultado_pid = -velocidad;

        if(resultado_pid < 0){
        	this->left = velocidad + resultado_pid;
        	this->right = velocidad;
        }
        else{
        	this->left = velocidad;
        	this->right = velocidad - resultado_pid;
        }
        delay_ms(1);
	}

private:
	int k_p, k_i, k_d;
	int velocidad;
	int error_integrado = 0, ultimo_proporcoional = 0;
};

#endif