#include "Machine.h"
#include "Assembly.h"
#include "Exception.h"
#include "TimeInterrupt.h"
#include "DiskInterrupt.h"
#include "KeyboardInterrupt.h"
#include "SystemCall.h"

Machine Machine::instance; /*鍗曟€佺被瀹炰緥鐨勫畾涔?/

/* 鍏ㄥ眬GDT銆両DT銆乀SS鍙橀噺 */
GDT g_GDT;
IDT g_IDT;

TaskStateSegment g_TaskStateSegment;

Machine &Machine::Instance()
{
	return instance;
}

void Machine::LoadIDT()
{
	IDTR idtr;
	GetIDT().FormIDTR(idtr);
	X86Assembly::LIDT((unsigned short *)(&idtr));
}

void Machine::LoadGDT()
{
	GDTR gdtr;
	GetGDT().FormGDTR(gdtr);
	X86Assembly::LGDT((unsigned short *)(&gdtr));
}

void Machine::LoadTaskRegister()
{
	X86Assembly::LTR(Machine::TASK_STATE_SEGMENT_SELECTOR);
}

extern "C" void MasterIRQ7();

void Machine::InitIDT()
{
	this->m_IDT = &g_IDT;
	/*
	 * 1. 灏咺DT涓? - 255涓〃椤瑰叏閮ㄥ～鍏ラ粯璁や腑鏂?寮傚父澶勭悊鍑芥暟鍏ュ彛锛岀‘
	 *    淇濅换鎰忎竴绉嶄腑鏂?寮傚父鍙戠敓鏃堕兘浼氳澶勭悊锛岄伩鍏嶅唴鏍稿穿婧冦€?
	 * 2. 瀵笽NT 0 - 31鍙峰紓甯革紝浣跨敤闈為粯璁ょ殑涓柇/寮傚父澶勭悊绋嬪簭瑕嗙洊鍏堝墠
	 *    榛樿澶勭悊鍑芥暟鍏ュ彛銆?
	 * 3. 璁剧疆鏃堕挓涓柇銆侀敭鐩樹腑鏂€佺鐩樹腑鏂瓑瀵瑰簲鐨勪腑鏂叆鍙ｃ€?
	 * 4. INT 0 - 31鍙峰紓甯镐腑浣跨敤榛樿澶勭悊鍑芥暟鐨勶紝涓€鑸儏鍐典笅涓嶅彲鑳藉彂鐢燂紝
	 *    瀵逛簬杩欎簺寮傚父鐨勫鐞嗘祦绋嬩笉杩涜鐜板満淇濆瓨鍜屾仮澶嶏紝浠呰緭鍑洪敊璇俊鎭紝
	 *    杩涘叆姝诲惊鐜紝绛夊緟浜哄伐骞查銆?
	 */
	for (int i = 0; i <= 255; i++)
	{
		if (i < 32)
			this->GetIDT().SetTrapGate(i, (unsigned long)IDT::DefaultExceptionHandler);
		else
			this->GetIDT().SetInterruptGate(i, (unsigned long)IDT::DefaultInterruptHandler);
	}
	/* 鍒濆鍖朓NT 0 - 31鍙峰紓甯?*/
	this->GetIDT().SetTrapGate(0, (unsigned long)Exception::DivideErrorEntrance);
	this->GetIDT().SetTrapGate(1, (unsigned long)Exception::DebugEntrance);
	this->GetIDT().SetTrapGate(2, (unsigned long)Exception::NMIEntrance);
	this->GetIDT().SetTrapGate(3, (unsigned long)Exception::BreakpointEntrance);
	this->GetIDT().SetTrapGate(4, (unsigned long)Exception::OverflowEntrance);
	this->GetIDT().SetTrapGate(5, (unsigned long)Exception::BoundEntrance);
	this->GetIDT().SetTrapGate(6, (unsigned long)Exception::InvalidOpcodeEntrance);
	this->GetIDT().SetTrapGate(7, (unsigned long)Exception::DeviceNotAvailableEntrance);
	this->GetIDT().SetTrapGate(8, (unsigned long)Exception::DoubleFaultEntrance);
	this->GetIDT().SetTrapGate(9, (unsigned long)Exception::CoprocessorSegmentOverrunEntrance);
	this->GetIDT().SetTrapGate(10, (unsigned long)Exception::InvalidTSSEntrance);
	this->GetIDT().SetTrapGate(11, (unsigned long)Exception::SegmentNotPresentEntrance);
	this->GetIDT().SetTrapGate(12, (unsigned long)Exception::StackSegmentErrorEntrance);
	this->GetIDT().SetTrapGate(13, (unsigned long)Exception::GeneralProtectionEntrance);

	/* 缂洪〉寮傚父(INT 14) UNIX V6++涓鏁翠釜杩涚▼鍥惧儚鎹㈠叆鎹㈠嚭锛岄潪椤靛紡浜ゆ崲锛屽洜姝や笉闇€瑕佺己椤靛紓甯稿鐞嗗嚱鏁?*/
	this->GetIDT().SetTrapGate(14, (unsigned long)Exception::PageFaultEntrance);
	/* Intel淇濈暀寮傚父(INT 15)  浣跨敤IDT::DefaultExceptionHandler() */
	this->GetIDT().SetTrapGate(16, (unsigned long)Exception::CoprocessorErrorEntrance);
	this->GetIDT().SetTrapGate(17, (unsigned long)Exception::AlignmentCheckEntrance);
	this->GetIDT().SetTrapGate(18, (unsigned long)Exception::MachineCheckEntrance);
	this->GetIDT().SetTrapGate(19, (unsigned long)Exception::SIMDExceptionEntrance);

	/* INT 20 - 31鍙峰紓甯镐负Intel淇濈暀鏈娇鐢ㄧ殑寮傚父 */

	/* 璁剧疆鏃堕挓涓柇鐨勪腑鏂棬 */
	this->GetIDT().SetInterruptGate(0x20, (unsigned long)Time::TimeInterruptEntrance);
	/* 璁剧疆閿洏涓柇鐨勪腑鏂棬 */
	this->GetIDT().SetInterruptGate(0x21, (unsigned long)KeyboardInterrupt::KeyboardInterruptEntrance);
	/* 璁剧疆IDT涓鐩樹腑鏂搴斾腑鏂棬 */
	this->GetIDT().SetInterruptGate(0x2E, (unsigned long)DiskInterrupt::DiskInterruptEntrance);
	/* 0x80鍙蜂腑鏂悜閲忎綔涓虹郴缁熻皟鐢紝璁剧疆绯荤粺璋冪敤瀵瑰簲鐨勯櫡鍏ラ棬 */
	this->GetIDT().SetTrapGate(0x80, (unsigned long)SystemCall::SystemCallEntrance);
	/* 8259A涓荤墖鐨刬rq7寮曡剼浼氫骇鐢熺殑鏈煡涓柇锛屾彁渚涗腑鏂鐞嗗嚱鏁扳€滃拷鐣ュ畠鈥?*/
	this->GetIDT().SetInterruptGate(0x27, (unsigned long)MasterIRQ7);
}

