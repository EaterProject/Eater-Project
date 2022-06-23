#include <vector>
#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "GraphicResource.h"
#include "DrawBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderTypes.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "ResourceFactory.h"

#include "EngineData.h"
#include "VertexDefine.h"
#include "SamplerStateDefine.h"
#include "BlendStateDefine.h"
#include "DepthStencilStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "DrawBufferDefine.h"
#include "InstanceBufferDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "ImageParser.h"
#include "ShaderResourceHashTable.h"
#include "MathDefine.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "Profiler/Profiler.h"

#define MAX_VECTOR3(v3) max(max(abs(v3.x), abs(v3.y)), abs(v3.z))
#define DISTANCE(vec1, vec2) sqrtf(powf(vec1.x - vec2.x, 2.0f) + powf(vec1.y - vec2.y, 2.0f) + powf(vec1.z - vec2.z, 2.0f))

using namespace DirectX::SimpleMath;

GraphicResourceFactory::GraphicResourceFactory()
{
	// Icon Texture Route 汲沥..
	m_TextureRoute = "../Assets/Texture/Graphic/";
}

GraphicResourceFactory::~GraphicResourceFactory()
{

}

void GraphicResourceFactory::Create(int width, int height)
{
	// Back Buffer 积己..
	CreateMainRenderTarget(RT_BackBuffer::GetHashCode(), width, height);
	CreateTextureRenderTarget(width, height);

	/// Global Resource 积己..
	CreateDepthStencilStates();
	CreateRasterizerStates();
	CreateSamplerStates();
	CreateBlendStates();

	CreateDepthStencilViews(width, height);
	CreateViewPorts(width, height);

	// Global Instance Buffer..
	CreateInstanceBuffers();

	// Global Draw Buffer..
	CreateQuadBuffer();
	CreateSSAOQuadBuffer();
	CreateBoxBuffer();
	CreateSphereBuffer();

	// Debug Buffer..
	CreateLineRayBuffer();
	CreateLineQuadBuffer();
	CreateLineAxisBuffer();
	CreateLineCircleBuffer();
	CreateLineCircleSphereBuffer();
	CreateLineBoxBuffer();
	CreateLineGridBuffer();
}

void GraphicResourceFactory::Start()
{

}

void GraphicResourceFactory::Release()
{

}

void GraphicResourceFactory::CreateTextureBuffer(std::string path, TextureBuffer** ppResource)
{
	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* newTex = nullptr;

	// Texture Buffer 积己..
	m_Result = g_Graphic->CreateTextureBuffer(path, &texResource, &newTex);

	// Texture 积己 己傍矫 Texture Buffer 火涝..
	if (newTex)
	{
		size_t indexSlash = path.rfind("/") + 1;
		size_t indexDot = path.rfind(".");

		std::string texName = path.substr(indexSlash, path.size() - indexSlash);

		// Texture Buffer 积己 棺 Data 火涝..
		if((*ppResource) == nullptr) (*ppResource) = new TextureBuffer();
		(*ppResource)->pTextureBuf = newTex;

		// Texture Size 汲沥..
		g_Graphic->GetImageSize(path, &(*ppResource)->Width, &(*ppResource)->Height);

		// Debug Name..
		GPU_RESOURCE_DEBUG_NAME(newTex, texName.c_str());

		// Reset Resource..
		texResource->Release();
	}
}

void GraphicResourceFactory::CreateMeshBuffer(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	switch (mesh->m_MeshType)
	{
	case MESH_TYPE::STATIC_MESH:
		CreateLoadBuffer<VertexInput::MeshVertex>(mesh, ppResource);
		break;
	case MESH_TYPE::SKIN_MESH:
		CreateLoadBuffer<VertexInput::SkinVertex>(mesh, ppResource);
		break;
	case MESH_TYPE::TERRAIN_MESH:
		CreateLoadBuffer<VertexInput::TerrainVertex>(mesh, ppResource);
		break;
	case MESH_TYPE::QUAD_MESH:
		CreateLoadBuffer<VertexInput::PosTexVertex>(mesh, ppResource);
		break;
	default:
		break;
	}
}

void GraphicResourceFactory::CreateAnimationBuffer(ModelData* model, ModelAnimationData* animation, AnimationBuffer** ppResource)
{
	if (model->BoneOffsetList.empty()) return;

	const std::vector<Matrix>& offsetList = model->BoneOffsetList;

	UINT struct_size = sizeof(XMFLOAT3X4);
	UINT row_offset = (UINT)offsetList.size();
	UINT column_offset = 0;
	UINT total_offset = 0;
	UINT max_offset = 0;

	Matrix nowOffsetTM;
	Matrix nowFrameTM;

	ParserData::CModelAnimation* nowModelAni = nullptr;
	ParserData::CAnimation* nowAni = nullptr;
	ParserData::CFrame* nowFrame = nullptr;

	// 货肺款 Animation Buffer 积己..
	if (*ppResource == nullptr) *ppResource = new AnimationBuffer();

	// 货肺 积己茄 Animation Buffer 火涝..
	AnimationBuffer* animationBuf = *ppResource;
	animationBuf->FrameOffset = row_offset;
	animationBuf->AnimationOffset.clear();
	animationBuf->AnimationOffset.push_back(total_offset);
	animationBuf->AnimationCount = animation->AnimationCount;

	(*ppResource) = animationBuf;

	std::vector<XMFLOAT3X4> animationList;
	for (auto& aniList : animation->AnimList)
	{
		nowModelAni = aniList.second;
		for (int frame = 0; frame < nowModelAni->m_TotalFrame; frame++)
		{
			for (int ani = 0; ani < nowModelAni->m_AnimationList.size(); ani++)
			{
				nowAni = nowModelAni->m_AnimationList[ani];
				nowOffsetTM = offsetList[ani];

				nowFrame = nowAni->m_AniData[frame];

				nowFrameTM = Matrix::CreateScale(nowFrame->m_WorldScale) * Matrix::CreateFromQuaternion(nowFrame->m_WorldRotQt) * Matrix::CreateTranslation(nowFrame->m_WorldPos);
				nowFrameTM = nowOffsetTM * nowFrameTM;

				XMFLOAT3X4 matrix;
				matrix._11 = nowFrameTM._11, matrix._12 = nowFrameTM._12; matrix._13 = nowFrameTM._13; matrix._14 = nowFrameTM._41;
				matrix._21 = nowFrameTM._21, matrix._22 = nowFrameTM._22; matrix._23 = nowFrameTM._23; matrix._24 = nowFrameTM._42;
				matrix._31 = nowFrameTM._31, matrix._32 = nowFrameTM._32; matrix._33 = nowFrameTM._33; matrix._34 = nowFrameTM._43;

				animationList.emplace_back(std::move(matrix));
			}
		}

		column_offset = nowModelAni->m_TotalFrame;
		total_offset += row_offset * column_offset;

		if (animationBuf->AnimationOffset.size() < animation->AnimList.size())
		{
			animationBuf->AnimationOffset.push_back(total_offset);
		}
	}

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = total_offset * struct_size;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = struct_size;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &animationList[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = total_offset;

	/// Buffer 积己..
	ID3D11Buffer* buffer = nullptr;
	ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)animationBuf->pAnimationBuf;

	// 捞傈 Animation Buffer 力芭..
	RELEASE_COM(srv);

	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &buffer);

	m_Result = g_Graphic->CreateShaderResourceView(buffer, &srvDesc, &srv);

	// Animation Buffer 历厘..
	animationBuf->pAnimationBuf = srv;

	GPU_RESOURCE_DEBUG_NAME(srv, std::string(model->Name + "_AnimationBuffer").c_str());

	// Reset Resource..
	buffer->Release();
}

