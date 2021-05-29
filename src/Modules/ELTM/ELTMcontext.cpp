// This file is a part of AtlasEngine
// CREATED : 12/05/2021
// UPDATED : 29/05/2021

#include <Modules/ELTM/eltm.h>

namespace AE
{
	ELTMcontext::ELTMcontext() : _stream()
	{
		_comments[0] = false;
		_comments[1] = false;
	}

	bool ELTMcontext::newContext(const char* file)
	{
		_file = file;
		_stream.tokenize(file);
		std::string import_file;
		std::string text;

		for(_line = 0; _line < _stream.getLineNumber(); _line++)
		{
			if(!_comments[0] && !_comments[1])
			{
				if(_stream.getToken(_line, 0).isKeyword())
				{
					switch(_stream.getToken(_line, 0).getReservedToken())
					{
						case kw_set:
						{
							if(_stream.getToken(_line, 1).isString())
							{
								if(_texts.count(_stream.getToken(_line, 1).getString()))
								{
									ELTMerrors error = already_declared_error(_stream.getToken(_line, 1).getString(), file, _line + 1);
									std::cout << red << error.what() << def << std::endl;
									_isError = true;
									return false;
								}
								if(!setID(true))
									return false;
							}
							else
							{				
								ELTMerrors error = syntax_error("ID name cannot be a keyword", file, _line + 1);
								std::cout << red << error.what() << def << std::endl;
								_isError = true;
								return false;
							}
							break;
						}
						case kw_import:
						{
							if(_stream.getToken(_line, 1).isString())
							{
								int currentLine = _line;
								if(!newContext(_stream.getToken(_line, 1).getString().c_str()))
								{
									_isError = true;
									return false;
								}
								_file = file;
								_line = currentLine;
							}
							else
							{
								ELTMerrors error = syntax_error("file name cannot be a keyword", file, _line + 1);
								std::cout << red << error.what() << def << std::endl;
								_isError = true;
								return false;
							}
							break;
						}
						case basic_comment: _comments[0] = true; break;
						case begin_long_comment: _comments[1] = true; _last_line_long_comment = _line; break;
						
						default: break;
					}
				}
				else 
				{
					Token::activateKw(false); 
					if(_stream.getToken(_line, 0).getString() == "___ELTM_TOKEN_COMMENT_BASIC_CODE___")
					{
						_comments[0] = true;
						Token::activateKw();
					}
					else if(!_texts.count(_stream.getToken(_line, 0).getString()))
					{
						Token::activateKw();
						ELTMerrors error = simple_error(std::string("undefined ID or keyword \"" + _stream.getToken(_line, 0).getString() + "\""), file, _line + 1);
						std::cout << red << error.what() << def << std::endl;
						_isError = true;
						return false;
					}
					if(!setID(false))
						return false;
				}
			}
			if(_comments[0])
				_comments[0] = false;
			if(_comments[1])
			{
				if(_stream.getToken(_line, 0).isKeyword())
				{
					if(_stream.getToken(_line, 0).getReservedToken() == end_long_comment)
						_comments[1] = false;
				}
			}

		}
		if(_comments[1])
		{
			ELTMwarning warning = no_end("long comment (/*)", file, _last_line_long_comment);
			std::cout << magenta << warning.what() << def << std::endl;
		}
		_isError = false;
		return true;
	}

	bool ELTMcontext::setID(bool isNewID)
	{
		std::string text;
		size_t found;
		bool long_text = false;
		int assignPos = 0;
		bool getText = false;

		if(isNewID)
			assignPos = 2;
		else
			assignPos = 1;
		int j = assignPos + 1;

		if(_stream.getToken(_line, assignPos).getReservedToken() == assign)
		{
			int currentLine = _line;
			Token::activateKw(false);
			
			while(_line < _stream.getLineNumber())
			{
				for(;j < _stream.getLineIndexNumber(_line); j++)
				{
					// Comment check
					if(_stream.getToken(_line, j).getString() == Token::mixable_keywords_token[basic_comment])
						_comments[0] = true;
					if(_stream.getToken(_line, j).getString() == Token::mixable_keywords_token[begin_long_comment])
						_comments[1] = true;

					if(!_comments[0] && !_comments[1])
					{
						if(_stream.getToken(_line, j).getString() == Token::mixable_keywords_token[kw_get] && _stream.getToken(_line, j+1).getString() == Token::mixable_keywords_token[begin_long_text])
							getText = true;

						// Long text begin check
						if(_stream.getToken(_line, j).getString() == Token::mixable_keywords_token[begin_long_text] && !getText)
						{
							if(j != assignPos + 1)
							{
								ELTMerrors error = syntax_error("long text key \"(\" needs to be at the begenning of a text", _file, _line + 1);
								std::cout << red << error.what() << def << std::endl;
								_isError = true;
								return false;
							}
							else
								long_text = true;
						}
						else if(_stream.getToken(_line, j).getString() == Token::mixable_keywords_token[end_long_text] && long_text) // Check for long text end
							long_text = false;
						else
						{
							if(getText)
							{
								if(_stream.getToken(_line, j + 3).getString() == Token::mixable_keywords_token[end_long_text])
								{
									if(_texts.count(_stream.getToken(_line, j + 2).getString()))
									{
										text = _texts[_stream.getToken(_line, j + 2).getString()];
										break;
									}

									ELTMerrors error = simple_error("\"get()\" : undefined ID", _file, _line + 1);
									std::cout << red << error.what() << def << std::endl;
									_isError = true;
									return false;
								}
								else
								{
									ELTMerrors error = simple_error("get ID begun without end", _file, _line + 1);
									std::cout << red << error.what() << def << std::endl;
									_isError = true;
									return false;
								}
							}
							else
							{
								// Get text needed
								text += _stream.getToken(_line, j).getString();
								text += " ";
							}
						}
					}

					if(_stream.getToken(_line, j).getString() == Token::mixable_keywords_token[end_long_comment] && _comments[1])
						_comments[1] = false;
				}

				if(!long_text)
					break;
				_line++;
				j = 0;
			}
			_texts[_stream.getToken(currentLine, assignPos - 1).getString()] = text;
			Token::activateKw();
			if(long_text)
			{
				ELTMwarning warning = no_end("long text (\"(\")", _file, currentLine + 1);
				std::cout << magenta << warning.what() << def << std::endl;
			}
		}
		else
		{
			ELTMerrors error = syntax_error("missing \"=\" after ID declaration", _file, _line + 1);
			std::cout << red << error.what() << def << std::endl;
			_isError = true;
			return false;
		}
		for(auto elem : _texts)
		{
			std::cout << elem.first << "	" << elem.second << std::endl;
		}
		return true;
	}
}

