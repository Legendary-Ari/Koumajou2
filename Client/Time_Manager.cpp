#include "stdafx.h"
#include "Time_Manager.h"

IMPLEMENT_SINGLETON(CTime_Manager)
CTime_Manager::CTime_Manager()
{
}


CTime_Manager::~CTime_Manager()
{
}

float CTime_Manager::Get_DeltaTime()
{
	if (m_fDeltaTime > 1.f)
		m_fDeltaTime = 0.016f;
	return m_fDeltaTime;
}
void CTime_Manager::Ready_Time_Manager()
{
	QueryPerformanceFrequency(&m_CpuTick); 
	QueryPerformanceCounter(&m_BeginTime);
	QueryPerformanceCounter(&m_EndTime); 

}

void CTime_Manager::Update_Time_Manager()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);
	m_fDeltaTime = float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart; 
	m_BeginTime = m_EndTime;
}