void GraphicResourceFactory::CreateImage(std::string name, Hash_Code hash_code, std::string fileName)
{
	// 货肺款 Resource Pointer 积己..
	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* newTex = nullptr;

	// Texture Buffer 积己..
	m_Result = g_Graphic->CreateTextureBuffer(m_TextureRoute + fileName, &texResource, &newTex);

	// Texture 积己 己傍矫 Texture Buffer 火涝..
	if (newTex)
	{
		// ShaderResourceView 积己..
		ShaderResourceView* newResource = new ShaderResourceView(hash_code, newTex);

		// Resource 殿废..
		g_ResourceManager->AddResource(hash_code, newResource);

		// Debug Name..
		CPU_RESOURCE_DEBUG_NAME(newResource, fileName.c_str());

		GPU_RESOURCE_DEBUG_NAME(newTex, fileName.c_str());

		// Reset Resource..
		texResource->Release();
	}
}

void GraphicResourceFactory::CreateDepthStencil(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture2D Resource 积己..
	m_Result = g_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
	}

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_DEPTH_STENCIL)
	{
		// DepthStencilView Resource 积己..
		m_Result = g_Graphic->CreateDepthStencilView(tex2D.Get(), dsvDesc, dsv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ DepthStencilView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ DepthStencilView ] '%s' FAILED!!", name.c_str());
		}
	}
	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		g_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
		}
	}

	// Resource 积己 棺 殿废..
	Texture2D* newTex2D = new Texture2D(hash_code, tex2D.Get());
	DepthStencilView* newDSV = RegisterResource<DepthStencilView, ID3D11DepthStencilView>(hash_code, dsv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());

	// DepthStencil 积己..
	DepthStencil* newResource = new DepthStencil(hash_code, newTex2D, newDSV, newSRV);

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newTex2D, (name + "_Tex2D").c_str());
	CPU_RESOURCE_DEBUG_NAME(newDSV, (name + "_DSV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newSRV, (name + "_SRV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(dsv.Get(), ((std::string)name + "_DSV").c_str());
	GPU_RESOURCE_DEBUG_NAME(srv.Get(), ((std::string)name + "_SRV").c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(dsv);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateRenderTexture(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture2D Resource 积己..
	m_Result = g_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
	}

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_RENDER_TARGET)
	{
		// RenderTargetView Resource 积己..
		m_Result = g_Graphic->CreateRenderTargetView(tex2D.Get(), rtvDesc, rtv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
		}
	}
	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Result = g_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
		}
	}
	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Result = g_Graphic->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
		}
	}

	// Resource 积己 棺 殿废..
	Texture2D* newTex2D = new Texture2D(hash_code, tex2D.Get());
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_code, uav.Get());

	// RenderTarget 积己..
	RenderTexture* newResource = new RenderTexture(hash_code, newTex2D, newRTV, newSRV, newUAV);

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newTex2D, (name + "_Tex2D").c_str());
	CPU_RESOURCE_DEBUG_NAME(newRTV, (name + "_RTV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newSRV, (name + "_SRV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newUAV, (name + "_UAV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(rtv.Get(), (name + "_RTV").c_str());
	GPU_RESOURCE_DEBUG_NAME(srv.Get(), (name + "_SRV").c_str());
	GPU_RESOURCE_DEBUG_NAME(uav.Get(), (name + "_UAV").c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateRenderBuffer(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Buffer Resource 积己..
	m_Result = g_Graphic->CreateBuffer(bufferDesc, subData, buffer.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", name.c_str());
	}

	// Bind Resource 积己..
	UINT bindFlag = bufferDesc->BindFlags;

	if (bindFlag & D3D11_BIND_RENDER_TARGET)
	{
		// RenderTargetView Resource 积己..
		m_Result = g_Graphic->CreateRenderTargetView(buffer.Get(), rtvDesc, rtv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
		}
	}
	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Result = g_Graphic->CreateShaderResourceView(buffer.Get(), srvDesc, srv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
		}
	}
	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Result = g_Graphic->CreateUnorderedAccessView(buffer.Get(), uavDesc, uav.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
		}
	}

	// Resource 积己 棺 殿废..
	Buffer* newBuffer = new Buffer(hash_code, buffer.Get());
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_code, uav.Get());

	// RenderTarget 积己..
	RenderBuffer* newResource = new RenderBuffer(hash_code, newBuffer, newRTV, newSRV, newUAV);

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newBuffer, (name + "_Buffer").c_str());
	CPU_RESOURCE_DEBUG_NAME(newRTV, (name + "_RTV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newSRV, (name + "_SRV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newUAV, (name + "_UAV").c_str());
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(buffer.Get(), (name + "_Buffer").c_str());
	GPU_RESOURCE_DEBUG_NAME(rtv.Get(), (name + "_RTV").c_str());
	GPU_RESOURCE_DEBUG_NAME(srv.Get(), (name + "_SRV").c_str());
	GPU_RESOURCE_DEBUG_NAME(uav.Get(), (name + "_UAV").c_str());

	// Reset Resource..
	RESET_COM(buffer);
	RESET_COM(rtv);
	RESET_COM(srv);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateDrawBuffer(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, const void* vertices, const void* indices)
{
	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuf = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuf = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	// Vertex Buffer 积己..
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, vertexBuf.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_VertexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_VertexBuf").c_str());
	}

	// Index Buffer 积己..
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = indices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, indexBuf.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_IndexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_IndexBuf").c_str());
	}

	// Resource Buffer 积己..
	Buffer* newVertexBuf = new Buffer(hash_code, vertexBuf.Get());
	Buffer* newIndexBuf = new Buffer(hash_code, indexBuf.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(hash_code, newVertexBuf, newIndexBuf);

	// Buffer Data 火涝..
	newBuf->Format = (DXGI_FORMAT)format;
	newBuf->Topology = (D3D_PRIMITIVE_TOPOLOGY)topology;
	newBuf->Stride = vSize;
	newBuf->IndexCount = iCount;
	newBuf->Offset = 0;

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newBuf);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newBuf, (name + "_DrawBuf").c_str());
	CPU_RESOURCE_DEBUG_NAME(newVertexBuf, (name + "_VertexBuf").c_str());
	CPU_RESOURCE_DEBUG_NAME(newIndexBuf, (name + "_IndexBuf").c_str());
	GPU_RESOURCE_DEBUG_NAME(newBuf->VertexBuf->Get(), (name + "_VertexBuf").c_str());
	GPU_RESOURCE_DEBUG_NAME(newBuf->IndexBuf->Get(), (name + "_IndexBuf").c_str());

	RESET_COM(vertexBuf);
	RESET_COM(indexBuf);
}

