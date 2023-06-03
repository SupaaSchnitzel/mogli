#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)
// Taken and adapted from https://github.com/oatpp/example-postgresql
ENUM(Role, v_int32, VALUE(GUEST, 0, "ROLE_GUEST"), VALUE(USER, 1, "ROLE_USER"), VALUE(ADMIN, 2, "ROLE_ADMIN"));

class UserDto : public oatpp::DTO {

	DTO_INIT(UserDto, DTO)

	DTO_FIELD(String, id);
	DTO_FIELD(String, userName, "username");
	DTO_FIELD(Enum<Role>::AsString, role, "role");
};

#include OATPP_CODEGEN_END(DTO)