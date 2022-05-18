#pragma once

/// 2022/01/06 18:03
/// SeoKyuHwang
///
/// # RenderTarget Class
///
/// - Rendering에 필요한 GraphicView Resource Class

class RenderTarget : public ResourceBase
{
public:
	RenderTarget(RESOURCE_TYPE type, Hash_Code hashCode, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderTarget();

public:
	void Reset() override;
	void Release() override;

public:
	void SetNumElements(UINT numElements);
	void SetResize(bool resize);

public:
	bool GetResize();

	RenderTargetView* GetRTV();
	ShaderResourceView* GetSRV();
	UnorderedAccessView* GetUAV();

protected:
	bool m_IsResize;

	RenderTargetView* m_RTV;
	ShaderResourceView* m_SRV;
	UnorderedAccessView* m_UAV;
};

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # RenderTarget Class
///
/// - Rendering에 필요한 GraphicView Resource Class
/// - Texture2D 기반으로 RenderTarget Bind Resoruce 생성

class RenderTexture : public RenderTarget
{
public:
	RenderTexture(Hash_Code hashCode, Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderTexture();

public:
	void Reset() override;
	void Release() override;

	void OnResize(int width, int height);
	void SetRatio(float ratio);
	void SetRatio(float width_ratio, float height_ratio);

public:
	Texture2D* GetTex2D();

private:
	Texture2D* m_Tex2D;
};

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # ResourceBuffer Class
///
/// - Rendering에 필요한 GraphicView Resource Class
/// - Buffer 기반으로 Buffer Bind Resoruce 생성

class RenderBuffer : public RenderTarget
{
public:
	RenderBuffer(Hash_Code hashCode, Buffer* buffer, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderBuffer();

public:
	void Reset() override;
	void Release() override;

	void OnResize(UINT byteWidth);

public:
	Buffer* GetBuffer();

private:
	Buffer* m_Buffer;
};

