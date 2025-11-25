// NucleotideAnalyzer.h: CNucleotideAnalyzer bildirimi

#pragma once
#include "resource.h"       // ana semboller



#include "NucAnalyzerLib_i.h"
#include <string>
#include <sstream>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Tek iş parçacıklı COM nesneleri, tam DCOM desteğini içermeyen Windows Mobile platformları gibi Windows CE platformlarında uygun şekilde desteklenmez. ATL'yi tek iş parçacıklı COM nesnelerinin oluşturulmasını desteklemeye zorlamak için _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA'yı tanımlayın ve tek iş parçacıklı COM nesnesi uygulamalarının kullanılmasına izin verin. Bu, DCOM olmayan Windows CE platformlarında desteklenen tek iş parçacığı modeli olduğu için, rgs dosyanızdaki iş parçacığı modeli 'Uygun' olarak ayarlandı."
#endif

using namespace ATL;


// CNucleotideAnalyzer

class ATL_NO_VTABLE CNucleotideAnalyzer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CNucleotideAnalyzer, &CLSID_NucleotideAnalyzer>,
	public IDispatchImpl<INucleotideAnalyzer, &IID_INucleotideAnalyzer, &LIBID_NucAnalyzerLibLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CNucleotideAnalyzer()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CNucleotideAnalyzer)
	COM_INTERFACE_ENTRY(INucleotideAnalyzer)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    STDMETHOD(Analyze)(BSTR sequence, BSTR* result) {
        if (!sequence || !result) return E_POINTER;

        std::wstring s(sequence, SysStringLen(sequence));
        long a = 0, t = 0, g = 0, c = 0;
        bool valid = true;

        for (wchar_t ch : s) {
            wchar_t u = towupper(ch);
            switch (u) {
            case L'A': ++a; break;
            case L'T': ++t; break;
            case L'G': ++g; break;
            case L'C': ++c; break;
            default: valid = false; break;
            }
        }

        std::wstringstream ss;
        ss << L"A=" << a << L";T=" << t << L";G=" << g << L";C=" << c
            << L";Valid=" << (valid ? L"True" : L"False");

        *result = SysAllocString(ss.str().c_str());
        return S_OK;
    }


};

OBJECT_ENTRY_AUTO(__uuidof(NucleotideAnalyzer), CNucleotideAnalyzer)
