module;
#define WIN32_LEAN_AND_MEAN
#define NextSection 1
#include <Windows.h>
#include <Psapi.h>
#include <expected>
export module patternScan;



export class c_patternScan {
public:
	enum class errors {
		error_module_get = 0,
		error_pe_file = 1,
		error_x64_machine = 2,
		error_address_not_found = 3,
	};


private:
	c_patternScan() = default;
	c_patternScan(const c_patternScan&) = delete;
	c_patternScan(const c_patternScan&&) = delete;
	c_patternScan& operator=(c_patternScan&) = delete;
	c_patternScan& operator=(c_patternScan&&) = delete;

public:
	static auto get() -> c_patternScan* {
		static c_patternScan init;
		return &init;
	}

public:
	auto StartScan(HMODULE hModule, const char* offset, const char* mask) -> std::expected<uintptr_t, errors> {
		using enum errors;
		MODULEINFO md{};
		if (!GetModuleInformation(GetCurrentProcess(), hModule, &md, sizeof(md))) [[unlikely]] return std::unexpected(error_module_get);
		uintptr_t start_module = reinterpret_cast<uintptr_t>(md.lpBaseOfDll);
		DWORD size_module = md.SizeOfImage;
		uintptr_t end_module = start_module + static_cast<uintptr_t>(size_module);
		size_t mask_len = strlen(mask);

		PIMAGE_DOS_HEADER DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(reinterpret_cast<BYTE*>(start_module));
		if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE) [[unlikely]] return std::unexpected(error_pe_file);

		PIMAGE_NT_HEADERS NTheader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<BYTE*>(DosHeader) + DosHeader->e_lfanew);
		if (NTheader->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64) [[unlikely]] return std::unexpected(error_x64_machine);

		WORD SectionCount = NTheader->FileHeader.NumberOfSections;
		PIMAGE_SECTION_HEADER SectionTable = IMAGE_FIRST_SECTION(NTheader);

		for (UINT i = 0; i < SectionCount; ++i) {
			if (((SectionTable->Characteristics & IMAGE_SCN_CNT_CODE) && (SectionTable->Characteristics & IMAGE_SCN_MEM_EXECUTE))) [[likely]] {
				DWORD SectionCode = SectionTable->VirtualAddress;
				DWORD SectionSize = SectionTable->Misc.VirtualSize;

				for (BYTE* StartCodeSection = (reinterpret_cast<BYTE*>(start_module) + SectionCode); StartCodeSection <= reinterpret_cast<BYTE*>((start_module + SectionCode) + SectionSize - mask_len); ++StartCodeSection) {
					bool found_address = true;
					for (SIZE_T byte_check = 0; byte_check < mask_len; ++byte_check) {

						if (mask[byte_check] == '?') {
							continue;
						}
						if (StartCodeSection[byte_check] != static_cast<BYTE>(offset[byte_check])) {
							found_address = false;
							break;
						}
					}
					if (found_address) [[likely]] {
						return reinterpret_cast<uintptr_t>(StartCodeSection);
					}
				}
			}
				
			SectionTable += NextSection;
		}
		return std::unexpected(error_address_not_found);
	}
};