void GraphicResourceFactory::CreateDrawBuffer(std::string name, Hash_Code hash_code, UINT format, UINT topology, UINT vByteSize, UINT iByteSize, UINT vSize, UINT iCount, UINT vUsage, UINT iUsage, const void* vertices, const void* indices)
{
	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuf = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuf = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	// Vertex Buffer 积己..
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, (D3D11_USAGE)vUsage, vertexBuf.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_VertexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_VertexBuf").c_str());
	}

	// Index Buffer 积己..
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = indices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, (D3D11_USAGE)iUsage, indexBuf.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_IndexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (name + "_IndexBuf").c_str());
	}

	// Resource Buffer 积己..
	Buffer* newVertexBuf = new Buffer(hash_code, vertexBuf.Get());
	Buffer* newIndexBuf = new Buffer(hash_code, indexBuf.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(hash_code, newVertexBuf, newIndexBuf);

	// Buffer Data 火涝..
	newBuf->Format = (DXGI_FORMAT)format;
	newBuf->Topology = (D3D_PRIMITIVE_TOPOLOGY)topology;
	newBuf->Stride = vSize;
	newBuf->IndexCount = iCount;
	newBuf->Offset = 0;

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newBuf);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newBuf, (name + "_DrawBuf").c_str());
	CPU_RESOURCE_DEBUG_NAME(newVertexBuf, (name + "_VertexBuf").c_str());
	CPU_RESOURCE_DEBUG_NAME(newIndexBuf, (name + "_IndexBuf").c_str());
	GPU_RESOURCE_DEBUG_NAME(newBuf->VertexBuf->Get(), (name + "_VertexBuf").c_str());
	GPU_RESOURCE_DEBUG_NAME(newBuf->IndexBuf->Get(), (name + "_IndexBuf").c_str());

	RESET_COM(vertexBuf);
	RESET_COM(indexBuf);
}

void GraphicResourceFactory::CreateInstanceBuffer(std::string name, Hash_Code hash_code, UINT byteSize, UINT instCount, const void* instance)
{
	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> instanceBuf = nullptr;

	/// Instance Buffer Desc 汲沥..
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = byteSize * instCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = instance;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Instance Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, instanceBuf.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Instance Buffer ] '%s' FAILED!!", (name + "_InstanceBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Instance Buffer ] '%s' FAILED!!", (name + "_InstanceBuf").c_str());
	}

	// Resource Buffer 积己..
	Buffer* newInstanceBuf = new Buffer(hash_code, instanceBuf.Get());

	// Resource Instance Buffer 积己..
	InstanceBuffer* newBuf = new InstanceBuffer(hash_code, newInstanceBuf);

	// Buffer Data 火涝..
	newBuf->DataSize = byteSize * instCount;
	newBuf->Stride = byteSize;
	newBuf->Offset = 0;

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newBuf);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newBuf, (name + "_InstanceBuf").c_str());
	CPU_RESOURCE_DEBUG_NAME(newInstanceBuf, (name + "_InstanceBuf").c_str());
	GPU_RESOURCE_DEBUG_NAME(newBuf->InstanceBuf->Get(), (name + "_InstanceBuf").c_str());

	RESET_COM(instanceBuf);
}

void GraphicResourceFactory::CreateRenderTargetView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;

	// Texture2D Resource 积己..
	m_Result = g_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
	}

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_RENDER_TARGET)
	{
		// ShaderResourceView Resource 积己..
		m_Result = g_Graphic->CreateRenderTargetView(tex2D.Get(), rtvDesc, rtv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
		}
	}

	// ShaderResourceView 积己..
	RenderTargetView* newResource = new RenderTargetView(hash_code, rtv.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(rtv.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(rtv);
}

void GraphicResourceFactory::CreateRenderTargetView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;

	// Texture Description 眠免..
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc(&texDesc);

	// Bind Resource 积己..
	UINT bindFlag = texDesc.BindFlags;

	if (bindFlag & D3D11_BIND_RENDER_TARGET)
	{
		// ShaderResourceView Resource 积己..
		m_Result = g_Graphic->CreateRenderTargetView(tex2D, rtvDesc, rtv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ RenderTargetView ] '%s' FAILED!!", name.c_str());
		}
	}

	// ShaderResourceView 积己..
	RenderTargetView* newResource = new RenderTargetView(hash_code, rtv.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(rtv.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(rtv);
}

void GraphicResourceFactory::CreateShaderResourceView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture2D Resource 积己..
	m_Result = g_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Result = g_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
		}
	}

	// ShaderResourceView 积己..
	ShaderResourceView* newResource = new ShaderResourceView(hash_code, srv.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(srv.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateShaderResourceView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture Description 眠免..
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc(&texDesc);

	// Bind Resource 积己..
	UINT bindFlag = texDesc.BindFlags;

	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Result = g_Graphic->CreateShaderResourceView(tex2D, srvDesc, srv.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ ShaderResourceView ] '%s' FAILED!!", name.c_str());
		}
	}

	// ShaderResourceView 积己..
	ShaderResourceView* newResource = new ShaderResourceView(hash_code, srv.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(srv.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateUnorderedAccessView(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture2D Resource 积己..
	m_Result = g_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Texture2D ] '%s' FAILED!!", name.c_str());
	}

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Result = g_Graphic->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
		}
	}

	// UnorderedAccessView 积己..
	UnorderedAccessView* newResource = new UnorderedAccessView(hash_code, uav.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(uav.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateUnorderedAccessView(std::string name, Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture Description 眠免..
	D3D11_TEXTURE2D_DESC texDesc;
	tex2D->GetDesc(&texDesc);

	// Bind Resource 积己..
	UINT bindFlag = texDesc.BindFlags;

	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Result = g_Graphic->CreateUnorderedAccessView(tex2D, uavDesc, uav.GetAddressOf());

		if (FAILED(m_Result))
		{
			PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
			PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ UnorderedAccessView ] '%s' FAILED!!", name.c_str());
		}
	}

	// UnorderedAccessView 积己..
	UnorderedAccessView* newResource = new UnorderedAccessView(hash_code, uav.Get());

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(uav.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(uav);
}


void GraphicResourceFactory::CreateDepthStencilState(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss = nullptr;

	// DepthStencilState Resource 积己..
	m_Result = g_Graphic->CreateDepthStencilState(dssDesc, dss.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ DepthStencilState ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ DepthStencilState ] '%s' FAILED!!", name.c_str());
	}

	// DepthStencilState 积己..
	DepthStencilState* newResource = new DepthStencilState(hash_code, dss.Get());

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(dss.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(dss);
}

void GraphicResourceFactory::CreateRasterizerState(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs = nullptr;

	// RasterizerState Resource 积己..
	m_Result = g_Graphic->CreateRasterizerState(rsDesc, rs.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ RasterizerState ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ RasterizerState ] '%s' FAILED!!", name.c_str());
	}

	// RasterizerState 积己..
	RasterizerState* newResource = new RasterizerState(hash_code, rs.Get());

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(rs.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(rs);
}

void GraphicResourceFactory::CreateBlendState(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11BlendState> bs = nullptr;

	// BlendState Resource 积己..
	m_Result = g_Graphic->CreateBlendState(bsDesc, bs.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ BlendState ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ BlendState ] '%s' FAILED!!", name.c_str());
	}

	// BlendState 积己..
	BlendState* newResource = new BlendState(hash_code, bs.Get());

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(bs.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(bs);
}

void GraphicResourceFactory::CreateSamplerState(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11SamplerState> ss = nullptr;

	// SamplerState Resource 积己..
	m_Result = g_Graphic->CreateSamplerState(ssDesc, ss.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ SamplerState ] '%s' FAILED!!", name.c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ SamplerState ] '%s' FAILED!!", name.c_str());
	}

	// SamplerState 积己..
	SamplerState* newResource = new SamplerState(hash_code, ss.Get());

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());

	GPU_RESOURCE_DEBUG_NAME(ss.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(ss);
}