void Machine::InitGDT()
{
	this->m_GDT = &g_GDT;

	// 鍒濆鍖朑DT涓殑4涓锛氬唴鏍镐唬鐮佹銆佸唴鏍告暟鎹锛岀敤鎴蜂唬鐮佹銆佺敤鎴锋暟鎹
	// limit = 0xfffff, base = 0x00000000, G = 1 , D = 1(32bit), P =1, DPL = 00, S = 1, TYPE = 1010 (code segment read only)
	// limit = 0xfffff, base = 0x00000000, G = 1, D = 1(32bit), P =1, DPL = 00, S = 1, TYPE = 0010 (data segment write/read)
	// limit = 0xfffff, base = 0x00000000, G = 1 , D = 1(32bit), P =1, DPL = 11, S = 1, TYPE = 1010 (code segment read only)
	// limit = 0xfffff, base = 0x00000000, G = 1, D = 1(32bit), P =1, DPL = 11, S = 1, TYPE = 0010 (data segment write/read)

	// TODO 娣诲姞鐩稿簲鐨勫彲璇荤殑甯搁噺锛屽GDTConsts::GRANULARITY_4K...
	SegmentDescriptor tmpDescriptor;
	// 0x08:00
	tmpDescriptor.SetSegmentLimit(0xfffff);
	tmpDescriptor.SetBaseAddress(0x00000000);
	tmpDescriptor.m_Granularity = 1;
	tmpDescriptor.m_DefaultOperationSize = 1;
	tmpDescriptor.m_SegmentPresent = 1;
	tmpDescriptor.m_DPL = 0x00;
	tmpDescriptor.m_System = 1;
	tmpDescriptor.m_Type = 0xA;
	GetGDT().SetSegmentDescriptor(1, tmpDescriptor);

	// 0x10:00
	tmpDescriptor.SetSegmentLimit(0xfffff);
	tmpDescriptor.SetBaseAddress(0x00000000);
	tmpDescriptor.m_Granularity = 1;
	tmpDescriptor.m_DefaultOperationSize = 1;
	tmpDescriptor.m_SegmentPresent = 1;
	tmpDescriptor.m_DPL = 0x00;
	tmpDescriptor.m_System = 1;
	tmpDescriptor.m_Type = 0x2;
	GetGDT().SetSegmentDescriptor(2, tmpDescriptor);

	// 0x18:00
	tmpDescriptor.SetSegmentLimit(0xfffff);
	tmpDescriptor.SetBaseAddress(0x00000000);
	tmpDescriptor.m_Granularity = 1;
	tmpDescriptor.m_DefaultOperationSize = 1;
	tmpDescriptor.m_SegmentPresent = 1;
	tmpDescriptor.m_DPL = 0x3;
	tmpDescriptor.m_System = 1;
	tmpDescriptor.m_Type = 0xA;
	GetGDT().SetSegmentDescriptor(3, tmpDescriptor);

	// 0x20:00
	tmpDescriptor.SetSegmentLimit(0xfffff);
	tmpDescriptor.SetBaseAddress(0x00000000);
	tmpDescriptor.m_Granularity = 1;
	tmpDescriptor.m_DefaultOperationSize = 1;
	tmpDescriptor.m_SegmentPresent = 1;
	tmpDescriptor.m_DPL = 0x3;
	tmpDescriptor.m_System = 1;
	tmpDescriptor.m_Type = 0x2;
	GetGDT().SetSegmentDescriptor(4, tmpDescriptor);

	/* 鍒濆鍖朤SS娈?*/
	this->m_TaskStateSegment = &g_TaskStateSegment;
	this->InitTaskStateSegment();
}

