#include "MonsterHealthWidget.h"
#include "Components/ProgressBar.h"

void UMonsterHealthWidget::UpdateHealthBar(float HealthPercent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(HealthPercent);
	}
}