void GraphicResourceFactory::CreateViewPort(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	// ViewPort 积己..
	ViewPort* newResource = new ViewPort(hash_code, ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);

	// Resize 汲沥..
	newResource->SetResize(true);

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());
}


void GraphicResourceFactory::CreateViewPort(std::string name, Hash_Code hash_code, float offsetX, float offsetY, float width, float height)
{
	// ViewPort 积己..
	ViewPort* newResource = new ViewPort(hash_code, offsetX, offsetY, width, height);

	// Resize 汲沥..
	newResource->SetResize(false);

	// Resoure 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newResource, name.c_str());
}


void GraphicResourceFactory::CreateMainRenderTarget(Hash_Code hash_code, UINT width, UINT height)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Swap Chain, Render Target View Resize
	m_Result = g_Graphic->CreateBackBuffer(width, height, tex2D.GetAddressOf(), rtv.GetAddressOf(), srv.GetAddressOf());

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ RenderTarget ] 'BackBuffer' FAILED!!");
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ RenderTarget ] 'BackBuffer' FAILED!!");
	}

	// Resource 积己 棺 殿废..
	Texture2D* newTex2D = new Texture2D(hash_code, tex2D.Get());
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_code, uav.Get());

	// Main RenderTarget 积己..
	RenderTexture* mainRenderTarget = new RenderTexture(hash_code, newTex2D, newRTV, newSRV, newUAV);

	// Resource 殿废..
	g_ResourceManager->AddMainRenderTarget(mainRenderTarget);

	// Debug Name..
	CPU_RESOURCE_DEBUG_NAME(newTex2D, "Main_Tex2D");
	CPU_RESOURCE_DEBUG_NAME(newRTV, "Main_RTV");
	CPU_RESOURCE_DEBUG_NAME(newSRV, "Main_SRV");
	CPU_RESOURCE_DEBUG_NAME(newUAV, "Main_UAV");
	CPU_RESOURCE_DEBUG_NAME(mainRenderTarget, "Main_RT");

	GPU_RESOURCE_DEBUG_NAME(rtv.Get(), "Main_RTV");
	GPU_RESOURCE_DEBUG_NAME(srv.Get(), "Main_SRV");
	GPU_RESOURCE_DEBUG_NAME(uav.Get(), "Main_UAV");

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateTextureRenderTarget(UINT width, UINT height)
{
	// HDR 傈侩 OutPut RenderTarget..
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = texDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	CreateRenderTexture(RT_OutPut1::GetName(), RT_OutPut1::GetHashCode(), &texDesc, nullptr, &rtvDesc, &srvDesc, &uavDesc);
	CreateRenderTexture(RT_OutPut2::GetName(), RT_OutPut2::GetHashCode(), &texDesc, nullptr, &rtvDesc, &srvDesc, &uavDesc);
	CreateRenderTexture(RT_OutPut3::GetName(), RT_OutPut3::GetHashCode(), &texDesc, nullptr, &rtvDesc, &srvDesc, &uavDesc);
}

