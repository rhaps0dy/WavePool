#ifndef _readme_h_
#define _readme_h_

#include <cstdio>

void showREADME()
{
	const char readme[] = "Controles\n"
		"---------\n"
		"A               - A�adir emisor bajo el cursor\n"
		"D               - Borrar emisor seleccionado\n"
		"Click derecho   - Seleccionar emisor / deseleccionar emisor si se clica lejos.\n"
		"Click izquierdo - Mover emisor seleccionado\n"
		"Q / Z           - Longitud de onda + / -\n"
		"W / X           - Amplitud + / - (puede haber overflow de color)\n"
		"E / C           - Velocidad de propagaci�n + / -\n"
		"P               - Pausar / continuar tiempo del emisor seleccionado\n";
	puts(readme);
}
#endif 
