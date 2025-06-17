#pragma once

#include "aw/render/system/shader_compiler.h"

#include <slang.h>
#include "slang-com-ptr.h"

namespace aw::render
{
	class VulkanSlangFilesystem;
	class VulkanShaderLoader final : public IShaderLoader
	{
	public:
		VulkanShaderLoader();

		IDeviceShaderModule* compile_shader(std::string_view shader_file_path, std::string_view entry_point, ShaderStage stage) override;

		void clear_cache() override;

	private:
		friend VulkanSlangFilesystem;
		void init_slang();

		std::mutex m_LibraryMutex;
		static std::unordered_map<std::string, core::RefPtr<IDeviceShaderModule>> m_Shaders;
		static std::unordered_map<std::string, Slang::ComPtr<slang::IModule>> m_LoadedModules;
 		static thread_local slang::IGlobalSession* m_GlobalSession;
		static VulkanSlangFilesystem* m_SlangFilesystem;
		Slang::ComPtr<slang::ISession> m_Session{};
	};
}