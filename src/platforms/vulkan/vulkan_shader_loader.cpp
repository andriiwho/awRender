#include "vulkan_shader_loader.h"
#include "vulkan_shader_module.h"
#include "core/slang-string-util.h"

#include <fmt/printf.h>
#include <slang.h>

namespace aw::render
{
	std::unordered_map<std::string, core::RefPtr<IDeviceShaderModule>> VulkanShaderLoader::m_Shaders;
	std::unordered_map<std::string, Slang::ComPtr<slang::IModule>> VulkanShaderLoader::m_LoadedModules;

	thread_local slang::IGlobalSession* VulkanShaderLoader::m_GlobalSession = nullptr;
	VulkanSlangFilesystem* VulkanShaderLoader::m_SlangFilesystem = nullptr;

	class VulkanSlangFilesystem final : public ISlangFileSystemExt
	{
	public:
		SLANG_CLASS_GUID(0x9f4d80a3, 0x7c2b, 0x49d6, 0xa5, 0x8e, 0x61, 0xe4, 0xa2, 0xf0, 0x9d, 0x3c);

		virtual ~VulkanSlangFilesystem() = default;
		SLANG_IUNKNOWN_QUERY_INTERFACE;

		ISlangUnknown* getInterface(const Slang::Guid& guid)
		{
			return static_cast<ISlangFileSystem*>(this);
		}

		uint32_t addRef() override { return 1; }
		uint32_t release() override { return 1; }
		void* castAs(const SlangUUID& guid) override
		{
			if (guid == ISlangFileSystem::getTypeGuid() || guid == getTypeGuid())
			{
				return this;
			}

			return nullptr;
		}

		SlangResult loadFile(const char* path, ISlangBlob** outBlob) override
		{
			if (core::g_vfs->file_exists(path))
			{
				const core::RefPtr reader = core::g_vfs->open_file_for_reading(path);
				const std::string contents = reader->read_all_to_string();
				*outBlob = Slang::StringUtil::createStringBlob(contents.data()).detach();
				return SLANG_OK;
			}

			return SLANG_E_NOT_FOUND;
		}

		SlangResult getFileUniqueIdentity(const char* path, ISlangBlob** outUniqueIdentity) override
		{
			*outUniqueIdentity = Slang::StringUtil::createStringBlob(path).detach();
			return SLANG_OK;
		}

		SlangResult calcCombinedPath(SlangPathType fromPathType, const char* fromPath, const char* path, ISlangBlob** pathOut) override
		{
			std::string out_path = core::IVirtualFileSystem::get_virtual_parent_path(fromPath);
			out_path += path;
			*pathOut = Slang::StringUtil::createStringBlob(out_path.data()).detach();
			return SLANG_OK;
		}

		SlangResult getPathType(const char* path, SlangPathType* pathTypeOut) override
		{
			if (const std::string_view path_view(path); path_view.ends_with(".slang-module") || path_view.ends_with(".slang"))
			{
				*pathTypeOut = SLANG_PATH_TYPE_FILE;
			}
			else
			{
				*pathTypeOut = SLANG_PATH_TYPE_DIRECTORY;
			}

			return SLANG_OK;
		}

		SlangResult getPath(PathKind kind, const char* path, ISlangBlob** outPath) override
		{
			return SLANG_E_NOT_IMPLEMENTED;
		}

		void clearCache() override
		{
		}

		SlangResult enumeratePathContents(const char* path, FileSystemContentsCallBack callback, void* userData) override
		{
			return SLANG_E_NOT_IMPLEMENTED;
		}

		OSPathKind getOSPathKind() override
		{
			return OSPathKind::OperatingSystem;
		}
	};

	VulkanShaderLoader::VulkanShaderLoader()
	{
		init_slang();
	}

