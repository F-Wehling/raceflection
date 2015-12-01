#pragma once

#include "Multithreading/JobScheduler.h"
#include "RenderSystem/RenderCommandPacket.h"

#include "Math/Math.h"
#include "Utilities/Handle.h"
#include "MemorySystem.h"

BEGINNAMESPACE

typedef ProxyAllocator<LinearAllocator, policy::NoSync, policy::NoBoundsChecking, policy::NoTracking, policy::NoTagging> CommandAllocator;
extern thread_local CommandAllocator gtl_CommandAllocator;
extern thread_local size_type gtl_RenderBucketOffset;
extern thread_local size_type gtl_RenderBucketRemaining;

template<typename T>
class RenderBucket {
private:
	static const size_type QueueGranularity = 32;
public:
	typedef T Key;
public:
	inline RenderBucket(size_type numDrawCalls) {
		//reserve storage
	}

	RenderBucket(size_type numDrawCalls, const float4x4& viewMatrix, const float4x4& projectionMatrix) {

	}

public:
	template<typename T>
	T* addCommand(Key key, size_type auxMemroy) {
		RenderCommandPacket packet = renderCommandPacket::Create<T>(auxMemory, gtl_CommandAllocator);

		size_type remaining = gtl_RenderBucketRemaining;
		size_type offset = gtl_RenderBucketOffset;

		if (remaining == 0) {
			offset = m_Current.fetch_add(QueueGranularity);
			remaining = QueueGranularity;
			gtl_RenderBucketOffset = offset;
		}

		size_type current = offset + ( QueueGranularity - remaining);
		m_Keys[current] = key;
		m_Packets[current] = packet;
		--remaining;

		gtl_RenderBucketRemaining = remaining;

		renderCommandPacket::StoreNextCommandPacket(packet, nullptr);
		renderCommandPacket::StoreRenderDispatcher(packet, T::sDispatcher);

		return renderCommandPacket::GetCommand<T>(packet);
	}

	template<typename T, typename U>
	T* appendCommand(U* command, size_type auxMemorySize) {
		RenderCommandPacket packet = renderCommandPacket::Create<T>(auxMemorySize, gtl_CommandAllocator);

		renderCommandPacket::StoreNextCommandPacket<U>(command, packet);

		renderCommandPacket::StoreNextCommandPacket(packet, nullptr);
		renderCommandPacket::StoreRenderDispatcher(packet, T::sDispatcher);

		return renderCommandPacket::GetCommand<T>(packet);
	}

	void sort() {

	}

	void submit() {
		//@TODO Update Matrices
		for (size_type i = 0; i < m_Current; ++i)
		{
			// decode the key, and set shaders, textures, constants, etc. if the material has changed.
			Key key = m_keys[i];
			Key::DecodeKey(key);

			RenderCommandPacket packet = m_Packets[i];
			do {
				submitPacket(packet);
				packet = renderCommandPacket::LoadNextCommandPacket(packet);
			} while (packet != nullptr);
		}
	}

private:
	void submitPacket(const RenderCommandPacket* packet) {
		const RenderDispatcher disp = renderCommandPacket::LoadRenderDispatcher(packet);
		const void* command = renderCommandPacket::LoadCommand(packet);
		disp(command); //execute command
	}

private:
	Key* m_Keys;
	RenderCommandPacket m_Packets;
	
	std::atomic<size_type> m_Current;

	struct Matrix {
		float4x4 m_viewMatrix;
		float4x4 m_projectionMatrix;
	};
};

ENDNAMESPACE