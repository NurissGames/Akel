// This file is a part of the Akel editor
// CREATED : 08/06/2021
// UPDATED : 15/08/2021

#ifndef __AK_EDITOR_APP__
#define __AK_EDITOR_APP__

#define AK_PROJECT_FILE_DIR std::string(Ak::Core::getMainDirPath() + "Editor")
#define AK_PROJECT_FILE_NAME std::string("editor")

#include <AkEpch.h>
#include <Akel_main.h>
#include <editorComponent.h>

class Editor : public Ak::Application
{
	public:
		Editor() : Ak::Application("Akel_Editor")
		{
			add_component(Ak::custom_malloc<EditorComponent>());
		}
		~Editor() = default;
};

Ak::Application* Akel_main()
{
	return Ak::custom_malloc<Editor>();
}

#endif // __AK_EDITOR_APP__
