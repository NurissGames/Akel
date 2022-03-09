// This file is a part of the Akel editor
// Authors : @kbz_8
// Created : 08/07/2021
// Updated : 09/03/2022

#ifndef __AK_EDITOR_CONSOLE__
#define __AK_EDITOR_CONSOLE__

#include <AkEpch.h>
#include <Panels/shell/shell.h>

class Console
{
	public:
		explicit Console(std::string name, std::shared_ptr<Ak::ELTM> eltm, size_t inputBufferSize = 256);
		void render(Ak::Maths::Vec2<int>& pos, Ak::Maths::Vec2<int>& size);
		~Console();

		Shell _sh;

	protected:
		std::string _input;

	private:
		void inputBar();
		void logPart();

		static int InputCallback(ImGuiInputTextCallbackData *data);

		std::string _name;
		bool _autoScroll;
		bool _coloredOutput;
		bool _scrollToBottom;
		size_t _inBufferSize = 0;
		float _WindowAlpha;

		std::shared_ptr<Ak::ELTM> _eltm;

		int _width, _height = 0;

		Ak::audioFile ee = 0;
};

#endif // __AK_EDITOR_CONSOLE__
