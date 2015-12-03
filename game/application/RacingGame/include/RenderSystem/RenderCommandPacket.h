#pragma once

#include <MemorySystem.h>

BEGINNAMESPACE

typedef void* RenderCommandPacket;
typedef void(*RenderDispatcher)(const void*);

namespace renderCommandPacket
{
	static const size_type OFFSET_NEXT_COMMAND_PACKET = 0u;
	static const size_type OFFSET_BACKEND_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET + sizeof(RenderCommandPacket);
	static const size_type OFFSET_COMMAND = OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(RenderDispatcher);

	template <typename T>
	inline size_type GetSize(size_type auxMemorySize){
		return OFFSET_COMMAND + sizeof(T) + auxMemorySize;
	};

	template <typename T, typename Alloc >
	inline RenderCommandPacket Create(size_type auxMemorySize, Alloc& allocator) {
		return eng_new_raw(GetSize<T>(auxMemorySize), allocator);
	}

	inline RenderCommandPacket* GetNextCommandPacket(RenderCommandPacket packet){
		return reinterpret_cast<RenderCommandPacket*>(reinterpret_cast<Byte*>(packet) + OFFSET_NEXT_COMMAND_PACKET);
	}

	template <typename T>
	inline RenderCommandPacket* GetNextCommandPacket(T* command) {
		return reinterpret_cast<RenderCommandPacket*>(reinterpret_cast<Byte*>(command) - OFFSET_COMMAND + OFFSET_NEXT_COMMAND_PACKET);
	}

	inline RenderDispatcher* GetRenderDispatcher(RenderCommandPacket packet) {
		return reinterpret_cast<RenderDispatcher*>(reinterpret_cast<char*>(packet) + OFFSET_BACKEND_DISPATCH_FUNCTION);
	}

	template <typename T>
	inline T* GetCommand(RenderCommandPacket packet){
		return reinterpret_cast<T*>(reinterpret_cast<char*>(packet) + OFFSET_COMMAND);
	}

	template <typename T>
	inline Byte* GetAuxiliaryMemory(T* command)	{
		return reinterpret_cast<Byte*>(command) + sizeof(T);
	}

	inline void StoreNextCommandPacket(RenderCommandPacket packet, RenderCommandPacket nextPacket)	{
		*renderCommandPacket::GetNextCommandPacket(packet) = nextPacket;
	}

	template <typename T>
	inline void StoreNextCommandPacket(T* command, RenderCommandPacket nextPacket){
		*renderCommandPacket::GetNextCommandPacket<T>(command) = nextPacket;
	}

	inline void StoreRenderDispatcher(RenderCommandPacket packet, RenderDispatcher dispatchFunction){
		*renderCommandPacket::GetRenderDispatcher(packet) = dispatchFunction;
	}

	inline const RenderCommandPacket LoadNextCommandPacket(const RenderCommandPacket packet){
		return *GetNextCommandPacket(packet);
	}

	inline const RenderDispatcher LoadRenderDispatcher(const RenderCommandPacket packet){
		return *GetRenderDispatcher(packet);
	}

	inline const void* LoadCommand(const RenderCommandPacket packet){
		return reinterpret_cast<char*>(packet) + OFFSET_COMMAND;
	}
}
ENDNAMESPACE