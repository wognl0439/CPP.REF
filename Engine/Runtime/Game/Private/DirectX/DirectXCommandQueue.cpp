﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXCommandQueue.h"

#include "DirectXCommon.h"
#include "Logging/LogMacros.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXDeviceContext.h"

using namespace std;

DirectXCommandQueue::DirectXCommandQueue(DirectXDeviceBundle* deviceBundle) : DirectXCommandQueue(deviceBundle, D3D12_COMMAND_LIST_TYPE_DIRECT)
{

}

DirectXCommandQueue::DirectXCommandQueue(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type) : Super()
{
	ID3D12Device5* device = deviceBundle->GetDevice();
	
	D3D12_COMMAND_QUEUE_DESC desc = { };
	desc.Type = type;
	HR(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));

	SetDeviceChildPtr(commandQueue.Get(), deviceBundle);
}

DirectXCommandQueue::~DirectXCommandQueue()
{

}

void DirectXCommandQueue::ExecuteCommandLists(DirectXDeviceContext* const* deviceContexts, size_t count)
{
	vector<ID3D12CommandList*> commandLists;
	commandLists.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		if (deviceContexts[i] != nullptr)
		{
			if (auto ptr = deviceContexts[i]->GetCommandList(); ptr != nullptr)
			{
				commandLists.emplace_back(ptr);
				continue;
			}
		}
	}

	if (!commandLists.empty())
	{
		commandQueue->ExecuteCommandLists((UINT)commandLists.size(), commandLists.data());
	}
}