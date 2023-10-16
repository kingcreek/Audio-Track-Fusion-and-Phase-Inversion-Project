/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imurugar <imurugar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:52:14 by imurugar          #+#    #+#             */
/*   Updated: 2023/10/16 11:06:29 by imurugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <sndfile.h>

int main() {
    // Need change, input files
    const char* archivoOriginal = "musica_original.wav";
    const char* archivoInverso2 = "inverso2.wav";

    // Open original file
    SF_INFO infoOriginal;
    SNDFILE* archivoSF = sf_open(archivoOriginal, SFM_READ, &infoOriginal);
    if (!archivoSF) {
        std::cerr << "Error al abrir el archivo original." << std::endl;
        return 1;
    }

    // Check if file is stereo
    if (infoOriginal.channels != 2) {
        std::cerr << "El archivo original no es estéreo." << std::endl;
        sf_close(archivoSF);
        return 1;
    }

    // Vector to store channels
    std::vector<double> twoChannels(infoOriginal.channels * infoOriginal.frames);
    //std::vector<double> canalDerecho(infoOriginal.channels * infoOriginal.frames);

	std::cout << "input 1" << std::endl;
    std::cout << " frames: " << infoOriginal.frames << std::endl;
    std::cout << " samplerate: " << infoOriginal.samplerate << std::endl;
    std::cout << " channels: " << infoOriginal.channels << std::endl;
    std::cout << " format: " << infoOriginal.format << std::endl;
    // Read frame chanel
    sf_readf_double(archivoSF, twoChannels.data(), infoOriginal.frames);
    //sf_readf_double(archivoSF, canalDerecho.data(), infoOriginal.frames);

    // Close file
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
	
    // Read samples from second audio
    std::vector<double> audioInverso2(infoInverso2.channels * infoInverso2.frames);
	std::vector<double> audioInverso2inverted(infoInverso2.channels * infoInverso2.frames);
	
    sf_readf_double(archivoInverso2SF, audioInverso2.data(), infoInverso2.frames);
	sf_readf_double(archivoInverso2SF, audioInverso2inverted.data(), infoInverso2.frames);
    sf_close(archivoInverso2SF);

    // Reverse wave from second file
    for (std::size_t i = 0; i < audioInverso2inverted.size(); i++) {
        audioInverso2inverted[i] = -audioInverso2inverted[i];
    }

    // Merge two channels, currently i literaly combine both, maybe works better with interleave
    for (std::size_t i = 0; i < twoChannels.size(); i++) {
		if(i < audioInverso2.size() && i < audioInverso2inverted.size())
		{
			if (i % 2 == 0)
				twoChannels[i] += audioInverso2inverted[i];
			else
				twoChannels[i] += audioInverso2[i];
		}
    }

    // merge second to other channel
    // for (std::size_t i = 0; i < canalDerecho.size(); i++) {
	// 	if(!audioInverso2[i])
	// 		break;
    //     canalDerecho[i] += audioInverso2[i];
    // }

    ///////////////////////////
	//Save result in new .wav//
	///////////////////////////

	// Merge both channels in new one, i think i dont need if i merge directly in apropiate channel
    std::vector<double> canalCombinado(infoOriginal.channels * infoOriginal.frames);  // stereo size
    for (int i = 0; i < (infoOriginal.frames); i++) {
        canalCombinado[i] = twoChannels[i];  // left channel
        //canalCombinado[i * 2 + 1] = canalDerecho[i];  // right channel
    }

    // Guarda el resultado en un nuevo archivo
    const char* archivoSalida = "audio_resultado.wav";
    SF_INFO infoSalida = infoOriginal;
    infoSalida.channels = 2;  // Set number of channels in 2 (estéreo)

    SNDFILE* archivoSalidaSF = sf_open(archivoSalida, SFM_WRITE, &infoSalida);
    if (!archivoSalidaSF) {
        std::cerr << "Error al crear el archivo de salida." << std::endl;
        return 1;
    }

    sf_writef_double(archivoSalidaSF, canalCombinado.data(), infoOriginal.frames);
    sf_close(archivoSalidaSF);

    return 0;
}