template<>
DepthStencilView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11DepthStencilView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	DepthStencilView* newResource = new DepthStencilView(hash_code, resource);

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
RenderTargetView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11RenderTargetView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	RenderTargetView* newResource = new RenderTargetView(hash_code, resource);

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
ShaderResourceView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11ShaderResourceView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	ShaderResourceView* newResource = new ShaderResourceView(hash_code, resource);

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
UnorderedAccessView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11UnorderedAccessView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	UnorderedAccessView* newResource = new UnorderedAccessView(hash_code, resource);

	// Resource 殿废..
	g_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<VertexInput::MeshVertex>(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	if (mesh->m_VertexList.empty()) return;

	// 货肺款 Buffer 积己..
	VertexBuffer* newVertexBuf = new VertexBuffer();
	IndexBuffer* newIndexBuf = new IndexBuffer();

	// 货肺款 Mesh Buffer 积己..
	if (*ppResource == nullptr) *ppResource = new MeshBuffer();

	// 货肺 积己茄 Mesh Buffer 火涝..
	MeshBuffer* newMeshBuf = *ppResource;

	newMeshBuf->VertexBuf = newVertexBuf;
	newMeshBuf->IndexBuf = newIndexBuf;

	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT iCount = (UINT)mesh->m_IndexList.size();
	UINT vByteSize = sizeof(VertexInput::MeshVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount * 3;

	Vector3 vMin(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	Vector3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	std::vector<VertexInput::MeshVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;
		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;
		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// Bounding Data..
		Vector3 P = vertices[i].Pos;

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	/// Buffer 积己..
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = &vertices[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Vertex Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &vb);

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_VertexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_VertexBuf").c_str());
	}

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = &indices[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Index Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &ib);

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_IndexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_IndexBuf").c_str());
	}

	// 逞败拎具且 Bounding Data..
	MeshProperty* subData = newMeshBuf->Mesh_Property;

	subData->BoundSphere.Center = (vMin + vMax) * 0.5f;
	subData->BoundSphere.Radius = DISTANCE(vMax, vMin) * 0.5f;

	subData->BoundBox.Center = (vMin + vMax) * 0.5f;
	subData->BoundBox.Extents = (vMax - vMin) * 0.5f;

	// 逞败拎具且 VertexBufferData 火涝..
	newVertexBuf->pVertexBuf = vb;
	newVertexBuf->Stride = sizeof(VertexInput::MeshVertex);

	// 逞败拎具且 IndexBufferData 火涝..
	newIndexBuf->Count = iCount * 3;
	newIndexBuf->pIndexBuf = ib;

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(vb, (mesh->m_NodeName + "_VertexBuffer").c_str());
	GPU_RESOURCE_DEBUG_NAME(ib, (mesh->m_NodeName + "_IndexBuffer").c_str());
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<VertexInput::SkinVertex>(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	if (mesh->m_VertexList.empty()) return;

	// 货肺款 Buffer 积己..
	VertexBuffer* newVertexBuf = new VertexBuffer();
	IndexBuffer* newIndexBuf = new IndexBuffer();

	// 货肺款 Mesh Buffer 积己..
	if (*ppResource == nullptr) *ppResource = new MeshBuffer();

	// 货肺 积己茄 Mesh Buffer 火涝..
	MeshBuffer* newMeshBuf = *ppResource;

	newMeshBuf->VertexBuf = newVertexBuf;
	newMeshBuf->IndexBuf = newIndexBuf;

	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT iCount = (UINT)mesh->m_IndexList.size();
	UINT vByteSize = sizeof(VertexInput::SkinVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount * 3;

	Vector3 vMin(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	Vector3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	std::vector<VertexInput::SkinVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;
		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;
		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// Bone Weights, Bone Index..
		UINT bCount = (UINT)mesh->m_VertexList[i]->m_BoneIndices.size();
		for (UINT j = 0; j < bCount; j++)
		{
			if (j < 4)
			{
				vertices[i].BoneIndex1[j] = mesh->m_VertexList[i]->m_BoneIndices[j];
				vertices[i].BoneWeight1[j] = mesh->m_VertexList[i]->m_BoneWeights[j];
			}
			else if (j < 8)
			{
				vertices[i].BoneIndex2[j - 4] = mesh->m_VertexList[i]->m_BoneIndices[j];
				vertices[i].BoneWeight2[j - 4] = mesh->m_VertexList[i]->m_BoneWeights[j];
			}
			else
			{
				continue;
			}
		}

		// Bounding Data..
		Vector3 P = vertices[i].Pos;

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	/// Buffer 积己..
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = &vertices[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Vertex Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &vb);

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_VertexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_VertexBuf").c_str());
	}

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = &indices[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Index Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &ib);

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_IndexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_IndexBuf").c_str());
	}

	// 逞败拎具且 Bounding Data..
	MeshProperty* subData = newMeshBuf->Mesh_Property;

	subData->BoundSphere.Center = (vMin + vMax) * 0.5f;
	subData->BoundSphere.Radius = DISTANCE(vMax, vMin) * 0.5f;

	subData->BoundBox.Center = (vMin + vMax) * 0.5f;
	subData->BoundBox.Extents = (vMax - vMin) * 0.5f;

	// 逞败拎具且 VertexBufferData 火涝..
	newVertexBuf->pVertexBuf = vb;
	newVertexBuf->Stride = sizeof(VertexInput::SkinVertex);

	// 逞败拎具且 IndexBufferData 火涝..
	newIndexBuf->Count = iCount * 3;
	newIndexBuf->pIndexBuf = ib;

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(vb, (mesh->m_NodeName + "_VertexBuffer").c_str());
	GPU_RESOURCE_DEBUG_NAME(ib, (mesh->m_NodeName + "_IndexBuffer").c_str());
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<VertexInput::TerrainVertex>(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	if (mesh->m_VertexList.empty()) return;

	// 货肺款 Buffer 积己..
	VertexBuffer* newVertexBuf = new VertexBuffer();
	IndexBuffer* newIndexBuf = new IndexBuffer();

	// 货肺款 Mesh Buffer 积己..
	if (*ppResource == nullptr) *ppResource = new MeshBuffer();

	// 货肺 积己茄 Mesh Buffer 火涝..
	MeshBuffer* newMeshBuf = *ppResource;

	newMeshBuf->VertexBuf = newVertexBuf;
	newMeshBuf->IndexBuf = newIndexBuf;

	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT iCount = (UINT)mesh->m_IndexList.size();
	UINT vByteSize = sizeof(VertexInput::TerrainVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount * 3;

	Vector3 vMin(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	Vector3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// Mask Pixel Data Parsing..
	ParserData::ImageData maskImage1 = ImageParser::LoadImagePixel(mesh->m_MaskName1.c_str(), 4, PIXEL_TYPE::FLOAT_TYPE);
	ParserData::ImageData maskImage2 = ImageParser::LoadImagePixel(mesh->m_MaskName2.c_str(), 4, PIXEL_TYPE::FLOAT_TYPE);

	std::vector<VertexInput::TerrainVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		vertices[i].Tex.x = vertices[i].Pos.x;
		vertices[i].Tex.y = vertices[i].Pos.z;

		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// 秦寸 Pixel Mask Color..
		int originWidth = (int)abs(mesh->m_VertexList[i]->m_Pos.x);
		int originHeight = (int)abs(mesh->m_VertexList[i]->m_Pos.z);
		int width, height;
		// ∴ ∴ ∴
		// ∴ ♂ ∴
		// ∴ ∴ ∴
		width = originWidth;
		if (width >= maskImage1.width)
		{
			width = maskImage1.width - 1;
		}
		height = originHeight;
		if (height >= maskImage1.height)
		{
			height = maskImage1.height - 1;
		}

		Vector4 maskColor1_00 = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_00 = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_00.z = maskColor2_00.x;
		maskColor1_00.w = maskColor2_00.y;
		maskColor1_00 *= 2.0f;

		// ∴ ∴ ∴
		// ∴ ∴ ♂
		// ∴ ∴ ∴
		width = originWidth + 1;
		if (width >= maskImage1.width)
		{
			width = maskImage1.width - 1;
		}
		height = originHeight;

		Vector4 maskColor1_p0 = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_p0 = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_p0.z = maskColor2_p0.x;
		maskColor1_p0.w = maskColor2_p0.y;
		maskColor1_p0 *= 0.5f;

		// ∴ ∴ ∴
		// ∴ ∴ ∴
		// ∴ ♂ ∴
		width = originWidth;
		height = originHeight + 1;
		if (height >= maskImage1.height)
		{
			height = maskImage1.height - 1;
		}

		Vector4 maskColor1_0p = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_0p = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_0p.z = maskColor2_0p.x;
		maskColor1_0p.w = maskColor2_0p.y;
		maskColor1_0p *= 0.5f;

		// ∴ ∴ ∴
		// ∴ ∴ ∴
		// ∴ ∴ ♂
		width = originWidth + 1;
		if (width >= maskImage1.width)
		{
			width = maskImage1.width - 1;
		}
		height = originHeight + 1;
		if (height >= maskImage1.height)
		{
			height = maskImage1.height - 1;
		}

		Vector4 maskColor1_pp = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_pp = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_pp.z = maskColor2_pp.x;
		maskColor1_pp.w = maskColor2_pp.y;
		maskColor1_pp *= 0.25f;

		// ∴ ∴ ∴
		// ♂ ∴ ∴
		// ∴ ∴ ∴
		width = originWidth - 1;
		if (width < 0)
		{
			width = 0;
		}
		height = originHeight;

		Vector4 maskColor1_m0 = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_m0 = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_m0.z = maskColor2_m0.x;
		maskColor1_m0.w = maskColor2_m0.y;
		maskColor1_m0 *= 0.5f;

		// ∴ ♂ ∴
		// ∴ ∴ ∴
		// ∴ ∴ ∴
		width = originWidth;
		height = originHeight - 1;
		if (height < 0)
		{
			height = 0;
		}

		Vector4 maskColor1_0m = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_0m = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_0m.z = maskColor2_0m.x;
		maskColor1_0m.w = maskColor2_0m.y;
		maskColor1_0m *= 0.5f;

		// ♂ ∴ ∴
		// ∴ ∴ ∴
		// ∴ ∴ ∴
		width = originWidth - 1;
		if (width < 0)
		{
			width = 0;
		}
		height = originHeight - 1;
		if (height < 0)
		{
			height = 0;
		}

		Vector4 maskColor1_mm = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_mm = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_mm.z = maskColor2_mm.x;
		maskColor1_mm.w = maskColor2_mm.y;
		maskColor1_mm *= 0.25f;

		// ∴ ∴ ♂ 
		// ∴ ∴ ∴
		// ∴ ∴ ∴
		width = originWidth + 1;
		if (width >= maskImage1.width)
		{
			width = maskImage1.width - 1;
		}
		height = originHeight - 1;
		if (height < 0)
		{
			height = 0;
		}

		Vector4 maskColor1_pm = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_pm = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_pm.z = maskColor2_pm.x;
		maskColor1_pm.w = maskColor2_pm.y;
		maskColor1_pm *= 0.25f;

		// ∴ ∴ ∴
		// ∴ ∴ ∴
		// ♂ ∴ ∴ 
		width = originWidth - 1;
		if (width < 0)
		{
			width = 0;
		}
		height = originHeight + 1;
		if (height >= maskImage1.height)
		{
			height = maskImage1.height - 1;
		}
		Vector4 maskColor1_mp = ImageParser::GetPixelColor(maskImage1, width, height);
		Vector4 maskColor2_mp = ImageParser::GetPixelColor(maskImage2, width, height);
		maskColor1_mp.z = maskColor2_mp.x;
		maskColor1_mp.w = maskColor2_mp.y;
		maskColor1_mp *= 0.25f;

		// Final Calculate..
		Vector4 finalColor = maskColor1_00 + maskColor1_p0 + maskColor1_0p + maskColor1_pp + maskColor1_m0 + maskColor1_0m + maskColor1_mm + maskColor2_pm + maskColor1_mp;
		vertices[i].Mask = finalColor / (finalColor.x + finalColor.y + finalColor.z + finalColor.w);

		// Bounding Data..
		Vector3 P = vertices[i].Pos;

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	/// Buffer 积己..
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = &vertices[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Vertex Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &vb);

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_VertexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_VertexBuf").c_str());
	}

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	initData.pSysMem = &indices[0];
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	// Index Buffer 积己..
	m_Result = g_Graphic->CreateBuffer(&bufferDesc, &initData, &ib);

	if (FAILED(m_Result))
	{
		PROFILE_RESULT(PROFILE_OUTPUT::LOG_FILE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_IndexBuf").c_str());
		PROFILE_RESULT(PROFILE_OUTPUT::VS_CODE, m_Result, "[ Graphic ][ Create ][ Buffer ] '%s' FAILED!!", (mesh->m_NodeName + "_IndexBuf").c_str());
	}

	// 逞败拎具且 Bounding Data..
	MeshProperty* subData = newMeshBuf->Mesh_Property;

	subData->BoundSphere.Center = (vMin + vMax) * 0.5f;
	subData->BoundSphere.Radius = DISTANCE(vMax, vMin) * 0.5f;

	subData->BoundBox.Center = (vMin + vMax) * 0.5f;
	subData->BoundBox.Extents = (vMax - vMin) * 0.5f;

	// 逞败拎具且 VertexBufferData 火涝..
	newVertexBuf->pVertexBuf = vb;
	newVertexBuf->Stride = sizeof(VertexInput::TerrainVertex);

	// 逞败拎具且 IndexBufferData 火涝..
	newIndexBuf->Count = iCount * 3;
	newIndexBuf->pIndexBuf = ib;

	// Debug Name..
	GPU_RESOURCE_DEBUG_NAME(vb, (mesh->m_NodeName + "_VertexBuffer").c_str());
	GPU_RESOURCE_DEBUG_NAME(ib, (mesh->m_NodeName + "_IndexBuffer").c_str());

	// Image Erase..
	ImageParser::EraseImagePixel(maskImage1);
	ImageParser::EraseImagePixel(maskImage2);
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<VertexInput::PosVertex>(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{

}

template<>
void GraphicResourceFactory::CreateLoadBuffer<VertexInput::PosTexVertex>(ParserData::CMesh* mesh, MeshBuffer** ppResource)
{
	// 货肺款 Buffer 积己..
	VertexBuffer* newVertexBuf = new VertexBuffer();
	IndexBuffer* newIndexBuf = new IndexBuffer();

	// 货肺款 Mesh Buffer 积己..
	if (*ppResource == nullptr) *ppResource = new MeshBuffer();
	(*ppResource)->VertexBuf = newVertexBuf;
	(*ppResource)->IndexBuf = newIndexBuf;

	// Quad Buffer..
	DrawBuffer* quadBuf = g_ResourceManager->GetDrawBuffer<DB_Quad>();

	// 逞败拎具且 VertexBufferData 火涝..
	newVertexBuf->pVertexBuf = quadBuf->VertexBuf->Get();
	newVertexBuf->Stride = quadBuf->Stride;

	// 逞败拎具且 IndexBufferData 火涝..
	newIndexBuf->Count = quadBuf->IndexCount;
	newIndexBuf->pIndexBuf = quadBuf->IndexBuf->Get();
}

void GraphicResourceFactory::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Default DepthStencilState 积己..
	CreateDepthStencilState(DSS_Defalt::GetName(), DSS_Defalt::GetHashCode(), &depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	// NoDepth DepthStencilState 积己..
	CreateDepthStencilState(DSS_OutLine::GetName(), DSS_OutLine::GetHashCode(), &depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	// NoStencil DepthStencilState 积己..
	CreateDepthStencilState(DSS_NoStencil::GetName(), DSS_NoStencil::GetHashCode(), &depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// NoDepth DepthStencilState 积己..
	CreateDepthStencilState(DSS_NoDepth::GetName(), DSS_NoDepth::GetHashCode(), &depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	// NoDepthStencil DepthStencilState 积己..
	CreateDepthStencilState(DSS_Mask::GetName(), DSS_Mask::GetHashCode(), &depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	// NoDepthStencil DepthStencilState 积己..
	CreateDepthStencilState(DSS_CubeMap::GetName(), DSS_CubeMap::GetHashCode(), &depthStencilDesc);
}

void GraphicResourceFactory::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;

	// Solid RasterizerState 积己..
	CreateRasterizerState(RS_Solid::GetName(), RS_Solid::GetHashCode(), &rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;

	// WireFrame RasterizerState 积己..
	CreateRasterizerState(RS_WireFrame::GetName(), RS_WireFrame::GetHashCode(), &rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	// NoCull RasterizerState 积己..
	CreateRasterizerState(RS_NoCull::GetName(), RS_NoCull::GetHashCode(), &rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.DepthBias = 100000;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 1.0f;

	// Depth RasterizerState 积己..
	CreateRasterizerState(RS_Depth::GetName(), RS_Depth::GetHashCode(), &rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthClipEnable = true;

	// CubeMap RasterizerState 积己..
	CreateRasterizerState(RS_CubeMap::GetName(), RS_CubeMap::GetHashCode(), &rasterizerDesc);
}

void GraphicResourceFactory::CreateSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// SamWrapMinLinear SamplerState 积己..
	CreateSamplerState(gSamWrapLinear::GetName(), gSamWrapLinear::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// SamWrapMinLinearPoint SamplerState 积己..
	CreateSamplerState(gSamWrapLinearPoint::GetName(), gSamWrapLinearPoint::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapLinerPoint SamplerState 积己..
	CreateSamplerState(gSamWrapAnisotropic::GetName(), gSamWrapAnisotropic::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = 0.0f;
	samplerDesc.MinLOD = 0.0f;

	// samWrapAnisotropic SamplerState 积己..
	CreateSamplerState(gSamClampAnisotropic::GetName(), gSamClampAnisotropic::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samClampMinLinear SamplerState 积己..
	CreateSamplerState(gSamClampLinear::GetName(), gSamClampLinear::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;//-FLT_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samClampMinLinearPoint SamplerState 积己..
	CreateSamplerState(gSamClampPoint::GetName(), gSamClampPoint::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = 0.0f;
	samplerDesc.MinLOD = 0.0f;

	// samWrapAnisotropic SamplerState 积己..
	CreateSamplerState(gSamBorderLinear::GetName(), gSamBorderLinear::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samBorderLinerPoint SamplerState 积己..
	CreateSamplerState(gSamBorderLinearPoint::GetName(), gSamBorderLinearPoint::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = { 1.0f };
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// gShadowSam SamplerState 积己..
	CreateSamplerState(gSamBorderComparisonLinearPoint::GetName(), gSamBorderComparisonLinearPoint::GetHashCode(), &samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samMirrorMinLinear SamplerState 积己..
	CreateSamplerState(gSamMirrorLinear::GetName(), gSamMirrorLinear::GetHashCode(), &samplerDesc);
}

void GraphicResourceFactory::CreateBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[2].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[3].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Blending First RenderTarget BlendState 积己..
	CreateBlendState(BS_AlphaBlend_ALL::GetName(), BS_AlphaBlend_ALL::GetHashCode(), &blendDesc);

	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	//blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	//blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Blending First RenderTarget BlendState 积己..
	CreateBlendState(BS_AlphaBlend_One::GetName(), BS_AlphaBlend_One::GetHashCode(), &blendDesc);
}

void GraphicResourceFactory::CreateDepthStencilViews(int width, int height)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Flags = 0;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Default DepthStencilView 积己..
	CreateDepthStencil(DS_Defalt::GetName(), DS_Defalt::GetHashCode(), &texDesc, nullptr, &descDSV, nullptr);
}

void GraphicResourceFactory::CreateViewPorts(int width, int height)
{
	// Default ViewPort 积己..
	CreateViewPort(VP_FullScreen::GetName(), VP_FullScreen::GetHashCode(), 0.0f, 0.0f, 1.0f, 1.0f, (float)width, (float)height);
	CreateViewPort(VP_HalfScreen::GetName(), VP_HalfScreen::GetHashCode(), 0.0f, 0.0f, 0.5f, 0.5f, (float)width, (float)height);
}

void GraphicResourceFactory::CreateInstanceBuffers()
{
	// Default Instance Buffer 积己..
	UINT instancMax = 500;

	std::vector<VertexInput::MeshDepthInstance>		meshDepthInstance(instancMax);
	std::vector<VertexInput::SkinMeshDepthInstance> skinMeshDepthInstance(instancMax);
	std::vector<VertexInput::MeshInstance>			meshInstance(instancMax);
	std::vector<VertexInput::SkinMeshInstance>		skinMeshInstance(instancMax);
	std::vector<VertexInput::MeshIDInstance>		meshIDInstance(instancMax);
	std::vector<VertexInput::SkinMeshIDInstance>	skinMeshIDInstance(instancMax);
	std::vector<VertexInput::ParticleInstance>		particleInstance(instancMax);

	CreateInstanceBuffer(IB_MeshDepth::GetName(), IB_MeshDepth::GetHashCode(), sizeof(VertexInput::MeshDepthInstance), instancMax, &meshDepthInstance[0]);
	CreateInstanceBuffer(IB_SkinMeshDepth::GetName(), IB_SkinMeshDepth::GetHashCode(), sizeof(VertexInput::SkinMeshDepthInstance), instancMax, &skinMeshDepthInstance[0]);
	CreateInstanceBuffer(IB_Mesh::GetName(), IB_Mesh::GetHashCode(), sizeof(VertexInput::MeshInstance), instancMax, &meshInstance[0]);
	CreateInstanceBuffer(IB_SkinMesh::GetName(), IB_SkinMesh::GetHashCode(), sizeof(VertexInput::SkinMeshInstance), instancMax, &skinMeshInstance[0]);
	CreateInstanceBuffer(IB_MeshID::GetName(), IB_MeshID::GetHashCode(), sizeof(VertexInput::MeshIDInstance), instancMax, &meshIDInstance[0]);
	CreateInstanceBuffer(IB_SkinMeshID::GetName(), IB_SkinMeshID::GetHashCode(), sizeof(VertexInput::SkinMeshIDInstance), instancMax, &skinMeshIDInstance[0]);
	CreateInstanceBuffer(IB_Particle::GetName(), IB_Particle::GetHashCode(), sizeof(VertexInput::ParticleInstance), instancMax, &particleInstance[0]);
}

void GraphicResourceFactory::CreateQuadBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT vCount = 4;
	UINT iCount = 6;

	UINT vByteSize = sizeof(VertexInput::PosTexVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::PosTexVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	vertices[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);

	vertices[0].Tex = Vector2(0.0f, 1.0f);
	vertices[1].Tex = Vector2(0.0f, 0.0f);
	vertices[2].Tex = Vector2(1.0f, 0.0f);
	vertices[3].Tex = Vector2(1.0f, 1.0f);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Quad::GetName(), DB_Quad::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosTexVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateSSAOQuadBuffer()
{
	UINT format = DXGI_FORMAT_R16_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT vCount = 4;
	UINT iCount = 6;

	UINT vByteSize = sizeof(VertexInput::PosNormalTexVertex) * vCount;
	UINT iByteSize = sizeof(USHORT) * iCount;

	std::vector<VertexInput::PosNormalTexVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	vertices[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);

	// Store far plane frustum corner indices in Normal.x slot.
	vertices[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertices[1].Normal = Vector3(1.0f, 0.0f, 0.0f);
	vertices[2].Normal = Vector3(2.0f, 0.0f, 0.0f);
	vertices[3].Normal = Vector3(3.0f, 0.0f, 0.0f);

	vertices[0].Tex = Vector2(0.0f, 1.0f);
	vertices[1].Tex = Vector2(0.0f, 0.0f);
	vertices[2].Tex = Vector2(1.0f, 0.0f);
	vertices[3].Tex = Vector2(1.0f, 1.0f);

	std::vector<USHORT> indices(iCount);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_SSAO::GetName(), DB_SSAO::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosNormalTexVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateBoxBuffer()
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;

	geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT vCount = (UINT)box.Vertices.size();
	UINT iCount = (UINT)box.Indices.size();
	UINT vByteSize = sizeof(VertexInput::MeshVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::MeshVertex> vertices(vCount);
	for (size_t i = 0; i < vCount; i++)
	{
		vertices[i].Pos = box.Vertices[i].Position;
		vertices[i].Normal = box.Vertices[i].Normal;
		vertices[i].Tex = box.Vertices[i].TexC;
		vertices[i].Tangent = box.Vertices[i].TangentU;
	}

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Box::GetName(), DB_Box::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::MeshVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateSphereBuffer()
{
	GeometryGenerator::MeshData sphere;

	GeometryGenerator geoGen;

	geoGen.CreateGeosphere(0.5f, 2, sphere);

	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT vCount = (UINT)sphere.Vertices.size();
	UINT iCount = (UINT)sphere.Indices.size();
	UINT vByteSize = sizeof(VertexInput::MeshVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::MeshVertex> vertices(vCount);
	for (size_t i = 0; i < vCount; i++)
	{
		vertices[i].Pos = sphere.Vertices[i].Position;
		vertices[i].Normal = sphere.Vertices[i].Normal;
		vertices[i].Tex = sphere.Vertices[i].TexC;
		vertices[i].Tangent = sphere.Vertices[i].TangentU;
	}

	std::vector<UINT> indices;
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Sphere::GetName(), DB_Sphere::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::MeshVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineRayBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vCount = 2;
	UINT iCount = 2;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(0.0f, 0.0f, 0.0f);
	vertices[1].Pos = Vector3(0.0f, 0.0f, 0.0f);
	vertices[0].Color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].Color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 1;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Line_Ray::GetName(), DB_Line_Ray::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, D3D11_USAGE_DYNAMIC, D3D11_USAGE_IMMUTABLE, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineQuadBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vCount = 4;
	UINT iCount = 10;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	vertices[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[0].Color = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	vertices[1].Color = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	vertices[2].Color = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	vertices[3].Color = Vector4(0.5f, 0.5f, 0.5f, 1.0f);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 1;
	indices[2] = 1; indices[3] = 2;
	indices[4] = 2; indices[5] = 0;
	indices[6] = 0; indices[7] = 3;
	indices[8] = 3; indices[9] = 2;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Line_Quad::GetName(), DB_Line_Quad::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineAxisBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vCount = 6;
	UINT iCount = 6;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	float extents = 0.1f;

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(0, 0, 0);
	vertices[1].Pos = Vector3(0, 0, 0);
	vertices[2].Pos = Vector3(0, 0, 0);
	vertices[3].Pos = Vector3(extents, 0, 0);
	vertices[4].Pos = Vector3(0, extents, 0);
	vertices[5].Pos = Vector3(0, 0, extents);
	vertices[0].Color = Vector4(1, 0, 0, 1);
	vertices[1].Color = Vector4(0, 1, 0, 1);
	vertices[2].Color = Vector4(0, 0, 1, 1);
	vertices[3].Color = Vector4(1, 0, 0, 1);
	vertices[4].Color = Vector4(0, 1, 0, 1);
	vertices[5].Color = Vector4(0, 0, 1, 1);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 3;
	indices[2] = 1; indices[3] = 4;
	indices[4] = 2; indices[5] = 5;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Line_Axis::GetName(), DB_Line_Axis::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineBoxBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vCount = 8;
	UINT iCount = 24;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	float extents = 1.0f;

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-extents, -extents, -extents);
	vertices[1].Pos = Vector3(-extents, -extents, +extents);
	vertices[2].Pos = Vector3(+extents, -extents, +extents);
	vertices[3].Pos = Vector3(+extents, -extents, -extents);
	vertices[4].Pos = Vector3(-extents, +extents, -extents);
	vertices[5].Pos = Vector3(-extents, +extents, +extents);
	vertices[6].Pos = Vector3(+extents, +extents, +extents);
	vertices[7].Pos = Vector3(+extents, +extents, -extents);
	vertices[0].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[1].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[2].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[3].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[4].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[5].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[6].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[7].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 1; indices[2] = 1; indices[3] = 2;
	indices[4] = 2; indices[5] = 3; indices[6] = 3; indices[7] = 0;
	indices[8] = 4; indices[9] = 5; indices[10] = 5; indices[11] = 6;
	indices[12] = 6; indices[13] = 7; indices[14] = 7; indices[15] = 4;
	indices[16] = 4; indices[17] = 0; indices[18] = 5; indices[19] = 1;
	indices[20] = 6; indices[21] = 2; indices[22] = 7; indices[23] = 3;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Line_Box::GetName(), DB_Line_Box::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, &vertices[0], &indices[0]);
	CreateDrawBuffer(DB_Line_Frustum::GetName(), DB_Line_Frustum::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, D3D11_USAGE_DYNAMIC, D3D11_USAGE_IMMUTABLE, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineCircleBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vCount = 180;
	UINT iCount = 360;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float x = cosf(angle * 3.14f / 180.0f);
		float z = -sinf(angle * 3.14f / 180.0f);

		vertices[i].Pos = Vector3(x, 0, z);
		vertices[i].Color = Vector4(0.75f, 0.75f, 0.75f, 1);
	}

	std::vector<UINT> indices(iCount);
	int index = 0;
	for (int i = 0; i < 180; i++)
	{
		indices[index] = i;
		indices[index + 1] = i + 1;
		index += 2;
	}
	indices[359] = 0;

	CreateDrawBuffer(DB_Line_Circle::GetName(), DB_Line_Circle::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineCircleSphereBuffer()
{
	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vCount = 540;
	UINT iCount = 1080;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float x = cosf(angle * 3.14f / 180.0f);
		float z = -sinf(angle * 3.14f / 180.0f);

		vertices[i].Pos = Vector3(x, 0, z);
		vertices[i].Color = Vector4(0, 1, 0, 1);
	}

	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float x = cosf(angle * 3.14f / 180.0f);
		float y = -sinf(angle * 3.14f / 180.0f);

		vertices[180 + i].Pos = Vector3(x, y, 0);
		vertices[180 + i].Color = Vector4(0, 0, 1, 1);
	}

	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float y = cosf(angle * 3.14f / 180.0f);
		float z = -sinf(angle * 3.14f / 180.0f);

		vertices[360 + i].Pos = Vector3(0, y, z);
		vertices[360 + i].Color = Vector4(1, 0, 0, 1);
	}

	std::vector<UINT> indices(iCount);
	int index = 0;
	for (int i = 0; i < 540; i++)
	{
		indices[index] = i;
		indices[index + 1] = i + 1;
		index += 2;
	}
	indices[359] = 0;
	indices[719] = 180;
	indices[1079] = 360;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Line_CircleSphere::GetName(), DB_Line_CircleSphere::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, &vertices[0], &indices[0]);
}

void GraphicResourceFactory::CreateLineGridBuffer()
{
	float wSize = 10.0f;
	float hSize = 10.0f;
	float width = 100.0f;
	float height = 100.0f;
	float wSizeHalf = width * wSize / 2.0f;
	float hSizeHalf = height * hSize / 2.0f;

	UINT vCount = (UINT)(((width + 1) + (height + 1)) * 2.0f);
	UINT iCount = (UINT)(((width + 1) + (height + 1)) * 2.0f);

	std::vector<VertexInput::PosColorVertex> vertices(vCount);
	std::vector<UINT> indices(iCount);
	int index = 0;
	for (int i = 0; i <= width; i++)
	{
		vertices[index].Pos = Vector3(-wSizeHalf, 0, -hSizeHalf + i * hSize);
		vertices[index + 1].Pos = Vector3(wSizeHalf, 0, -hSizeHalf + i * hSize);
		vertices[index].Color = Vector4(1, 1, 1, 1);
		vertices[index + 1].Color = Vector4(1, 1, 1, 1);
		indices[index] = index;
		indices[index + 1] = index + 1;
		index += 2;
	}

	for (int i = 0; i <= height; i++)
	{
		vertices[index].Pos = Vector3(-wSizeHalf + i * wSize, 0, -hSizeHalf);
		vertices[index + 1].Pos = Vector3(-wSizeHalf + i * wSize, 0, hSizeHalf);
		vertices[index].Color = Vector4(1, 1, 1, 1);
		vertices[index + 1].Color = Vector4(1, 1, 1, 1);
		indices[index] = index;
		indices[index + 1] = index + 1;
		index += 2;
	}

	UINT format = DXGI_FORMAT_R32_UINT;
	UINT topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	UINT vByteSize = sizeof(VertexInput::PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	// Draw Buffer 积己..
	CreateDrawBuffer(DB_Line_Grid::GetName(), DB_Line_Grid::GetHashCode(), format, topology, vByteSize, iByteSize, sizeof(VertexInput::PosColorVertex), iCount, &vertices[0], &indices[0]);
}
