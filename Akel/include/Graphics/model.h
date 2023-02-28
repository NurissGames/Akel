// This file is a part of Akel
// Authors : @kbz_8
// Created : 12/02/2023
// Updated : 28/02/2023

#ifndef __AK_MODEL__
#define __AK_MODEL__

#include <Akpch.h>
#include <Graphics/mesh.h>
#include <Graphics/material_library.h>

namespace Ak
{
	class AK_API Model
	{
		friend class ModelAttribute;

		public:
			Model() = default;
			Model(std::filesystem::path file);
			Model(const Mesh& mesh);
			Model(Mesh&& mesh);

			inline void setMaterial(MaterialID material) noexcept { _material = material; }
			inline MaterialID getMaterial() const noexcept { return _material; }

			inline const std::shared_ptr<Mesh>& getMesh() { return _mesh; }

			~Model() = default;

		private:
			void load();
			void loadOBJ();
			void loadGLTF();

			std::shared_ptr<Mesh> _mesh;
			std::filesystem::path _file;
			MaterialID _material = nullmaterial;
	};
}

#endif
