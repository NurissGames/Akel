// This file is a part of Akel
// Authors : @kbz_8
// Created : 08/06/2021
// Updated : 25/12/2021

#ifndef __AK_APPLICATION__
#define __AK_APPLICATION__

#include <Akpch.h>
#include <Platform/input.h>
#include <Core/Components/components.h>
#include <Utils/fps.h>

namespace Ak
{
	class Application : public ComponentStack
	{
		public:
			Application(const char* name = "application");
			void run();
			void end();
			~Application();

		private:
			const char* _name = nullptr;
			Input _in;
			CounterFPS _fps;
	};
}

#endif // __AK_APPLICATION__
