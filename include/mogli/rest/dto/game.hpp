#pragma once
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(Platform, v_int32, VALUE(PC, 0, "PLATFORM_PC"), VALUE(MAC, 1, "PLATFORM_MAC"), VALUE(Linux, 2, "PLATFORM_LINUX"),
	 VALUE(Other, 3, "PLATFORM_OTHER"));

ENUM(Player, v_int32, VALUE(Single, 0, "PLAYER_SINGLE"), VALUE(Multi, 1, "PLAYER_MULTI"));

ENUM(GameType, v_int32, VALUE(Action, 0, "GAMETYPE_ACTION"), VALUE(Multi, 1, "GAMETYPE_ADVENTURE"));

class GameDto : public oatpp::DTO {

	DTO_INIT(GameDto, DTO)

	DTO_FIELD(String, id);
	DTO_FIELD(String, title, "title");
	DTO_FIELD(String, releaseDate, "releaseDate");
	DTO_FIELD(String, version, "version");
	DTO_FIELD(List<Enum<Platform>::AsString>, platform, "platform");
	DTO_FIELD(List<Enum<Player>::AsString>, player, "player");
	DTO_FIELD(List<Enum<GameType>::AsString>, gametype, "gametype");
};

#include OATPP_CODEGEN_END(DTO)
