#include "client.h"
#include <readline/readline.h>

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	config = iniciar_config();
	if (config == NULL) {
		log_error(logger, "Error leyendo la config.");
		abort();
	}

	valor = config_get_string_value (config, "CLAVE");
	ip = config_get_string_value (config, "IP");
	puerto = config_get_string_value (config, "PUERTO");

	log_info(logger, valor);

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conexion creada");

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);	

	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create ("./tp0.log", "Logger tp0", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("./cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	leido = readline("> ");
	while(leido[0] != '\0'){
		log_info(logger, leido);
		leido = readline("> ");
	}
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();

	leido = readline("> ");
	while(leido[0] != '\0'){
		agregar_a_paquete(paquete, leido, string_length(leido) + 1);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
	free(leido);
	liberar_conexion(conexion);

	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
}
