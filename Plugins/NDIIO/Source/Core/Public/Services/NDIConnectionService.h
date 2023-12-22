/*
	Copyright (C) 2023 Vizrt NDI AB. All rights reserved.

	This file and it's use within a Product is bound by the terms of NDI SDK license that was provided
	as part of the NDI SDK. For more information, please review the license and the NDI SDK documentation.
*/

#pragma once

#include <CoreMinimal.h>
#include <Engine/TextureRenderTarget2D.h>
#include <AudioDevice.h>
#include <Misc/EngineVersionComparison.h>
#if (ENGINE_MAJOR_VERSION > 5) || ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 3))
#include <ISubmixBufferListener.h>
#endif
#include <Widgets/SWindow.h>

DECLARE_EVENT_OneParam(FNDICoreDelegates, FNDIConnectionServiceSendVideoEvent, int64)
DECLARE_EVENT_SixParams(FNDICoreDelegates, FNDIConnectionServiceSendAudioEvent, int64, float*, int32, int32, const int32, double)

/**
	A service which runs and triggers updates for interested parties to be notified of
	Audio and Video Frame events
*/
class NDIIO_API FNDIConnectionService final : public ISubmixBufferListener
{
public:
	static FNDIConnectionServiceSendVideoEvent EventOnSendVideoFrame;
	static FNDIConnectionServiceSendAudioEvent EventOnSendAudioFrame;

public:
	/**
		Constructs a new instance of this object
	*/
	FNDIConnectionService();

	// Begin the service
	bool Start();

	// Stop the service
	void Shutdown();

	bool BeginBroadcastingActiveViewport();
	void StopBroadcastingActiveViewport();

	bool IsRunningInPIE() const
	{
		return bIsInPIEMode;
	}

private:
	// Handler for when the render thread frame has ended
	void OnEndRenderFrame();

	void OnFEngineLoopInitComplete();

	// Handler for when the active viewport back buffer is about to be resized
	void OnActiveViewportBackbufferPreResize(void* Backbuffer);

	// Handler for when the back buffer is read to present to the end user
	void OnActiveViewportBackbufferReadyToPresent(SWindow& Window, const FTexture2DRHIRef& Backbuffer);

	FTextureResource* GetVideoTextureResource() const;

	virtual void OnNewSubmixBuffer(const USoundSubmix* OwningSubmix, float* AudioData, int32 NumSamples, int32 NumChannels, const int32 SampleRate, double AudioClock) override final;


private:
	bool bIsInitialized = false;
	bool bIsAudioInitialized = false;
	bool bIsBroadcastingActiveViewport = false;
	bool bIsInPIEMode = false;

	FCriticalSection AudioSyncContext;
	FCriticalSection RenderSyncContext;

	UTextureRenderTarget2D* VideoTexture = nullptr;
	class UNDIMediaSender* ActiveViewportSender = nullptr;
};