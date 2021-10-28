#pragma once
#include "ByteWork.h"

template<class D>
void SomethingHappenedSecond
(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)//, DataDevice* r)
{
	static constexpr int N = getTDeviceN<D>::N;
	D::data.ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;
	D::data.init2();
#ifndef NOOUT
	actionPack1.add(D::data);
	theApp.dlg->SetDlgItemText(IDC_EDIT16, std::to_string(D::data.agmt.xAcross.result()).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT17, std::to_string(D::data.agmt.yAlong.result()).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT18, std::to_string(D::data.agmt.zFlat.result()).c_str());	
	//theApp.dlg->SetDlgItemText(IDC_EDIT13, std::to_string(D::data.accel.x).c_str());
	//theApp.dlg->SetDlgItemText(IDC_EDIT14, std::to_string(D::data.magn.x).c_str());
	//theApp.dlg->SetDlgItemText(IDC_EDIT15, std::to_string(D::data.gir.x).c_str());	
#endif 
	theApp.dlg->SetDlgItemText(IDC_EDIT20, (scenarioNName[D::data.scenario] + " (" + std::to_string(N) + ")").c_str());

}

int cycle0 = 0;

template<class D> static void SomethingHappenedFirst
(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)//, DataDevice* r)
{
	static constexpr int N = getTDeviceN<D>::N;
	D::data.ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;
	D::data.init2();
	actionPack.add(D::data);
#ifndef NOOUT
	CString s = get_bits<CString>((unsigned char*)D::data.bitData, 10, def_l);
	theApp.dlg->SetDlgItemText(IDC_EDIT21, s);
	CString s1 = get_bits<CString>((unsigned char*)D::data.bitData + 10, 10, def_l);
	theApp.dlg->SetDlgItemText(IDC_EDIT22, s1);

	theApp.dlg->SetDlgItemText(IDC_EDIT1, std::to_string(D::data.accel.x).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT2, std::to_string(D::data.magn.x).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT3, std::to_string(D::data.gir.x).c_str());

	theApp.dlg->SetDlgItemText(IDC_EDIT4, std::to_string(D::data.accel.y).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT5, std::to_string(D::data.magn.y).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT6, std::to_string(D::data.gir.y).c_str());

	theApp.dlg->SetDlgItemText(IDC_EDIT7, std::to_string(D::data.accel.z).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT8, std::to_string(D::data.magn.z).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT9, std::to_string(D::data.gir.z).c_str());

	theApp.dlg->SetDlgItemText(IDC_EDIT10, std::to_string(D::data.agmt.madgwickXYZ.x).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT11, std::to_string(D::data.agmt.madgwickXYZ.y).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT12, std::to_string(D::data.agmt.madgwickXYZ.z).c_str());

	theApp.dlg->SetDlgItemText(IDC_EDIT13, std::to_string(D::data.agmt.xAcross.result()).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT14, std::to_string(D::data.agmt.yAlong.result()).c_str());
	theApp.dlg->SetDlgItemText(IDC_EDIT15, std::to_string(D::data.agmt.zFlat.result()).c_str());
	//theApp.dlg->SetDlgItemText(IDC_EDIT46, std::to_string(D::data.agmt.xTouch).c_str());
	//theApp.dlg->SetDlgItemText(IDC_EDIT45, std::to_string(D::data.agmt.yTouch).c_str());
#endif
	theApp.dlg->SetDlgItemText(IDC_EDIT19, (scenarioNName[D::data.scenario] + " (" + std::to_string(N) + ")").c_str());

}