// ------------- NOTE 3: 鍒涘缓杩涚▼鍏敤鐨?#鐢ㄦ埛椤佃〃鍜屾牳蹇冮〉琛? 娉ㄦ剰椤电洰褰曡〃涔熻鍒涘缓锛屼絾鏄?#杩涚▼绉佹湁 -------------
void Machine::InitKernelPageTable()
{
	// 鏍稿績椤佃〃
	PageTable *pkernelPageTable = (PageTable *)(KERNEL_PAGE_TABLE_BASE_ADDRESS + KERNEL_SPACE_START_ADDRESS);
	for (unsigned int i = 0; i < PageTable::ENTRY_CNT_PER_PAGETABLE; i++)
	{
		pkernelPageTable->m_Entrys[i].m_UserSupervisor = 0;
		pkernelPageTable->m_Entrys[i].m_Present = 1;
		pkernelPageTable->m_Entrys[i].m_ReadWriter = 1;
		pkernelPageTable->m_Entrys[i].m_PageBaseAddress = i;
	}
	this->m_KernelPageTable = pkernelPageTable;
}

void Machine::InitZeroUserPageTable()
{
	// 0#鐢ㄦ埛椤佃〃
	PageTable *pUserPageTable = (PageTable *)(USER_PAGE_TABLE_BASE_ADDRESS + KERNEL_SPACE_START_ADDRESS);
	for (unsigned int i = 0; i < PageTable::ENTRY_CNT_PER_PAGETABLE; i++)
	{
		pUserPageTable->m_Entrys[i].m_UserSupervisor = 1;
		pUserPageTable->m_Entrys[i].m_Present = 1;
		pUserPageTable->m_Entrys[i].m_ReadWriter = 1;
		pUserPageTable->m_Entrys[i].m_PageBaseAddress = 0x00000 + i;
	}

	this->m_UserPageTable = pUserPageTable;
}
// ------------- END NOTE 3 -------------

