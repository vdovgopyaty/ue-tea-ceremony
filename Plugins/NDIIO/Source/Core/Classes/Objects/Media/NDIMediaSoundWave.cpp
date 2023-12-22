/*
	Copyright (C) 2023 Vizrt NDI AB. All rights reserved.

	This file and it's use within a Product is bound by the terms of NDI SDK license that was provided
	as part of the NDI SDK. For more information, please review the license and the NDI SDK documentation.
*/

#include <Objects/Media/NDIMediaSoundWave.h>
#include <Objects/Media/NDIMediaReceiver.h>


UNDIMediaSoundWave::UNDIMediaSoundWave(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set the Default Values for this object
	this->bLooping = false;
	this->NumChannels = 1;
	this->SampleRate = 48000;

	this->Duration = INDEFINITELY_LOOPING_DURATION;
}

/**
	Set the Media Source of this object, so that when this object is called to 'GeneratePCMData' by the engine
	we can request the media source to provide the pcm data from the current connected source
*/
void UNDIMediaSoundWave::SetConnectionSource(UNDIMediaReceiver* InMediaSource)
{
	// Ensure there is no thread contention for generating pcm data from the connection source
	FScopeLock Lock(&SyncContext);

	// Do we have a media source object to work with
	if (this->MediaSource != nullptr)
	{
		// Are we already registered with the incoming media source object
		if (this->MediaSource != InMediaSource)
		{
			// It doesn't look like we are registered with the incoming, make sure
			// to unregistered with the previous source
			this->MediaSource->UnregisterAudioWave(this);
		}
	}

	// Ensure we have a reference to the media source object
	this->MediaSource = InMediaSource;
}

/**
	Called by the engine to generate pcm data to be 'heard' by audio listener objects
*/
int32 UNDIMediaSoundWave::OnGeneratePCMAudio(TArray<uint8>& OutAudio, int32 NumSamples)
{
	// Ensure there is no thread contention for generating pcm data from the connection source
	FScopeLock Lock(&SyncContext);

	// set the default value, in case we have no connection source
	int32 samples_generated = 0;

	OutAudio.Reset();
	OutAudio.AddZeroed(NumSamples * sizeof(int16));

	// check the connection source and continue
	if (this->MediaSource != nullptr)
	{
		samples_generated = MediaSource->GeneratePCMData(this, OutAudio.GetData(), NumSamples);
	}

	// return to the engine the number of samples actually generated
	return samples_generated;
}

bool UNDIMediaSoundWave::IsReadyForFinishDestroy()
{
	// Ensure that there is no thread contention for generating data
	FScopeLock Lock(&SyncContext);

	return USoundWaveProcedural::IsReadyForFinishDestroy();
}
