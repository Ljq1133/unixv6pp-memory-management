#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

struct PageTableEntry
{
	unsigned int m_Present : 1;
	unsigned int m_ReadWriter : 1;
	unsigned int m_UserSupervisor : 1;
	unsigned int m_WriteThrough : 1;
	unsigned int m_CacheDisabled : 1;
	unsigned int m_Accessed : 1;
	unsigned int m_Dirty : 1;
	unsigned int m_PageTableAttribueIndex : 1;
	unsigned int m_GlobalPage : 1;
	unsigned int m_ForSystemUser : 3;
	unsigned int m_PageBaseAddress : 20;
} __attribute__((packed));

class PageTable
{
public:
	static const unsigned int ENTRY_CNT_PER_PAGETABLE = 1024;
	static const unsigned int SIZE_PER_PAGETABLE_MAP = 0x400000;

public:
	PageTable();
	~PageTable();

public:
	PageTableEntry m_Entrys[ENTRY_CNT_PER_PAGETABLE];
};

#endif
