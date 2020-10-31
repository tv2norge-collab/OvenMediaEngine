//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2020 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <base/ovlibrary/ovlibrary.h>

#define CONVERTER_RETURN_IF(condition)   \
	if (condition)                       \
	{                                    \
		if (optional == Optional::False) \
		{                                \
			OV_ASSERT2(#condition);      \
		}                                \
                                         \
		return;                          \
	}                                    \
	[[maybe_unused]] Json::Value &object = parent_object[key];

namespace api
{
	namespace conv
	{
		enum class Optional
		{
			True,
			False
		};

		inline void SetString(Json::Value &parent_object, const char *key, const ov::String &value, Optional optional)
		{
			CONVERTER_RETURN_IF(value.IsEmpty());

			object = value.CStr();
		}

		inline void SetBool(Json::Value &parent_object, const char *key, bool value)
		{
			parent_object[key] = value;
		}
	}  // namespace conv
}  // namespace api