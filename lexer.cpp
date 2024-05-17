#include <cctype>
#include <iostream>
#include <string>
#include <vector>

namespace Lexer
{
	enum TokenType
	{
		IDENTIFIER, LITERAL, NUMBER
	};

	struct Token
	{
		TokenType type;
		std::string value;
	};

	enum State {OUT, IN_IDENTIFIER, IN_STR, IN_NUMBER, IN_ESC};

	class Scanner
	{
		private:
			State state;

		public:
			auto get_state()
			{
				return this->state;
			}

			Scanner()
			{
				this->state = OUT;
			}

			void feed(char c)
			{
				switch (this->state)
				{
					case OUT:
						if (c == '"')
							this->state = IN_STR;

						if (c == '_' || isalpha(c))
							this->state = IN_IDENTIFIER;

						if (isdigit(c))
							this->state = IN_NUMBER;

						break;

					case IN_IDENTIFIER:
						if(
								isspace(c) || 
								c == '\n' || 
								!isalnum(c)
							) 
							this->state = OUT;

						break;

					case IN_STR:
						if (c == '"' || c == '\n')
							this->state = OUT;

						if (c == '\\')
							this->state = IN_ESC;

						break;
					case IN_ESC:
						if (c == '\"')
							this->state = IN_STR;

						break;
					case IN_NUMBER:
						if (!isdigit(c))
							this->state = OUT;
						break;
				}
			}
	};

	class Lexer 
	{
		int line;
		std::string source;
		std::vector<Token> tokens;

		public:
			Lexer(std::string& source)
			{
				this->line = 1;
				this->source = source;
				this->tokens = std::vector<Token>();
			}

			void scan()
			{
				auto scanner = new Scanner();


				std::string value; 
				for (auto it = source.begin(); it != source.end(); ++it)
				{
					scanner->feed(*it);
					State last_state;

					switch (scanner->get_state())
					{
						case OUT:
							if (last_state == OUT)
								continue;

							if (last_state == IN_IDENTIFIER)
								this->tokens.push_back(Token{ IDENTIFIER, value });

							if (last_state == IN_STR)
								this->tokens.push_back(Token{ LITERAL, value });

							last_state = OUT;
							value = "";
							break;

						case IN_IDENTIFIER:
							value += *it;
							last_state = IN_IDENTIFIER;
							break;
						
						case IN_STR:
							if (last_state == OUT)
								++it;

							value += *it;
							last_state = IN_STR;
							break;

						case IN_NUMBER:
							value += *it;
							last_state = IN_NUMBER;
							break;

						case IN_ESC:
							last_state = IN_ESC;
							continue;
					}
				}
			}

			void print_tokens() {
				for(auto& token : tokens) {
					std::cout << token.type << " " << token.value << std::endl;
				}
			}
	};
}
