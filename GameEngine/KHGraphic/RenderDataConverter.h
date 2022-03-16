#pragma once
class RenderDataConverter
{
public:
	RenderDataConverter();
	~RenderDataConverter();

public:
	void ConvertRenderData(MeshData* originData, RenderData* renderData);

private:
	void ConvertMeshBuffer(MeshBuffer* originBuf, MeshRenderData* convertData);
	void ConvertMaterial(MaterialData* originMat, MaterialRenderData* convertMat);

};

