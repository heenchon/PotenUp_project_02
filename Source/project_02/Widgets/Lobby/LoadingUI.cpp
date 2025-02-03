#include "LoadingUI.h"

#include "Animation/WidgetAnimation.h"


void ULoadingUI::NativeConstruct()
{
	EndDelegate.BindDynamic(this, &ThisClass::OnFinishFadeOut);
	BindToAnimationFinished(FadeOut, EndDelegate);
}

void ULoadingUI::OnFinishFadeOut()
{
	RemoveFromParent();
}

void ULoadingUI::PlayStart()
{
	PlayAnimation(FadeIn);
}


void ULoadingUI::PlayEnd()
{
	PlayAnimation(FadeOut);
}

