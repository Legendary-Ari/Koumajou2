#include "stdafx.h"
#include "NumberUi.h"


CNumberUi::CNumberUi()
{
}


CNumberUi::~CNumberUi()
{
}

HRESULT CNumberUi::Ready_GameObject()
{
	return E_NOTIMPL;
}

void CNumberUi::InitUpdate_GameObject()
{
}

int CNumberUi::Update_GameObject()
{
	return 0;
}

void CNumberUi::Late_Update_GameObject()
{
}

void CNumberUi::Render_GameObject()
{
}

void CNumberUi::Release_GameObject()
{
}

void CNumberUi::RenderNumber(UINT _ui, CNumberUi::ID _eId, const _vec3& _vPos)
{
	if (_ui > 9)
		return;
	wstring Anim_Key = L"Number_Font";
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(Anim_Key);
	switch (_eId)
	{
	case CNumberUi::BIG:
		Anim_Key += L"Big";
		break;
	case CNumberUi::NORMAL:
		Anim_Key += L"Normal";
		break;
	case CNumberUi::SMALL:
		Anim_Key += L"Small";
		break;
	}
	const ANIMATION* pAnim = CPrefab_Manager::Get_Instance()->Get_AnimationPrefab(Anim_Key);
	
	if (nullptr == pTexInfo)
		return;
	D3DXVECTOR3 vScroll = CScroll_Manager::Get_Scroll();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.75f, 0.75f, 0.f);
	D3DXMatrixTranslation(&matTrans, _vPos.x, _vPos.y, 0.f);
	matWorld = matScale * matTrans;
	const RECT& rect = pAnim->vecRect[_ui];
	float 	fCenterX = float(((rect.right - rect.left)*0.5f));
	float 	fCenterY = float(((rect.bottom - rect.top) * 0.5f));
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &rect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}