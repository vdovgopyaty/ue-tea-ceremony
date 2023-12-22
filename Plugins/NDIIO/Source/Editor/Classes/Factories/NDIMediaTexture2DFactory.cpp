/*
	Copyright (C) 2023 Vizrt NDI AB. All rights reserved.

	This file and it's use within a Product is bound by the terms of NDI SDK license that was provided
	as part of the NDI SDK. For more information, please review the license and the NDI SDK documentation.
*/

#include <Factories/NDIMediaTexture2DFactory.h>

#include <AssetTypeCategories.h>
#include <Objects/Media/NDIMediaTexture2D.h>
#include <Misc/EngineVersionComparison.h>

#define LOCTEXT_NAMESPACE "NDIIOEditorMediaSoundWaveFactory"

UNDIMediaTexture2DFactory::UNDIMediaTexture2DFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	this->bCreateNew = true;
	this->bEditAfterNew = true;

	this->SupportedClass = UNDIMediaTexture2D::StaticClass();
}

FText UNDIMediaTexture2DFactory::GetDisplayName() const { return LOCTEXT("NDIMediaTexture2DFactoryDisplayName", "NDI Media Texture2D"); }

#if ENGINE_MAJOR_VERSION == 4
uint32 UNDIMediaTexture2DFactory::GetMenuCategories() const { return EAssetTypeCategories::MaterialsAndTextures; }
#else
uint32 UNDIMediaTexture2DFactory::GetMenuCategories() const { return EAssetTypeCategories::Textures; }
#endif

UObject* UNDIMediaTexture2DFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (UNDIMediaTexture2D* Resource = NewObject<UNDIMediaTexture2D>(InParent, InName, Flags | RF_Transactional))
	{
		Resource->UpdateResource();
		return Resource;
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE