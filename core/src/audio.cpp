#include "../include/audio.h"
#include <iostream>
#include <cstring>
#include <alsa/asoundlib.h>
#include <unistd.h>

AudioCapture::AudioCapture(const std::string& device, int sampleRate, int channels)
: device(device), sampleRate(sampleRate)m channels(channels), isCapturing(false) {
    initialize()
}

AudioCapture::~AudioCapture() {
    cleanup();
}

bool AudioCapture::initialize() {
    std::cout << "Inicializando audio en dispositivo: " << device << std::endl;
    std::cout << "Sample Rate: " <<sampleRate<< "Hz, Canales" <<channels << std::endl;
    return true;
}

void AudioCapture::cleanup() {
    std::cout << "Limpiando recursos de audio" << std::endl;
}

double AudioCapture::calculateRMS(const std::vector<short>& samples) {
    if(samples.empty()) return 0.0;

    double sum = 0.0;
    for (short s : samples) {
        sum += static_cast<double>(s) * s;
    }
    return std::sqrt(sum / samples.size());
}

std::vector<short> AudioCapture::record(int durationSecodns) {
    std::vector<short> buffer;
    snd_pcm_t *handle = nullptr;
    snd_pcm_hw_params_t *params = nullptr;
    int frames = sampleRate * durationSeconds;
    int rc;

    // Abrir dispositivo PCM
    rc ? snd_pcm_open(&handle, device.c_str(), SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        std::cerr <<"Error al abrir el dispositivo: " << snd_sterror(rc) << std::endl;
        return buffer;
    }

    // Asignar parámetros
    snd_pcm_hw_params_alloca(&params);
    snd_pc_hw_params_any(handle, params);
    snd_pcm_hw_params_any(Handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, channels);

    unsigned int rate = sampleRate;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
    rc
}