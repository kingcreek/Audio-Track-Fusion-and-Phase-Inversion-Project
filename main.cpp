/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imurugar <imurugar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:52:14 by imurugar          #+#    #+#             */
/*   Updated: 2023/10/16 10:59:58 by imurugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <sndfile.h>

int main() {
    // Definir el nombre de los archivos de audio originales y los inversos
    const char* archivoOriginal = "musica_original.wav";
    const char* archivoInverso2 = "inverso2.wav";

    // Abrir el archivo original
    SF_INFO infoOriginal;
    SNDFILE* archivoSF = sf_open(archivoOriginal, SFM_READ, &infoOriginal);
    if (!archivoSF) {
        std::cerr << "Error al abrir el archivo original." << std::endl;
        return 1;
    }

    // Verificar que el archivo original es estéreo
    if (infoOriginal.channels != 2) {
        std::cerr << "El archivo original no es estéreo." << std::endl;
        sf_close(archivoSF);
        return 1;
    }

    // Crear vectores para almacenar muestras de cada canal
    std::vector<double> twoChannels(infoOriginal.channels * infoOriginal.frames);
    //std::vector<double> canalDerecho(infoOriginal.channels * infoOriginal.frames);

	std::cout << "input 1" << std::endl;
    std::cout << " frames: " << infoOriginal.frames << std::endl;
    std::cout << " samplerate: " << infoOriginal.samplerate << std::endl;
    std::cout << " channels: " << infoOriginal.channels << std::endl;
    std::cout << " format: " << infoOriginal.format << std::endl;
    // Leer las muestras de los canales
    sf_readf_double(archivoSF, twoChannels.data(), infoOriginal.frames);
    //sf_readf_double(archivoSF, canalDerecho.data(), infoOriginal.frames);

    // Cerrar el archivo original
    sf_close(archivoSF);
	
	////////////////////
	//Open second file//
	////////////////////

    SF_INFO infoInverso2;
    SNDFILE* archivoInverso2SF = sf_open(archivoInverso2, SFM_READ, &infoInverso2);
    if (!archivoInverso2SF) {
        std::cerr << "Error al abrir el segundo archivo inverso." << std::endl;
        return 1;
    }
	
	std::cout << "input 2" << std::endl;
    std::cout << " frames: " << infoInverso2.frames << std::endl;
    std::cout << " samplerate: " << infoInverso2.samplerate << std::endl;
    std::cout << " channels: " << infoInverso2.channels << std::endl;
    std::cout << " format: " << infoInverso2.format << std::endl;
	
    // Leer las muestras del segundo audio inverso
    std::vector<double> audioInverso2(infoInverso2.channels * infoInverso2.frames);
	std::vector<double> audioInverso2inverted(infoInverso2.channels * infoInverso2.frames);
	
    sf_readf_double(archivoInverso2SF, audioInverso2.data(), infoInverso2.frames);
	sf_readf_double(archivoInverso2SF, audioInverso2inverted.data(), infoInverso2.frames);
    sf_close(archivoInverso2SF);

    // Invierte la onda de sonido del segundo audio inverso
    for (std::size_t i = 0; i < audioInverso2inverted.size(); i++) {
        audioInverso2inverted[i] = -audioInverso2inverted[i];
    }

    // Combina el segundo audio inverso invertido con el canal izquierdo
    for (std::size_t i = 0; i < twoChannels.size(); i++) {
		if(i < audioInverso2.size() && i < audioInverso2inverted.size())
		{
			if (i % 2 == 0)
				twoChannels[i] += audioInverso2inverted[i];
			else
				twoChannels[i] += audioInverso2[i];
		}
    }

    // Combina el segundo audio inverso (sin invertir) con el canal derecho
    // for (std::size_t i = 0; i < canalDerecho.size(); i++) {
	// 	if(!audioInverso2[i])
	// 		break;
    //     canalDerecho[i] += audioInverso2[i];
    // }

    ///////////////////////////
	//Save result in new .wav//
	///////////////////////////

	// Combina los canales izquierdo y derecho en un nuevo vector
    std::vector<double> canalCombinado(infoOriginal.channels * infoOriginal.frames);  // El tamaño se duplica para el estéreo
    for (int i = 0; i < (infoOriginal.frames); i++) {
        canalCombinado[i] = twoChannels[i];  // Canal izquierdo
        //canalCombinado[i * 2 + 1] = canalDerecho[i];  // Canal derecho
    }

    // Guarda el resultado en un nuevo archivo
    const char* archivoSalida = "audio_resultado.wav";
    SF_INFO infoSalida = infoOriginal;
    infoSalida.channels = 2;  // Establece el número de canales a 2 (estéreo)

    SNDFILE* archivoSalidaSF = sf_open(archivoSalida, SFM_WRITE, &infoSalida);
    if (!archivoSalidaSF) {
        std::cerr << "Error al crear el archivo de salida." << std::endl;
        return 1;
    }

    sf_writef_double(archivoSalidaSF, canalCombinado.data(), infoOriginal.frames);
    sf_close(archivoSalidaSF);

    return 0;
}
