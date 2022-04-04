Texture2D gLastMipMap : register(t0);

float4 HizMipMap_PS(float4 PosH : SV_POSITION) : SV_TARGET
{
    uint lastMipWidth, lastMipHeight;
    gLastMipMap.GetDimensions(lastMipWidth, lastMipHeight);
    
    // get integer pixel coordinates
    uint3 nCoords = uint3(PosH.xy, 0);
    
    // fetch a 2x2 neighborhood and compute the max
    nCoords.xy *= 2;
    
    float4 vTexels;
    vTexels.x = gLastMipMap.Load( nCoords );
    vTexels.y = gLastMipMap.Load( nCoords, uint2(1,0) );
    vTexels.z = gLastMipMap.Load( nCoords, uint2(0,1) );
    vTexels.w = gLastMipMap.Load(nCoords, uint2(1, 1));

    // Determine the largest depth value and use it as the new down sampled
    // color.
    float fMaxDepth = max(max(vTexels.x, vTexels.y), max(vTexels.z, vTexels.w));
    
    // TODO NDarnell We could throw away the rest of this code for other textures.
    // Maybe break this into another shader and use seperate shaders for oddly sized textures.
    
    // if we are reducing an odd-sized texture,
    // then the edge pixels need to fetch additional texels
    float2 vExtra;
    if( (lastMipWidth & 1) && nCoords.x == lastMipWidth-3 )
    {
        vExtra.x = gLastMipMap.Load( nCoords, uint2(2,0) );
        vExtra.y = gLastMipMap.Load(nCoords, uint2(2, 1));
        fMaxDepth = max( fMaxDepth, max( vExtra.x, vExtra.y ) );
    }
    
    if( (lastMipHeight & 1) && nCoords.y == lastMipHeight-3 )
    {
        vExtra.x = gLastMipMap.Load( nCoords, uint2(0,2) );
        vExtra.y = gLastMipMap.Load(nCoords, uint2(1, 2));
        fMaxDepth = max( fMaxDepth, max( vExtra.x, vExtra.y ) );
    }
    
    // extreme case: If both edges are odd, fetch the bottom-right corner texel
    if( ( ( lastMipWidth & 1 ) && ( lastMipHeight & 1 ) ) &&
        nCoords.x == lastMipWidth-3 && nCoords.y == lastMipHeight-3 )
    {
        fMaxDepth = max(fMaxDepth, gLastMipMap.Load(nCoords, uint2(2, 2)));
    }
    
    return fMaxDepth;
}