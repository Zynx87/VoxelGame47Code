// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelGame47Code.h"
#include "VoxelRenderComponent.h"
#include "DynamicMeshBuilder.h"
#include "Engine.h"


//Learn about FPrimitiveSceneProxy and then Creating Scene Proxy
//differences between proc wiki and brickgame in the Proxies
//also differenes in the actors (cube, triangle, lathe actors) in proc wiki


/** Vertex Buffer */
class FVoxelChunkVertexBuffer : public FVertexBuffer
{
public:
	TArray<FDynamicMeshVertex> Vertices;
	virtual void InitRHI()
	{
		if (Vertices.Num() > 0)
		{
			FRHIResourceCreateInfo CreateInfo;
			VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Dynamic, CreateInfo);
			// Copy the vertex data into the vertex buffer.
			void* VertexBufferData = RHILockVertexBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FDynamicMeshVertex), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
			RHIUnlockVertexBuffer(VertexBufferRHI);
		}
	}
};



/** Index Buffer */
class FVoxelChunkIndexBuffer : public FIndexBuffer
{
public:
	TArray<uint16> Indices;
	virtual void InitRHI()
	{
		if (Indices.Num() > 0)
		{
			FRHIResourceCreateInfo CreateInfo;
			IndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), Indices.Num() * sizeof(uint16), BUF_Static, CreateInfo);
			// Write the indices to the index buffer.
			void* Buffer = RHILockIndexBuffer(IndexBufferRHI, 0, Indices.Num() * sizeof(uint16), RLM_WriteOnly);
			FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(uint16));
			RHIUnlockIndexBuffer(IndexBufferRHI);
		}
	}
};


/** Vertex Factory */
class FVoxelChunkVertexFactory : public FLocalVertexFactory
{
public:

	void Init(const FVoxelChunkVertexBuffer& VertexBuffer, uint32 FaceIndex)
	{
		// Initialize the vertex factory's stream components.
		DataType NewData;
		NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
		NewData.TextureCoordinates.Add(STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&VertexBuffer, FDynamicMeshVertex, TextureCoordinate, VET_Float2));
		NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&VertexBuffer, FDynamicMeshVertex, Color, VET_Color);
		// Use a stride of 0 to use the same TangentX/TangentZ for all faces using this vertex factory.
		//NewData.TangentBasisComponents[0] = FVertexStreamComponent(&TangentBuffer, sizeof(FPackedNormal)* (2 * FaceIndex + 0), 0, VET_PackedNormal);
		NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
		//NewData.TangentBasisComponents[1] = FVertexStreamComponent(&TangentBuffer, sizeof(FPackedNormal)* (2 * FaceIndex + 1), 0, VET_PackedNormal);
		NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(&VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
		check(!IsInRenderingThread());
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			InitVoxelChunkVertexFactory,
			FVoxelChunkVertexFactory*, VertexFactory, this,
			DataType, NewData, NewData,
			{
			VertexFactory->SetData(NewData);
		});
	}
};



UVoxelRenderComponent::UVoxelRenderComponent(const FObjectInitializer& Initializer)
: Super(Initializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	CastShadow = true;
	bUseAsOccluder = true;
	bCanEverAffectNavigation = true;
	bAutoRegister = false;

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

