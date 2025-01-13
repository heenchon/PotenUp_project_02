#include "StatusProgressBar.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

// 초창기 세팅을 통해 컴파일 과정에서 확인 가능하게 설정
void UStatusProgressBar::NativePreConstruct()
{
	Thumbnail->SetBrush(ThumbnailOptions);
	StatusBar->SetWidgetStyle(StatusBarStyleOptions);
}

void UStatusProgressBar::SetPercent(const float NewValue)
{
	StatusBar->SetPercent(NewValue);
}
