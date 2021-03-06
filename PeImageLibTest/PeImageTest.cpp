#include "stdafx.h"
#include <string>
#include "CppUnitTest.h"
#include <PeImage\PeDecoder.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PeDecoder;
using std::ostringstream;
using std::endl;
namespace PeImageLibTest
{
	TEST_CLASS(PeImageTest)
	{
	public:
		PeImageTest() :
			pe_(GetModuleHandle(L"user32.dll"), true)
		{
			wchar_t str[MAX_PATH];
			GetModuleFileName(GetModuleHandle(L"user32.dll"), str, sizeof(str));
			Logger::WriteMessage(str);
			Assert::IsTrue(pe_.IsPe());

		}
		TEST_METHOD(TestDosHeader)
		{
			auto& dosHeader = pe_.GetDosHeader();
			auto ptr = dosHeader->GetPtr();
			auto& dosStub = dosHeader->GetDosStub();
			DWORD size = 0;
			if (dosStub)
			{
				size = dosStub->GetSize();
			}
		}
		TEST_METHOD(TestNtHeader)
		{
			auto& nt = pe_.GetNtHeader();
			auto& de = nt->GetDataDirectoryEntries();
			ostringstream out;
			for each (auto& var in *de)
			{
				out << "ADDR:" << var.VirtualAddress << " SIZE:" << var.Size << endl;
			}
			Logger::WriteMessage(out.str().c_str());
		}
		TEST_METHOD(TestSections)
		{
			auto& sections = pe_.GetSections();
			ostringstream out;
			for each (auto& var in *sections)
			{
				out << "SECTION: " << std::string((char*)&var.Name) << endl;
			}
			Logger::WriteMessage(out.str().c_str());

		}
		TEST_METHOD(TestExportDirectory)
		{
			auto& exportDir = pe_.GetExportDirectory();

			if (exportDir)
			{
				ostringstream out;
				out << "Export:" << endl << "----------" << endl;
				for each (auto& var in *exportDir)
				{
					auto name = (char*)pe_.RvaToDataPtr(*var.NameRva());
					out << name << endl;
				}
				Logger::WriteMessage(out.str().c_str());
			}
			else
			{
				Logger::WriteMessage("Export Not Exist");
			}
		}
		TEST_METHOD(TestImportDirectory)
		{
			auto& importDir = pe_.GetImportDirectory();
			auto type = importDir->GetPe().GetImageType();
			
			ostringstream out;
			for each (auto& var in *importDir)
			{
				out << endl << "# " << var.GetName() << endl;
				out << "----------" << endl;
				if (type == ImageType::PE32)
				{
					for each (auto& val in var.GetThunk32())
					{
						if (!val.IsSnapByOrdinal())
						{
							auto name = val.GetImportByName();
							out << (PCHAR)name->Name << " - "
								<< val.GetFuncAddress() << endl;
						}
						else
						{
							out << val.GetOrdinal() << " - " << val.GetFuncAddress() << endl;
						}

					}
				}
				else
				{
					for each (auto& val in var.GetThunk64())
					{
						if (!val.IsSnapByOrdinal())
						{
							auto name = val.GetImportByName();
							out << (PCHAR)name->Name << " - "
								<< val.GetFuncAddress() << endl;
						}
						else
						{
							out << val.GetOrdinal() << " - " << val.GetFuncAddress() << endl;
						}
					}
				}
				out << "----------" << endl;
			}

			Logger::WriteMessage(out.str().c_str());
		}

		TEST_METHOD(TestRelocDirectory)
		{
			auto& reloc = pe_.GetRelocDirectory();
			if (reloc)
			{
				ostringstream out;
				for each (auto& var in *reloc)
				{
					out << "Rva: " << (void*)var.GetPtr()->VirtualAddress << " Count: " << var.GetCount() << endl;
					for each (auto& node in var)
					{
						out << (void*)node.CurrentRelocRva() << " : " << node.CurrentType() << endl;
					}
				}
				Logger::WriteMessage(out.str().c_str());
			}

		}
		TEST_METHOD(TestResourceDirectory)
		{
			auto& res = pe_.GetResourceDirectory();
			if (res)
			{
				for each (auto& var in *res)
				{
					if (var.NameIsString())
					{
					}
				}
			}
		}
	private:
		PeImage pe_;

	};
}