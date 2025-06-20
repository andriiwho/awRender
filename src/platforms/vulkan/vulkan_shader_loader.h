#pragma once

#include "aw/render/system/shader_compiler.h"

#include <slang.h>
#include "slang-com-ptr.h"

#include <xhash>

namespace aw::render
{
	struct VulkanShaderName
	{
		std::string shader_file_path;
		std::string entry_point;
		ShaderStage stage;

		bool operator==(const VulkanShaderName& other) const noexcept
		{
			return shader_file_path == other.shader_file_path && entry_point == other.entry_point && stage == other.stage;
		}
	};
}

template<>
struct std::hash<aw::render::VulkanShaderName>
{
	std::size_t operator()(const aw::render::VulkanShaderName& name) const noexcept
	{
		using namespace aw::core;
		usize hash_code = 0;
		hash_code = hash_combine(hash_code, std::hash<std::string>{}(name.shader_file_path));
		hash_code = hash_combine(hash_code, std::hash<std::string>{}(name.entry_point));
		hash_code = hash_combine(hash_code, static_cast<usize>(name.stage));
		return hash_code;
	}
};

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

		void reflect_shader(const Slang::ComPtr<slang::IComponentType>& linked_program, std::string_view entry_point) const;

		std::mutex m_LibraryMutex;
		static std::unordered_map<VulkanShaderName, core::RefPtr<IDeviceShaderModule>> m_Shaders;
		static std::unordered_map<std::string, Slang::ComPtr<slang::IModule>> m_LoadedModules;
 		static thread_local slang::IGlobalSession* m_GlobalSession;
		static VulkanSlangFilesystem* m_SlangFilesystem;
		Slang::ComPtr<slang::ISession> m_Session{};
	};
}