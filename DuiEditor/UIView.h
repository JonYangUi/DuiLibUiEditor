#pragma once

namespace DuiLib {

//�༭������ʾ
class CUIFormView : public CContainerUI
{
public:
	CUIFormView(void);
	virtual ~CUIFormView(void);
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);


	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	SIZE GetInitSize();
	void SetInitSize(int cx, int cy);

	void Refresh();
private:
};


}
