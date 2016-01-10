#pragma once

#include "Multithreading/JobScheduler.h"
#include "RenderSystem/RenderCommandPacket.h"
#include "RenderSystem/RenderCommand.h"

#include "Math/Math.h"
#include "Utilities/Handle.h"
#include "MemorySystem.h"
#include "Utilities/RadixSort.h"

#include "Delegates/Delegate.h"

BEGINNAMESPACE


template<typename _K>
class RenderBucket {
private:
	static const size_type QueueGranularity = 32;
	static const size_type StoragePerQueue = KILOBYTE(64);
public:
    typedef _K Key;

	struct RenderBucketCallbacks{
		Delegate<void(MaterialHandle)> uploadMaterial;
	};
public:
	inline RenderBucket(size_type numRenderCommands)
		: m_Current(0), m_CommandCount(0), m_MaxNumRenderPackets(numRenderCommands)
	{
		//reserve storage
		m_Keys = eng_new_N(Key, numRenderCommands, g_DefaultAllocator);
		m_Packets = eng_new_N(RenderCommandPacket, numRenderCommands, g_DefaultAllocator);
		m_CommandStorage = eng_new_N(Byte, JobScheduler::NumThreads * StoragePerQueue);
		for (size_type i = 0; i < JobScheduler::NumThreads; ++i) {
			mtl_CommandAllocator[i].initialize( m_CommandStorage + i * StoragePerQueue, StoragePerQueue );
			mtl_RenderBucketOffset[i] = 0;
			mtl_RenderBucketRemaining[i] = 0;
		}
		resetVariables();
	}

	inline ~RenderBucket() {
		eng_delete_array(m_CommandStorage, g_DefaultAllocator);
		eng_delete_array(m_Packets, g_DefaultAllocator);
		eng_delete_array(m_Keys, g_DefaultAllocator);
	}

public:
	template<typename T>
	T* addCommand(Key key, size_type auxMemory) {
		RenderCommandPacket packet = renderCommandPacket::Create<T>(auxMemory, mtl_CommandAllocator[ThreadID]);
		
		size_type remaining = mtl_RenderBucketRemaining[ThreadID];
		size_type offset = mtl_RenderBucketOffset[ThreadID];

		if (remaining == 0) {
			offset = m_Current.fetch_add(QueueGranularity);
			remaining = QueueGranularity;
			mtl_RenderBucketOffset[ThreadID] = offset;
		}

		size_type current = offset + (QueueGranularity - remaining);
		m_Keys[current] = key;
		m_Packets[current] = packet;
		--remaining;

		++m_CommandCount;

		mtl_RenderBucketRemaining[ThreadID] = remaining;

		renderCommandPacket::StoreNextCommandPacket(packet, nullptr);
		renderCommandPacket::StoreRenderDispatcher(packet, T::sDispatcher);
		
		return renderCommandPacket::GetCommand<T>(packet);
	}

	template<typename T, typename U>
	T* appendCommand(U* command, size_type auxMemorySize) {
		RenderCommandPacket packet = renderCommandPacket::Create<T>(auxMemorySize, mtl_CommandAllocator[ThreadID]);

		renderCommandPacket::StoreNextCommandPacket<U>(command, packet);

		renderCommandPacket::StoreNextCommandPacket(packet, nullptr);
		renderCommandPacket::StoreRenderDispatcher(packet, T::sDispatcher);

		return renderCommandPacket::GetCommand<T>(packet);
	}

	void sort() {
		//std::sort(m_Keys, m_Keys + m_MaxNumRenderPackets);
	}

	void submit() {
		RenderBucketCallbacks empty;
		submit(empty);
	}

	void submit(const RenderBucketCallbacks& cb) {
		MaterialHandle currentMaterial = InvalidMaterialHandle;
		for (size_type i = 0; i < m_Current; ++i)
		{
			// decode the key, and set shaders, textures, constants, etc. if the material has changed.
			Key key = m_Keys[i];
			MaterialHandle material;

			RenderCommandPacket packet = m_Packets[i];
			if (!packet) continue;

			Key::Decode(key, material);

			if (material != currentMaterial) {
				//not uploaded
				cb.uploadMaterial(material);
				currentMaterial = material;
			}

			do {
				submitPacket(packet);
				packet = renderCommandPacket::LoadNextCommandPacket(packet);
			} while (packet != nullptr);
		}
		resetVariables();
	}

private:
	void submitPacket(const RenderCommandPacket packet) {
		const RenderDispatcher disp = renderCommandPacket::LoadRenderDispatcher(packet);
		const void* command = renderCommandPacket::LoadCommand(packet);
		disp(command); //execute command
	}


	void resetVariables() {
		m_CommandCount = 0;
		m_Current = 0;
		std::memset(m_Keys, -1ull, sizeof(Key) * m_MaxNumRenderPackets);
		std::memset(m_Packets, 0, sizeof(RenderCommandPacket) * m_MaxNumRenderPackets);
		for (size_type i = 0; i < JobScheduler::NumThreads; ++i) {
			mtl_CommandAllocator[i].reset();
			std::memset(mtl_CommandAllocator[i].getStart(), 0, mtl_CommandAllocator[i].getSize());
			mtl_RenderBucketOffset[i] = 0;
			mtl_RenderBucketRemaining[i] = 0;
		}
	}

private:
	Key* m_Keys;
	RenderCommandPacket* m_Packets;
	uint32 m_MaxNumRenderPackets;
	
	std::atomic<size_type> m_Current;
	std::atomic<size_type> m_CommandCount;

	struct Matrix {
        float32 m_viewMatrix[4][4];
        float32 m_projectionMatrix[4][4];
	};

	//typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> CommandAllocator;
	typedef LinearAllocator CommandAllocator;
	Byte* m_CommandStorage;
	CommandAllocator mtl_CommandAllocator[ JobScheduler::NumThreads ];
	size_type mtl_RenderBucketOffset[ JobScheduler::NumThreads ];
	size_type mtl_RenderBucketRemaining[ JobScheduler::NumThreads ];
};

ENDNAMESPACE
