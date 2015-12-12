//load_stage_enemy_list.cpp

#include "play_game.h"
#include <fstream>
#include <exception>
#include "enemy_order.h"
#include "enemy_order_parser.h"
#include "msgbox.h"

void MyGameProject::PlayGame::load_stage_enemy_list(int _stage_num)
{
	std::ifstream ifs;
	if (_stage_num == 0){ ifs.open("../../data/map/stage0.data"); }
	std::string raw_text;

	if (ifs)
	{
		std::string current_line;
		while(!ifs.eof())
		{
			std::getline(ifs,current_line);
			raw_text += current_line;
		}

		msgbox_ok(raw_text.c_str());

		AST::OrderList order_list;
		if
			(
			!qi::phrase_parse
			(
			raw_text.begin(),
			raw_text.end(),
			Parser::OrderList<decltype(raw_text.begin())>(),
			ascii::space,
			order_list
			)
			)
		{
			throw std::runtime_error("Invalid map data.");
		}

		Converter::Processor processor(code_list);
		processor.process(order_list);
	}
	else
	{
		throw std::runtime_error("No stage map file.");
	}
}
