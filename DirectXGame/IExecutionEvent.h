#pragma once
class IExecutionEvent
{
public:
	virtual void onExecute(int index) = 0;
};

