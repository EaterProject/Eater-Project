#pragma once

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # DepthStencil Class
///
/// - Rendering에 필요한 GraphicView Resource Class
/// - Texture2D 기반으로 DepthStencil Bind Resoruce 생성

class DepthStencil : public ResourceBase
{
public:
	DepthStencil(Hash_Code hashCode, Texture2D* tex2D, DepthStencilView* dsv, ShaderResourceView* srv);
	~DepthStencil();

public:
	void Reset() override;
	void Release() override;

	void OnResize(int width, int height); 
	void SetRatio(float ratio);
	void SetRatio(float width_ratio, float height_ratio);

public:
	Texture2D* GetTex2D();

	DepthStencilView* GetDSV();
	ShaderResourceView* GetSRV();

private:
	Texture2D* m_Tex2D;

	DepthStencilView* m_DSV;
	ShaderResourceView* m_SRV;
};