#include <string>
#include "DirectDefine.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderManagerBase.h"
#include "ShaderManager.h"
#include "ShaderTypes.h"

OriginalShader::operator VertexShader* ()
{
	return pThis->GetVertexShader(shaderName);
}

OriginalShader::operator PixelShader* ()
{
	return pThis->GetPixelShader(shaderName);
}

OriginalShader::operator ComputeShader* ()
{
	return pThis->GetComputeShader(shaderName);
}
