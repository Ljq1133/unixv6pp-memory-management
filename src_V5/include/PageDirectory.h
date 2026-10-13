#ifndef PAGE_DIRECTORY_H
#define PAGE_DIRECTORY_H

#include "PageTable.h"

struct PageDirectoryEntry
{
	unsigned int m_Present : 1;
	unsigned int m_ReadWriter : 1;
	unsigned int m_UserSupervisor : 1;
	unsigned int m_WriteThrough : 1;
	unsigned int m_CacheDisabled : 1;
	unsigned int m_Accessed : 1;
	unsigned int m_Reserved : 1;
	unsigned int m_PageSize : 1;
	unsigned int m_GlobalPage : 1;
	unsigned int m_ForSystemUser : 3;
	unsigned int m_PageTableBaseAddress : 20;
} __attribute__((packed));

class PageDirectory
{
public:
	PageDirectory();
	~PageDirectory();

	PageTable &GetPageTableByIdx(int idx);
	PageDirectoryEntry &GetPageDirectoryEntryByIdx(int idx);

	// // ------------- NOTE 3: 从页目录中获取两张用户页表的地址 -------------
	// PageTable* GetUserPageTableFromDirectory();
	// // ------------- END NOTE 3 -------------

public:
	PageDirectoryEntry m_Entrys[1024];
};

#endif
