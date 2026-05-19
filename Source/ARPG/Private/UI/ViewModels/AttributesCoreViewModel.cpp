#include "UI/ViewModels/AttributesCoreViewModel.h"

void UAttributesCoreViewModel::Init(UAttributeCore* Attribute)
{
	if (!Attribute) return;

	CoreAttribute = Attribute;

	Current = CoreAttribute->Current;
	Max = CoreAttribute->Max;
	Buff = CoreAttribute->Buff;

	CoreAttribute->OnValuesChanged.AddDynamic(this, &UAttributesCoreViewModel::UpdateValues);
}

void UAttributesCoreViewModel::UpdateValues(float NewCurrent, float NewMax, float NewBuff)
{
	Current = NewCurrent;
	Max = NewMax;
	Buff = NewBuff;

	OnViewModelChanged.Broadcast(Current, Max, Buff);
}
