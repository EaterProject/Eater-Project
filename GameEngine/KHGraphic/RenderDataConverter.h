#pragma once
class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertRenderData(MeshData* originData, RenderData* renderData);

private:
	void ConvertIndexBuffer(IndexBuffer* originBuf, MeshRenderData* convertData);
	void ConvertVertexBuffer(VertexBuffer* originBuf, MeshRenderData* convertData);
	void ConvertMaterial(MaterialData* originMat, MeshRenderData* convertData);
	void ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat);

};