void Machine::InitPageDirectory()
{
	PageDirectory *pPageDirectory = (PageDirectory *)(PAGE_DIRECTORY_BASE_ADDRESS + KERNEL_SPACE_START_ADDRESS);
	unsigned int *pDirectory = (unsigned int *)pPageDirectory;
	unsigned int *pKernelPageTable = (unsigned int *)(KERNEL_PAGE_TABLE_BASE_ADDRESS + KERNEL_SPACE_START_ADDRESS);
	unsigned int kPageTableIdx = KERNEL_SPACE_START_ADDRESS / PageTable::SIZE_PER_PAGETABLE_MAP;

	for (unsigned int i = 0; i < PageTable::ENTRY_CNT_PER_PAGETABLE; i++)
	{
		pDirectory[i] = 0;
		pKernelPageTable[i] = (i << 12) | 0x003;
	}

	pDirectory[0] = KERNEL_PAGE_TABLE_BASE_ADDRESS | 0x007;
	pDirectory[kPageTableIdx] = KERNEL_PAGE_TABLE_BASE_ADDRESS | 0x003;

	this->m_PageDirectory = pPageDirectory;
	this->m_KernelPageTable = (PageTable *)pKernelPageTable;
}
void Machine::InitUserPageTable()
{
	PageDirectory *pPageDirectory = this->m_PageDirectory;
	PageTable *pUserPageTable =
		(PageTable *)(USER_PAGE_TABLE_BASE_ADDRESS + KERNEL_SPACE_START_ADDRESS);
	unsigned int idx = USER_PAGE_TABLE_BASE_ADDRESS >> 12;

	for (unsigned int j = 0; j < USER_PAGE_TABLE_CNT; j++, idx++)
	{
		pPageDirectory->m_Entrys[j].m_UserSupervisor = 1;
		pPageDirectory->m_Entrys[j].m_Present = 1;
		pPageDirectory->m_Entrys[j].m_ReadWriter = 1;
		/*
		 * 椤电洰褰曢」BaseAddress瀛楁涓褰曢〉琛ㄧ殑鐗╃悊璧峰鍦板潃锛岃€岄潪绾挎€у湴鍧€銆?
		 * 涔熷氨鏄锛屽垎椤垫満鍒朵腑缁忕敱椤电洰褰曢」BaseAddress瀛楁鎵句笅涓€绾ч〉琛ㄦ槸
		 * 鏍规嵁椤佃〃鐨勭墿鐞嗗湴鍧€鎵惧埌瀹冦€傚垎椤垫満鍒剁殑杩愪綔涓嶄緷璧栧垎椤垫満鍒剁殑鏈韩--瀵圭嚎鎬у湴鍧€鐨勮В鏋愩€?
		 */
		pPageDirectory->m_Entrys[j].m_PageTableBaseAddress = idx;

		for (unsigned int i = 0; i < PageTable::ENTRY_CNT_PER_PAGETABLE; i++)
		{
			pUserPageTable[j].m_Entrys[i].m_UserSupervisor = 1;
			pUserPageTable[j].m_Entrys[i].m_Present = 1;
			pUserPageTable[j].m_Entrys[i].m_ReadWriter = 1;
			pUserPageTable[j].m_Entrys[i].m_PageBaseAddress = 0x00000 + i + j * 1024;
		}
	}

	this->m_UserPageTable = pUserPageTable;
}

