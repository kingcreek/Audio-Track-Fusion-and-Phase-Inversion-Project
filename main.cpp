/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imurugar <imurugar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:52:14 by imurugar          #+#    #+#             */
/*   Updated: 2023/10/16 17:15:18 by imurugar         ###   ########.fr       */
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

	std::cout << " input 1 " << std::endl;
    std::cout << " frames: " << infoOriginal.frames << std::endl;
    std::cout << " samplerate: " << infoOriginal.samplerate << std::endl;
    std::cout << " channels: " << infoOriginal.channels << std::endl;
    std::cout << " format: " << infoOriginal.format << std::endl;
    // Read frame chanel
    sf_readf_double(archivoSF, twoChannels.data(), infoOriginal.channels * infoOriginal.frames);

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
	
	std::cout << " input 2 " << std::endl;
    std::cout << " frames: " << infoInverso2.frames << std::endl;
    std::cout << " samplerate: " << infoInverso2.samplerate << std::endl;
    std::cout << " channels: " << infoInverso2.channels << std::endl;
    std::cout << " format: " << infoInverso2.format << std::endl;
	
	
	
    // Read samples from second audio
    std::vector<double> secondSound(infoInverso2.channels * infoInverso2.frames);
	
    sf_readf_double(archivoInverso2SF, secondSound.data(), infoInverso2.channels * infoInverso2.frames);
    sf_close(archivoInverso2SF);

	double *audioOut = new double[infoInverso2.frames];
	if (infoInverso2.channels == 2)
	{
    	for(int i = 0; i < infoInverso2.frames; i++)
    	{
        	audioOut[i] = 0;
        	for(int j = 0; j < infoInverso2.channels; j++)
            	audioOut[i] += secondSound[i*infoInverso2.channels + j];
        	audioOut[i] /= infoInverso2.channels;
    	}
	}
	else{
		for(int i = 0; i < infoInverso2.frames; i++)
			audioOut[i] += secondSound[i];
	}
	
	
	double factorVolumeReduccion = 0.5;
	double factorVolumeReduccion1 = 0.6;
	int k = 0;
	for (std::size_t i = 0; i < twoChannels.size(); i++) {
		if(i < (std::size_t)infoInverso2.frames)
		{
			twoChannels[i] *= factorVolumeReduccion1;
			if (i % 2 == 0)
				twoChannels[i] += audioOut[k];
			else
				twoChannels[i] += -(audioOut[k++]);
			twoChannels[i] *= factorVolumeReduccion;
		}
    }
	
    // Save audio in new file
    const char* archivoSalida = "audio_resultado.wav";
    SF_INFO infoSalida = infoOriginal;
    infoSalida.channels = 2;  // Set number of channels in 2 (stereo)

    SNDFILE* archivoSalidaSF = sf_open(archivoSalida, SFM_WRITE, &infoSalida);
    if (!archivoSalidaSF) {
        std::cerr << "Error al crear el archivo de salida." << std::endl;
        return 1;
    }

    sf_writef_double(archivoSalidaSF, twoChannels.data(), infoOriginal.frames);
    sf_close(archivoSalidaSF);

    return 0;
}
