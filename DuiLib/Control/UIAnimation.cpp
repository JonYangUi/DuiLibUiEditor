#include "StdAfx.h"
#include "UIAnimation.h"
#include <vector>
#include <algorithm>

namespace DuiLib {
	struct CUIAnimation::Imp
	{
		std::vector<CAnimationData*> m_arAnimations;
	};
	
	CUIAnimation::CUIAnimation(CControlUI* pOwner):m_pImp(new CUIAnimation::Imp())
	{
		ASSERT(pOwner != nullptr);
		m_pControl = pOwner;
	}
	CUIAnimation:: ~CUIAnimation()
	{
		if(m_pImp)
		{
			delete m_pImp;
			m_pImp = nullptr;
		}
	}

	void CUIAnimation::AttachAnimationControl(CControlUI* pOwner)
	{
		m_pControl = pOwner;
	}

	BOOL CUIAnimation::StartAnimation(int nElapse, int nTotalFrame, int nAnimationID /*= 0*/, BOOL bLoop/* = FALSE*/)
	{
		bool bpAnimation = false;

		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if ((nullptr != pData) || (nElapse <= 0) || (nTotalFrame <= 0) || (nullptr == m_pControl))
		{
			ASSERT(FALSE);
			return FALSE; //pData 
		}
		else
		{
            CAnimationData* pAnimation = new CAnimationData(nElapse, nTotalFrame, nAnimationID, bLoop);
			if (pAnimation != nullptr)
            {
                if (m_pControl->GetManager()->SetTimer(m_pControl, nAnimationID, nElapse))
                {
                    m_pImp->m_arAnimations.push_back(pAnimation);
                    bpAnimation = true;
                }
			}
		}

		return bpAnimation;
	}

	void CUIAnimation::StopAnimation(int nAnimationID /*= 0*/)
	{
		if (m_pControl != nullptr)
		{
            if (nAnimationID != 0)
            {
                CAnimationData* pData = GetAnimationDataByID(nAnimationID);
                if (nullptr != pData)
                {
                    m_pControl->GetManager()->KillTimer(m_pControl, nAnimationID);
                    m_pImp->m_arAnimations.erase(std::remove(m_pImp->m_arAnimations.begin(), \
                        m_pImp->m_arAnimations.end(), pData), m_pImp->m_arAnimations.end());

                    if (pData != nullptr)
                    {
                        delete pData;
                        pData = nullptr;
                    }
                }
            }
            else
            {
                int nCount = m_pImp->m_arAnimations.size();
                for (int i = 0; i < nCount; ++i)
                {
                    CAnimationData* pData = m_pImp->m_arAnimations[i];
                    if (pData)
                    {
                        m_pControl->GetManager()->KillTimer(m_pControl, pData->m_nAnimationID);
                        if (pData != nullptr)
                        {
                            delete pData;
                            pData = nullptr;
                        }
                    }
                }
                m_pImp->m_arAnimations.clear();
            }
		}
	}

	BOOL CUIAnimation::IsAnimationRunning(int nAnimationID)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		return nullptr != pData;
	}

	int CUIAnimation::GetCurrentFrame(int nAnimationID/* = 0*/)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if (pData == nullptr)
		{
			ASSERT(FALSE);
			return -1;
		}
		return pData->m_nCurFrame;
	}

	BOOL CUIAnimation::SetCurrentFrame(int nFrame, int nAnimationID/* = 0*/)
	{
		CAnimationData* pData = GetAnimationDataByID(nAnimationID);
		if (pData)
		{
			if ((nFrame >= 0) && (nFrame <= pData->m_nTotalFrame))
			{
				pData->m_nCurFrame = nFrame;
				return TRUE;
			}
		}

		ASSERT(FALSE);
		return FALSE;
	}

	void CUIAnimation::OnAnimationElapse(int nAnimationID)
	{
		if (m_pControl != nullptr)
		{
            CAnimationData* pData = GetAnimationDataByID(nAnimationID);
			if (pData != nullptr)
            {
                int nCurFrame = pData->m_nCurFrame;
                if (nCurFrame == 0)
                {
                    OnAnimationStart(nAnimationID, pData->m_bFirstLoop);
                    pData->m_bFirstLoop = FALSE;
                }

                OnAnimationStep(pData->m_nTotalFrame, nCurFrame, nAnimationID);

                if (nCurFrame >= pData->m_nTotalFrame)
                {
                    OnAnimationStop(nAnimationID);
                    if (pData->m_bLoop)
                    {
                        pData->m_nCurFrame = 0;
                    }
                    else
                    {
                        m_pControl->GetManager()->KillTimer(m_pControl, nAnimationID);
                        m_pImp->m_arAnimations.erase(std::remove(m_pImp->m_arAnimations.begin(), \
								m_pImp->m_arAnimations.end(), pData), m_pImp->m_arAnimations.end());

                        delete pData;
						pData = nullptr;
                    }
                }

                if (pData != nullptr)
                {
                    ++(pData->m_nCurFrame);
                }
            } 
		}
	}

	CAnimationData* CUIAnimation::GetAnimationDataByID(int nAnimationID)
	{
		CAnimationData* pAnimationDateRet = nullptr;
		int nCount = m_pImp->m_arAnimations.size();
		for (int i = 0; i < nCount; ++i)
		{
			if (m_pImp->m_arAnimations[i]->m_nAnimationID == nAnimationID)
			{
				pAnimationDateRet = m_pImp->m_arAnimations[i];
				break;
			}
		}

		return pAnimationDateRet;
	}

} // namespace DuiLib