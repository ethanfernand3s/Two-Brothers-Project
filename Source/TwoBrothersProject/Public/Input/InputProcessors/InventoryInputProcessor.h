// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"

/**
 * 
 */
class FInventoryInputProcessor : public IInputProcessor
{
public:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	FInventoryInputProcessor() = default;
	virtual ~FInventoryInputProcessor() override = default; // polymorphic type, so virtual

	// Called before Slate dispatches to widgets
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual const TCHAR* GetDebugName() const override { return TEXT("InventoryInputProcessor"); }
};

