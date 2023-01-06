// This file is a part of Akel
// Authors : @kbz_8
// Created : 05/03/2022
// Updated : 06/01/2023

#include <Graphics/entity.h>
#include <Core/core.h>

#define R_MASK 0xFF000000
#define G_MASK 0x00FF0000
#define B_MASK 0x0000FF00
#define A_MASK 0x000000FF

namespace Ak
{
	Entity3D::Entity3D(Models _model, Maths::Vec3<float> _position, Maths::Vec3<float> _scale, Maths::Vec4<float> _color)
	{
	    model = _model;
		position = std::move(_position);
		scale = std::move(_scale);
		color = std::move(_color);
	}

	Entity3D::Entity3D(Models _model, Maths::Vec3<float> _position, Maths::Vec3<float> _scale, Colors _color)
    {
	    model = _model;
		position = std::move(_position);
		scale = std::move(_scale);
        uint32_t col_val = static_cast<uint32_t>(_color);
        color.SET(static_cast<float>((col_val & R_MASK) >> 24) / 255, static_cast<float>((col_val & G_MASK) >> 16) / 255, static_cast<float>((col_val & B_MASK) >> 8) / 255, static_cast<float>((col_val & A_MASK)) / 255);
    }

	void Entity3D::destroy() noexcept
	{
		_vbo.destroy();
		_ibo.destroy();
	}

	void Entity3D::initBuffers()
	{
        switch(model)
        {
			case Models::cube:
			{
				const std::vector<Vertex> vertexData = {
					{{position.X          , position.Y          , position.Z + scale.Z}, {1.0f,  0.0f,  0.0f}},
					{{position.X + scale.X, position.Y          , position.Z + scale.Z}, {0.0f,  1.0f,  0.0f}},
					{{position.X + scale.X, position.Y + scale.Y, position.Z + scale.Z}, {0.0f,  0.0f,  1.0f}},
					{{position.X          , position.Y + scale.Y, position.Z + scale.Z}, {1.0f,  0.0f,  1.0f}},
					{{position.X          , position.Y          , position.Z}          , {1.0f,  1.0f,  0.0f}},
					{{position.X + scale.X, position.Y          , position.Z}          , {0.0f,  1.0f,  1.0f}},
					{{position.X + scale.X, position.Y + scale.Y, position.Z}          , {1.0f,  1.0f,  1.0f}},
					{{position.X          , position.Y + scale.Y, position.Z}          , {0.5f,  0.5f,  0.0f}},
				};

				const std::vector<uint32_t> indexData = {
					0, 1, 2, 2, 3, 0,
					4, 5, 6, 6, 7, 4,
					3, 2, 6, 6, 7, 3,
					0, 1, 5, 5, 4, 0,
					0, 3, 7, 7, 4, 0,
					1, 2, 6, 6, 5, 1
				};

				_vbo.create(sizeof(Vertex) * vertexData.size(), vertexData.data());
				_ibo.create(sizeof(uint32_t) * indexData.size(), indexData.data());

				break;
			}

            default : Core::log::report(ERROR, "Entity 3D : bad model"); break;
        }
	}
}
