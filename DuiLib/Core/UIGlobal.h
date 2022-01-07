#ifndef __UIGLOBAL_H__
#define __UIGLOBAL_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API UIGlobal
	{
	public:
		static UIGlobal* GetInstance();
		static void Release();

		UIRenderFactory *GetRenderFactory();
		static UIRender *CreateRenderTarget();
		static UIFont *CreateFont();
		static UIPen *CreatePen();
		static UIBitmap *CreateBitmap();
		static UIBrush *CreateBrush();
		static UIImage *CreateImage();

		//�����ؼ���ͼ�꣬������קʱ��ͼƬ
		static UIBitmap* CreateControlBitmap(CControlUI* pControl, DWORD dwFilterColor = 0, CControlUI* pStopControl = NULL);

		//�����ڲ����ڵı�����ˢ
		static UIBitmap* CreateControlBackBitmap(CControlUI* pControl, const RECT &rcWnd, DWORD dwFilterColor);

		//////////////////////////////////////////////////////////////////////////
		//HSL��ɫת��
		static void RGBtoHSL(DWORD ARGB, float* H, float* S, float* L);

		//HSL��ɫת��
		static void HSLtoRGB(DWORD* ARGB, float H, float S, float L);

		//HSL��ɫת��
		static DWORD AdjustHslColor(DWORD dwColor, short H, short S, short L);

		//HSL��ɫת��
		static void  AdjustHslImage(bool bUseHSL, UIImage *imageInfo, short H, short S, short L);

		//alpha utilities
		static void CheckAlphaColor(DWORD& dwColor);
	private:
		UIGlobal();
		UIGlobal(const UIGlobal &src);
		const UIGlobal& operator=(const UIGlobal& src);
		~UIGlobal();

	private:
		CStdRefPtr<UIRenderFactory> m_renderEngineFactory;
	};

} // namespace DuiLib

#endif // __UIRENDER_H__
