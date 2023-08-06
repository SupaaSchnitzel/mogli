#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace mogli::rest::dto {

	class GameMedia : public oatpp::DTO {
		DTO_INIT(GameMedia, DTO)
		
		DTO_FIELD(String, logo);
		DTO_FIELD(String, icon);
		DTO_FIELD(String, banner);
		DTO_FIELD(String, boxart);
	};

	class GameInfo : public oatpp::DTO {
		DTO_INIT(GameInfo, DTO)

		DTO_FIELD(String, id);
		DTO_FIELD(String, title);
		DTO_FIELD(String, description);
		DTO_FIELD(Vector<String>, tags);
		DTO_FIELD(Object<GameMedia>, media);
	};
}

#include OATPP_CODEGEN_END(DTO)
