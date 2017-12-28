#ifndef IRUNTIMESTATE_H
#define IRUNTIMESTATE_H

#include "ITagBase.h"
class cVariablesProvider;

class ISlotState
{
public:
    virtual ~ISlotState(){}
    virtual QString expBoardName() const = 0;
    virtual QMap<int, ITagBase*> channelInfo() const  = 0;
};

class IRuntimeState
{
public:
    virtual ~IRuntimeState(){}
    virtual ITagBase* baseInfo() const  = 0;
    virtual ITagBase* variablesInfo() const  = 0;
    virtual QMap<int, ISlotState*>  inputInfo() = 0;
    virtual QMap<int, ISlotState*> outputInfo() = 0;
    virtual cVariablesProvider* provider() = 0;
};

#endif // IRUNTIMESTATE_H

