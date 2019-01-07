#include "Debug.h"

#include <comdef.h>

std::wstring DebugUtils::COMErrorMessage(HRESULT hr)
{
    return _com_error{hr}.ErrorMessage();
}
