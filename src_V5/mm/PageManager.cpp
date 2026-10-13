#include "PageManager.h"
#include "Allocator.h"

unsigned int PageManager::PHY_MEM_SIZE;
unsigned int UserPageManager::USER_PAGE_POOL_SIZE;

PageManager::PageManager(BitMapAllocator *allocator)
{
	this->m_pAllocator = allocator;
}

int PageManager::Initialize()
{
	for (unsigned int i = 0; i < MEMORY_MAP_ARRAY_SIZE; i++)
	{
		this->map[i].m_AddressIdx = 0;
		this->map[i].m_Size = 0;
	}
	// NOTE:COW
	for (int i = 0; i < MAX_MANAGED_PAGE_COUNT; ++i)
	{
		Page[i] = 0;
	}
	return 0;
}

unsigned long PageManager::AllocMemory(unsigned long size)
{
	unsigned long newaddr = this->m_pAllocator->Alloc(this->bitmap, size);
	unsigned long pageIdx = newaddr >> 12;
	unsigned long pages = (size + PageManager::PAGE_SIZE - 1) / PageManager::PAGE_SIZE;
	if (newaddr)
	{
		for (unsigned long i = 0; i < pages && pageIdx + i < MAX_MANAGED_PAGE_COUNT; i++)
		{
			Page[pageIdx + i] = 1;
		}
	}
	return newaddr;
}

unsigned long PageManager::FreeMemory(unsigned long size, unsigned long startAddress)
{
	if (size == 0 || startAddress == 0)
	{
		return 0;
	}
	unsigned long pages = (size + PageManager::PAGE_SIZE - 1) / PageManager::PAGE_SIZE;
	unsigned long basePageIdx = startAddress >> 12;
	for (unsigned long i = 0; i < pages; i++)
	{
		unsigned long pageIdx = basePageIdx + i;
		if (pageIdx < MAX_MANAGED_PAGE_COUNT && Page[pageIdx] > 0)
		{
			Page[pageIdx]--;
			if (Page[pageIdx] > 0)
			{
				continue;
			}
		}
		this->m_pAllocator->Free(this->bitmap, PageManager::PAGE_SIZE, startAddress + i * PageManager::PAGE_SIZE);
	}
	return 1;
}

PageManager::~PageManager()
{
}

KernelPageManager::KernelPageManager(BitMapAllocator *allocator)
	: PageManager(allocator)
{
}

int KernelPageManager::Initialize()
{
	PageManager::Initialize();

	this->map[0].m_AddressIdx =
		KERNEL_PAGE_POOL_START_ADDR / PageManager::PAGE_SIZE;
	this->map[0].m_Size =
		KERNEL_PAGE_POOL_SIZE / PageManager::PAGE_SIZE;

	// NOTE:1
	this->bitmap.set(KERNEL_PAGE_POOL_START_ADDR, KERNEL_PAGE_POOL_SIZE / PageManager::PAGE_SIZE);

	return 0;
}

UserPageManager::UserPageManager(BitMapAllocator *allocator)
	: PageManager(allocator)
{
}

int UserPageManager::Initialize()
{
	PageManager::Initialize();

	this->map[0].m_AddressIdx =
		USER_PAGE_POOL_START_ADDR / PageManager::PAGE_SIZE;
	this->map[0].m_Size =
		USER_PAGE_POOL_SIZE / PageManager::PAGE_SIZE;

	// NOTE:1
	this->bitmap.set(USER_PAGE_POOL_START_ADDR, USER_PAGE_POOL_SIZE / PageManager::PAGE_SIZE);

	return 0;
}
