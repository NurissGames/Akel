// This file is a part of Akel
// Authors : @kbz_8
// Created : 19/03/2024
// Updated : 19/03/2024

#ifndef __AK_CORE_CONFIGURATION_FILE__
#define __AK_CORE_CONFIGURATION_FILE__

#include <Core/PreCompiled.h>

namespace Ak
{
	class AK_CORE_API ConfigurationFile
	{
		public:
			ConfigurationFile(const std::filesystem::path& filepath);

			inline std::size_t GetSize() const { return  std::filesystem::file_size(m_filepath); }
			inline std::filesystem::path GetPath() const { return m_filepath; }
			inline std::filesystem::path GetDirectory() const { return m_filepath.parent_path(); }

			inline toml::table& GetToml() { return m_table; }

			void WriteToFile() const;

			~ConfigurationFile() = default;

		private:
			toml::table m_table;
			std::filesystem::path m_filepath;
	};
}

#endif
