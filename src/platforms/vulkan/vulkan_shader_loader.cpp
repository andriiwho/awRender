#include "vulkan_shader_loader.h"
#include "vulkan_shader_module.h"

#include <fmt/printf.h>

#include <shaderc/shaderc.hpp>

namespace aw::render
{
	std::unordered_map<std::string, core::RefPtr<IDeviceShaderModule>> VulkanShaderLoader::m_Shaders;

	shaderc_shader_kind to_shader_kind(const ShaderStage stage)
	{
		switch (stage)
		{
			case ShaderStage::vertex:
				return shaderc_glsl_vertex_shader;
			case ShaderStage::fragment:
				return shaderc_glsl_fragment_shader;
			case ShaderStage::compute:
				return shaderc_glsl_compute_shader;
			default:
				break;
		}

		throw std::runtime_error("Unknown shader stage");
	}

	class ShaderCIncludeHandler final : public shaderc::CompileOptions::IncluderInterface
	{
	public:
		shaderc_include_result* GetInclude(const char* requested_source, shaderc_include_type type, const char* requesting_source, size_t include_depth) override
		{
			// Find a mapped parent directory
			const std::string_view path(requesting_source);
			if (const auto separator_pos = path.find("://"); separator_pos != std::string_view::npos)
			{
				for (const std::string_view shader_mapped_dir = path.substr(0, separator_pos + std::string_view("://").length());
					const auto& file : core::g_vfs->list_files_in_mapped_directory(shader_mapped_dir))
				{
					const std::string mapped_file_path = file;
					if (core::g_vfs->file_exists(mapped_file_path))
					{
						const core::RefPtr file_reader = core::g_vfs->open_file_for_reading(mapped_file_path);
						const std::string file_contents = file_reader->read_all_to_string();
						const auto result = aw_new shaderc_include_result();
						const auto copied_content = new char[file_contents.size() + 1];
						memcpy(copied_content, file_contents.data(), file_contents.size());
						copied_content[file_contents.size()] = '\0';
						result->content = copied_content;
						result->content_length = file_contents.size();

						const auto copied_source_name = new char[file_reader->get_path().size() + 1];
						memcpy(copied_source_name, file_reader->get_path().data(), file_reader->get_path().size());
						copied_source_name[file_reader->get_path().size()] = '\0';
						result->source_name = copied_source_name;
						result->source_name_length = file_reader->get_path().size();
						result->user_data = nullptr;
						m_Mappings[std::string(file_reader->get_path())] = result;
						return result;
					}

					const auto error_message = new std::string(fmt::format("Failed to find file for include '{}'", mapped_file_path));
					const auto result = aw_new shaderc_include_result();
					result->content = error_message->c_str();
					result->content_length = error_message->size();
					result->user_data = error_message;
					return result;
				}
			}

			return nullptr;
		}

		void ReleaseInclude(shaderc_include_result* data) override
		{
			for (auto& [mapping, result] : m_Mappings)
			{
				if (result == data)
				{
					m_Mappings.erase(mapping);
					break;
				}
			}

			delete[] data->content;
			delete[] data->source_name;

			if (data->user_data)
			{
				delete static_cast<std::string*>(data->user_data);
			}

			aw_delete(data);
		}

	private:
		std::unordered_map<std::string, shaderc_include_result*> m_Mappings;
	};

	IDeviceShaderModule* VulkanShaderLoader::compile_shader(core::IFileReader* file_reader, const std::string_view entry_point, const ShaderStage stage)
	{
		{
			std::lock_guard lock(m_LibraryMutex);
			if (const auto iter = m_Shaders.find(std::string(file_reader->get_path())); iter != m_Shaders.end())
			{
				return iter->second.get();
			}
		}

		{
			const shaderc::Compiler compiler;
			shaderc::CompileOptions options;

			auto include_handler = std::make_unique<ShaderCIncludeHandler>();
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_1);
			options.SetSourceLanguage(shaderc_source_language_glsl);
			options.SetTargetSpirv(shaderc_spirv_version_1_3);
			options.SetIncluder(std::move(include_handler));

			const std::string shader_source = file_reader->read_all_to_string();
			const auto result = compiler.CompileGlslToSpv(shader_source.c_str(), shader_source.size(), to_shader_kind(stage), file_reader->get_path().data(), "main", options);
			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				throw std::runtime_error(fmt::format("Failed to compile shader '{}': {}", file_reader->get_path(), result.GetErrorMessage()));
			}

			fmt::println("Compilation of shader '{}' finished with {} warnings", file_reader->get_path(), result.GetNumWarnings());
			if (result.GetNumWarnings() > 0)
			{
				fmt::println("'{}': {}", file_reader->get_path(), result.GetErrorMessage());
			}

			std::vector<core::u32> spirv_binary(result.cbegin(), result.cend());
			const core::RefPtr out_module = aw_new VulkanShaderModule(
				spirv_binary,
				stage,
				entry_point.data());

			// Add shader to the library and return
			std::lock_guard lock(m_LibraryMutex);
			m_Shaders[std::string(file_reader->get_path())] = out_module;
			return out_module.get();
		}
	}
} // namespace aw::render