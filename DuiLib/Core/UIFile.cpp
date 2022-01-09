#include "StdAfx.h"
#include "UIFile.h"


///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	CUIFile::CUIFile()
	{
		m_pData = NULL; 
		m_dwSize = 0;
	}

	CUIFile::~CUIFile()
	{
		Empty();
	}

	LPBYTE CUIFile::GetData() const
	{
		return m_pData;
	}

	DWORD  CUIFile::GetSize() const
	{
		return m_dwSize;
	}

	void CUIFile::Empty()
	{
		if(m_pData != NULL) { delete []m_pData; m_pData = NULL; }
		m_dwSize = 0U;
	}

	BOOL CUIFile::LoadFile(const STRINGorID &bitmap, LPCTSTR type, HINSTANCE instance)
	{
		Empty();

		//��ȡ˳��  skin�ļ��� ==> zip�ļ� ==> dll��Դ ==> �ļ�����·��
		do 
		{
			if( type == NULL )
			{
				CDuiString sFile = CPaintManagerUI::GetResourcePath();
				if( CPaintManagerUI::GetResourceZip().IsEmpty() )
				{
					sFile += bitmap.m_lpstr;
					HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
						FILE_ATTRIBUTE_NORMAL, NULL);
					if( hFile == INVALID_HANDLE_VALUE ) break;
					m_dwSize = ::GetFileSize(hFile, NULL);
					if( m_dwSize == 0 ) break;

					DWORD dwRead = 0;
					m_pData = new BYTE[ m_dwSize + 1 ];
					m_pData[m_dwSize] = '\0';
					::ReadFile( hFile, m_pData, m_dwSize, &dwRead, NULL );
					::CloseHandle( hFile );

					if( dwRead != m_dwSize ) 
					{
						Empty();
						break;
					}
				}
				else 
				{
					sFile += CPaintManagerUI::GetResourceZip();
					HZIP hz = NULL;
					if( CPaintManagerUI::IsCachedResourceZip() ) 
						hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
					else {
						CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
						UISTRING_CONVERSION;
						hz = OpenZip(sFile.GetData(), UIT2A(sFilePwd.GetData()));
					}
					if( hz == NULL ) break;
					ZIPENTRY ze; 
					int i = 0; 
					CDuiString key = bitmap.m_lpstr;
					key.Replace(_T("\\"), _T("/")); 
					if( FindZipItem(hz, key, true, &i, &ze) != 0 ) break;
					m_dwSize = ze.unc_size;
					if( m_dwSize == 0 ) break;
					m_pData = new BYTE[ m_dwSize + 1];
					m_pData[m_dwSize] = '\0';
					int res = UnzipItem(hz, i, m_pData, m_dwSize);
					if( res != 0x00000000 && res != 0x00000600)
					{
						Empty();
						if( !CPaintManagerUI::IsCachedResourceZip() )
							CloseZip(hz);
						break;
					}
					if( !CPaintManagerUI::IsCachedResourceZip() )
						CloseZip(hz);
				}
			}
			else 
			{
				HINSTANCE dllinstance = NULL;
				if (instance)
				{
					dllinstance = instance;
				}
				else
				{
					dllinstance = CPaintManagerUI::GetResourceDll();
				}
				HRSRC hResource = ::FindResource(dllinstance, bitmap.m_lpstr, type);
				if( hResource == NULL ) break;
				HGLOBAL hGlobal = ::LoadResource(dllinstance, hResource);
				if( hGlobal == NULL ) 
				{
					FreeResource(hResource);
					break;
				}

				m_dwSize = ::SizeofResource(dllinstance, hResource);
				if( m_dwSize == 0 ) break;
				m_pData = new BYTE[ m_dwSize + 1];
				m_pData[m_dwSize] = '\0';
				::CopyMemory(m_pData, (LPBYTE)::LockResource(hGlobal), m_dwSize);
				::FreeResource(hResource);
			}
		} while (0);

		while (!m_pData)
		{
			//������ͼƬ, ��ֱ��ȥ��ȡbitmap.m_lpstrָ���·��
			HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
				FILE_ATTRIBUTE_NORMAL, NULL);
			if( hFile == INVALID_HANDLE_VALUE ) break;
			m_dwSize = ::GetFileSize(hFile, NULL);
			if( m_dwSize == 0 ) break;

			DWORD dwRead = 0;
			m_pData = new BYTE[ m_dwSize + 1];
			m_pData[m_dwSize] = '\0';
			::ReadFile( hFile, m_pData, m_dwSize, &dwRead, NULL );
			::CloseHandle( hFile );

			if( dwRead != m_dwSize ) 
			{
				Empty();
			}
			break;
		}
		return m_dwSize > 0;
	}


	BOOL CUIFile::LoadFile(LPCTSTR pStrImage, LPCTSTR type, HINSTANCE instance)
	{
		return LoadFile(STRINGorID(pStrImage), type, instance);
	}

	BOOL CUIFile::LoadFile(UINT nID, LPCTSTR type, HINSTANCE instance)
	{
		return LoadFile(STRINGorID(nID), type, instance);
	}

} // namespace DuiLib
