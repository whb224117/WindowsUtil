#pragma once
#include "..\Common\Common.h"
class EscapeState :
	public State
{
public:

	EscapeState()
	{
	}

	~EscapeState()
	{
	}
	virtual  const State*  Next(const shared_ptr<Instruction>& inst) const override;
};

