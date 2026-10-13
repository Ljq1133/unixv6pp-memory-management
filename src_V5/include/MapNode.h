#ifndef MAP_NODE_H
#define MAP_NODE_H

/*
 *@comment 这个结构对应Unixv6中的map结构
 *下面给出map结构参考
 * struct map	@line 2515
 * {
	char *m_size;
	char *m_addr;
 * }
 */
struct MapNode
{
	unsigned long m_Size;
	/*
	 * 注释可能是不对的。
	 * m_addr 表示数据块在整个空间中的索引位置，
	 * 例如physical内存中4k一块，若m_AddressIdx为2，
	 * 则表示0x2000(8k)的位置，同理swap区中，
	 * 数据块大小为512byte
	 */
	unsigned long m_AddressIdx; // 分配空间的起始地址
};

// NOTE:1
#define M_PAGE_SIZE 4096

class BitMap
{
public:
	static const int BITMAP_WORD_BITS = 64;
	static const int BITMAP_WORD_COUNT = 256;
	static const int BITMAP_PAGE_COUNT = BITMAP_WORD_BITS * BITMAP_WORD_COUNT;

	unsigned long m_AddressIdx; // 分配空间的起始地址, 不变
	int rows;
	unsigned long m_PageCount;
	unsigned long long int map[BITMAP_WORD_COUNT];
	void set(unsigned long startAddr, unsigned long page_num)
	{
		m_AddressIdx = startAddr;
		m_PageCount = page_num;
		rows = (page_num + BITMAP_WORD_BITS - 1) / BITMAP_WORD_BITS;
		if (rows > BITMAP_WORD_COUNT)
		{
			rows = BITMAP_WORD_COUNT;
			m_PageCount = BITMAP_PAGE_COUNT;
		}
		for (int i = 0; i < BITMAP_WORD_COUNT; i++)
		{
			map[i] = 0;
		}
		for (unsigned long i = m_PageCount; i < (unsigned long)rows * BITMAP_WORD_BITS; i++)
		{
			map[i / BITMAP_WORD_BITS] |= (1ULL << (i % BITMAP_WORD_BITS));
		}
	};
};

#endif
