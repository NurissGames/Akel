// This file is a part of Akel
// Authors : @kbz_8
// Created : 28/03/2023
// Updated : 28/03/2023

#define DR_FLAC_IMPLEMENTATION
#include <dr/dr_flac.h>
#define DR_MP3_IMPLEMENTATION
#include <dr/dr_mp3.h>
#define DR_WAV_IMPLEMENTATION
#include <dr/dr_wav.h>

#include <Audio/buffer.h>
#include <Core/log.h>

namespace Ak
{
	AudioBuffer::AudioBuffer(std::filesystem::path file)
	{
		if(alcGetCurrentContext() == nullptr)
		{
			Core::log::report(ERROR, "Audio Buffer : you need to add Ak::AudioCompent to your application to use audio");
			return;
		}
		std::filesystem::path extension = file.extension();

		if(!std::filesystem::exists(file))
			Core::log::report(ERROR, "Audio Buffer : file doesn't exists '%s'", file.c_str());

		if(extension == ".mp3")
			loadMP3(std::move(file));
		else if(extension == ".wav")
			loadWAV(std::move(file));
		else if(extension == ".flac")
			loadFlac(std::move(file));
		else if(extension == ".ogg")
			loadOGG(std::move(file));
		else
			Core::log::report(ERROR, "Audio Buffer : unknown audio file format '%s'", file.c_str());
	}

	void AudioBuffer::loadMP3(std::filesystem::path file)
	{
		std::string data = readAudioFile(file);

		drmp3_config config;
		drmp3_uint64 totalPCMFrameCount;
		auto sampleData = drmp3_open_memory_and_read_pcm_frames_s16(data.data(), data.size(), &config, &totalPCMFrameCount, nullptr);
		if(!sampleData)
		{
			Core::log::report(ERROR, "Audio Buffer : cannot load '%s', cannot load samples", file.c_str());
			return;
		}

		alGenBuffers(1, &_buffer);
		alBufferData(_buffer, (config.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sampleData, totalPCMFrameCount, config.sampleRate);

		drmp3_free(sampleData, nullptr);
	}

	void AudioBuffer::loadFlac(std::filesystem::path file)
	{
		// TODO
	}

	void AudioBuffer::loadWAV(std::filesystem::path file)
	{
		std::string data = readAudioFile(file);

		uint32_t channels;
		uint32_t sampleRate;
		drwav_uint64 totalPCMFrameCount;
		auto sampleData = drwav_open_memory_and_read_pcm_frames_s16(data.data(), data.size(), &channels, &sampleRate, &totalPCMFrameCount, nullptr);
		if(!sampleData)
		{
			Core::log::report(ERROR, "Audio Buffer : cannot load '%s', cannot load samples", file.c_str());
			return;
		}

		alGenBuffers(1, &_buffer);
		alBufferData(_buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, sampleData, totalPCMFrameCount, sampleRate);

		drwav_free(sampleData, nullptr);
	}

	void AudioBuffer::loadOGG(std::filesystem::path file)
	{
		std::string fdata = readAudioFile(file);

		int32_t channels;
		int32_t samplesPerSec;
		int16_t *data;
		auto size = stb_vorbis_decode_memory(reinterpret_cast<uint8_t*>(fdata.data()), static_cast<uint32_t>(fdata.size()), &channels, &samplesPerSec, &data);

		if(size == -1)
		{
			Core::log::report(ERROR, "Audio Buffer : cannot load '%s', cannot find size", file.c_str());
			return;
		}

		alGenBuffers(1, &_buffer);
		alBufferData(_buffer, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, samplesPerSec);

		free(data);
	}

	std::string AudioBuffer::readAudioFile(std::filesystem::path file)
	{
		std::ifstream is(std::move(file));
		std::stringstream buffer;
		buffer << is.rdbuf();
		return buffer.str();
	}

	void AudioBuffer::setBuffer(ALuint buffer) noexcept
	{
		if(alcGetCurrentContext() == nullptr)
			return;
		if(_buffer)
			alDeleteBuffers(1, &_buffer);
		_buffer = buffer;
	}

	void AudioBuffer::deleteBuffer() noexcept
	{
		if(alcGetCurrentContext() == nullptr)
			return;
		if(_buffer)
			alDeleteBuffers(1, &_buffer);
	}
}
