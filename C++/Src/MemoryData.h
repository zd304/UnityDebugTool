#ifndef __MEMORY_DATA_H__
#define __MEMORY_DATA_H__

#include "inc.h"

enum MemoryObjType
{
	MemoryObjType_RenderTexture,
	MemoryObjType_Texture2D,
	MemoryObjType_CubeMap,
	MemoryObjType_Mesh,
	MemoryObjType_AnimationClip,

	MemoryObjType_Count
};

enum RenderTextureFormat
{
	RenderTextureFormat_ARGB32,
	RenderTextureFormat_Depth,
	RenderTextureFormat_ARGBHalf,
	RenderTextureFormat_Shadowmap,
	RenderTextureFormat_RGB565,
	RenderTextureFormat_ARGB4444,
	RenderTextureFormat_ARGB1555,
	RenderTextureFormat_Default,
	RenderTextureFormat_ARGB2101010,
	RenderTextureFormat_DefaultHDR,
	RenderTextureFormat_ARGB64,
	RenderTextureFormat_ARGBFloat,
	RenderTextureFormat_RGFloat,
	RenderTextureFormat_RGHalf,
	RenderTextureFormat_RFloat,
	RenderTextureFormat_RHalf,
	RenderTextureFormat_R8,
	RenderTextureFormat_ARGBInt,
	RenderTextureFormat_RGInt,
	RenderTextureFormat_RInt,
	RenderTextureFormat_BGRA32,
	RenderTextureFormat_RGB111110Float = 22,
	RenderTextureFormat_RG32,
	RenderTextureFormat_RGBAUShort,
	RenderTextureFormat_RG16,

	RenderTextureFormat_Count
};

enum TextureFormat
{
	TextureFormat_Alpha8 = 1,
	TextureFormat_ARGB4444,
	TextureFormat_RGB24,
	TextureFormat_RGBA32,
	TextureFormat_ARGB32,
	TextureFormat_RGB565 = 7,
	TextureFormat_R16 = 9,
	TextureFormat_DXT1,
	TextureFormat_DXT5 = 12,
	TextureFormat_RGBA4444,
	TextureFormat_BGRA32,
	TextureFormat_RHalf,
	TextureFormat_RGHalf,
	TextureFormat_RGBAHalf,
	TextureFormat_RFloat,
	TextureFormat_RGFloat,
	TextureFormat_RGBAFloat,
	TextureFormat_YUY2,
	TextureFormat_RGB9e5Float,
	TextureFormat_BC4 = 26,
	TextureFormat_BC5,
	TextureFormat_BC6H = 24,
	TextureFormat_BC7,
	TextureFormat_DXT1Crunched = 28,
	TextureFormat_DXT5Crunched,
	TextureFormat_PVRTC_RGB2,
	TextureFormat_PVRTC_RGBA2,
	TextureFormat_PVRTC_RGB4,
	TextureFormat_PVRTC_RGBA4,
	TextureFormat_ETC_RGB4,
	TextureFormat_ATC_RGB4,
	TextureFormat_ATC_RGBA8,
	TextureFormat_EAC_R = 41,
	TextureFormat_EAC_R_SIGNED,
	TextureFormat_EAC_RG,
	TextureFormat_EAC_RG_SIGNED,
	TextureFormat_ETC2_RGB,
	TextureFormat_ETC2_RGBA1,
	TextureFormat_ETC2_RGBA8,
	TextureFormat_ASTC_RGB_4x4,
	TextureFormat_ASTC_RGB_5x5,
	TextureFormat_ASTC_RGB_6x6,
	TextureFormat_ASTC_RGB_8x8,
	TextureFormat_ASTC_RGB_10x10,
	TextureFormat_ASTC_RGB_12x12,
	TextureFormat_ASTC_RGBA_4x4,
	TextureFormat_ASTC_RGBA_5x5,
	TextureFormat_ASTC_RGBA_6x6,
	TextureFormat_ASTC_RGBA_8x8,
	TextureFormat_ASTC_RGBA_10x10,
	TextureFormat_ASTC_RGBA_12x12,
	TextureFormat_ETC_RGB4_3DS,
	TextureFormat_ETC_RGBA8_3DS,
	TextureFormat_RG16,
	TextureFormat_R8,
	TextureFormat_PVRTC_2BPP_RGB = -127,
	TextureFormat_PVRTC_2BPP_RGBA = -127,
	TextureFormat_PVRTC_4BPP_RGB = -127,
	TextureFormat_PVRTC_4BPP_RGBA = -127,

	TextureFormat_Count = TextureFormat_R8 + 1
};

class MemoryData
{
public:
	std::string name;
	MemoryObjType type;
	long size;
};

class MemoryData_RenderTexture : public MemoryData
{
public:
	int width;
	int height;
	int depth;
	RenderTextureFormat format;
};

class MemoryData_Texture2D : public MemoryData
{
public:
	int width;
	int height;
	int mipmapCount;
	TextureFormat format;
};

class MemoryData_CubeMap : public MemoryData
{
public:
	int width;
	int height;
	int mipmapCount;
	TextureFormat format;
};

class MemoryData_Mesh : public MemoryData
{
public:
	int readable;
	int vcount;
	int subMeshCount;
	std::vector<int> icounts;
};

class MemoryData_AnimationClip : public MemoryData
{
public:
	float length;
	int isLooping;
};

#endif