	IDeviceShaderModule* VulkanShaderLoader::compile_shader(const std::string_view shader_file_path, const std::string_view entry_point, const ShaderStage stage)
	{
		{
			std::lock_guard lock(m_LibraryMutex);
			if (const auto iter = m_Shaders.find(std::string(shader_file_path)); iter != m_Shaders.end())
			{
				fmt::println("Found shader '{}' with entry point '{}' in cache. Loading...", shader_file_path, entry_point);
				return iter->second.get();
			}
		}

		{
			fmt::println("Compiling shader '{}' with entry point '{}' ...", shader_file_path, entry_point);
			Slang::ComPtr<slang::IModule> module;
			if (const auto iter = m_LoadedModules.find(std::string(shader_file_path)); iter != m_LoadedModules.end())
			{
				module = iter->second;
			}
			else
			{
				Slang::ComPtr<slang::IBlob> diag_blob;
				module = m_Session->loadModule(shader_file_path.data(), diag_blob.writeRef());
				if (module)
				{
					std::lock_guard lock(m_LibraryMutex);
					m_LoadedModules[std::string(shader_file_path)] = module;
				}

				if (diag_blob)
				{
					fmt::print("{}\n", static_cast<const char*>(diag_blob->getBufferPointer()));
				}
			}

			if (!module)
			{
				throw std::runtime_error(fmt::format("Failed to load shader module: {}", shader_file_path));
			}

			Slang::ComPtr<slang::IEntryPoint> entry_point_obj;
			if (const SlangResult result = module->findEntryPointByName(entry_point.data(), entry_point_obj.writeRef());
				result != SLANG_OK)
			{
				throw std::runtime_error(fmt::format("Failed to find entry point: {} in shader module: {}", entry_point, shader_file_path));
			}

			slang::IComponentType* link_components[] = { module, entry_point_obj };
			Slang::ComPtr<slang::IComponentType> program{};
			if (const SlangResult result = m_Session->createCompositeComponentType(link_components, 2, program.writeRef());
				result != SLANG_OK)
			{
				throw std::runtime_error(fmt::format("Failed to create composite component type: {} in shader module: {}", entry_point, shader_file_path));
			}

			Slang::ComPtr<slang::IComponentType> linked_program{};
			Slang::ComPtr<ISlangBlob> diag_blob{};
			if (const SlangResult result = program->link(linked_program.writeRef(), diag_blob.writeRef());
				result != SLANG_OK)
			{
				throw std::runtime_error(fmt::format("Failed to link shader module: {}", shader_file_path));
			}

			if (diag_blob)
			{
				fmt::print("{}\n", static_cast<const char*>(diag_blob->getBufferPointer()));
			}

			constexpr SlangInt entry_point_id = 0;
			constexpr SlangInt target_id = 0;
			Slang::ComPtr<slang::IBlob> spirv_blob;
			if (const SlangResult result = linked_program->getEntryPointCode(entry_point_id, target_id, spirv_blob.writeRef(), diag_blob.writeRef()); result != SLANG_OK)
			{
				throw std::runtime_error(fmt::format("Failed to get entry point code: {} in shader module: {}", entry_point, shader_file_path));
			}

			{
				const std::span byte_code(const_cast<core::u32*>(static_cast<const core::u32*>(spirv_blob->getBufferPointer())), spirv_blob->getBufferSize() / sizeof(core::u32));
				std::lock_guard lock(m_LibraryMutex);
				const auto out_shader = aw_new VulkanShaderModule(byte_code, stage, entry_point.data());
				m_Shaders[std::string(shader_file_path)] = out_shader;
				return out_shader;
			}

			return nullptr;
		}
	}

	void VulkanShaderLoader::clear_cache()
	{
		m_Shaders.clear();
		m_LoadedModules.clear();
	}

	void VulkanShaderLoader::init_slang()
	{
		if (!m_GlobalSession)
		{
			if (const SlangResult result = slang_createGlobalSession(SLANG_API_VERSION, &m_GlobalSession);
				result != SLANG_OK)
			{
				throw std::runtime_error("Failed to create slang global session");
			}

			m_SlangFilesystem = new VulkanSlangFilesystem();
		}

		m_GlobalSession->checkCompileTargetSupport(SLANG_SPIRV);
		slang::TargetDesc spirv_target_desc{};
		spirv_target_desc.format = SLANG_SPIRV;
		spirv_target_desc.profile = m_GlobalSession->findProfile("spirv_1_3");

		std::vector<slang::CompilerOptionEntry> options = {
			{
				.name = slang::CompilerOptionName::Capability,
				.value = {
					.kind = slang::CompilerOptionValueKind::Int,
					.intValue0 = m_GlobalSession->findCapability("SPV_EXT_descriptor_indexing"),
				},
			},
			{
				.name = slang::CompilerOptionName::Capability,
				.value = {
					.kind = slang::CompilerOptionValueKind::Int,
					.intValue0 = m_GlobalSession->findCapability("spvRayTracingKHR"),
				},
			}
		};
		spirv_target_desc.compilerOptionEntries = options.data();
		spirv_target_desc.compilerOptionEntryCount = static_cast<core::u32>(options.size());

		std::vector<slang::PreprocessorMacroDesc> macros{
			{ "AW_VULKAN_BACKEND", "1" }
		};

		std::vector<const char*> search_paths{
			"shaders://"
		};

		const slang::SessionDesc session_desc{
			.targets = &spirv_target_desc,
			.targetCount = 1,
			.searchPaths = search_paths.data(),
			.searchPathCount = static_cast<core::u32>(search_paths.size()),
			.preprocessorMacros = macros.data(),
			.preprocessorMacroCount = static_cast<core::u32>(macros.size()),
			.fileSystem = m_SlangFilesystem,

		};
		if (const SlangResult result = m_GlobalSession->createSession(session_desc, m_Session.writeRef());
			result != SLANG_OK)
		{
			throw std::runtime_error("Failed to create slang session");
		}
	}
} // namespace aw::render