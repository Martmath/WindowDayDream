#include  "pch.h"
#include  "GetDevices.h"
std::vector<DevicePropertiesDN*>*
Run(std::function<bool(DevicePropertiesDN*)>  filter, bool exitAfterFirst)
{
	auto DD = getDevices::getInstance();
	DD->InitGetData(filter, exitAfterFirst);
	return (DD->EnumDevices());
};

void ClearVectorDevices(std::vector<DevicePropertiesDN*>* v)
{
	for (vector<DevicePropertiesDN*>::iterator itr = v->begin(); itr != v->end(); ++itr) delete (*itr);	
	v->clear();
	delete v;
};