/*
	Copyright (C) 2023 Vizrt NDI AB. All rights reserved.

	This file and it's use within a Product is bound by the terms of NDI SDK license that was provided
	as part of the NDI SDK. For more information, please review the license and the NDI SDK documentation.
*/

#pragma once

#include <CoreMinimal.h>

#include <Modules/ModuleManager.h>
#include <Styling/SlateStyle.h>

class NDIIOEDITOR_API FNDIIOEditorModule : public IModuleInterface
{
	public:
		virtual void StartupModule() override;
		virtual void ShutdownModule() override;

	private:
		TUniquePtr<FSlateStyleSet> StyleInstance;
};

IMPLEMENT_MODULE(FNDIIOEditorModule, NDIIOEditor)