void Machine::InitTaskStateSegment()
{
	TaskStateSegment &tss = this->GetTaskStateSegment();
	tss.m_CR3 = 0x200000; /* Physical base address of page directory */
	tss.m_CS = Machine::USER_CODE_SEGMENT_SELECTOR;
	tss.m_DS = Machine::USER_DATA_SEGMENT_SELECTOR;
	tss.m_SS = Machine::USER_DATA_SEGMENT_SELECTOR;
	tss.m_ES = tss.m_FS = tss.m_GS = Machine::USER_DATA_SEGMENT_SELECTOR;

	tss.m_EBP = 0xC0400000;
	tss.m_ESP = 0xC0400000;
	tss.m_EIP = 0xC0000000; // runtime
	tss.m_EFLAGS = 0x200;	/* 浠呬粎enable IF 浣?*/
	tss.m_SS0 = Machine::KERNEL_DATA_SEGMENT_SELECTOR;
	tss.m_ESP0 = 0xC0400000; /* 鏍稿績鎬佸湴鍧€绌洪棿鏈熬浣滀负鏍堝簳 */

	/*
	 * 灏咷DT琛ㄧ殑绗?椤?Machine::TASK_STATE_SEGMENT_IDX)鎸囧悜TSS娈点€?
	 *
	 * 鐢变簬GDT琚娊璞℃垚SegmentDescriptor鏁扮粍锛屾墍浠ユ病鏈夊TSS娈?
	 * 鎻忚堪绗︾殑鎶借薄锛屽洜姝ら渶瑕佸皢TaskStateSegmentDescriptor寮鸿浆锛?
	 * 浠ヨ繘琛屽繀瑕佺殑璁剧疆銆?
	 */
	TaskStateSegmentDescriptor *p_TSSDescriptor =
		(TaskStateSegmentDescriptor *)(&(GetGDT().GetSegmentDescriptor(Machine::TASK_STATE_SEGMENT_IDX)));
	p_TSSDescriptor->SetSegmengLimit(0x68 - 1);
	p_TSSDescriptor->SetBaseAddress((unsigned long)&g_TaskStateSegment);
	p_TSSDescriptor->m_Granularity = 1;
	p_TSSDescriptor->m_Type = 0x9; // 绗笁浣嶄负busy浣嶏紝璁剧疆涓?
	p_TSSDescriptor->m_Present = 0x1;
	p_TSSDescriptor->m_Available = 0x1;
	p_TSSDescriptor->m_DescriptorPrivilegeLevel = 0x00;
}
void Machine::EnablePageProtection()
{
	/*
	 * pageDirBaseAddr鏄湪楂樹綅鍐呮牳绌洪棿鐨勭嚎鎬у湴鍧€锛岄渶瑕佽浆鎹负鐗╃悊鍦板潃銆?
	 * PhysicalAddress = LinearAddress - 0xC0000000
	 */
	// ------------- NOTE 3: 涓嶈闂唴鏍搁〉鐩綍鎸囬拡锛岀洿鎺ョ敤鍦板潃 -------------
	// unsigned int pageDirBaseAddr = (unsigned int)(&GetPageDirectory());
	// unsigned int pageDirPhyBaseAddr = pageDirBaseAddr - Machine::KERNEL_SPACE_START_ADDRESS;
	unsigned int pageDirPhyBaseAddr = (unsigned int)(PAGE_DIRECTORY_BASE_ADDRESS);
	// ------------- END NOTE 3 -------------

	/* 瀵勫瓨鍣–R3涓啓鍏ラ〉鐩綍璧峰鐗╃悊鍦板潃锛孋R0鐨凱G浣嶇疆1锛屽紑鍚垎椤垫満鍒?*/
	__asm__ __volatile__("	movl %0, %%cr3;		\
							movl %%cr0, %%eax;	\
							orl $0x80000000, %%eax;	\
							movl %%eax, %%cr0" : : "a"(pageDirPhyBaseAddr));
}

IDT &Machine::GetIDT()
{
	return *(this->m_IDT);
}

GDT &Machine::GetGDT()
{
	return *(this->m_GDT);
}

PageDirectory &Machine::GetPageDirectory()
{
	return *(this->m_PageDirectory);
}

PageTable &Machine::GetKernelPageTable()
{
	return *(this->m_KernelPageTable);
}

PageTable *Machine::GetUserPageTableArray()
{
	return this->m_UserPageTable;
}

TaskStateSegment &Machine::GetTaskStateSegment()
{
	return *(this->m_TaskStateSegment);
}
